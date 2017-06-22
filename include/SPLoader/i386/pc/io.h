

#ifndef _SPLOADER_I386_PC_IO_H
#define _SPLOADER_I386_PC_IO_H


int __inb(int port);

int __inw(int port);

int __inl(int port);

void __outb(int port, int value);

void __outw(int port, int value);

void __outl(int port, int value);
 

#endif