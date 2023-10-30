/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:52:21 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/25 17:52:21 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*only_dollar(char *args, char *var_name)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = ft_substr(args, 0, ft_strlen(var_name) + 1);
	tmp2 = ft_substr(args, ft_strlen(var_name) + 1, ft_strlen(args));
	tmp3 = ft_strjoin(tmp, tmp2);
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
	free(var_value);
	return (args);
}

int	check_if_onlydollar(char *args, int i)
{
	if (args[i + 1] != '\0' && args[i + 1] != ' '
		&& args[i + 1] != '\'' && args[i + 1] != '\"')
		return (1);
	if ((args[i + 1] == '\"' || args[i + 1] == '\'')
		&& args[i + 2] != '\0' && args[i + 2] != ' '
		&& args[i + 2] != '\'' && args[i + 2] != '\"')
		return (1);
	return (0);
}

void	free_lexer_list(t_utils_hold *utils_hold)
{
	t_lexer	*tmp;
	t_lexer	*lexertmp;

	lexertmp = utils_hold->simple_cmds->redirections;
	while (lexertmp)
	{
		tmp = lexertmp->next;
		if (lexertmp->str)
			free(lexertmp->str);
		free(lexertmp);
		lexertmp = tmp;
	}
	lexertmp = NULL;
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
	if (count_quotes(utils_hold) == 1)
		return (ft_error(2, utils_hold));
	utils_hold->args = replace_env_vars(utils_hold);
	if (token_reader(utils_hold) == 1)
		return (ft_error(1, utils_hold));
	parser(utils_hold);
	prepare_executor(utils_hold);
	free_lexer_list(utils_hold);
	reset_utils_hold(utils_hold);
	return (1);
}
