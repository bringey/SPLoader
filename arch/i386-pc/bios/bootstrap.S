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

BOOT_SEGMENT = 0x07C0        // default BIOS addr to load boot sector
BOOT_ADDRESS = 0x7C00

LOADER_LBA_LOCATION  = 0x1B4 // location of the loader LBA
LOADER_SIZE_LOCATION = 0x1BC // location of the loader size

#define mkcode(type, char) (type << 8) | char

// error codes
//   byte 1: 0
//   byte 0: error code character

E_BOOTSTRAP_DEFAULT  = mkcode(0, '0')
E_BOOTSTRAP_DISK     = mkcode(0, '1')
E_BOOTSTRAP_NOLBA    = mkcode(0, '2')
E_BOOTSTRAP_TOOBIG   = mkcode(0, '3')
E_BOOTSTRAP_TOOSMALL = mkcode(0, '4')

// debug codes
//   byte 1: 1
//   byte 0: debug code character

E_BOOTSTRAP_DEBUG1 = mkcode(1, 'A')
E_BOOTSTRAP_DEBUG2 = mkcode(1, 'B')
E_BOOTSTRAP_DEBUG3 = mkcode(1, 'C')


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

	//debug_breakpoint(3)
	jmp     $0, $LOADER_ADDRESS

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

	pushw   %bp
	movw    %sp, %bp

	pushw   %dx


	MAX_SECTORS_PER_TRANSFER = 127

	// determine the number of sectors we need to load
	// sectors = ceil( loader_size / dp_bytes_per_sector )
	xorw    %dx, %dx
	movw    loader_size, %ax
	orw     %ax, %ax                       // check if the loader_size is zero
	jz      loadimage_toosmall             // if so exit
	divw    dp_bytes_per_sector            // ax = sectors to load
	orw     %dx, %dx
	jz      loadimage_sectors              // test if the remainder is zero
	incw    %ax                            // remainder not zero, round up

loadimage_sectors:

	cmpw    $MAX_SECTORS_PER_TRANSFER, %ax
	jg      loadimage_toobig               // test if the image is too big

	pushw   $0                             // buffer: LOADER_SEGMENT:0
	pushw   $LOADER_SEGMENT
	pushl   loader_lba+4                   // LBA of the loader image
	pushl   loader_lba
	pushw   drive                          // drive: drive we booted from
	pushw   %ax                            // sectors: AX
	call    readsectors
	add     $16, %sp
	orw     %ax, %ax
	jz      loadimage_success              // was the read successful?

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
	popw    %dx
	popw    %bp
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
** If an error occurred, AX will be nonzero and CF will be set.
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

	pushw   %dx
	pushw   %si

	// setup the DAP (Data address packet)
	movw    $16, dap                    // 16 bytes is the size of the packet

	movw    4(%bp), %ax                 // set the sectors to read
	movw    %ax, dap_sectors

	movw    16(%bp), %ax                // move the buffer segment to the dap
	movw    %ax, dap_buffer_segment

	movw    18(%bp), %ax                // move the buffer offset to the dap
	movw    %ax, dap_buffer_offset

	movl    8(%bp), %eax                // move the lower 32-bits of the lba
	movl    %eax, dap_lowlba            // to the dap
	movl    12(%bp), %eax               // move the upper 32-bits of the lba
	movl    %eax, dap_highlba           // to the dap
	xorl    %eax, %eax                  // clear %eax

	// perform INT 0x13, extended read
	movw    $dap, %si                   // ds:si = pointer to dap
	movw    6(%bp), %dx                 // set the drive to use from argument
	movb    $0x42, %ah                  // extended read, AH=0x42
	int     $0x13                       // do the read

	popw    %si
	popw    %dx

	popw    %bp
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

	movw    die_code, %ax         // ah = error type, al = error code
	orb     %ah, %ah              // test error code
	jz      die_booterror         // if ah=0 then we got a boot error
	movw    $str_debugError, %si  // otherwise it's a debug breakpoint
	jmp     die_printerror
die_booterror:
	movw    $str_bootError, %si
die_printerror:
	call    dispMsg               // print error message
	movw    $str_errorSuffix, %si
	call    dispMsg
	call    dispChar              // print the error code
	
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

drive:                           // the drive number we booted from
	.byte   0

drive_parameters:
	.word 0x1E
	.word 0
	.long 0
	.long 0
	.long 0
	.quad 0
	.word 0
	.long 0

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
	.long   0                    // upper 32 bits

	.org    LOADER_SIZE_LOCATION
loader_size:                     // size (in bytes) of the loader image
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
//.fill 0x200, 1, 0xF0
//.fill 0x400, 1, 0xF

