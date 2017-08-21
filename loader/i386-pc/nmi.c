

#include <SPLoader/i386-pc/nmi.h>
#include <SPLoader/i386/io.h>


void __nmi_enable(void) {

    __outb(0x70, __inb(0x70) & 0x7F);

}

void __nmi_disable(void) {

    __outb(0x70, __inb(0x70) | 0x80);

}