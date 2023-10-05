/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 01:39:31 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 01:39:31 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

t_global	g_global;

void	sigint_handler(int sig_num)
{
	write (1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig_num;
}

int	main(int ac, char **av, char **envp)
{
	t_utils_hold	utils;

	if (ac != 1 || av[1])
	{
		printf("This program takes no arguments!\n");
		exit(0);
	}
	signal(SIGINT, sigint_handler);
	utils.envp = ft_envpdup(envp);
	pwd_find(&utils);
	implement_utils_hold(&utils);
	// printf("\n%s\n\n", "Welcome to minishell!");
	g_global.exit_code = 0;
	minishell_loop(&utils);
}
