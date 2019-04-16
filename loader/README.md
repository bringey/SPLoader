# SPLoader/loader
Stage 1 binary for SPLoader

This project repository contains the stage 1 binary used by SPLoader. This
project creates two binaries: bootstrap and stage 1 (bootstrap.bin and
loader.bin, respectively). These binaries are then installed to a disk, image
or any other medium using the `spl-install` tool.


The process for booting an OS is as follows:
```
bootstrap -> stage 1 -> OS

OR (if a stage 2 exists)

bootstrap -> stage 1 -> stage 2 -> OS
```
Note that the bootstrap is platform-dependent and may not exist for some
platforms.

## Building

See this [README.md](https://github.com/SPLoader/SPLoader/blob/develop/README.md)
for details on setting up cmake. The CMakeLists.txt adds the following targets:
 * loader.elf
 * loader.bin
 * bootstrap.elf
 * bootstrap.bin

The *.bin files are flat binaries to be installed to the target medium. Their
layout depends on the target platform.

### Adding new source files

The `config/loader/sources` file is a list of source files to be compiled and
linked. When adding a new module to the loader binary, add the relative path of
the source file to this list.

Architecture-specific source files should go in `config/loader/<arch>/sources`

Platform-specific source files should go in `config/loader/<arch>/<platform>/sources`

### Porting to new targets

When porting the following must be done:
 * Create `config/loader/<arch>` and `config/loader/<arch>/<platform>`
   directories where arch is the target architecture/isa and platform is the
   target platform (ie `config/loader/arm/raspberrypi`)
 * Add a `sources` and `config.cmake` files to each of these directories
   (use i386-pc as an example)
 * Functions for `_<name>` identifiers must be implemented (type 2 functions)
