/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 01:52:51 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 01:52:51 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_max_long(char	*arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i = 1;
	while (arg[i] == '0')
		i++;
	if (ft_strlen(arg) < 19)
		return (1);
	if (ft_strlen(arg) > 19)
		return (0);
	if (arg[0] == '-')
	{
		if (ft_strncmp(arg, "9223372036854775808", ft_strlen(arg)) > 0)
			return (0);
	}
	else if (ft_strncmp(arg, "9223372036854775807", ft_strlen(arg)) > 0)
		return (0);
	return (1);
}

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	free_utils_hold(t_utils_hold *utils_hold)
{
	ft_simple_cmdsclear(&utils_hold->simple_cmds);
	if (utils_hold->cmd)
		free(utils_hold->cmd);
	free(utils_hold->args);
	free(utils_hold->pwd);
	free(utils_hold->old_pwd);
	free_array(utils_hold->envp);
	free_array(utils_hold->paths);
	rl_clear_history();
	if (utils_hold->pipes)
		free(utils_hold->pid);
}

int	ft_exit(t_utils_hold *utils_hold)
{
	int	exit_code;

	exit_code = 0;
	if (check_max_long(utils_hold->args) == 0)
		printf("minishell: exit: %s: numeric argument required\n", utils_hold->args);
	else
		exit_code = ft_atoi(utils_hold->args);
	free_utils_hold(utils_hold);
	rl_clear_history();
	printf("Exiting minishell...\n");
	if (exit_code < 0 || exit_code > 255)
		exit_code = 255;
	exit(exit_code);
	return (1);
}
