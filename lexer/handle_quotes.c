/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:25:38 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:25:38 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_parser_utils	init_parser(t_lexer *lexer_list, t_utils_hold *utils_hold)
{
	t_parser_utils	parser_utils;

	parser_utils.lexer_list = lexer_list;
	parser_utils.redirections = NULL;
	parser_utils.num_redirections = 0;
	parser_utils.utils = utils_hold;
	return (parser_utils);
}

int	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	add_node(char *str, int token, t_lexer **lexer_list,
		t_utils_hold *utils_hold)
{
	t_lexer	*new_node;
	t_lexer	*tmp;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (0);
	new_node->str = str;
	new_node->token = token;
	new_node->next = NULL;
	new_node->i = utils_hold->index_list++;
	new_node->next = NULL;
	new_node->prev = NULL;
	if (!*lexer_list)
		*lexer_list = new_node;
	else
	{
		tmp = *lexer_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->prev = tmp;
	}
	return (1);
}

int	token_reader2(t_utils_hold *utils_hold)
{
	int		i;

	i = 0;
	utils_hold->lexer_list = NULL;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == ' ')
			i++;
		else if (utils_hold->args[i] == '\"')
			i = handle_double_quotes(utils_hold, i);
		else if (utils_hold->args[i] == '\'')
			i = handle_single_quotes(utils_hold, i);
		else if (utils_hold->args[i] == '|' || utils_hold->args[i] == '<'
			|| utils_hold->args[i] == '>')
			i = handle_token2(utils_hold, i);
		else
			i = handle_word(utils_hold, i);
		if (i == -1)
			return (1);
	}
	return (0);
}

int	token_reader(t_utils_hold *utils_hold)
{
	utils_hold->lexer_list = NULL;
	if (ft_strlen(utils_hold->args) == 0 || only_spaces(utils_hold->args) == 1)
		return (reset_utils_hold(utils_hold));
	if (check_pipes(utils_hold) == 1)
		return (1);
	if (token_reader2(utils_hold) == 1)
		return (1);
	lexer_to_args(utils_hold);
	return (0);
}
