/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:15:45 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:15:45 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	while (utils_hold->pwd[i])
	{
		ft_putchar_fd(utils_hold->pwd[i], 1);
		i++;
	}
	ft_putchar_fd('\n', 1);
	return (0);
}
