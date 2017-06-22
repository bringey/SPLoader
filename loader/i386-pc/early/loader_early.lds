/*
** Linker script for the loader_early binary
** This binary is located right after the bootstrap and is 1 sector (512 bytes)
** in size.
*/

/* gets loaded at 07C0:0200 (or 0x7E00) */
/* so ld will error if the binary is larger than 1 sector */

ENTRY(early)

SECTIONS
{
    . = 0x200;
    .text : { *(.text) }
    . = 0x400;
}