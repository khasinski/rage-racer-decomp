#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "usage: $0 input.c output.o" >&2
    exit 2
fi

IN="$1"
OUT="$2"

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
TOOL_DIR="$ROOT/build/toolchain"
BIN_DIR="$TOOL_DIR/bin"
# The game was built entirely with gcc 2.6.3; see docs/names.md,
# "Was 2.7.2 ever used?". The variable is kept so an experiment can still ask
# for another cc1 by path, but nothing in the build sets it.
RAGE_CC1_VERSION="${RAGE_CC1_VERSION:-2.6.3}"
case "$RAGE_CC1_VERSION" in
    2.6.3)
        CC1="$BIN_DIR/cc1-psx-263"
        ;;
    *)
        echo "rage-pc: unsupported RAGE_CC1_VERSION=$RAGE_CC1_VERSION" >&2
        exit 2
        ;;
esac
if [ "$(uname -s)" = "Darwin" ] && [ "${RAGE_CC1_DARWIN:-1}" != "0" ] && [ -x "$CC1-darwin" ]; then
    CC1="$CC1-darwin"
fi
MASPSX_DIR="${MASPSX_DIR:-$TOOL_DIR/maspsx}"

MASPSX_URL="${MASPSX_URL:-https://github.com/mkst/maspsx.git}"

CPP="${CPP:-mipsel-none-elf-cpp}"
AS="${AS:-mipsel-none-elf-as}"
PYTHON="${PYTHON:-python3}"

mkdir -p "$BIN_DIR" "$(dirname "$OUT")"

if [ "$RAGE_CC1_VERSION" = "2.6.3" ] && [ ! -x "$CC1" ]; then
    echo "rage-pc: missing $CC1; build/copy gcc2.6.3-psx cc1 there first" >&2
    exit 1
fi

if [ ! -f "$MASPSX_DIR/maspsx.py" ]; then
    rm -rf "$MASPSX_DIR"
    git clone --depth 1 "$MASPSX_URL" "$MASPSX_DIR" >/dev/null
fi

run_cc1() {
    if "$CC1" -version </dev/null >/dev/null 2>&1; then
        "$CC1" -quiet -mcpu=3000 -g -mgas -gcoff -O2 -G0 -funsigned-char - -o -
    elif command -v qemu-i386 >/dev/null 2>&1; then
        qemu-i386 "$CC1" -quiet -mcpu=3000 -g -mgas -gcoff -O2 -G0 -funsigned-char - -o -
    elif command -v qemu-i386-static >/dev/null 2>&1; then
        qemu-i386-static "$CC1" -quiet -mcpu=3000 -g -mgas -gcoff -O2 -G0 -funsigned-char - -o -
    elif command -v docker >/dev/null 2>&1 && docker version --format '{{.Server.Version}}' >/dev/null 2>&1; then
        cc1_rel="${CC1#$ROOT/}"
        docker run --rm --platform linux/386 -i \
            -v "$ROOT:/work" -w /work \
            i386/alpine:3.20 "/work/$cc1_rel" \
            -quiet -mcpu=3000 -g -mgas -gcoff -O2 -G0 -funsigned-char - -o -
    else
        echo "rage-pc: $CC1 is not runnable on this host and no qemu-i386 or Docker fallback is available" >&2
        exit 1
    fi
}

# -MD makes cpp write "$OUT.d" alongside the object, which the Makefile
# includes so a header edit rebuilds the units that use it.
"$CPP" -I"$ROOT/include" -I"$ROOT/src/main" -undef -Wall -fno-builtin \
    -MD -MF "$OUT.d" -MT "$OUT" "$IN" \
    | run_cc1 \
    | "$PYTHON" "$MASPSX_DIR/maspsx.py" \
        --expand-div --aspsx-version=2.34 --force-stdin \
    | "$AS" -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -I"$ROOT/include" -o "$OUT" -
