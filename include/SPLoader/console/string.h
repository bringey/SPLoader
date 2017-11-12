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

#ifndef __ASM__

/*
** Stores a decimal string representation of the given value in buf.
*/
int spl_con_cvtdec(char *buf, int value);

/*
** Stores a hexadecimal string representation of the given value in buf.
*/
int spl_con_cvthex(char *buf, int value);

/*
** Stores an octal string representation of the given value in buf.
*/
int spl_con_cvtoct(char *buf, int value);


/*
** Returns the length of the given string
*/
unsigned spl_con_strlen(char *str);

#endif  // __ASM__

#endif