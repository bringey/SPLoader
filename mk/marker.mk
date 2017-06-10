# mk/marker.mk
# Target for creating output directories

.SECONDEXPANSION:

# marker file to create when making directory
MARKER_FILE := .marker

# Marker prereq
MARKER = $$(@D)/$(MARKER_FILE)


%/$(MARKER_FILE):
	@echo "    MKDIR $(dir $@)"
	@mkdir -p $(dir $@)
	@touch $@

.PRECIOUS: %/$(MARKER_FILE)