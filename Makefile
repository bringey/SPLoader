
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
LOADER_OBJ := $(addprefix $(OBJDIR)/loader/,$(LOADER_OBJ))

LOADER_FINAL_OBJ = loader.o
LOADER_FINAL_OBJ := $(addprefix $(BINDIR)/loader/,$(LOADER_FINAL_OBJ))

LOADER_BIN = loader.bin
LOADER_BIN := $(addprefix $(BINDIR)/loader/,$(LOADER_BIN))

LOADER_OBJ_LIST = $(LOADER_ENTRY_OBJ) $(LOADER_ARCH_OBJ) $(LOADER_OBJ)


# Pattern Rules

# C source to object file
#
$(OBJDIR)/%.o: %.c $(MARKER)
	$(CC_V) -MD $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

# Assembly source to object file (with preprocessing)
#
$(OBJDIR)/%.o: %.S $(MARKER)
	@$(CPP) -MD $(CPPFLAGS) -o $(OBJDIR)/$*.s $<
	$(AS_V) $(ASFLAGS) -o $@ $(OBJDIR)/$*.s -a=$(OBJDIR)/$*.lst
	@$(RM) -f $(OBJDIR)/$*.s

# TARGETS

.PHONY: all loader.bin bootstrap.bin

loader.bin: $(LOADER_BIN)

bootstrap.bin: $(BIOS_BOOTSTRAP_BIN)

$(LOADER_FINAL_OBJ): $(LOADER_OBJ_LIST) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -e early -Ttext 0x1000 $(LOADER_OBJ_LIST)

$(LOADER_BIN): $(LOADER_FINAL_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -o $@ -s -e early --oformat binary -Ttext 0x1000 $<


#
# Clean and realclean
#
# clean: removes all compiled files, while keeping directory structure
# realclean: same as clean, but removes directories
#
# If you want to clean/realclean a specific project use the target
# project-clean where project is the project you want to clean/realclean
#

CLEAN_TARGETS := $(addsuffix -clean,$(PROJECTS))
REALCLEAN_TARGETS := $(addsuffix -realclean,$(PROJECTS))

$(CLEAN_TARGETS): %-clean:
	find $(BUILD_DIR)/$* -type f -delete

$(REALCLEAN_TARGETS): %-realclean:
	rm -r -f $(BUILD_DIR)/$*

clean: $(CLEAN_TARGETS)

realclean: $(REALCLEAN_TARGETS)
	rm -f $(BUILD_DIR)/*

-include $(LOADER_OBJ:.o=.d)
