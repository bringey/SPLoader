

BIOS_BOOTSTRAP_OBJ = bios/bootstrap.o
BIOS_BOOTSTRAP_OBJ := $(addprefix $(OBJDIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_OBJ))

BIOS_BOOTSTRAP_BIN = bios/bootstrap.bin
BIOS_BOOTSTRAP_BIN := $(addprefix $(BINDIR)/$(PLATFORMDIR)/,$(BIOS_BOOTSTRAP_BIN))



i386-pc-bios-bootstrap: $(BIOS_BOOTSTRAP_BIN)

$(BIOS_BOOTSTRAP_BIN): $(BIOS_BOOTSTRAP_OBJ) $(MARKER)
	$(LD_V) $(LDFLAGS) -Ttext 0x0 -s --oformat binary -e begtext -o $@ $<