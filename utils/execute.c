/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:03:22 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:03:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_command(char *cmd, t_utils_hold *utils_hold)
{
	if (!ft_strcmp(cmd, "echo"))
		utils_hold->exit_code = ft_echo(utils_hold);
	else if (!ft_strcmp(cmd, "exit"))
		utils_hold->exit_code = ft_exit(utils_hold);
	else if (!ft_strcmp(cmd, "pwd"))
		utils_hold->exit_code = ft_pwd(utils_hold);
	else if (!ft_strcmp(cmd, "env"))
		utils_hold->exit_code = ft_env(utils_hold);
	else if (!ft_strcmp(cmd, "export"))
		utils_hold->exit_code = ft_export(utils_hold);
	else if (!ft_strcmp(cmd, "unset"))
		utils_hold->exit_code = ft_unset(utils_hold);
	else if (!ft_strcmp(cmd, "cd"))
		utils_hold->exit_code = ft_cd(utils_hold);
	return ;
}

char	*which_command(t_utils_hold *utils_hold)
{
	int			i;
	char		*cmd;
	static char	*builtins[7][1] = {{"echo"},
	{"cd"},
	{"pwd"},
	{"export"},
	{"unset"},
	{"env"},
	{"exit"}
	};

	i = 0;
	utils_hold->cmd = take_command(utils_hold);
	while (builtins[i][0])
	{
		if (!ft_strcmp(utils_hold->cmd, builtins[i][0]))
		{
			cmd = builtins[i][0];
			execute_command(cmd, utils_hold);
			break ;
		}
		i++;
	}
	free(utils_hold->cmd);
	return (NULL);
}

void	single_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int		pid;
	int		status;
	char	*exit_cmd;

	exit_cmd = take_command_to_check(utils_hold->args);
	status = 0;
	send_heredoc(utils_hold, cmd);
	if (!ft_strcmp(exit_cmd, "exit") || !ft_strcmp(exit_cmd, "cd")
		|| !ft_strcmp(exit_cmd, "export") || !ft_strcmp(exit_cmd, "unset"))
	{
		free(exit_cmd);
		which_command(utils_hold);
		return ;
	}
	free(exit_cmd);
	if (utils_hold->args[0] == '>' || utils_hold->args[0] == '<')
	{
		reset_utils_hold(utils_hold);
		return ;
	}
	pid = fork();
	if (pid < 0)
		ft_error(5, utils_hold);
	if (pid == 0)
		handle_cmd(cmd, utils_hold);
	waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			utils_hold->exit_code = WEXITSTATUS(status);
}

int	prepare_executor(t_utils_hold *utils_hold)
{
	g_global.in_cmd = 1;
	if (check_builtins(utils_hold) == 1 
		|| check_cmd(utils_hold->simple_cmds, utils_hold) == 0
		|| utils_hold->args[0] == '>' || utils_hold->args[0] == '<')
	{
		if (utils_hold->pipes == 0)
			single_cmd(utils_hold->simple_cmds, utils_hold);
		else
		{
			utils_hold->pid = ft_calloc(sizeof(int), utils_hold->pipes + 2);
			if (!utils_hold->pid)
				return (ft_error(1, utils_hold));
			executor(utils_hold);
		}
	}
	g_global.in_cmd = 0;
	return (0);
}
