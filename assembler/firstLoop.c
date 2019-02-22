#include <stdio.h>
#include "firstLoop.h"
#include "lineParse.h"
#include "word.h"
#include "consts.h"
#include "table.h"


void finishTable(Table *, int);

/*do the first loop over the file fp*/
int doFirstLoop(FILE *fp, Word binaryCode[], Table **symbolTable, DataTable **dataTable)
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
        if(line[0] != COMMENT_MARKER)
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

    finishTable(*symbolTable, IC);

    return ret;
}

/*add the ic to each value in the table*/
void finishTable(Table *st, int IC)
{
    TableRow *tr = st->head;
    while(tr != NULL)/*loop throw while the pointer is not null*/
    {
        if(tr->symbolKind == K_DATA || tr->symbolKind == K_STRING)
            tr->name += IC; /*update the value*/
        tr = tr->next; /*move the pointer to the next row*/
    }
}
