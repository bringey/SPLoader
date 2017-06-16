
# Default target TODO
all: loader.bin

include mk/marker.mk

include mk/verbose.mk

include mk/constants.mk

include includes.default.mk

# include includes.mk if it exists
-include includes.mk

include arch/$(ISA)-$(PLATFORM)/arch.mk


LOADER_OBJ =
LOADER_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_OBJ))

LOADER_FINAL_OBJ = loader.o
LOADER_FINAL_OBJ := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BUILD_DIR)/loader/,$(LOADER_BIN))

LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) $(LOADER_ARCH_OBJ) $(LOADER_OBJ)


# Pattern Rules

# C source to object file
#
$(BUILD_DIR)/%.o: %.c $(MARKER)
	$(CC_V) -MD $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

# Assembly source to object file (with preprocessing)
#
$(BUILD_DIR)/%.o: %.S $(MARKER)
	@$(CPP) -MD $(CPPFLAGS) -o $(BUILD_DIR)/$*.s $<
	$(AS_V) $(ASFLAGS) -o $@ $(BUILD_DIR)/$*.s -a=$(BUILD_DIR)/$*.lst
	@$(RM) -f $(BUILD_DIR)/$*.s

# TARGETS

.PHONY: all loader.bin bootstrap.bin

loader.bin: $(LOADER_BIN)

bootstrap.bin: $(BIOS_BOOTSTRAP_BIN)

$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -e early -Ttext 0x1000 $(LOADER_OBJ_LIST)

$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -s -e early --oformat binary -Ttext 0x1000 $<


#
# Clean
#

purge:
	$(RM) -r $(BUILD_DIR)
