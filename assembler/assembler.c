#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "firstLoop.h"
#include "secondLoop.h"
#include "consts.h"
#include "table.h"
#include "DataTable.h"
#include "word.h"
#include "error.h"

/*
 *This is the main file that will contain the main loops and main function calls.
 */

void fileCompiler(char *);

int main(int argc, char *argv[])
{
    if(argc == 1) /*check if there is more arguments then the program name*/
    {
        /*the program didnt got any file to compile return error messege*/
        printf("\nthere wasnt any files parsed in the arguments to compile.\n");
    }
    else
    {
        /*got arguments, loop throw them*/
        int i; /*used as index for the array*/
        for(i = 1; i<argc; ++i)
        {
            
            /*start the compile of the file with the made name*/
            fileCompiler(argv[i]);
        }
    }
    return 0;
}

void freeTables(Table *);
void freeDataTables(DataTable *);

void printBinary(Word binaryCode[], int length);

/*this function gets a file name and make the first and second loops*/
void fileCompiler(char *fileName)
{
    char *fileWithExtention;
    int ret = SUCCESS;
    FILE *fp;
    Exeption eRet = SUCCESS;
    Word binaryCode[MAX_PROG_LENGTH];
    Table symbolTable;
    Table externalTable;
    DataTable dataTable;

    fileWithExtention = (char *)malloc(strlen(fileName) + 3); /*malloc place for the file name*/;
    strcpy(fileWithExtention, fileName); /*copy the file name*/
    strcat(fileWithExtention, ".as"); /*add the end .as to the file name at the end of the string*/

    /*open the file*/
    fp = fopen(fileWithExtention, "r");
    if(fp == NULL)
    {
        printf("can't open file: %s\n", fileWithExtention);
        return ;
    }
    /*file open success*/
    symbolTable.head = NULL;
    externalTable.head = NULL;
    dataTable.head = NULL;
    ret  = doFirstLoop(fp, binaryCode, &symbolTable, &dataTable);
    if(ret != ERROR_RETURN)/*do the secondLoop only if the first pass returned SUCCESS*/
    {
        /*reopen file at the start*/
        fclose(fp);
        fp = fopen(fileWithExtention, "r");
        if(fp == NULL)
        {
            printf("can't open file: %s\n", fileWithExtention);
            return;
        }
        eRet  = doSecondLoop(fp, binaryCode, &symbolTable, &externalTable);
    }

    if(eRet == SUCCESS)
    {
        /*all of comile finished without errors, make output files*/
        printBinary(binaryCode, ret);
    }/*if there was a errors dont make output files*/

    freeTables(&symbolTable);
    freeTables(&externalTable);
    freeDataTables(&dataTable);

    /*finished the compile of the code*/
}

void print_as_binary(unsigned int a);

void printBinary(Word binaryCode[], int length)
{
    int i=0;
    for (i=0; i<length; i++) {
        print_as_binary(binaryCode[i].data.data);
        printf("\n");
    }
}

/*print unsinged int numbers as binary numbers. */
void print_as_binary(unsigned int a)
{
    int numbits = 12; /*take the maximum size of int in byte and convert it to bit */
    int i,temp;/*for loops and temps*/

    /*the loop print the number from the end to the start of the number*/
    for (i = numbits; i > 0; i--)
    {
        temp = a >> (i-1);/*takes the most right bit*/
        printf("%d", temp & 1);/*if the bit is 1 will print 1 else will print 0*/
    }
}

void freeRow(TableRow *);

void freeTables(Table *t)
{
    TableRow *tr = t->head;
    if(tr != NULL)
        freeRow(tr);
}

void freeRow(TableRow *tr)
{
    if(tr->next != NULL)
        freeRow(tr->next);
    free(tr);
}

void freeDRow(DataTableRow *);

void freeDataTables(DataTable *t)
{
    DataTableRow *dtr = t->head;
    if(dtr != NULL)
        freeDRow(dtr);
}

void freeDRow(DataTableRow *dtr)
{
    if(dtr->next != NULL)
        freeDRow(dtr->next);
    free(dtr);
}
