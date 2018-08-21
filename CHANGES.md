
# Changelog

## v0.0.5 2018-08-23

Moved core and loader projects into "src" directory
Added library target for core project (libcore)
Per-project CFLAGS, CPPFLAGS, etc for build system
Fixed Makefile failing to build usb.img on newly cloned repo
General purpose printf functions

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