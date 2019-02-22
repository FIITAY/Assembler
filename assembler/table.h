#ifndef TABLE
#define TABLE

enum lineKind {K_DATA, K_STRING, K_EXTERN, K_ENTERY, K_CODE};

typedef struct tableRow
{
    int   name;    /*the name of the row, will be the IC or DC usually*/
    char *content; /*place for the content of the row*/
    enum lineKind symbolKind; /*will point to witch type of symbol is this pointing to*/
    int is_entery; /*if need to be entery will be VAL_TRUE, else VAL_FALSE*/
    struct tableRow *next; /*pointer to the next row of the table*/
} TableRow;

typedef struct table
{
    TableRow* head;
}Table;

/*make new table in the varuble table*/
#define NEW_TABLE(table)\
    table = (Table *)malloc(sizeof(Table));


#endif
