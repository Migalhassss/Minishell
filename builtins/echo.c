/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:01:22 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/16 17:01:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_n(char *args)
{
	int	i;

	i = 1;
	if (!ft_strncmp(args, "-n", 2))
	{
		while (args[i] == 'n')
			i++;
		if (args[i] == ' ')
			return (i);
		else if (args[i] == '\0')
			return (i);
	}
	return (-1);
}

void	print_args(char	*args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i] == '|')
			return ;
		else if (args[i] == '<')
			return ;
		else if (args[i] == '>')
			return ;
		ft_putchar_fd(args[i], 1);
		i++;
	}
}

int	ft_echo(t_utils_hold *utils_hold)
{
	int	i;
	int	n_flag;

	i = 0;
	n_flag = check_n(utils_hold->args);
	if (n_flag != -1)
		i = n_flag;
	print_args(utils_hold->args);
	while (utils_hold->args[i] == ' ')
		i++;
	if (n_flag == -1)
		ft_putchar_fd('\n', 1);
	return (0);
}
