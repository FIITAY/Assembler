/*File: secondLoop.c
 *Author: Itay Finci
 *
 *this file takes a file and proccess the second loop over it
 */

#include "secondLoop.h"
#include "firstLoop.h"
#include "lineParse.h"
#include "consts.h"

/*constants of target and source operands*/
#define OPERAND_TARGET 0
#define OPERAND_SOURCE 1

/*decleration of functions from the end of the file*/
Exeption finishEncodinCode(char *, Word [], Table *, Table * ,int *);
Exeption finishEntery(char *, Table *);

/*this function gets opened file, binary code, symbol table and do the second loop over it*/
Exeption doSecondLoop(FILE *fp, Word binaryCode[], Table *symbolTable, Table *externalTable)
{
    int IC = 0; /*step 1*/
    char line[MAX_CHARS_IN_LINE];
    char originalLine[MAX_CHARS_IN_LINE];
    char *restOfLine;
    Exeption ret = SUCCESS; /*start the loop thinking that there wont be any errors*/


    while(fgets(line, MAX_CHARS_IN_LINE, fp) != NULL) /*read line from fp, insert it into line ptr and the length to lineLength*/
    {
        Exeption currRet = SUCCESS;
        strcpy(originalLine, line); /*make a copy of line for error handle printing*/
        restOfLine = NULL; /*reset restOfLine each run*/
        if(comment(line) != VAL_TRUE) /*check if the line is cooment, if it is ignore*/
        {
            enum lineKind kind;
            restOfLine = strchr(line, ':');
            /*set restOfLine to be without the lable*/
            if(restOfLine == NULL)/*if there isnt ':' rest of line is the whole line*/
                restOfLine = line;
            else /*if there is ':' rest of line is the part after it*/
            {
                restOfLine = strtok(line, ":");/*remove the label from the strtok of line*/
                restOfLine = strtok(NULL, "");/*put inside restOfLine the reminds of line*/
            }
            /*check the kind of the line*/
            kind = checkKind(strtok(restOfLine, " \t\n"));
            if(kind == K_ENTERY) /*if the kind is entery this is the parse*/
            {
                /*step 6- finish encoding the entery into the symbolTable*/
                currRet = finishEntery(strtok(NULL,"\n"), symbolTable);
                if(currRet != SUCCESS) /*if thee was error print the error*/
                    errorHandle(originalLine, currRet);
            }
            if(kind == K_CODE) /*if the kind is code this is the parse*/
            {
                /*step 7- finish the coding of the 2 and 3 words*/
                currRet = finishEncodinCode(strtok(NULL,"\n"),binaryCode,symbolTable, externalTable ,&IC);
                if(currRet != SUCCESS) /*if thee was error print the error*/
                    errorHandle(originalLine, currRet);
            }
            /*if kind is K_DATA \ K_EXTERN \ K_STRING or step 6 or step 9 go to here*/
        }
        /*comment do nothing*/
        if(currRet != SUCCESS)/*if failed update ret value*/
            ret = currRet;
    }
    /*step 10*/
    return ret; /*if there was error- it will return error, else will return SUCCESS*/
}

TableRow *copyRow(TableRow *);

/*this function takes line that is entery and parse the label into the symbol table*/
Exeption finishEntery(char *restOfLine, Table *symbolTable)
{
    char *label = strtok(restOfLine, " \t\n"); /*takes the first word from restOfLine to label*/
    int found = VAL_FALSE; /*start thinking that the label is not found*/
    TableRow *tr = symbolTable->head; /*make tr point to the symbol table head*/
    if(label == NULL)/*if there is no label in the string return error because there is missing parm.*/
        return MISSING_OPERATORS;
    /*search for the label in the symbol table*/
    while (tr != NULL && found == VAL_FALSE)
    {
        if(strcmp(label, tr->content) == 0) /*if the label is this row, make this row entery and change found*/
        {
            tr->is_entery= VAL_TRUE; /*this label is entery*/
            found = VAL_TRUE; /*the label was found*/
        }else
            tr = tr->next; /*label not found point to the next word*/
    }
    if(found == VAL_FALSE) /*if found is false return error- the label dosnt exists*/
        return ENTERY_LABEL_NOT_FOUND;
    if(strtok(NULL, " \t\n") != NULL) /*if there is another word in the end of the file return error*/
        return GARBUIGE_AT_THE_END;
    return SUCCESS; /*threre wasnt any errors return success*/
}

Exeption buildParmWord(char *, Table *, Word *, Table *, int, int *);

/*this function takes code line and finish his second and third word*/
Exeption finishEncodinCode(char *restOfLine, Word binaryCode[], Table *symbolTable, Table *externalsTable ,int *IC)
{
    struct commandWord cword = binaryCode[*IC].command; /*make cword point to the command first word*/
    char *operand;
    int currIC;   /* will be used locally, as we move the global after each param */
    Exeption ret = SUCCESS;
    Exeption error = SUCCESS;

    *IC = *IC + 1; /*move ic one word forward to skip command*/

    switch (cword.opcode) {
        case OPCODE_MOV:
        case OPCODE_CMP:
        case OPCODE_ADD:
        case OPCODE_SUB:
        case OPCODE_LEA:
            /*there are two args*/
            currIC = *IC;
            /*if both of the parameters are registers dont move the ic forward because it uses the same word*/
            if(!(cword.targetOp == ADDR_MODE_REGISTER && cword.sourceOp == ADDR_MODE_REGISTER))
                *IC = *IC + 1; /*move ic one word forward*/
            if(cword.opcode == OPCODE_LEA && cword.sourceOp != ADDR_MODE_DIRECT)
                error =  ILLIGAL_PARATMETER;
            else
            {
                operand = strtok(restOfLine, " ,\t\n"); /*take the first word and put it inside operand*/
                restOfLine = strtok(NULL, ""); /*make restOfLine the rest Of the line without the first parm*/
                /*finish the encoding of the second and first word*/
                ret = buildParmWord(operand, symbolTable, &binaryCode[currIC], externalsTable ,OPERAND_SOURCE, &currIC);
                if(ret != SUCCESS)
                    error =  ret;
                else
                {
                    /*if the first parameter was register but the second wasnt a regirst put 0 in the place of the second. */
                    if(cword.sourceOp == ADDR_MODE_REGISTER && cword.targetOp != ADDR_MODE_REGISTER)
                        (binaryCode[currIC].reg).targetOp = 0;
                }
            }
            /*fallthrow to handle target opearand*/
        case OPCODE_NOT:
        case OPCODE_CLR:
        case OPCODE_INC:
        case OPCODE_DEC:
        case OPCODE_JMP:
        case OPCODE_BNE:
        case OPCODE_RED:
        case OPCODE_PRN:
        case OPCODE_JSR:
            /*there is one arg*/
            /*check targetOp*/
            currIC = *IC;
            *IC = *IC + 1; /*move ic one word forward*/
            /*check if the adreesing mode is legal*/
            if((cword.opcode != OPCODE_PRN && cword.opcode != OPCODE_CMP) && cword.targetOp == ADDR_MODE_IMIDIATE)
                error = ILLIGAL_PARATMETER;
            else
            {
                /*make operand equal to the next word from restOfLine*/
                operand = strtok(restOfLine, " ,\n\t");
                /*finish encoding the second parm*/
                ret = buildParmWord(operand, symbolTable, &binaryCode[currIC], externalsTable, OPERAND_TARGET, &currIC);
                if(ret != SUCCESS)
                {
                    error =  ret;
                }
                else
                {
                    /*if the second parameter was register but the first wasnt a regirst put 0 in the place of the first. */
                    if(cword.targetOp == ADDR_MODE_REGISTER && cword.sourceOp != ADDR_MODE_REGISTER)
                        (binaryCode[currIC].reg).sourceOp = 0;
                }
            }
            break;
        default:
            break;
    }

    return error; /*if there wasnt any error return success*/
}

/*this function take operand and encode his binary word*/
Exeption buildParmWord(char *operand, Table *symbolTable, Word *output, Table *externalsTable, int regMode, int *IC)
{
    int opFormat = operandFormat(operand); /*find the format of the operand*/
    Exeption ret = SUCCESS; /*start by thinking that the parse was successful*/
    if(opFormat == ADDR_MODE_REGISTER) /*if the operand is register*/
    {
        (output->reg).type= TYPE_A; /*the register mode is A*/
        if(strlen(operand) == 3 && operand[0] == '@' && operand[1] == 'r')/*the length of register name is 3 : @ r n*/
        {
            char regNumber = operand[2]; /*the third char of the operand is the number*/
            if(!(regNumber >= '0' && regNumber <= '7')) /*check if the number of the registor is legal*/
                return ILLIGAL_REGISTER_NUMBER;
            if(regMode == OPERAND_SOURCE) /*if the operand is source set the reg number is the source op*/
                (output->reg).sourceOp = regNumber - '0';
            else /*set the regnumber in the destOp*/
                (output->reg).targetOp = regNumber - '0';
        }
        else /*there is error in the format*/
            return ILLIGAL_REGISTER_FORMAT;
    }
    if(opFormat == ADDR_MODE_DIRECT) /*if the operand is label*/
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
        if(trLabel->symbolKind != K_EXTERN) /*if the lable isnt extern*/
        {
            /*copy the name of the label to the output word*/
            int i,mask;
            mask =1;
            (output->parmNum).number = 0; /*reset the number firstly*/
            for(i=0;i<NUMBER_BIT_LEN; i++)
            {
                /*puts the place in the memory that is the name plus the load address*/
                (output->parmNum).number |= (trLabel->name+LOAD_ADDR) & mask;
                mask = mask << 1; /*move the mask forword*/
            }
        }
        else /*if the label is extern*/
        {
            TableRow *tr = externalsTable->head; /*make tr point to the externals table head*/
            TableRow *ext = (TableRow *)malloc(sizeof(TableRow)); /*make new row */
            /*extern is 0*/
            (output->parmNum).number = 0; /*reset the number to 0*/
            /*make ext*/
            ext->name = *IC; /*make the name equal to the ic*/
            strcpy(ext->content, trLabel->content); /*copy the name of the lable*/
            ext->is_entery = trLabel->is_entery; /*set if the extern is entery*/
            ext->symbolKind = K_EXTERN; /*set the kind extern*/
            ext->next = NULL; /*set the next null*/
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
    if(opFormat == ADDR_MODE_IMIDIATE)/*if the operand is number*/
    {
        int number, i, mask;
        ret = checkLegalityNumber(operand); /*check if the number is ligal*/
        if(ret != SUCCESS)
            return ret;/*there was error dont parse the data*/
        number = atoi(operand);/*parse the string to int*/
        (output->parmNum).type = TYPE_A; /*number is constant so type is A*/
        mask =1; /*set the mask base*/
        (output->parmNum).number = 0; /*reset the number at the start*/
        /*copy the 10 first bits*/
        for(i=0;i<NUMBER_BIT_LEN; i++)
        {
            (output->parmNum).number |= number & mask; /*copy the bit in the place i*/
            mask = mask << 1; /*move the mask forward*/
        }
    }

    return ret; /*return if there was error*/
}
