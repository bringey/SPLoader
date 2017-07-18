
#ifndef _SPLOADER_SYM_SYMBOLTABLE_H
#define _SPLOADER_SYM_SYMBOLTABLE_H

#ifndef __ASM__

#include <SPLoader/sym/Symbol.h>

typedef struct SymbolTable_s {
    SYM_WORD length;
    Symbol *table;
} SymbolTable;

#endif  // __ASM__

#endif