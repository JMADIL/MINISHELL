TARGET = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
MINIFLAGS = -lreadline

READLINE = $(shell brew --prefix readline)
CFLAGS = -Wall -Wextra -Werror -I$(READLINE)/include -fsanitize=address
MINIFLAGS =  -lreadline -L$(READLINE)/lib -g -Wl,-no_pie

SRCS=	$(TARGET).c 							\
	cleaning.c 									\
	./pars/errors.c 							\
	./pars/initenv.c 							\
	./pars/pars_env.c 							\
	./pars/ft_strtok.c 							\
	./pars/handling.c 							\
	./pars/utils.c 								\
	./pars/ft_free.c 							\
	./pars/token.c 								\
	./pars/token_utils.c 						\
	./pars/parser.c 							\
	./pars/expansion_utils.c 					\
	./pars/expansion_utils2.c 					\
	./pars/expansion_utils3.c 					\
	./pars/quotes_checker.c	 					\
	./pars/nodes.c 								\
	./pars/heredoc.c 							\
	./pars/redirection.c 						\
	./pars/split_cmd.c 							\
	./exec/error1.c								\
	./exec/error2.c 							\
	./exec/executor.c 						    \
	./exec/heredoc_utils.c 						\
	./exec/heredoc_exec.c 						\
	./exec/parsing_split.c 						\
	./exec/pipex_utils1.c 						\
	./exec/env.c								\
	./exec/gnl/gnl.c 							\
	./exec/pipex_utils2.c 						\
	./exec/pipex.c 								\
	./exec/signals.c 							\
	./exec/signals2.c 							\
	./exec/utils.c 								\
	./exec/builtins/builtin_cd.c 				\
	./exec/builtins/builtin_echo.c 				\
	./exec/builtins/builtin_env.c 				\
	./exec/builtins/builtin_exit.c 				\
	./exec/builtins/builtin_export_utils.c 		\
	./exec/builtins/builtin_export_utils1.c 	\
	./exec/builtins/builtin_export.c 			\
	./exec/builtins/builtin_pwd.c 				\
	./exec/builtins/builtin_redirection_utils.c \
	./exec/builtins/builtin_redirection_utils1.c\
	./exec/builtins/builtin_redirection.c 		\
	./exec/builtins/builtin_unset.c 			\
	./exec/builtins/builtin_utils.c 			\
	./exec/builtins/builtin_utils1.c 			\
	./exec/builtins/builtin.c 					\

RM = rm -rf
NAME = $(TARGET).a
OBJS = $(SRCS:.c=.o)
HEADER = $(TARGET).h
LIBFT = ./exec/libft/libft.a
GET_NEXT_LINE = ./exec/gnl/get_next_line.h

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all: $(TARGET)

$(TARGET): $(NAME) $(HEADER) $(LIBFT)
	@echo "$(BLUE)Making $(TARGET) ...$(DEF_COLOR)"
	@${CC} ${CFLAGS} -o $(TARGET) $(TARGET).c $(NAME) $(LIBFT) ${MINIFLAGS}
	@echo "$(GRAY) ___  ____       _     _          _ _"
	@echo "$(GRAY) |  \/  |_|     |_|   | |        | | |"
	@echo "$(GRAY) | .  . |_ _ __  _ ___| |__   ___| | |"
	@echo "$(GRAY) | |\/| | | '_ \| / __| '_ \ / _ \ | |"
	@echo "$(GRAY) | |  | | | | | | \__ \ | | |  __/ | |"
	@echo "$(GRAY) \_|  |_/_|_| |_|_|___/_| |_|\___|_|_|"
	@echo "$(GRAY)\n==The Executable of [$(TARGET)] has been compiled!==\n$(DEF_COLOR)"
	@echo "$(YELLOW)\n=> Usage: ./$(TARGET)\n$(DEF_COLOR)"

$(NAME): $(OBJS)
	@echo "$(BLUE)Archiving the Library $(NAME) ...$(DEF_COLOR)"
	@ar rcs $(NAME) $^

%.o: %.c $(HEADER) $(GET_NEXT_LINE)
	@echo "$(BLUE)Compiling $< ...$(DEF_COLOR)"
	@${CC} ${CFLAGS} -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Compiling the Library LIBFT ...$(DEF_COLOR)"
	@make -C ./exec/libft

clean:
	@echo "$(MAGENTA)Cleaning minishell objects ...$(DEF_COLOR)"
	@$(RM) $(OBJS)
	@make -C ./exec/libft clean
	@echo "$(MAGENTA)Cleaning libft objects ...$(DEF_COLOR)"

fclean: clean
	@echo "$(RED)Cleaning $(LIBFT) ...$(DEF_COLOR)"
	@echo "$(RED)Cleaning $(NAME) and $(TARGET) ...$(DEF_COLOR)"
	@$(RM) $(NAME) $(TARGET)
	@make -C ./exec/libft fclean
	@rm -rf minishell.dSYM/

re: fclean all

.PHONY: all clean fclean re
