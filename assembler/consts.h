#ifndef CONSTS
#define CONSTS

/*File: consts.h
 *Author: Itay Finci
 *
 *in this file i will declare consts that will be usend in the project.
 */

#define MAX_PROG_LENGTH 2048 /*making it big to be able to hold a long projects*/
#define VAL_TRUE 1 /*declaring a true value that will be the same in the whole project*/
#define VAL_FALSE 0 /*declaring a false value that will be the same in the whole project*/
#define LOAD_ADDR 100 /*as stated in the given information*/
#define MAX_CHARS_IN_LINE 81 /*as stated in the given information*/
#define MAX_LABEL_LENGTH 31 /*as declered in the given infromation*/
#define ERROR_RETURN -10 /*declaring a error exit value that will be the same in the whole*/
#define COMMENT_MARKER ';' /*as defined in the given information*/

/*this opcodes are used in some test acroos the project*/
#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 6
#define OPCODE_INC 7
#define OPCODE_PRN 12
#define OPCODE_JSR 13

/*thous are the given addressing mode for parameters*/
#define ADDR_MODE_IMIDIATE 1
#define ADDR_MODE_DIRECT 3
#define ADDR_MODE_REGISTER 5

#endif
