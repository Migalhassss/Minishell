/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 19:08:04 by micarrel          #+#    #+#             */
/*   Updated: 2023/11/02 19:08:22 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup_2(const char *src)
{
	char	*dest;
	int		i;
	int		len;

	len = 0;
	i = 0;
	dest = NULL;
	while (src[i] != '\0')
	{
		if (src[i] != '"' && src[i] != '\'')
			len++;
		i++;
	}
	dest = (char *)malloc(len * sizeof(char) + 1);
	if (!dest)
		return (0);
	i = 0;
	while (src[i] != '\0')
	{
		if (src[i] != '"' && src[i] != '\'')
			dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	check_args(char *hold_args)
{
	int	i;

	i = 0;
	if (hold_args[0] == '_')
		;
	else if (hold_args[0] == '=' || ft_isdigit(hold_args[0]) == 1
		|| ft_isalpha(hold_args[0]) == 0)
	{
		printf("minishell: export: `%s': not a valid identifier\n", hold_args);
		return (-1);
	}
	i++;
	while (hold_args[i])
	{
		if (ft_isalnum2(hold_args) == 0)
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				hold_args);
			return (-1);
		}
		i++;
	}
	return (0);
}
