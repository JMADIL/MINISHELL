#include "../minishell"

t_lexer *ft_init_lexer(char *input)
{
    t_lexer lexer;

    lexer = malloc(sizeof(t_lexer));
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = ft_strlen(input);
    return(lexer);
}
t_lexer *ft_strtok(char *input, t_list *minienv)
{
    t_lexer *lexer;
    t_token *token;
    t_token *token_list;
    bool    heredoc;

    if(!input)
        return (NULL);
    lexer = ft_init_lexer(input);
    // head of linked list of tokens
    token_list = NULL;
    // state flag: are we inside a heredoc?
    heredoc = false;
    token = ->> ft_get_next_token();<<-
    while(token)
    {
        if(!token)
            break ;
        if(token->value)
            ->>ft_tokadd_back(, ->>ft_newtok()<<-);<<-
        ->>ft_free_token();<<-
        token = ->>ft_get_next_token();<<-
    }
    free(token);
    return (token_list);
}