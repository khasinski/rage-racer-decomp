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

.PHONY: all setup stage split build check progress clean distclean help

all: build check

setup:
	$(PY) -m venv .venv
	.venv/bin/pip install -r requirements.txt

stage:
	$(PY) tools/scripts/stage_discs.py

split:
	rm -rf $(ASM_DIR) $(LD_SCRIPT) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES)
	$(PY) -m splat split $(SPLAT_CFG)
	$(PY) tools/scripts/gen_nonmatching_asm.py --version $(VERSION) --basename $(BASENAME)

$(BUILD)/asm/%.s.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -Iinclude -I$(ASM_DIR) -o $@ $<

define compile_c_object
	@mkdir -p $(dir $@)
	$(if $(RAGE_CC1_VERSION_OBJ),RAGE_CC1_VERSION=$(RAGE_CC1_VERSION_OBJ)) $(if $(RAGE_CC1_DARWIN_OBJ),RAGE_CC1_DARWIN=$(RAGE_CC1_DARWIN_OBJ)) $(CC_WRAPPER) $< $@
endef

$(BUILD)/src/%.c.o: src/%.c | $(BUILD)
	$(call compile_c_object)

$(BUILD)/src/main/PAL/main/func_80073748.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007849C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8006A9D8.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8006A0AC.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_800728F4.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80072734.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_800681F0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_800684B0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/StFreeRing.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007B014.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80043134.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80042FA0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80060CB0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/GameRefreshSequenceVolumeScale.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80020C24.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002317C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8005026C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8006E1A8.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80033C18.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80039280.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8001F330.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_800207E0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80021A08.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80022324.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002BF68.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002C168.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002FAE8.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002FBEC.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002FC84.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002FD9C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80024420.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80027A84.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80027D84.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80037714.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80032D5C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8003AE2C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80046600.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80079BB4.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80079E7C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007A17C.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007A2D0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007A3A0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007A410.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007A488.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8007AE68.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/lib/libsnd/SsSeqRestartPlayback.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/lib/libsnd/SpuVmCalculateCurrentPitch.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/lib/libsnd/SpuVmCalculateTonePitch.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8001FC30.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8005B070.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8003A148.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8001B974.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3

# These active sources still need cc1-psx-272. Some use GCC attribute placement
# rejected by gcc2.6.3-psx; others produce different linked section bytes under
# 2.6.3 and would shift the final executable.
$(BUILD)/src/main/PAL/main/func_8001D338.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_80026484.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_800455EC.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_8006DD30.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_8006E390.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/lib/libsnd/SsSeqOpen.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/lib/libspu/_SpuSetAnyVoice.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2


$(BUILD):
	@mkdir -p $@

UNDEFINED_SYMS := linkers/$(VERSION)/undefined_syms_auto.$(BASENAME).txt
UNDEFINED_FUNCS := linkers/$(VERSION)/undefined_funcs_auto.$(BASENAME).txt
UNDEFINED_MANUAL := linkers/$(VERSION)/undefined_syms_manual.txt
ADDR_ALIASES := linkers/$(VERSION)/undefined_addr_aliases.$(BASENAME).txt

build: $(OUT_BIN)

$(ADDR_ALIASES): $(OBJS)
	$(PY) tools/scripts/gen_undefined_addr_aliases.py --nm $(NM) --output $@ $(OBJS)

$(ELF): $(OBJS) $(LD_SCRIPT) $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(UNDEFINED_MANUAL) $(ADDR_ALIASES)
	$(LD) -EL -T $(LD_SCRIPT) -T $(UNDEFINED_SYMS) -T $(UNDEFINED_FUNCS) \
	      -T $(UNDEFINED_MANUAL) -T $(ADDR_ALIASES) \
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
	       $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(ADDR_ALIASES)

help:
	@echo "Targets:"
	@echo "  setup             Create .venv and install Python tooling"
	@echo "  stage             Symlink local disc dumps and extract boot EXEs"
	@echo "  split VERSION=PAL Run splat for PAL or USA"
	@echo "  build VERSION=PAL Build split output"
	@echo "  check VERSION=PAL Verify rebuilt EXE SHA-1"
	@echo "  progress          Update docs/PROGRESS.md and badge JSON"
	@echo "  clean             Remove build/ for selected VERSION"
	@echo "  distclean         Also remove generated asm/linker output"

# --- per-object overrides for newly decompiled functions (from working tree) ---
$(BUILD)/src/main/PAL/main/func_8002A810.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_80029E50.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_8002A6B0.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/func_80077A88.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
$(BUILD)/src/main/PAL/main/func_8006DB74.c.o: RAGE_CC1_VERSION_OBJ := 2.7.2
