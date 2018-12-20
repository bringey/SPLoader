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

#define ldr_isupper(ch) (ch >= 'A' && ch <= 'Z')
#define ldr_islower(ch) (ch >= 'a' && ch <= 'z')

#define ldr_toupper(ch) (ch - ('a' - 'A'))
#define ldr_tolower(ch) (ch + ('a' - 'A'))


//long strtol(const char *str, char **endptr, int base);


// long int to string
int ldr_ltostr(long i, unsigned base, char *buf);

// unsigned long int to string
int ldr_ultostr(unsigned long i, unsigned base, char *buf);


void* ldr_memcpy(void *dest, const void *source, size_t num);

//
// Convert all lowercase characters in str to uppercase
//
int ldr_strupper(char *str);

//
// Convert all uppercase characters in str to lowercase
//
int ldr_strlower(char *str);


size_t ldr_strlen(const char *str);

#endif  // __ASM__


#endif