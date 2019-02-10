#include <stdio.h>
#include "firstLoop.h"
#include "lineParse.h"
#include "word.h"
#include "consts.h"
#include "table.h"

void finishTable(table *, int);

/*do the first loop over the file fp*/
int doFirstLoop(FILE *fp, word **binaryCode, table **symbolTable)
{
    char *line;
    int lineLength = 0;
    int ret = SUCCESS; /*start the loop thhinking that there wont be any errors*/
    int IC = 0,
        DC = 0;/*first step of the first loop */
    int L = 0;
    int i; /*indexes for loops*/
    /*loop over the file line by line (scond step)*/
    while(fgets(line, MAX_CHARS_IN_LINE, fp)) /*read line from fp, insert it into line ptr and the length to lineLength*/
    {
        word *parseOutput;
        L = parseLine(line, &parseOutput);
        /*add it to the binaryCode array*/
        for(i=0;i<L;i++)
        {
            /*copy the word from the parsed array to the binary code array to the place IC+i*/
            (*binaryCode)[IC+i] = parseOutput[i]; 
        }
        /*finished cycle, bringing the IC to IC+L (step 14)*/
        IC = IC+L;
    }

    finishTable(*symbolTable, IC);

    return ret;
}

/*add the ic to each value in the table*/
void finishTable(table *st, int IC)
{
    tableRow *tr = st->head;
    while(tr != NULL)/*loop throw while the pointer is not null*/
    {
        tr->name += IC; /*update the value*/
        tr = tr->next; /*move the pointer to the next row*/
    }
}