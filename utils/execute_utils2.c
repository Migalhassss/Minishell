/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:10:36 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:10:36 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*take_command_to_check(char *args)
{
	char	*cmd;
	int		i;

	i = 0;
	if (args[i] == '\0')
		return (ft_strdup(""));
	while (args[i] != ' ' && args[i] != '\0')
		i++;
	cmd = strndup(args, i);
	return (cmd);
}

char	*take_command(t_utils_hold *utils_hold)
{
	char	*cmd;
	char	*args;
	int		i;

	i = 0;
	if (utils_hold->args[i] == '\0')
		return (ft_strdup(""));
	while (utils_hold->args[i] != ' ' && utils_hold->args[i] != '\0')
		i++;
	cmd = strndup(utils_hold->args, i);
	while (utils_hold->args[i] == ' ' && utils_hold->args[i] != '\0')
		i++;
	args = ft_strdup(utils_hold->args + i);
	free(utils_hold->args);
	utils_hold->args = args;
	return (cmd);
}

int	check_builtins(t_utils_hold *utils_hold)
{
	int			return_value;
	char		*cmd;

	return_value = 0;
	cmd = take_command_to_check(utils_hold->args);
	if (!ft_strcmp(cmd, "cd"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "echo"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "env"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "exit"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "export"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "pwd"))
		return_value = 1;
	else if (!ft_strcmp(cmd, "unset"))
		return_value = 1;
	free(cmd);
	if (return_value == 1)
		return (1);
	return (0);
}
