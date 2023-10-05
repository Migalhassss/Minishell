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
			break;
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
			printf("tmp = %s\n", tmp);
		}
		i++;
	}
	str = ft_strdup(tmp);
	free(tmp);
	return (str);
}

int		count_quotes(t_utils_hold *utils_hold)
{
	if (any_quote(utils_hold->args) == 0)
		return (0);
	if (single_quote(utils_hold->args) == 1 || double_quote(utils_hold->args) == 1)
		return (1);
	utils_hold->args = delete_quotes(utils_hold->args, '"');
	utils_hold->args = delete_quotes(utils_hold->args, '\'');
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
