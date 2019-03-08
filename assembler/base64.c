/*File: base64.c
 *Author: Itay Finci
 *
 *this file takes a memory word and show his base64 representation.
 */

#include <stdlib.h>
#include "base64.h"

#define CHAR_PER_WORD 2
#define BITS_PER_NUMBER 6
#define FIRST_LOWER_INDEX 26
#define FIRST_NUMBER_INDEX 52
#define LAST_NUMBER_INDEX 61
#define PLUS_INDEX 62

char intToBase(int);

/*this function gets a word and output the base64 presentation into out, out need to already be malloced*/
void turnToBase(Word w, char *out)
{
    int firstChar = 0, secondChar = 0;/*reseting the two chars*/
    int i, mask  = 1; /*index*/

    /*copy the first 6 bits into the secondChar int*/
    for(i = 0; i < BITS_PER_NUMBER; i++)
    {
        secondChar |= w.data.data & mask; /*copy the bit with the mask to second char*/
        mask = mask << 1; /*move the mask*/
    }
    /*copy the second 6 bits to the firstChar int*/
    for(i = 0; i < BITS_PER_NUMBER; i++)
    {
        firstChar |= w.data.data & mask; /*copy the bit with the mask to first char*/
        mask = mask << 1; /*move the mask*/
    }
    /*shifting the higher opart of the number back to the lower part*/
    firstChar = firstChar >> BITS_PER_NUMBER;

    out[0] = intToBase(firstChar);
    out[1] = intToBase(secondChar);
}

/*takes an index and return the right char in base64
 * In base64, we have first the capital A-Z, than a-z, 0-9, + & /  in this order.
 */
char intToBase(int num)
{
    if(num < FIRST_LOWER_INDEX)
        return 'A' + num;
    if(num < FIRST_NUMBER_INDEX)    /* no need to make sure it is equal or bigger, as it would return if it less */
        return 'a' + (num - FIRST_LOWER_INDEX);
    if(num < PLUS_INDEX)
        return '0' + (num - FIRST_NUMBER_INDEX);
    if(num == PLUS_INDEX)
        return '+';
    else
        return '/';
}
