/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:25:46 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:25:46 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// do add_node function

void	print_list(t_lexer *lexer_list)
{
	t_lexer	*tmp;

	tmp = lexer_list;

	while (tmp)
	{
		printf("Str = %s | ", tmp->str);
		printf("Token = %d\n", tmp->token);
		tmp = tmp->next;
	}
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

/*
" | " = 1
" > " =  2
" < " = 3
" >> " = 4
" << " = 5
*/

int	handle_token(t_utils_hold *utils_hold, int i, t_lexer **lexer_list)
{
	int	j;

	j = 0;
	if (utils_hold->args[i + j] == '|')
	{
		if (!add_node(ft_strdup("|"), 1, lexer_list, utils_hold))
			return (-1);
		utils_hold->pipes++;
		j++;
	}
	else if (utils_hold->args[i + j] == '<')
	{
		if (utils_hold->args[i + j + 1] == '<')
		{
			if (!add_node(ft_strdup("<<"), 5, lexer_list, utils_hold))
				return (-1);
			j += 2;
		}
		else
		{
			if (!add_node(ft_strdup("<"), 3, lexer_list, utils_hold))
				return (-1);
			j++;
		}
	}
	else if (utils_hold->args[i + j] == '>')
	{
		if (utils_hold->args[i + j + 1] == '>')
		{
			if (!add_node(ft_strdup(">>"), 4, lexer_list, utils_hold))
				return (-1);
			j += 2;
		}
		else
		{
			if (!add_node(ft_strdup(">"), 2, lexer_list, utils_hold))
				return (-1);
			j++;
		}
	}
	else
		j++;
	return (j);
}

int	token_reader(t_utils_hold *utils_hold)
{
	int		i;
	int		j;

	i = 0;
	utils_hold->lexer_list = NULL;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == ' ')
			i++;
		else if (utils_hold->args[i] == '\"')
		{
			i++;
			j = i;
			while(utils_hold->args[j] != '\"' && utils_hold->args[j] != '\0')
				j++;
			if (!add_node(ft_substr(utils_hold->args, i, j), 0,
					&utils_hold->lexer_list, utils_hold))
				return (0);
			i = j + 1;
		}
		else if (utils_hold->args[i] == '|' || utils_hold->args[i] == '<'
			|| utils_hold->args[i] == '>')
		{
			j = handle_token(utils_hold, i, &utils_hold->lexer_list);
			if (j == -1)
				return (0);
			i += j;
		}
		else
		{
			j = 0;
			while (utils_hold->args[i + j] != ' ' && utils_hold->args[i + j] != '|'
				&& utils_hold->args[i + j] != '<' && utils_hold->args[i + j] != '>'
				&& utils_hold->args[i + j] != '\0')
				j++;
			if (!add_node(ft_substr(utils_hold->args, i, j), 0,
					&utils_hold->lexer_list, utils_hold))
				return (0);
			i += j;
		}
	}
	return (1);
}
