# mk/loader.mk
# Makefile for the loader

SUBDIR := loader

# Architecture-specific makefile
-include mk/$(ISA)/loader.mk

# Platform-specific makefile
-include mk/$(ISA)/$(PLATFORM)/loader.mk


LOADER_OBJ =
LOADER_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_OBJ))

LOADER_FINAL_OBJ = loader.o
LOADER_FINAL_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_BIN))

LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) $(LOADER_ARCH_OBJ) $(LOADER_OBJ)

.PHONY: loader.bin

loader.bin: $(LOADER_BIN)


$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -e early -Ttext 0x10000 $(LOADER_OBJ_LIST)

$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -s -e early --oformat binary -Ttext 0x10000 $<

undefine SUBDIR