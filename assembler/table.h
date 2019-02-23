#ifndef TABLE
#define TABLE

#include "consts.h"

enum lineKind {K_DATA, K_STRING, K_EXTERN, K_ENTERY, K_CODE, K_NULL};

typedef struct tableRow
{
    int   name;    /*the name of the row, will be the IC or DC usually*/
    char content[MAX_CHARS_IN_LINE]; /*place for the content of the row*/
    enum lineKind symbolKind; /*will point to witch type of symbol is this pointing to*/
    int is_entery; /*if need to be entery will be VAL_TRUE, else VAL_FALSE*/
    struct tableRow *next; /*pointer to the next row of the table*/
} TableRow;

typedef struct table
{
    TableRow* head;
}Table;

#endif
