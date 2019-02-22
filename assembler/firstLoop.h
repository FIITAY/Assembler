#ifndef FIRSTLOOP
#define FIRSTLOOP

#include <stdio.h>
#include "word.h"
#include "table.h"
#include "DataTable.h"
/*the only accssesable function,
will get the file to compile and pointer to the final binary array and the tables*/
int doFirstLoop(FILE *, Word *, Table **, DataTable **);

#endif
