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

int	ifis_double_quotes(t_utils_hold *utils_hold)
{
	if (utils_hold->args[0] == '\"' 
		&& utils_hold->args[1] == '\"' 
		&& ft_strlen(utils_hold->args) == 2)
	{
		utils_hold->args = delete_quotes(utils_hold->args, '\"');
		return (1);
	}
	return (0);
}

int	count_quotes(t_utils_hold *utils_hold)
{
	char	*tmp;

	tmp = ft_strtrim(utils_hold->args, " ");
	if (ft_strlen(tmp) == 0 || !tmp)
	{
		free(tmp);
		reset_utils_hold(utils_hold);
	}
	free(tmp);
	if (any_quote(utils_hold->args) == 0)
		return (0);
	if (single_quote(utils_hold->args) == 1
		|| double_quote(utils_hold->args) == 1)
		return (1);
	if (ifis_double_quotes(utils_hold) == 1)
		return (0);
	remove_quotes(utils_hold);
	tmp = ft_strtrim(utils_hold->args, " ");
	if (ft_strlen(tmp) == 0 || !tmp
		|| ft_strlen(utils_hold->args) == 0)
	{
		free(tmp);
		reset_utils_hold(utils_hold);
	}
	free(tmp);
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
