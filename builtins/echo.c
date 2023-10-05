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

// void	print_env(t_utils_hold *utils_hold, int *i)
// {
// 	int		j;
// 	char 	*env_value;
// 	char	*tmp;

// 	j = *i + 1;
// 	while (utils_hold->args[j] && utils_hold->args[j] != ' '
// 			&& utils_hold->args[j] != '\'' && utils_hold->args[j] != '"')
// 		j++;
// 	env_value = ft_substr(utils_hold->args, *i + 1, j - 1);
// 	tmp = get_env_value(env_value, utils_hold->envp);
// 	ft_putstr_fd(tmp, 1);
// 	(*i) += ft_strlen(env_value);
// 	free(tmp);
// 	free(env_value);
// }


void	printnopipe(char	*str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
			return ;
		else if (str[i] == '<')
			return ;
		else if (str[i] == '>')
			return ;
		ft_putchar_fd(str[i], 1);
		i++;
	}
}

void	print_args(char	*args)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = ft_split(args, ' ');
	while (tmp[i])
	{
		if (ft_strchr(tmp[i], '|') || ft_strchr(tmp[i], '>') || ft_strchr(tmp[i], '<'))
		{
			printnopipe(tmp[i]);
			return ;
		}
		ft_putstr_fd(tmp[i], 1);
		if (tmp[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	free_array(tmp);
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
