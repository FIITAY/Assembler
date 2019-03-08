#ifndef WORD
#define WORD

/*File: word.h
 *Author: Itay Finci
 *
 *this file is declaring the word union that will be used to hold the bit words in the project
 */

#define TYPE_A 0 /*value 00*/
#define TYPE_R 2 /*value 10*/
#define TYPE_E 1 /*value 01*/

/*the lengths of each part of memory inside the words*/
#define NUMBER_BIT_LEN 10
#define ARE_LEN 2
#define OPCODE_LEN 4
#define OPERAND_LEN 3
#define REGISTER_LEN 5
#define DATA_LEN 12

/*this struct represent the first word of each command*/
struct commandWord
{
    unsigned int type     : ARE_LEN;
    unsigned int targetOp : OPERAND_LEN;
    unsigned int opcode   : OPCODE_LEN;
    unsigned int sourceOp : OPERAND_LEN;
};

/*this struct represent a full 12 bit data word*/
struct dataWord
{
    unsigned int data : DATA_LEN;
};

/*this struct represent register word*/
struct regWord
{
    unsigned int type     : ARE_LEN;
    unsigned int targetOp : REGISTER_LEN;
    unsigned int sourceOp   : REGISTER_LEN;
};

/*this struct is for words that are mode (A,R,E) and number*/
struct parmNumber
{
    unsigned int type     : ARE_LEN;
    unsigned int number   : NUMBER_BIT_LEN;
};

/*this union will hold all the types of words in the project*/
typedef union word{
    struct commandWord command;
    struct dataWord data;
    struct regWord reg;
    struct parmNumber parmNum;
}Word;

#endif
