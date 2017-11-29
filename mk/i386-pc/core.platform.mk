# mk/i386-pc/core.platform.mk
# Platform-specific makefile for the core module

CORE_PLAT_OBJ = idt/isr.o \
                idt/stubs.o \
                kbd/kbd.o \
                mem/driver.o \
                timer/timer.o \
                io.o \
                nmi.o

CORE_PLAT_OBJ := $(addprefix $(BUILD_DIR)/$(PLATFORMDIR)/,$(CORE_PLAT_OBJ))
CORE_PLAT_DEP := $(CORE_PLAT_OBJ:.o=.d)



-include $(CORE_PLAT_DEP)
