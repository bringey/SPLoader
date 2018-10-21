/*
** File: include/loader/string.h
**
** Author: bringey
*/

#ifndef _LOADER_STRING_H
#define _LOADER_STRING_H

#ifndef __ASM__

#include <stdbool.h>
#include <stddef.h>

#define isupper(ch) (ch >= 'A' && ch <= 'Z')
#define islower(ch) (ch >= 'a' && ch <= 'z')

#define toupper(ch) (ch - ('a' - 'A'))
#define tolower(ch) (ch + ('a' - 'A'))


//long strtol(const char *str, char **endptr, int base);


// long int to string
int ltostr(long i, unsigned base, char *buf);

// unsigned long int to string
int ultostr(unsigned long i, unsigned base, char *buf);


void* memcpy(void *dest, const void *source, size_t num);

//
// Convert all lowercase characters in str to uppercase
//
int strupper(char *str);

//
// Convert all uppercase characters in str to lowercase
//
int strlower(char *str);


size_t strlen(const char *str);

#endif  // __ASM__


#endif