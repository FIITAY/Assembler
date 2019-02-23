#ifndef DATATABLE
#define DATATABLE

#include "word.h"
#include "consts.h"

typedef struct dataTableRow
{
    int   DC;    /*the DC value of the line */
    Word content; /*place for the content of the row*/
    struct dataTableRow *next; /*pointer to the next row of the table*/
} DataTableRow;

typedef struct
{
    DataTableRow* head;
}DataTable;

#endif
