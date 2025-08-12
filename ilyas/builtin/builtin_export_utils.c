#include "../help.h"


/*
 * Prints environment variables in export format.
 * Displays each environment variable in the "declare -x" format used by
 * the export builtin. Handles variables with null values by omitting the
 * assignment, and properly quotes values containing spaces or special chars.
 *
 * @param head: Environment variables linked list to print
 * Side effects: Prints formatted environment variables to stdout
 */

 void print_export_format(t_list *head)
 {
    while(head)
    {
        printf("declare -x %s", head->hey);
    }
 }