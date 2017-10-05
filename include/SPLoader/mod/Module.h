
#ifndef _SPLOADER_MOD_MODULE_H
#define _SPLOADER_MOD_MODULE_H

#ifndef __ASM__

typedef struct Module_s {

    ModuleHeader header;        // copy of the header loaded from the module file
    uint32_t executableSize;    // size in bytes of the module executable
    void* executableLocation;   // memory location of the loaded executable
    int (*init)(void);
    int (*deinit)(void);

} Module;


#endif


#endif