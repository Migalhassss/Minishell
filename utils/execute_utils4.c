/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:22:26 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/29 21:29:56 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: infile: No such file or directory\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDIN_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

int	handle_outfile(t_lexer *redirection, char *file)
{
	int	fd;

	fd = check_append_outfile(redirection, file);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: outfile: Error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

void	better_args(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '<' || utils_hold->args[i] == '>')
		{
			tmp = ft_substr(utils_hold->args, 0, i);
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			tmp = ft_strtrim(utils_hold->args, " ");
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			return ;
		}
		i++;
	}
	return ;
}

int	check_redirections_utils(t_simple_cmds *cmd, t_lexer *tmp)
{
	while (cmd->redirections)
	{
		if (cmd->redirections->token == 3)
		{
			while (tmp && tmp->token != 3)
				tmp = tmp->next;
			if (handle_infile(tmp->next->str))
				return (1);
			tmp = tmp->next;
		}
		else if (cmd->redirections->token == 2
			|| cmd->redirections->token == 4)
		{
			while (tmp && tmp->token != 2
				&& tmp->token != 4)
				tmp = tmp->next;
			if (handle_outfile(cmd->redirections, tmp->next->str))
				return (1);
			tmp = tmp->next;
		}
		if (cmd->redirections->token == 5)
		{
			if (handle_infile(cmd->hd_file_name))
				return (1);
		}
		cmd->redirections = cmd->redirections->next;
	}
	return (0);
}

int	check_redirections(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	t_lexer			*start;
	t_lexer			*tmp;
	t_utils_hold	utils_tmp;

	utils_tmp.args = ft_strdup(utils_hold->args);
	token_reader(&utils_tmp);
	better_args(utils_hold);
	start = cmd->redirections;
	tmp = utils_tmp.lexer_list;
	while (cmd->redirections)
	{
		if (check_redirections_utils(cmd, tmp) == 1)
			return (1);
	}
	free_tmp(&utils_tmp);
	cmd->redirections = start;
	return (0);
}
