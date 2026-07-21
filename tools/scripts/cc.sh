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
RAGE_CC1_VERSION="${RAGE_CC1_VERSION:-2.6.3}"
case "$RAGE_CC1_VERSION" in
    2.7.2)
        CC1="$BIN_DIR/cc1-psx-272"
        ;;
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
CC1_GZ="$BIN_DIR/cc1-psx-272.gz"
CC1_SHA="$BIN_DIR/cc1-psx-272.gz.sha256"
MASPSX_DIR="${MASPSX_DIR:-$TOOL_DIR/maspsx}"

CC1_URL="https://github.com/Xeeynamo/ff7-decomp/releases/download/init/cc1-psx-272.gz"
CC1_SHA256="1959ced957d8780489874de30d9af79a9154174624b8a00976f4a7f3fad87ac6"
MASPSX_URL="${MASPSX_URL:-https://github.com/mkst/maspsx.git}"
MASPSX_FLAGS="${MASPSX_FLAGS:-}"
MASPSX_PATCH="$ROOT/tools/patches/maspsx-delay-flags.patch"

CPP="${CPP:-mipsel-none-elf-cpp}"
AS="${AS:-mipsel-none-elf-as}"
PYTHON="${PYTHON:-python3}"

mkdir -p "$BIN_DIR" "$(dirname "$OUT")"

download() {
    url="$1"
    out="$2"
    if command -v curl >/dev/null 2>&1; then
        curl -L --fail --silent --show-error -o "$out" "$url"
    elif command -v wget >/dev/null 2>&1; then
        wget -q -O "$out" "$url"
    else
        echo "rage-pc: need curl or wget to fetch $url" >&2
        exit 1
    fi
}

if [ "$RAGE_CC1_VERSION" = "2.7.2" ] && [ ! -x "$CC1" ]; then
    if [ ! -f "$CC1_GZ" ]; then
        download "$CC1_URL" "$CC1_GZ"
    fi
    printf '%s  %s\n' "$CC1_SHA256" "$CC1_GZ" > "$CC1_SHA"
    shasum -a 256 -c "$CC1_SHA" >/dev/null
    gzip -cd "$CC1_GZ" > "$CC1"
    chmod +x "$CC1"
fi

if [ "$RAGE_CC1_VERSION" = "2.6.3" ] && [ ! -x "$CC1" ]; then
    echo "rage-pc: missing $CC1; build/copy gcc2.6.3-psx cc1 there first" >&2
    exit 1
fi

if [ ! -f "$MASPSX_DIR/maspsx.py" ]; then
    rm -rf "$MASPSX_DIR"
    git clone --depth 1 "$MASPSX_URL" "$MASPSX_DIR" >/dev/null
fi

# Reliable "already applied?" guard: --reverse --check succeeds iff the patch is
# already applied. (The old `--help | grep` guard was flaky across python builds.)
if ! git -C "$MASPSX_DIR" apply --recount --reverse --check "$MASPSX_PATCH" >/dev/null 2>&1; then
    if [ ! -f "$MASPSX_PATCH" ]; then
        echo "rage-pc: missing $MASPSX_PATCH; maspsx delay-slot flags unavailable" >&2
        exit 1
    fi
    if ! git -C "$MASPSX_DIR" apply --recount "$MASPSX_PATCH"; then
        echo "rage-pc: failed to apply maspsx delay-slot patch in $MASPSX_DIR" >&2
        exit 1
    fi
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

"$CPP" -I"$ROOT/include" -I"$ROOT/src/main" -undef -Wall -fno-builtin "$IN" \
    | run_cc1 \
    | {
        # Allow targeted per-object maspsx experiments without changing the
        # project default wrapper flags.
        maspsx_extra_args=()
        if [ -n "$MASPSX_FLAGS" ]; then
            # shellcheck disable=SC2206
            maspsx_extra_args=($MASPSX_FLAGS)
        fi
        if grep -q 'MASPSX_FLAGS:.*--stack-return-delay' "$IN"; then
            maspsx_extra_args+=(--stack-return-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--store-return-delay' "$IN"; then
            maspsx_extra_args+=(--store-return-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--la-return-delay' "$IN"; then
            maspsx_extra_args+=(--la-return-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--la-call-delay' "$IN"; then
            maspsx_extra_args+=(--la-call-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--store-call-delay' "$IN"; then
            maspsx_extra_args+=(--store-call-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--store-jump-delay' "$IN"; then
            maspsx_extra_args+=(--store-jump-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--store-branch-delay' "$IN"; then
            maspsx_extra_args+=(--store-branch-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--addiu-branch-delay' "$IN"; then
            maspsx_extra_args+=(--addiu-branch-delay)
        fi
        if grep -q 'MASPSX_FLAGS:.*--load-dest-temp' "$IN"; then
            maspsx_extra_args+=(--load-dest-temp)
        fi
        maspsx_tool="$MASPSX_DIR/maspsx.py"
        if [ "${IN##*/}" = "func_800487D8.c" ]; then
            maspsx_tool="$ROOT/tools/scripts/maspsx_wrappers/func_800487D8.py"
        fi
        "$PYTHON" "$maspsx_tool" \
            --expand-div --aspsx-version=2.34 --force-stdin \
            ${maspsx_extra_args[@]+"${maspsx_extra_args[@]}"} \
            | {
                if [ "${IN##*/}" = "func_80028120.c" ]; then
                    awk '!($0 ~ /^[[:space:]]*nop[[:space:]]*#/ && index($0, "DEBUG: branch/jump"))'
                else
                    cat
                fi
            }
    } \
    | "$AS" -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -I"$ROOT/include" -o "$OUT" -
