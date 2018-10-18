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

The `config/sources` file is a list of source files to be compiled and linked.
When adding a new module to the loader binary, add the relative path of the
source file to this list.

Architecture-specific source files should go in `config/<arch>/sources`

Platform-specific source files should go in `config/<arch>/<platform>/sources`

### Porting to new targets

When porting the following must be done:
 * Create `config/<arch>` and `config/<arch>/<platform>` directories where
   arch is the target architecture/isa and platform is the target platform
   (ie `config/arm/raspberrypi`)
 * Add a `sources` and `config.cmake` files to each of these directories
 * Functions for `_<name>` identifiers must be implemented

## Versioning

The loader version is stored in the [VERSION](VERSION) file. This version gets
passed along when building.

Currently, the project is in development phase (major version 0). See the
[CONTRIBUTING.md](https://github.com/SPLoader/SPLoader/blob/develop/CONTRIBUTING.md)
file for details on tagging new versions.

Only the minor version number should be incremented at this point in time. The
minor version number should be incremented when a major feature is finished.

## License

This project is licensed under the MIT License - See the [LICENSE](LICENSE) file
for more details.
