# mk/constants.mk
# Makefile for constant variable declarations


# Directories

INC_DIR := include

# Source directory

SRC_DIR := src

# project-specific build directory
#OUT_DIR = $(BUILD_DIR)/
#OUT_ARCH_DIR = $(OUT_DIR)/$(ISA)
#OUT_PLAT_DIR = $(OUT_DIR)/$(ISA)-$(PLATFORM)

ISADIR      = $(ISA)
PLATFORMDIR = $(ISA)-$(PLATFORM)


ISA_MAKEFILE      = mk/$(ISA)..mk
PLATFORM_MAKEFILE = mk/$(ISA)-$(PLATFORM).mk

MKIMAGE_DIR := tools/mkimage
MKIMAGE := $(MKIMAGE_DIR)/mkimage
