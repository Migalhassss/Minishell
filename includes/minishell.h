/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 02:20:49 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/22 02:20:49 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct s_lexer
{
	char			*str;
	int				i;
	int				token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_parser_utils
{
	t_lexer				*lexer_list;
	t_lexer				*redirections;
	int					num_redirections;
	struct s_utils_hold	*utils;
}	t_parser_utils;

typedef struct s_utils_hold
{
	char					*args;
	char					*cmd;
	char					**paths;
	char					**envp;
	struct s_simple_cmds	*simple_cmds;
	t_lexer					*lexer_list;
	char					*pwd;
	char					*old_pwd;
	int						pipes;
	int						*pid;
	bool					heredoc;
	bool					reset;
	int						index_list;
	int						exit_code;
}	t_utils_hold;

typedef struct s_simple_cmds
{
	char					**str;
	int						builtin;
	int						num_redirections;
	char					*hd_file_name;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

typedef struct s_global
{
	int	in_cmd;
	int	in_heredoc;
	int	stop_heredoc;
}	t_global;

extern t_global	g_global;

void			ft_lstclear2(t_lexer **lst);
void			*ft_lstclear_cmd(t_lexer **lst);

// ------------------UTILS_MAIN------------------//
int				ft_strcmp(const char *s1, const char *s2);
char			*check_path(char **envp);
void			path_find(t_utils_hold *utils_hold);
void			free_array(char **split_array);
// ------------------INT_MAIN--------------------//
void			sigint_handler(int sig_num);
char			**ft_envpdup(char **envp);
int				pwd_find(t_utils_hold *utils_hold);
int				implement_utils_hold(t_utils_hold *utils_hold);
// ----------------------------------------------//

void			print_list(t_lexer *lexer_list);
//------------------MINISHELL_LOOP---------------//
int				minishell_loop(t_utils_hold *utils_hold);
int				reset_utils_hold(t_utils_hold *utils_hold);
int				count_quotes(t_utils_hold *utils_hold);
int				find_matching_quote(char *line, int i, int *num_del, int del);
int				token_reader(t_utils_hold *utils_hold);
int				handle_token(t_utils_hold *utils_hold, int i,
					t_lexer **lexer_list);
int				add_node(char *str, int token, t_lexer **lexer_list,
					t_utils_hold *utils_hold);
// ----------------------------------------------//

// ------------------PARSER----------------------//
int				parser(t_utils_hold *utils_hold);
t_lexer			*lexer_clear_one(t_lexer **lexer_list);
void			lexer_delete_first(t_lexer **lexer_list);
void			lexer_delete_one(t_lexer **lexer_list, int index_list);
int				check_error(t_utils_hold *utils_hold);
t_parser_utils	init_parser(t_lexer *lexer_list, t_utils_hold *utils_hold);
t_simple_cmds	*init_cmd(t_parser_utils *parser_utils);
void			rm_redirections(t_parser_utils *parser_utils);
int				add_redirection(t_lexer *tmp, t_parser_utils *parser_utils);
t_lexer			*ft_newlexer(char *str, int token);
void			lexeradd_back(t_lexer **lst, t_lexer *new);
int				count_args(t_lexer *lexerlist);
t_simple_cmds	*ft_simple_cmdsnew(char **str, int num_redirections,
					t_lexer *redirections);
void			simple_cmdsadd_back(t_simple_cmds **lst, t_simple_cmds *new);
// ----------------------------------------------//

// ------------------SINGLE_CMD------------------//
int				check_builtins(t_utils_hold *utils_hold);
char			*take_command_to_check(char *args);
char			*which_command(t_utils_hold *utils_hold);
char			*take_command(t_utils_hold *utils_hold);
void			execute_command(char *cmd, t_utils_hold *utils_hold);
void			handle_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold);
int				find_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold);
char			**resplit_str(char **double_arr);
char			*join_split_str(char **split_str, char *new_str);
int				cmd_not_found(char *str, t_utils_hold *utils_hold);
// ----------------------------------------------//

//------------------EXECUTOR---------------------//
void			free_tmp(t_utils_hold *utils_tmp);
int				send_heredoc(t_utils_hold *utils_hold, t_simple_cmds *cmd);
char			*generate_heredoc_filename(void);
int				ft_heredoc(t_utils_hold *utils_hold, t_lexer *heredoc,
					char *file_name);
char			*delete_quotes(char *str, char c);
int				create_heredoc(bool quotes,
					t_lexer *tmp, char *file_name);
int				check_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold);
char			*check_dollar_sign(t_utils_hold *utils_hold, char *str);
size_t			dollar_sign(char *str);
int				handle_digit_after_dollar(int j, char *str);
int				question_mark(char **tmp);
int				loop_if_dollar_sign(t_utils_hold *utils_hold,
					char *str, char **tmp, int j);
size_t			equal_sign(char *str);
int				after_dol_lenght(char *str, int j);
char			*char_to_str(char c);
// --------------After send here_doc-------------//
int				ft_fork(t_utils_hold *utils_hold, int end[2],
					int fd_in, t_simple_cmds *cmd);
void			dup_cmd(t_simple_cmds *cmd,
					t_utils_hold *utils_hold, int end[2], int fd_in);
int				check_fd_heredoc(t_utils_hold *utils_hold,
					int end[2], t_simple_cmds *cmd);
int				pipe_wait(int *pid, int amount, t_utils_hold *utils_hold);
t_simple_cmds	*ft_simple_cmdsfirst(t_simple_cmds *map);
// ----------------------------------------------//

// --------------PREPARE-EXECUTOR----------------//
int				prepare_executor(t_utils_hold *utils_hold);
void			single_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold);
int				ft_error(int error, t_utils_hold *utils_hold);
int				executor(t_utils_hold *utils_hold);
// ----------------------------------------------//

//--------------ECHO--------------;
int				ft_echo(t_utils_hold *utils_hold);
int				check_n(char *args);
void			ft_user(t_utils_hold *utils_hold, int i);
char			*check_user(char **envp);
void			ft_spaxe(t_utils_hold *utils_hold, int *i);
void			do_user(t_utils_hold *utils_hold, int *i);
void			ft_make_dollar(t_utils_hold *utils_hold, int *i, int flag);
void			ft_make_invisible_2(t_utils_hold *utils_hold, int *i);
void			ft_make_invisible(t_utils_hold *utils_hold, int *i);
void			ft_make_expand_error(t_utils_hold *utils_hold, int *i);
void			ft_make_quote(t_utils_hold *utils_hold, int *i);
// ------------------------------//

// --------------ENV--------------//
int				ft_env(t_utils_hold *utils_hold);
// ------------------------------//

// --------------CD---------------//
int				ft_cd(t_utils_hold *utils_hold);
int				cd_home(t_utils_hold *utils_hold);
int				ft_strcharcmp2(char *s1, const char *s2, char c);
void			update_pwd(t_utils_hold *utils_hold);
int				check_cd_args(t_utils_hold *utils_hold);
int				check_n_args(t_utils_hold *utils_hold);
int				cd_minus(t_utils_hold *utils_hold);
int				cd_dotdot(t_utils_hold *utils_hold);
int				cd_path(t_utils_hold *utils_hold);
// ------------------------------//

// --------------PWD--------------//
int				ft_pwd(t_utils_hold *utils_hold);
// ------------------------------//

// --------------EXPORT-----------//
int				ifexists_without_value(char *evnp, char *str);
int				ft_export(t_utils_hold *utils_hold);
int				check_args(char **hold_args);
int				check_if_exists_helper(char *str, t_utils_hold *utils_hold);
int				ft_strcharcmp(char *s1, const char *s2, char c);
void			update_envp(char *hold_args, t_utils_hold *utils_hold);
int				check_if_exists3(char *hold_arg, t_utils_hold *utils_hold);
int				check_if_exists_helper2(char *str, t_utils_hold *utils_hold);
int				ft_arrlen(char **arr);
void			ft_full_env(t_utils_hold *utils_hold);
char			*ft_strdup_2(const char *src);
void			add_envp(char *hold_args, t_utils_hold *utils_hold, int arrlen);
// ------------------------------//

// --------------UNSET------------//
int				ft_unset(t_utils_hold *utils_hold);
int				check_if_exists2(char **hold_args, t_utils_hold *utils_hold);
char			**dup_array(t_utils_hold *utils_hold, int j);
char			*ft_strdup2(const char *src, char c);
int			checkerror_identifier(char **hold_args);
// ------------------------------//

// --------------EXIT-------------//
int				ft_exit(t_utils_hold *utils_hold);
void			free_utils_hold(t_utils_hold *utils_hold);
void			ft_simple_cmdsclear(t_simple_cmds **lst);
// ------------------------------//
char			*get_env_value(char *env_name, t_utils_hold *utils_hold);
void			print_env(t_utils_hold *utils_hold, int *i);
int				check_redirections(t_simple_cmds *cmd, t_utils_hold *utils_hold	);
void			clean_exit(t_utils_hold *utils_hold, int exit_code);

/*
" | " = 1
" > " =  2
" < " = 3
" >> " = 4
" << " = 5
*/

#endif