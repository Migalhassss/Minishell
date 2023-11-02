
NAME		= minishell

FILES	=	main.c 							\
			error/error.c					\
			builtins/echo.c					\
			builtins/exit.c					\
			builtins/pwd.c					\
			builtins/env.c					\
			builtins/export.c				\
			builtins/export_utils.c			\
			builtins/export_utils2.c		\
			builtins/unset.c				\
			builtins/unset_utils.c			\
			builtins/cd.c					\
			builtins/cd_utils.c				\
			utils/utils.c					\
			utils/utils2.c					\
			utils/utils3.c					\
			utils/parse_envp.c				\
			utils/minishell_loop.c			\
			utils/minishell_loop_utils1.c	\
			utils/minishell_loop_utils2.c	\
			utils/minishell_loop_utils3.c	\
			utils/execute.c					\
			utils/execute_utils1.c			\
			utils/execute_utils2.c			\
			utils/execute_utils3.c			\
			utils/execute_utils4.c			\
			utils/execute_utils5.c			\
			utils/execute_heredoc.c			\
			utils/execute_heredoc_utils1.c	\
			parser/parser.c					\
			parser/parser_utils.c			\
			parser/parser_utils2.c			\
			lexer/token_reader.c			\
			lexer/token_utils.c				\
			lexer/handle_quotes.c			\

OBJS		=	$(FILES:.c=.o)

READLINE_DIR = $(shell brew --prefix readline)
READLINE_LIB = -lreadline -lhistory -L $(READLINE_DIR)/lib

RM	= rm -f

CC	= cc

CFLAGS	= -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT_PATH	= ./libft --no-print-directory
LIBFT_NAME	= ./libft/libft.a

all: $(LIBFT_NAME) $(NAME)


$(LIBFT_NAME):
	@make -C $(LIBFT_PATH)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_LIB) $(LIBFT_NAME)
clean:
		@make fclean -C $(LIBFT_PATH)
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all