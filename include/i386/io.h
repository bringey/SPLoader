/*
** File: include/i386/io.h
**
** Contains routines callable from C for using the x86 in and out instructions
*/

#ifndef _I386_IO_H
#define _I386_IO_H

#ifndef __ASM__


int __inb(int port);

int __inw(int port);

int __inl(int port);

void __outb(int port, int value);

void __outw(int port, int value);

void __outl(int port, int value);

#endif  // __ASM__

#endif