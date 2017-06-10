/*
** File: loader/arch/i386/bios/bootstrap.S
**
** Author: Brennan Ringey
**
** Assembly source for the BIOS bootstrap binary for the loader. This
** binary is installed to the MBR and is responsible for loading the loader
** binary.
**
** The boot process:
**     bootstrap.bin -> loader.bin -> kern.bin
*/

#include <arch/i386-pc/bios/bootstrap.h>

BOOT_SEGMENT    = 0x07C0     /* default BIOS addr to load boot sector */
BOOT_ADDRESS    = 0x00007C00
START_SEGMENT   = 0x0000     /* where we'll put the startup code */
START_OFFSET    = 0x00007E00
SECTOR_SIZE     = 0x200      /* typical sector size for floppy & HD */
BOOTSTRAP_SIZE  = 446        /* maximum size of the bootstrap code */ 
OFFSET_LIMIT	= 65536 - SECTOR_SIZE

PARTITION1_LOCATION     = 0x1BE
PARTITION2_LOCATION     = 0x1CE
PARTITION3_LOCATION     = 0x1DE
PARTITION4_LOCATION     = 0x1EE
BOOT_SIGNATURE_LOCATION = 0x1FE

// ===========================================================================
//                      START OF MBR BOOTSTRAP CODE
// ===========================================================================

	.arch i686
	.code16
	.text

	.global begtext

begtext:
/*
** Entry point.	Begin by setting up a runtime stack.
*/
	movw    $BOOT_SEGMENT, %ax  // get our data seg
	movw    %ax, %ds
	movw    %ax, %ss            // stack segment starts at BOOT_SEGMENT
	movw    $0x4000, %ax        // and the stack starts 0x4000 beyond that
	movw    %ax, %sp

/*
** Next, verify that the disk is there and working.
*/
	movb    $0x01, %ah           // test the disk status and make sure
	movb    drive, %dl           // it's safe to proceed
	int     $0x13
	jnc     diskok

	movw    $err_diskstatus, %si // Something went wrong; print a message
	call    dispMsg              // and die.
	jmp     die

diskok:
	movw    $0, %ax              // Reset the disk
	movb    drive, %dl
	int     $0x13

	// Check if BIOS supports INT 13h extensions
	movb    $0x41, %ah
	movb    drive, %dl
	movw    $0x55AA, %bx
	int     $0x13
	jnc     lbasupport

	movw    $err_nolba, %si      // BIOS does not support 0x13 extensions
	call    dispMsg              // print error message and die.
	jmp     die

lbasupport:

/*
** Disk is OK, now time the load the loader binary, typically located right
** after the MBR.
*/
	movw    $msg_go, %si
	call    dispMsg
	jmp die
	//call    loadimage
	// image is loaded, time to setup




/*
** Psuedo-breakpoint, jump here to stop execution
** completely. Useful for debugging.
*/
die:
	hlt
	jmp     die

/*
** Support routine - display a message byte by byte to the monitor.
*/
dispMsg:
	pushw   %ax
	pushw   %bx
repeat:
	lodsb                        // grab next character

	movb    $0x0e, %ah           // write and advance cursor
	movw    $0x07, %bx           // page 0, white on blank, no blink
	orb     %al, %al             // AL is character to write
	jz      getOut               // if we've reached the NUL, get out

	int     $0x10                // otherwise, print and repeat
	jmp     repeat

getOut:                          // we're done, so return
	popw    %bx
	popw    %ax
	ret


/*
** DATA AREAS.
**
** Next sector number and head number to read from.
*/

/*
** Data Address Packet
**
** Offset     Size    Description
** ===========================================================================
**      0     byte    size of packet (16 bytes)
**      1     byte    always 0
**      2     word    number of sectors to transfer (max 127 on some BIOSes)
**      4     long    transfer buffer (16 bit segment:16 bit offset)
**      8     long    lower 32-bits of 48-bit starting LBA
**     12     long    upper 32-bits of 48-bit starting LBAs
*/

dap_size           = dap
dap_sectors        = dap+2
dap_buffer         = dap+4
dap_buffer_segment = dap+6
dap_buffer_offset  = dap_buffer
dap_lowlba         = dap+8
dap_highlba        = dap+12

dap:
	.byte 16
	.byte 0
	.word 0
	.long 0
	.long 0
	.long 0



sec:
	.word 2  // cylinder=0, sector=1
head:
	.word 0  // head=0
max_sec:
	.byte 19 // up to 18 sectors per floppy track
max_head:
	.byte 2  // only two r/w heads per floppy drive

/*
** Status and error messages.
*/
msg_loading:
	.asciz "Loading"
msg_dot:
	.asciz "."
msg_go:
	.asciz "done."
msg_bar:
	.asciz "|"

/*
** Error messages.
*/
err_diskstatus:
	.asciz "Disk not ready.\n\r"
err_diskread:
	.asciz "Read failed\n\r"
err_toobig:
	.asciz "Too big\n\r"
err_diskfail:
	.asciz "Can't proceed\n\r"
err_nolba:
	.asciz "BIOS does not support INT 0x13 extensions\n\r"

/*
** Data areas.
*/

/*
** Location and size of the loader image (loader.bin)
** These are set when installing the loader to the boot medium
*/

loader_lba:                      // logical block address of the image
	.long   0
loader_sectors:                  // number of sectors to load
	.word   0

drive:
	.word   0x80

// ===========================================================================
//                      END OF MBR BOOTSTRAP CODE
// ===========================================================================



/*
** Partition table entries
** These are here to ensure that the size of the bootstrap code is less than or
** equal to 446 bytes. If it isn't, the assembler will complain about setting
** the location counter backwards.
*/


	.org PARTITION1_LOCATION
partition1:

	.org PARTITION2_LOCATION
partition2:

	.org PARTITION3_LOCATION
partition3:

	.org PARTITION4_LOCATION
partition4:



/*
** End of the first sector of the boot program.  The last two bytes
** of this sector must be AA55 in order for the disk to be recognized
** by the BIOS as bootable.
*/
	.org BOOT_SIGNATURE_LOCATION

boot_sig:
	.word 0xAA55
