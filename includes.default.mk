
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

# =============================================================================
# FLAGS
# =============================================================================

# if set warnings will be treated as errors
WERROR :=

WARNFLAGS := -Wall -Wextra

CFLAGS = -std=gnu11 -ffreestanding $(WARNFLAGS) $(WERROR)

CPPFLAGS = -I$(INC_DIR)

ASFLAGS =

LDFLAGS =
