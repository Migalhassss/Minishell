/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 18:00:31 by micarrel          #+#    #+#             */
/*   Updated: 2023/11/02 17:41:36 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*env_replace_utils(t_utils_hold *utils_hold, int i,
		bool in_quotes, char *var_name)
{
	char	*var_value;

	while (utils_hold->args[i] != '\0')
	{
		if (utils_hold->args[i] == '\"')
			in_quotes = !in_quotes;
		if (utils_hold->args[i] == '\'' && in_quotes == false)
		{
			i++;
			while (utils_hold->args[i] != '\'')
				i++;
		}
		if (utils_hold->args[i] == '$'
			&& check_if_onlydollar(utils_hold->args, i) == 1)
		{
			var_name = take_var_name(utils_hold->args, i);
			var_value = get_env_value(var_name, utils_hold);
			utils_hold->args = detect_dollar_sigs(utils_hold->args,
					var_value, var_name, &i);
			free(var_name);
			i--;
		}
		i++;
	}
	return (utils_hold->args[i] = '\0', utils_hold->args);
}

char	*replace_env_vars(t_utils_hold *utils_hold)
{
	int		i;
	char	*var_name;
	bool	in_quotes;
	char	*tmp;

	if (ft_strchr(utils_hold->args, '$') == NULL)
		return (utils_hold->args);
	var_name = NULL;
	i = 0;
	in_quotes = false;
	utils_hold->args = env_replace_utils(utils_hold, i,
			in_quotes, var_name);
	tmp = ft_strtrim(utils_hold->args, " ");
	free(utils_hold->args);
	utils_hold->args = tmp;
	return (utils_hold->args);
}
