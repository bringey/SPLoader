/*
** File: tools/src/install.h
**
** Author: bringey
**
*/

#ifndef _INSTALL_H
#define _INSTALL_H

#include "common.h"

typedef int (*InstallBootFunc)(Prog*);
typedef int (*InstallLoaderFunc)(Prog*);


typedef struct Target {

    const char *name;
    int (*installBoot)(Prog*);
    int (*installLoader)(Prog*);

} Target;


#endif // _INSTALL_H