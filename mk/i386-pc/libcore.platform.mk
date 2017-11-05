# mk/i386-pc/libcore.platform.mk
# Platform-specific makefile for libcore project

LIBCORE_PLAT_OBJ = console/driver.o


LIBCORE_PLAT_OBJ := $(addprefix $(OUT_PLAT_DIR)/,$(LIBCORE_PLAT_OBJ))
LIBCORE_PLAT_DEP := $(LIBCORE_PLAT_OBJ:.o=.d)

-include $(LIBCORE_PLAT_DEP)
