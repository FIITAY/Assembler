#include <ctype.h>
#include <string.h>
#include "lineParse.h"

#define EXTERNAL -1

const char *COMMAND_NAME_WORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop"};
const char *SAVEDWORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop", "data", "string", "entery", "extern"};

int parseLine(char *line, Word **output, Table *symbolTable, DataTable *data, int *IC, int *DC)
{
    int L = 0;
    enum lineKind kind;
    char *word, *label, *restOfLine;
    char originalLine[MAX_CHARS_IN_LINE];
    TableRow *trLabel = NULL, *tr;
    Exeption ret;
    word = NULL;
    label = NULL;
    restOfLine = NULL;
    strcpy(originalLine, line);
    /*label check*/
    if(strchr(line, ':') != NULL)
    {
        label = strtok(line, ":");
        if((ret = checkLable(label, symbolTable)) != SUCCESS)
        {
            errorHandle(originalLine, ret);
            L= ERROR_RETURN;
            goto FINISH;
        }
    }
    /*kind check*/
    if(label != NULL)
        word = strtok(NULL, " \t\n");
    else
        word = strtok(line, " \t\n");
    if(word != NULL)
    {
        kind = checkKind(word);
        if(kind == K_DATA || kind == K_STRING)
        {
            /*steps 5-7*/
            if(label != NULL)
            {
                trLabel = makeLabel(*DC, label, kind);
            }
            /*encode data/string to the table*/
            restOfLine = strtok(NULL,""); /*gets the rest of the line*/
            ret = parseKindData(restOfLine, data, DC, kind);
            if(ret != SUCCESS)
            {
                errorHandle(originalLine, ret);
                L = ERROR_RETURN;
                goto FINISH;
            }
            L=0; /*if there is data or string the IC dont need to be updated*/
        }
        if(kind == K_ENTERY || kind == K_EXTERN)/*dont need to entery label of this kind of commands to the table*/
        {
            /*steps 8-10*/
            /*entery do nothing, extern do step 11*/
            if(kind == K_EXTERN)
            {
                restOfLine = strtok(NULL,""); /*gets the rest of the line*/
                ret = parseExtern(restOfLine,symbolTable);
                if(ret != SUCCESS)
                {
                    errorHandle(originalLine, ret);
                    L= ERROR_RETURN;
                    goto FINISH;
                }
            }
            L=0; /*if there is entery or extern the IC dont need to be updated*/
        }
        if(kind == K_CODE)
        {
            /*steps 11-15*/
            if(label != NULL)
            {
                trLabel = makeLabel(*IC, label, K_CODE);
            }
            restOfLine = strtok(NULL,""); /*gets the rest of the line*/
            ret = parseKindCode(word, restOfLine,output, &L);
            if(ret != SUCCESS)
            {
                errorHandle(originalLine, ret);
                L = ERROR_RETURN;
                goto FINISH;
            }
        }
    }
    else
    {
        /*empty line, check if there is label or not*/
        if(label != NULL)
        {
            /*enter label as code*/
            trLabel = makeLabel(*IC, label, K_CODE);
        }
    }

FINISH: 
    if(trLabel != NULL)
    {
        tr = symbolTable->head;
        if(symbolTable->head == NULL)/*if there is no head put in the head*/
            symbolTable->head = trLabel;
        else /*there is head put in last*/
        {
            while(tr->next != NULL)/*finds the lasst row in the table*/
            tr = tr->next;
            tr->next = trLabel; /*make the last row the new label*/
        }
    }

    return L;
}

TableRow *makeLabel(int name, char *content, enum lineKind kind)
{
    TableRow *trLabel = (TableRow *) malloc(sizeof(TableRow));/*malloc place for the new row*/
    trLabel->name = name;
    strcpy(trLabel->content, content); /*copy the name of the label to the field in the row*/
    trLabel->symbolKind = kind;
    trLabel->is_entery = VAL_FALSE;
    trLabel->next = NULL;
    return trLabel;
}

/*check that all of the characters are from the abc (capital or not) or numbers*/
Exeption checkLable(char *label, Table *symbolTable)
{
    int i;
    TableRow *tr = symbolTable->head;
    /*check that all characters are valid*/
    for(i=0;i<strlen(label); i++)
    {
        char c = label[i];
        if(!((c >= 'a' && c<= 'z') || (c >= 'A' && c<= 'Z') || (c >= '0' && c<= '9')))
        {
            return ILIGAL_LABEL_CHARACTERS;
        }
    }
    /*check that the label is not one of the saved words*/
    for(i=0;i < AMOUNT_SAVED_WORDS; i++)
    {
        if(strcmp(label, SAVEDWORDS[i]) == 0)
        {
            return ILIGAL_LABEL_NAME;
        }
    }
/*check that the name isnt in the label list already*/
    while(tr != NULL)
    {
        if(strcmp(label, tr->content) == 0)
        {
            return LABEL_ALREADY_EXSIST;
        }
        tr = tr->next;
    }
    return SUCCESS;
}

enum lineKind checkKind(char *firstWord)
{
    if(firstWord == NULL)
        return K_NULL;
    if(strcmp(firstWord, ".data") == 0)
        return K_DATA;
    if(strcmp(firstWord, ".string") == 0)
        return K_STRING;
    if(strcmp(firstWord, ".extern") == 0)
        return K_EXTERN;
    if(strcmp(firstWord, ".entery") == 0)
        return K_ENTERY;
    return K_CODE;
}


Exeption parseKindData(char *line, DataTable *data, int *DC, enum lineKind kind)
{
    DataTableRow *dtr = data->head; /*point to the last place to add the new word to*/
    char *str;
    int number,j,mask;
    Exeption ret;

    /*get dtr to the last row*/
    if(data->head != NULL)
    {
        while(dtr->next != NULL)
            dtr = dtr->next;
    }

    str = strtok(line, " ,\n\t"); /*gets the first parm*/

    while(str != NULL)
    {
        int i, sizeOfDw;
        Word *dw;
        switch(kind)
        {
            case K_DATA:
                /*firstly check that all of the chars in str are either number or "-" or "."*/
                ret = checkLegalityNumber(str);
                if(ret != SUCCESS)
                    return ret;/*there was error dont parse the data*/
                number = atoi(str);/*parse the string to double*/
                /*parse the data in word into dw as number*/
                dw = (Word *)malloc(sizeof(Word));
                mask = 1;
                for(j=0; j<12;j++)/*copy the 12 bits of the char into the data word*/
                {
                    (dw->data).data |= (number & mask);
                    mask = mask << 1;
                }
                break;
            case K_STRING:
                /*parse the data in the word into dw as string*/
                sizeOfDw = (int) strlen(str) - 2;/*remove the count of the starting " and the closing "*/
                if(str[0] == '\"' && str[sizeOfDw+1]== '\"') /*only if this is string and now floating text*/
                {
                    dw = (Word *)malloc((sizeOfDw+1)*sizeof(Word));/*for the amount of chars inside the string + \0*/
                    for(i=0; i< sizeOfDw;i++)
                    {
                        int mask = 1,j;
                        ((dw+i)->data).data = 0;
                        for(j=0; j<12;j++)/*copy the 12 bits of the char into the data word*/
                        {
                            ((dw+i)->data).data |= (str[i+1] & mask);
                            mask = mask << 1;
                        }
                    }
                    /*add \0*/
                    (dw+sizeOfDw)->data.data = 0; /*value of \0 is 0*/
                }
                else if(str[sizeOfDw+1]!='\"')
                    return FORGOT_QUETATION;/*the user forgot the quetatoin sign in the start or the end, its not comment*/
                break;
            case K_EXTERN:
            case K_ENTERY:
            case K_CODE:
            case K_NULL:
                break; /*not the right function*/
        }
        /*add dw into the end of the data table*/
        for(i=0; i<sizeOfDw+1; i++) /*will loop the amount of chars in the string +1 for the \0*/
        {
            /*make new row*/
            DataTableRow *newRow = (DataTableRow *)malloc(sizeof(DataTableRow));
            newRow->DC = *DC;
            newRow->content = *(dw+i);
            newRow->next = NULL;
            /*add new row to the data table*/
            if(data->head == NULL)
            {
                data->head = newRow;
                dtr = data->head;
            }
            else
            {
                dtr->next = newRow;
                dtr = dtr->next;
            }
        }
        str = strtok(NULL," ,\n\t");
    }
    return SUCCESS;
}

Exeption checkLegalityNumber(char *str)
{
    int strLen = (int) strlen(str);
    int i;
    if(str[0] != '-' && !isdigit(str[0]))
        /*illigial char inside a number*/
        return ILLIGAL_CHAR_IN_NUMBER;
    for(i=1;i<strLen;i++)
    {
        if(!isdigit(str[i]))
            return ILLIGAL_CHAR_IN_NUMBER;
    }
    return SUCCESS;
}

void buildFirstWordCode(Word **, int , char *, char *);

Exeption parseKindCode(char *command, char *line, Word **output, int *L)
{
    int i, commandFound;
    char *firstOp = NULL, *secondOp = NULL, *temp;
    commandFound = NOT_FOUND;
    /*check if the command exists*/
    for(i = 0; i < AMOUNT_OF_COMMANDS && commandFound == NOT_FOUND; i++)
    {
        if(strcmp(command, COMMAND_NAME_WORDS[i]) == 0)
            commandFound = i;
    }
    if(commandFound == NOT_FOUND){
        return COMMAND_NOT_FOUND;
    }

    /*everything else in parsing the command*/
    /*check if the command is from the first group of 2 operands, this group opcodes are from 0 to 3 and 6*/
    if((commandFound >= OPCODE_MOV && commandFound <=OPCODE_SUB) || commandFound == OPCODE_LEA)
    {
        /*two operands*/
        *L = 3; /*this type of operands need 3 words*/
        firstOp  = strtok(line, " \t\n");
        if(firstOp == NULL)
            return MISSING_OPERATORS;
        temp = strtok(NULL, " \n\t");
        if(temp == NULL)
            return MISSING_OPERATORS;
        if(temp[0] != ',' && firstOp[strlen(firstOp)-1] != ',' )/*if the last char in the first operand is comman or the first of the second ther is no missing comma*/
            return MISSING_COMMA;
        if(temp[0] == ',' && firstOp[strlen(firstOp)-1] == ',' )/*there is extra comma (2 commas)*/
            return EXTRA_COMMA;
        /*decide how to get the parameter to the secondOp var*/
        if(strlen(temp) > 1 && temp[0] == ',')
        {
            secondOp = (char *)malloc((strlen(temp)-1)*sizeof(char));
            memmove(secondOp, temp+1, strlen(temp)-1);
        }
        else if(strlen(temp) > 1 && temp[0] != ',')
        {
            secondOp = (char *)malloc((strlen(temp))*sizeof(char));
            memmove(secondOp, temp, strlen(temp));
        }
        else
            secondOp = strtok(NULL, " \t\n");
        if(secondOp == NULL)
            return MISSING_OPERATORS;

    }
    /*check if the command is from the second group of 1 operands, this group opcodes are 4,5 and 7 to 13 */
    else if(commandFound == OPCODE_NOT || commandFound == OPCODE_CLR || (commandFound >= OPCODE_INC && commandFound <= OPCODE_JSR))
    {
        /*one operand*/
        *L = 2; /*this type of operands need 2 words*/
        firstOp  = strtok(line, " \t\n");
        if(firstOp == NULL)
            return MISSING_OPERATORS;
    }
    /*check if the command is from the third group of 0 operands, this group opcodes are 14 and 15*/
    else
    {
        /*zero operands*/
        *L = 1; /*this type of operands need 1 word*/
    }

    *output = (Word *)malloc(sizeof(Word)*(*L));
    buildFirstWordCode(output, commandFound, firstOp, secondOp);

    /*check if there is gurbuige in the end of the line return error*/
    temp = strtok(NULL, " \t\n");
    if(temp != NULL) /*if at the end of the line there is text that is not a comment its illigal*/
        return GARBUIGE_AT_THE_END;
    /*succedded with no errors*/
    return SUCCESS;
}

void buildFirstWordCode(Word **output, int opCode, char *firstOp, char *secondOp)
{
    Word *cword;
    cword = &((*output)[0]);
    (cword->command).opcode = opCode;
    (cword->command).type = TYPE_A;
    if(firstOp != NULL)
    {
        (cword->command).targetOp = operandFormat(firstOp);
    }
    else
    {
        (cword->command).targetOp = 0;
    }
    if(secondOp != NULL)
    {
        (cword->command).sourceOp = operandFormat(secondOp);
        free(secondOp);
    }
    else
    {
        (cword->command).sourceOp = 0;
    }
    /*add the word to output*/
    *output[0] = *cword;
}

int operandFormat(char *operand)
{
    if(operand[0] == '@')
        return ADDR_MODE_REGISTER; /*this is a register*/
    else if(operand[0] == '-' || isdigit(operand[0]))
        return ADDR_MODE_IMIDIATE; /*this is a number*/
    else 
        return ADDR_MODE_DIRECT; /*this is not a register or a number so it must be label*/
}

Exeption parseExtern(char *restOfLine,Table *symbolTable)
{
    char *label, *temp;
    Exeption ret;
    label = strtok(restOfLine, " \t\n");
    ret = checkLable(label, symbolTable);
    if(ret != SUCCESS)
        return ret;/*error return*/
    else
    {
        /*add the label into the symbol table*/
        TableRow *trLabel = makeLabel(EXTERNAL, label, K_EXTERN);
        TableRow *tr;
        tr = symbolTable->head;
        if(symbolTable->head == NULL)/*if there is no head put in the head*/
            symbolTable->head = trLabel;
        else /*there is head put in last*/
        {
            while(tr->next != NULL)/*finds the lasst row in the table*/
                tr = tr->next;
            tr->next = trLabel; /*make the last row the new label*/
        }
    }
    /*check for garbuige*/
    temp = strtok(NULL, " \t\n");
    if(temp != NULL)
        return GARBUIGE_AT_THE_END;
    return SUCCESS;
}
