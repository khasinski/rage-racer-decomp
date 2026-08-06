"""Read a PS1 MODE2/2352 disc image and pull RAGE.BIN / RAGE.STR apart.

The archive format is documented in docs/names.md section 6 and section 45a,
derived from the game's own loader (LoadDiscArchiveIndex / LoadAsset):

    RAGE.BIN sector 0 = 135 entries of { u32 sectorOffset, u32 sizeInBytes }
    asset i: LBA = LBA(RAGE.BIN) + sectorOffset, length = sizeInBytes

Nothing here hardcodes a path; the image is always an argument.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass
from pathlib import Path

RAW_SECTOR = 2352
USER_DATA = 2048
HEADER = 24  # sync(12) + address(3) + mode(1) + subheader(8)


class Disc:
    """Sector-addressed view of a MODE2/2352 track."""

    def __init__(self, path: Path):
        self.path = Path(path)
        self.size = self.path.stat().st_size
        if self.size % RAW_SECTOR == 0:
            self.raw = True
            self.sector_count = self.size // RAW_SECTOR
        elif self.size % USER_DATA == 0:
            # Already a cooked .iso.
            self.raw = False
            self.sector_count = self.size // USER_DATA
        else:
            raise ValueError(
                f"{path}: {self.size} bytes is neither a multiple of 2352 nor 2048"
            )
        self._fh = self.path.open("rb")

    def close(self) -> None:
        self._fh.close()

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.close()

    def sector(self, lba: int) -> bytes:
        if self.raw:
            self._fh.seek(lba * RAW_SECTOR + HEADER)
            return self._fh.read(USER_DATA)
        self._fh.seek(lba * USER_DATA)
        return self._fh.read(USER_DATA)

    def read(self, lba: int, nbytes: int) -> bytes:
        out = bytearray()
        n = (nbytes + USER_DATA - 1) // USER_DATA
        for i in range(n):
            out += self.sector(lba + i)
        return bytes(out[:nbytes])


# --------------------------------------------------------------------------
# Minimal ISO 9660 directory walk. Enough to find RAGE.BIN and RAGE.STR.
# --------------------------------------------------------------------------


@dataclass
class IsoFile:
    name: str
    lba: int
    size: int


def _both_endian32(b: bytes, off: int) -> int:
    return struct.unpack_from("<I", b, off)[0]


def read_root_directory(disc: Disc) -> list[IsoFile]:
    pvd = disc.sector(16)
    if pvd[1:6] != b"CD001":
        raise ValueError("no ISO 9660 PVD at sector 16")
    root_record = pvd[156 : 156 + 34]
    root_lba = _both_endian32(root_record, 2)
    root_len = _both_endian32(root_record, 10)

    data = disc.read(root_lba, root_len)
    files: list[IsoFile] = []
    pos = 0
    while pos < len(data):
        rec_len = data[pos]
        if rec_len == 0:
            # Padding to the end of the sector; jump to the next one.
            pos = (pos // USER_DATA + 1) * USER_DATA
            if pos >= len(data):
                break
            continue
        rec = data[pos : pos + rec_len]
        lba = _both_endian32(rec, 2)
        size = _both_endian32(rec, 10)
        name_len = rec[32]
        name = rec[33 : 33 + name_len].decode("ascii", "replace")
        files.append(IsoFile(name, lba, size))
        pos += rec_len
    return files


def volume_label(disc: Disc) -> str:
    return disc.sector(16)[40:72].decode("ascii", "replace").strip()


# --------------------------------------------------------------------------
# The RAGE.BIN archive
# --------------------------------------------------------------------------


@dataclass
class ArchiveEntry:
    index: int
    sector_offset: int
    size: int
    lba: int


def read_archive_index(disc: Disc, base_lba: int, count: int) -> list[ArchiveEntry]:
    """Sector 0 of the archive is the (position, size) table the loader reads."""
    toc = disc.sector(base_lba)
    out = []
    for i in range(count):
        off, size = struct.unpack_from("<II", toc, i * 8)
        out.append(ArchiveEntry(i, off, size, base_lba + off))
    return out


def find_file(files: list[IsoFile], name: str) -> IsoFile:
    for f in files:
        # ISO names carry a ";1" version suffix.
        if f.name.split(";")[0].upper() == name.upper():
            return f
    raise KeyError(name)
