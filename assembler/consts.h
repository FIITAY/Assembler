#ifndef CONSTS
#define CONSTS

#define MAX_PROG_LENGTH 2048
#define VAL_TRUE 1
#define VAL_FALSE 0
#define loadAddr 100 /*as stated in the given information*/
#define MAX_CHARS_IN_LINE 80 /*as stated in the given information*/
#define AMOUNT_SAVED_WORDS 16
#define AMOUNT_OF_COMMANDS 16
#define ERROR_RETURN -10
#define COMMENT_MARKER ';'

#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 6
#define OPCODE_INC 7
#define OPCODE_PRN 12
#define OPCODE_JSR 13

#define ADDR_MODE_IMIDIATE 1
#define ADDR_MODE_DIRECT 3
#define ADDR_MODE_REGISTER 5

extern const char *SAVEDWORDS[];
extern const char *COMMAND_NAME_WORDS[];

#endif
