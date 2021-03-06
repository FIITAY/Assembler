/*File: assembler.c
 *Author: Itay Finci
 *
 *in this file i will handle the main function that will loop over the files that sent as arguments and compile them.
 */

/*includes that will be used in this file*/
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
#include "base64.h"

/*declaring the compile function that will be programed in the end.*/
void fileCompiler(char *);

/*this is the main function that will be called automaticly when the project is running, will take the files from the argv and compile
 *them.*/
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

/*declaring functions that will be used in the compile but dont need them in the main*/
void freeTables(Table *);
void freeDataTables(DataTable *);

/*this function is for debug only*/
void printBinary(Word binaryCode[], int length);

void exportExternalTable(Table extT, char *fn);
void exportEnteryTable(Table symT, char *fn);
void exportBinaryTable(Word [], char *, int, int, int);

char *filename;

/*this function gets a file name and make the first and second loops*/
void fileCompiler(char *fn)
{
    char *fileWithExtention = NULL;
    int ret = SUCCESS;
    FILE *fp;
    Exeption eRet = SUCCESS;
    Word binaryCode[MAX_PROG_LENGTH];
    Table symbolTable;
    Table externalTable;
    DataTable dataTable;
    int IC, DC;

    fileWithExtention = (char *)malloc((strlen(fn) + 4)*sizeof(char)); /*malloc place for the file name*/
    strcpy(fileWithExtention, fn); /*copy the file name*/
    strcat(fileWithExtention, ".as"); /*add the end .as to the file name at the end of the string*/

    /*open the file*/
    fp = fopen(fileWithExtention, "r");
    if(fp == NULL)/*check if the file was opned successfully*/
    {
        printf("can't open file: %s\n", fileWithExtention);/*there was error opening the file*/
        return ;
    }

    /*update the extern file name*/
    filename = fileWithExtention;

    /*file open success*/
    symbolTable.head = NULL;
    externalTable.head = NULL;
    dataTable.head = NULL;
    /*do the first loop of the compile procidure over the file*/
    ret  = doFirstLoop(fp, binaryCode, &symbolTable, &dataTable, &IC, &DC);
    if(ret != ERROR_RETURN)/*do the secondLoop only if the first pass returned SUCCESS*/
    {
        /*reopen file at the start*/
        fclose(fp);
        fp = fopen(fileWithExtention, "r");
        if(fp == NULL)/*check if the file was opned successfully*/
        {
            printf("can't open file: %s\n", fileWithExtention);/*there was error opening the file*/
            return;
        }
        /*do the second loop of the compile procidure over the file*/
        eRet  = doSecondLoop(fp, binaryCode, &symbolTable, &externalTable);
        fclose(fp);
    }
    /*if the two loops was finished whithout any errors, make the output files, else skip making the output files*/
    if(eRet == SUCCESS)
    {
        /*for debug, print the binary*/
        printBinary(binaryCode, ret);
        /*export the files needed*/
        exportBinaryTable(binaryCode, fn, ret, IC, DC);
        exportExternalTable(externalTable, fn);
        exportEnteryTable(symbolTable, fn);
        /*need to make the export of the .ob file*/
    }/*if there was a errors dont make output files*/

    /*free the memory that was malloced*/
    free(fileWithExtention);
    freeTables(&symbolTable);
    freeTables(&externalTable);
    freeDataTables(&dataTable);

    /*finished the compile of the code*/
}


/*for debug only*/
void print_as_binary(unsigned int a);

/*for debug only*/
void printBinary(Word binaryCode[], int length)
{
    int i;
    for (i=0; i<length; i++) {
        print_as_binary(binaryCode[i].data.data);
        printf("\n");
    }
}

/*for debug only*/
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

void exportBinaryTable(Word binaryCode[], char *fn, int length, int IC, int DC)
{
    FILE *fp = NULL;
    char *fileWithExtention = NULL;
    int i;
    char out[3]; /*place for 2 chars and \0*/
    /*open file*/
    fileWithExtention = (char *)malloc((strlen(fn) + 4)*sizeof(char)); /*malloc place for the file name*/;
    strcpy(fileWithExtention, fn); /*copy the file name*/
    strcat(fileWithExtention, ".ob"); /*add the end .as to the file name at the end of the string*/
    fp = fopen(fileWithExtention, "w"); /*open the file as write mode*/
    if(fp == NULL)
    {
        printf("cannot write to file: %s", fileWithExtention);/*there was error opening the new files*/
        return; /*cant continue the rest of the file*/
    }
    out[2] = '\0'; /*init string end*/
    fprintf(fp, "%d %d\n", IC, DC); /*print the .ob header*/
    for(i=0; i<length; i++)
    {
        turnToBase(binaryCode[i], out);
        fprintf(fp, "%s\n", out);
    }
    free(fileWithExtention);

}

/*this function gets externals table and print it into the file named <fileName>.ext*/
void exportExternalTable(Table extT, char *fn)
{
    TableRow *tr = extT.head;
    char *fileWithExtention = NULL;
    FILE *fp = NULL;
    while(tr != NULL)
    {
        if(fp == NULL)
        {
            /*open file*/
            fileWithExtention = (char *)malloc((strlen(fn) + 5)*sizeof(char)); /*malloc place for the file name*/;
            strcpy(fileWithExtention, fn); /*copy the file name*/
            strcat(fileWithExtention, ".ext"); /*add the end .as to the file name at the end of the string*/
            fp = fopen(fileWithExtention, "w"); /*open the file as write mode*/
            if(fp == NULL)
            {
                printf("cannot write to file: %s", fileWithExtention);/*there was error opening the new files*/
                return; /*cant continue the rest of the file*/
            }
        }
        /*print into the file*/
        fprintf(fp,"%s\t%d\n",tr->content,LOAD_ADDR + tr->name);
        tr = tr->next;
    }
    if(fp != NULL)
        fclose(fp);
    if(fileWithExtention != NULL)
        free(fileWithExtention);

}

/*this function gets the symbol table and if there are entery labels export them into <fileName>.ent*/
void exportEnteryTable(Table symT, char *fn)
{
    FILE *fp = NULL;
    TableRow *tr = symT.head;
    char *fileWithExtention = NULL;
    while(tr != NULL)
    {
        if(tr->is_entery == VAL_TRUE)
        {
            /*only if there is need to print entery open the file to be sure that there is need for this file*/
            if(fp == NULL)
            {
                /*open file*/
                fileWithExtention = (char *)malloc((strlen(fn) + 5)*sizeof(char)); /*malloc place for the file name*/;
                strcpy(fileWithExtention, fn); /*copy the file name*/
                strcat(fileWithExtention, ".ent"); /*add the end .as to the file name at the end of the string*/
                fp = fopen(fileWithExtention, "w"); /*open the file into fp*/
                if(fp == NULL)
                {
                    printf("cannot write to file: %s", fileWithExtention);/*there was error opening the new files*/
                    return; /*cant continue the rest of the file*/
                }
            }
            /*print into the file*/
            fprintf(fp,"%s\t%d\n",tr->content,LOAD_ADDR + tr->name);
        }
        tr = tr->next;
    }
    /*make sure to close the file if needed*/
    if(fp != NULL)
        fclose(fp);
    if(fileWithExtention != NULL)
        free(fileWithExtention);
}

void freeRow(TableRow *);

/*function that will take a table and free it using an recursive function.*/
void freeTables(Table *t)
{
    TableRow *tr = t->head;
    if(tr != NULL)
        freeRow(tr);
}

/*this function take a table and free the memory using recursive logic*/
void freeRow(TableRow *tr)
{
    if(tr->next != NULL)/*check if there is next row*/
        freeRow(tr->next);/*free the next row*/
    free(tr);/*when returning from the recursive calles, free this row*/
}

void freeDRow(DataTableRow *);

/*function that will take a dataTable and free it using an recursive function.*/
void freeDataTables(DataTable *t)
{
    DataTableRow *dtr = t->head;
    if(dtr != NULL)
        freeDRow(dtr);
}

/*this function take a dataTable and free the memory using recursive logic*/
void freeDRow(DataTableRow *dtr)
{
    if(dtr->next != NULL)/*check if there is next row*/
        freeDRow(dtr->next);/*free the next row*/
    free(dtr);/*when returning from the recursive calles, free this row*/
}
