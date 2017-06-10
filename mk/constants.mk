# mk/constants.mk
# Makefile for constant variable declarations


# Directories

INC_DIR := include


ARCHDIR = arch
BINDIR  = $(BUILD_DIR)/bin
OBJDIR  = $(BUILD_DIR)/obj

ISADIR      = $(ARCHDIR)/$(ISA)
PLATFORMDIR = $(ARCHDIR)/$(ISA)-$(PLATFORM)
