#ifndef LINEPARSE
#define LINEPARSE

/*File: lineParse.h
 *Author: Itay Finci
 *
 *decleration of function accessable from lineParse.c to other files
 */

#include "word.h"
#include "consts.h"
#include "error.h"
#include "table.h"
#include "DataTable.h"
#include <stdlib.h>

#define NOT_FOUND -1 /*constant of NOT_FOUND exeption across the project*/
#define LABEL_MARKER ':' /*the marker of label is : at the end*/

/*gets line of code and word array to return the machine code in, returnes in the int the number of words generated (L)*/
int parseLine(char *, Word **, Table *, DataTable *, int *, int *);
/*check if label is legal or not*/
Exeption checkLable(char *, Table *);
/*check the kind of line based on the first word*/
enum lineKind checkKind(char *);
/*parse the first word of kind code */
Exeption parseKindCode(char *, char *, Word **, int *);
/*parse data into the dataTable*/
Exeption parseKindData(char *, DataTable *, int *);
/*parse string into the dataTable*/
Exeption parseKindString(char *, DataTable *, int *);
/*make table row from given data*/
TableRow *makeLabel(int, char *, enum lineKind);
/*parse kind extern into the symbolTable*/
Exeption parseExtern(char *,Table *);
/*find the addressing format of operand*/
int operandFormat(char *);
/*check if a number is leagal as data*/
Exeption checkLegalityNumber(char *);

#endif
