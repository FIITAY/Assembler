#ifndef DATATABLE
#define DATATABLE

/*File: DataTable.h
 *Author: Itay Finci
 *
 *declaring a DataTable struct that will be used to hold the data of .data or .string in the first loop
 */

/*when using the wordTable need to include thous files*/
#include "word.h"
#include "consts.h"

/*defining a row in the table that will hold the pointer to the next row*/
typedef struct dataTableRow
{
    int   DC;    /*the DC value of the line */
    Word content; /*place for the content of the row*/
    struct dataTableRow *next; /*pointer to the next row of the table*/
} DataTableRow;

/*defining the table as a liked list of rows*/
typedef struct
{
    DataTableRow *head;
}DataTable;

#endif
