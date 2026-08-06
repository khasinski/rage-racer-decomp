"""PSY-Q VAB / VAG audio.

The game hands these blocks straight to the SDK, so the format is Sony's, not
Namco's: src/main/PAL/main/audio/audio.c StartAudioSlotLoad(slot, header, body,
table) calls SsVabOpenHeadSticky(header) + SsVabTransBody(body), i.e. argument 2
is a VAB header (VH) and argument 3 its body (VB). Which sub-block is which is
therefore fixed by the call, not guessed:

    CAR_xx.2ND   StartAudioSlotLoad(3, sub[1], sub[3], sub[2])
    SELBGM.BIN   StartAudioSlotLoad(1, sub[0], sub[2], sub[1])

VAB header, checked against RG3.VH: 0x20 fixed bytes, 128 * 16 program
attributes, programCount * 16 * 32 tone attributes, then 256 u16 VAG sizes in
units of 8 bytes. For RG3.VH that is 0x20 + 0x800 + 27*512 + 0x200 = 16416
bytes, exactly the asset's length on disc.
"""

from __future__ import annotations

import struct

VAB_MAGIC = b"pBAV"      # "VABp" little-endian
SEQ_MAGIC = b"pQES"      # "SEQp"

# Sony SPU ADPCM predictor coefficients (x64).
_F0 = (0.0, 60.0, 115.0, 98.0, 122.0)
_F1 = (0.0, 0.0, -52.0, -55.0, -60.0)


def looks_like_vab(buf: bytes, off: int) -> bool:
    return 0 <= off <= len(buf) - 4 and buf[off : off + 4] == VAB_MAGIC


def looks_like_seq(buf: bytes, off: int) -> bool:
    return 0 <= off <= len(buf) - 4 and buf[off : off + 4] == SEQ_MAGIC


def parse_vab_header(buf: bytes, off: int) -> dict | None:
    if not looks_like_vab(buf, off):
        return None
    ver, vab_id, fsize = struct.unpack_from("<III", buf, off + 4)
    _res, programs, tones, vags = struct.unpack_from("<4H", buf, off + 0x10)
    mvol, mpan = buf[off + 0x18], buf[off + 0x19]
    tone_base = off + 0x20 + 128 * 16
    size_base = tone_base + programs * 16 * 32
    if size_base + 512 > len(buf):
        return None
    raw = struct.unpack_from("<256H", buf, size_base)
    sizes = [w * 8 for w in raw[:vags + 1]]
    # The table is 1-based: entry 0 is unused in most VABs.
    sizes = [s for s in sizes if s]
    offsets, acc = [], 0
    for s in sizes:
        offsets.append(acc)
        acc += s
    return {
        "version": ver,
        "vabId": vab_id,
        "declaredSize": fsize,
        "programs": programs,
        "tones": tones,
        "vags": vags,
        "masterVolume": mvol,
        "masterPan": mpan,
        "headerSize": size_base + 512 - off,
        "vagSizes": sizes,
        "vagOffsets": offsets,
        "bodySize": acc,
    }


def decode_vag(data: bytes) -> bytes:
    """One SPU-ADPCM stream to signed 16-bit little-endian PCM."""
    out = bytearray()
    s1 = s2 = 0.0
    for b in range(0, len(data) - 15, 16):
        hdr = data[b]
        shift = hdr & 0x0F
        filt = min((hdr >> 4) & 0x0F, 4)
        flags = data[b + 1]
        if flags & 0x07 == 7:   # end marker with no data
            break
        f0, f1 = _F0[filt], _F1[filt]
        for i in range(28):
            byte = data[b + 2 + (i >> 1)]
            nib = (byte >> 4) if (i & 1) else (byte & 0x0F)
            if nib > 7:
                nib -= 16
            samp = float(nib << (12 - shift)) if shift <= 12 else 0.0
            samp += (s1 * f0 + s2 * f1) / 64.0
            s2, s1 = s1, samp
            v = int(round(samp))
            v = -32768 if v < -32768 else (32767 if v > 32767 else v)
            out += struct.pack("<h", v)
        if flags & 0x01:        # loop/end of this sample
            break
    return bytes(out)


def wav(pcm: bytes, rate: int = 44100) -> bytes:
    return (
        b"RIFF" + struct.pack("<I", 36 + len(pcm)) + b"WAVEfmt "
        + struct.pack("<IHHIIHH", 16, 1, 1, rate, rate * 2, 2, 16)
        + b"data" + struct.pack("<I", len(pcm)) + pcm
    )
