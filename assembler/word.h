#ifndef WORD
#define WORD

#define TYPE_A 0 /*value 00*/
#define TYPE_R 2 /*value 10*/
#define TYPE_E 1 /*value 01*/

#define NUMBER_BIT_LEN 10
#define ARE_LEN 2
#define OPCODE_LEN 4
#define OPERAND_LEN 3
#define REGISTER_LEN 5
#define DATA_LEN 12

struct commandWord
{
    unsigned int type     : ARE_LEN;
    unsigned int targetOp : OPERAND_LEN;
    unsigned int opcode   : OPCODE_LEN;
    unsigned int sourceOp : OPERAND_LEN;
};

struct dataWord
{
    unsigned int data : DATA_LEN;
};

struct regWord
{
    unsigned int type     : ARE_LEN; /*always zero*/
    unsigned int destOp   : REGISTER_LEN;
    unsigned int sourceOp : REGISTER_LEN;
};

struct parmNumber
{
    unsigned int type     : ARE_LEN;
    unsigned int number   : NUMBER_BIT_LEN;
};

typedef union word{
    struct commandWord command;
    struct dataWord data;
    struct regWord reg;
    struct parmNumber parmNum;
}Word;

#endif
