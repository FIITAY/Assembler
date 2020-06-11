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
enum commands {OPCODE_MOV=0,
 OPCODE_CMP=1,
 OPCODE_ADD=2,
 OPCODE_SUB=3,
 OPCODE_NOT=4,
 OPCODE_CLR=5,
 OPCODE_LEA=6,
 OPCODE_INC=7,
 OPCODE_DEC=8,
 OPCODE_JMP=9,
 OPCODE_BNE=10,
 OPCODE_RED=11,
 OPCODE_PRN=12,
 OPCODE_JSR=13,
 OPCODE_RTS=14,
 OPCODE_STOP=15};

 /*the file name will be updated for each file*/
 extern char *filename;

/*thous are the given addressing mode for parameters*/
#define ADDR_MODE_IMIDIATE 1
#define ADDR_MODE_DIRECT 3
#define ADDR_MODE_REGISTER 5

#endif
