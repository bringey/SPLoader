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

#include <SPLoader/i386/pc/bios/bootstrap.h>
#include <SPLoader/MBR/MBR.h>
#include <SPLoader/loader.h>

DP_LOCATION  = 0x400 // location of the drive parameter structure
DAP_LOCATION = 0x420 // location of the data address packet structure

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

	.globl die, die_code, dispMsg, dispChar

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
	movw    $BOOT_STACK, %ax    // and the stack starts 0x4000 beyond that
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
	movw    $0x1E, (%si)
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

	//debug_breakpoint(3)
	jmp     LOADER16_ADDRESS

/*
** Loads the loader image located at the LBA stored in loader_lba
**
** On success, AX is zero. On failure, AX contains an exit code
**
** Returns
**    AX=0                    - success
**    AX=E_BOOTSTRAP_DISK     - a read error occurred
**    AX=E_BOOTSTRAP_TOOSMALL - the loader image size was zero
**    AX=E_BOOTSTRAP_TOOBIG   - the loader image size > 127 sectors
*/
loadimage:
	pushw   %bx
	pushw   %cx
	pushw   %dx
	pushw   %si
	pushw   %di


	MAX_SECTORS_PER_TRANSFER = 127

	// determine the number of sectors we need to load
	// sectors = ceil( (loader_size + loader16_size) / dp_bytes_per_sector )
	xorw    %dx, %dx
	movw    loader_size, %ax
	addw    loader16_size, %ax
	orw     %ax, %ax                       // check if the loader_size is zero
	jz      loadimage_toosmall             // if so exit
	divw    dp_bytes_per_sector            // ax = sectors to load
	orw     %dx, %dx
	jz      loadimage_sectors              // test if the remainder is zero
	incw    %ax                            // remainder not zero, round up

loadimage_sectors:

	cmpw    $MAX_SECTORS_PER_TRANSFER, %ax
	jg      loadimage_toobig               // test if the image is too big

	// process:
	// load the entire thing to 0x10000
	// move loader16.bin to 0x7E00
	// shift loader.bin so that it starts at 0x10000
	// clear the mem after the move

	movw    $dap, %si                      // si = pointer to DAP struct
	movw    $16, dap_size                  // set the size to 16 (must be set)
	movw    $LOADER_SEGMENT, dap_buffer_segment  // setup transfer buffer
	movw    $0, dap_buffer_offset                // 1000:0000 (0x10000)
	movw    %ax, dap_sectors               // set the sectors to read
	movl    loader_lba, %eax               // set the starting LBA
	movl    %eax, dap_lowlba
	xorl    %eax, %eax                     // clear the high order bits
	call    readsectors                    // do the read
	orw     %ax, %ax
	jnz     loadimage_diskerror            // was read successful?

	// move loader16.bin to 0x7E00
	// CX = loader16_size
	// DS:SI = 1000:0000
	// ES:DI = 07C0:0200

	movw    loader_size, %bx               // BX = loader_size (save for later)
	movw    loader16_size, %cx             // CX = loader16_size
	movw    %cx, %dx                       // DX = CX (save for later)

	pushw   %ds                            // save segment regs
	pushw   %es
	movw    $BOOT_SEGMENT, %ax
	movw    %ax, %es                       // ES = 0x07C0
	movw    $LOADER_SEGMENT, %ax
	movw    %ax, %ds                       // DS = 0x1000

	xorw    %si, %si                       // SI = 0
	movw    $LOADER16_ADDRESS, %di         // DI = 0x200
	cld
	rep movsb                              // copy

	// shift loader.bin to 0x10000
	// CX = loader_size
	// DS:SI = 1000:(loader16_size)
	// ES:DI = 1000:0000

	movw    %ax, %es                       // AX still has LOADER_SEGMENT
	movw    %bx, %cx                       // CX = loader_size
	movw    %dx, %si                       // SI = loader16_size
	xorw    %di, %di                       // DI = 0
	rep movsb

	// clear parts of loader.bin leftover from the shift
	// CX = loader16_size
	// AX = 0
	// ES:DI = 1000:(loader_size)
	movw    %dx, %cx                       // DX still has loader16_size
	xorw    %ax, %ax                       // AX = 0
	movw    %bx, %di                       // BX still has loader_size
	rep stosb                              // clear mem


	popw    %es                            // restore segment regs
	popw    %ds

	jmp     loadimage_success              // done!

loadimage_diskerror:
	movw    $E_BOOTSTRAP_DISK, %ax         // read error occurred
	jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_DISK

loadimage_toosmall:
	movw    $E_BOOTSTRAP_TOOSMALL, %ax     // image size is zero
	jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_TOOSMALL

loadimage_toobig:
	movw    $E_BOOTSTRAP_TOOBIG, %ax       // image is too big ( > 127 sectors)
	jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_TOOBIG

loadimage_success:
	xorw    %ax, %ax                       // success, AX=0

loadimage_exit:
	popw    %di
	popw    %si
	popw    %dx
	popw    %cx
	popw    %bx
	ret


/*
** Routine for calculating the number of sectors to read from an image size.
** Drive Parameters must be read before calling this function.
**
** Arguments:
**   AX: image size
**
** Returns:
**   AX: sector count
*/
//sectorcount:
//	ret



/*
** Reads a specified number of sectors from a starting LBA of a drive
** If an error occurred, AX will be nonzero and CF will be set.
**
** Arguments:
**   SI: pointer to the DAP structure
**
** Returns:
**   AX = 0 on success, nonzero otherwise
*/
readsectors:
	pushw   %dx

	movw    drive, %dx
	movw    $0x4200, %ax
	int     $0x13

	popw    %dx
	ret

/*
** Support routine - display a message byte by byte to the monitor.
**
** Arguments
**    DS:SI - The pointer to the null-terminated string to display
*/
dispMsg:
	pushw   %ax
dispMsg_loop:
	lodsb                        // grab next character
	orb     %al, %al             // AL is character to write
	jz      dispMsg_exit         // if we've reached the NUL, get out

	call    dispChar             // otherwise, print and repeat
	jmp     dispMsg_loop

dispMsg_exit:                    // we're done, so return
	popw    %ax
	ret

/*
** Support routine - display a character to the monitor.
**
** Arguments:
**    AL - The character to write
*/
dispChar:
	pushw   %ax
	pushw   %bx
	movb    $0x0e, %ah           // write and advance cursor
	movw    $0x07, %bx           // page 0, white on black, no blink
	int     $0x10
	popw    %ax
	popw    %bx
	ret

/*
** Stops execution. Displays the error code stored in label die_code
*/
die:
	pushf                         // save register state
	pushw   %ds                   // only needed for debugging
	pushw   %es
	pushw   %fs
	pushw   %gs
	pusha

	movw    die_code, %ax         // load the error into AX (AH=type, AL=code)
	movw    %ax, %bx
	shrw    $8, %bx               // get the error type into bx
	shlw    $1, %bx               // multiply by 2
	movw    errortab(%bx), %si    // get the error string using bx as an index
	call    dispMsg               // print the error type
	movw    $str_errorSuffix, %si
	call    dispMsg               // print ". Code: "
	call    dispChar              // print error code

	popa                          // restore registers, so we can inspect them
	popw    %gs                   // with qemu/gdb once halted
	popw    %fs
	popw    %es
	popw    %ds
	popf

die_loop:
	hlt
	jmp     die_loop

die_code:
	.word E_BOOTSTRAP_DEFAULT


/*
** DATA AREAS.
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

dap = DAP_LOCATION

/*	.align 4

dap:
	.byte 16
	.byte 0
	.word 0
	.long 0
	.long 0
	.long 0*/

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

drive_parameters = DP_LOCATION

	//.align 4

drive:                           // the drive number we booted from
	.byte   0

/*drive_parameters:
	.word 0x1E
	.word 0
	.long 0
	.long 0
	.long 0
	.quad 0
	.word 0
	.long 0*/

/*
** Error table
** Array of strings describing the error type
*/
errortab:
	.word str_bootError       // errortab[0] = boot error
	.word str_debugError      // errortab[1] = debug error

str_bootError:
	.asciz "Boot error"

str_debugError:
	.asciz "Debug breakpoint"

str_errorSuffix:
	.asciz ". Code: "


/*
** Location and size of the loader image (loader.bin)
** These are set when installing the loader to the boot medium
*/
	.org    LOADER_LBA_LOCATION
loader_lba:                      // logical block address of the image
	.long   0                    // lower 32 bits
//	.long   0                    // upper 32 bits

	.org    LOADER_SIZE_LOCATION
loader_size:                     // size (in bytes) of the loader image
	.word   0

	.org    LOADER16_SIZE_LOCATION
loader16_size:
	.word   0

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

/*
** The extra data at the end here is for testing the loadimage function
** We can pretend that this data is the loader image, loader.bin
*/

//.fill 130 * 512, 1, 0xF0

//.fill 0x200, 1, 0xFF
//.fill 0x700, 1, 0xF0
//.fill 0x400, 1, 0xF

