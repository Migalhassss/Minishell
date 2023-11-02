/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:45:16 by micarrel          #+#    #+#             */
/*   Updated: 2023/11/02 19:01:47 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_this_char(char *args, int i)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(args, 0, i);
	tmp2 = ft_substr(args, i + 1, ft_strlen(args));
	free(args);
	args = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
	return (args);
}

int	quote_inlast(char *args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (args[i - 1] == '\"')
		return (1);
	return (0);
}

int	ifis_onlydouble_quotes(char *args, int i)
{
	if (args[i] == '\"' && args[i + 1] == '\"'
		&& (args[i + 2] == '\0' || args[i + 2] == ' ')
		&& (args[i - 1] == ' ' || args[i - 1] == '\0'))
		return (1);
	return (0);
}

char	*remove_quotes_utils(char	*tmp, unsigned int i, bool in_quotes)
{
	while (tmp[i])
	{
		if (tmp[i] && tmp[i] == '\"')
		{
			if (ifis_onlydouble_quotes(tmp, i) == 1)
				tmp = remove_this_char(tmp, i);
			tmp = remove_this_char(tmp, i);
			if (i > ft_strlen(tmp) || !ft_strlen(tmp) || !tmp[i])
				break ;
			in_quotes = !in_quotes;
		}
		if (tmp[i] && tmp[i] == '\'' && in_quotes == false)
		{
			tmp = remove_this_char(tmp, i);
			if (i > ft_strlen(tmp))
				break ;
			while (tmp[i] && tmp[i] != '\'')
				i++;
			tmp = remove_this_char(tmp, i);
			i--;
		}
		i++;
	}
	return (tmp);
}

void	remove_quotes(t_utils_hold *utils_hold)
{
	char				*tmp;
	unsigned int		i;
	bool				in_quotes;

	i = 0;
	in_quotes = false;
	if (any_quote(utils_hold->args) == 0)
		return ;
	tmp = ft_strdup(utils_hold->args);
	free(utils_hold->args);
	tmp = remove_quotes_utils(tmp, i, in_quotes);
	utils_hold->args = ft_strdup(tmp);
	free(tmp);
}
