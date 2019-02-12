#define MAX_PROG_LENGTH 2048
#define VAL_TRUE 1
#define VAL_FALSE 0
#define SUCCESS 0
#define loadAddr 100 /*as stated in the given information*/
#define MAX_CHARS_IN_LINE 80 /*as stated in the given information*/
#define AMOUNT_SAVED_WORDS 16
#define AMOUNT_OF_COMMANDS 16
const char *SAVEDWORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop", "data", "string", "entery", "extern"};

const char *COMMAND_NAME_WORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop"};