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

#include <SPLoader/arch/i386-pc/bios/bootstrap.h>
#include <SPLoader/MBR/MBR.h>

BOOT_SEGMENT    = 0x07C0     /* default BIOS addr to load boot sector */
BOOT_ADDRESS    = 0x00007C00
START_SEGMENT   = 0x0000     /* where we'll put the startup code */
START_OFFSET    = 0x00007E00
SECTOR_SIZE     = 0x200      /* typical sector size for floppy & HD */
BOOTSTRAP_SIZE  = 446        /* maximum size of the bootstrap code */ 
OFFSET_LIMIT	= 65536 - SECTOR_SIZE

	.arch i686
	.code16
	.text


// ===========================================================================
//                      START OF MBR BOOTSTRAP CODE
// ===========================================================================

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

	movb    %dl, drive          // save the drive number that we booted from

/*
** Next, verify that the disk is there and working.
*/
	movb    $0x01, %ah           // test the disk status and make sure
	int     $0x13                // it's safe to proceed
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
/* 
** Ideally, we would use CHS addressing at this point, but CHS is gross.
** Pretty much every PC since the 90s support these extensions, so support
** for machines before then will be a TODO
**
** For now, it is an error if these extensions are not supported
*/

lbasupport:

/*
** Now we need to get the parameters of the boot device
*/

	movw    $drive_parameters, %si
	movb    drive, %dl
	movw    $0x4800, %ax
	int     $0x13
	jnc     diskready

	jmp     die                    // failed to get drive parameters, die

diskready:

/*
** Disk is OK, now time the load the loader binary, typically located right
** after the MBR.
*/

	call    loadimage
	jmp die
	// image is loaded, time to setup


/*
** Loads the loader image located at the LBA stored in loader_lba
*/
loadimage:

	movw    $1, %ax
	movw    $0, %cx
	movb    drive, %dl
	xorw    %bx, %bx
	movw    %bx, %es
	movw    $0x3000, %bx
	call    readsectors

	ret



/*
** Reads a specified number of sectors from an LBA of a drive
**
** ax    = number of sectors to read
** cx    = starting LBA
** dl    = drive to read from
** es:bx = address of the read buffer
*/
readsectors:

	pushw   %si

	// clear the DAP
	movl    $0, dap
	movl    $0, dap+4
	movl    $0, dap+8
	movl    $0, dap+12

	// setup the DAP
	movw    $16, dap
	movw    %ax, dap_sectors
	movw    %es, dap_buffer_segment
	movw    %bx, dap_buffer_offset
	movw    %cx, dap_lowlba

	// perform INT 0x13
	movw    $dap, %si
	xorw    %ax, %ax
	movb    $0x42, %ah
	int     $0x13

	popw    %si

	ret


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

	.align 4

dap:
	.byte 16
	.byte 0
	.word 0
	.long 0
	.long 0
	.long 0

/*
** Boot drive parameters
**
** Offset     Size    Description
** ============================================================================
**      0        2    size of buffer (Must be set to 0x1E)
**      2        2    information flags
**      4        4    physical cylinder count
**      8        4    physical head count
**     12        4    physical sectors per track
**     16        8    absolute number of sectors
**     24        2    bytes per sector
**     26        4    (optional) pointer to Enhanced Disk Drive parameters
*/

dp_information       = drive_parameters+2
dp_cylinders         = drive_parameters+4
dp_heads             = drive_parameters+8
dp_sectors_per_track = drive_parameters+12
dp_sectors           = drive_parameters+16
dp_bytes_per_sector  = drive_parameters+24
dp_edd               = drive_parameters+26

	.align 4

drive_parameters:
	.word 0x1E
	.word 0
	.long 0
	.long 0
	.long 0
	.quad 0
	.word 0
	.long 0


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
	.long   0                    // lower 32 bits
	.long   0                    // upper 32 bits

loader_sectors:                  // number of sectors to load
	.word   0

drive:                           // the drive number we booted from
	.byte   0

// ===========================================================================
//                      END OF MBR BOOTSTRAP CODE
// ===========================================================================

/*
** Partition table entries
** These are here to ensure that the size of the bootstrap code is less than or
** equal to 446 bytes. If it isn't, the assembler will complain about setting
** the location counter backwards.
*/


	.org MBR_PARTITION1
partition1:

	.org MBR_PARTITION2
partition2:

	.org MBR_PARTITION3
partition3:

	.org MBR_PARTITION4
partition4:



/*
** End of the first sector of the boot program.  The last two bytes
** of this sector must be AA55 in order for the disk to be recognized
** by the BIOS as bootable.
*/
	.org MBR_BOOTSIG_LOCATION

boot_sig:
	.word MBR_BOOTSIG
