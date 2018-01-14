# mk/loader.mk
# Makefile for the loader

SUBDIR := loader

# Architecture-specific makefile
-include $(ISA_MAKEFILE)

# Platform-specific makefile
-include $(PLATFORM_MAKEFILE)

loader_CFLAGS   ?= $(CFLAGS)
loader_CPPFLAGS ?= $(CPPFLAGS)
loader_ASFLAGS  ?= $(ASFLAGS)


LOADER_OBJ = main.o

LOADER_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_OBJ))
LOADER_DEP := $(LOADER_OBJ:.o=.d)

LOADER_FINAL_OBJ = loader.final.o
LOADER_FINAL_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BUILD_DIR)/,$(LOADER_BIN))

LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) \
                  $(LOADER_ARCH_OBJ) \
                  $(LOADER_PLAT_OBJ) \
                  $(LOADER_OBJ) \
                  $(CORE_LIB)

.PHONY: loader.bin

loader.bin: $(LOADER_BIN)


$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(LOADER_LD_SCRIPT) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -T $(LOADER_LD_SCRIPT) $(LOADER_OBJ_LIST)

$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(LOADER_LD_SCRIPT) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -T $(LOADER_LD_SCRIPT) -s --oformat binary $<


-include $(LOADER_DEP)


undefine SUBDIR