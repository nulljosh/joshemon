# Joshemon loop handoff (2026-11-03, afternoon)

## What the loop is
Joshemon is a GBA cartridge written in C (gba/), the size of about half of Pokemon Sapphire, public at nulljosh/joshemon and joshemon.jaybulb.com. The loop works down the open items in roadmap.md and the GitHub issues, one pass at a time. Each pass: change it, `cd gba && make`, play it headless with build/gbarun, look at the screens, run /code-review on the diff, keep regress.sh at 0, commit and push by exact path, and tag and release only when Joshua asks. Rules come from pret/pokeruby and the feel from Sapphire. Replies to Joshua are one short line.

## Where things stand
- **Released:** v2.0.0 is the latest tag and GitHub release.
- **Committed, not yet tagged or released:** 2.0.1 through 2.4.0 (HEAD is ROUTE 15, the LUMEN SHOALS). That covers the glyph drawing fix, the BASALT RIVER route with four trainers, held items (GIVE, TAKE, LEFTOVERS and three type boosters) and the Lumen Shoals route. Joshua cuts the releases himself with the release script.
- **Open issues:** #5 (SwiftUI mGBA shell), #6 (App Store submissions), #7 (sprite redraw with legend.py), #9 (double battles and other battle depth missing next to Sapphire).
- **Sprites:** graded C+. The redraw in #7 is queued before the App Store submit.
- **Context:** context still fills up because this session started before ENABLE_CLAUDEAI_MCP_SERVERS=false was set. Quit Claude Code and relaunch, then check `/context`: no claude.ai connector or engineering plugin tools should be listed. If they are still listed, the setting is not working and the connectors must be removed on claude.ai.

## Next, in order
1. Joshua runs the release script to cut 2.0.1 through 2.4.0: `bash /private/tmp/claude-501/-Users-joshua/3b307cf0-b195-4c23-9cdf-ff052fcc710f/scratchpad/rel/release.sh`.
2. Issue #7, art redraw pass 1: creatures #1 to #10 (SPRIGLING, BRAMBLET, THORNWARDEN, KINDLIT, FLAROX, INFERNYX, SPLASHKIT, RIVOTTER, TIDELORD, NIBBIT). Use the gba/tools/legend.py layered-shape method, generalized so each new sprite replaces the old one by index instead of appending. Compare against the contact sheet of the old sprites at scratchpad/art/old_1_10.png, made by scratchpad/art/old.py.
3. Issue #5: the SwiftUI shell around the mGBA core (MPL-2.0) for macOS and iOS.
4. Issue #6: App Store submissions, then the GitHub release.

## How to work here
- **Maps:** a `MapInfo` in gba/data/world.c. Rows of tile letters, exactly w x h (legend in world.h, art in tools/paint.c). Warps `W_`, signs `S_`, stock `K_`, wild tables, a song. New maps get a MAP_* entry in MAP_INFO order. Dark maps set MF_DARK.
- **People:** `N_` arrays (place, dir, look, flags, item, `say`/`alt` lines, trainer `tr`, `cond` flag list where 0x8000 means the flag must be clear). Write looks by name, never as a number. New looks get a LOOK_DATA entry and a LOOK_* enum before LOOK_BALL.
- **Story flags:** FLAG_* in world.h. Bump SAVE_MAGIC in main.c when the save layout changes (now 0x3A4A4F54).
- **Songs:** a spec in music/songs.py and a SONG_* entry in world.h in the same order, then `python3 tools/compose.py`. Sampled instruments come from tools/samples.py.
- **Build:** `cd gba && make`. `make test` runs the Gen 3 rules in src/rules.c. `make build/gbarun` builds the headless runner.
- **Battle rules:** read pokeruby before changing one: `gh api repos/pret/pokeruby/contents/<path> -H "Accept: application/vnd.github.raw"`. src/battle_util.c has the ability code, src/battle_main.c the turn order, data/battle_scripts_1.s the scripts, src/data/battle_moves.c the move table, src/data/text/move_names_en.h the names. `python3 gba/tools/gen3_moves.py` compares the move table with pokeruby.
- **QA:** `build/gbarun ROM SCRIPT SHOTDIR [wav [raw]]`, SHOTDIR must exist. Script lines: `wait N`, `press KEYS N`, `shot NAME`, `qa 03000000`, `mash KEYS FIELD [V]`, `waitfor FIELD V`, `expect FIELD V`. tools/walk.py builds scripts from moves. `gbarun monkey FRAMES SEED` presses random keys and fails if the screen freezes.
- **QA builds:** `make qa` (qa_field: every badge, at HARBOR), `make build/qa_wild.gba WILD=N` (every grass step meets species N), `make build/qa_dip.gba` (a CHAMPION with all 105 caught), `make build/qa_real.gba` (the league as a new player), `make build/qa_fx.gba` (every move effect), `make build/qa_juke.gba` (every song). Start on the title with START+L+R for a new game with a L5 starter.
- **Regression:** `gba/tools/regress.sh 1` then `2`. 29 scripts, about two minutes, each capped at 120 s. Every line reads 0.
- **Saves:** two 16 KB SRAM slots in turn (main.c, `sram_write`/`sram_read`), each with a footer of count, hash and magic. A save blanks the slot's footer magic, writes the data, then the footer, so a cut mid-save leaves the other slot whole.
- **Runner quirks:** a step started while a key is held still finishes, so put `wait 20` between moves. Key presses during a battle intro land in the menu. Background tasks stay under 5 minutes, and headless Chrome gets `--mute-audio`.
- **File size:** a global pre-commit hook blocks text files over about 20 KB. Big files live in parts (data/parts/world_N.inc, data/dex_N.inc, tools/parts/paint_N.inc, src/*_N.inc, src/battle_team.inc, src/battle_fx.inc). Start a new part when one nears 18 KB. The hook also blocks the big data files on a first commit, so that one commit needs Joshua to run it himself.
- **Video:** each chapter gets a clip in qa/gba. Record a segment through a fifo into a 240x160 mp4, then concat with `setpts=0.5*PTS,scale=720:480:flags=neighbor,fps=30` and `atempo=2.0`, and recompress at crf 30 to stay under 30 MB.
- **Old JavaScript:** in ~/Documents/Code/_attic/joshemon-js/. Deleting it needs Joshua's OK.

## Rules
- **Public since 2026-11-02:** commit and push nulljosh/joshemon each pass. Stage by exact path. `./deploy.sh` updates the landing when a release goes out.
- **New creature names:** check each one against the official species list, with no exact or contained matches.
- **Grading:** grade the work yourself and keep going. Joshua stopped opening media, so send nothing unless he asks.

## Restart prompt
```
/loop /work check all repos for remaining roadmap tasks and all GitHub repos for open issues, work them until 0 issues or problems
```
