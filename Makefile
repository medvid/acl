# makefile for ARM cryptographic library

LIB := acl.a

CROSS_COMPILE = arm-linux-gnueabihf-

AR       := $(CROSS_COMPILE)ar
AS       := $(CROSS_COMPILE)as
CC       := $(CROSS_COMPILE)gcc
RANLIB   := $(CROSS_COMPILE)ranlib

RM       := rm -f

CFLAGS   = -Wall -I. -c
ASFLAGS  =

SRC_DIRS := aes common curves ecc gf_2 gf_p primes prng rsa sha
LST_DIR  := lst
OBJ_DIR  := obj

CFLAGS   += -Wa,-alhms=$(LST_DIR)/$*.lst
ASFLAGS  += -alhms=$(LST_DIR)/$*.lst


SRCS     := $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
SRCS_ASM := $(wildcard $(addsuffix /*.s,$(SRC_DIRS)))

OBJS     := $(subst .c,.o,$(SRCS))
OBJS_ASM := $(subst .s,.o,$(SRCS_ASM))
OBJS_PRE := $(addprefix $(OBJ_DIR)/,$(OBJS) $(OBJS_ASM))

TEST_DIR := test
TEST_ELF := test.elf
TEST_BIN := test.bin
SRCS_TEST := $(wildcard $(TEST_DIR)/*.c)
OBJS_TEST := $(subst .c,.o,$(SRCS_TEST))
OBJS_TEST_PRE := $(addprefix $(OBJ_DIR)/,$(OBJS_TEST))


all: lib test

lib: $(LIB)

test: $(TEST_ELF)

-include $(OBJS_PRE:.o=.d)
-include $(OBJS_TEST_PRE:.o=.d)

$(LIB): $(OBJS_PRE)
	$(AR) rcu $@ $(OBJS_PRE)
	$(RANLIB) $@

$(TEST_ELF): $(LIB) $(OBJS_TEST_PRE)
	$(CC) $(OBJS_TEST_PRE) $(LIB) -o $@


$(OBJ_DIR)/%.o: %.c
	@for dir in $(LST_DIR) $(OBJ_DIR); do mkdir -p $$dir/$$(dirname $<); done
	$(CC) -MM -MT $@ -I. $< > $(OBJ_DIR)/$*.d
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: %.s
	@for dir in $(LST_DIR) $(OBJ_DIR); do mkdir -p $$dir/$$(dirname $<); done
	$(CC) -MM -MT $@ -I. $< > $(OBJ_DIR)/$*.d
	$(AS) $(ASFLAGS) $< -o $@

clean:
	-$(RM) -r $(LIB) $(LST_DIR) $(OBJ_DIR)

.PHONY: all lib clean test
