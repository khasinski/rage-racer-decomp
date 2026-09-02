#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import argparse
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HOME = Path.home()


@dataclass(frozen=True)
class Version:
    name: str
    serial: str
    exe_name: str
    sha1: str
    cue: Path | None = None
    track01: Path | None = None
    archive: Path | None = None
    archive_cue: str | None = None
    archive_track01: str | None = None


VERSIONS = [
    Version(
        name="PAL",
        serial="SCES-006.50",
        exe_name="SCES_006.50",
        sha1="2913e15648eddef40821c5f666460abc04155ee6",
        cue=HOME / "Downloads/Rage Racer (Europe)/Rage Racer (Europe)/Rage Racer (Europe).cue",
        track01=HOME / "Downloads/Rage Racer (Europe)/Rage Racer (Europe)/Rage Racer (Europe) (Track 01).bin",
    ),
    Version(
        name="USA",
        serial="SLUS-004.03",
        exe_name="SLUS_004.03",
        sha1="2661e8bf18d209c98fd70d33e18ab88b10abd52b",
        cue=HOME / "Downloads/Rage Racer/Rage Racer.cue",
        track01=HOME / "Downloads/Rage Racer/Rage Racer (Track 01).bin",
    ),
    Version(
        name="JAP10",
        serial="SLPS-006.00 (v1.0)",
        exe_name="SLPS_006.00",
        sha1="f0ca386e1c7b2c5961b8c2a53cc751a83ae0d406",
        archive=HOME / "Downloads/Rage Racer (Japan) (v1.0).7z",
        archive_cue="Rage Racer (Japan) (v1.0).cue",
        archive_track01="Rage Racer (Japan) (v1.0) (Track 01).bin",
    ),
    Version(
        name="JAP11",
        serial="SLPS-006.00 (v1.1)",
        exe_name="SLPS_006.00",
        sha1="bfa7a4cf466480133c10845eae632a0c4e122360",
        cue=HOME / "Downloads/old/Rage Racer (Japan)/Rage Racer (Japan).cue",
        track01=HOME / "Downloads/old/Rage Racer (Japan)/Rage Racer (Japan) (Track 01).bin",
    ),
]


def force_symlink(src: Path, dest: Path) -> None:
    dest.parent.mkdir(parents=True, exist_ok=True)
    if dest.exists() or dest.is_symlink():
        dest.unlink()
    os.symlink(src, dest)


def raw2352_to_iso(track_path: Path, iso_path: Path) -> None:
    iso_path.parent.mkdir(parents=True, exist_ok=True)
    with track_path.open("rb") as src, iso_path.open("wb") as dest:
        while True:
            sector = src.read(2352)
            if not sector:
                break
            if len(sector) != 2352:
                raise ValueError(f"{track_path}: partial sector of {len(sector)} bytes")
            dest.write(sector[24 : 24 + 2048])


def extract_from_iso(iso_path: Path, out_dir: Path, *names: str) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    cmd = ["7z", "x", "-y", f"-o{out_dir}", str(iso_path), *names]
    result = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    # 7z reports "Unexpected end of archive" because the ISO filesystem includes
    # CDDA extents outside track 1. The requested small files still extract.
    missing = [name for name in names if not (out_dir / name).exists()]
    if missing:
        sys.stderr.write(result.stdout)
        raise RuntimeError(f"failed to extract {', '.join(missing)} from {iso_path}")


def psx_header(path: Path) -> dict[str, int]:
    data = path.read_bytes()[:0x800]
    if data[:8] != b"PS-X EXE":
        raise ValueError(f"{path}: not a PS-X EXE")
    return {
        "entry": struct.unpack_from("<I", data, 0x10)[0],
        "text_addr": struct.unpack_from("<I", data, 0x18)[0],
        "text_size": struct.unpack_from("<I", data, 0x1C)[0],
        "stack": struct.unpack_from("<I", data, 0x30)[0],
    }


def sha1(path: Path) -> str:
    h = hashlib.sha1()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def resolve_disc_inputs(version: Version, build_dir: Path) -> tuple[Path, Path]:
    if version.archive is None:
        if version.cue is None or version.track01 is None:
            raise ValueError(f"{version.name}: incomplete disc input configuration")
        return version.cue, version.track01

    if not version.archive.exists():
        raise FileNotFoundError(version.archive)
    if version.archive_cue is None or version.archive_track01 is None:
        raise ValueError(f"{version.name}: incomplete archive member configuration")
    extracted_dir = build_dir / "disc"
    cue = extracted_dir / version.archive_cue
    track01 = extracted_dir / version.archive_track01
    if not cue.exists() or not track01.exists():
        extracted_dir.mkdir(parents=True, exist_ok=True)
        result = subprocess.run(
            [
                "7z", "x", "-y", f"-o{extracted_dir}", str(version.archive),
                version.archive_cue, version.archive_track01,
            ],
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        if not cue.exists() or not track01.exists():
            sys.stderr.write(result.stdout)
            raise RuntimeError(f"failed to extract {version.name} disc inputs")
    return cue, track01


def stage(version: Version) -> None:
    disc_dir = ROOT / "disc" / version.name
    asset_dir = ROOT / "assets" / version.name
    build_dir = ROOT / "build" / "extract" / version.name
    cue, track01 = resolve_disc_inputs(version, build_dir)
    for path in (cue, track01):
        if not path.exists():
            raise FileNotFoundError(path)

    force_symlink(cue, disc_dir / cue.name)
    force_symlink(track01, disc_dir / track01.name)

    iso_path = build_dir / "track01.iso"
    raw2352_to_iso(track01, iso_path)
    with tempfile.TemporaryDirectory(prefix=f"rage-{version.name.lower()}-") as tmp:
        extracted = Path(tmp)
        extract_from_iso(iso_path, extracted, version.exe_name, "SYSTEM.CNF")
        asset_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(extracted / version.exe_name, asset_dir / "main.exe")
        shutil.copy2(extracted / "SYSTEM.CNF", asset_dir / "SYSTEM.CNF")

    actual = sha1(asset_dir / "main.exe")
    if actual != version.sha1:
        raise RuntimeError(f"{version.name}: SHA-1 mismatch: expected {version.sha1}, got {actual}")

    header = psx_header(asset_dir / "main.exe")
    print(
        f"{version.name}: {version.serial} staged, SHA-1 {actual}, "
        f"entry=0x{header['entry']:08X}, text=0x{header['text_addr']:08X}/0x{header['text_size']:X}"
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--version", action="append", choices=[version.name for version in VERSIONS],
        help="stage only the selected version (repeatable; default: all)",
    )
    args = parser.parse_args()
    selected = set(args.version or [version.name for version in VERSIONS])
    for version in VERSIONS:
        if version.name not in selected:
            continue
        stage(version)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
