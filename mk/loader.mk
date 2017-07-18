# mk/loader.mk
# Makefile for the loader

SUBDIR := loader

# Architecture-specific makefile
-include $(ISA_MAKEFILE)

# Platform-specific makefile
-include $(PLATFORM_MAKEFILE)


LOADER_OBJ = main/main.o \
             console/out.o \
             console/string.o \
             kbd/parse.o
LOADER_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_OBJ))
LOADER_DEP := $(LOADER_OBJ:.o=.d)

LOADER_FINAL_OBJ = loader.final.o
LOADER_FINAL_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BUILD_DIR)/,$(LOADER_BIN))

LOADER_SYMTAB = symtab.o
LOADER_SYMTAB := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_SYMTAB))


LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) \
                  $(LOADER_ARCH_OBJ) \
                  $(LOADER_PLAT_OBJ) \
                  $(LOADER_OBJ)

.PHONY: loader.bin

loader.bin: $(LOADER_BIN)


$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -r -e _start -Ttext 0x10000 $(LOADER_OBJ_LIST)

$(LOADER_SYMTAB): $(LOADER_FINAL_OBJ:.o=.nl) $(MARKER)
	$(MKSYMTAB) $< > $*.S
	$(AS_V) $(ASFLAGS) -o $@ $*.S


$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(LOADER_SYMTAB) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -s -e _start --oformat binary -Ttext 0x10000 $(LOADER_FINAL_OBJ) $(LOADER_SYMTAB)


-include $(LOADER_DEP)


undefine SUBDIR