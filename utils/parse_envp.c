/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:12:22 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:12:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pwd_find(t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	while (utils_hold->envp[i])
	{
		if (!ft_strncmp(utils_hold->envp[i], "PWD=", 4))
			utils_hold->pwd = ft_substr(utils_hold->envp[i],
					4, ft_strlen(utils_hold->envp[i]) - 4);
		if (!ft_strncmp(utils_hold->envp[i], "OLDPWD=", 7))
			utils_hold->old_pwd = ft_substr(utils_hold->envp[i],
					7, ft_strlen(utils_hold->envp[i]) - 7);
		i++;
	}
	return (1);
}

char	*check_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5));
		i++;
	}
	return (ft_strdup("\0"));
}

void	path_find(t_utils_hold *utils_hold)
{
	char	*path;
	int		i;
	char	*part_path;

	path = check_path(utils_hold->envp);
	utils_hold->paths = ft_split(path, ':');
	free(path);
	i = 0;
	while (utils_hold->paths[i])
	{
		if (ft_strncmp(&utils_hold->paths[i]
				[ft_strlen(utils_hold->paths[i]) - 1], "/", 1) != 0)
		{
			part_path = ft_strjoin(utils_hold->paths[i], "/");
			free(utils_hold->paths[i]);
			utils_hold->paths[i] = part_path;
		}
		i++;
	}
}
