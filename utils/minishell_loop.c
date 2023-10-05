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
	char	*tmp;

	(void) utils_hold;
	tmp = ft_itoa(g_global.exit_code);
	return (tmp);
}

char	*take_var_name(char *args, int i) {
	char *var_name;
	int	j;

	j = 0;
	i++;
	var_name = malloc(ft_strlen(args) + 1);
	while (args[i] != '\0' && args[i] != ' ' && args[i] != '$'
		&& args[i] != '\'' && args[i] != '\"')
		var_name[j++] = args[i++];
	var_name[j] = '\0';
	return (var_name);
}

char	*empty_env(char *args, char *var_name, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = ft_substr(args, 0, i);
	tmp2 = ft_substr(args, i + ft_strlen(var_name) + 1, ft_strlen(args));
	tmp3 = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
	free(args);
	return (tmp3);
}

char	*update_args(char *args, char *var_value, char *var_name, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = ft_substr(args, 0, i);
	tmp2 = ft_strjoin(tmp, var_value);
	free(tmp);
	tmp = ft_substr(args, i + ft_strlen(var_name) + 1, ft_strlen(args));
	tmp3 = ft_strjoin(tmp2, tmp);
	free(tmp);
	free(tmp2);
	free(args);
	return (tmp3);
}

char	*detect_dollar_sigs(char *args, char *var_value, char *var_name, int *i)
{
	if (ft_strlen(var_value) == 0)
		args = empty_env(args, var_name, (*i));
	else
		args = update_args(args, var_value, var_name, (*i));
	(*i) = (*i) + ft_strlen(var_value);
	free(var_value);
	return (args);
}

char	*replace_env_vars(char *args, char **envp)
{
	char	*var_name;
	char	*var_value;
	int		i;
	bool	in_quotes;

	if (ft_strchr(args, '$') == NULL)
		return (args);
	i = 0;
	in_quotes = false;
	while (args[i] != '\0')
	{
		if (args[i] == '\"')
			in_quotes = !in_quotes;
		if (args[i] == '\'' && in_quotes == false)
		{
			i++;
			while (args[i] != '\'')
				i++;
		}
		if (args[i] == '$')
		{
			var_name = take_var_name(args, i);
			var_value = get_env_value(var_name, envp);
			args = detect_dollar_sigs(args, var_value, var_name, &i);
			free(var_name);
		}
		i++;
	}
	args[i] = '\0';
	return (args);
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
	utils_hold->args = replace_env_vars(utils_hold->args, utils_hold->envp);
	if (count_quotes(utils_hold) == 1)
		return (ft_error(2, utils_hold));
	if (ft_strlen(utils_hold->args) == 0)
		reset_utils_hold(utils_hold);
	if (token_reader(utils_hold) == 0)
		return (ft_error(1, utils_hold));
	parser(utils_hold);
	prepare_executor(utils_hold);
	reset_utils_hold(utils_hold);
	return (1);
}
