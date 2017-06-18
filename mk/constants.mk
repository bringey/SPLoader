# mk/constants.mk
# Makefile for constant variable declarations


# Directories

INC_DIR := include

OUT_DIR = $(BUILD_DIR)/$(SUBDIR)

ISADIR      = $(SUBDIR)/$(ISA)
PLATFORMDIR = $(SUBDIR)/$(ISA)/$(PLATFORM)


ISA_MAKEFILE      = $(ISADIR)/arch.mk
PLATFORM_MAKEFILE = $(PLATFORMDIR)/platform.mk