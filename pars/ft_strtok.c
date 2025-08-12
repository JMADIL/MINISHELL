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
    
}