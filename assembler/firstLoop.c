/*File: firstLoop.c
 *Author: Itay Finci
 *
 *in this file I will program the first loop sequence that will take the assembly file and will make all the first words and the symbol
 tables and the data tables. will find error in the first loop but finish looping over all of the file always.
 */


/*includes that will be used in this file*/
#include <string.h>
#include "firstLoop.h"
#include "lineParse.h"
#include "word.h"
#include "consts.h"
#include "table.h"

/*declaring functions that will be programmed in the end of the file*/
void finishTable(Table *, int);
void finishData(DataTable *, int *, Word []);

/*do the first loop over the file fp*/
int doFirstLoop(FILE *fp, Word binaryCode[], Table *symbolTable, DataTable *dataTable, int *instructCount, int *dataCount)
{
    char line[MAX_CHARS_IN_LINE];
    int ret = SUCCESS; /*start the loop thinking that there wont be any errors*/
    int IC = 0,
        DC = 0;/*first step of the first loop */
    int L = 0;
    int i; /*indexes for loops*/
    /*loop over the file line by line (scond step)*/
    while(fgets(line, MAX_CHARS_IN_LINE, fp) != NULL) /*read line from fp, insert it into line ptr and the length to lineLength*/
    {
        Word *parseOutput;
        if(comment(line) != VAL_TRUE)
        {
            L = parseLine(line, &parseOutput,symbolTable, dataTable, &IC, &DC);
            if(L != ERROR_RETURN || L!=0)
            {
                /*add it to the binaryCode array*/
                for(i=0;i<L;i++)
                {
                    /*copy the word from the parsed array to the binary code array to the place IC+i*/
                    binaryCode[IC+i] = parseOutput[i];
                }
                /*finished cycle, bringing the IC to IC+L (step 14)*/
                IC = IC+L;
            }else
                ret = ERROR_RETURN;/*random error because there was error in line*/
        }
        /*comment do nothing*/
    }

    /*if the parsing finished without any errors in the first loop, update the values inside the tables and return the IC*/
    if(ret != ERROR_RETURN)
    {
        *instructCount = IC; /*return the amount of instruction words*/
        *dataCount = DC; /*return the amount of data words*/
        finishTable(symbolTable, IC);
        finishData(dataTable, &IC, binaryCode);
        return IC;
    }
    /*return the error return.*/
    return ret;
}

/*check if the line is comment*/
int comment(char *line)
{
    char temp[MAX_CHARS_IN_LINE]; /*will hold copy of the line*/
    char *firstWord;/*will hold the first word in the line*/
    strcpy(temp,line); /*copy the line*/
    firstWord = strtok(temp, " \t"); /*take the first word in the line and put it inside firstWord*/
    if(firstWord[0] == COMMENT_MARKER)/*if the first char that is not white space is comment marker return true*/
        return VAL_FALSE;
    return VAL_FALSE;/*else false*/
}

/*add the ic to each value in the table*/
void finishTable(Table *st, int IC)
{
    TableRow *tr = st->head;
    while(tr != NULL)/*loop throw while the pointer is not null*/
    {
        if(tr->symbolKind == K_DATA || tr->symbolKind == K_STRING)
        {
            tr->name += IC; /*update the value*/
        }
        tr = tr->next; /*move the pointer to the next row*/
    }
}

/*add the data to the end of the binary code*/
void finishData(DataTable *dt, int *IC, Word binaryCode[])
{
    DataTableRow *dtr = dt->head;
    while(dtr != NULL)/*loop over the whole list*/
    {
        dtr->DC = *IC;/*make the value of the line address the same as the base IC+DC*/
        binaryCode[*IC] = (dtr->content); /*copy the line to the respective cell*/
        /*moce the ic and the dtr forward*/
        *IC = *IC + 1;
        dtr = dtr->next;
    }
}
