/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:09:24 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:09:24 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fork(t_utils_hold *utils_hold, int end[2], int fd_in, t_simple_cmds *cmd)
{
	static int	i = 0;

	if (utils_hold->reset == true)
	{
		i = 0;
		utils_hold->reset = false;
	}
	utils_hold->pid[i] = fork();
	if (utils_hold->pid[i] < 0)
		ft_error(5, utils_hold);
	if (utils_hold->pid[i] == 0)
		dup_cmd(cmd, utils_hold, end, fd_in);
	i++;
	return (0);
}

int	pipe_wait(int *pid, int amount, t_utils_hold *utils_hold)
{
	int	i;
	int	status;

	i = 0;
	while (i < amount)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	if (WIFEXITED(status))
		utils_hold->exit_code = WEXITSTATUS(status);
	return (0);
}

t_simple_cmds	*ft_simple_cmdsfirst(t_simple_cmds *map)
{
	int	i;

	i = 0;
	if (!map)
		return (NULL);
	while (map->prev != NULL)
	{
		map = map->prev;
		i++;
	}
	return (map);
}

int	send_heredoc(t_utils_hold *utils_hold, t_simple_cmds *cmd)
{
	t_lexer	*start;
	int		sl;
	int		fd;
	start = cmd->redirections;
	sl = 0;
	while (cmd->redirections)
	{
		if (cmd->redirections->token == 5)
		{
			if (cmd->hd_file_name)
				free(cmd->hd_file_name);
			cmd->hd_file_name = generate_heredoc_filename();
			fd = open(cmd->hd_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
				ft_error(5, utils_hold);
			sl = ft_heredoc(utils_hold, cmd->redirections, cmd->hd_file_name);
			close(fd);
			if (sl)
				return (reset_utils_hold(utils_hold));
		}
		cmd->redirections = cmd->redirections->next;
	}
	cmd->redirections = start;
	return (0);
}

int	executor(t_utils_hold *utils_hold)
{
	int		end[2];
	int		fd_in;

	fd_in = STDIN_FILENO;
	while (utils_hold->simple_cmds)
	{
		if (utils_hold->simple_cmds->next)
			pipe(end);
		send_heredoc(utils_hold, utils_hold->simple_cmds);
		ft_fork(utils_hold, end, fd_in, utils_hold->simple_cmds);
		close(end[1]);
		if (utils_hold->simple_cmds->prev)
			close(fd_in);
		fd_in = check_fd_heredoc(utils_hold, end, utils_hold->simple_cmds);
		if (utils_hold->simple_cmds->next)
			utils_hold->simple_cmds = utils_hold->simple_cmds->next;
		else
			break ;
	}
	pipe_wait(utils_hold->pid, utils_hold->pipes, utils_hold);
	utils_hold->simple_cmds = ft_simple_cmdsfirst(utils_hold->simple_cmds);
	return (0);
}
