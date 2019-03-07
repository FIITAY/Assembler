/*File: error.c
 *Author: Itay Finci
 *
 *in this file i will program the function that takes the error and output it to the user
 */

/*includes that im using in this file*/
#include "error.h"
#include <stdio.h>

/*error handler- takes the line and the exep and output it in the following structure:
 Exeption at: LINE
        ERROR MESSAGE*/
void errorHandle(char *fileName, char *line, Exeption exep)
{
    printf("\nFile %s ,Exeption at- %s", fileName, line);/*print the first line of the message*/
    switch(exep)/*choose the rigth message for this exeption*/
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
            printf("       missing command operands.\n");
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
        case EMPTY_LABEL:
            printf("       cant have empty label.\n");
            break;
        case LABLE_LONGER_THEN_ALLOW:
            printf("       lable max length is 31 characters.\n");
            break;
        case ILLIGAL_STARTING_LETTER:
            printf("       the first letter of label must be letter and not number.\n");
            break;
        case LABEL_BEFORE_EMPTY_LINE:
            printf("       you are not allowed to have lable before empty line.\n");
            break;
    }
}
