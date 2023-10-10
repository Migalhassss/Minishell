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

int	create_heredoc(t_lexer *heredoc, bool quotes,
	t_utils_hold *utils_hold, char *file_name)
{
	int		fd;
	char	*line;

	(void) quotes;
	(void) utils_hold;
	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline(">");
	while (line && ft_strncmp(heredoc->str, line, ft_strlen(heredoc->str)))
	{
		// printf("heredoc->str = %s\n", heredoc->str);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	free(line);
	if (!line)
		return (EXIT_FAILURE);
	close(fd);
	return (0);
}

int	ft_heredoc(t_utils_hold *utils_hold, t_lexer *heredoc, char *file_name)
{
	bool	quotes;
	int		sl;

	sl = 0;
	if ((heredoc->str[0] == '\"'
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\"')
		|| (heredoc->str[0] == '\''
			&& heredoc->str[ft_strlen(heredoc->str) - 1] == '\''))
		quotes = true;
	else
		quotes = false;
	delete_quotes(heredoc->str, '\"');
	delete_quotes(heredoc->str, '\'');
	sl = create_heredoc(heredoc, quotes, utils_hold, file_name);
	utils_hold->heredoc = true;
	return (sl);
}

char	*generate_heredoc_filename(void)
{
	static int	i = 0;
	char		*num;
	char		*file_name;

	num = ft_itoa(i++);
	file_name = ft_strjoin("build/.tmp_heredoc_file_", num);
	free(num);
	return (file_name);
}

void	dup_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold, int end[2], int fd_in)
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
