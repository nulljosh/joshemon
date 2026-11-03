# Joshemon loop handoff (2026-11-02, night)

## What the loop is
Joshemon is a GBA cartridge written in C (gba/), built to the size of Pokemon Sapphire: eight gyms, a league, a post-game, all 105 creatures catchable. Each pass takes one item from roadmap.md and writes it straight into gba/data/world.c (maps, people, trainers, lines, flags), with engine changes in gba/src and songs in gba/music/songs.py. Each pass: `cd gba && make`, play it headless with build/gbarun and a key script, look at the screens, fix, and grade it yourself. Commit and push each pass. Tag and release when a version is worth playing, then run `./deploy.sh` for the landing. Replies to Joshua are one short line.

The loop is stopped right now. The resume point is the abilities pass for v1.5.0.

## Where things stand
- **Released:** v1.4.0 (natures). Earlier today: 1.0.0 the full game, 1.1.0 CEDAR COAST and the ELDER GROTTO, 1.2.0 per-type move effects, 1.3.0 the DUNE ROAD and the SUN RUINS. The landing and the in-browser player are at joshemon.jaybulb.com.
- **Story:** chapters 1 to 8, VICTORY ROAD, the league with the HALL OF FAME, the post-game (CAELITH #107, PROF. CEDAR's two starters and the diploma), the BATTLE HALL, and the COVEWICK area. SPECIES_COUNT is 107. Full playthrough video is in qa/gba.
- **Uncommitted, for 1.5.0 (abilities, not built or tested yet):**
  - gba/data/parts/dex_4.inc (new) gives every species one ability: `ABILITY[]`, `ABILITY_NAME[]`, and the `AB_*` enum in dex.h. 33 in all.
  - Wired in gba/src/battle.inc (INTIMIDATE and PRESSURE on entry, status immunities, OWN TEMPO in `inflict`), battle_2.inc (the common Gen 3 effects: absorbs, pinch boosts, GUTS, contact effects, SPEED BOOST at end of turn) and battle_3.inc (RUN AWAY, NATURAL CURE on switch-out, `on_entry(FOE)`).
  - Still to do, in order: show the ability on the SUMMARY (menus.inc, a line at y 88 under the nature); `make` and fix compile errors; `make test`; check battle_2.inc is under 20 KB; play a wild SNARLET battle (INTIMIDATE) with gbarun and look at the screens; `tools/regress.sh 1` then `2` (RNG shifts may need script tweaks, as c6d did); commit, tag v1.5.0, release, deploy.
- **Roadmap open:** "Battle depth" (held items, double battles, weather, on top of abilities) and "Next areas" (a river route between BASALT and SKYREACH that needs SURF, a second chain of islands off LUMEN, a sandstorm).

## Next, in order
1. Finish the abilities pass and ship v1.5.0 (list above).
2. The river route between BASALT and SKYREACH.
3. The island chain off LUMEN and the sandstorm, from roadmap.md.
4. Held items, double battles and weather, from roadmap.md.

## How to work here
- **Maps:** a `MapInfo` in gba/data/world.c. Rows of tile letters, exactly w x h (legend in world.h, art in tools/paint.c). Warps `W_`, signs `S_`, stock `K_`, wild tables, a song. New maps get a MAP_* entry in MAP_INFO order. Dark maps set MF_DARK.
- **People:** `N_` arrays (place, dir, look, flags, item, `say`/`alt` lines, trainer `tr`, `cond` flag list where 0x8000 means the flag must be clear). Write looks by name, never as a number. New looks get a LOOK_DATA entry and a LOOK_* enum before LOOK_BALL.
- **Story flags:** FLAG_* in world.h. Bump SAVE_MAGIC in main.c when the save layout changes (now 0x3A4A4F54).
- **Songs:** a spec in music/songs.py and a SONG_* entry in world.h in the same order, then `python3 tools/compose.py`. Sampled instruments come from tools/samples.py.
- **Build:** `cd gba && make`. `make test` runs the Gen 3 rules in src/rules.c. `make build/gbarun` builds the headless runner.
- **QA:** `build/gbarun ROM SCRIPT SHOTDIR [wav [raw]]`, SHOTDIR must exist. Script lines: `wait N`, `press KEYS N`, `shot NAME`, `qa 03000000`, `mash KEYS FIELD [V]`, `waitfor FIELD V`, `expect FIELD V`. tools/walk.py builds scripts from moves.
- **QA builds:** `make qa` (qa_field: every badge, at HARBOR), `make build/qa_dip.gba` (a CHAMPION with all 105 caught), `make build/qa_real.gba` (the league as a new player), `make build/qa_fx.gba` (every move effect), `make build/qa_juke.gba` (every song). Start on the title with START+L+R for a new game with a L5 starter.
- **Regression:** `gba/tools/regress.sh 1` then `2`. 26 scripts, about two minutes, each capped at 120 s. Every line reads 0.
- **Runner quirks:** a step started while a key is held still finishes, so put `wait 20` between moves. Key presses during a battle intro land in the menu. Background tasks stay under 5 minutes, and headless Chrome gets `--mute-audio`.
- **File size:** a global pre-commit hook blocks text files over about 20 KB. Big files live in parts (data/parts/world_N.inc, data/dex_N.inc, tools/parts/paint_N.inc, src/*_N.inc). Start a new part when one nears 18 KB.
- **Video:** each chapter gets a clip in qa/gba. Record a segment through a fifo into a 240x160 mp4, then concat with `setpts=0.5*PTS,scale=720:480:flags=neighbor,fps=30` and `atempo=2.0`, and recompress at crf 30 to stay under 30 MB.
- **Old JavaScript:** in ~/Documents/Code/_attic/joshemon-js/. Deleting it needs Joshua's OK.

## Rules
- **Public since 2026-11-02:** commit and push nulljosh/joshemon each pass. Stage by exact path. `./deploy.sh` updates the landing when a release goes out.
- **Battle rules:** read pokeruby before changing one (`gh api search/code -f q="Name repo:pret/pokeruby"`).
- **New creature names:** check each one against the official species list, with no exact or contained matches.
- **Grading:** grade the work yourself and keep going. Joshua stopped opening media, so send nothing unless he asks.

## Restart prompt
```
/loop Grow Joshemon (~/Documents/Code/joshemon), the GBA cartridge in C (gba/), to the size of Pokemon Sapphire: eight gyms, the ASH CIRCLE villains, a league, a post-game, all 105 creatures catchable. Resume the abilities pass for v1.5.0 first: the 1.5.0 ability data and wiring are already in the working tree (gba/data/parts/dex_4.inc, dex.c, dex.h, battle.inc, battle_2.inc, battle_3.inc), uncommitted. Finish the SUMMARY line, make, make test, a wild SNARLET check, regress.sh 1 and 2, then commit by exact path, tag v1.5.0, release and deploy. After that, work down "Battle depth" and "Next areas" in roadmap.md, one item per pass, written straight into gba/data/world.c with engine changes in gba/src and songs in gba/music/songs.py. Each pass: cd gba && make, play it with build/gbarun and a key script, look at the screens, fix, grade. Each chapter gets a video with sound. Commit and push each pass, stage by exact path. Release and deploy when a version is worth playing. No JavaScript. Loop until A+.
```
