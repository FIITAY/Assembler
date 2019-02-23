#include "secondLoop.h"
#include "lineParse.h"
#include "consts.h"

#define OPERAND_TARGET 0
#define OPERAND_SOURCE 1

Exeption finishEncodinCode(char *, Word [], Table *, Table * ,int *);
Exeption finishEntery(char *, Table *);

Exeption doSecondLoop(FILE *fp, Word binaryCode[], Table *symbolTable, Table *externalTable)
{
    int IC = 0; /*step 1*/
    char line[MAX_CHARS_IN_LINE];
    char *restOfLine;
    Exeption ret = SUCCESS; /*start the loop thinking that there wont be any errors*/
    while(fgets(line, MAX_CHARS_IN_LINE, fp) != NULL) /*read line from fp, insert it into line ptr and the length to lineLength*/
    {
        Exeption currRet = SUCCESS;
        if(line[0] != COMMENT_MARKER)
        {
            enum lineKind kind;
            restOfLine = strchr(line, ':');
            if(restOfLine == NULL)
                restOfLine = line;
            kind = checkKind(strtok(restOfLine, " \t\n"));
            if(kind == K_ENTERY) /*step 5*/
            {
                /*step 6*/
                currRet = finishEntery(restOfLine, symbolTable);
                if(currRet != SUCCESS)
                    errorHandle(line, currRet);
            }
            if(kind == K_CODE)
            {
                /*step 7- finish the coding of the 2 and 3 words*/
                currRet = finishEncodinCode(strtok(NULL, ""),binaryCode,symbolTable, externalTable ,&IC);
                if(currRet != SUCCESS)
                    errorHandle(line, currRet);
            }
            /*if kind is K_DATA \ K_EXTERN \ K_STRING or step 6 or step 9 go to here*/
        }
        /*comment do nothing*/
        if(currRet != SUCCESS)/*if failed update ret value*/
            ret = currRet;
    }
    /*step 10*/
    return ret;
}

TableRow *copyRow(TableRow *);

Exeption finishEntery(char *restOfLine, Table *symbolTable)
{
    char *label = strtok(restOfLine, " \t\n");
    int found = VAL_FALSE;
    TableRow *tr = symbolTable->head;
    if(label == NULL)
        return MISSING_OPERATORS;
    /*search for the label in the symbol table*/
    while (tr != NULL && found == VAL_FALSE)
    {
        if(strcmp(label, tr->content) == 0)
        {
            tr->is_entery= VAL_TRUE;
            found = VAL_TRUE;
        }else
            tr = tr->next;
    }
    if(found == VAL_FALSE)
        return ENTERY_LABEL_NOT_FOUND;
    if(strtok(NULL, " \t\n") != NULL)
        return GARBUIGE_AT_THE_END;
    return SUCCESS;
}

Exeption buildParmWord(char *, Table *, Word *, Table *, int, int *);

Exeption finishEncodinCode(char *restOfLine, Word binaryCode[], Table *symbolTable, Table *externalsTable ,int *IC)
{
    struct commandWord cword = binaryCode[*IC].command;
    char *operand;

    *IC = *IC + 1; /*move ic one word forward to skip command*/

    if(cword.targetOp != 0)
    {
        /*check targetOp*/
        /*check if the type of paramtel is ligal whith the currenct command*/
        if(cword.targetOp == ADDR_MODE_IMIDIATE && !(cword.opcode == OPCODE_CMP || cword.opcode == OPCODE_PRN))
            return ILLIGAL_PARATMETER;
        operand = strtok(restOfLine, " ,\n\t");
        buildParmWord(operand, symbolTable, &binaryCode[*IC], externalsTable ,OPERAND_TARGET, IC);

        /*if the first parameter was register but the second wasnt a regirst put 0 in the place of the second. */
        if(cword.targetOp == ADDR_MODE_REGISTER && cword.sourceOp != ADDR_MODE_REGISTER)
            (binaryCode[*IC].reg).sourceOp = 0;
        /*if both of the parameters are registers dont move the ic forward because it uses the same word*/
        if(!(cword.targetOp == ADDR_MODE_REGISTER && cword.sourceOp == ADDR_MODE_REGISTER))
            *IC = *IC + 1; /*move ic one word forward*/
    }
    if(cword.sourceOp != 0)
    {
        /*check sourceOp*/
        /*check if the type of paramtel is ligal whith the currenct command*/
        if(cword.opcode == OPCODE_LEA && cword.sourceOp != ADDR_MODE_DIRECT)
            return ILLIGAL_PARATMETER;
        operand = strtok(NULL, " \n\t");
        buildParmWord(operand, symbolTable, &binaryCode[*IC], externalsTable, OPERAND_SOURCE, IC);
        /*if the second parameter was register but the first wasnt a regirst put 0 in the place of the first. */
        if(cword.sourceOp == ADDR_MODE_REGISTER && cword.targetOp != ADDR_MODE_REGISTER)
            (binaryCode[*IC].reg).destOp = 0;
        *IC = *IC + 1; /*move ic one word forward*/
    }

    return SUCCESS;
}

Exeption buildParmWord(char *operand, Table *symbolTable, Word *output, Table *externalsTable, int regMode, int *IC)
{
    int opFormat = operandFormat(operand);
    Exeption ret = SUCCESS;
    if(opFormat == ADDR_MODE_REGISTER)
    {
        (output->reg).type= TYPE_A;
        if(strlen(operand) == 3)/*the length of register name is 3 : @ r n*/
        {
            int regNumber = operand[2] - '0'; /*the third char of the operand is the number*/
            if(regNumber < 0 || regNumber > 7)
                return ILLIGAL_REGISTER_NUMBER;
            if(regMode == OPERAND_SOURCE)
                (output->reg).sourceOp = regNumber;
            else
                (output->reg).destOp = regNumber;
        }
        else
            return ILLIGAL_REGISTER_FORMAT;
    }
    if(opFormat == ADDR_MODE_DIRECT)
    {
        /*labels*/
        /*firstly find the operan in the symbol table*/
        TableRow *trLabel = symbolTable->head;
        while(trLabel!= NULL && strcmp(operand, trLabel->content) != 0)
            trLabel = trLabel->next;
        if(trLabel == NULL)
            return OPERAND_LABEL_NOT_FOUND;
        /*trLabel have the line of the header that is the operand*/
        /*put the respective type inside the output word- if the label is not external type R if the label is external type E*/
        (output->parmNum).type = (trLabel->symbolKind != K_EXTERN) ? TYPE_R : TYPE_E;
        if(trLabel->symbolKind != K_EXTERN)
        {
            /*copy the name of the label to the output word*/
            int i,mask;
            mask =1;
            for(i=0;i<NUMBER_BIT_LEN; i++)
            {
                (output->parmNum).number = trLabel->name & mask;
                mask = mask << 1;
            }
        }
        else
        {
            TableRow *tr = externalsTable->head;
            TableRow *ext = (TableRow *)malloc(sizeof(TableRow));
            /*extern is 0*/
            (output->parmNum).number = 0;
            /*make ext*/
            ext->name = *IC;
            strcpy(ext->content, trLabel->content);
            ext->is_entery = trLabel->is_entery;
            ext->symbolKind = K_EXTERN;
            ext->next = NULL;
            /*add ext to the table*/
            if(externalsTable->head == NULL)
            {
                /*if there is no head make the curreent external as head*/
                externalsTable->head = ext;
            }
            else
            {
                /*find last row and add the ext row after*/
                while(tr->next != NULL)
                    tr = tr->next;
                tr->next = ext;
            }
        }
    }
    if(opFormat == ADDR_MODE_IMIDIATE)
    {
        int number, i, mask;
        ret = checkLegalityNumber(operand);
        if(ret != SUCCESS)
            return ret;/*there was error dont parse the data*/
        number = atoi(operand);/*parse the string to int*/
        (output->parmNum).type = TYPE_A; /*number is constant so type is A*/
        mask =1;
        for(i=0;i<NUMBER_BIT_LEN; i++)
        {
            (output->parmNum).number = number & mask;
            mask = mask << 1;
        }
    }

    return ret;
}
