/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:10:54 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/27 20:04:24 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cmd_not_found(char *str, t_utils_hold *utils_hold)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	utils_hold->exit_code = 127;
	return (-1);
}

char	*join_split_str(char **split_str, char *new_str)
{
	char	*tmp;
	char	*add_space;
	int		i;

	if (!split_str || !split_str[0])
		return (NULL);
	tmp = ft_strdup(split_str[0]);
	i = 1;
	while (split_str[i])
	{
		new_str = tmp;
		add_space = ft_strjoin(new_str, " ");
		free(new_str);
		tmp = ft_strjoin(add_space, split_str[i]);
		free(add_space);
		i++;
	}
	new_str = tmp;
	return (new_str);
}

char	**resplit_str(char **double_arr)
{
	char	**ret;
	char	*joined_str;

	joined_str = join_split_str(double_arr, NULL);
	free_array(double_arr);
	ret = ft_split(joined_str, ' ');
	free(joined_str);
	joined_str = join_split_str(ret, NULL);
	free_array(ret);
	ret = ft_split(joined_str, ' ');
	free(joined_str);
	return (ret);
}

int	find_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int		i;
	char	*mycmd;

	i = 0;
	if (cmd->str[0][0] == '\0')
		return (-1);
	cmd->str = resplit_str(cmd->str);
	if (!access(cmd->str[0], F_OK))
		execve(cmd->str[0], cmd->str, utils_hold->envp);
	while (utils_hold->paths[i])
	{
		mycmd = ft_strjoin(utils_hold->paths[i], cmd->str[0]);
		if (!access(mycmd, F_OK))
			execve(mycmd, cmd->str, utils_hold->envp);
		free(mycmd);
		i++;
	}
	cmd_not_found(cmd->str[0], utils_hold);
	return (-1);
}

int	check_append_outfile(t_lexer *redirections, char *file)
{
	int	fd;

	if (redirections->token == 4)
		fd = open(file,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}
