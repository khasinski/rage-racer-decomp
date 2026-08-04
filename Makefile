# rage-pc - Rage Racer (PS1) matching decompilation scaffold.

VERSION    ?= PAL
BASENAME   := main

TARGET_SHA_PAL := 2913e15648eddef40821c5f666460abc04155ee6
TARGET_SHA_USA := 2661e8bf18d209c98fd70d33e18ab88b10abd52b
TARGET_SHA := $(TARGET_SHA_$(VERSION))

ROOT       := $(abspath .)
PY         ?= $(if $(wildcard $(ROOT)/.venv/bin/python),$(ROOT)/.venv/bin/python,python3)
SPLAT_CFG  := configs/$(VERSION)/$(BASENAME).yaml

BUILD      := build/$(VERSION)
ASM_DIR    := asm/$(VERSION)/$(BASENAME)
SRC_DIR    := src/$(BASENAME)
LD_SCRIPT  := linkers/$(VERSION)/$(BASENAME).ld
TARGET_BIN := assets/$(VERSION)/$(BASENAME).exe

ELF        := $(BUILD)/$(BASENAME).elf
OUT_BIN    := $(BUILD)/$(BASENAME).exe

CC_WRAPPER := tools/scripts/cc.sh
AS         := mipsel-none-elf-as
LD         := mipsel-none-elf-ld
NM         := mipsel-none-elf-nm
OBJCOPY    := mipsel-none-elf-objcopy

ASM_SRCS := $(shell find $(ASM_DIR) -name '*.s' -not -path '*/nonmatchings/*' 2>/dev/null)
C_SRCS   := $(shell find $(SRC_DIR)/$(VERSION) -name '*.c' 2>/dev/null)

ASM_OBJS := $(ASM_SRCS:%.s=$(BUILD)/%.s.o)
C_OBJS   := $(C_SRCS:%=$(BUILD)/%.o)
OBJS := $(ASM_OBJS) $(C_OBJS)

# Header dependencies, written by cpp -MD in tools/scripts/cc.sh. Without
# these a change under include/ leaves every dependent object stale, which
# has repeatedly hidden real breakage behind a passing build.
C_DEPS := $(C_OBJS:.o=.o.d)
-include $(C_DEPS)

.PHONY: all setup stage split build check progress clean distclean help

all: build check

setup:
	$(PY) -m venv .venv
	.venv/bin/pip install -r requirements.txt

stage:
	$(PY) tools/scripts/stage_discs.py

split:
	rm -rf $(ASM_DIR) $(LD_SCRIPT) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES) $(ADDR_HALVES)
	$(PY) -m splat split $(SPLAT_CFG)
	$(PY) tools/scripts/gen_nonmatching_asm.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/symbolise_data_words.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/symbolise_header.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/gen_bss.py --version $(VERSION) --basename $(BASENAME)

$(BUILD)/asm/%.s.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -Iinclude -I$(ASM_DIR) -o $@ $<

define compile_c_object
	@mkdir -p $(dir $@)
	$(CC_WRAPPER) $< $@
endef

$(BUILD)/src/%.c.o: src/%.c | $(BUILD)
	$(call compile_c_object)

$(BUILD):
	@mkdir -p $@

UNDEFINED_SYMS := linkers/$(VERSION)/undefined_syms_auto.$(BASENAME).txt
UNDEFINED_FUNCS := linkers/$(VERSION)/undefined_funcs_auto.$(BASENAME).txt
UNDEFINED_MANUAL := linkers/$(VERSION)/undefined_syms_manual.txt
ADDR_ALIASES := linkers/$(VERSION)/undefined_addr_aliases.$(BASENAME).txt
ADDR_HALVES := linkers/$(VERSION)/addr_halves.$(BASENAME).txt

build: $(OUT_BIN)

# splat's undefined_syms_auto / undefined_funcs_auto are NOT linked in: every
# line in them is an assignment, and an assignment overrides a real definition,
# so linking them would pin each address forever. They stay on disk because the
# disassembler reads them for symbol names. This target distils them down to
# the addresses the link actually still needs, and only that file is linked.
$(ADDR_ALIASES): $(OBJS) $(UNDEFINED_MANUAL) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_HALVES)
	$(PY) tools/scripts/gen_undefined_addr_aliases.py --nm $(NM) --output $@ \
	      --source $(UNDEFINED_SYMS) --source $(UNDEFINED_FUNCS) \
	      --manual $(UNDEFINED_MANUAL) --manual $(ADDR_HALVES) $(OBJS)

$(ELF): $(OBJS) $(LD_SCRIPT) $(UNDEFINED_MANUAL) $(ADDR_ALIASES) $(ADDR_HALVES)
	$(LD) -EL -T $(LD_SCRIPT) \
	      -T $(UNDEFINED_MANUAL) -T $(ADDR_ALIASES) -T $(ADDR_HALVES) \
	      -Map $(BUILD)/$(BASENAME).map -o $@

$(OUT_BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

check: $(OUT_BIN)
	@echo "$(TARGET_SHA)  $(OUT_BIN)" | shasum -c -

progress:
	$(PY) tools/scripts/progress_report.py

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm/$(VERSION)/$(BASENAME) $(LD_SCRIPT) \
	       $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES) $(ADDR_HALVES)

help:
	@echo "Targets:"
	@echo "  setup             Create .venv and install Python tooling"
	@echo "  stage             Symlink local disc dumps and extract boot EXEs"
	@echo "  split VERSION=PAL Run splat for PAL or USA"
	@echo "  build VERSION=PAL Build split output"
	@echo "  check VERSION=PAL Verify rebuilt EXE SHA-1"
	@echo "  progress          Refresh badge JSON and print the progress table"
	@echo "  clean             Remove build/ for selected VERSION"
	@echo "  distclean         Also remove generated asm/linker output"
