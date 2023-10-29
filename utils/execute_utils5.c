/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:49:03 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/29 22:02:40 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tmp(t_utils_hold *utils_tmp)
{
	t_lexer	*current;
	t_lexer	*next;

	current = utils_tmp->lexer_list;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	free(utils_tmp->args);
}

int	ft_envplen(char	*envp)
{
	int	i;

	i = 0;
	while (envp[i] != '\0' && envp[i] != '=')
		i++;
	return (i);
}

int	ft_envpcmp(char *envp, char *env_name)
{
	int	i;

	i = 0;
	while (envp[i] != '\0' && envp[i] != '=')
	{
		if (envp[i] != env_name[i])
			return (1);
		i++;
	}
	if (env_name[i] != '\0')
		return (1);
	return (0);
}

char	*get_env_value(char *env_name, t_utils_hold *utils_hold)
{
	int		i;

	i = 0;
	if (!env_name)
		return (NULL);
	if (env_name[0] == '$')
		return (ft_strdup("$"));
	if (env_name[0] == '?')
		return (ft_itoa(utils_hold->exit_code));
	while (utils_hold->envp[i])
	{
		if (!ft_envpcmp(utils_hold->envp[i], env_name))
			return (ft_strdup(utils_hold->envp[i] + ft_strlen(env_name) + 1));
		i++;
	}
	return (ft_strdup(""));
}

void	clean_exit(t_utils_hold *utils_hold, int exit_code)
{
	t_lexer	*next;
	t_lexer	*current;

	current = utils_hold->simple_cmds->redirections;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	free_lexer(utils_hold->lexer_list);
	ft_simple_cmdsclear(&utils_hold->simple_cmds);
	free(utils_hold->args);
	free(utils_hold->pwd);
	free(utils_hold->old_pwd);
	free_array(utils_hold->paths);
	free(utils_hold->simple_cmds);
	if (utils_hold->pipes)
		free(utils_hold->pid);
	exit(exit_code);
}
