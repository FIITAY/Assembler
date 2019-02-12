#include "word.h"
#include "consts.h"
#include "error.h"
#include "table.h"
#include <string.h>
#include <stdlib.h>
#define NOT_FOUND -1
#define LABEL_MARKER ':'
#define ERROR_RETURN -10



int findChar(char *, char);
int checkLable(char *, int, table **);
enum lineKind checkKind(char *, int);


int parseLine(char *line, word **output, table **symbolTable, int IC)
{
    int L, labelEndIndex, returnValue;
    enum lineKind kind;
    if((labelEndIndex = findChar(line, LABEL_MARKER)) != NOT_FOUND)
    {
        if(checkLable(line, labelEndIndex, symbolTable) == ERROR_RETURN)
            return ERROR_RETURN;
    }

    kind = checkKind(line, labelEndIndex);
    if(kind == K_DATA || kind == K_STRING)
    {
        /*steps 5-7*/

        L=0; /*if there is data or string the IC dont need to be updated*/
    }
    if(kind == K_ENTERY || kind == K_EXTERN)
    {
        /*steps 8-10*/


        L=0; /*if there is entery or extern the IC dont need to be updated*/
    }
    if(kind == K_CODE)
    {
        int isExsist = VAL_FALSE, i;
        char *str;
        char cmp[5];
        int length =0, lineLength;
        int commandFound = NOT_FOUND;
        /*steps 11-15*/
        if(labelEndIndex != NOT_FOUND)
        {
            /*enter the label to the symbol table as kind code*/
            TableRow *tr = (*symbolTable)->head; /*make pointer to the symbol table*/
            TableRow *label = (TableRow *) malloc(sizeof(TableRow));/*malloc place for the new row*/
            label->name = IC; /*the new row needs the IC as the name*/
            strncpy(label->content, line, labelEndIndex-1); /*copy the name of the label to the field in the row*/
            label->symbolKind = K_CODE; /*the kind of the label is code*/
            while(tr->next != NULL)/*finds the lasst row in the table*/
                tr = tr->next;
            tr->next = label; /*make the last row the new label*/
        }

        if(labelEndIndex == NOT_FOUND)
            str= line;
        else
            str = line + labelEndIndex + 1;
        lineLength = strlen(str);
        while(length<4 && length < lineLength && ((str[length] >= 'a' && str[length] <= 'z') || str[length] == '.' ))
        {
            /*not white space, put it in the command string*/
            cmp[length] = str[length]; 
            length++; /*addvance the index*/
        }
        cmp[length] = '\0'; /*make the string \0 terminated*/
        memmove(cmp, cmp, length);
        /*check if the command exists*/
        for(i = 0; i < AMOUNT_OF_COMMANDS && commandFound == NOT_FOUND; i++)
        {
            if(strcmp(cmp, COMMAND_NAME_WORDS[i]) == 0)
                commandFound = i;
        }
        if(commandFound == NOT_FOUND){
            errorHandle(line, Exeption exep);
            return ERROR_RETURN;
        }
        
    }

    return L;
}

/*this function finds the first show of char c in string line*/
int findChar(char *line, char c)
{
    int i, length;
    length = strlen(line);
    for(i=0; i < length; i++)
    {
        if(*(line+i) == c)
            return i;
    }
    return NOT_FOUND;
}

/*check that all of the characters are from the abs (capital or not) or numbers*/
int checkLable(char *line, int endIndex, table **symbolTable)
{
    int i;
    char *str;
    TableRow *tr = (*symbolTable)->head;
    /*check that all characters are valid*/
    for(i=0;i<endIndex; i++)
    {
        char c = *(line + i);
        if(!((c >= 'a' && c<= 'z') || (c >= 'A' && c<= 'Z') || (c >= '0' && c<= '9')))
        {
            errorHandle(line, ILIGAL_LABEL_CHARACTERS);
            return ERROR_RETURN;
        }
    }
    strncpy(str, line, endIndex-1);
    /*check that the label is not one of the saved words*/
    for(i=0;i < AMOUNT_SAVED_WORDS; i++)
    {
        if(strcmp(str, SAVEDWORDS[i]) == 0)
        {
            errorHandle(line, ILIGAL_LABEL_NAME);
            return ERROR_RETURN;
        }
    }
/*check that the name isnt in the label list already*/
    while(tr != NULL)
    {
        if(strcmp(str, tr->content) == 0)
        {
            errorHandle(line, LABEL_ALREADY_EXSIST);
            return ERROR_RETURN;
        }
        tr = tr->next;
    }
    return VAL_TRUE;
}

enum lineKind checkKind(char *line, int labelEnd)
{
    char *str;
    char cmp[8];
    int length =0, lineLength;
    if(labelEnd == NOT_FOUND)
        str= line;
    else
        str = line + labelEnd + 1;
    lineLength = strlen(str);
    while(length<7 && length < lineLength && ((str[length] >= 'a' && str[length] <= 'z') || str[length] == '.' ))
    {
        /*not white space, put it in the command string*/
        cmp[length] = str[length]; 
        length++; /*addvance the index*/
    }
    cmp[length] = '\0'; /*make the string \0 terminated*/
    memmove(cmp, cmp, length);
    if(strcmp(cmp, ".data") == 0)
        return K_DATA;
    if(strcmp(cmp, ".string") == 0)
        return K_STRING;
    if(strcmp(cmp, ".extern") == 0)
        return K_EXTERN;
    if(strcmp(cmp, ".entery") == 0)
        return K_ENTERY;
    return K_CODE;
}

