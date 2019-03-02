#ifndef SECONDLOOP
#define SECONDLOOP

/*File: secondLoop.h
 *Author: Itay Finci
 *
 *this is the interface to the secondLoop.c
 */

#include <stdio.h>
#include <string.h>
#include "word.h"
#include "table.h"
#include "error.h"

/*this function gets file and do the second loop over it*/
Exeption doSecondLoop(FILE *, Word [], Table *, Table *);

#endif
