# Rage Racer asset browser

Extracts every asset out of a Rage Racer disc and serves a web page that lists,
previews and 3D-renders them.

**Game data must never enter this repository.** The extractor writes to
`tools/decomp-wip/assets/` by default, which `.gitignore` already covers as a
whole (`tools/decomp-wip/`). The HTML page ships as source only; it loads a
manifest you generate locally.

## Usage

```sh
# extract everything, then serve the browser
python3 tools/assetbrowser/extract.py "/path/to/Rage Racer (Europe) (Track 01).bin" --serve

# faster passes while poking at one thing
python3 tools/assetbrowser/extract.py "<disc>" --only CAR_00,BIG1 --no-vram --no-raw
python3 tools/assetbrowser/extract.py "<disc>" -o /somewhere/else
```

The disc image is always an argument; nothing is hardcoded. Track 01 of a
MODE2/2352 rip is expected, but a cooked 2048-byte-per-sector `.iso` also works.

Flags: `-o/--out`, `--only` (indices or name substrings), `--serve [PORT]`,
`--exe` (re-derive the asset name table from a real `SCES_006.50`),
`--no-raw`, `--no-vram`, `--no-audio`.

Output: `manifest.json`, `index.html`, `models/*.json`, `textures/*.png`,
`images/*_vram.png`, `audio/*.wav`, `raw/*.bin`. A full run takes a few minutes
and produces roughly 370 MB.

## Where the formats come from

Every layout here is read out of the decompilation, not sniffed from headers.
The file comments name the exact source. In short:

| thing | source |
|---|---|
| archive TOC | `LoadDiscArchiveIndex`, `docs/names.md` §6 |
| asset names | `g_AssetPaths`, `docs/names.md` §45a |
| model bank | `RegisterModelBank` / `SelectModelBank` in `asset/model_banks.c` |
| model faces | `SubmitModel` / `SubmitModelFaces` + `jtbl_8007DA14` |
| course objects | `RegisterCourseModels` / `SubmitCourseModel` + `jtbl_8007DA54` |
| terrain cells | `InstallTerrainCellData` / `SubmitTerrainCellFaces` + `jtbl_8007D9F4` |
| images | `UploadImageAsset` / `UploadImageBlock` in `asset/image_upload.c` |
| car texture page | `g_CarImageRect` = `{704, 0, 64, 256}` |
| pack sub-blocks | `LoadRaceAssets`, `LoadCarSelectAssets`, `LoadSelectBgmAssets` |
| VAB / VAG | `StartAudioSlotLoad` hands them to `SsVabOpenHeadSticky` |

`models.py` carries the byte-level tables; read its module docstring first.

## Confidence

The parsers assert rather than assume: a model bank must tile its own extent, a
terrain cell must end exactly where the next one begins, no face may index past
its vertex pool. Those checks pass on all 135 assets of the retail PAL disc, so
a silent misparse would have to be self-consistent 135 times over.

What is *not* decoded, and is shown as raw bytes: `RES.DAT`, the SEQ sequence
blocks inside `SELBGM.BIN` and the car packs, the track camera / environment /
waypoint / event sub-blocks of a `.2ND` pack, and the 11 FMV streams in
`RAGE.STR` (indexed in the manifest, not extracted).
