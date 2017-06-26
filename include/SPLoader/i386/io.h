/*
** File: include/SPLoader/i386/io.h
**
** Contains routines callable from C for using the x86 in and out instructions
*/

#ifndef _SPLOADER_I386_IO_H
#define _SPLOADER_I386_IO_H


int __inb(int port);

int __inw(int port);

int __inl(int port);

void __outb(int port, int value);

void __outw(int port, int value);

void __outl(int port, int value);
 

#endif