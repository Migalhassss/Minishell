 
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

int	check_error(t_utils_hold *utils_hold)
{
	if (utils_hold->lexer_list == NULL)
		return (0);
	if (utils_hold->lexer_list->token == 1)
		return (printf("Pipe cant be first\n"));
	return (1);
}

t_lexer	*lexer_clear_one(t_lexer **lexer_list)
{
	if ((*lexer_list)->str)
	{
		free((*lexer_list)->str);
		(*lexer_list)->str = NULL;
	}
	free(*lexer_list);
	*lexer_list = NULL;
	return (NULL);
}

void	lexer_delete_first(t_lexer **lexer_list)
{
	t_lexer	*node;

	node = *lexer_list;
	*lexer_list = node->next;
	lexer_clear_one(&node);
	if (*lexer_list)
		(*lexer_list)->prev = NULL;
}

void	lexer_delete_one(t_lexer **lexer_list, int index_list)
{
	t_lexer	*node;
	t_lexer	*prev;
	t_lexer	*start;

	start = *lexer_list;
	node = start;
	if ((*lexer_list)->i == index_list)
	{
		lexer_delete_first(lexer_list);
		return ;
	}
	while (node && node->i != index_list)
	{
		prev = node;
		node = node->next;
	}
	if (node)
		prev->next = node->next;
	else
		prev->next = NULL;
	if (prev->next)
		prev->next->prev = prev;
	lexer_clear_one(&node);
	*lexer_list = start;
}

t_parser_utils	init_parser(t_lexer *lexer_list, t_utils_hold *utils_hold)
{
	t_parser_utils	parser_utils;

	parser_utils.lexer_list = lexer_list;
	parser_utils.redirections = NULL;
	parser_utils.num_redirections = 0;
	parser_utils.utils = utils_hold;
	return (parser_utils);
}

void	lexeradd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_lexer	*ft_newlexer(char *str, int token)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->token = token;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

int	add_redirection(t_lexer *tmp, t_parser_utils *parser_utils)
{
	int		i;
	int		j;
	t_lexer	*new_node;

	new_node = ft_newlexer(ft_strdup(tmp->str), tmp->token);
	if (!new_node)
		return (printf("Error: malloc failed\n"));
	lexeradd_back(&parser_utils->redirections, new_node);
	i = tmp->i;
	j = tmp->next->i;
	lexer_delete_one(&parser_utils->lexer_list, i);
	lexer_delete_one(&parser_utils->lexer_list, j);
	parser_utils->num_redirections++;
	return (1);
}	

void	rm_redirections(t_parser_utils *parser_utils)
{
	t_lexer	*tmp;

	tmp = parser_utils->lexer_list;
	while (tmp && tmp->token == 0)
		tmp = tmp->next;
	if (!tmp || tmp->token == 1)
		return ;
	if (!tmp->next)
		reset_utils_hold(parser_utils->utils);
	if (tmp->next->token)
		reset_utils_hold(parser_utils->utils);
	if ((tmp->token >= 2 && tmp->token <= 5))
		add_redirection(tmp, parser_utils);
	rm_redirections(parser_utils);
}

int	count_args(t_lexer *lexerlist)
{
	int		i;
	t_lexer	*tmp;

		i = 0;
		tmp = lexerlist;
		while (tmp && tmp->token != 1)
	{
		if (tmp->token == 0)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

t_simple_cmds	*ft_simple_cmdsnew(char **str, int num_redirections,
	t_lexer *redirections)
{
	t_simple_cmds	*new_node;

	new_node = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	printf("str[0] = %s\n", str[0]);
	new_node->builtin = 0;
	new_node->hd_file_name = NULL;
	new_node->num_redirections = num_redirections;
	new_node->redirections = redirections;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_simple_cmds	*init_cmd(t_parser_utils *parser_utils)
{
	char	**str;
	int		i;
	int		arg_size;
	t_lexer	*tmp;

	i = 0;
	rm_redirections(parser_utils);
	arg_size = count_args(parser_utils->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		return (NULL);
	tmp = parser_utils->lexer_list;
	while (arg_size > 0 && tmp)
	{
		if (tmp->str)
		{
			str[i] = ft_strdup(tmp->str);
			if (!str[i])
			{
				while (i > 0)
					free(str[--i]);
				free(str);
				return (NULL);
			}
			lexer_delete_one(&parser_utils->lexer_list, tmp->i);
			tmp = parser_utils->lexer_list;
			i++;
		}
		arg_size--;
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

int	parser(t_utils_hold *utils_hold)
{
	t_simple_cmds	*simple_cmds;
	t_parser_utils	parser_utils;

	utils_hold->simple_cmds = NULL;
	if (utils_hold->lexer_list->token == 1)
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
