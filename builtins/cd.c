/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 01:42:59 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 01:42:59 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd_home(t_utils_hold *utils_hold)
{
	int		i;
	char	*home;

	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp2(utils_hold->envp[i], "HOME=", '='))
		{
			home = ft_strdup(utils_hold->envp[i] + 5);
			free(utils_hold->old_pwd);
			utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
			free(utils_hold->pwd);
			utils_hold->pwd = ft_strdup(home);
			update_pwd(utils_hold);
			if (chdir(home) == -1)
				return (-1);
			free(home);
			return (0);
		}
		i++;
	}
	return (-1);
}

int	cd_path(t_utils_hold *utils_hold)
{
	free(utils_hold->old_pwd);
	utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
	if (chdir(utils_hold->args) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(utils_hold->args, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	free(utils_hold->pwd);
	utils_hold->pwd = getcwd(NULL, 0);
	update_pwd(utils_hold);
	return (0);
}

int	cd_minus(t_utils_hold *utils_hold)
{
	char	*tmp;

	if (!utils_hold->old_pwd)
		return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), -1);
	else
	{
		tmp = ft_strdup(utils_hold->old_pwd);
		free(utils_hold->old_pwd);
		utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
		free(utils_hold->pwd);
		utils_hold->pwd = ft_strdup(tmp);
		free(tmp);
		if (chdir(utils_hold->pwd) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(utils_hold->pwd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
		free(utils_hold->pwd);
		utils_hold->pwd = getcwd(NULL, 0);
		update_pwd(utils_hold);
	}
	return (0);
}

int	cd_dotdot(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->pwd[i])
		i++;
	while (utils_hold->pwd[i] != '/' && i > 1)
		i--;
	tmp = ft_substr(utils_hold->pwd, 0, i);
	free(utils_hold->old_pwd);
	utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
	free(utils_hold->pwd);
	utils_hold->pwd = ft_strdup(tmp);
	free(tmp);
	update_pwd(utils_hold);
	if (chdir(utils_hold->pwd) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(utils_hold->pwd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	utils_hold->pwd = getcwd(NULL, 0);
	return (0);
}

int	ft_cd(t_utils_hold *utils_hold)
{
	if (!utils_hold->args[0])
	{
		if (cd_home(utils_hold) == -1)
			return (1);
	}
	else if (check_cd_args(utils_hold) == -1)
		return (1);
	return (0);
}
