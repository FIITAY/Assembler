#ifndef TABLE
#define TABLE

/*File: table.c
 *Author: Itay Finci
 *
 *this file declare a table that will be used as symbol table and extern table
 */

#include "consts.h"

/*the kinds of lines that can be*/
enum lineKind {K_DATA, K_STRING, K_EXTERN, K_ENTERY, K_CODE, K_NULL};

/*the definition of row in the table, each row point to the next row*/
typedef struct tableRow
{
    int   name;    /*the name of the row, will be the IC or DC usually*/
    char content[MAX_CHARS_IN_LINE]; /*place for the content of the row*/
    enum lineKind symbolKind; /*will point to witch type of symbol is this pointing to*/
    int is_entery; /*if need to be entery will be VAL_TRUE, else VAL_FALSE*/
    struct tableRow *next; /*pointer to the next row of the table*/
} TableRow;

/*the head of the table will be declared as this struct*/
typedef struct table
{
    TableRow* head;
}Table;

#endif
