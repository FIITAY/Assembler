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
    fprintf(stderr, "\nFile %s, Exeption at- %s", fileName, line);/*print the first line of the message*/
    switch(exep)/*choose the rigth message for this exeption*/
    {
        case SUCCESS:
            break;
        case ILIGAL_LABEL_CHARACTERS:
            fprintf(stderr, "       the label you entered have characters that are not allowed in label.\n");
            break;
        case ILIGAL_LABEL_NAME:
            fprintf(stderr, "       the name of the label cant be one of the saved word.\n");
            break;
        case LABEL_ALREADY_EXSIST:
            fprintf(stderr, "       you cant use the same name twice for label.\n");
            break;
        case COMMAND_NOT_FOUND:
            fprintf(stderr, "       illigal command name.\n");
            break;
        case MISSING_OPERATORS:
            fprintf(stderr, "       missing command operands.\n");
            break;
        case MISSING_COMMA:
            fprintf(stderr, "       missing a comma between operators.\n");
            break;
        case GARBUIGE_AT_THE_END:
            fprintf(stderr, "       garbuige at the end of the line is illigal.\n");
            break;
        case FORGOT_QUETATION:
            fprintf(stderr, "       forgot the \" sign in the start or the end of string data.\n");
            break;
        case ILLIGAL_CHAR_IN_NUMBER:
            fprintf(stderr, "       inside a number you can only have minus at the start and the numbers 0-9.\n");
            break;
        case ENTERY_LABEL_NOT_FOUND:
            fprintf(stderr, "       entery label not found in file.\n");
            break;
        case ILLIGAL_PARATMETER:
            fprintf(stderr, "       this parameter cannot be with this command.\n");
            break;
        case ILLIGAL_REGISTER_NUMBER:
            fprintf(stderr, "       register number is between 0 to 7.\n");
            break;
        case ILLIGAL_REGISTER_FORMAT:
            fprintf(stderr, "       register format is @r# when # is a number between 0 to 7.\n");
            break;
        case OPERAND_LABEL_NOT_FOUND:
            fprintf(stderr, "       the label refered as operand not found in the symbol table.\n");
            break;
        case EXTRA_COMMA:
            fprintf(stderr, "       there is more than 1 comma between two operands.\n");
            break;
        case EMPTY_LABEL:
            fprintf(stderr, "       cant have empty label.\n");
            break;
        case LABLE_LONGER_THEN_ALLOW:
            fprintf(stderr, "       lable max length is 31 characters.\n");
            break;
        case ILLIGAL_STARTING_LETTER:
            fprintf(stderr, "       the first letter of label must be letter and not number.\n");
            break;
        case LABEL_BEFORE_EMPTY_LINE:
            fprintf(stderr, "       you are not allowed to have lable before empty line.\n");
            break;
    }
}
