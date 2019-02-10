#include "error.h"
#include <stdio.h>
/*error handler*/
void errorHandle(char *line, Exeption exep)
{
    printf("\nExeption at: %s\n", line);
    switch(exep)
    {
        case ILIGAL_LABEL_CHARACTERS:
            printf("       the label you entered have characters that are not allowed in label.\n");
            break;
        case ILIGAL_LABEL_NAME:
            printf("       the name of the label cant be one of the saved word.\n");
            break;
        case LABEL_ALREADY_EXSIST:
            printf("       you cant use the same name twice for label.\n");
    }
}