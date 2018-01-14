# mk/core.mk
# Makefile for the core module
#
# Targets:
#   core.elf: Core executable for stage 2 (loaded by stage 1 loader.bin)
#   libcore.a: Minimal core library needed by stage 1
#

SUBDIR := core

-include $(ISA_MAKEFILE)

-include $(PLATFORM_MAKEFILE)

core_CFLAGS   := $(CFLAGS)
core_CPPFLAGS := $(CPPFLAGS)
core_ASFLAGS  := $(ASFLAGS)

CORE_OBJ = kbd/parse.o \
           mem.o \
           menu.o \
           version.o

CORE_LIB_OBJ = abort.o \
               console.o \
               err.o \
               printf.o \
               string.o

CORE_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_OBJ))
CORE_DEP := $(CORE_OBJ:.o=.d)

CORE_LIB_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_LIB_OBJ))
CORE_LIB_DEP := $(CORE_LIB_OBJ:.o=.d)


# core.final.obj contains debugging symbols, core.elf does not
CORE_FINAL_OBJ := core.final.o
CORE_FINAL_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_FINAL_OBJ))

CORE_ELF = core.elf
CORE_ELF := $(addprefix $(BUILD_DIR)/core/,$(CORE_ELF))

CORE_LIB = libcore.a
CORE_LIB := $(addprefix $(BUILD_DIR)/core/,$(CORE_LIB))

CORE_OBJ_LIST = $(CORE_ENTRY_OBJ) \
                $(CORE_ARCH_OBJ) \
                $(CORE_PLAT_OBJ) \
                $(CORE_OBJ) \
                $(CORE_LIB_ARCH_OBJ) \
                $(CORE_LIB_PLAT_OBJ) \
                $(CORE_LIB_OBJ)

CORE_LIB_OBJ_LIST = $(CORE_LIB_ARCH_OBJ) \
                    $(CORE_LIB_PLAT_OBJ) \
                    $(CORE_LIB_OBJ)

.PHONY: core.final.o core.elf libcore.a

core.final.o: $(CORE_FINAL_OBJ)

core.elf: $(CORE_ELF)

libcore.a: $(CORE_LIB)

$(CORE_FINAL_OBJ): $(CORE_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -r $(CORE_OBJ_LIST)

$(CORE_ELF): $(CORE_FINAL_OBJ) $(MARKER)
	$(OBJCOPY_V) -R .comment -R .eh_frame -g $(CORE_FINAL_OBJ) $@

$(CORE_LIB): $(CORE_LIB_OBJ_LIST) $(MARKER)
	$(AR_V) rcs $@ $(CORE_LIB_OBJ_LIST)

-include $(CORE_DEP) $(CORE_LIB_DEP)

undefine SUBDIR