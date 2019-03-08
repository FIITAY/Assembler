/*File: base64.h
 *Author: Itay Finci
 *
 *header file for the external functions from base64.c
 */

#ifndef BASE64
#define BASE64

/*this function gets a word and output the base64 presentation into out, out need to already be malloced*/
void turnToBase(Word, char *);

#endif /* base64_h */
