
#ifndef _SPLOADER_MOD_MODHEADER_H
#define _SPLOADER_MOD_MODHEADER_H

#define MODULE_SIGNATURE "SPLOADER MODULE!"
#define MODULE_SIGNATURE_LEN 16
#define MODULE_NAME_LEN 32


#ifndef __ASM__

#include <stdint.h>

typedef struct ModuleHeader_s {

    char signature[MODULE_SIGNATURE_LEN];
    char name[MODULE_NAME_LEN];
    uint32_t version;
    uint32_t dependencyTableSize;
    uint32_t dependencyTableOffset;
    uint32_t executableFormat; // format of the executable file (ie PE, ELF)
    uint32_t executableSize;   // size in bytes of the executable file
    uint32_t executableOffset; // location of the executable file

} ModuleHeader;

#endif


#endif