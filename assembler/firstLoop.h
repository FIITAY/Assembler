#ifndef FIRSTLOOP
#define FIRSTLOOP

/*File: firstLoop.h
 *Author: Itay Finci
 *
 *in this file i will handle the declaration of the functions that are needed to be accessed from the outside of the first loop
 */

#include <stdio.h>
#include "word.h"
#include "table.h"
#include "DataTable.h"
/*the only accssesable function,
will get the file to compile and pointer to the final binary array and the tables*/
int doFirstLoop(FILE *, Word *, Table *, DataTable *, int *, int *);
/*this function get a line and check if this line is comment line*/
int comment(char []);

#endif
