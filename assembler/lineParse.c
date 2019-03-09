/*File: lineParse.c
 *Author: Itay Finci
 *
 *in this file i will make the parsing of one line for the first loop.
 *there will be some fuctions that will be opened externally because the second loop need them too.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lineParse.h"

/*this value is the value that i will put as the IC of lables that are externals*/
#define EXTERNAL -1
#define AMOUNT_SAVED_WORDS 27 /*length of the array*/
#define AMOUNT_OF_COMMANDS 16 /*there are 16 commands in the assembly*/

/*the command name list*/
const char *COMMAND_NAME_WORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop"};
/*the list of save words in this lang*/
const char *SAVEDWORDS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp",
 "bne", "red", "prn", "jsr", "rts", "stop", "data", "string", "entery", "extern",
    "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*this function gets a line and output the parsing of it into the respective data struct, return value is L.*/
int parseLine(char *fileName, char *line, Word **output, Table *symbolTable, DataTable *data, int *IC, int *DC)
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
    strcpy(originalLine, line);/*make a copy of the line for the error handler output*/
    /*label check*/
    if(strchr(line, LABEL_MARKER) != NULL)/*if there is ':' in the line there is lable before it*/
    {
        label = strtok(line, ":");/*gets the lable word*/
        if((ret = checkLable(label, symbolTable)) != SUCCESS)
        {
            /*the lable is not legal, make the returnes error and go to the finish proccess*/
            errorHandle(fileName, originalLine, ret);
            L= ERROR_RETURN;
            goto FINISH;
        }
    }

    /*check if there is need to give strtok the line or not*/
    if(label != NULL)
        word = strtok(NULL, " \t\n");
    else
        word = strtok(line, " \t\n");
    /*if there is another word in the line continue parsing*/
    if(word != NULL)
    {
        /*check what kind of line is it*/
        kind = checkKind(word);
        if(kind == K_DATA || kind == K_STRING)/*if the line is data or string this is the parsing*/
        {
            /*steps 5-7*/
            if(label != NULL)/*if there is lable, make it and put it into trLable*/
            {
                trLabel = makeLabel(*DC, label, kind);
            }

            /*encode data/string to the table*/
            restOfLine = strtok(NULL,""); /*gets the rest of the line*/
            /*parse the data from the rest of the line and put it into the dataTable*/
            if(kind == K_DATA)
                ret = parseKindData(restOfLine, data, DC);
            if(kind == K_STRING)
                ret = parseKindString(restOfLine, data, DC);
            if(ret != SUCCESS)/*if there was error parsing, return error and go to the finish proccess*/
            {
                errorHandle(fileName, originalLine, ret);
                L = ERROR_RETURN;
                goto FINISH;
            }
            L=0; /*if there is data or string the IC dont need to be updated*/
        }
        if(kind == K_ENTERY || kind == K_EXTERN)/*if the line is entery or extern this is the parsing*/
        {
            /*steps 8-10*/
            /*entery do nothing, extern do step 11*/
            if(kind == K_EXTERN)/*parse extern*/
            {
                restOfLine = strtok(NULL,""); /*gets the rest of the line*/
                ret = parseExtern(restOfLine,symbolTable);/*parse the extern into the symboltable*/
                if(ret != SUCCESS)/*if there was error parsing, return error and go to the finish proccess*/
                {
                    errorHandle(fileName, originalLine, ret);
                    L= ERROR_RETURN;
                    goto FINISH;
                }
            }
            L=0; /*if there is entery or extern the IC dont need to be updated*/
        }
        if(kind == K_CODE)/*if the line is code this is the parsing*/
        {
            /*steps 11-15*/
            if(label != NULL)/*if there is lable, make it and put it into trLable*/
            {
                trLabel = makeLabel(*IC, label, K_CODE);
            }
            restOfLine = strtok(NULL,""); /*gets the rest of the line*/
            ret = parseKindCode(word, restOfLine,output, &L);/*parse the command and put it inside output*/
            if(ret != SUCCESS)/*if there was error parsing, return error and go to the finish proccess*/
            {
                errorHandle(fileName, originalLine, ret);
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
            /*there is label, return error*/
            errorHandle(fileName, originalLine, LABEL_BEFORE_EMPTY_LINE);
            L = ERROR_RETURN;
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

/*this function take information of lable and make a table row from it*/
TableRow *makeLabel(int name, char *content, enum lineKind kind)
{
    TableRow *trLabel = (TableRow *) malloc(sizeof(TableRow));/*malloc place for the new row*/
    trLabel->name = name;/*set the row name*/
    strcpy(trLabel->content, content); /*copy the name of the label to the field in the row*/
    trLabel->symbolKind = kind; /*set the row kind*/
    trLabel->is_entery = VAL_FALSE; /*at this point all of the rows are not entery*/
    trLabel->next = NULL; /*make the row point to null*/
    return trLabel; /*return the new row*/
}

/*check that all of the characters are from the abc (capital or not) or numbers*/
Exeption checkLable(char *label, Table *symbolTable)
{
    int i;
    TableRow *tr = symbolTable->head; /*make tr point to the head of the table*/

    if(label == NULL)
        return EMPTY_LABEL;/*cannot have empty label return error*/

    /*if the lable is longer than alowed return error*/
    if(strlen(label) > MAX_LABEL_LENGTH)
        return LABLE_LONGER_THEN_ALLOW;

    if(!((label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
        return ILLIGAL_STARTING_LETTER;/*the first letter of label must be letter capital or not*/

    /*check that the rest characters are valid*/
    for(i=1;i<strlen(label); i++)
    {
        char c = label[i]; /*take the char in this place*/
        if(!((c >= 'a' && c<= 'z') || (c >= 'A' && c<= 'Z') || (c >= '0' && c<= '9')))
        {
            return ILIGAL_LABEL_CHARACTERS;/*if the char is not letter or number return error*/
        }
    }

    /*check that the label is not one of the saved words*/
    for(i=0;i < AMOUNT_SAVED_WORDS; i++)
    {
        if(strcmp(label, SAVEDWORDS[i]) == 0)/*if the lable and the word equals return error*/
        {
            return ILIGAL_LABEL_NAME;
        }
    }
/*check that the name isnt in the label list already*/
    while(tr != NULL)
    {
        if(strcmp(label, tr->content) == 0)/*if the lable and the lable in the row equals return error*/
        {
            return LABEL_ALREADY_EXSIST;
        }
        tr = tr->next; /*move the pointer to the next row*/
    }
    return SUCCESS; /*the lable is legal*/
}

/*check what kind of row is the row*/
enum lineKind checkKind(char *firstWord)
{
    if(firstWord == NULL)/*if there is no word return that this row type is null*/
        return K_NULL;
    if(strcmp(firstWord, ".data") == 0)/*if the first word is .data return kind data*/
        return K_DATA;
    if(strcmp(firstWord, ".string") == 0)/*if the first word is .string return kind string*/
        return K_STRING;
    if(strcmp(firstWord, ".extern") == 0)/*if the first word is .extern return kind extern*/
        return K_EXTERN;
    if(strcmp(firstWord, ".entry") == 0)/*if the first word is .entry return kind entry*/
        return K_ENTERY;
    return K_CODE; /*if the first word isnt one of the above return kind code*/
}

Exeption checkValidStringRow(char *);

/*this function gets a string data line and parse it into the dataTable*/
Exeption parseKindString(char *line, DataTable *data, int *DC)
{
    DataTableRow *dtr = data->head; /*point to the last place to add the new word to*/
    char *str;
    Exeption ret;

    /*get dtr to the last row*/
    if(data->head != NULL)
    {
        while(dtr->next != NULL)
            dtr = dtr->next;
    }

    ret = checkValidStringRow(line);
    if(ret != SUCCESS)
        return ret;
    str = strtok(line, "\"\n"); /*gets the string*/
    while(str != NULL)
    {
        int i, sizeOfDw;
        Word *dw;
        /*parse the data in the word into dw as string*/
        sizeOfDw = (int) strlen(str);
        dw = (Word *)malloc((sizeOfDw+1)*sizeof(Word));/*for the amount of chars inside the string + \0*/
        for(i=0; i< sizeOfDw;i++)
        {
            int mask = 1,j;
            ((dw+i)->data).data = 0;
            for(j=0; j<12;j++)/*copy the 12 bits of the char into the data word*/
            {
                ((dw+i)->data).data |= (str[i] & mask);
                mask = mask << 1;
            }
        }
        /*add \0*/
        (dw+sizeOfDw)->data.data = 0; /*value of \0 is 0*/
        /*add dw into the end of the data table*/
        for(i=0; i<sizeOfDw+1; i++) /*will loop the amount of chars in the string +1 for the \0*/
        {
            /*make new row*/
            DataTableRow *newRow = (DataTableRow *)malloc(sizeof(DataTableRow));
            newRow->DC = *DC;/*set the dc*/
            *DC = *DC + 1; /*increase the dc value*/
            newRow->content = *(dw+i);/*set the content of the row*/
            newRow->next = NULL; /*set the next row as null*/
            /*add new row to the data table*/
            if(data->head == NULL)
            {/*if there is no head set it as the head*/
                data->head = newRow;
                dtr = data->head;
            }
            else
            {/*there is head set it as the last row*/
                dtr->next = newRow;
                dtr = dtr->next;
            }
        }
        str = strtok(NULL, "\"\n"); /*gets the next string*/
    }
    return SUCCESS; /*there wasnt any errors return success*/
}

/*check that the format of the line is valid:
 "string" [, "string"]* */
Exeption checkValidStringRow(char *str)
{
    int i = 0;
    int strLen = strlen(str);
    int foundString = VAL_FALSE;
    while(str[i] != '\0')
    {
        /*skip white spaces*/
        while(i < strLen && isspace(str[i]))
            i++;
        /*check if reached the EOF*/
        if(i == strLen)
            return (foundString == VAL_TRUE) ? SUCCESS : MISSING_OPERATORS;

        if(str[i] != '"')
            return FORGOT_QUETATION;
        i++;
        while(i < strLen && str[i] != '"')
            i++;
        if(str[i] != '"')
            return FORGOT_QUETATION;
        foundString = VAL_TRUE;
        i++;
        /*skip white spaces*/
        while(i < strLen && isspace(str[i]))
            i++;
        if(i == strLen || str[i] == '\0')
            return SUCCESS;
        if(str[i] != ',')
            return MISSING_COMMA;
        i++;
    }
    return SUCCESS;
}

/*this function gets a the rest of the line and parse lines of data into the dataTable*/
Exeption parseKindData(char *line, DataTable *data, int *DC)
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

    str = strtok(line, ",\n"); /*gets the first parm*/

    /*loop while there is another word*/
    while(str != NULL)
    {
        Word *dw;
        /*make new row*/
        DataTableRow *newRow = (DataTableRow *)malloc(sizeof(DataTableRow));
        /*firstly check that all of the chars in str are either number or "-" or "."*/
        ret = checkLegalityNumber(str);
        if(ret != SUCCESS)
            return ret;/*there was error dont parse the data*/
        number = atoi(str);/*parse the string to double*/
        newRow->DC = *DC;/*set the dc*/
        *DC = *DC + 1; /*increase the dc value*/
        newRow->next = NULL; /*set the next row as null*/
        /*parse the data in word into dw as number*/
        dw = &(newRow->content);
        (dw->data).data = 0;
        mask = 1;
        for(j=0; j<12;j++)/*copy the 12 bits of the char into the data word*/
        {
            (dw->data).data |= (number & mask);
            mask = mask << 1;
        }
        /*add new row to the data table*/
        if(data->head == NULL)
        {/*if there is no head set it as the head*/
            data->head = newRow;
            dtr = data->head;
        }
        else
        {/*there is head set it as the last row*/
            dtr->next = newRow;
            dtr = dtr->next;
        }
        str = strtok(NULL,",\n");/*gets the next data word*/
    }
    return SUCCESS; /*there wasnt any errors return success*/
}

/*check that a number is legal*/
Exeption checkLegalityNumber(char *str)
{
    int strLen = (int) strlen(str); /*the length of str into strLen*/
    int i = 0;
    /*skip white spaces*/
    while(isspace(str[i]) && i < strLen)
        i++;
    if(str[i] == '\0')
        return MISSING_OPERATORS;
    if(str[i] != '-' && str[i] != '+' && !isdigit(str[i]))/*if the first letter is not + or - or digit return error*/
        /*illigial char inside a number*/
        return ILLIGAL_CHAR_IN_NUMBER;
    else
        i++;
    /*forward i while the char at str is digit*/
    while(isdigit(str[i]) && i < strLen)
        i++;
    /*skip white spaces*/
    while(isspace(str[i]) && i < strLen)
        i++;
    if(i < strLen)
    {
        if(str[i] != '-' && str[i] != '+' && !isdigit(str[i]))
            return  ILLIGAL_CHAR_IN_NUMBER;
        return MISSING_COMMA;
    }
    return SUCCESS;/*this is legal number, return success*/
}

void buildFirstWordCode(Word **, int , char *, char *);

/*parse lines of kind code into output, return SUCCESS or exeption*/
Exeption parseKindCode(char *command, char *line, Word **output, int *L)
{
    int i, commandFound;
    char *firstOp = NULL, *secondOp = NULL, *temp, *temp2;
    commandFound = NOT_FOUND;
    /*check if the command exists*/
    for(i = 0; i < AMOUNT_OF_COMMANDS && commandFound == NOT_FOUND; i++)
    {
        if(strcmp(command, COMMAND_NAME_WORDS[i]) == 0)/*if the command equals to the command name from the array change the commandFound*/
            commandFound = i;
    }
    if(commandFound == NOT_FOUND){/*if the command dont exists return error*/
        return COMMAND_NOT_FOUND;
    }

    /*everything else in parsing the command*/
    /*check if the command is from the first group of 2 operands, this group opcodes are from 0 to 3 and 6*/
    if((commandFound >= OPCODE_MOV && commandFound <=OPCODE_SUB) || commandFound == OPCODE_LEA)
    {
        /*two operands*/
        *L = 3; /*this type of operands need 3 words*/
        firstOp  = strtok(line, " \t\n");
        if(firstOp == NULL)/*if there is no firstOp return error*/
            return MISSING_OPERATORS;
        temp = strtok(NULL, " \n\t"); /*gets the next word*/
        if(temp == NULL)/*if temp is null there is no rest of line after firstOp*/
        {
            temp = (char *)malloc(strlen(firstOp)*sizeof(char)); /*malloc place for temp*/
            
            strcpy(temp,firstOp); /*make a copy of firstOp*/
            if(strchr(temp, ',') != NULL) /*if there is ',' in the string the two operators are there*/
            {
                firstOp = strtok(firstOp, " \t,"); /*make the firstOp without the secondOp*/
                if(firstOp == NULL)/*if there is no firstOp return error*/
                    return MISSING_OPERATORS;
                strtok(temp, ","); /*remove the firstOp from temp*/
                temp2 = strtok(NULL,""); /*gets the part after the comma from the strtok*/
                if(temp2 != NULL)
                {
                    /*copy the secondOp into his var*/
                    secondOp = (char *)malloc((strlen(temp2))*sizeof(char));
                    memmove(secondOp, temp2, strlen(temp2));
                }else
                    secondOp = NULL; /*if temp2 is null secondop is null*/
            }
            free(temp);/*free the uneeded temp var*/
            if(secondOp == NULL) /*if there is not second op return error*/
                return MISSING_OPERATORS;
            if(strchr(secondOp,',') != NULL)
                return EXTRA_COMMA; /*already removed 1 comma, any extra found comma is illigal*/
        }
        else /*temp != NULL*/
        {
            if(strlen(firstOp) == 1 && firstOp[0] == ',')
                return MISSING_OPERATORS; /*if firstOp is only comma then a operator is missing*/
            if(temp[0] != ',' && firstOp[strlen(firstOp)-1] != ',' )/*if the last char in the first operand is comman or the first of the second ther is no missing comma*/
                return MISSING_COMMA;
            if(temp[0] == ',' && firstOp[strlen(firstOp)-1] == ',' )/*there is extra comma (2 commas)*/
                return EXTRA_COMMA;
            /*decide how to get the parameter to the secondOp var*/
            if(strlen(temp) > 1 && temp[0] == ',')/*if there is comma at the start and temp is longer then 1*/
            {
                secondOp = (char *)malloc((strlen(temp)-1)*sizeof(char));
                memmove(secondOp, temp+1, strlen(temp)-1);
            }
            else if(strlen(temp) > 0 && temp[0] != ',')/*there is no comma in the start of themp and temp is longer then 0*/
            {
                secondOp = (char *)malloc((strlen(temp))*sizeof(char));
                memmove(secondOp, temp, strlen(temp));
            }
            else
            {
                temp = strtok(NULL, " \t\n"); /*take the next word from strtok*/
                if(temp != NULL)
                {
                    secondOp = (char *)malloc((strlen(temp))*sizeof(char));
                    memmove(secondOp, temp, strlen(temp));
                }else
                    secondOp = NULL; /*if there wasnt any word in temp make secondOp null*/

            }


            if(secondOp == NULL)/*if there is not second op return error*/
                return MISSING_OPERATORS;
            if(strcmp(secondOp,",") == 0 && strtok(NULL, " \t\n") != NULL)
                return EXTRA_COMMA; /*check if there is continue to the line there is extra comma*/
            if(strchr(secondOp,',') != NULL)
                return GARBUIGE_AT_THE_END; /*already removed 1 comma, any extra found comma is illigal*/

        }
        /*check if the two operator types are both regirsters.*/
        if(operandFormat(firstOp) == ADDR_MODE_REGISTER && operandFormat(secondOp) == ADDR_MODE_REGISTER)
            *L=2; /*if there are 2 registers as the operators saave 2 word because you can put to regirsters in the same word*/
    }
    /*check if the command is from the second group of 1 operands, this group opcodes are 4,5 and 7 to 13 */
    else if(commandFound == OPCODE_NOT || commandFound == OPCODE_CLR || (commandFound >= OPCODE_INC && commandFound <= OPCODE_JSR))
    {
        /*one operand*/
        *L = 2; /*this type of operands need 2 words*/
        firstOp  = strtok(line, " \t\n");
        if(firstOp == NULL) /*if firstOp is missing return error*/
            return MISSING_OPERATORS;
        if(strchr(firstOp, ',') != NULL)
            return GARBUIGE_AT_THE_END; /*extra comma/extra chars*/
    }
    /*check if the command is from the third group of 0 operands, this group opcodes are 14 and 15*/
    else
    {
        /*zero operands*/
        *L = 1; /*this type of operands need 1 word*/
        if(strtok(line, " \t\n") != NULL)
            return GARBUIGE_AT_THE_END; /*there is extra chars that should not be there*/
    }

    /*malloc place to put the bit words into */
    *output = (Word *)malloc(sizeof(Word)*(*L));
    buildFirstWordCode(output, commandFound, firstOp, secondOp);/*make the first word of the command*/

    /*check if there is gurbuige in the end of the line return error*/
    temp = strtok(NULL, " \t\n");
    if(temp != NULL) /*if at the end of the line there is text that is not a comment its illigal*/
        return GARBUIGE_AT_THE_END;
    /*succedded with no errors*/
    return SUCCESS;
}

/*this function builds the first word of command and put it into output*/
void buildFirstWordCode(Word **output, int opCode, char *firstOp, char *secondOp)
{
    Word *cword;
    cword = &((*output)[0]); /*make cword point to the respective word in memory*/
    (cword->command).opcode = opCode; /*make the word opcode the command opcode*/
    (cword->command).type = TYPE_A; /*the first word type is always A*/
    /*set the operands mode in the word*/
    if(firstOp != NULL && secondOp != NULL) /*there are two word*/
    {
        (cword->command).sourceOp = operandFormat(firstOp); /*set the targetOp to the format of firstOp*/
        (cword->command).targetOp = operandFormat(secondOp); /*set the secondOp to the format if secondOp*/
        free(secondOp);/*free the memory from secondOp*/
    }
    else if(firstOp != NULL && secondOp == NULL)/*there is firstOp and no secondOp*/
    {
        (cword->command).targetOp = operandFormat(firstOp); /*set the destOp to the format of firstOp*/
        (cword->command).sourceOp = 0; /*set targetOp to 0*/
    }
    else /*no operands- set both of the operands to 0*/
    {
        (cword->command).targetOp = 0;
        (cword->command).sourceOp = 0;
    }
    /*add the word to output*/
    *output[0] = *cword;
}

/*this function get operand and return his addresing format- 1, 3, 5*/
int operandFormat(char *operand)
{
    if(operand[0] == '@')
        return ADDR_MODE_REGISTER; /*this is a register*/
    else if(operand[0] == '-' || isdigit(operand[0]))
        return ADDR_MODE_IMIDIATE; /*this is a number*/
    else 
        return ADDR_MODE_DIRECT; /*this is not a register or a number so it must be label*/
}

/*this function get line of kind extern and parse it*/
Exeption parseExtern(char *restOfLine,Table *symbolTable)
{
    char *label, *temp;
    Exeption ret;
    label = strtok(restOfLine, " \t\n"); /*put the next word into label*/
    ret = checkLable(label, symbolTable); /*check that the lable is legal*/
    if(ret != SUCCESS) /*if the label is not legal return error*/
        return ret;
    else
    {
        /*add the label into the symbol table*/
        TableRow *trLabel = makeLabel(EXTERNAL, label, K_EXTERN); /*make the label as external and put it into trLabel*/
        TableRow *tr;
        tr = symbolTable->head; /*make tr point to the head of the table*/
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
    temp = strtok(NULL, " \t\n"); /*get the rest of the line*/
    if(temp != NULL) /*if its not null it garbuige*/
        return GARBUIGE_AT_THE_END;
    
    return SUCCESS; /*there wasnt any error, return success*/
}
