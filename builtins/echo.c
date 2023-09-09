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

char	*check_user(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "USER=", 5))
			return (ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5));
		i++;
	}
	return (ft_strdup("\0"));
}

void	ft_spaxe(t_utils_hold *utils_hold, int *i)
{
	if (utils_hold->args[*i] == ' ')
	{
		while (utils_hold->args[*i] == ' ')
			(*i)++;
		ft_putchar_fd(' ', 1);
	}
}

void	do_user(t_utils_hold *utils_hold, int *i)
{
	char	*user;

	user = check_user(utils_hold->envp);
	ft_putchar_fd('\'', 1);
	ft_putstr_fd(user, 1);
	ft_putchar_fd('\'', 1);
	free(user);
	(*i) += 8;
}

void	ft_skipti(t_utils_hold *utils_hold, int *i)
{
	while (utils_hold->args[*i] && utils_hold->args[*i] != ' ' 
		&& utils_hold->args[*i] != '\'' && utils_hold->args[*i] != '"' 
		&& utils_hold->args[*i + 1] != '$')
		(*i)++;
}

void	ft_make_invaux(t_utils_hold *utils_hold, int *i)
{
	ft_skipti(utils_hold, i);
	if (utils_hold->args[*i] == ' ' && utils_hold->args[*i + 1] == '$')
	{
		ft_putchar_fd(' ', 1);
		(*i)++;
	}
	else if(utils_hold->args[*i + 1] == '$')
		(*i)++;
	if (utils_hold->args[*i] != '\0' && utils_hold->args[*i] == ' ')
	{
		while (utils_hold->args[*i] && utils_hold->args[*i] == ' ')
		{
			ft_putchar_fd(' ', 1);
			(*i)++;
		}
		(*i)--;
	}
	else if (utils_hold->args[*i] != '\0' && utils_hold->args[*i] == '$')
	{
		ft_putchar_fd('$', 1);
		(*i)++;
	}
}

void	print_env(t_utils_hold *utils_hold, int *i)
{
	int		j;
	char 	*env_value;
	char	*tmp;

	j = *i + 1;
	while (utils_hold->args[j] && utils_hold->args[j] != ' '
			&& utils_hold->args[j] != '\'' && utils_hold->args[j] != '"')
		j++;
	env_value = ft_substr(utils_hold->args, *i + 1, j - 1);
	tmp = get_env_value(env_value, utils_hold->envp);
	ft_putstr_fd(tmp, 1);
	(*i) += ft_strlen(env_value);
	free(tmp);
	free(env_value);
}

void	ft_make_invisible_2(t_utils_hold *utils_hold, int *i)
{
	while (utils_hold->args[*i] && utils_hold->args[*i] != '"')
	{
		// if (utils_hold->args[*i] == '$' && utils_hold->args[*i + 1] != ' ')
		// 	print_env(utils_hold, i);
		while (utils_hold->args[*i] == ' ')
		{
			ft_putchar_fd(' ', 1);
			(*i)++;
		}
		if (utils_hold->args[*i] == '"')
			return  ;
		if (utils_hold->args[*i] == '$' && (utils_hold->args[*i + 1] == '"'
				|| utils_hold->args[*i + 1] == ' '))
		{
			ft_putchar_fd('$', 1);
			(*i)++;
		}
		else if (utils_hold->args[*i] == '$')
			ft_make_invaux(utils_hold, i);
		else
		{
			ft_putchar_fd(utils_hold->args[*i], 1);
			(*i)++;
		}
	}
}

// void	ft_make_expand_error(t_utils_hold *utils_hold, int *i)
// {
// 	char	*exit_str;

// 	if (utils_hold->args[*i] == '$' && utils_hold->args[*i + 1] == '?')
// 	{
// 		exit_str = ft_itoa(g_global.exit_code);
// 		(*i)++;
// 		ft_putstr_fd(exit_str, 1);
// 		free(exit_str);
// 		g_global.exit_code = 0;
// 	}
// }

void	ft_make_dollar(t_utils_hold *utils_hold, int *i, int flag)
{
	if (flag == 1)
		ft_make_invisible_2(utils_hold, i);
	else if (utils_hold->args[*i] == '$' && utils_hold->args[*i + 1] == ' ')
		ft_putchar_fd('$', 1);
	else if (utils_hold->args[*i] == '$' && (utils_hold->args[*i + 1] == '\''
			|| utils_hold->args[*i + 1] == '"'))
		(*i)++;
	else if (utils_hold->args[*i] == '$' && utils_hold->args[*i + 1] == '\0')
		ft_putchar_fd('$', 1);
	else if (utils_hold->args[*i] == '$' && utils_hold->args[*i + 1] != ' ')
		print_env(utils_hold, i);
	else
		return ;
}

void	ft_make_quote(t_utils_hold *utils_hold, int *i)
{
	(*i)++;
	while (utils_hold->args[*i] && utils_hold->args[*i] != '\'' && utils_hold->args[*i] != '"')
	{
		if (utils_hold->args[*i - 1] == '\'')
		{
			while (utils_hold->args[*i] != '\'' && utils_hold->args[*i])
			{
				ft_putchar_fd(utils_hold->args[*i], 1);
				(*i)++;
			}
			return  ;
		}
		else if (utils_hold->args[*i - 1] == '\"')
		{
			while (utils_hold->args[*i] != '"' && utils_hold->args[*i])
			{
				if (utils_hold->args[*i] == '$')
				{
					ft_make_dollar(utils_hold, i, 1);
					return  ;
				}
				ft_putchar_fd(utils_hold->args[*i], 1);
				(*i)++;
			}
			return  ;
		}
		(*i)++;
	}
}

void	ft_user(t_utils_hold *utils_hold, int i)
{
	while (utils_hold->args[i])
	{
		ft_spaxe(utils_hold, &i);
		if (!ft_strncmp(utils_hold->args + i + 1, "'$USER'", 7))
			do_user(utils_hold, &i);
		else if (!ft_strncmp(utils_hold->args + i, "\'\"$USER\"\'", 9))
		{
			ft_putstr_fd("\"$USER\"", 1);
			i += 8;
		}
		else if (utils_hold->args[i] == '$')
			ft_make_dollar(utils_hold, &i, 0);
		else if (utils_hold->args[i] == '\'' || utils_hold->args[i] == '"')
			ft_make_quote(utils_hold, &i);
		else
		{
			if (utils_hold->args[i] != '\'')
				ft_putchar_fd(utils_hold->args[i], 1);
		}
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
	while (utils_hold->args[i] == ' ')
		i++;
	ft_user(utils_hold, i);
	if (n_flag == -1)
		ft_putchar_fd('\n', 1);
	return (0);
}
