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

void	update_pwd(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp2(utils_hold->envp[i], "PWD=", '='))
		{
			tmp = ft_strdup(utils_hold->envp[i] + 4);
			free(utils_hold->envp[i]);
			utils_hold->envp[i] = ft_strjoin("PWD=", utils_hold->pwd);
			free(tmp);
			break ;
		}
		i++;
	}
	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp2(utils_hold->envp[i], "OLDPWD=", '='))
		{
			tmp = ft_strdup(utils_hold->envp[i] + 7);
			free(utils_hold->envp[i]);
			utils_hold->envp[i] = ft_strjoin("OLDPWD=", utils_hold->old_pwd);
			free(tmp);
			break ;
		}
		i++;
	}
}

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
			utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
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
	utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
	if (chdir(utils_hold->args) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(utils_hold->args, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	utils_hold->pwd = getcwd(NULL, 0);
	update_pwd(utils_hold);
	return (0);
}

int	cd_minus(t_utils_hold *utils_hold)
{
	char	*tmp;

	if (!utils_hold->old_pwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (-1);
	}
	else
	{
		tmp = ft_strdup(utils_hold->old_pwd);
		utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
		utils_hold->pwd = ft_strdup(tmp);
		free(tmp);
		if (chdir(utils_hold->pwd) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(utils_hold->pwd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
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
	utils_hold->old_pwd = ft_strdup(utils_hold->pwd);
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

int	check_n_args(t_utils_hold *utils_hold)
{
	int		i;
	char	**hold_args;

	i = 0;
	hold_args = ft_split(utils_hold->args, ' ');
	while (hold_args[i] != NULL)
		i++;
	if (i > 1)
		return (-1);
	return (0);
}

int	check_cd_args(t_utils_hold *utils_hold)
{
	if (check_n_args(utils_hold) == -1)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (-1);
	}
	else if (utils_hold->args[0] == '~' && utils_hold->args[1] == '\0')
	{
		if (cd_home(utils_hold) == -1)
			return (-1);
	}
	else if (utils_hold->args[0] == '-' && utils_hold->args[1] == '\0')
	{
		if (cd_minus(utils_hold) == -1)
			return (-1);
	}
	else if (utils_hold->args[0] == '.' && utils_hold->args[1] == '.'
		&& utils_hold->args[2] == '\0')
	{
		if (cd_dotdot(utils_hold) == -1)
			return (-1);
	}
	else
	{
		if (cd_path(utils_hold) == -1)
			return (-1);
	}
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
