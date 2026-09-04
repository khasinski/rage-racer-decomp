# rage-pc - Rage Racer (PS1) matching decompilation scaffold.

# The game had one European release, SCES-00650. This tree calls it PAL
# throughout; EUR was a second name for the same build and is gone.
VERSION    ?= PAL
BASENAME   := main

TARGET_SHA_PAL := 2913e15648eddef40821c5f666460abc04155ee6
TARGET_SHA_USA := 2661e8bf18d209c98fd70d33e18ab88b10abd52b
TARGET_SHA_JAP10 := f0ca386e1c7b2c5961b8c2a53cc751a83ae0d406
TARGET_SHA_JAP11 := bfa7a4cf466480133c10845eae632a0c4e122360
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
READELF    := mipsel-none-elf-readelf
OBJDIFF    ?= build/toolchain/bin/objdiff-cli

ASM_SRCS := $(shell find $(ASM_DIR) -name '*.s' -not -path '*/nonmatchings/*' 2>/dev/null)
# Compile exactly the translation units named by the selected splat config.
# Regional configs can therefore reuse a proven PAL C unit while leaving the
# surrounding, changed code as regional assembly.
C_SRCS   := $(shell awk -F, '/, c,/{gsub(/^[[:space:]]+|[[:space:]\]]+$$/, "", $$3); print "src/main/" $$3 ".c"}' $(SPLAT_CFG) 2>/dev/null)
SRC_ASM  := $(shell awk -F, '/, hasm,/{gsub(/^[[:space:]]+|[[:space:]\]]+$$/, "", $$3); print "src/main/" $$3 ".s"}' $(SPLAT_CFG) 2>/dev/null)

ASM_OBJS := $(ASM_SRCS:%.s=$(BUILD)/%.s.o)
C_OBJS   := $(C_SRCS:%=$(BUILD)/%.o)
SRC_ASM_OBJS := $(SRC_ASM:%=$(BUILD)/%.o)
OBJS := $(ASM_OBJS) $(C_OBJS) $(SRC_ASM_OBJS)

# HANDWRITTEN_ASM expands to an assembler .include, which cpp's dependency
# output cannot see.  Add the sibling assembly file explicitly so editing a
# named GTE/COP2 (or other hand-written) routine cannot leave a stale object.
HANDWRITTEN_ASM_C_SRCS := $(foreach src,$(C_SRCS),$(if $(wildcard $(src:.c=.s)),$(src)))
$(foreach src,$(HANDWRITTEN_ASM_C_SRCS),$(eval $(BUILD)/$(src).o: $(src:.c=.s)))

# Header dependencies, written by cpp -MD in tools/scripts/cc.sh. Without
# these a change under include/ leaves every dependent object stale, which
# has repeatedly hidden real breakage behind a passing build.
C_DEPS := $(C_OBJS:.o=.o.d)
-include $(C_DEPS)

.PHONY: all setup stage port split build check audit-code test progress expected report clean distclean help

all: build check

setup:
	$(PY) -m venv .venv
	.venv/bin/pip install -r requirements.txt

stage:
	$(PY) tools/scripts/stage_discs.py

port:
	@if [ "$(VERSION)" = PAL ]; then echo "port requires VERSION=USA, JAP10, or JAP11" >&2; exit 2; fi
	$(PY) tools/scripts/map_version_functions.py --target-exe $(TARGET_BIN) \
	      --output configs/$(VERSION)/sym.main.txt \
	      --report $(BUILD)/function-map.json
	$(PY) tools/scripts/port_pal_text_units.py $(VERSION)

split:
	rm -rf $(BUILD)/src $(ASM_DIR) $(LD_SCRIPT) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES) $(ADDR_HALVES)
	$(PY) -m splat split $(SPLAT_CFG)
	$(PY) tools/scripts/gen_nonmatching_asm.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/symbolise_data_words.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/symbolise_header.py --version $(VERSION) --basename $(BASENAME)
	$(PY) tools/scripts/strip_nonmatching_markers.py --version $(VERSION) --basename $(BASENAME)

$(BUILD)/asm/%.s.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -Iinclude -I$(ASM_DIR) -o $@ $<

define compile_c_object
	@mkdir -p $(dir $@)
	$(CC_WRAPPER) $< $@
endef

$(BUILD)/src/%.c.o: src/%.c | $(BUILD)
	$(call compile_c_object)

$(BUILD)/src/%.s.o: src/%.s | $(BUILD)
	@mkdir -p $(dir $@)
	$(AS) -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -Iinclude -I$(ASM_DIR) -o $@ $<

$(BUILD):
	@mkdir -p $@

UNDEFINED_SYMS := linkers/$(VERSION)/undefined_syms_auto.$(BASENAME).txt
UNDEFINED_FUNCS := linkers/$(VERSION)/undefined_funcs_auto.$(BASENAME).txt
UNDEFINED_MANUAL := linkers/$(VERSION)/undefined_syms_manual.txt
ADDR_ALIASES := linkers/$(VERSION)/undefined_addr_aliases.$(BASENAME).txt
ADDR_HALVES := linkers/$(VERSION)/addr_halves.$(BASENAME).txt
PAL_TEXT_ALIASES := configs/$(VERSION)/pal_text_aliases.txt

# A coarse assembly-only comparison target has no C relocations for
# gen_nonmatching_asm.py to record. Keep the linker dependency present; once a
# regional C source is introduced the split step overwrites it with real pairs.
$(ADDR_HALVES):
	@mkdir -p $(dir $@)
	@touch $@

build: $(OUT_BIN)

# splat's undefined_syms_auto / undefined_funcs_auto are NOT linked in: every
# line in them is an assignment, and an assignment overrides a real definition,
# so linking them would pin each address forever. They stay on disk because the
# disassembler reads them for symbol names. This target distils them down to
# the addresses the link actually still needs, and only that file is linked.
$(ADDR_ALIASES): tools/scripts/gen_undefined_addr_aliases.py $(OBJS) $(UNDEFINED_MANUAL) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_HALVES) $(PAL_TEXT_ALIASES) configs/$(VERSION)/sym.main.txt
	$(PY) tools/scripts/gen_undefined_addr_aliases.py --nm $(NM) --output $@ \
	      --source $(UNDEFINED_SYMS) --source $(UNDEFINED_FUNCS) \
	      --source configs/$(VERSION)/sym.main.txt \
	      --manual $(UNDEFINED_MANUAL) --manual $(ADDR_HALVES) \
	      --manual $(PAL_TEXT_ALIASES) $(OBJS)

$(ELF): $(OBJS) $(LD_SCRIPT) $(UNDEFINED_MANUAL) $(ADDR_ALIASES) $(ADDR_HALVES) $(PAL_TEXT_ALIASES)
	$(LD) -EL -T $(LD_SCRIPT) \
	      -T $(ADDR_ALIASES) -T $(UNDEFINED_MANUAL) -T $(ADDR_HALVES) \
	      -T $(PAL_TEXT_ALIASES) \
	      -Map $(BUILD)/$(BASENAME).map -o $@

$(OUT_BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

check: $(OUT_BIN)
	@echo "$(TARGET_SHA)  $(OUT_BIN)" | shasum -c -

audit-code:
	$(PY) tools/scripts/code_debt.py --check
	$(PY) -m unittest tools.tests.test_code_debt

# Enumerated rather than discovered: tools/ has no __init__.py, so unittest
# discovery cannot import it, but the namespace package resolves by name.
TESTS := tools.tests.test_code_debt tools.tests.test_gen_expected \
         tools.tests.test_gen_objdiff_config tools.tests.test_gen_undefined_addr_aliases \
         tools.tests.test_progress_report \
         tools.tests.test_strip_nonmatching_markers tools.tests.test_regional_porting

test:
	$(PY) -m unittest $(TESTS)

progress:
	$(PY) tools/scripts/progress_report.py --version $(VERSION)

# objdiff compares what this tree builds against objects disassembled from the
# game itself. `expected` produces that second side; `report` scores it and
# writes the file decomp.dev ingests. Both need a build that already passed
# `check`, because the target side is named from the verified build's symbols.
expected: check
	$(PY) tools/scripts/gen_expected.py --version $(VERSION) --basename $(BASENAME) \
	      --python $(PY) --as $(AS) --objcopy $(OBJCOPY) --readelf $(READELF)

report: expected
	$(PY) tools/scripts/gen_objdiff_config.py --version $(VERSION) --basename $(BASENAME)
	$(OBJDIFF) report generate -p . -o $(BUILD)/report.json

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm/$(VERSION)/$(BASENAME) $(LD_SCRIPT) \
	       $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES) $(ADDR_HALVES)

help:
	@echo "Targets:"
	@echo "  setup             Create .venv and install Python tooling"
	@echo "  stage             Symlink local disc dumps and extract boot EXEs"
	@echo "  port VERSION=USA  Refresh a regional split from the verified PAL build"
	@echo "  split VERSION=PAL Run splat for PAL, USA, JAP10, or JAP11"
	@echo "  build VERSION=PAL Build split output"
	@echo "  check VERSION=PAL Verify rebuilt EXE SHA-1"
	@echo "  audit-code        Check that game-code scaffolding debt did not increase"
	@echo "  test              Run the tooling unit tests"
	@echo "  progress          Refresh badge JSON and print the progress table"
	@echo "  expected          Build the objdiff target objects from the game EXE"
	@echo "  report            Write build/$$(VERSION)/report.json for decomp.dev"
	@echo "  clean             Remove build/ for selected VERSION"
	@echo "  distclean         Also remove generated asm/linker output"
