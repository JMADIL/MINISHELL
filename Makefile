CC = cc
NAME = minishell
CFLAGS = -Wall -Wextra -Werror -I$(shell brew --prefix readline)/include -g3 -fsanitize=address
LDFLAGS = -lreadline -L$(shell brew --prefix readline)/lib

SRCS = minishell.c \
       cleaning.c \
       pars/errors.c \
       pars/initenv.c \
       pars/pars_env.c \
       pars/ft_strtok.c \
       pars/handling.c \
       pars/utils.c \
       pars/ft_free.c \
       pars/token.c \
       pars/token_utils.c \
       pars/parser.c \
       pars/expansion_utils.c \
       pars/expansion_utils2.c \
       pars/expansion_utils3.c \
       pars/quotes_checker.c \
       pars/nodes.c \
       pars/heredoc.c \
       pars/redirection.c \
       pars/split_cmd.c \
       exec/error1.c \
       exec/error2.c \
       exec/executor.c \
       exec/heredoc_utils.c \
       exec/heredoc_exec.c \
       exec/parsing_split.c \
       exec/pipex_utils1.c \
       exec/env.c \
       exec/gnl/gnl.c \
       exec/pipex_utils2.c \
       exec/pipex.c \
       exec/signals.c \
       exec/signals2.c \
       exec/utils.c \
       exec/builtins/builtin_cd.c \
       exec/builtins/builtin_echo.c \
       exec/builtins/builtin_env.c \
       exec/builtins/builtin_exit.c \
       exec/builtins/builtin_export_utils.c \
       exec/builtins/builtin_export_utils1.c \
       exec/builtins/builtin_export.c \
       exec/builtins/builtin_pwd.c \
       exec/builtins/builtin_redirection_utils.c \
       exec/builtins/builtin_redirection_utils1.c \
       exec/builtins/builtin_redirection.c \
       exec/builtins/builtin_unset.c \
       exec/builtins/builtin_utils.c \
       exec/builtins/builtin_utils1.c \
       exec/builtins/builtin.c\
       exec/builtins/io.c\
	exec/create_tmp_heredoc.c

OBJS = $(SRCS:.c=.o)
LIBFT = exec/libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C exec/libft

clean:
	rm -f $(OBJS)
	make -C exec/libft clean

fclean: clean
	rm -f $(NAME)
	make -C exec/libft fclean
	rm -rf minishell.dSYM/

re: fclean all

.PHONY: all clean fclean re