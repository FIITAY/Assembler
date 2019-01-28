struct commandWord
{
    unsigned int type     : 2;
    unsigned int targetOp : 3;
    unsigned int opcode   : 4;
    unsigned int sourceOp : 3;
};

struct dataWord
{
    unsigned int data : 12;
};

struct regWord
{
    unsigned int type     : 2; /*always zero*/
    unsigned int destOp   : 5;
    unsigned int sourceOp : 5;
};

typedef union word{
    struct commandWord command;
    struct dataWord data;
    struct regWord reg;
}word;