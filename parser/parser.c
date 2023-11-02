/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:01:26 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:01:26 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fill_str_array(t_parser_utils *parser_utils, char **str, int arg_size)
{
	int		i;
	t_lexer	*tmp;

	i = 0;
	tmp = parser_utils->lexer_list;
	while (arg_size > 0 && tmp)
	{
		if (tmp->str)
		{
			str[i] = ft_strdup(tmp->str);
			if (!str[i])
				return (-1);
			lexer_delete_one(&parser_utils->lexer_list, tmp->i);
			tmp = parser_utils->lexer_list;
			i++;
		}
		arg_size--;
	}
	return (i);
}

t_simple_cmds	*init_cmd(t_parser_utils *parser_utils)
{
	char	**str;
	int		i;
	int		arg_size;

	i = 0;
	rm_redirections(parser_utils);
	arg_size = count_args(parser_utils->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		return (NULL);
	i = fill_str_array(parser_utils, str, arg_size);
	if (i == -1)
	{
		free_array(str);
		return (NULL);
	}
	str[i] = NULL;
	return (ft_simple_cmdsnew(str,
			parser_utils->num_redirections, parser_utils->redirections));
}

void	simple_cmdsadd_back(t_simple_cmds **lst, t_simple_cmds *new)
{
	t_simple_cmds	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp)
	{
		if (tmp->next)
		{
			tmp = tmp->next;
		}
		else
			break ;
	}
	tmp->next = new;
	new->prev = tmp;
}

int	pipe_last(t_utils_hold *utils_hold)
{
	t_lexer	*tmp;

	tmp = utils_hold->lexer_list;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->token == 1)
		return (1);
	return (0);
}

int	parser(t_utils_hold *utils_hold)
{
	t_simple_cmds	*simple_cmds;
	t_parser_utils	parser_utils;

	utils_hold->simple_cmds = NULL;
	if (utils_hold->lexer_list->token == 1 || pipe_last(utils_hold) == 1)
		return (ft_error(0, utils_hold));
	while (utils_hold->lexer_list)
	{
		if (utils_hold->lexer_list->token == 1)
			lexer_delete_one(&utils_hold->lexer_list,
				utils_hold->lexer_list->i);
		if (check_error(utils_hold) != 1)
			return (0);
		parser_utils = init_parser(utils_hold->lexer_list, utils_hold);
		simple_cmds = init_cmd(&parser_utils);
		if (!simple_cmds)
			return (0);
		if (!utils_hold->simple_cmds)
			utils_hold->simple_cmds = simple_cmds;
		else
			simple_cmdsadd_back(&utils_hold->simple_cmds, simple_cmds);
		utils_hold->lexer_list = parser_utils.lexer_list;
	}
	return (1);
}
