/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:05:08 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/25 15:42:23 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	new_node->builtin = 0;
	new_node->hd_file_name = NULL;
	new_node->num_redirections = num_redirections;
	new_node->redirections = redirections;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}
