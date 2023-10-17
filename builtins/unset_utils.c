/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 18:49:47 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/17 13:57:09 by micarrel         ###   ########.fr       */
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
		return (free(s1), 1);
	else
	{
		new_s1 = ft_strdup2(s1, c);
		s1 = new_s1;
	}
	while (s1[i] && s2[i] && s1[i] != c && s2[i] != c)
	{
		if (s1[i] != s2[i])
			return (free(new_s1), 0);
		i++;
	}
	if (s1[i] == c && s2[i] == c)
		flag = 1;
	free(new_s1);
	if (flag == 1)
		return (1);
	return (0);
}

int	checkerror_identifier(char **hold_args)
{
	int	i;
	int	j;

	i = -1;
	if (ft_strchr(hold_args[1], '=') != NULL)
	{
		printf("minishell: export: `%s': not a valid identifier\n",
			hold_args[i]);
		return (-1);
	}
	while (hold_args[++i])
	{
		j = 0;
		while (hold_args[i][j])
		{
			if (ft_isdigit(hold_args[i][0]) == 1)
			{
				printf("minishell: export: `%s': not a valid identifier\n",
					hold_args[i]);
				return (-1);
			}
			j++;
		}
	}
	return (0);
}
