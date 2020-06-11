#ifndef ERROR
#define ERROR

/*File: error.h
 *Author: Itay Finci
 *
 *in this file i will declare the possible exeption and the function that will handle errors.
 */

/*declaration of the possible exeptions in the compile procidures*/
typedef enum {SUCCESS,
    ILIGAL_LABEL_CHARACTERS,
    ILIGAL_LABEL_NAME,
    LABEL_ALREADY_EXSIST,
    COMMAND_NOT_FOUND,
    MISSING_OPERATORS,
    MISSING_COMMA,
    GARBUIGE_AT_THE_END,
    FORGOT_QUETATION,
    ILLIGAL_CHAR_IN_NUMBER,
    ENTERY_LABEL_NOT_FOUND,
    ILLIGAL_PARATMETER,
    ILLIGAL_REGISTER_NUMBER,
    ILLIGAL_REGISTER_FORMAT,
    OPERAND_LABEL_NOT_FOUND,
    EXTRA_COMMA,
    EMPTY_LABEL,
    LABLE_LONGER_THEN_ALLOW,
    ILLIGAL_STARTING_LETTER,
    LABEL_BEFORE_EMPTY_LINE
} Exeption;

/*declaration of the error handle function that prints the right error message*/
void errorHandle(char *, Exeption);

#endif
