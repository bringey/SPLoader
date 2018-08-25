
#ifndef _STRING_H
#define _STRING_H

#ifndef __ASM__

#include <stdbool.h>
#include <stddef.h>

#define spl_isupper(ch) (ch >= 'A' && ch <= 'Z')
#define spl_islower(ch) (ch >= 'a' && ch <= 'z')

#define spl_toupper(ch) (ch - ('a' - 'A'))
#define spl_tolower(ch) (ch + ('a' - 'A'))


//long spl_strtol(const char *str, char **endptr, int base);


// long int to string
int spl_ltostr(long i, unsigned base, char *buf);

// unsigned long int to string
int spl_ultostr(unsigned long i, unsigned base, char *buf);


//
// Convert all lowercase characters in str to uppercase
//
int spl_strupper(char *str);

//
// Convert all uppercase characters in str to lowercase
//
int spl_strlower(char *str);


size_t spl_strlen(const char *str);

#endif  // __ASM__


#endif