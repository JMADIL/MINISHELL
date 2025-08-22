/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajamoun <ajamoun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:12:45 by ajamoun           #+#    #+#             */
/*   Updated: 2025/08/22 04:26:26 by ajamoun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
//this one should be on the libft.h
typedef struct s_list
{
	char			*key;
	char			*value;
	int				check;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;


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
t_lexer *ft_strtok(char *input, t_list *minienv);
void	ft_init_fields(t_token *token);
void	ft_set_token_type(t_token token, int op_len, char *op, bool *heredoc);
t_token	*handling_operator(t_lexer lexer, bool heredoc);
t_token	*handling_quotes(t_lexer *lexer, char quote);
t_token	*handling_words(t_lexer *lexer);
t_list	*ft_initenv(char **env);
t_cmdarg	*ft_init_node(void);
t_cmdarg	*get_next_node(t_token	*token_list);
t_cmdarg	*ft_newnode(t_cmdarg *node);
void	ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new);
bool	resize_cmd_array(t_cmdarg **node, int new_capacity);
int	ft_pars_env(char *env_line, char **key, char **value);
static int	extend_cmd_for_word(t_cmdarg **node);
static void	add_or_join_word(t_cmdarg **node, t_token *token_list, char *value);
void	*parsing_word(t_cmdarg	**node, t_token *token_list);
void	*parsing_redi(t_cmdarg **node, t_token *token_list);
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
t_token	*get_next_token(t_lexer *lexer, t_list *minienv, bool heredoc);
int ft_isspace(char c);
int	ft_isspecial(char c);
ssize_t	dollar_position(char *str);
bool	ft_is_cmd(t_token *current);


#endif
