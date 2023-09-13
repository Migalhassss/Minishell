/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:12:27 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:12:27 by micarrel         ###   ########.fr       */
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

	i = 0;
	s = 0;
	while (line[i])
	{
		if (line[i] == 39)
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
			break;
		if (line[i] == 34)
			d++;
		i++;
	}
	if (d % 2 != 0)
		return (1);
	return (0);
}

int		count_quotes(char *line)
{
	if (any_quote(line) == 0)
		return (0);
	if (single_quote(line) == 1 || double_quote(line) == 1)
		return (1);
	return (0);
}

char	**ft_envpdup(char **envp)
{
	char	**aux;
	size_t	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	aux = ft_calloc(sizeof(char *), i + 1);
	if (!aux)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		aux[i] = ft_strdup(envp[i]);
		if (aux[i] == NULL)
		{
			free_array(aux);
			return (aux);
		}
		i++;
	}
	return (aux);
}

void	free_array(char **split_array)
{
	int	i;

	i = 0;
	while (split_array[i])
	{
		if (split_array[i])
			free(split_array[i]);
		i++;
	}
	if (split_array)
		free(split_array);
}
