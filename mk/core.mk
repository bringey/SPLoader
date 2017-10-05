# mk/core.mk
# Makefile for the core module

SUBDIR := core

-include $(ISA_MAKEFILE)

-include $(PLATFORM_MAKEFILE)

CORE_OBJ = kbd/parse.o \
           menu.o

CORE_OBJ := $(addprefix $(BUILD_DIR)/core/,$(CORE_OBJ))
CORE_DEP := $(CORE_OBJ:.o=.d)

CORE_ELF = core.elf
CORE_ELF := $(addprefix $(BUILD_DIR)/core/,$(CORE_ELF))

CORE_OBJ_LIST = $(CORE_ENTRY_OBJ) \
                $(CORE_ARCH_OBJ) \
                $(CORE_PLAT_OBJ) \
                $(CORE_OBJ)

.PHONY: core.elf

core.elf: $(CORE_ELF)

$(CORE_ELF): $(CORE_OBJ_LIST) $(LOADER_FINAL_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -r $(CORE_OBJ_LIST)

-include $(CORE_DEP)

undefine SUBDIR