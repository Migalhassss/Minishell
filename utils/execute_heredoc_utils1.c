/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:09:58 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:09:58 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_fd_heredoc(t_utils_hold *utils_hold, int end[2], t_simple_cmds *cmd)
{
	int	fd_in;

	if (utils_hold->heredoc)
	{
		close(end[0]);
		fd_in = open(cmd->hd_file_name, O_RDONLY);
	}
	else
		fd_in = end[0];
	return (fd_in);
}

int	create_heredoc(bool quotes,
	t_lexer *tmp, char *file_name)
{
	int		fd;
	char	*line;

	(void) quotes;
	while (tmp && tmp->token != 5)
		tmp = tmp->next;
	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline(">");
	while (line && ft_strncmp(tmp->next->str, line, ft_strlen(tmp->next->str)
		&& !g_global.stop_heredoc))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
		if (g_global.stop_heredoc)
			break;
	}
	free(line);
	if (g_global.stop_heredoc ||!line)
		return (EXIT_FAILURE);
	close(fd);
	return (0);
}

int	ft_heredoc(t_utils_hold *utils_hold, t_lexer *heredoc, char *file_name)
{
	bool			quotes;
	int				sl;
	t_utils_hold	tmp_utils;
	t_lexer			*tmp;

	sl = 0;
	if ((heredoc->str[0] == '\"'
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\"')
		|| (heredoc->str[0] == '\''
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\''))
		quotes = true;
	else
		quotes = false;
	tmp_utils.args = ft_strdup(utils_hold->args);
	token_reader(&tmp_utils);
	tmp = tmp_utils.lexer_list;
	g_global.in_heredoc = 1;
	sl = create_heredoc(quotes, tmp, file_name);
	g_global.in_heredoc = 0;
	utils_hold->heredoc = true;
	free_tmp(&tmp_utils);
	return (sl);
}

char	*generate_heredoc_filename(void)
{
	static int	i = 0;
	char		*num;
	char		*file_name;

	num = ft_itoa(i++);
	file_name = ft_strjoin("/tmp/.tmp_heredoc_file_", num);
	free(num);
	return (file_name);
}

void	dup_cmd(t_simple_cmds *cmd,
		t_utils_hold *utils_hold, int end[2], int fd_in)
{
	if (cmd->prev && dup2(fd_in, STDIN_FILENO) < 0)
		ft_error(4, utils_hold);
	close(end[0]);
	if (cmd->next && dup2(end[1], STDOUT_FILENO) < 0)
		ft_error(4, utils_hold);
	close(end[1]);
	if (cmd->prev)
		close(fd_in);
	handle_cmd(cmd, utils_hold);
}
