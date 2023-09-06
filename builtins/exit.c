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
	free_utils_hold(utils_hold);
	rl_clear_history();
	printf("%d", g_global.exit_code);
	printf("Exiting minishell...\n");
	exit(0);
	return (1);
}
