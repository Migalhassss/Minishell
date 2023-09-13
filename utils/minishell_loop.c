#include "../includes/minishell.h"

int	implement_utils_hold(t_utils_hold *utils_hold)
{
	utils_hold->simple_cmds = NULL;
	utils_hold->lexer_list = NULL;
	utils_hold->reset = false;
	utils_hold->pid = NULL;
	utils_hold->heredoc = false;
	utils_hold->pipes = 0;
	utils_hold->index_list = 0;
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
	g_global.in_heredoc = 0;
	path_find(utils_hold);
	// init_signals();
	return (1);
}

void	*ft_lstclear_cmd(t_lexer **lst)
{
	if ((*lst)->str)
	{
		free((*lst)->str);
		(*lst) = NULL;
	}
	free(*lst);
	*lst = NULL;
	return (NULL);
}

void	ft_simple_cmdsclear(t_simple_cmds **lst)
{
	t_simple_cmds	*tmp;
	// t_lexer			*redirections_tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		// redirections_tmp = (*lst)->redirections;
		// ft_lstclear_cmd(&redirections_tmp);
		if ((*lst)->str)
			free_array((*lst)->str);
		if ((*lst)->hd_file_name)
			free((*lst)->hd_file_name);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	ft_lstclear2(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstclear_cmd(lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	reset_utils_hold(t_utils_hold *utils_hold)
{
	ft_simple_cmdsclear(&utils_hold->simple_cmds);
	// ft_lstclear2(&utils_hold->lexer_list);
	free(utils_hold->args);
	if (utils_hold->pid)
		free(utils_hold->pid);
	free_array(utils_hold->paths);
	implement_utils_hold(utils_hold);
	utils_hold->reset = true;
	minishell_loop(utils_hold);
	return (1);
}

char	*make_leaks_clean(char *tmp2, char *str, char *exit_code, int flag)
{
	char	*tmp;
	char	*tmp3;

	tmp = strndup(str, flag);
	free(str);
	tmp3 = ft_strjoin(tmp, exit_code);
	free(tmp);
	tmp = ft_strjoin(tmp3, tmp2);
	free(tmp2);
	free(tmp3);
	return (tmp);
}

char	*exit_code(t_utils_hold *utils_hold)
{
	int	i;
	int	flag;
	char	*tmp;
	char	*tmp2;
	char	*exit_code;

	i = 0;
	flag = 0;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '\'')
		{
			i++;
			while (utils_hold->args[i] != '\'' && utils_hold->args[i])
				i++;
		}
		if (utils_hold->args[i] == '$' && utils_hold->args[i + 1] == '?')
		{
			flag = i;
			tmp2 = ft_strdup(utils_hold->args + i + 2);
			break ;
		}
		i++;
	}
	if (flag == 0)
		return (utils_hold->args);
	exit_code = ft_itoa(g_global.exit_code);
	tmp = make_leaks_clean(tmp2, utils_hold->args, exit_code, flag);
	free(exit_code);
	return (tmp);
}

char	*check_if_is_first(t_utils_hold *utils_hold)
{
	char	*tmp;
	char	*exit_code;

	exit_code = NULL;
	tmp = NULL;
	if (utils_hold->args[0] == '$' && utils_hold->args[1] == '?')
	{
		exit_code = ft_itoa(g_global.exit_code);
		tmp = ft_strjoin(exit_code, utils_hold->args + 2);
	}
	else
		return (utils_hold->args);
	free(utils_hold->args);
	free(exit_code);
	utils_hold->args = ft_strdup(tmp);
	free(tmp);
	return (utils_hold->args);
}

void	put_in_args(t_utils_hold *utils_hold, char *env_value, int i)
{
	char	*tmp2;
	char	*tmp3;


	tmp2 = ft_substr(env_value, 0, ft_strlen(env_value));
	free(env_value);
	tmp3 = ft_strjoin(tmp2, utils_hold->args + i);
	free(tmp2);
	free(utils_hold->args);
	utils_hold->args = tmp3;
}

void	transfer2(t_utils_hold *utils_hold, int *i)
{
	int		j;
	char	*tmp;
	char	*env_value;

	if (utils_hold->args[*i + 1] == '?')
	{
		tmp = ft_itoa(g_global.exit_code);
		put_in_args(utils_hold, tmp, *i);
		free(tmp);
		return ;
	}
	j = *i + 1;
	while (utils_hold->args[*i] && utils_hold->args[*i] != ' '
		&& utils_hold->args[*i] != '\'' && utils_hold->args[*i] != '"')
		(*i)++;
	tmp = ft_substr(utils_hold->args, j, *i - 1);
	env_value = get_env_value(tmp, utils_hold->envp);
	free(tmp);
	put_in_args(utils_hold, env_value, *i);
}

void	clean_args(t_utils_hold *utils_hold, char *cmd, int flag)
{
	char	*tmp;
	char	*tmp2;

	if (flag == 1)
	{
		tmp = ft_strjoin(cmd, " ");
		free(cmd);
		tmp2 = ft_strjoin(tmp, utils_hold->args);
		free(utils_hold->args);
		free(tmp);
		utils_hold->args = tmp2;
	}
	else
		free(cmd);
}

void	 check_user2(t_utils_hold *utils_hold, int *i)
{	
	if (!ft_strncmp(utils_hold->args + *i, "'$USER'", 7))
		put_in_args(utils_hold, get_env_value(utils_hold->args + *i, utils_hold->envp), *i);
	else if (!ft_strncmp(utils_hold->args + *i, "\'\"$USER\"\'", 9))
	{
		put_in_args(utils_hold, "\"$USER\"", *i);
		*i += 8;
	}
}

void	envp_value(t_utils_hold *utils_hold)
{
	int		i;
	int		flag;
	char	*cmd;

	i = 0;
	flag = 0;
	cmd = take_command_to_check(utils_hold->args);
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '\'')
		{
			i++;
			while (utils_hold->args[i] != '\'' && utils_hold->args[i])
				i++;
		}
		if (utils_hold->args[i] == '$')
		{
			flag = 1;
			transfer2(utils_hold, &i);
		}
		if (utils_hold->args[i] == '"')
			check_user2(utils_hold, &i);
		i++;
	}
	clean_args(utils_hold, cmd, flag);
}

int	minishell_loop(t_utils_hold *utils_hold)
{
	char	*tmp;

	utils_hold->args = readline("minishell> ");
	tmp = ft_strtrim(utils_hold->args, " ");
	free(utils_hold->args);
	utils_hold->args = tmp;
	if (!utils_hold->args)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(0);
	}
	if (ft_strlen(utils_hold->args) == 0)
		reset_utils_hold(utils_hold);
	add_history(utils_hold->args);
	if (count_quotes(utils_hold->args) == 1)
		return (ft_error(2, utils_hold));
	envp_value(utils_hold);
	if (token_reader(utils_hold) == 0)
		return (ft_error(1, utils_hold));
	parser(utils_hold);
	prepare_executor(utils_hold);
	reset_utils_hold(utils_hold);
	return (1);
}
