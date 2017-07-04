/*
** File: include/SPLoader/console/string.h
**
** Author: Brennan Ringey
**
** Contains utility int-to-string conversion functions for the console module
**
*/

#ifndef _SPLOADER_CONSOLE_STRING_H
#define _SPLOADER_CONSOLE_STRING_H


/*
** Stores a decimal string representation of the given value in buf.
*/
int con_cvtdec(char *buf, int value);

/*
** Stores a hexadecimal string representation of the given value in buf.
*/
int con_cvthex(char *buf, int value);

/*
** Stores an octal string representation of the given value in buf.
*/
int con_cvtoct(char *buf, int value);


#endif