
NAME		= minishell

FILES	=	main.c 							\
			error/error.c					\
			builtins/echo.c					\
			builtins/exit.c					\
			builtins/pwd.c					\
			builtins/env.c					\
			builtins/export.c				\
			builtins/unset.c				\
			builtins/cd.c					\
			utils/utils.c					\
			utils/parse_envp.c				\
			utils/minishell_loop.c			\
			utils/execute.c					\
			utils/execute_utils1.c			\
			utils/execute_utils2.c			\
			utils/execute_heredoc.c			\
			utils/execute_heredoc_utils1.c	\
			parser/parser.c					\
			lexer/token_reader.c			\
			#lexer/handle_token.c			\
			lexer/handle_quotes.c			\

OBJS		=	$(FILES:.c=.o)

READLINE_DIR = $(shell brew --prefix readline)
READLINE_LIB = -lreadline -lhistory -L $(READLINE_DIR)/lib

RM	= rm -f

CC	= gcc

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