
# Changelog

## v0.1.0 2018-08-23

**Restructure Update**

The build system and project structure has been overhauled for this update.
SPLoader will no longer feature a stage 2 binary (core). The core project was
removed, leaving the loader project as the only project in the build system.
Since there is only one project to build, project support has been removed
from the build system.

From this point on, SPLoader will only be a stage 1 bootloader. Stage 2
functionality can still be achieved by creating a stage 2 binary that SPLoader
will execute.

Changes to build system:
 * Removal of projects, makefile only builds loader now
 * Core and loader project makefiles removed, contents of loader makefile moved
   into main Makefile.
 * Arch-specific and platform-specific makefiles are located in `mk/<arch>.mk`
   and `mk/<arch>-<platform>.mk`, respectively
 * Unused variables were removed or commented out

Changes to project structure:
 * Source files now reside in `src` folder
    * All files in `src/core` were removed or moved to `src`
    * All files in `src/loader` were moved to `src`
 * Removal of `include/SPLoader`
    * Most files in this folder were moved to `include`
    * Public header files for an API are no longer needed, so this folder was
      removed.
    * All source file includes have been refactored to use this renaming


## v0.0.5 2018-08-23

* Moved core and loader projects into "src" directory
* Added library target for core project (libcore)
* Per-project CFLAGS, CPPFLAGS, etc for build system
* Fixed Makefile failing to build usb.img on newly cloned repo
* General purpose printf functions

## v0.0.4 2017-10-13

Moved more stuff from loader to core, reorganized header files.

## v0.0.3 2017-10-05

Added core project to source tree. Moved deprecated modules from loader to core
project.

Added
 * ModuleHeader typedef
 * Module typedef
 * core/
 * mk/core.mk
 * mk/i386-pc/core.platform.mk

## v0.0.2 2017-09-25

Added
 * __int32 support function for i386-pc platform

## v0.0.1 2017-09-23

Initial Version