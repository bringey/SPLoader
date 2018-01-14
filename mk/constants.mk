# mk/constants.mk
# Makefile for constant variable declarations


# Directories

INC_DIR := include

# Source directory

SRC_DIR := src

# project-specific build directory
OUT_DIR = $(BUILD_DIR)/$(SUBDIR)
OUT_ARCH_DIR = $(OUT_DIR)/$(ISA)
OUT_PLAT_DIR = $(OUT_DIR)/$(ISA)-$(PLATFORM)

ISADIR      = $(SUBDIR)/$(ISA)
PLATFORMDIR = $(SUBDIR)/$(ISA)-$(PLATFORM)


ISA_MAKEFILE      = mk/$(ISA)/$(SUBDIR).arch.mk
PLATFORM_MAKEFILE = mk/$(ISA)-$(PLATFORM)/$(SUBDIR).platform.mk

MKIMAGE_DIR := tools/mkimage
MKIMAGE := $(MKIMAGE_DIR)/mkimage
