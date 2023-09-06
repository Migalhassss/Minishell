/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:18:22 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:18:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup2(const char *src, char c)
{
	char	*dest;
	int		i;
	int		len;

	len = 0;
	while (src[len] != '\0')
		len++;
	i = 0;
	dest = (char *)malloc(len * sizeof(char) + 2);
	if (!dest)
		return (0);
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = c;
	i++;
	dest[i] = '\0';
	return (dest);
}

int	ft_strcharcmp2(char *s1, const char *s2, char c)
{
	int		i;
	int		flag;
	char	*new_s1;

	i = 0;
	flag = 0;
	if (s1[ft_strlen(s1)] == c)
	{
		free(s1);
		return (1);
	}
	else
	{
		new_s1 = ft_strdup2(s1, c);
		s1 = new_s1;
	}
	while (s1[i] && s2[i] && s1[i] != c && s2[i] != c)
	{
		if (s1[i] != s2[i])
		{
			free(new_s1);
			return (0);
		}
		i++;
	}	
	if (s1[i] == c && s2[i] == c)
		flag = 1;
	free(new_s1);
	if (flag == 1)
		return (1);
	return (0);
}

int	check_if_exists_helper2(char *str, t_utils_hold *utils_hold)
{
	int	i;

	i = 0;
	while (utils_hold->envp[i])
	{
		if (ft_strcharcmp2(str, utils_hold->envp[i], '=') == 1)
			return (i);
		i++;
	}
	return (-1);
}

int	check_if_exists2(char **hold_args, t_utils_hold *utils_hold)
{
	int	i;
	int	j;

	i = 0;
	while (hold_args[i])
	{
		if (check_if_exists_helper2(hold_args[i], utils_hold) != -1)
		{
			j = check_if_exists_helper2(hold_args[i], utils_hold);
			return (j);
		}
		i++;
	}
	return (-1);
}

char	**dup_array(t_utils_hold *utils_hold, int j)
{
	int		i;
	int		k;
	char	**dup_envp;

	i = 0;
	k = 0;
	while (utils_hold->envp[k])
		k++;
	dup_envp = ft_calloc(sizeof(char *), k);
	k = 0;
	while (utils_hold->envp[i])
	{
		if (i == j)
			i++;
		if (utils_hold->envp[i] != NULL)
				dup_envp[k] = ft_strdup(utils_hold->envp[i]);
		else
			return (dup_envp);
		k++;
		i++;
	}
	free_array(utils_hold->envp);
	dup_envp[k] = NULL;
	return (dup_envp);
}

int	ft_unset(t_utils_hold *utils_hold)
{
	int		j;
	int		i;
	char	**hold_args;
	char	**dup_envp;

	j = 0;
	i = 0;
	hold_args = ft_split(utils_hold->args, ' ');
	if (check_if_exists2(hold_args, utils_hold) != -1)
	{
		while (hold_args[i])
		{
			j = check_if_exists2(hold_args, utils_hold);
			dup_envp = dup_array(utils_hold, j);
			utils_hold->envp = dup_envp;
			i++;
		}
	}
	else
	{
		free_array(hold_args);
		return (1);
	}
	free_array(hold_args);
	return (0);
}
