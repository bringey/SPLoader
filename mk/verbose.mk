# mk/verbose.mk
# Variables containing verbose and non-verbose versions
# of commands

VERBOSE ?= 1

CC_0 = @echo "    CC      $@"; $(CC)
CC_1 = $(CC)
CC_V = $(CC_$(VERBOSE))

CPP_0 = @echo "    CPP     $@"; $(CPP)
CPP_1 = $(CPP)
CPP_V = $(CPP_$(VERBOSE))

AS_0 = @echo "    AS      $@"; $(AS)
AS_1 = $(AS)
AS_V = $(AS_$(VERBOSE))

AR_0 = @echo "    AR      $@"; $(AR)
AR_1 = $(AR)
AR_V = $(AR_$(VERBOSE))

LD_0 = @echo "    LD      $@"; $(LD)
LD_1 = $(LD)
LD_V = $(LD_$(VERBOSE))

NM_0 = @echo "    NM      $@"; $(NM)
NM_1 = $(NM)
NM_V = $(NM_$(VERBOSE))

OBJCOPY_0 = @echo "    OBJCOPY $@"; $(OBJCOPY)
OBJCOPY_1 = $(OBJCOPY)
OBJCOPY_V = $(OBJCOPY_$(VERBOSE))

BUILDIMAGE_0 = @echo "    BUILDIMAGE $@"; $(BUILDIMAGE)
BUILDIMAGE_1 = $(BUILDIMAGE)
BUILDIMAGE_V = $(BUILDIMAGE_$(VERBOSE))
