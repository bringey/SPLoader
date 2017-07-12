/*
** File: loader/i386-pc/idt/isr.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/i386-pc/idt/isr.h>
#include <SPLoader/i386-pc/idt/IDTGate.h>
#include <SPLoader/loader.h>
#include <SPLoader/i386/x86arch.h>
#include <SPLoader/i386/io.h>
#include <SPLoader/abort.h>
#include <SPLoader/err.h>
#include <SPLoader/console/out.h>

#include <stddef.h>
#include <stdint.h>

#define ISR_TABLE_SIZE 256


ISRHandler IDT_ISR_TABLE[ISR_TABLE_SIZE];


int isr_init(void) {

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

        isr_install(i, isr_defaultUnexpectedHandler, NULL);
    }

    isr_install(INT_VEC_KEYBOARD, isr_defaultExpectedHandler, NULL);
    isr_install(INT_VEC_TIMER, isr_defaultExpectedHandler, NULL);
    //isr_install(INT_VEC_MYSTERY, isr_defaultExpectedHandler, NULL);

    // exceptions (the first 32 ISR vectors are reserved for exceptions)

    for (int i = 0; i != 0x20; ++i) {
        isr_install(i, isr_exceptionHandler, NULL);
    }

    /*
	** ICW1
	*/
	__outb( PIC_MASTER_CMD_PORT, PIC_ICW1BASE | PIC_NEEDICW4 );
	__outb( PIC_SLAVE_CMD_PORT, PIC_ICW1BASE | PIC_NEEDICW4 );

	/*
	** ICW2: master offset of 20 in the IDT, slave offset of 28
	*/
	__outb( PIC_MASTER_IMR_PORT, 0x20 );
	__outb( PIC_SLAVE_IMR_PORT, 0x28 );

	/*
	** ICW3: slave attached to line 2 of master, bit mask is 00000100
	**	 slave id is 2
	*/
	__outb( PIC_MASTER_IMR_PORT, PIC_MASTER_SLAVE_LINE );
	__outb( PIC_SLAVE_IMR_PORT, PIC_SLAVE_ID );

	/*
	** ICW4
	*/
	__outb( PIC_MASTER_IMR_PORT, PIC_86MODE );
	__outb( PIC_SLAVE_IMR_PORT, PIC_86MODE );

	/*
	** OCW1: allow interrupts on all lines
	*/
	__outb( PIC_MASTER_IMR_PORT, 0x00 );
	__outb( PIC_SLAVE_IMR_PORT, 0x00 );

    return E_SUCCESS;
}

int isr_install(int vector, ISRHandler isr, ISRHandler *oldIsr) {

    if (vector >= ISR_TABLE_SIZE) {
        return E_ARGBOUNDS;
    }

    if (oldIsr != NULL) {
        *oldIsr = IDT_ISR_TABLE[vector];
    }

    IDT_ISR_TABLE[vector] = isr;

    return E_SUCCESS;
}

void isr_defaultExpectedHandler(int vector, int code) {
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
        abort("Unexpected 'expected' interrupt");
    }
}

void isr_defaultUnexpectedHandler(int vector, int code) {
    con_printf("vector = 0x%02x, code = 0x%02x\n", vector, code);
    abort("Unexpected interrupt");
}

void isr_exceptionHandler(int vector, int code) {
    char *name;
    // The names for each exception are taken from the intel manual, table 6-1
    switch (vector) {
        case INT_VEC_DIVIDE_ERROR:
            name = "Divide Error";
            break;
        case INT_VEC_DEBUG_EXCEPTION:
            name = "Debug Exception";
            break;
        case INT_VEC_NMI_INTERRUPT:
            name = "NMI Interrupt";
            break;
        case INT_VEC_BREAKPOINT:
            name = "Breakpoint";
            break;
        case INT_VEC_INTO_DETECTED_OVERFLOW:
            name = "Overflow";
            break;
        case INT_VEC_BOUND_RANGE_EXCEEDED:
            name = "BOUND Range Exceeded";
            break;
        case INT_VEC_INVALID_OPCODE:
            name = "Invalid Opcode";
            break;
        case INT_VEC_DEVICE_NOT_AVAILABLE:
            name = "Device Not Available";
            break;
        case INT_VEC_DOUBLE_EXCEPTION:
            name = "Double Fault";
            break;
        case INT_VEC_COPROCESSOR_OVERRUN:
            name = "Coprocessor Segment Overrun";
            break;
        case INT_VEC_INVALID_TSS:
            name = "Invalid TSS";
            break;
        case INT_VEC_SEGMENT_NOT_PRESENT:
            name = "Segment Not Present";
            break;
        case INT_VEC_STACK_FAULT:
            name = "Stack Segment Fault";
            break;
        case INT_VEC_GENERAL_PROTECTION:
            name = "General Protection";
            break;
        case INT_VEC_PAGE_FAULT:
            name = "Page Fault";
            break;
        case INT_VEC_COPROCESSOR_ERROR:
            name = "Math Fault";
            break;
        case INT_VEC_ALIGNMENT_CHECK:
            name = "Alignment Check";
            break;
        case INT_VEC_MACHINE_CHECK:
            name = "Machine Check";
            break;
        case INT_VEC_SIMD_FP_EXCEPTION:
            name = "SIMD Floating-Point Exception";
            break;
        case INT_VEC_VIRT_EXCEPTION:
            name = "Virtualization Exception";
            break;
        default:
            name = "Reserved";
            break;
    }
    con_printf("Exception %02x, Code: %x (%s)\n", vector, code, name);
    abort("Exception");
}