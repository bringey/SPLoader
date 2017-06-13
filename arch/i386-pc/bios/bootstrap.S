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


E_BOOTSTRAP_DEFAULT = '0'
E_BOOTSTRAP_DISK    = '1'
E_BOOTSTRAP_NOLBA   = '2'

E_BOOTSTRAP_DEBUG1 = 'A'
E_BOOTSTRAP_DEBUG2 = 'B'
E_BOOTSTRAP_DEBUG3 = 'C'


//
// Macro for stopping execution with a code as an immediate/register/address
//
#define exit(code) \
	movw code, die_code; \
	jmp die

//
// Macro for stopping execution with a debug code (1 - 3)
//
#define debug_breakpoint(num) exit($E_BOOTSTRAP_DEBUG##num)

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

	exit($E_BOOTSTRAP_DISK)      // something went wrong; display code and stop

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

	// BIOS does not support 0x13 extensions, die
	exit($E_BOOTSTRAP_NOLBA)
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

	exit($E_BOOTSTRAP_DISK)      // failed to get drive parameters, die

diskready:

/*
** Disk is OK, now time the load the loader binary, typically located right
** after the MBR.
*/

	call    loadimage
	orw     %ax, %ax
	jz      nextstage

	exit(%ax)                    // error occured during load, die

nextstage:

/* 
** image is loaded, now we transfer control to the loader, which will
** handle loading the kernel from the boot partition
*/

	debug_breakpoint(3)

/*
** Loads the loader image located at the LBA stored in loader_lba
**
** On success, AX is zero. On failure, AX contains an exit code
*/
loadimage:

	// determine the number of sectors we need to load
	xorw    %dx, %dx
	movw    loader_size, %ax
	divw    dp_bytes_per_sector         // ax = loader_size / bytes_per_sector
	orw     %dx, %dx
	jz      loadimage_sectors           // test if the remainder is zero
	incw    %ax                         // remainder not zero, round up

loadimage_sectors:


	// test code for readsectors
	// this just reads the first sector (MBR) and stores it at 0x3000
	pushw   $0x3000                     // buffer location: 0:3000
	pushw   $0
	pushl   $0x0                        // LBA upper 32 bits
	pushl   $0x0                        // LBA lower 32 bits
	pushw   drive                       // drive to read from
	pushw   $1                          // read 1 sector
	call    readsectors
	add     $16, %sp

	xorw    %ax, %ax                    // success, AX=0
	ret



/*
** __cdecl void readsectors(
**          word sectors,
**          word drive,
**          quadword lba,
**          word buffer_segment,
**          word buffer_offset
** );
**
** Reads a specified number of sectors from a starting LBA of a drive
** If an error occurred, the Carry Flag will be set.
**
** Arguments
**    word sectors
**        The number of sectors to load, note that some BIOS limit this to 127
**    word drive
**        The drive to read from (0x80 for the first drive, 0x0 for floppy)
**    quad lba
**        The 48-bit starting LBA to read from, stored as a quadword.
**    word buffer_segment
**    word buffer_offset
**        Segment:Offset pair for the location in memory to store the sectors
**
** Returns
**    AX = 0 on success, nonzero otherwise
**
*/
readsectors:

	pushw   %bp
	movw    %sp, %bp

	pushw   %cx
	pushw   %dx
	pushw   %si
	pushw   %di

	// setup the DAP
	movw    $16, dap

	movw    4(%bp), %ax
	movw    %ax, dap_sectors

	movw    16(%bp), %ax
	movw    %ax, dap_buffer_segment

	movw    18(%bp), %ax
	movw    %ax, dap_buffer_offset

	movl    8(%bp), %eax
	movl    %eax, dap_lowlba
	movl    12(%bp), %eax
	movl    %eax, dap_highlba

	// perform INT 0x13
	movw    $dap, %si
	xorw    %ax, %ax
	movw    6(%bp), %dx
	movb    $0x42, %ah
	int     $0x13

	popw    %di
	popw    %si
	popw    %dx
	popw    %cx

	popw    %bp
	ret


/*
** Stops execution. Displays the error code stored in label die_code
*/
die:
	pusha                      // save register state
	pushw   %ds                // only needed for debugging
	pushw   %es
	pushw   %fs
	pushw   %gs
	pushf


	movb    $0x0e, %ah         // write and advance cursor
	movb    die_code, %al      // char to print = die_code
	movw    $0x07, %bx         // page 0, white on black, no blink
	int     $0x10
	
	popf                       // restore registers, so we can inspect them
	popw    %gs                // with qemu/gdb once halted
	popw    %fs
	popw    %es
	popw    %ds
	popa

die_loop:
	hlt
	jmp     die_loop

die_code:
	.word E_BOOTSTRAP_DEFAULT


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
** Location and size of the loader image (loader.bin)
** These are set when installing the loader to the boot medium
*/

loader_lba:                      // logical block address of the image
	.long   0                    // lower 32 bits
	.long   0                    // upper 32 bits

loader_size:                     // size (in bytes) of the loader image
	.word   30000

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
	.fill 16, 1, 0

	.org MBR_PARTITION2
partition2:
	.fill 16, 1, 0

	.org MBR_PARTITION3
partition3:
	.fill 16, 1, 0

	.org MBR_PARTITION4
partition4:
	.fill 16, 1, 0



/*
** End of the first sector of the boot program.  The last two bytes
** of this sector must be AA55 in order for the disk to be recognized
** by the BIOS as bootable.
*/
	.org MBR_BOOTSIG_LOCATION

boot_sig:
	.word MBR_BOOTSIG
