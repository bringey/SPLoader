# mk/core.mk
# Makefile for the core module

SUBDIR := core

-include $(ISA_MAKEFILE)

-include $(PLATFORM_MAKEFILE)

CORE_OBJ = console/string.o \
           kbd/parse.o \
           console.o \
           mem.o \
           menu.o \
           version.o

CORE_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_OBJ))
CORE_DEP := $(CORE_OBJ:.o=.d)

# core.final.obj contains debugging symbols, core.elf does not
CORE_FINAL_OBJ := core.final.o
CORE_FINAL_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_FINAL_OBJ))

CORE_ELF = core.elf
CORE_ELF := $(addprefix $(BUILD_DIR)/core/,$(CORE_ELF))

CORE_OBJ_LIST = $(CORE_ENTRY_OBJ) \
                $(CORE_ARCH_OBJ) \
                $(CORE_PLAT_OBJ) \
                $(CORE_OBJ)

.PHONY: core.final.o core.elf

core.final.o: $(CORE_FINAL_OBJ)

core.elf: $(CORE_ELF)

$(CORE_FINAL_OBJ): $(CORE_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -r $(CORE_OBJ_LIST)

$(CORE_ELF): $(CORE_FINAL_OBJ) $(MARKER)
	$(OBJCOPY_V) -R .comment -R .eh_frame -g $(CORE_FINAL_OBJ) $@

-include $(CORE_DEP)

undefine SUBDIR