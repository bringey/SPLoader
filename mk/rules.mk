# mk/rules.mk
# Rule templates for project makefiles
#
# This makefile defines one variable, GENERATE_RULES, to be used in an eval
# function for every project. This is done so that each project will use its
# own CFLAGS, CPPFLAGS, etc when building.


#
# Function to generate rules for a project
# project specific variables used in a rule should be defined as
# <project>_<variable> (ie loader_CFLAGS)
# $(1): project name
#
define GENERATE_RULES =

#
# .c to .o rule
#
$$(BUILD_DIR)/$(1)/%.o: $(1)/%.c $$(MARKER)
	$$(CC_V) -MD $$($(1)_CPPFLAGS) $$($(1)_CFLAGS) -o $$@ -c $$<

#
# .S to .o rule
# This pattern rule first preprocesses the .S file using cpp, then
# assembles the preprocessed .s file to the target .o file
#
$$(BUILD_DIR)/$(1)/%.o: $(1)/%.S $$(MARKER)
	@$$(CPP) -MD -MT $$@ $$($(1)_CPPFLAGS) -o $$(BUILD_DIR)/$(1)/$$*.s $$<
	$$(AS_V) $$($(1)_ASFLAGS) -o $$@ $$(BUILD_DIR)/$(1)/$$*.s -a=$$(BUILD_DIR)/$(1)/$$*.lst

endef



