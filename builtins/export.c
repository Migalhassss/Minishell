/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 01:52:56 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 01:52:56 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_if_exists(char **hold_args, t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	while (hold_args[i])
	{
		if (check_if_exists_helper(hold_args[i], utils_hold) == 0)
			return (1);
		i++;
	}
	return (-1);
}

int	check_if_exists3(char *hold_arg, t_utils_hold *utils_hold)
{
	int	j;

	if (check_if_exists_helper2(hold_arg, utils_hold) != -1)
	{
		j = check_if_exists_helper2(hold_arg, utils_hold);
		return (j);
	}
	return (-1);
}

void	update_envp(char *hold_args, t_utils_hold *utils_hold)
{
	int	i;
	int	k;

	i = 0;
	k = check_if_exists3(hold_args, utils_hold);
	if (k == -1)
		return ;
	while (utils_hold->envp[i])
	{
		if (i == k)
		{
			free(utils_hold->envp[i]);
			utils_hold->envp[i] = ft_strdup_2(hold_args);
			return ;
		}
		i++;
	}
	return ;
}

void	add_envp(char *hold_args, t_utils_hold *utils_hold, int arrlen)
{
	int		i;
	char	**new_envp;

	i = -1;
	while (utils_hold->envp[++i])
		if (ifexists_without_value(utils_hold->envp[i], hold_args) == 2)
			return ;
	i = 0;
	new_envp = malloc(sizeof(char *)
			* (ft_arrlen(utils_hold->envp) + arrlen + 1));
	while (utils_hold->envp[i])
	{
		new_envp[i] = ft_strdup_2(utils_hold->envp[i]);
		i++;
	}
	free_array(utils_hold->envp);
	new_envp[i++] = ft_strdup_2(hold_args);
	new_envp[i] = NULL;
	utils_hold->envp = new_envp;
	return ;
}

int	ft_export(t_utils_hold *utils_hold)
{
	char	**hold_args;
	int		i;

	i = 0;
	if (utils_hold->args[0] == '\0')
		ft_full_env(utils_hold);
	else
	{
		hold_args = ft_split(utils_hold->args, ' ');
		if (check_args(hold_args) == -1)
		{
			free_array(hold_args);
			return (1);
		}
		while (hold_args[i])
		{
			if (check_if_exists_helper(hold_args[i], utils_hold) == 1)
				update_envp(hold_args[i], utils_hold);
			else
				add_envp(hold_args[i], utils_hold, ft_arrlen(hold_args));
			i++;
		}
		free_array(hold_args);
	}
	return (0);
}
