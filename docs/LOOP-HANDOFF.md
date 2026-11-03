# Joshemon loop handoff

## Restart prompt
/loop Grow Joshemon (~/Documents/Code/joshemon), the GBA cartridge in C (gba/), to the size of Pokemon Sapphire: 8 gyms, the ASH CIRCLE villains, a league, a post-game, all 105 creatures catchable, a day or two to beat. Work down "Full game" in roadmap.md, one item per pass, written straight into gba/data/world.c (maps, people, trainers, lines, flags) with engine changes in gba/src and songs in gba/music/songs.py. Our own world, story, creatures and music, Sapphire look and feel. Each pass: cd gba && make, play it with build/gbarun and a key script, look at the screens, fix, grade. Each chapter gets a video with sound (gbarun wav + raw frames through ffmpeg). Never push or deploy. No JavaScript. Loop until A+.

## Where it stands (2026-11-02, evening)
- **Chapters 1 to 8 play start to finish:** all eight badges (TIDE, STONE, FEATHER, VOLT, VENOM, MIND, FROST, DRAGON) and every system: battles, catching, the BAG, DEX and PC, marts, field skills, SURF, fishing, the BIKE, TMs, FLY, saves.
- **Chapter 6 data:** it sits in a readable block in data/world.c (search "Chapter 6") with named fields and multi-line rows. Write chapters 7 and 8 the same way.
- **Soundtrack refresh:** done and self-graded A. Joshua no longer opens the files, so grade it yourself.
  - 33 songs in gba/music/songs.py, our own melodies; the R/S/E OST is a style reference only.
  - tools/compose.py arranges each song and writes it into world.c. It warns when a long or strong-beat melody note is outside its chord.
  - Sampled instruments come from tools/samples.py and are mixed in sound.inc. The whole sound tick runs on the VBlank interrupt.
  - Cues: gyms, marts and Rest Houses have their own songs. Leaders, MAYA and the ASH CIRCLE each have their own battle music. Evolution has its own song.
- **Chapter 7, part 1 is done:** ROUTE 8 and FROSTGATE. The maps are in the "Chapter 7" block of world.c. New tiles: `s` snow, `A` deep snow, `F` snowy fir. Maps with border 'F' get snowy paths and signs and a snow battle backdrop. QA start: START+L+R+B+DOWN puts you surfing north of LUMEN with the MIND BADGE. Scripts: c7a (beach boulder), c7b (to FROSTGATE).
- **Chapter 7, part 2 (done 2026-11-02):**
  - ICE CAVE north of FROSTGATE: tiles `Q` ice wall, `I` ice, `e` frosted floor. On `I` you keep sliding until something solid stops you (world_step). Route "uululdruruu" from (9,14). BOREAL there sets FLAG_BOREAL_MET, which removes the GYM guard.
  - GYM 7: ice floor, two skiers, LEADER BOREAL gives the FROST BADGE and TM16 BLIZZARD. Route to BOREAL "uulur" from (6,14).
  - Scripts c7c (cave) and c7g (gym, badge, trainer card). QA starts: START+L+R+B+UP is FROSTGATE under the cave; add RIGHT for the gym door with BOREAL met.
  - Video: qa/gba/c7-frost.mp4 (c7b + c7c + c7g).
- **Chapter 8, part 1 (done 2026-11-02):** FROZEN BAY (MAP_ROUTE9) through FROSTGATE's west edge, and the SUNKEN SHRINE (MAP_SHRINE, MF_CAVE, brick and slate, SURF now works in caves). CINDRA (LOOK_CINDRA) vanishes once beaten (cond on FLAG_BEAT_CINDRA), and her win lines wake the sea sleeper. QA start: START+L+R+B+UP+LEFT is FROSTGATE's west edge with the FROST BADGE. Script: c8a (bay, shrine, five battles, CINDRA).
- **Chapter 8, part 2 (done 2026-11-02):** legend #106 THALASSOR. `python3 tools/legend.py` draws it from layered shapes and writes it into data/art.c after the first 105 (`--png DIR` previews). SPECIES_COUNT is 106, and SPECIES is sized SPECIES_COUNT + 1. wake_sleeper handles later legends: npc .flag is set when you beat it, and .flag + 1 when you catch it, so FLAG_THAL_CAUGHT must follow FLAG_SEA_CALM. Any id from 105 up gets the legend song. Script: c8b (c8a, then out to sea and the fight).
- **Chapter 8, part 3 (done 2026-11-02):** DRAKESPIRE (MAP_DRAKE) through the reef gap at the bay's south edge (a swimmer blocks it until FLAG_SEA_CALM), its theme SONG_DRAKE, Rest House, Mart, FLY. GYM 8 (MAP_GYM8): STRENGTH statues, tamers IVO and RHEA, LEADER WYRMOND (badge 8, TM17 DRAGON CLAW). Boulders now slide on indoor floor, and cleared tiles match the map's floor (ground() in main.c, the KYO underlay in paint.c). The town map loops now size from NODES and LINKS. QA start: START+L+R+B+UP+LEFT+SELECT is DRAKESPIRE with SEA_CALM. Script: c8c. Video: qa/gba/c8-drakespire.mp4.
- **VICTORY ROAD (done 2026-11-02):** MAP_VICTORY1 (SMASH, SURF, STRENGTH) and MAP_VICTORY2 (dark maze), six aces, SONG_VICTORY. QA start: START+L+R+B+UP+LEFT+RIGHT+SELECT is DRAKESPIRE by the cave with the DRAGON BADGE. Script: c9a.
- **League, part 1 (done 2026-11-02):** MAP_LEAGUE (gate), MAP_LOBBY, MAP_E4A to E4D and MAP_CHAMP. Each room's exit has an invisible blocker that vanishes once its trainer is beaten. tr_song: names starting "ELITE " play SONG_ELITE, and "CHAMPION " plays SONG_CHAMPION. In the QA build only, your PP never drops, and you take no damage from ELITE and CHAMPION trainers, so the lone L80 can run the gauntlet on mashed A. Script: c10a (c9a, then the league).
- **League, part 2 (done 2026-11-02):** trainer_talk calls maya_arrives() after AURIC (FLAG_BEAT_AURIC) and hall_of_fame() after the final MAYA (FLAG_BEAT_MAYAF), both in story.inc. MAYA stands in MAP_CHAMP while AURIC is beaten and she isn't, so a loss can be retried. The HALL OF FAME scene (SONG_HOF) shows each partner, then credits and THE END; it sets FLAG_CHAMPION, warps home and saves. gbarun now gives every run an empty in-memory save. The QA build takes no damage in the league rooms. Scripts: c10a, c10b (VICTORY ROAD to THE END). Video: qa/gba/c10-league.mp4.
- **Post-game, part 1 (done 2026-11-02):** postgame_prof() in story.inc. PROF. CEDAR (NF_POSTGAME, at CEDAR's north edge) gives the two starters you didn't pick once FLAG_CHAMPION is set, then the DIPLOMA when 1..105 are all caught. A count over the compiled tables showed all 105 already obtainable, so only the starters were missing.
- **Fixed:** the old positional NPC data used raw look numbers (30 BALL, 31 PC, 32 TABLET). These broke when chapter 7 inserted new looks: item balls on routes 2 to 6 drew as skiers and PCs as BOREAL. They now use names. Add new looks freely, but never write a look as a number.
- **Post-game, part 2 (done 2026-11-02):** CAELITH #107, with SPECIES_COUNT 107; tools/legend.py draws both late legends. MAP_SUMMIT is north of SKYREACH (9,0), with a guard there while FLAG_CHAMPION is clear. wake_sleeper now names the species in its lines. `make build/qa_dip.gba` makes a CHAMPION with all 105 caught: START+L+R starts facing PROF. CEDAR, and +UP starts in SKYREACH under the summit. Script: c11b.
- **Post-game, part 3 (done 2026-11-02):** BATTLE HALL (MAP_HALL) beside the league; battle_hall() is in story.inc. While hall_mode is set, a loss skips the Rest House warp. Save has hall_best, and SAVE_MAGIC is 0x3A4A4F53. On qa_dip, START+L+R+DOWN starts at the hall door. Script: c12a. Video: qa/gba/c11-postgame.mp4.
- **Loop finished (2026-11-02):** the grade is in roadmap.md, under "Grade". The audits were a strict catch count, story gates, map limits, cave water, FLY and the level curve, all clean. Scripts c12b (FLY) and c12a (hall) run on qa_dip.
- **Full playthrough video (2026-11-02):** qa/gba/full-playthrough.mp4 (15 min at 2x, all 8 gyms; a -small copy under 30MB was sent to Joshua). It stitches c1, c4, g1 to g5, c6d, c7a to c7g, c8b, c8c, c11a, c11b and c12a.
- **Gym scripts g1 to g5:** on qa_field, START+L+R plus UP, RIGHT, DOWN, LEFT or UP+LEFT starts under gym 1 to 5's door with only the earlier badges and a L80 on ICE BEAM. Each script walks the gym, beats both trainers and the leader, and checks positions. All pass.
- **League balance and the new player (2026-11-02):** build/qa_real.gba (QA_FIELD + QA_REAL) starts in the lobby with six creatures at 54 to 56, real damage and PP; +RIGHT starts fresh at AURIC's door. Scripts l1 (whole league) and l2 (AURIC and MAYA). League levels are 3 lower, HYPER and MAX POTION are new items at the end of the ITEM enum (SAVE_MAGIC 0x3A4A4F54). After a faint the team screen opens on the first creature still standing. The player's walking frames are hand-drawn grids (HERO in tools/paint.c) and the battle back is drawn at 64x64 (bake_back64). Everyone else walks in the player's frames in their own colours (bake_look: cap or bare head, long hair, lab coat), and trainer portraits are finished at 64x64 by refine64 (1 pixel outline, eyes, hair shine, cheeks).
- **Recipe:** record each segment through a fifo into a 240x160 mp4. Then concat, `setpts=0.5*PTS,scale=720:480:flags=neighbor,fps=30` and `atempo=2.0`, and recompress with crf 30 to get under 30MB.
- **The old JavaScript** sits in ~/Documents/Code/_attic/joshemon-js/. Deleting it outright needs Joshua's OK.

## How the data works
- **Maps:** each is a `MapInfo` in gba/data/world.c with:
  - rows of tile letters, exactly w x h (the legend is in world.h, art in tools/paint.c);
  - warps `W_`, signs `S_`, stock `K_`, wild tables and a song.
- **People:** `N_` arrays hold place, dir, look, flags, item, lines `say`/`alt`, trainer `tr` and a `cond` flag list (0x8000 means the flag must be clear).
- **Story flags:** `FLAG_*` in world.h. Bump SAVE_MAGIC in main.c when the Save layout changes.
- **New maps:** add a MAP_* enum entry in MAP_INFO order.
- **New looks:** add a LOOK_DATA entry plus a LOOK_* enum entry before LOOK_BALL.
- **Dark maps:** set MF_DARK. Darkness is drawn on BG3.
- **New songs:** add a spec to music/songs.py and a SONG_* entry in world.h in the same order, then run `python3 tools/compose.py`.

## QA
- **Headless runs:** `build/gbarun ROM SCRIPT SHOTDIR [wav [raw]]`. SHOTDIR must exist.
- **Script lines:**
  - `wait N`, `press KEYS N`, `shot NAME`;
  - `qa 03000000` (the QA struct is pinned there);
  - `mash KEYS FIELD [V]`, `waitfor FIELD V`, `expect FIELD V`. Fields: battle, talking, map, x, y, dir, song.
- **Script helper:** tools/walk.py turns moves like "u3 r2 fight talk at:MAP,X,Y" into a script.
- **QA builds:**
  - `make qa` builds build/qa_field.gba: START+L+R+B starts at LUMEN ISLE with a L80 TIDELORD; add SELECT to start surfing at ROUTE 5.
  - `make build/qa_juke.gba` plays every song for a listening render.
- **Regression:** scripts/c6d.run and c6h.run must report 0 FAIL. The other scripts in gba/scripts cover walking, dialog, battles, story, menus and field skills.
- **Runner quirks:**
  - A step started while a key is held still finishes, so put `wait 20` between moves.
  - Key presses during a battle intro land in the menu.
  - The BAG remembers its pocket.

## Bigger world loop (2026-11-02)
The COVEWICK area is done: ROUTE 10 (the SEAWALK), COVEWICK TOWN with a Rest House, Mart and gift house, and ROUTE 11 (the woods). The maps are MAP_ROUTE10 to MAP_ROUTE11 (67 to 72), with data under the `cv_` prefix in world.c (`L10_`/`L11_` were already taken by the league and the post-game). Its song is SONG_COVE. `START+L+R+UP+RIGHT` on qa_field starts you under the SEAWALK pier. All 25 scripts pass (`gba/tools/regress.sh`): the 21 old ones plus cv1, cv2, co1 (CEDAR COAST and ELDER GROTTO on qa_dip, `START+L+R+RIGHT`) and co2 (the coast guard on a fresh save). Next, more areas from roadmap.md's "Bigger world". Promo files are in qa/ad.

## State 2026-11-02, late
The latest release is v1.4.0. Shipped since 1.0.0: CEDAR COAST and the ELDER GROTTO (1.1.0), the in-browser player on the site, per-type battle effects (1.2.0), the DUNE ROAD and the SUN RUINS (1.3.0), and natures (1.4.0). Every pass runs `gba/tools/regress.sh 1` and then `2` (26 scripts, about 2 minutes, each capped at 120 s), commits, tags vX.Y.0, releases the .gba and runs `npx wrangler deploy`. Next: abilities (one per species, the common Gen 3 effects first), then a river route between BASALT and SKYREACH. Background tasks stay under 5 minutes, and any headless Chrome gets `--mute-audio`.

## Abilities in progress (uncommitted, 2026-11-02 night)
The working tree holds abilities for 1.5.0, not built or tested yet. `gba/data/parts/dex_4.inc` gives every species one ability (`ABILITY[]`, `ABILITY_NAME[]`, the `AB_*` enum in dex.h), 33 in all. They are wired in `gba/src/battle.inc` (`abil`, `ab_say`, `on_entry` for INTIMIDATE and PRESSURE, `can_status` for INSOMNIA, LIMBER, IMMUNITY, WATER VEIL, MAGMA ARMOR, and OWN TEMPO in `inflict`), `battle_2.inc` (PRESSURE PP, COMPOUNDEYES, LEVITATE, FLASH FIRE, WATER/VOLT ABSORB, the OVERGROW/BLAZE/TORRENT/SWARM pinch boost, GUTS, THICK FAT, SHELL ARMOR, ROCK HEAD, contact effects STATIC/POISON POINT/EFFECT SPORE/ROUGH SKIN, INNER FOCUS, CLEAR BODY/KEEN EYE, EARLY BIRD, SHED SKIN and SPEED BOOST at end of turn) and `battle_3.inc` (RUN AWAY, NATURAL CURE on switch-out, `on_entry(FOE)` after the lead comes out). Still to do: show the ability on the SUMMARY (`menus.inc`, a line at y 88 under the nature), `make` and fix compile errors, `make test`, check `battle_2.inc` is still under 20 KB, play a wild battle against a SNARLET (INTIMIDATE) with gbarun and look at the screens, run `tools/regress.sh 1` and `2` (RNG shifts may need script tweaks like c6d did), then commit, tag v1.5.0, release and deploy. After that, the river route between BASALT and SKYREACH.

## Rules
- **Public** (since 2026-11-02): commit and push to nulljosh/joshemon each pass. `./deploy.sh` updates joshemon.jaybulb.com. A release goes out when a version is worth playing.
- **Battle rules:** read pokeruby before changing one (`gh api search/code -f q="Name repo:pret/pokeruby"`).
- **New creature names:** check every one against the official species list, with no exact or contained matches.
- **Replies to Joshua:** one short line. Grade the work yourself and keep going.
