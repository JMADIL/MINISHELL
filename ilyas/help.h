
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

// Exit codes
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_MISUSE 2

typedef enum e_token_type
{
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}						t_token_type;

//--structs
typedef struct s_list_heredoc
{
	char				*delimiter;
	char				*line;
	int					fd;
}						t_list_heredoc;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	bool				addspace;
	bool				variable;
	struct s_token		*current;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_lexer
{
	char				*input;
	int					pos;
	int					len;
}						t_lexer;

typedef struct s_redi_list
{
	t_token_type		type;
	char				*file;
	char				*content;
	bool				is_last;
	int					tmp_fd;
	bool				is_ambiguous;
	bool				expand;
	bool				variable;
	int					heredoc_fd;
	struct s_redi_list	*next;
}						t_redi_list;

typedef struct s_cmdarg
{
	char				**cmd;
	int					cmdsize;
	int					cmd_capacity;
	bool				is_builtin;
	int					origin_stdout;
	int					origin_stdin;
	t_redi_list			*redirections;
	t_redi_list			*input;
	t_redi_list			*output;
	struct s_cmdarg		*next;
}						t_cmdarg;

typedef struct s_list
{
	char			*key;
	char			*value;
	int				check;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

// Shell state struct
typedef struct s_shell {
    t_list           *env;       // Environment variables
    int             last_status;// Last command exit status
    bool            running;    // Shell running state
    char            *pwd;       // Current working directory
    char            *old_pwd;   // Previous working directory
}   t_shell;



extern int g_exit_status;

#endif

//functions ajemi
char *ft_strdup(char *str);
int ft_strlen(char *l);
int	is_vaid_n_flag(const char *str);
char	*join_args_from_index(char **cmd, int i);
void prent_echo_output(const char *tmp, int n_flag);
int builtin_echo(char **cmd);
char *get_path_value(t_list *env);
int add_env_node(t_list **env, char *key, char *value);
t_list *find_env_var(const char *key, t_list *env);