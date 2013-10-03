# Makefile for ARM cryptographic library

##### Build configuration ##############

# Disable built-in rules and variables
MAKEFLAGS += -rR

ifneq ($(wildcard config.mk),)
# By default, build library + test application
all: lib test

# Include project configuration
include config.mk

ifneq ($(filter-out %clean,$(MAKECMDGOALS)),$(if $(MAKECMDGOALS),,fail))
config.mk: configure Makefile rules.mk
	@echo Configuration is out-of-date, running configure
	@sed -n "/.*Configured with/s/[^:]*: //p" $@ | sh
endif
else
config.mk:
ifneq ($(filter-out %clean,$(MAKECMDGOALS)),$(if $(MAKECMDGOALS),,fail))
	@echo "Please call configure before running make!"
	@exit 1
endif
endif

# Don't use implicit rules for these suffixes
%.c:
%.d:
%.S:
configure:
Makefile:


##### Tools ############################

# Toolchain tools
AR       := $(CROSS_COMPILE)ar
AS       := $(CROSS_COMPILE)gcc
CC       := $(CROSS_COMPILE)gcc
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy
OBJDUMP  := $(CROSS_COMPILE)objdump
RANLIB   := $(CROSS_COMPILE)ranlib

# Common tools
RM       := rm -f


##### Board ############################

# $(BOARD) should be set in config.mk

# Board-specific directory
BOARD_DIR := board/$(BOARD)
BOARD_CONFIG := $(BOARD_DIR)/config.mk

# Rebuild project after config change
ifneq ($(BOARD),)
include $(BOARD_CONFIG)
config.mk: $(BOARD_CONFIG)
endif

# Linker script for test application
TEST_LD  := $(BOARD_DIR)/test.ld
LDFLAGS  += -T $(TEST_LD)


##### Options ##########################

# Common flags for assembly/C sources
CFLAGS   += -c

# produce debugging information
# $(DEBUG)=yes/no should be set in config.mk
ifeq ($(DEBUG),yes)
CFLAGS   += -g -O0
else
CFLAGS   += -O3
endif

# board-specific flags
# $(CPUFLAGS) should be set in board config
CFLAGS   += $(CPUFLAGS)

# Endianess
# $(ENDIAN)=BIG/LITTLE should be set in board config
ifeq ($(ENDIAN),BIG)
CFLAGS   += -mbig-endian
CFLAGS   += -DBIG_ENDIAN
LDFLAGS  += -EB
else
CFLAGS   += -mlittle-endian
CFLAGS   += -DLITTLE_ENDIAN
LDFLAGS  += -EL
endif

# Compile in thumb mode
# $(THUMB)=1/0 should be set in board config
# NOTE: Thumb mode currently is not supported
ifeq ($(THUMB),1)
CFLAGS += -mthumb
# CFLAGS += -mthumb-interwork
endif

# Create assembly listings
CFLAGS   += -Wa,-alhms=$(LST_DIR)/$*.lst

# Flags for assembly sources
ASFLAGS  += $(CFLAGS)

# Flags for C sources
CCFLAGS  += $(CFLAGS)
CCFLAGS  += -Wall -Wextra -std=c99 -pedantic
CCFLAGS  += -Wno-unused-parameter -Wno-sign-compare

# Flags for -MD invocation
DEPFLAGS += -MM

# Add include directories to CCFLAGS and DEPFLAGS
INCFLAGS += -I./include
CCFLAGS  += $(INCFLAGS)
DEPFLAGS += $(INCFLAGS)


##### Targets ##########################

# Phony targets
.PHONY: all clean lib test

# Directories
ACL_DIRS := aes common curves ecc gf_2 gf_p primes prng rsa sha
TEST_DIRS := test std $(BOARD_DIR)
LST_DIR  := lst
OBJ_DIR  := obj
BIN_DIR  := bin

# Target definitions
ACL_LIB  := $(BIN_DIR)/acl.a
TEST_ELF := $(BIN_DIR)/test.elf
TEST_BIN := $(BIN_DIR)/test.bin
TEST_ASM := $(BIN_DIR)/test.asm
TEST_MAP := $(BIN_DIR)/test.map

# Dependency lists
ACL_SRCS := $(wildcard $(addsuffix /*.c,$(ACL_DIRS)))
ACL_SRCS += $(wildcard $(addsuffix /*.S,$(ACL_DIRS)))

ACL_OBJS := $(patsubst %.c,%.o,$(ACL_SRCS))
ACL_OBJS := $(patsubst %.S,%.o,$(ACL_OBJS))
ACL_OBJS := $(addprefix $(OBJ_DIR)/,$(ACL_OBJS))

TEST_SRCS := $(wildcard $(addsuffix /*.c,$(TEST_DIRS)))
TEST_SRCS += $(wildcard $(addsuffix /*.S,$(TEST_DIRS)))

TEST_OBJS := $(patsubst %.c,%.o,$(TEST_SRCS))
TEST_OBJS := $(patsubst %.S,%.o,$(TEST_OBJS))
TEST_OBJS := $(addprefix $(OBJ_DIR)/,$(TEST_OBJS))

# Targets for clean rule
CLEAN_TARGETS := $(ACL_LIB) $(LST_DIR) $(OBJ_DIR) $(BIN_DIR)


##### Rules ############################

lib: $(ACL_LIB)

test: $(TEST_BIN) $(TEST_ASM)

# Rule for ARM crypto library
$(ACL_LIB): $(ACL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(call quiet-command,$(AR) rcu $@ $(ACL_OBJS),"  AR $@")
	$(call quiet-command,$(RANLIB) $@,"  RANLIB $@")

# Rule for test application
$(TEST_ELF): $(TEST_OBJS) $(ACL_LIB) $(TEST_LD)
	@mkdir -p $(BIN_DIR)
	$(call quiet-command,$(LD) $(LDFLAGS) -Map $(TEST_MAP) $(TEST_OBJS) $(ACL_LIB) -o $@,"  LD $@")

# Rule for project cleanup
clean:
	-$(call quiet-command,$(RM) -r $(CLEAN_TARGETS),"  RM $(CLEAN_TARGETS)")

# Include rules with generated dependencies
-include $(ACL_OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)

# Include common rules
include rules.mk
