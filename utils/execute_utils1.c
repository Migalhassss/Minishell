/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:49:16 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/25 15:49:16 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_lexer(t_lexer *redirections)
{
	t_lexer	*current;
	t_lexer	*next;

	current = redirections;
	if (!redirections)
		return ;
	if (redirections->prev != NULL)
		while (redirections->prev)
			redirections = redirections->prev;
	while (current)
	{
		next = current->next;
		if (current->str)
			free(current->str);
		free(current);
		current = next;
	}
}

int	check_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int		i;
	char	*mycmd;

	i = 0;
	if (!cmd->str ||!cmd->str[0] || !cmd->str[0][0])
		return (-1);
	cmd->str = resplit_str(cmd->str);
	if (!access(cmd->str[0], F_OK))
		return (0);
	while (utils_hold->paths[i])
	{
		mycmd = ft_strjoin(utils_hold->paths[i], cmd->str[0]);
		if (!access(mycmd, F_OK))
		{
			free(mycmd);
			return (0);
		}
		free(mycmd);
		i++;
	}
	cmd_not_found(cmd->str[0], utils_hold);
	return (1);
}

int	check_redirections_doc(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	t_lexer	*tmp;

	if (utils_hold->args[0] == '>' || utils_hold->args[0] == '<')
		return (0);
	if (cmd->str[0][0] == '>' || cmd->str[0][0] == '<')
		return (0);
	tmp = cmd->redirections;
	if (cmd->prev != NULL)
		while (cmd->prev)
			cmd = cmd->prev;
	while (tmp)
	{
		if (tmp->token == 5)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	join_split_str2(t_utils_hold *utils_hold, t_simple_cmds *cmd)
{
	free(utils_hold->args);
	utils_hold->args = join_split_str(cmd->str, NULL);
	remove_quotes(utils_hold);
}

void	handle_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int		exit_code;
	char	*tmp;

	exit_code = 0;
	if (cmd->redirections)
	{
		if (check_redirections(cmd, utils_hold))
		{
			perror("error");
			clean_exit(utils_hold, exit_code);
		}
	}
	join_split_str2(utils_hold, cmd);
	if (check_redirections_doc(cmd, utils_hold))
	{
		tmp = ft_strjoin(utils_hold->args, " ");
		free(utils_hold->args);
		utils_hold->args = ft_strjoin(tmp, cmd->hd_file_name);
		free(tmp);
	}
	if (cmd->str[0][0] != '\0' && check_builtins(utils_hold) == 1)
		which_command(utils_hold);
	else if (cmd->str[0][0] != '\0')
		exit_code = find_cmd(cmd, utils_hold);
	clean_exit(utils_hold, exit_code);
}
