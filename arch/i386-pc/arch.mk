

BIOS_BOOTSTRAP_OBJ = bios/bootstrap.o
BIOS_BOOTSTRAP_OBJ := $(addprefix $(OBJDIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_OBJ))

BIOS_BOOTSTRAP_BIN = bios/bootstrap.bin
BIOS_BOOTSTRAP_BIN := $(addprefix $(BINDIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_BIN))

LOADER_ENTRY_OBJ = early/early.o
LOADER_ENTRY_OBJ := $(addprefix $(OBJDIR)/$(PLATFORMDIR)/,$(LOADER_ENTRY_OBJ))

LOADER_ARCH_OBJ = early/a20gate.o

LOADER_ARCH_OBJ := $(addprefix $(OBJDIR)/$(PLATFORMDIR)/,$(LOADER_ARCH_OBJ))

i386-pc-bios-bootstrap: $(BIOS_BOOTSTRAP_BIN)

$(BIOS_BOOTSTRAP_BIN): $(BIOS_BOOTSTRAP_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -Ttext 0x0 -s --oformat binary -e begtext -o $@ $<

-include $(BIOS_BOOTSTRAP_OBJ:.o=.d)
-include $(LOADER_ENTRY_OBJ:.o=.d)
-include $(LOADER_ARCH_OBJ:.o=.d)

