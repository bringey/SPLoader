# Makefile
# Main makefile for the SPLoader project
#
# This makefile is the only makefile to be used when building. It contains all
# the targets from each project as well as maintainence rules for cleaning,
# documentation, and packing.
#
# The default target creates a bootable usb image for testing purposes via
# qemu or real hardware. This will likely change in the future
#
# Configuration
# Configuration is handled by two makefiles: includes.default.mk and
# includes.mk (optional). The former contains settings for the default
# configuration (i386-pc) while the latter is user-configurable. includes.mk
# is not tracked by git and contains developer-specific configuration.
#
# Cleaning
# There are two rules for cleaning a build:
#   1. clean: Removes compiled files from ALL projects
#   2. <project>-clean: Removes compiled files for a specified project
#
# Cleaning is used for forcing a rebuild. Rebuilding is necessary if any
# compilation flags are added/removed. This makefile will not rebuild
# automatically if any flags are changed.
#
# Documentation
# TBD


# Utility makefiles
# these are in separate makefiles for the sake of brevity
# -----------------------------------------------------------------------------

include mk/marker.mk

include mk/verbose.mk

include mk/constants.mk

# -----------------------------------------------------------------------------

# include the default configuration (i686-elf)
include includes.default.mk

# include includes.mk if it exists
-include includes.mk

BUILD_SYSTEM := Makefile $(wildcard *.mk) $(wildcard mk/*.mk)


VERSION := $(shell head -n 1 VERSION)

# Pattern rules

#
# .c to .o rule
#
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_SYSTEM) $(MARKER)
	$(CC_V) -MD $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

#
# .S to .o rule
# This pattern rule first preprocesses the .S file using cpp, then
# assembles the preprocessed .s file to the target .o file
#
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S $(BUILD_SYSTEM) $(MARKER)
	@$(CPP) -MD -MT $@ $(CPPFLAGS) -o $(BUILD_DIR)/$*.s $<
	$(AS_V) $(ASFLAGS) -o $@ $(BUILD_DIR)/$*.s -a=$(BUILD_DIR)/$*.lst

# Architecture-specific makefile
-include $(ISA_MAKEFILE)

# Platform-specific makefile
-include $(PLATFORM_MAKEFILE)

LOADER_OBJ = abort.o \
             console.o \
             main.o \
             mem.o \
             printf.o \
             string.o \
             version.o

LOADER_OBJ := $(addprefix $(BUILD_DIR)/,$(LOADER_OBJ))
LOADER_DEP := $(LOADER_OBJ:.o=.d)

LOADER_FINAL_OBJ = loader.final.o
LOADER_FINAL_OBJ := $(addprefix $(BUILD_DIR)/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BUILD_DIR)/,$(LOADER_BIN))

LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) \
                  $(LOADER_ARCH_OBJ) \
                  $(LOADER_PLAT_OBJ) \
                  $(LOADER_OBJ)

.PHONY: loader.bin

loader.bin: $(LOADER_BIN)

$(BUILD_DIR)/version.o: DEFINES += -DVERSION=\"$(VERSION)\"

$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(LOADER_LD_SCRIPT) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -T $(LOADER_LD_SCRIPT) $(LOADER_OBJ_LIST)

$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(LOADER_LD_SCRIPT) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -T $(LOADER_LD_SCRIPT) -s --oformat binary $<


-include $(LOADER_DEP)


# Pattern Rules

#
# object file to namelist (useful when debugging)
#
$(BUILD_DIR)/%.nl: $(BUILD_DIR)/%.o $(MARKER)
	$(NM_V) -n $< > $@




# TARGETS

$(BUILD_DIR)/usb.img: $(BIOS_BOOTSTRAP_BIN) $(LOADER_BIN) $(MKIMAGE)
	$(MKIMAGE) -o $@ -b $(BIOS_BOOTSTRAP_BIN) $(LOADER_BIN)

$(MKIMAGE):
	$(MAKE) -C $(MKIMAGE_DIR)

.PHONY: all clean


#
# Clean
#

allclean: clean clean-tools

#
# cleans build directory
# all files and subdirectories (excluding .gitignore) will be deleted
# so take caution when overriding BUILD_DIR
#
clean:
	find $(BUILD_DIR) -maxdepth 1 -mindepth 1 ! -name '.gitignore' -exec rm -r {} +

clean-tools:
	$(MAKE) -C $(MKIMAGE_DIR) clean
