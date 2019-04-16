/*
** File: loader/i386-pc/idt/isr.c
**
** Author: bringey
**
*/

#include <loader/i386-pc/idt/isr.h>
#include <loader/i386-pc/idt/IDTGate.h>
#include <loader/i386-pc/pic.h>
#include <loader/i386/io.h>
#include <loader/abort.h>
#include <loader/err.h>
#include <loader/console.h>

#include <loader/i386-pc/loader.h>

#include <stddef.h>
#include <stdint.h>

#define ISR_TABLE_SIZE 256


ISRHandler IDT_ISR_TABLE[ISR_TABLE_SIZE];


int __isr_init(void) {

    // defined in stubs.S
    extern uint32_t IDT_ISR_STUBTABLE[ISR_TABLE_SIZE];

    uint32_t stub;
    IDTGate *gate;

    for (int i = 0; i != ISR_TABLE_SIZE; ++i) {
        stub = IDT_ISR_STUBTABLE[i];
        gate = (IDTGate*)LOADER_IDT_ADDRESS + i;
        gate->offset_lo = (uint16_t)(stub & 0xFFFF);
        gate->selector = LOADER_GDT_CODE32;
        gate->flags = (uint16_t)(IDT_PRESENT | IDT_DPL_0 | IDT_INT32_GATE);
        gate->offset_hi = (uint16_t)((stub >> 16) & 0xFFFF);

        __isr_install(i, __isr_defaultUnexpectedHandler, NULL);
    }

    __isr_install(INT_VEC_KEYBOARD, __isr_defaultExpectedHandler, NULL);
    __isr_install(INT_VEC_TIMER, __isr_defaultExpectedHandler, NULL);
    //__isr_install(INT_VEC_MYSTERY, isr_defaultExpectedHandler, NULL);

    // exceptions (the first 32 ISR vectors are reserved for exceptions)

    for (int i = 0; i != 0x20; ++i) {
        __isr_install(i, __isr_exceptionHandler, NULL);
    }

    /*
    ** ICW1
    */
    __outb(PIC_MASTER_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_NEEDICW4);
    __outb(PIC_SLAVE_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_NEEDICW4);

    /*
    ** ICW2: master offset of 20 in the IDT, slave offset of 28
    */
    __outb(PIC_MASTER_DATA_PORT, 0x20);
    __outb(PIC_SLAVE_DATA_PORT, 0x28);

    /*
    ** ICW3: slave attached to line 2 of master, bit mask is 00000100
    ** slave id is 2
    */
    __outb(PIC_MASTER_DATA_PORT, 4);
    __outb(PIC_SLAVE_DATA_PORT, 2);

    /*
    ** ICW4
    */
    __outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
    __outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);

    /*
    ** OCW1: allow interrupts on all lines
    */
    __outb(PIC_MASTER_DATA_PORT, 0x00);
    __outb(PIC_SLAVE_DATA_PORT, 0x00);

    return E_SUCCESS;
}

int __isr_install(int vector, ISRHandler isr, ISRHandler *oldIsr) {

    if (vector >= ISR_TABLE_SIZE) {
        return E_ARGBOUNDS;
    }

    if (oldIsr != NULL) {
        *oldIsr = IDT_ISR_TABLE[vector];
    }

    IDT_ISR_TABLE[vector] = isr;

    return E_SUCCESS;
}

void __isr_defaultExpectedHandler(int vector, int code) {
    (void)code;

    //con_printf("[INT] %02x %02x ", vector, code);

    // default handler that does nothing
    // just tell the PIC that we're done

    if (vector >= 0x20 && vector < 0x30) {
        __outb(PIC_MASTER_CMD_PORT, PIC_EOI);
        if (vector > 0x27) {
            __outb(PIC_SLAVE_CMD_PORT, PIC_EOI);
        }
    } else {
        error("Unexpected 'expected' interrupt");
    }
}

void __isr_defaultUnexpectedHandler(int vector, int code) {
    con_printf("vector = 0x%02x, code = 0x%02x\n", vector, code);
    error("Unexpected interrupt");
}

void __isr_exceptionHandler(int vector, int code) {
    con_printf("Exception %02x, Code: %x\n", vector, code);
    abort();
}