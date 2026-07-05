#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import struct
from pathlib import Path


HEADER_FIELDS = (
    "entry_pc",
    "initial_gp",
    "text_addr",
    "text_size",
    "data_addr",
    "data_size",
    "bss_addr",
    "bss_size",
    "stack_top",
    "stack_pointer",
    "stack_base",
    "stack_offset",
)


def main() -> int:
    parser = argparse.ArgumentParser(description="Print PS-X EXE header metadata.")
    parser.add_argument("exe", type=Path)
    args = parser.parse_args()

    data = args.exe.read_bytes()
    if len(data) < 0x800 or data[:8] != b"PS-X EXE":
        raise SystemExit(f"{args.exe}: not a PS-X EXE")

    values = struct.unpack_from("<12I", data, 0x10)
    fields = dict(zip(HEADER_FIELDS, values))
    text_end = fields["text_addr"] + fields["text_size"]

    print(f"path: {args.exe}")
    print(f"size: 0x{len(data):X} ({len(data)} bytes)")
    print(f"sha1: {hashlib.sha1(data).hexdigest()}")
    print(f"sha256: {hashlib.sha256(data).hexdigest()}")
    for name in HEADER_FIELDS:
        print(f"{name}: 0x{fields[name]:08X}")
    print(f"text_end: 0x{text_end:08X}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
