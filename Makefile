# Makefile
# Main makefile for the SPLoader project
#
# This makefile is the only makefile to be used when building. It contains all
# the targets from each project as well as maintainence rules for cleaning,
# documentation, and packing. Rules shared between projects are to be defined
# here or in mk/rules.mk (if you need project-specific variables).
#
# This build system uses projects as an organizational unit. Projects are
# individual parts that each have their own targets and dependencies. Each
# project has its own makefile, stored in mk/<project>.mk. Every project
# makefile gets included by this makefile. When adding a new project, add the
# project makefile to the mk folder and add the project's name to the PROJECTS
# variable.
#
# The build system avoids recursive make by including all makefiles into this
# one. When writing project makefiles, take care in naming variables. Since
# every makefile gets included here there is only a global scope for variable
# names. It is suggested that project variable names are prefixed by the
# project name (ie LOADER_CFLAGS, CORE_CPPFLAGS, etc).
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
# There are three rules for cleaning a build:
#   1. clean: Removes compiled files from ALL projects
#   2. <project>-clean: Removes compiled files for a specified project
#   3. purge: Delete the build directory
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

include mk/rules.mk

# -----------------------------------------------------------------------------

# include the default configuration (i686-elf)
include includes.default.mk

# include includes.mk if it exists
-include includes.mk

VERSION := $(shell head -n 1 VERSION)

DEFINES += -DVERSION="\"$(VERSION)\""

PROJECTS := libcore loader core

# generate project pattern rules
$(foreach proj,$(PROJECTS),$(eval $(call GENERATE_RULES,$(proj))))


include $(addsuffix .mk,$(addprefix mk/,$(PROJECTS)))

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

.PHONY: all clean $(CLEAN_TARGETS)


#
# Clean
#

CLEAN_ARGS = -type f \
             -name "*.o" -delete -o \
             -name "*.d" -delete -o \
             -name "*.s" -delete -o \
             -name "*.lst" -delete -o \
             -name "*.bin" -delete
CLEAN_TARGETS := $(addsuffix -clean,$(PROJECTS))

$(CLEAN_TARGETS): %-clean:
	find $(BUILD_DIR)/$* $(CLEAN_ARGS)

clean:
	find $(BUILD_DIR) $(CLEAN_ARGS)

purge:
	$(RM) -r $(BUILD_DIR)
