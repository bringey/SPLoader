/*
** File: tools/src/target/i386-pc.h
**
** Author: bringey
**
*/

#ifndef _TARGET_I386_PC_H
#define _TARGET_I386_PC_H

#include "../install.h"

void target_i386pc_setDefaults(Prog *prog);

void target_i386pc_installBoot(Prog *prog);

void target_i386pc_installLoader(Prog *prog);


#endif