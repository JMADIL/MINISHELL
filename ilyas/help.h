
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
#include <sys/stat.h>

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

//builtin_cd
void update_pwd_vars(t_list *env, char *new_path, char *old_path);
char *expand_tilde(char *cmd, t_list *env);
int resolve_cd_target(char **cmd, char **path, char *old_path, t_list **env);
int builtin_cd(char **cmd, t_list **env);
//builtin_echo
int	is_vaid_n_flag(const char *str);
char	*join_args_from_index(char **cmd, int i);
void prent_echo_output(const char *tmp, int n_flag);
int builtin_echo(char **cmd);
//builtin_env
int builtin_env(t_list **env);
//builtin_exit
int numeric(const char *str);
int builtin_exit(char **cmd, t_list **env);
//builtin_export_utils
void	print_export_format(t_list *head);
void	print_sorted_export(t_list *env);
void swap_env_nodes(t_list *ptr1);
void	update_existing_env(t_list *dup_key, char *key, char *value, char *cmd);
//builtin_export_utils1
int	print_export_identifier_error(const char *cmd);
void	handle_export_no_assign(char **key, char **value, const char *cmd);
void	append_to_env_value(t_list *dup_key, char *key, char *value);
int	parse_export_argument(const char *cmd, char **key, char **value);
//builtin_export
t_list *find_env_var(const char *key, t_list *env);
// static void process_export_variable(const char *cmd, char **key, char **value, t_list **env);
int validate_export_identifier(const char *cmd);
int	builtin_export(char **cmd, t_list **env);
//builtin_pwd	
int ft_pwd(t_list *env);
//builtin_redirection_utils
int	open_file_for_builtin(char *filename, t_token_type type);
void	restore_original_fds(t_cmdarg *cmd);
void	display_redi_error(char *filename, char *error_msg);
void	handle_file_open_error(char *filename);
int	process_all_redirections(t_redi_list *redi_list);
bool	check_ambiguous_redirect(char *filename);
int	count_commands(t_cmdarg *cmd_list);
int	check_if_builtin(char *cmd);
int	execute_builtin_command(t_cmdarg *cmd, t_list **env);
void	print_error(char *msg);
//builtin_redirection
int	handle_append_redi(t_redi_list *redi);
int	handle_output_redi(t_redi_list *redi);
int	handle_input_redi(t_redi_list *redi);
int	setup_builtin_redirections(t_cmdarg *cmd);
int	execute_builtin_with_redi(t_cmdarg *cmd_list, t_list **env);
//builtin_unset
int	remove_env_node(t_list **env_list, t_list *node);
int	init_minimal_env(t_list **env);
int	builtin_unset(char **cmd, t_list **env);
//builtin_utils
int add_env_node(t_list **env, char *key, char *value);
char *ft_strdup(char *str);
int ft_strlen(char *l);
t_list *copy_env_list(t_list *env);
void alloc_key_value(char *cmd, char **key, char **value, char *equals_pos);
t_list	*find_node(t_list *env, char *key);
int	size_dp(char **c);
//builtin
void	free_arr(char **cmd);
int is_builtin(char *cmd);
int exec_builtin(t_cmdarg *shell, t_list **env);

//out the builtin directory i have this 

//env.c
void free_env_list(t_list **env);
char *get_path_value(t_list *env);
char	*check_exec(char *p, t_list *env, int *no_file);
//exec_cmd.c
char *search_command_in_path(char *cmd, char *path_value);
char *resolve_command_path(char *cmd, t_list *env);
static void xec_external_command(t_cmdarg *current_cmd, char **env);//I WILL NOT USE THIS
//errors.c
void	safe_free(char *s);
void	print_error_exit(const char *cmd_name, const char *error, int status);
int	safe_open(const char *file, int flag);
void	execve_error_cleanup(char **cmd_path, char **cmd_name, char **envp);

void	finish_exec(pid_t last_cmd_pid);

//pipex_utils.c
void	cmd_not_found_exit(t_cmdarg *curr_cmd, int no_file);
void	handle_heredoc_input(t_redi_list *input);
static int	open_redir_file(const char *filename, int mode);
int	handle_append_output(t_redi_list *output);
int	process_output_redirections(t_redi_list *output);
int	process_input_redirections(t_redi_list *input);
char	*validate_exec_path(char *p);
void	exec_malloc_fail(char *cmd_path, char *cmd_name);

//pipex.c
bool	is_directory(const char *path);
static void	exec_external_command(t_cmdarg *current_cmd, t_list *env);
void	exec_builtin_in_child(t_cmdarg *current_cmd, t_list **env);
void	exec_child_process(t_cmdarg *current_cmd, t_list *env, int tmp_in,
		int p_fd[2]);

//signals.c
void	sigint_interactive(int sig);
void	sigint_parent_wait(int sig);
void	sigint_heredoc(int sig);
void	sigint_heredoc_child(int sig);
void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);
void	restore_interactive_signals(void);
void	setup_child_signals(void);
void	setup_parent_wait_signals(void);
void	setup_parent_heredoc_signals(void);
//gnl
char		*get_next_line(int fd);
char		*my_strjoin(char *s1, char *s2);
char		*my_strdup(const char *s1);

//heredoc_utils.c
t_redi_list	*get_last_input_redirection(t_redi_list *redi);
t_redi_list	*get_last_output_redirection(t_redi_list *redi);
void	init_redirection_metadata(t_cmdarg *cmd);
int	is_heredoc_end(char *line, const char *delimiter);
void	read_heredoc_input_gnl(char *delim, int fd_pipe[2], t_redi_list *heredoc, t_list *env);

//utils.c
int	size_list(t_cmdarg *node);
void	free_all(char **bf, int j);
void	ft_alloc(char **envp, int *i, char *key_equals, t_list *env);
char	**get_env(t_list *env);
void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd);