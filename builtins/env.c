/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 01:51:56 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 01:51:56 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	if (utils_hold->args[0] != '\0')
	{
		ft_putstr_fd("env: '", 1);
		ft_putstr_fd(utils_hold->args, 1);
		ft_putstr_fd("': No such file or directory\n", 1);
		return (1);
	}
	while (utils_hold->envp[i])
	{
		if (utils_hold->envp[i] && utils_hold->envp[i][0] != '\0')
		{
			ft_putstr_fd(utils_hold->envp[i], 1);
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
	return (0);
}
