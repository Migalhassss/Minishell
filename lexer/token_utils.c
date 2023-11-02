/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:38:07 by micarrel          #+#    #+#             */
/*   Updated: 2023/11/02 19:05:52 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_single_quotes(t_utils_hold *utils_hold, int i)
{
	int		j;

	j = i;
	i++;
	while (utils_hold->args[i] != '\'' && utils_hold->args[i] != '\0'
		&& (size_t)i <= ft_strlen(utils_hold->args))
		i++;
	if (!add_node(ft_substr(utils_hold->args, j, i - j), 0,
			&utils_hold->lexer_list, utils_hold))
		return (-1);
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
