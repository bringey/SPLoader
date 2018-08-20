/*
** File: src/loader/i386-pc/stage0a/bootstrap.S
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

#define __ASM__
#include <loader/i386-pc/stage0a/bootstrap.h>
#include <i386-pc/mbr.h>
#include <loader/i386-pc/loader.h>
#include <loader/i386-pc/BootHeader.h>

//
// Macro for stopping execution with a code as an immediate/register/address
//
.macro EXIT code
    movw    \code, die_code
    jmp     die
.endm

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
    xorw    %ax, %ax
    movw    %ax, %ss
    movw    $LOADER_STACK_ADDRESS, %ax // stack: 0000:4000 and downward
    movw    %ax, %sp

    movb    %dl, drive          // save the drive number that we booted from

/*
** Next, verify that the disk is there and working.
*/
    movb    $0x01, %ah           // test the disk status and make sure
    int     $0x13                // it's safe to proceed
    jnc     diskok

    EXIT    $E_BOOTSTRAP_DISK       // something went wrong; display code and stop

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
    EXIT    $E_BOOTSTRAP_NOLBA
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

    EXIT    $E_BOOTSTRAP_DISK         // failed to get drive parameters, die

diskready:

/*
** Disk is OK, now time the load the loader binary, typically located right
** after the MBR.
*/

    call    loadimage
    orw     %ax, %ax
    jz      nextstage

    EXIT    %ax                       // error occured during load, die

nextstage:

/* 
** image is loaded, now we transfer control to the loader, which will
** handle loading the kernel from the boot partition
*/
    jmp     $LOADER_SEGMENT, $0x0

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
    pushw   %si
    pushw   %cx
    pushw   %dx

    // read two sectors from boot_lba to 07C0:0200
    // the first sector is the boot header
    // the second contains loader16.bin
    movw    $dap, %si
    movw    $16, dap_size                  // set the size to 16 (must be set)
    movw    $BOOT_SEGMENT, dap_buffer_segment  // setup transfer buffer
    movw    $0x200, dap_buffer_offset          // 07C0:0200 (0x7E00)
    movw    $1, dap_sectors                    // set the sectors to read
    movl    boot_lba, %ecx                     // ECX = boot partition LBA
    movl    %ecx, dap_lowlba
    call    readsectors                    // do the read
    testw   %ax, %ax                       // check for disk error
    jnz     loadimage_diskerror

    // test the boot header located at 07C0:0200, make sure the signature is
    // correct. If it isn't then the image is either corrupted, or isn't a
    // sploader image.
    movl    bh_sig, %eax
    xorl    $BOOT_HEADER_SIGNATURE_LO, %eax // test the lower four bytes
    jnz     loadimage_badheader             // if ZF is set = header matched
    movl    bh_sig + 4, %eax
    xorl    $BOOT_HEADER_SIGNATURE_HI, %eax // test the upper four bytes
    jnz     loadimage_badheader

    // read the loaderSize field in the header and determine how many sectors
    // to load from the disk, %AX = ceil(bh_loaderSize / dp_bytes_per_sector)
    movw    bh_loaderSize, %ax
    orw     %ax, %ax                       // check if the loader_size is zero
    jz      loadimage_toosmall             // if so exit
    xorw    %dx, %dx
    divw    dp_bytes_per_sector            // ax = sectors to load
    orw     %dx, %dx
    jz      loadimage_sectors              // test if the remainder is zero
    incw    %ax                            // remainder not zero, round up

loadimage_sectors:
    cmpw    $MAX_SECTORS_PER_TRANSFER, %ax
    jg      loadimage_toobig                    // test if the image is too big

    movw    $LOADER_SEGMENT, dap_buffer_segment // setup transfer buffer
    movw    $0, dap_buffer_offset               // 1000:0000 (0x10000)
    movw    %ax, dap_sectors                    // set the sectors to read
    incl    %ecx                                // skip the header we just read
    movl    %ecx, dap_lowlba                    // read earlier
    call    readsectors                         // do the read
    testw   %ax, %ax
    jnz     loadimage_diskerror

    // success! exit with %ax = 0
    xorw    %ax, %ax
    jmp     loadimage_exit

loadimage_diskerror:
    movw    $E_BOOTSTRAP_DISK, %ax         // read error occurred
    jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_DISK

loadimage_toosmall:
    movw    $E_BOOTSTRAP_TOOSMALL, %ax     // loader.bin size was zero
    jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_TOOSMALL

loadimage_toobig:
    movw    $E_BOOTSTRAP_TOOBIG, %ax       // loader.bin size > 127 sectors
    jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_TOOBIG

loadimage_badheader:
    movw    $E_BOOTSTRAP_BADHEADER, %ax    // image header is invalid
    jmp     loadimage_exit                 // error, AX=E_BOOTSTRAP_BADHEADER

loadimage_exit:
    popw    %dx                            // we're done, clean it and beat it
    popw    %cx
    popw    %si
    ret

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
    movw    $str_bootError, %si
    call    dispMsg
    movw    die_code, %ax
    call    dispChar
die_loop:
    hlt
    jmp     die_loop

die_code:
    .word E_BOOTSTRAP_DEFAULT


/*
** Some useful symbols
*/

DP_LOCATION  = 0x8000 // location of the drive parameter structure
DAP_LOCATION = 0x8020 // location of the data address packet structure
BOOT_HEADER  = 0x200  // location of the BootHeader structure

// some BIOS's limit the dap_sectors to 127
MAX_SECTORS_PER_TRANSFER = 127

/*
** bh - Boot header
**
** See include/SPLoader/i386-pc/BootHeader.h for details
*/

bh_sig = BOOT_HEADER + BOOT_HEADER_SIGNATURE
bh_flags = BOOT_HEADER + BOOT_HEADER_FLAGS
bh_loaderSize = BOOT_HEADER + BOOT_HEADER_LOADER_SIZE

/*
** dap - Data Address Packet
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


/*
** dp - Boot drive parameters
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

drive:                           // the drive number we booted from
    .byte   0

str_bootError:
    .asciz "Boot error. Code: "

endtext:

// ===========================================================================
//                      END OF MBR BOOTSTRAP CODE
// ===========================================================================

// boot footer
// contains the location of the stage 1 partition which is either the
// post-MBR gap (BIOS/MBR) or a BIOS boot partition (BIOS/GPT)

    .org    BOOT_LBA_LOCATION
boot_lba:
    .long   0

// there are two extra bytes right before the partition table for use if
// needed (0x1BC-0x1BD)
