#include "error.h"
#include <stdio.h>
/*error handler*/
void errorHandle(char *line, Exeption exep)
{
    printf("\nExeption at: %s", line);
    switch(exep)
    {
        case SUCCESS:
            break;
        case ILIGAL_LABEL_CHARACTERS:
            printf("       the label you entered have characters that are not allowed in label.\n");
            break;
        case ILIGAL_LABEL_NAME:
            printf("       the name of the label cant be one of the saved word.\n");
            break;
        case LABEL_ALREADY_EXSIST:
            printf("       you cant use the same name twice for label.\n");
            break;
        case COMMAND_NOT_FOUND:
            printf("       illigal command name.\n");
            break;
        case MISSING_OPERATORS:
            printf("       missing command operators.\n");
            break;
        case MISSING_COMMA:
            printf("       missing a comma between operators.\n");
            break;
        case GARBUIGE_AT_THE_END:
            printf("       garbuige at the end of the line is illigal.\n");
            break;
        case FORGOT_QUETATION:
            printf("       forgot the \" sign in the start or the end of string data.\n");
            break;
        case ILLIGAL_CHAR_IN_NUMBER:
            printf("       inside a number you can only have minus at the start and the numbers 0-9.\n");
            break;
        case ENTERY_LABEL_NOT_FOUND:
            printf("       entery label not found in file.\n");
            break;
        case ILLIGAL_PARATMETER:
            printf("       this parameter cannot be with this command.\n");
            break;
        case ILLIGAL_REGISTER_NUMBER:
            printf("       register number is between 0 to 7.\n");
            break;
        case ILLIGAL_REGISTER_FORMAT:
            printf("       register format is @r# when # is a number between 0 to 7.\n");
            break;
        case OPERAND_LABEL_NOT_FOUND:
            printf("       the label refered as operand not found in the symbol table.\n");
            break;
        case EXTRA_COMMA:
            printf("       there is more than 1 comma between two operands.\n");
            break;
    }
}
