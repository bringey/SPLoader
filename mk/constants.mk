# mk/constants.mk
# Makefile for constant variable declarations


# Directories

INC_DIR := include

OUT_DIR = $(BUILD_DIR)/$(SUBDIR)

ISADIR      = $(SUBDIR)/$(ISA)
PLATFORMDIR = $(SUBDIR)/$(ISA)-$(PLATFORM)


ISA_MAKEFILE      = mk/$(ISA)/$(SUBDIR).arch.mk
PLATFORM_MAKEFILE = mk/$(ISA)-$(PLATFORM)/$(SUBDIR).platform.mk

MKIMAGE_DIR := tools/mkimage
MKIMAGE := $(MKIMAGE_DIR)/mkimage

MKSYMTAB := python tools/mksymtab.py
