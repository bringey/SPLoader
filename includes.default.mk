
# Makefile containing the default configuration variables
# This file is used by the root Makefile if includes.mk does
# not exist.

# location of the build directory
BUILD_DIR := build

# makefile verbosity
# 0: Only show command variable and output file
# 1: Show entire command
VERBOSE := 0

ISA := i386
PLATFORM := pc

# =============================================================================
# COMMANDS
# =============================================================================

CC := i686-elf-gcc

CPP := i686-elf-cpp

AS := i686-elf-as

LD := i686-elf-ld

AR := i686-elf-ar

NM := i686-elf-nm

OBJCOPY := i686-elf-objcopy

# =============================================================================
# FLAGS
# =============================================================================

# if set warnings will be treated as errors
WERROR :=

WARNFLAGS := -Wall -Wextra

CFLAGS = -std=gnu11 -ffreestanding $(WARNFLAGS) $(WERROR)

CPPFLAGS = -I$(INC_DIR) $(USER_DEFINES) $(DEFINES)

ASFLAGS =

LDFLAGS =


#
# User-specified compile flags
#
# Debug flags (DEBUG):
# DEBUG_FILENAMES: filename and line number will be printed during abort
# DEBUG_BACKTRACE: a stack backtrace will be printed during abort
# DEBUG_KBD_PARSE: scancodes are printed as they are received, as well as
#                  the result of the scancode buffer
#
# Feature enable flags (FE):
# FE_MENU:    Menu shown to user at boot, includes E_TIMER, E_CONSOLE
# FE_TIMER:   allows for a boot delay
# FE_CONSOLE: logging facilities during boot
#
USER_DEFINES = -DFE_MENU