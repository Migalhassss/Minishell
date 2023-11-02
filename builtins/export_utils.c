/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 18:44:27 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/11/02 19:08:16 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcharcmp(char *s1, const char *s2, char c)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != c && s2[i] != c)
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (s1[i] == c && s2[i] == c)
		return (1);
	return (0);
}

int	envp_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	ifexists_without_value(char *evnp, char *str)
{
	int	i;

	i = 0;
	while (str[i] && (evnp[i] != '=' && str[i] != '='))
	{
		if (str[i] != evnp[i])
			return (1);
		i++;
	}
	if (evnp[i] == '=')
		return (2);
	return (0);
}

int	check_if_exists_helper(char *str, t_utils_hold *utils_hold)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp(str, utils_hold->envp[i], '='))
			flag = 1;
		else if (ifexists_without_value(utils_hold->envp[i], str) == 0)
			flag = 1;
		i++;
	}
	return (flag);
}

int	ft_isalnum2(char	*str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (ft_isalnum(str[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}
