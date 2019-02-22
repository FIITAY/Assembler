#ifndef LINEPARSE
#define LINEPARSE

#include "word.h"
#include "consts.h"
#include "error.h"
#include "table.h"
#include "DataTable.h"
#include <stdlib.h>
#define NOT_FOUND -1
#define LABEL_MARKER ':'
/*gets line of code and word array to return the machine code in, returnes in the int the number of words generated (L)*/
int parseLine(char *, Word **, Table **, DataTable **, int *, int *);

int findChar(char *, char);
Exeption checkLable(char *, Table **);
enum lineKind checkKind(char *);
Exeption parseKindCode(char *, char *, Word **, int *);
TableRow *makeLabel(int, char *, enum lineKind);
Exeption parseExtern(char *,Table **);
int operandFormat(char *);
Exeption checkLegalityNumber(char *);

#endif
