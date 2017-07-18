# symtab branch

This branch contains support for embedding the loader's symbol table in loader.bin.
This is useful for debugging since the function names can be printed in a backtrace.
However, a symbol table can be quite large and is a bit overkill for a bootloader.
All symbol table stuff has been relocated from master to this branch in the case it
may be needed for something else.

Backtraces still exist in master except that only stack addresses are printed.  The 
compilation flag DEBUG_BACKTRACE must be set.


# SPLoader
Rewrite of the CSCI-452 Systems Programming bootloader

The original CSCI-452 bootloader simply loaded flat kernel/program
binaries at specified addresses.

Planned features:
 * a proper disk driver
 * reading from a FAT partition
 * Multiboot compliant
