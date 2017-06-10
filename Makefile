
include mk/marker.mk

include mk/verbose.mk

include mk/constants.mk

include includes.default.mk

# include includes.mk if it exists
-include includes.mk

include arch/$(ISA)-$(PLATFORM)/arch.mk


# Default target TODO
#all:


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

