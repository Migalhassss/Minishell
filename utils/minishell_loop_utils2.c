/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:59:36 by micarrel          #+#    #+#             */
/*   Updated: 2023/10/28 21:41:45 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*make_leaks_clean(char *tmp2, char *str, char *exit_code, int flag)
{
	char	*tmp;
	char	*tmp3;

	tmp = strndup(str, flag);
	free(str);
	tmp3 = ft_strjoin(tmp, exit_code);
	free(tmp);
	tmp = ft_strjoin(tmp3, tmp2);
	free(tmp2);
	free(tmp3);
	return (tmp);
}

char	*exit_code(t_utils_hold *utils_hold)
{
	char	*tmp;

	(void) utils_hold;
	tmp = ft_itoa(utils_hold->exit_code);
	return (tmp);
}

char	*take_var_name(char *args, int i)
{
	char	*var_name;
	int		j;

	j = 0;
	i++;
	var_name = malloc(ft_strlen(args) + 1);
	if (ft_isdigit(args[i]) == 1)
	{
		var_name[j] = args[i];
		var_name[j + 1] = '\0';
		return (var_name);
	}
	while (args[i] != '\0' && args[i] != ' ' && args[i] != '$'
		&& args[i] != '\'' && args[i] != '\"')
		var_name[j++] = args[i++];
	var_name[j] = '\0';
	return (var_name);
}

char	*empty_env(char *args, char *var_name, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = ft_substr(args, 0, i);
	tmp2 = ft_substr(args, i + ft_strlen(var_name) + 1, ft_strlen(args));
	if (ft_strlen(tmp2) == 0)
	{
		free(tmp2);
		free(args);
		return (tmp);
	}
	tmp3 = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
	free(args);
	return (tmp3);
}

char	*update_args(char *args, char *var_value, char *var_name, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = ft_substr(args, 0, i);
	tmp2 = ft_strjoin(tmp, var_value);
	free(tmp);
	tmp = ft_substr(args, i + ft_strlen(var_name) + 1, ft_strlen(args));
	tmp3 = ft_strjoin(tmp2, tmp);
	free(tmp);
	free(tmp2);
	free(args);
	return (tmp3);
}
