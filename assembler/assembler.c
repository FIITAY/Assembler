#include <stdio.h>
#include <string.h>
#include "firstLoop.h"
#include "secondLoop.h"

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

/*this function gets a file name and make the first and second loops*/
void fileCompiler(char *fileName)
{
    char *fileWithExtention = (char *)maloc(strlen(fileName) + 3); /*malloc place for the file name*/
    int ret;
    strcpy(fileWithExtention, fileName); /*copy the file name*/
    strcat(fileWithExtention, ".as"); /*add the end .as to the file name at the end of the string*/

    /*open the file*/
    FILE *fp = fopen(fileWithExtention, "r");
    if(fp == NULL)
    {
        printf("can't open file: %s\n", fileWithExtention);
        return ;
    }
    /*file open success*/
    ret  = doFirstPass(fp);
    if(ret == SUCCESS)/*do the secondLoop only if the first pass returned SUCCESS*/
        ret  = doSecondPass(fp);

    if(ret == SUCCESS)
    {
        /*all of comile finished without errors, make output files*/
    }/*if there was a errors dont make output files*/

    /*finished the compile of the code*/
}
