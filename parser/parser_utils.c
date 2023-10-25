/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:02:25 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/25 15:43:06 by micarrel         ###   ########.fr       */
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
