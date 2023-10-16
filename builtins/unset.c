/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:18:22 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:18:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_if_exists_helper2(char *str, t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp2(str, utils_hold->envp[i], '=') == 1)
			return (i);
		i++;
	}
	return (-1);
}

int	check_if_exists2(char **hold_args, t_utils_hold *utils_hold)
{
	int	i;
	int	j;

	i = 0;
	while (hold_args[i])
	{
		if (check_if_exists_helper2(hold_args[i], utils_hold) != -1)
		{
			j = check_if_exists_helper2(hold_args[i], utils_hold);
			return (j);
		}
		i++;
	}
	return (-1);
}

char	**dup_array(t_utils_hold *utils_hold, int j)
{
	int		i;
	int		k;
	char	**dup_envp;

	i = 0;
	k = 0;
	while (utils_hold->envp[k])
		k++;
	dup_envp = ft_calloc(sizeof(char *), k);
	k = 0;
	while (utils_hold->envp[i])
	{
		if (i == j)
			i++;
		if (utils_hold->envp[i] != NULL)
			dup_envp[k] = ft_strdup(utils_hold->envp[i]);
		else
			return (dup_envp);
		k++;
		i++;
	}
	free_array(utils_hold->envp);
	dup_envp[k] = NULL;
	return (dup_envp);
}

int	check_args1(char **hold_args)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	flag = 0;
	while (hold_args[i])
	{
		j = 0;
		if (checkerror_identifier(hold_args[i]) == 1)
			return (-1);
		while (hold_args[i][j])
		{
			else if (hold_args[i][j] == '=')
				flag = 1;
			j++;
		}
		if (flag == 0)
			return (-1);
		flag = 0;
		i++;
	}
	return (0);
}

int	ft_unset(t_utils_hold *utils_hold)
{
	int		i;
	char	**hold_args;
	char	**dup_envp;

	i = 0;
	hold_args = ft_split(utils_hold->args, ' ');
	if (!check_args1(hold_args)
		&& check_if_exists2(hold_args, utils_hold) != -1)
	{
		while (hold_args[i])
		{
			dup_envp = dup_array(utils_hold,
					check_if_exists2(hold_args, utils_hold););
			utils_hold->envp = dup_envp;
			i++;
		}
	}
	else
	{
		free_array(hold_args);
		return (1);
	}
	free_array(hold_args);
	return (0);
}
