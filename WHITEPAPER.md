# Joshémon Technical Whitepaper

**v1.4.0** | November 2026

Joshémon is a full monster-collecting RPG that runs on Game Boy Advance hardware: 105 creatures, eight gyms, a league and a post-game, in a 2 MB cartridge written in C with no engine and no libraries. This paper covers how the world, the battles and the sound fit on a 16.78 MHz ARM7 with 288 KB of RAM.

## The world as data

Every map lives in one hand-edited C file, `gba/data/world.c`. A map is a string of tile letters (T tree, ~ water, k pier, O boulder, and so on), plus arrays of warps, signs and people. A person has a place, a look, lines to say, an optional trainer, and a condition: a list of story flags that must be set or clear for them to appear. The whole story is told through those flags. About 200 of them, kept in one bit array, decide who stands where, which doors open and what people say.

At build time a host program, `tools/paint.c`, reads the same file and draws every map. It picks each tile's edges from its neighbours (shorelines, path borders, tree canopies), draws the people from small hand-drawn grids, and cuts the result into 8x8 hardware tiles with duplicates removed. The cartridge only copies finished tiles. It never draws a map at run time.

## Battles

The battle rules follow the pokeruby decompilation of the 2003 games: the damage formula with its random factor, same-type bonus and type chart, natures, critical hits, accuracy and evasion stages, status, catching odds, EXP and evolution. They live in `src/rules.c`, apart from the screens, so `make test` can check them on the host against hand-worked numbers. Trainers pick moves by expected damage. The screens are built from sprites over a painted backdrop, with text drawn into a tile surface by ARM code running from fast internal RAM.

## Text

The GBA has no framebuffer for tiled modes, so text is drawn into tiles that are handed out the first time a cell is touched. A dialog box claims only the tiles it covers. The font is 5x7 with proportional widths, and every glyph row becomes one masked 32-bit write per tile.

## Sound

The four built-in tone channels play the lead, the harmony, the triangle bass and the drums. A software mixer on top plays up to six sampled instruments (brass, strings, piano, flute, bass and a drum kit) through Direct Sound, refilled from the vertical-blank interrupt. The songs are written as chords and melodies in `music/songs.py`, and `tools/compose.py` turns them into note tables in the cartridge. Every melody is original.

## Saves

The save is one struct written to the cartridge's 32 KB SRAM: party, box, bag, flags, position and money. A magic number marks the layout, and it changes whenever the layout does.

## Testing

`build/gbarun` runs the cartridge headless on libmgba. It follows a key script (`press A 2`, `wait 30`, `shot name`, `expect map 68`) and writes screenshots, sound and raw video. A small helper turns moves like `u3 r2 fight talk` into scripts. Twenty-six scripts walk every chapter, gym and side area, and each pass has to finish with zero failures. The same runner records the videos and the ad.

MIT License, 2026 Joshua Trommel.
