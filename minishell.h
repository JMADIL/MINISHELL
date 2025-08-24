/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irfei <irfei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:45 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/24 06:41:47 by irfei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "exec/libft/libft.h"
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

//macros
#define CD_HOME_ERROR "minishell: cd: HOME not set\n"
#define CD_OLDPWD_ERROR "minishell: cd: OLDPWD not set\n"
#define CD_ERROR_RETURN(old_path, msg) \
	return (free(old_path), write(2, msg, ft_strlen(msg)), 1)
	

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
	char				*delim;
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

// Shell state struct
typedef struct s_shell {
    t_list           *env;       
    int             last_status;
    bool            running;   
    char            *pwd;       
    char            *old_pwd;
}   t_shell;



extern int g_exit_status;


//heredoc
int create_tmp_heredoc(void);

//gnl
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
#endif

//execution
int	execution(t_cmdarg *current_cmd, t_list *env);
void	wait_for_all_children(int *status, pid_t last_pid);
void	handle_parent_fd(int *tmp_in, int pip_fd[2], t_cmdarg *current_cmd);
int	create_pipe_safe(int pip[2]);
int	ft_fork_and_exec(t_cmdarg *cmd, t_list *env, int *tmp_in,
						pid_t *last_cmd_pid);

//builtin_cd
void update_pwd_vars(t_list *env, char *new_path, char *old_path);
char *expand_tilde(char *cmd, t_list *env);
int resolve_cd_target(char **cmd, char **path, char *old_path, t_list **env);
int builtin_cd(char **cmd, t_list **env);
//builtin_echo
int	is_valid_n_flag(const char *str);
char	*join_args_from_index(char **cmd, int i);
void print_echo_output(char *tmp, int n_flag, int out);
int builtin_echo(char **cmd, t_cmdarg *shell, int out);
//builtin_env
int builtin_env(t_list **env, int out);
//builtin_exit
int numeric(const char *str);
int builtin_exit(char **cmd, t_list **env);
//builtin_export_utils
void	print_export_format(t_list *head);
void	print_sorted_export(t_list **env);
void swap_env_nodes(t_list *ptr1);
void	update_existing_env(t_list *dup_key, char **key, char **value,
		char *cmd);
//builtin_export_utils1
int	print_export_identifier_error(const char *cmd);
void	handle_export_no_assign(char **key, char **value, const char *cmd);
void	append_to_env_value(t_list *dup_key, char **key, char **value);
int	parse_export_argument(const char *cmd, char **key, char **value);
//builtin_export
t_list *find_env_var(const char *key, t_list *env);
// static void process_export_variable(const char *cmd, char **key, char **value, t_list **env);
int validate_export_identifier(const char *cmd);
int	builtin_export(char **cmd, t_list **env);
//builtin_pwd	
int builtin_pwd(t_list **env, int out);
//builtin_redirection_utils
int	open_file_for_builtin(char *filename, t_token_type type);
void	restore_original_fds(t_cmdarg *cmd);
void	display_redi_error(char *filename, char *error_msg);
void	handle_file_open_error(char *filename);
int	process_all_redirections(t_redi_list *redi_list);
bool	check_ambiguous_redirect(char *filename);// hadi blast is ambiguous
bool	dontexpand_heredoc_del(t_token *tmp);
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

//errors.c
void	safe_free(char *s);
void	print_error_exit(const char *cmd_name, const char *error, int status);
int	safe_open(const char *file, int flag);
void	execve_error_cleanup(char **cmd_path, char **cmd_name, char **envp);

void	finish_exec(pid_t last_cmd_pid);

//pipex_utils.c
void	cmd_not_found_exit(t_cmdarg *curr_cmd, int no_file);
void	handle_heredoc_input(t_redi_list *input);
int	open_redir_file(const char *filename, int mode);
int	handle_append_output(t_redi_list *output);
void	process_output_redirections(t_redi_list *output);
void	process_input_redirections(t_redi_list *input);
char	*validate_exec_path(char *p);
void	exec_malloc_fail(char *cmd_path, char *cmd_name);

//pipex.c
bool	is_directory(const char *path);
void	exec_external_command(t_cmdarg *current_cmd, t_list *env);
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


//heredoc_utils.c
t_redi_list	*get_last_input_redirection(t_redi_list *redi);
t_redi_list	*get_last_output_redirection(t_redi_list *redi);
void	init_redirection_metadata(t_cmdarg *cmd);
int	is_heredoc_end(char *line, const char *delimiter);
void	read_heredoc_input_gnl(char *delim, int fd_pipe[2], t_redi_list *heredoc, t_list *env);

//heredoc.c
int	heredoc_child_process(t_redi_list *heredoc, int fd_pipe[2], t_list *env);
void	heredoc_parent_finalize(int fd_pipe[2], pid_t pid, int *status,
		t_redi_list *in);
int	handle_single_heredoc(t_redi_list *in, int fd_pipe[2], t_list *env);
int	process_all_heredocs(t_cmdarg *shell, t_list *env);

//utils.c
int	size_list(t_cmdarg *node);
void	free_all(char **bf, int j);
void	ft_alloc(char **envp, int *i, char *key_equals, t_list *env);
char	**get_env(t_list *env);
void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd);

//parsing_split.c
char	**split_with_braces(const char *s, char sep);

// parsing
bool	ft_redierrors(t_token	*token);
bool	ft_pipeerrors(t_token *token);
void	expand_norm(t_token **token, t_list	*minienv, ssize_t dpos);
void	expand_exit_status(t_token **token);
void	expand_variables(t_token **token, t_list *minienv);
bool	ft_condition(t_token **token, ssize_t dollar_pos);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_expand(t_token **token, t_list	*minienv, ssize_t dollat_pos);
char	*ft_getenv(char *var, t_list *minienv);
char	*ft_getvar(char *var, t_list *minienv);
int	ft_get_varlen(char *str);
char	*help_tmp2(t_token **token, ssize_t dollar_pos);
bool	please_expand(t_token *tmp);
void	ft_free_token(t_token *token);
void	ft_free_tokenlist(t_token *token_list);
void	ft_free_cmdlist(t_cmdarg *cmdarg_list);
t_lexer *ft_init_lexer(char *input);
t_token *ft_strtok(char *input, t_list *minienv);
void	ft_init_fields(t_token *token);
void	ft_set_token_type(t_token *token, int op_len, char *op, bool *heredoc);
t_token	*handling_operator(t_lexer *lexer, bool *heredoc);
t_token	*handling_quotes(t_lexer *lexer, char quote);
t_token	*handling_words(t_lexer *lexer);
t_list	*ft_initenv(char **env);
t_cmdarg	*ft_init_node(void);
t_cmdarg	*get_next_node(t_token	*token_list);
t_cmdarg	*ft_newnode(t_cmdarg *node);
void	ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new);
bool	resize_cmd_array(t_cmdarg **node, int new_capacity);
int	ft_pars_env(char *env_line, char **key, char **value);
void	*parsing_word(t_cmdarg	**node, t_token *token_list);
void	parsing_redi(t_cmdarg **node, t_token *token_list);
int	no_need_chars(char *input);
bool ft_quote_checker(char *input);
bool	ft_isredi(t_token *token);
t_redi_list	*ft_redinew(t_token *token, bool expand);
void	ft_rediradd(t_redi_list **redi, t_redi_list *new);
void	join_or_add_first_word(t_cmdarg **node, char **split_words,
		bool first_word_joins);
void	extend_cmd_array_if_needed(t_cmdarg **node, int needed_capacity);
void	add_split_words(t_cmdarg **node, char **split_words,
		bool first_word_joins);
void	ft_free_split_words(char **split_words);
void	handle_split_word(t_cmdarg **node, t_token *token_list, char *value);
t_token	*ft_dollar_joining(t_token *token, t_token *new_token);
int	ft_toksize(t_token *lst);
t_token	*newtok(t_token *token);
void	ft_tokadd_back(t_token **token_list, t_token *token);
bool	ft_tojoin(t_lexer *lexer);
t_token *ft_jointok(t_token *token, t_lexer **lexer, t_list *minienv, bool *heredoc);
t_token	*get_next_token(t_lexer *lexer, t_list *minienv, bool *heredoc);
int ft_isspace(char c);
int	ft_isspecial(char c);
ssize_t	dollar_position(char *str);
bool	ft_is_cmd(t_token *current);
void	ft_free_node(t_cmdarg *node);
void	ft_free_redi_list(t_redi_list *redi);
char	*expand_inchar(char *value, t_list *minienv, ssize_t dollar_pos);
void	expand_exit_status_inchar(char **value);
bool	ft_condition_inchar(char *value, ssize_t dollar_pos);
void	expand_var_in_char(char	**value, t_list *minienv);
bool	op_is_duplicated(t_lexer *lexer, char op[3]);
char	*help_tmp2_inchar(char *value, ssize_t dollar_pos);
void	ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list);
void	ft_close_pipe(t_redi_list *redi);

#endif
