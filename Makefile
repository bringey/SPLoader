
# Default target TODO
all: loader.bin

include mk/marker.mk

include mk/verbose.mk

include mk/constants.mk

include includes.default.mk

# include includes.mk if it exists
-include includes.mk

include mk/loader.mk

# Pattern Rules

# C source to object file
#
$(BUILD_DIR)/%.o: %.c $(MARKER)
	$(CC_V) -MD $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

# Assembly source to object file (with preprocessing)
#
$(BUILD_DIR)/%.o: %.S $(MARKER)
	@$(CPP) -MD -MT $@ $(CPPFLAGS) -o $(BUILD_DIR)/$*.s $<
	$(AS_V) $(ASFLAGS) -o $@ $(BUILD_DIR)/$*.s -a=$(BUILD_DIR)/$*.lst
#	@$(RM) -f $(BUILD_DIR)/$*.s

# TARGETS

$(BUILD_DIR)/usb.img: $(BIOS_BOOTSTRAP_BIN) $(LOADER_EARLY_BIN) $(LOADER_BIN)
	tools/mkimage/mkimage -o $@ -b $(BIOS_BOOTSTRAP_BIN) $(LOADER_EARLY_BIN) $(LOADER_BIN)

.PHONY: all 
#
# Clean
#

purge:
	$(RM) -r $(BUILD_DIR)
