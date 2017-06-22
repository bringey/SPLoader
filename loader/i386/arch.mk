
LOADER_ARCH_OBJ = support/io.o

LOADER_ARCH_OBJ := $(addprefix $(BUILD_DIR)/$(ISADIR)/,$(LOADER_ARCH_OBJ))
