

#
# BIOS Bootstrap binary targets
#

BIOS_BOOTSTRAP_OBJ = stage0a/bootstrap.o
BIOS_BOOTSTRAP_OBJ := $(addprefix $(BUILD_DIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_OBJ))
BIOS_BOOTSTRAP_DEP := $(BIOS_BOOTSTRAP_OBJ:.o=.d)

BIOS_BOOTSTRAP_FINAL_OBJ = stage0a/bootstrap.final.o
BIOS_BOOTSTRAP_FINAL_OBJ := $(addprefix $(BUILD_DIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_FINAL_OBJ))

BIOS_BOOTSTRAP_BIN = bootstrap.bin
BIOS_BOOTSTRAP_BIN := $(addprefix $(BUILD_DIR)/,$(BIOS_BOOTSTRAP_BIN))

#
# Platform-specific object files
#

LOADER_ENTRY_OBJ = stage0b/early.o

LOADER_ENTRY_OBJ := $(addprefix $(BUILD_DIR)/$(PLATFORMDIR)/,$(LOADER_ENTRY_OBJ))
LOADER_ENTRY_DEP := $(LOADER_ENTRY_OBJ:.o=.d)

LOADER_PLAT_OBJ = stage0b/a20gate.o \
                  stage0b/memmap.o \
                  abort.o \
                  console.o \
                  disk.o \
                  io.o \
                  mem.o \
                  realmode.o \
                  startup.o

LOADER_PLAT_OBJ := $(addprefix $(BUILD_DIR)/$(PLATFORMDIR)/,$(LOADER_PLAT_OBJ))
LOADER_PLAT_DEP := $(LOADER_PLAT_OBJ:.o=.d)

LOADER_LD_SCRIPT := $(SRC_DIR)/$(PLATFORMDIR)/loader.lds

#
# Optional compiles
#
DEFINES += -DOPT_DISK_MBR -DOPT_DISK_GPT
OPTIONAL_OBJ := disk/mbr.o disk/gpt.o


$(BIOS_BOOTSTRAP_FINAL_OBJ): $(BIOS_BOOTSTRAP_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -Ttext 0x0 -e begtext -o $@ $(BIOS_BOOTSTRAP_OBJ)

$(BIOS_BOOTSTRAP_BIN): $(BIOS_BOOTSTRAP_FINAL_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -Ttext 0x0 -s --oformat binary -e begtext -o $@ $<


#
# PHONY targets
#

.PHONY: bootstrap.bin

bootstrap.bin: $(BIOS_BOOTSTRAP_BIN)

-include $(BIOS_BOOTSTRAP_DEP)
-include $(LOADER_PLAT_DEP)
-include $(LOADER_ENTRY_DEP)
