/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 18:38:04 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/13 23:33:12 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_pwd(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->envp[i++])
	{
		if (ft_strcharcmp2(utils_hold->envp[i], "PWD=", '='))
		{
			tmp = ft_strdup(utils_hold->envp[i] + 4);
			free(utils_hold->envp[i]);
			utils_hold->envp[i] = ft_strjoin("PWD=", utils_hold->pwd);
			free(tmp);
			break ;
		}
	}
	update_oldpwd(utils_hold);
}

void	update_oldpwd(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->envp[i++])
	{
		if (ft_strcharcmp2(utils_hold->envp[i], "OLDPWD=", '='))
		{
			tmp = ft_strdup(utils_hold->envp[i] + 7);
			free(utils_hold->envp[i]);
			utils_hold->envp[i] = ft_strjoin("OLDPWD=", utils_hold->pwd);
			free(tmp);
			break ;
		}
	}
}

int	check_n_args(t_utils_hold *utils_hold)
{
	int		i;
	char	**hold_args;

	i = 0;
	hold_args = ft_split(utils_hold->args, ' ');
	while (hold_args[i] != NULL)
		i++;
	free_array(hold_args);
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
		if (cd_home(utils_hold) == -1)
			return (-1);
	else if (utils_hold->args[0] == '-' && utils_hold->args[1] == '\0')
		if (cd_minus(utils_hold) == -1)
			return (-1);
	else if (utils_hold->args[0] == '.' && utils_hold->args[1] == '.'
		&& utils_hold->args[2] == '\0')
		if (cd_dotdot(utils_hold) == -1)
			return (-1);
	else
		if (cd_path(utils_hold) == -1)
			return (-1);
	return (0);
}
