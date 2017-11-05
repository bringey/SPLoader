# mk/libcore.mk
# Makefile for the minimal core library (libcore)

SUBDIR := libcore

-include $(ISA_MAKEFILE)

-include $(PLATFORM_MAKEFILE)

LIBCORE_OBJ = console.o \
              string.o

LIBCORE_OBJ := $(addprefix $(OUT_DIR)/,$(LIBCORE_OBJ))
LIBCORE_DEP := $(LIBCORE_OBJ:.o=.d)

LIBCORE_LIB := libcore.a
LIBCORE_LIB := $(addprefix $(OUT_DIR)/,$(LIBCORE_LIB))

LIBCORE_OBJ_LIST = $(LIBCORE_ARCH_OBJ) \
                   $(LIBCORE_PLAT_OBJ) \
                   $(LIBCORE_OBJ)


.PHONY: libcore.a

libcore.a: $(LIBCORE_LIB)

$(LIBCORE_LIB): $(LIBCORE_OBJ_LIST) $(MARKER)
	$(AR_V) rcs $@ $(LIBCORE_OBJ_LIST)


-include $(LIBCORE_DEP)

undefine SUBDIR
