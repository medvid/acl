$(OBJ_DIR)/%.o: %.c config.mk
	@for dir in $(LST_DIR) $(OBJ_DIR); do mkdir -p $$dir/$$(dirname $<); done
	@$(CC) $(DEPFLAGS) -MT $@ $< > $(OBJ_DIR)/$*.d
	$(call quiet-command,$(CC) $(CCFLAGS) $< -o $@,"  CC $@")

$(OBJ_DIR)/%.o: %.S config.mk
	@for dir in $(LST_DIR) $(OBJ_DIR); do mkdir -p $$dir/$$(dirname $<); done
	@$(CC) $(DEPFLAGS) -MT $@ $< > $(OBJ_DIR)/$*.d
	$(call quiet-command,$(AS) $(ASFLAGS) $< -o $@,"  AS $@")

%.bin: %.elf
	@mkdir -p $(BIN_DIR)
	$(call quiet-command,$(OBJCOPY) -O binary $< $@,"  OBJCOPY $@")

%.asm: %.elf
	@mkdir -p $(BIN_DIR)
	$(call quiet-command,$(OBJDUMP) -D $< > $@,"  OBJDUMP $@")

# Run `make V=1 TARGET` for verbose output
quiet-command = $(if $(V),$1,$(if $(2),@echo $2 && $1, @$1))
