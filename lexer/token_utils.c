/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:38:07 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/31 18:04:44 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	handle_single_quotes(t_utils_hold *utils_hold, int i)
{
	int		j;

	j = i + 1;
	i++;
	if (utils_hold->args[i] == '\'' && utils_hold->args[i + 1] != '\0'
			&& (size_t)i <= ft_strlen(utils_hold->args))
		i += 2;
	while (utils_hold->args[i] != '\'' && utils_hold->args[i] != '\0'
			&& (size_t)i <= ft_strlen(utils_hold->args))
		i++;
	if (utils_hold->args[i + 1] == '\'' && utils_hold->args[i + 1] != '\0'
			&& (size_t)i <= ft_strlen(utils_hold->args))
		i++;
	if (!add_node(ft_substr(utils_hold->args, j, i - j), 0,
			&utils_hold->lexer_list, utils_hold))
		return (-1);
	i++;
	return (i);
}

int	handle_double_quotes(t_utils_hold *utils_hold, int i)
{
	int		j;
	bool	quote;

	j = i;
	i++;
	quote = false;
	while (utils_hold->args[i] != '\"' && utils_hold->args[i] != '\0'
			&& (size_t)i <= ft_strlen(utils_hold->args))
		i++;
	i++;
	if (!add_node(ft_substr(utils_hold->args, j, i - j), 0,
			&utils_hold->lexer_list, utils_hold))
		return (-1);
	return (i);
}

int	handle_token2(t_utils_hold *utils_hold, int i)
{
	int		j;

	j = handle_token(utils_hold, i, &utils_hold->lexer_list);
	if (j == -1)
		return (-1);
	return (i + j);
}

char	*str_withoutquotes(char *str, int i, int j)
{
	char	*new_str;
	int		k;

	k = 0;
	new_str = (char *)malloc(sizeof(char) * (j - i + 1));
	if (!new_str)
		return (NULL);
	while (i < j)
	{
		if (str[i] == '\"' || str[i] == '\'')
			i++;
		if (str[i] != '\0')
			new_str[k] = str[i];
		else
			break ;
		k++;
		i++;
	}
	new_str[k] = '\0';
	return (new_str);
}

int	handle_word(t_utils_hold *utils_hold, int i)
{
	int		j;
	int		k;
	j = i;
	k = 0;
	while (utils_hold->args[j] != ' ' && utils_hold->args[j] != '|'
		&& utils_hold->args[j] != '<' && utils_hold->args[j] != '>'
		&& utils_hold->args[j] != '\0')
	{
		j++;
		k++;
	}
	if (!add_node(str_withoutquotes(utils_hold->args, i, j), 0,
			&utils_hold->lexer_list, utils_hold))
		return (-1);
	return (i + k);
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
