/*
** File: src/version.c
**
** Author: bringey
**
*/ 


#include <loader/version.h>

#ifndef VERSION
#warning VERSION was not defined: using default 0.0.0
#define VERSION 0.0.0
#endif


const char *VERSION_STR = VERSION;
