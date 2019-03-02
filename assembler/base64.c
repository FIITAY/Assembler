/*
 *This file will contain the function for the base64 haneling
 */

#include <stdlib.h>
#include <stdio.h>
#include "word.h"
#define CHAR_PER_WORD 2
#define BITS_PER_NUMBER 6
#define LAST_CAPITAL_INDEX 25
#define LAST_LOWER_INDEX 51
#define LAST_NUMBER_INDEX 61
#define PLUS_INDEX 62

char *turnToBase(Word w);

int main()
{
    Word t;
    char *c;
    t.data.data= 2572;
    c = turnToBase(t);
    putchar(c[0]);
    putchar(c[1]);
    free(c);
}




char intToBase(int);

char *turnToBase(Word w)
{
    char *out;
    int firstChar = 0, secondChar = 0;/*reseting the two chars*/
    int i, mask  = 1; /*index*/
    /*copy the first 6 bits into the firstChar int*/
    for(i = 0; i < BITS_PER_NUMBER; i++)
    {
        firstChar |= w.data.data & mask; /*copy the bit with the mask to first char*/
        mask = mask << 1; /*move the mask*/
    }
    /*copy the second 6 bits to the secondChar int*/
    for(i = 0; i< BITS_PER_NUMBER; i++)
    {
        secondChar |= w.data.data & mask; /*copy the bit with the mask to second char*/
        mask = mask << 1; /*move the mask*/
    }

    out = (char *)malloc(sizeof(char) * CHAR_PER_WORD);

    out[0] = intToBase(firstChar);
    out[1] = intToBase(secondChar);

    return out;
}

char intToBase(int num)
{
    if(num <= LAST_CAPITAL_INDEX)
        return 'A' + num;
    if(num > LAST_CAPITAL_INDEX && num <= LAST_LOWER_INDEX)
        return 'a' + (num-LAST_CAPITAL_INDEX);
    if(num > LAST_LOWER_INDEX && num <= LAST_NUMBER_INDEX)
        return '0' + (num- LAST_NUMBER_INDEX);
    if(num == PLUS_INDEX)
        return '+';
    else
        return '/';
}
