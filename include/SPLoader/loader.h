#ifndef _SPLOADER_LOADER_H
#define _SPLOADER_LOADER_H

//
// Location where loader_early.bin gets loaded to
//
#define LOADER_EARLY_SEGMENT 0x7C0
#define LOADER_EARLY_ADDRESS 0x200

//
// Where loader.bin gets loaded to
//
#define LOADER_ADDRESS 0x10000
#define LOADER_SEGMENT 0x01000

//
// Location of the loader's GDT
//
#define LOADER_GDT_SEGMENT 0x01100
#define LOADER_GDT_ADDRESS 0x11000


#endif