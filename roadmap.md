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

- [x] Chapter 7 (2026-11-02, video `qa/gba/c7-frost.mp4`): ROUTE 8 snow pass, FROSTGATE, an ice cave with sliding floors, GYM 7 LEADER BOREAL (ice), FROST BADGE. The ASH CIRCLE heads for the sunken shrine.
  - [x] ROUTE 8: north of LUMEN by sea, a boulder on the beach that needs STRENGTH, two skiers and a hiker, ice creatures in deep snow. FROSTGATE with its own theme, a Rest House, a Mart and a FLY point. Snow tiles, snowy firs, a snow battle backdrop.
  - [x] ICE CAVE north of FROSTGATE (sliding floors), BOREAL found there, GYM 7, FROST BADGE.
- [x] Chapter 8 (2026-11-02, video `qa/gba/c8-drakespire.mp4`): the sea sleeper wakes.
  - [x] FROZEN BAY (ROUTE 9, west of FROSTGATE): deep snow, a fisher, a swimmer, ASH grunts, SURF out to a rock island. The SUNKEN SHRINE: a flooded hall you SURF across, ADMIN EMBER, ASH LEADER CINDRA at the sleeper's pool. When she loses, the sleeper wakes and swims out to sea. (2026-11-02, script c8a)
  - [x] Legend #106 THALASSOR (water and dragon), drawn by `gba/tools/legend.py`. Once CINDRA falls it rises in the bay's south water; catch it, or calm it and come back. (2026-11-02, script c8b)
  - [x] DRAKESPIRE over the calmed sea (a swimmer blocks the reef gap until then), its own brass theme, Rest House, Mart, FLY point. GYM 8: push the dragon statues with STRENGTH, two dragon tamers, LEADER WYRMOND, the DRAGON BADGE and TM17 DRAGON CLAW. (script c8c)
  The rest: ASH CIRCLE leader CINDRA waits at the sunken shrine, and a second legend (#106, water and dragon) gives battle. Then DRAKESPIRE and GYM 8 LEADER WYRMOND (dragon) for the DRAGON BADGE.
- [x] VICTORY ROAD (2026-11-02, script c9a): south of DRAKESPIRE. Floor 1 needs ROCK SMASH, SURF and STRENGTH; floor 2 is a dark maze for FLASH. Six ace trainers and its own theme. The exit at the top of floor 2 still needs the league.
- [x] JOSHEMON LEAGUE (2026-11-02, video `qa/gba/c10-league.mp4`): the ELITE FOUR (dark, ghost, steel, fighting), the CHAMPION, then MAYA as the last challenger. HALL OF FAME and credits.
  - [x] The gate past VICTORY ROAD, the lobby (heal, shop), ELITE FOUR NOX, VESPERA, FERRAND and BRAKKA, CHAMPION AURIC, each door sealed until its trainer falls, with ELITE FOUR and CHAMPION battle themes. (2026-11-02, script c10a)
  - [x] MAYA runs in after AURIC for one last battle, then the HALL OF FAME (each partner with its level), credits, THE END, and you wake at home with the game saved. (script c10b)
- [x] Post-game (2026-11-02, video `qa/gba/c11-postgame.mp4`): the third sleeper (#107, sky), a battle hall with streaks, and a DEX diploma from PROF. CEDAR when all 105 are caught.
  - [x] PROF. CEDAR, back in CEDAR TOWN after the HALL OF FAME, gives you the two starters you didn't pick, then the DEX DIPLOMA once all 105 are caught. (2026-11-02, script c11a; `make build/qa_dip.gba` shows the diploma)
  - [x] Legend #107 CAELITH (flying and psychic), drawn in `tools/legend.py`. It sleeps on SKYREACH SUMMIT, up a path north of SKYREACH TOWN that a guard keeps closed until you are CHAMPION. (2026-11-02, script c11b on `build/qa_dip.gba`)
  - [x] The BATTLE HALL next to the LEAGUE: the CHAMPION takes on random trainers with three fully evolved creatures at your strongest's level, two levels stronger every seventh win. You heal between battles, a loss just ends the streak, and the best streak is saved. (script c12a)
- [x] All 105 can be had (2026-11-02): every creature is in a wild table or evolves from one; the two starters you didn't pick come from PROF. CEDAR after the league.
- [x] New tiles: ice floor and ice wall (ICE CAVE, GYM 7). The shrine and the league rooms reuse brick, slate and the indoor set.

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
- [x] COVEWICK (2026-11-02, script cv1; cv2 checks the gate). HARBOR CITY's pier runs north as ROUTE 10, THE SEAWALK, a boardwalk over open sea with islets of tall grass. It has four trainers and two items, and a guard who lets you through only with the TIDE BADGE. It leads to COVEWICK TOWN, an island fishing village with its own theme, a Rest House, a Mart and a FLY point. In a house, MARIS gives you PIXIEBELL at level 10; NF_MON gifts are new. West of town is ROUTE 11, COVEWICK WOODS: three trainers, a CUT tree hiding TM04, and a STRENGTH boulder hiding ULTRA BALLS for later. The town map has three new places on it.
- [x] CEDAR COAST and the ELDER GROTTO (2026-11-02, post-game; script co1, with co2 checking the guard). Once you are CHAMPION, a guard steps aside at CEDAR TOWN's east edge. ROUTE 12 runs along the south shore, with ledges, a beach, wild creatures at levels 54 to 58 including NIMBUSONG, and four trainers at 57 to 60. At its end is the ELDER GROTTO, a cave around an underground lake. ROCK SMASH opens the west path and STRENGTH the east, and two more trainers wait inside. At the top, GEMSHADE (#54) sleeps as a level 62 crystal you can fight or catch; any creature can now be a sleeper.
- [ ] Next areas: a desert route with sandstorm tiles, a river route between BASALT and SKYREACH that needs SURF, and a second chain of islands off LUMEN.

Promo: `qa/ad/joshemon-ad.mp4` is a 30 s ad made the way the Joshua Tree ad was: real footage, short narrated lines on the beats, and the title theme as the music bed. `qa/ad/joshemon-x-cover.png` is the 1500x500 X header. The title logo now reads JOSHÉMON, with the acute drawn in main.c's title code.

## Compared with Sapphire (2026-11-02, for reference, not queued)
Sapphire has much more besides: about 200 creatures in its dex where we have 106, 16 towns and 34 routes where we have 8 towns and 9 routes, 50 TMs where we have 17, and DIVE and WATERFALL. Its battles have abilities, natures, held items, double battles and weather. It also has breeding and eggs, shinies, berries, contests, secret bases, the Safari Zone, the Battle Tower, trainer rematches on the PokeNav, trading, and the Good and Super Rods.

## Decided
- 2026-11-02: renamed Gryphon to JOSHEMON. It was local only until Joshua said "I want it public" the same day: now a public repo, a release with the ROM, and joshemon.jaybulb.com. Never on a store.
- 2026-11-02: our own 105 creatures, not the real sprites. Melodies are always our own; the R/S/E soundtrack is a style reference only.
