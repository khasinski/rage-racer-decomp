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


$(BUILD)/src/main/PAL/main/cd/GameSetCdVolume.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/boot/GameDrawBootLogo.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/menu/GameDrawClassChangeCurtain.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/race/GameDrawSpeedDigits.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/race/GameApplyReplayFrame.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/race/GameInitRecordTables.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/car/GameSeedCarLapProgress.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/car/GameAccumulateLapProgress.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameBlendAngle.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameGetReverseTrackAngle.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameInterpolateTrackPoint.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameSmoothTrackAngle.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/car/GameAdvanceCarPosition.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/save/GameDrawMemoryCardScreen.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/render/GameBuildRaceHudPrims.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/race/GameUpdateRacePosition.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/render/GameUpdateScriptedCamera.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameTriggerRaceCues.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/lib/libsnd/SsSeqRestartPlayback.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/lib/libsnd/SpuVmCalculateCurrentPitch.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/race/GameDrawSeriesClearedWash.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/car/GameClampCarLateralOffset.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3

# No object needs cc1-psx-272 any more: every unit that was pinned to it now
# matches under gcc 2.6.3. See docs/names.md, "Was 2.7.2 ever used?".

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
$(BUILD)/src/main/PAL/main/car/GameUpdateCarDrivetrain.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/gte/GameSubmitCourseModel2.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
$(BUILD)/src/main/PAL/main/track/GameInstallTrackPoints.c.o: RAGE_CC1_VERSION_OBJ := 2.6.3
