
#ifndef _SPLOADER_SYM_SYMBOL_H
#define _SPLOADER_SYM_SYMBOL_H

#ifndef SYM_WORD
#define SYM_WORD unsigned
#endif

#ifndef __ASM__

typedef struct Symbol_s {
    char *name;
    SYM_WORD location;
} Symbol;

#endif  // __ASM__


#endif