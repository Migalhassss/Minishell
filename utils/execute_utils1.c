#include "../includes/minishell.h"

int	cmd_not_found(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (-1);
}

char	*join_split_str(char **split_str, char *new_str)
{
	char	*tmp;
	char	*add_space;
	int		i;

	tmp = ft_strdup(split_str[0]);
	i = 1;
	while (split_str[i])
	{
		new_str = tmp;
		add_space = ft_strjoin(new_str, " ");
		free(new_str);
		tmp = ft_strjoin(add_space, split_str[i]);
		free(add_space);
		i++;
	}
	new_str = tmp;
	return (new_str);
}

char	**resplit_str(char **double_arr)
{
	char	**ret;
	char	*joined_str;

	joined_str = join_split_str(double_arr, NULL);
	free_array(double_arr);
	ret = ft_split(joined_str, ' ');
	free(joined_str);
	joined_str = join_split_str(ret, NULL);
	free_array(ret);
	ret = ft_split(joined_str, ' ');
	free(joined_str);
	return (ret);
}

int	find_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int		i;
	char	*mycmd;

	i = 0;
	cmd->str = resplit_str(cmd->str);
	if (!access(cmd->str[0], F_OK))
		execve(cmd->str[0], cmd->str, utils_hold->envp);
	while (utils_hold->paths[i])
	{
		mycmd = ft_strjoin(utils_hold->paths[i], cmd->str[0]);
		if (!access(mycmd, F_OK))
			execve(mycmd, cmd->str, utils_hold->envp);
		free(mycmd);
		i++;
	}
	cmd_not_found(cmd->str[0]);
	free_array(cmd->str);
	return (-1);
}

int	check_append_outfile(t_lexer *redirections, char *file)
{
	int	fd;

	if (redirections->token == 4)
		fd = open(file,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

int	handle_infile(char *file)
{
	int	fd;

	printf("file = %s\n", file);
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: infile: No such file or directory\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDIN_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

int	handle_outfile(t_lexer *redirection, char *file)
{
	int	fd;

	printf("file = %s\n", file);
	fd = check_append_outfile(redirection, file);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: outfile: Error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}


void	better_args(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '<' || utils_hold->args[i] == '>')
		{
			tmp = ft_substr(utils_hold->args, 0, i);
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			tmp = ft_strtrim(utils_hold->args, " ");
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			return ;
		}
		i++;
	}
	return ;
}

void	free_tmp(t_utils_hold *utils_tmp)
{
	int i = 0;

	while (utils_tmp->lexer_list->str)
	{
		i++;
		utils_tmp->lexer_list = utils_tmp->lexer_list->next;
	}
	free(utils_tmp->args);
	free(utils_tmp->lexer_list);
}

int	check_redirections(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	t_lexer	*start;
	t_lexer	*tmp;
	t_utils_hold	utils_tmp;

	utils_tmp.args = ft_strdup(utils_hold->args);
	token_reader(&utils_tmp);
	better_args(utils_hold);
	start = cmd->redirections;
	tmp = utils_tmp.lexer_list;
	while (cmd->redirections)
	{
		if (cmd->redirections->token == 3)
		{
			while (tmp->token != 3)
				tmp = tmp->next;
			if (handle_infile(tmp->next->str))
				return (1);
		}
		else if (cmd->redirections->token == 2
			|| cmd->redirections->token == 4)
		{
			while (tmp->token != 2
				&& tmp->token != 4)
				tmp = tmp->next;
			if (handle_outfile(cmd->redirections, tmp->next->str))
				return (1);
		}
		else if (cmd->redirections->token == 5)
		{
			while (tmp->token != 5)
				tmp = tmp->next;
			if (handle_infile(tmp->next->str))
				return (1);
		}
		cmd->redirections = cmd->redirections->next;
	}
	
	free_tmp(&utils_tmp);
	cmd->redirections = start;
	return (0);
}

char	*get_env_value(char *env_name, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], env_name, ft_strlen(env_name)))
			return (ft_strdup(envp[i] + ft_strlen(env_name) + 1));
		i++;
	}
	return (ft_strdup(""));
}

void	clean_exit(t_utils_hold *utils_hold, int exit_code)
{
	free(utils_hold->args);
	free(utils_hold->pwd);
	free(utils_hold->old_pwd);
	free_array(utils_hold->paths);
	rl_clear_history();
	free(utils_hold->simple_cmds);
	if (utils_hold->pipes)
		free(utils_hold->pid);
	exit(exit_code);
}

void	handle_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->redirections)
	{
		if (check_redirections(cmd, utils_hold))
		{
			clean_exit(utils_hold, exit_code);
			perror("error\n");
		}
	}
	if (cmd->str[0][0] != '\0' && check_builtins(utils_hold) == 1)
	{
		which_command(utils_hold);
		if (cmd->str)
			free_array(cmd->str);
	}
	else if (cmd->str[0][0] != '\0')
	{
		exit_code = find_cmd(cmd, utils_hold);
	}
	free_array(utils_hold->envp);
	clean_exit(utils_hold, exit_code);
}
