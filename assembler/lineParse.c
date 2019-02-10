#include "word.h"
#include "consts.h"
#include "error.h"
#include "table.h"
#include <string.h>
#include <stdlib.h>
#define NOT_FOUND -1
#define LABEL_MARKER ':'

enum lineKind {K_DATA, K_STRING, K_EXTERN, K_ENTERY, K_ELSE};

int findChar(char *, char);
int checkLable(char *, int, table **);
enum lineKind checkKind(char *, int);


int parseLine(char *line, word **, table **symbolTable)
{
    int L, labelEndIndex;
    enum lineKind kind;
    if((labelEndIndex = findChar(line, LABEL_MARKER)) != NOT_FOUND)
    {
        checkLable(line, labelEndIndex, symbolTable);
    }

    kind = checkKind(line, labelEndIndex);
    if(kind == K_DATA || kind == K_STRING)
    {
        /*steps 5-7*/
    }
    if(kind == K_ENTERY || kind == K_EXTERN)
    {
        /*steps 8-10*/
    }
    if(kind == K_ELSE)
    {
        /*steps 11-15*/
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
            return VAL_FALSE;
        }
    }
    strncpy(str, line, endIndex-1);
    /*check that the label is not one of the saved words*/
    for(i=0;i < AMOUNT_SAVED_WORDS; i++)
    {
        if(strcmp(str, SAVEDWORDS[i]) == 0)
        {
            errorHandle(line, ILIGAL_LABEL_NAME);
            return VAL_FALSE;
        }
    }
/*check that the name isnt in the label list already*/
    while(tr != NULL)
    {
        if(strcmp(str, tr->content) == 0)
        {
            errorHandle(line, LABEL_ALREADY_EXSIST);
            return VAL_FALSE;
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
    return K_ELSE;
}

