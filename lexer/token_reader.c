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

/*
" | " = 1
" > " =  2
" < " = 3
" >> " = 4
" << " = 5
*/

int	handle_check1(t_utils_hold *utils_hold, int i, int *j, t_lexer **lexer_list)
{
	if (utils_hold->args[i + (*j) + 1] == '>')
	{
		if (!add_node(ft_strdup(">>"), 4, lexer_list, utils_hold))
			return (-1);
		(*j) += 2;
	}
	else
	{
		if (!add_node(ft_strdup(">"), 2, lexer_list, utils_hold))
			return (-1);
		(*j)++;
	}
	return (0);
}

int	handle_check2(t_utils_hold *utils_hold, int i, int *j, t_lexer **lexer_list)
{
	if (utils_hold->args[i + (*j) + 1] == '<')
	{
		if (!add_node(ft_strdup("<<"), 5, lexer_list, utils_hold))
			return (-1);
		(*j) += 2;
	}
	else
	{
		if (!add_node(ft_strdup("<"), 3, lexer_list, utils_hold))
			return (-1);
		(*j)++;
	}
	return (0);
}

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
		if (handle_check2(utils_hold, i, &j, lexer_list) == -1)
			return (-1);
	}
	else if (utils_hold->args[i + j] == '>')
	{
		if (handle_check1(utils_hold, i, &j, lexer_list) == -1)
			return (-1);
	}
	else
		j++;
	return (j);
}

int	check_pipes(t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	if (utils_hold->args[0] == '|'
		|| utils_hold->args[ft_strlen(utils_hold->args) - 1] == '|')
		return (ft_error(0, utils_hold));
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '\"')
		{
			i++;
			while (utils_hold->args[i] != '\"' && utils_hold->args[i] != '\0')
				i++;
		}
		if (utils_hold->args[i] == '|')
			if (utils_hold->args[i + 1] == '|')
				return (ft_error(0, utils_hold));
		i++;
	}
	return (0);
}

int	token_reader(t_utils_hold *utils_hold)
{
	utils_hold->lexer_list = NULL;
	if (ft_strlen(utils_hold->args) == 0)
		return (0);
	if (ft_strchr(utils_hold->args, '|') && check_pipes(utils_hold))
		return (ft_error(0, utils_hold));
	if (token_reader2(utils_hold) == 1)
		return (1);
	return (0);
}
