# Joshemon loop handoff (2026-11-02, night)

## What the loop is
Joshemon is a GBA cartridge written in C (gba/), the size of about half of Pokemon Sapphire, public at nulljosh/joshemon and joshemon.jaybulb.com. Joshua's loop now runs to the next major, v2.0.0, on stability, performance and reliability, checked against the source material (pret/pokeruby for the rules, Sapphire for the feel). It works down "Toward 2.0.0" in roadmap.md, one line per pass, each a minor release. Each pass: change it, `cd gba && make`, play it headless with build/gbarun, look at the screens, run /code-review on the diff, keep regress.sh at 0, commit and push, tag, release, `./deploy.sh`. Replies to Joshua are one short line.

## Where things stand
- **Released:** v2.0.0, the stability major: 1.5 to 1.9 plus boxes drawn as one fill and thin rings (pixel-identical). gbarun has `peek HEXADDR`. Profiling showed text glyph drawing, not boxes, is what still drops frames in battle (roadmap).
- **v1.9.0:** The QA struct has `lag`, the frames that missed their VBlank (counted by the VBlank interrupt, read in present()). gbarun `monkey FRAMES SEED` presses random keys and FAILs if the screen freezes for a minute. Scripts mk1 (LUMEN, qa_field: walking drops no frames, then 20000 random frames) and mk2 (post-game, qa_dip) are in the regression. Text box clears still drop one frame each.
- **v1.8.0:** Saves go to two 16 KB SRAM slots in turn (main.c, `sram_write`/`sram_read`), each with a footer of count, hash and magic. A save first blanks the slot's footer magic, writes the data, then the footer, so a cut mid-save leaves the other slot whole; CONTINUE loads the newest slot that checks out. One-copy saves from before 1.8.0 still load. Script sv1 saves twice, power-cycles with gbarun `reset`, then breaks the newest slot with `corrupt 10` and checks it falls back.
- **v1.7.0:** STRUGGLE was an empty slot in the move table (a creature out of PP used a nameless move); it is real now, and `make test` checks every learnable move exists. About 30 moves that only said "But it failed!" work as in Gen 3, in src/battle_moves.inc: PROTECT, DETECT, ENDURE, REST, FOCUS ENERGY, LEECH SEED, MIST, REFLECT, LIGHT SCREEN, SAFEGUARD, HAZE, BELLY DRUM, PSYCH UP, MEAN LOOK, FORESIGHT, MIRROR MOVE, NATURE POWER, MUD and WATER SPORT, HEAL BELL, REFRESH, TELEPORT, MAGNITUDE, FLAIL, REVERSAL, COUNTER, PSYWAVE, ENDEAVOR. Per-side state is `B.fx`. `make build/qa_wild.gba WILD=12 MOVES=182,156,73,222` gives the lead four moves to try.
- **v1.6.0:** the move table matches pokeruby's src/data/battle_moves.c: power, accuracy, PP, priority, targets, the Gen 3 names, and real contact flags (`MOVE_CONTACT` in dex_4.inc, from F_MAKES_CONTACT). `make test` checks that every stat-raising status move aims at the user (HOWL buffed the foe in 1.5.0). `python3 gba/tools/gen3_moves.py` compares the table with pokeruby and prints the contact bits; only SPIT UP should differ. LOW KICK keeps power 50 (our creatures have no weights).
- **v1.5.0:** abilities. Every creature has one of 33 Gen 3 abilities (gba/data/parts/dex_4.inc), shown on the SUMMARY. INTIMIDATE and PRESSURE speak on entry and wait for the other side when it is not out yet (`B.pend`). FLASH FIRE takes any FIRE move and boosts its holder's (`B.ffire`). SPEED BOOST skips a creature's first turn in (`B.fresh`). Contact effects roll per hit.
- **Fixed in 1.5.0, from pokeruby:** GROWL, LEER, SAND ATTACK and every other stat-lowering status move used to lower the user's own stats; they now hit the target. GUILLOTINE, HORN DRILL and FISSURE did nothing; they now work as Gen 3 one-hit KOs. RAPID SPIN lost its later-generation SPEED boost. Move names use the Gen 3 spellings (THUNDERPUNCH, ANCIENTPOWER, HI JUMP KICK) so all fit in 12 letters.
- **Earlier:** 1.0.0 the full game, 1.1.0 CEDAR COAST, 1.2.0 per-type move effects, 1.3.0 the DUNE ROAD, 1.4.0 natures.
- **pokeruby copies:** `gh api repos/pret/pokeruby/contents/<path> -H "Accept: application/vnd.github.raw"` fetches a file. src/battle_util.c has the ability code, src/battle_main.c the turn order, data/battle_scripts_1.s the scripts, src/data/battle_moves.c the move table, src/data/text/move_names_en.h the names.

## Next, in order
0. Landing hero (uncommitted in web/: index.html parallax and gradient, new hero.mp4). The montage opens on an indoor map with black edges: re-cut from outdoor walks only (record cv1, de1, c7b, co1, c2 through a fifo), screenshot phone width through a 390px iframe (headless Chrome won't go under ~500px), grade to A+, then `./deploy.sh`.
1. Joshua's next ask (roadmap "At 2.0.0"): the sprite redraw, then native macOS and iOS apps to the App Store, a release, and the Mac app opened here to play. The two small "Toward 2.0.0" lines (glyph speed, map dead-end check) can ride along.
2. Then v2.0.0. At 2.0.0 Joshua wants native macOS and iOS apps (roadmap "At 2.0.0"), shipped to the App Store with a GitHub release, then the Mac app opened here to play. Then back to "Bigger world" (the river route between BASALT and SKYREACH).

## How to work here
- **Maps:** a `MapInfo` in gba/data/world.c. Rows of tile letters, exactly w x h (legend in world.h, art in tools/paint.c). Warps `W_`, signs `S_`, stock `K_`, wild tables, a song. New maps get a MAP_* entry in MAP_INFO order. Dark maps set MF_DARK.
- **People:** `N_` arrays (place, dir, look, flags, item, `say`/`alt` lines, trainer `tr`, `cond` flag list where 0x8000 means the flag must be clear). Write looks by name, never as a number. New looks get a LOOK_DATA entry and a LOOK_* enum before LOOK_BALL.
- **Story flags:** FLAG_* in world.h. Bump SAVE_MAGIC in main.c when the save layout changes (now 0x3A4A4F54).
- **Songs:** a spec in music/songs.py and a SONG_* entry in world.h in the same order, then `python3 tools/compose.py`. Sampled instruments come from tools/samples.py.
- **Build:** `cd gba && make`. `make test` runs the Gen 3 rules in src/rules.c. `make build/gbarun` builds the headless runner.
- **QA:** `build/gbarun ROM SCRIPT SHOTDIR [wav [raw]]`, SHOTDIR must exist. Script lines: `wait N`, `press KEYS N`, `shot NAME`, `qa 03000000`, `mash KEYS FIELD [V]`, `waitfor FIELD V`, `expect FIELD V`. tools/walk.py builds scripts from moves.
- **QA builds:** `make qa` (qa_field: every badge, at HARBOR), `make build/qa_wild.gba WILD=N` (every grass step meets species N; START+L+R+SELECT starts on ROUTE 2 beside the grass, three steps LEFT), `make build/qa_dip.gba` (a CHAMPION with all 105 caught), `make build/qa_real.gba` (the league as a new player), `make build/qa_fx.gba` (every move effect), `make build/qa_juke.gba` (every song). Start on the title with START+L+R for a new game with a L5 starter.
- **Regression:** `gba/tools/regress.sh 1` then `2`. 29 scripts, about two minutes, each capped at 120 s. Every line reads 0.
- **Runner quirks:** a step started while a key is held still finishes, so put `wait 20` between moves. Key presses during a battle intro land in the menu. Background tasks stay under 5 minutes, and headless Chrome gets `--mute-audio`.
- **File size:** a global pre-commit hook blocks text files over about 20 KB. Big files live in parts (data/parts/world_N.inc, data/dex_N.inc, tools/parts/paint_N.inc, src/*_N.inc, src/battle_team.inc, src/battle_fx.inc). Start a new part when one nears 18 KB.
- **Video:** each chapter gets a clip in qa/gba. Record a segment through a fifo into a 240x160 mp4, then concat with `setpts=0.5*PTS,scale=720:480:flags=neighbor,fps=30` and `atempo=2.0`, and recompress at crf 30 to stay under 30 MB.
- **Old JavaScript:** in ~/Documents/Code/_attic/joshemon-js/. Deleting it needs Joshua's OK.

## Rules
- **Public since 2026-11-02:** commit and push nulljosh/joshemon each pass. Stage by exact path. `./deploy.sh` updates the landing when a release goes out.
- **Battle rules:** read pokeruby before changing one (`gh api search/code -f q="Name repo:pret/pokeruby"`).
- **New creature names:** check each one against the official species list, with no exact or contained matches.
- **Grading:** grade the work yourself and keep going. Joshua stopped opening media, so send nothing unless he asks.

## Restart prompt
```
/loop Joshemon (~/Documents/Code/joshemon): loop until the next major semver (v2.0.0). Focus on stability, performance and reliability, comparing behaviour against the source material (pret/pokeruby Gen 3 rules and Pokemon Sapphire feel). Work down "Toward 2.0.0" in roadmap.md, one line per pass, read docs/LOOP-HANDOFF.md first. Run /code-review on each pass's diff before committing. Keep all regression scripts at 0 FAIL, commit and push each pass, minor bump per pass. Replies to Joshua: very short TLDR. No subagent fan-out, no questions.
```
