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

// Token types for lexer/parser
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC,
    TOKEN_EOF
}   t_token_type;

// Token struct
typedef struct s_token {
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

// Linked list for environment variables
typedef struct s_env {
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

// Redirection types
typedef enum e_redir_type {
    REDIR_NONE,
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
}   t_redir_type;

// Redirection struct
typedef struct s_redir {
    t_redir_type     type;
    char            *filename;
    struct s_redir  *next;
}   t_redir;

// Command struct
typedef struct s_cmd {
    char            **argv;      // Command and arguments
    t_redir         *redirs;    // Redirections
    struct s_cmd    *next;      // For pipelines
}   t_cmd;

// Shell state struct
typedef struct s_shell {
    t_env           *env;       // Environment variables
    int             last_status;// Last command exit status
    bool            running;    // Shell running state
    char            *pwd;       // Current working directory
    char            *old_pwd;   // Previous working directory
}   t_shell;

// AST node types
typedef enum e_node_type {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_SUBSHELL
}   t_node_type;

// AST node for complex command parsing
typedef struct s_ast_node {
    t_node_type         type;
    t_cmd              *cmd;        // For command nodes
    struct s_ast_node  *left;      // Left child
    struct s_ast_node  *right;     // Right child
}   t_ast_node;

// For signal handling
typedef struct s_signal {
    int     sigint_received;
    int     sigquit_received;
    pid_t   child_pid;
}   t_signal;

// For heredoc handling
typedef struct s_heredoc {
    char    *delimiter;
    char    *content;
    int     fd[2];  // pipe file descriptors
}   t_heredoc;

// Builtin function pointer
typedef int (*t_builtin_func)(t_shell *shell, char **args);

// Builtin command struct
typedef struct s_builtin {
    char            *name;
    t_builtin_func  func; 
}   t_builtin;

extern int g_exit_status;

#endif



//functions ajemi
char *ft_strdup(char *str);
int ft_strlen(char *l);
int	is_vaid_n_flag(const char *str);
char	*join_args_from_index(char **cmd, int i);
void prent_echo_output(const char *tmp, int n_flag);
int builtin_echo(char **cmd);
char *get_path_value(t_env *env);