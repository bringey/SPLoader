# Contributing guidelines

This document should contain everything you need to know in order to contribute
to this project. Please read each section to get started.

Table of Contents
<!-- TOC -->

- [Contributing guidelines](#contributing-guidelines)
    - [Building](#building)
    - [Code Conventions](#code-conventions)
        - [General](#general)
            - [Function Names](#function-names)
            - [Typedefs](#typedefs)
            - [Variables/Symbols](#variablessymbols)
            - [Whitespace](#whitespace)
        - [Organization](#organization)
            - [Arch and Platform directories](#arch-and-platform-directories)
            - [Header Guards](#header-guards)
        - [Portability](#portability)
    - [Contributing](#contributing)

<!-- /TOC -->

## Building

TBD

## Code Conventions

### General

#### Function Names

Function names have the form `mod_fooBar` where `mod` is the module name and
fooBar is the routine name. A module is just a grouping of similar functions,
variables, etc. camelCase is preferred for the routine name, but snake_case is
also acceptible. Underscore prefixes must be used for marking portability, see
[Portability](#Portability) for details on this.

#### Typedefs

Typedef names should be in PascalCase. If the typedef name contains an acronym,
only the first letter is capitalized (ie `GptHeader` instead of `GPTHeader`).
Typedefs must NEVER define pointer types except for opaque datatypes (Never use
the following: `struct MyType { ... } *MyType;`).

#### Variables/Symbols

There is no specific convention for variable/symbol names. The only convention
is that macro constants and constant variables/symbols must be capitalized with
underscores (ie `MY_CONSTANT_VARIABLE`).

#### Whitespace

All files (except Makefiles) must use 4-space tabs for indentation. Linux style
newlines ('\n') are to be used in all files. NO TABS!

### Organization

Source code is organized into separate git repos. Each repo is added to the
main repository as a submodule. For example, the SPLoader/loader repo contains
the stage 1 binary for the bootloader.

Header files for each project reside in the `include/` folder. Common header
files or library headers used by multiple projects go in the root repo's
`include/` folder. C and assembly source files reside in a `src/` folder.

All public header files must reside in the `include/` folder. Header files for
a project are located in a directory with the project's name in the `include/`
folder. A public header is a header that can be include'd by any source file.
A private header is a header that can only be include'd by certain source
files. Private headers should reside with the source files.

The following file extensions are used:
  * .c for C source files
  * .h for C header files
  * .lds for linker scripts
  * .S for C Preprocessed assembly source
  * .s for assembly source

#### Arch and Platform directories

Architecture-specific and platform-specific code should be located in special
subdirectories known as arch directories and platform directories,
respectively. An arch directory contains code that is specific for a certain
architecture or ISA. The `loader/src/i386` and `loader/include/i386` are arch
directories. A platform directory contains code that is specific for a certain
architecture and platform combination. The `loader/src/i386-pc` and
`loader/include/i386-pc` are platform directories. A source or header file is
considered platform dependendent if it resides in an arch directory or a
platform directory.

#### Header Guards

All header files must have a guard. The guard macro must start with `_`
followed by the pathname of the header file with `/` replaced with `_` and is
suffixed with `_H`. Assembly guards should also be used. These allow assembly
files to include header files without including any C syntax. Use the `__ASM__`
macro as a guard. C syntax is to be included if `__ASM__` is NOT defined. The
following is an example header

include/foo/example.h
```C
#ifndef _FOO_EXAMPLE_H
#define _FOO_EXAMPLE_H

// macro definitions goes here

#ifndef __ASM__

// C stuff goes here

#endif

#endif
```

When including a header from an assembly file, define the `__ASM__` macro
first. For example:
```C
#define __ASM__
#include <foo/example.h>
// ASM Code
```

### Portability

Functions should be written with portability in mind. A system is in place to
help with this. Each function is either platform-dependent or 
platform-independent. Using "foo" as a function name, the following naming
convention is used:

 1. `foo`: platform-independent implementation and prototype
 2. `_foo`: platform-dependent implementation, platform-independent prototype
 3. `__foo`: platform-dependent implementation, platform-dependent prototype

For Type 1, this function is platform-independent and should be able to be
compiled for any platform. No restrictions are placed on the location of the
function's prototype and implementation.

For Type 2, this function is platform-dependent and is to be compiled for a
specific architecture or platform. The function's implementation MUST reside
in a platform directory or an arch directory. The function's prototype however,
does not have this restriction. For example, the `_con_put` prototype is
defined in `loader/include/console.h` and its implementation resides
in `loader/src/i386-pc/console.c`. Note that in this example,
`loader/src/i386-pc` is a platform directory.

For Type 3, this function is also platform-dependent like Type 2. The only
difference here is that the protoype is also platform-dependent. BOTH the
prototype and the implementation MUST reside in a platform directory or an arch
directory.

Functions of type 1 CAN ONLY call functions of types 1 and 2. A type 1 function
CANNOT call a type 3 function. Functions of types 2 and 3 can call any
function.

Assembly functions are always of type 2 or 3. An assembly function can NEVER
be marked as a type 1 function (for obvious reasons).

When porting to a new architecture/platform, ONLY type 2 functions will need to
be implemented. And of course, type 3 functions can be added if needed.

The following table displays the function types and their properties:

| type | name  | prototype   | implementation | can call         |
|------|-------|-------------|----------------|------------------|
|    1 |   foo | independent |    independent | foo, _foo        |
|    2 |  _foo | independent |      dependent | foo, _foo, __foo |
|    3 | __foo |   dependent |      dependent | foo, _foo, __foo |

## Contributing

TBD
