# Joshemon roadmap

The loop works down this list one item per pass: `cd gba && make`, play it headless with `build/gbarun` and a key script, read the screens, fix, grade. Each chapter gets a video with sound.

## Done
- **The game in a browser (passes 1 to 33).** Gen 3 rules from pokeruby, our own 105 creatures, the Sapphire look, chapters 1 to 5. Joshua's grades: C, then B+, then "very close to pokemon, different enough".
- **The cartridge (C0 to C8, 2026-11-02).** Joshua: "no more JavaScript, write it in C". Everything was rebuilt as a GBA cartridge:
  - toolchain, maps, people, text and menus;
  - SRAM saves, battles, trainers and story;
  - sound, the BAG, DEX and team screens, field skills.

  The painter (`gba/tools/paint.c`) matched the old one pixel for pixel. Then all JavaScript was removed; the old build sits in `~/Documents/Code/_attic/joshemon-js/`.
- **Chapter 6 (2026-11-02):** ROUTE 7, LUMEN ISLE, the dark LIGHTHOUSE lit by FLASH, ADMIN EMBER, MAYA 4, GYM 6 LEADER SABLE and the MIND BADGE. Video: `qa/gba/c6-lumen.mp4`.
- **Soundtrack refresh (2026-11-02):** Joshua asked for one in the Ruby/Sapphire/Emerald style, using their OST as the reference.
  - 27 songs with our own melodies, written as specs in `gba/music/songs.py`.
  - Sampled instruments (brass, strings, piano, flute, bass, drum kit) with reverb, mixed on the sample channel.
  - New cues: gym, leader battle, rival, ASH CIRCLE battle, mart, Rest House, evolution.

## Full game
Target: as big as Pokemon Sapphire, a day or two to beat. That means 8 gyms, the ASH CIRCLE, a league, a post-game, and all 105 creatures catchable.

  The rest: ASH CIRCLE leader CINDRA waits at the sunken shrine, and a second legend (#106, water and dragon) gives battle. Then DRAKESPIRE and GYM 8 LEADER WYRMOND (dragon) for the DRAGON BADGE.

## Grade (2026-11-02, end of the build loop)
**The brief's checklist: A+.** It has 8 gyms, the ASH CIRCLE through CINDRA, VICTORY ROAD, the league and HALL OF FAME, and a post-game with CAELITH, the BATTLE HALL and the DIPLOMA. All 105 creatures can be had, with two more legends on top. 33 songs, and every chapter has a video with sound in `qa/gba`.

The final checks came back clean:
- a strict catch count that uses real tiles and rates;
- every story gate has a setter outside the QA code;
- no map passes the person or sprite limits;
- FLY reaches the new towns;
- the level curve rises three to five per gym, with the league at 55 to 62.

All 16 scripts pass.

**Size against Sapphire: about half.** It has 66 maps, 9 routes, 107 creatures and 17 TMs, where Sapphire has hundreds of maps, 34 routes, about 200 creatures and 50 TMs. By estimate, not a stopwatch, it takes 10 to 15 hours to beat, which fits "a day or two". Sapphire takes 25 to 30.

**League with a real team (2026-11-02):** `make build/qa_real.gba` gives six creatures at 54 to 56 with real damage and PP, mashing each one's strongest move (scripts l1, l2). At the old levels a fresh team still lost to AURIC, so the league dropped three levels (ELITE FOUR 49 to 55, AURIC 54 to 58, MAYA 55 to 59, close to Sapphire's 46 to 58). Now a fresh team beats AURIC and MAYA, and a no-item run clears all four of the ELITE FOUR. HYPER POTION (FROSTGATE, DRAKESPIRE, lobby) and MAX POTION (lobby) were added so players can heal between rooms. The player was redrawn by hand: walking frames in tools/paint.c (HERO), and a full 64x64 back for battles (bake_back64).

## Bigger world (toward Sapphire's size)
The game is about half of Sapphire's size. This loop adds routes and towns one area per pass. Each area gets its own wild creatures, trainers, items, a field skill to come back with, and a script that walks it.
- [ ] Battle depth still missing next to Sapphire: held items, double battles, weather (RAIN DANCE, SUNNY DAY, SANDSTORM, HAIL still fail), and the moves that need them or more state: TAUNT, TORMENT, DISABLE, ENCORE, ATTRACT, PERISH SONG, BATON PASS, SPIKES, BIDE, two-turn moves.
- [ ] Next areas: a river route between BASALT and SKYREACH that needs SURF, a second chain of islands off LUMEN, and a sandstorm on the DUNE ROAD.

Promo: `qa/ad/joshemon-ad.mp4` is a 30 s ad made the way the Joshua Tree ad was: real footage, short narrated lines on the beats, and the title theme as the music bed. `qa/ad/joshemon-x-cover.png` is the 1500x500 X header. The title logo now reads JOSHÉMON, with the acute drawn in main.c's title code.

## Toward 2.0.0: stability, speed, and the source material
Joshua, 2026-11-02: loop to the next major on stability, performance and reliability, checked against pokeruby. One pass per line, each a minor release. Done through 2.0.0: Gen 3 abilities, the move table, dead moves, two-slot saves, monkey tests and frame time.
- [ ] Text: drawing glyphs is the frame cost now, not the boxes (2.0.0 draws boxes as one fill and thin rings). A menu's four labels take about 200 scanlines, over one frame, so a battle drops about 7 frames at text and menu changes. Walking drops none. Next: draw each glyph row with one cell lookup per tile, or cache glyph tiles.
- [ ] Bug: an item pickup in scripts/co1.run (about 99 s in, the river route past CEDAR TOWN) prints "JOSH found a" and then a row of garbage glyphs. The item name lookup is reading past its table.
- [ ] A static check that every map's warps and edges lead somewhere walkable (the monkey covers hangs, not dead ends).

## At 2.0.0: native apps (Joshua, 2026-11-02)
- [ ] macOS and iOS apps: SwiftUI shell around the mGBA core (MPL-2.0) with joshemon.gba bundled, touch pad on iPhone, keyboard and controller on Mac, saves in the app's container. Probe the store name first (the "-émon" ending may meet 5.2.1); fall back to a new store name, Joshémon everywhere else.
- [ ] Submit both to the App Store, GitHub release and tags for the apps, then open the Mac app here and play it.

## Sprites to Sapphire quality (before the App Store submit)
Joshua graded them C+ on 2026-11-02 (self-grade was B-), from a sheet of all 107 fronts. Readable and cute, but many share a few body templates (the boxer biped, the same quadruped), evolutions mostly grow instead of changing shape, a lot of them fill only half of the 64x64 frame, and shading is flat with no light source. The three late legends, drawn by tools/legend.py from layered shapes, are the bar (about A-). Joshua, same day: "nice and colorful but still look elementary and childish compared to pokemon sprites." So the target is Sapphire's grown-up look: real anatomy and proportion, attitude in the pose, darker selective outlines, hue-shifted shadows, and fewer cartoon eyes and flat fills.
- [ ] Redraw the fronts and backs about ten per pass with the legend.py method: distinct silhouette per line, top-left light with three-tone shading, a coloured outline, filling the frame the way Sapphire does. Look at each pass on a contact sheet next to the old one.

## Compared with Sapphire (2026-11-02, for reference, not queued)
Sapphire has much more besides: about 200 creatures in its dex where we have 106, 16 towns and 34 routes where we have 8 towns and 9 routes, 50 TMs where we have 17, and DIVE and WATERFALL. Its battles have abilities, natures, held items, double battles and weather. It also has breeding and eggs, shinies, berries, contests, secret bases, the Safari Zone, the Battle Tower, trainer rematches on the PokeNav, trading, and the Good and Super Rods.

## Decided
- 2026-11-02: renamed Gryphon to JOSHEMON. It was local only until Joshua said "I want it public" the same day: now a public repo, a release with the ROM, and joshemon.jaybulb.com. Never on a store.
- 2026-11-02: our own 105 creatures, not the real sprites. Melodies are always our own; the R/S/E soundtrack is a style reference only.
