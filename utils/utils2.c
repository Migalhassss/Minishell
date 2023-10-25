/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:44:47 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/25 18:09:14 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_matching_quote(char *line, int i, int *num_del, int del)
{
	int	j;

	if (!line[i] || !line[i + 1])
		return (-1);
	j = i + 1;
	*num_del += 1;
	while (line[j] && line[j] != del)
		j++;
	if (line[j] == del)
		*num_del += 1;
	return (j - i);
}

int	any_quote(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 34 || line[i] == 39)
			return (1);
		i++;
	}
	return (0);
}

int	single_quote(char *line)
{
	int	i;
	int	s;
	int	int_quotes;

	i = 0;
	s = 0;
	int_quotes = 0;
	while (line[i])
	{
		if (line[i] == 34)
			int_quotes += 1;
		if (line[i] == 39 && int_quotes % 2 == 0)
			s++;
		i++;
	}
	if (s % 2 != 0)
		return (1);
	return (0);
}

int	double_quote(char *line)
{
	int	i;
	int	d;

	i = 0;
	d = 0;
	while (line[i])
	{
		if (line[i] == 39)
			while (line[i] != 39 && line[i])
				i++;
		if (!line[i])
			break ;
		if (line[i] == 34)
			d++;
		i++;
	}
	if (d % 2 != 0)
		return (1);
	return (0);
}

char	*delete_quotes(char *str, char c)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = ft_strdup(str);
	free(str);
	while (tmp[i])
	{
		if (tmp[i] == c)
		{
			j = 0;
			while (tmp[i + j] == c)
				j++;
			ft_strlcpy(&tmp[i], &tmp[i + j], ft_strlen(tmp) - i);
		}
		i++;
	}
	str = ft_strdup(tmp);
	free(tmp);
	return (str);
}
