// Story: trainers who spot you, their battles and rewards, and the cutscenes. Included into main.c after battle.inc.
// Ports game.js checkTrainers, npcStep, spotted, trainerTalk.

static void npc_step(int i, int dir) {
  ndir[i] = dir;
  for (int k = 1; k <= 16; k++) { noff[i] = k; next_frame(); }
  nx[i] += DX[dir]; ny[i] += DY[dir]; noff[i] = 0;
}
// The "!" over a trainer's head (game.js ui.bubble): a small box on the text surface.
static void bubble(int i, int on) {
  int x = MAP_PADX * 16 - camx + nx[i] * 16 + 3, y = MAP_PADY * 16 - camy + ny[i] * 16 - 15;
  if (!on) { px(0, x - 1, y, 12, 14); return; }
  box(x - 1, y, 12, 14); text("!", x + 2, y + 3, C_RED, 0);
}
static void maya_arrives(void);
static void hall_of_fame(void);
static void postgame_prof(void);
static void battle_hall(void);
static void trainer_talk(int i) {
  const Npc *n = &mi->npcs[i]; const Trainer *t = n->tr;
  say_lines(t->intro);
  surf_clear();
  int r = trainer_battle(t, n->look);
  if (r != R_WIN) return;
  set_flag(t->beat); if (t->rival) set_flag(t->rival_flag); if (t->flag) set_flag(t->flag);
  if (t->badge) { music(SONG_CAUGHT); say_lines(t->win); music(map_song()); set_flag(BADGE_FLAG[t->badge - 1]); }
  if (t->tm != 255) {
    S.bag[t->tm]++; sfx(SFX_BUY);
    blen = 0; cat(PLAYER_NAME); cat(" received "); cat(ITEM_NAME[t->tm]); cat("! It holds "); cat(MOVES[ITEMS[t->tm].tm].name); cat("."); say(buf);
  }
  say_lines(t->after);
  surf_clear();
  if (t->beat == FLAG_BEAT_AURIC) maya_arrives();
  else if (t->beat == FLAG_BEAT_MAYAF) hall_of_fame();
}
static void spotted(int i) {
  const Trainer *t = mi->npcs[i].tr; music(t && tr_song(t) == SONG_ASHBATTLE ? SONG_ASH : SONG_SPOTTED); // the ASH CIRCLE walk in to their own theme
  bubble(i, 1); wait(45); bubble(i, 0);
  int d = ndir[i];
  while ((nx[i] - me_x) * (nx[i] - me_x > 0 ? 1 : -1) + (ny[i] - me_y) * (ny[i] - me_y > 0 ? 1 : -1) > 1) npc_step(i, d);
  me_dir = d ^ 1;
  trainer_talk(i);
}
// A trainer who has not lost looks down its line of sight; walls and people block it.
static int check_trainers(void) {
  for (int i = 0; i < mi->nnpcs; i++) {
    const Npc *n = &mi->npcs[i];
    if (!n->tr || !visible(i) || flag(n->tr->beat)) continue;
    for (int k = 1; k <= n->tr->sight; k++) {
      int x = nx[i] + DX[ndir[i]] * k, y = ny[i] + DY[ndir[i]] * k;
      if (x == me_x && y == me_y) { spotted(i); return 1; }
      if (has(SOLID, tile_at(x, y)) || npc_at(x, y) >= 0) break;
    }
  }
  return 0;
}

// ---------- full-screen scenes (intro, starter pick, chapter card): their own colours in palette bank 12, a few
// sprites placed by the scene, drawn on the text surface.
#define SCENE_PAL 12
static int scene_n; static Obj scene_obj[8];
static void render_scene(void) {
  for (int i = 0; i < 128; i++) oam[i] = i < scene_n ? scene_obj[i] : (Obj){ 1 << 9, 0, 0, 0 };
}
static void scene_sprite(int k, int x, int y, int tile, int pal) { scene_obj[k] = (Obj){ (y & 255), (x & 511) | (3 << 14), tile | (pal << 12), 0 }; }
static void scene_cols(const u32 *c, int n) { for (int i = 0; i < n; i++) PAL_BG[SCENE_PAL * 16 + 1 + i] = HEX(c[i]); }
static void scene_begin(void) { surf_clear(); scene_n = 0; scene_open = 1; REG_DISPCNT = MODE0 | BG0_ON | OBJ_ON | OBJ_1D; }
static void scene_end(void) { scene_open = 0; scene_n = 0; surf_clear(); }
static int isq(int v) { int r = 0; while ((r + 1) * (r + 1) <= v) r++; return r; }
static void ellipse(int cx, int cy, int rx, int ry, int c, int edge) {
  for (int y = -ry; y <= ry; y++) { int w = (rx * isq(10000 - 10000 * y * y / (ry * ry)) + 50) / 100; px(y * 3 > ry ? edge : c, cx - w, cy + y, w * 2, 1); }
}

// game.js introSpeech: a dusk gradient, the PROF's portrait, a creature on a dark disc
static void intro_speech(void) {
  scene_begin(); music(SONG_INTRO);
  u32 cols[15]; int n = 0, band[160];
  for (int y = 0; y < 160; y++) { // the JS gradient, which only has a handful of steps once it is 15-bit
    u32 c = HEX(((16 + y / 8) << 16) | ((20 + y / 6) << 8) | (48 + y / 4)), k = 0;
    while ((int)k < n && HEX(cols[k]) != c) k++;
    if ((int)k == n && n < 13) cols[n++] = ((16 + y / 8) << 16) | ((20 + y / 6) << 8) | (48 + y / 4);
    band[y] = (k < (u32)n ? k : n - 1) + 1;
  }
  cols[n] = 0x283868; cols[n + 1] = 0x202c58;
  scene_cols(cols, n + 2);
  pen = SCENE_PAL;
  for (int y = 0; y < 160; y++) px(band[y], 0, y, 240, 1);
  ellipse(120, 96, 70, 10, n + 1, n + 2);
  pen = TEXT_PAL;
  load_art(0, 0, ART_PORTRAIT(LOOK_PROF), 0); load_art(64, 1, ART_MON(25, 0), 0);
  scene_sprite(0, 40, 34, 0, 0); scene_sprite(1, 148, 34, 64, 1); scene_n = 2;
  static const char *const L[] = { "Hi! Sorry to keep you waiting!", "Welcome to the world of creatures!", "My name is CEDAR. Everyone around here calls me the PROF.",
    "Creatures live everywhere: in the grass, in the water, up in the sky.", "Some people keep them as partners and battle side by side with them.",
    "I study them. Every year I find kinds I never knew existed.", "And you must be JOSH! You've just moved to CEDAR TOWN, haven't you?",
    "Well, JOSH, your very own adventure is about to begin. Let's go!", 0 };
  say_lines(L);
  fade(16);
  scene_end();
}

// game.js starterScreen: three creatures on a cream sheet, pick with left/right, then YES/NO
static int pick_sel;
static void pick_draw(void) {
  pen = SCENE_PAL;
  for (int i = 0; i < 3; i++) {
    int x = 16 + i * 72, on = i == pick_sel;
    px(1, x - 2, 66, 68, 20); for (int y = 64; y < 88; y += 8) if (y >= 66) px(2, x - 2, y, 68, 4); // the sheet behind the old pad
    ellipse(x + 32, 76, 30, 7, on ? 3 : 5, on ? 4 : 6);
    px(1, x + 26, 0, 12, 12); for (int y = 0; y < 12; y += 8) px(2, x + 26, y, 12, 4);
    if (on) for (int k = 0; k < 4; k++) px(7, x + 28 + k, 2 + k, 7 - k * 2, 1); // game.js arrow, red
  }
  pen = TEXT_PAL;
}
static void pick_place(void) {
  for (int i = 0; i < 3; i++) scene_sprite(i, 16 + i * 72, 16 - (i == pick_sel && (frame >> 4) & 1 ? 2 : 0), i * 64, i);
  scene_n = 3;
}
static void starter_screen(void) {
  static const u32 C[] = { 0xf8e8c0, 0xf0dca8, 0xf8b860, 0xd89840, 0xe0c890, 0xc8b070, 0xe05040 };
  scene_begin(); scene_cols(C, 7); music(SONG_WORLD);
  pen = SCENE_PAL; for (int y = 0; y < 160; y += 8) { px(1, 0, y, 240, 8); px(2, 0, y, 240, 4); } pen = TEXT_PAL;
  for (int i = 0; i < 3; i++) load_art(i * 64, i, ART_MON(STARTERS[i], 0), 0);
  pick_sel = -1; pick_draw(); pick_place();
  say("PROF. CEDAR's bag holds three creatures. Pick one, quick!");
  for (;;) {
    pick_sel = 0; pick_draw();
    say_now("Which one will you take?");
    for (;;) {
      pick_place(); next_frame();
      int to = pick_sel;
      if (pressed(KEY_LEFT) && pick_sel > 0) to--;
      if (pressed(KEY_RIGHT) && pick_sel < 2) to++;
      if (to != pick_sel) { pick_sel = to; pick_draw(); }
      if (pressed(KEY_A)) break;
    }
    blen = 0; cat("Do you choose "); cat(SPECIES[STARTERS[pick_sel]].name); cat("?"); say_now(buf);
    static const char *const YN[] = { "YES", "NO" };
    if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) == 0) {
      Mon *m = &S.party[0]; make_mon(m, STARTERS[pick_sel], 5); S.nparty = 1; S.starter = m->id; mark(S.seen, m->id); mark(S.caught, m->id);
      break;
    }
  }
  scene_end();
}

// game.js profScene: you reach the top of CEDAR TOWN without a partner
static int npc_index(int nf) { for (int i = 0; i < mi->nnpcs; i++) if (mi->npcs[i].flags & nf) return i; return -1; }
static void heal_all(void) { for (int i = 0; i < S.nparty; i++) { Mon *m = &S.party[i]; m->hp = m->stats[0]; m->status = 0; for (int k = 0; k < 4; k++) if (m->moves[k].id) m->moves[k].pp = MOVES[m->moves[k].id].pp; } }
static void prof_scene(void) {
  me_dir = DIR_UP; walk = 0;
  say("???: H-help! Somebody, help me!");
  int p = npc_index(NF_PROF);
  nx[p] = me_x; ny[p] = 0; ndir[p] = DIR_DOWN; set_flag(FLAG_PROFOUT);
  npc_step(p, DIR_DOWN);
  say("PROF. CEDAR: You there! A wild SNARLET chased me right out of the grass!");
  say("PROF. CEDAR: My bag, by my feet! Grab a partner from it and help me!");
  surf_clear();
  starter_screen();
  load_map_gfx(); REG_DISPCNT = world_dispcnt();
  make_mon(&B.foe, 12, 2); B.tr = 0; story_battle = 1; battle(); story_battle = 0;
  const char *nm = mon_name(&S.party[0]);
  say("PROF. CEDAR: Phew... That was close. Thank you!");
  blen = 0; cat("PROF. CEDAR: You must be JOSH, the new kid in town. You and "); cat(nm); cat(" work well together."); say(buf);
  blen = 0; cat("PROF. CEDAR: Keep "); cat(nm); cat(". I think it chose you as much as you chose it."); say(buf);
  music(SONG_CAUGHT); say("JOSH received the DEX!");
  say("PROF. CEDAR: The DEX records every creature you see and catch. Fill it up for me, would you?");
  S.bag[ITEM_BALL] += 5; S.bag[ITEM_POTION] += 1; say("JOSH received 5 BALLS and a POTION!");
  music(SONG_WORLD); say("PROF. CEDAR: My daughter MAYA is training on ROUTE 1, just north of here. Go say hello!");
  set_flag(FLAG_STARTER);
  surf_clear();
  npc_step(p, DIR_UP);
  S.flags[FLAG_PROFOUT >> 3] &= ~(1 << (FLAG_PROFOUT & 7));
}
static void heal_jingle(void) { music(SONG_HEAL); wait(150); music(map_song()); }
// game.js homeRest: MOM's door in CEDAR TOWN
static void home_rest(void) {
  if (!flag(FLAG_STARTER)) say("MOM: Go say hello to PROF. CEDAR, sweetie! He's just north of town.");
  else {
    say("MOM: JOSH! You look worn out. Come in and take a nap.");
    fade(16); heal_jingle(); heal_all(); S.heal[0] = HOME.map; S.heal[1] = HOME.x; S.heal[2] = HOME.y; S.heal[3] = HOME.dir; fade(0);
    say("MOM: All rested? Go get them, sweetie!");
  }
  surf_clear();
}
// game.js nurse: the Rest House counter
static void nurse(void) {
  static const char *const YN[] = { "YES", "NO" };
  say("Welcome to the REST HOUSE! We restore tired creatures to full health.");
  say_now("Shall we restore your team?");
  if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) != 0) { say("We hope to see you again!"); return; }
  say("OK, I'll take your team for a few seconds.");
  heal_jingle(); heal_all(); S.heal[0] = cur; S.heal[1] = 4; S.heal[2] = 3; S.heal[3] = DIR_UP;
  say("Thank you for waiting! Your team is fighting fit. We hope to see you again!");
}
static int badge_count(void) { int n = 0; for (int b = 0; b < BADGE_COUNT; b++) n += flag(BADGE_FLAG[b]); return n; }
static void shake(int n) {
  sfx(SFX_WIPE); for (int i = 0; i < n; i++) { shake_x = i % 4 < 2 ? 2 : -2; wait(2); } shake_x = 0; }

// game.js chapterCard: dark sky, stars drifting, a big gold title, PRESS A
static int hash(int x, int y) { return (int)((((u32)x * 73856093u) ^ ((u32)y * 19349663u)) % 997); }
static void chapter_card(const char *title, const char *sub) {
  static const u32 C[] = { 0x080810, 0x606880, 0xf8f8f8, 0xf8d040, 0xc06818, 0x283878, 0xa8b0d0 };
  scene_begin(); scene_cols(C, 7); music(SONG_INTRO); pen = SCENE_PAL; px(1, 0, 0, 240, 160);
  int sx[50], sy[50];
  for (int i = 0; i < 50; i++) sx[i] = -1;
  for (int t = 0;; t++) {
    for (int i = 0; i < 50; i++) {
      int x = (hash(i, 3) + frame / (2 + i % 3)) % 240, y = hash(3, i) % 160;
      if (sx[i] >= 0) px(1, sx[i], sy[i], 1, 1);
      px(i % 5 ? 2 : 3, x, y, 1, 1); sx[i] = x; sy[i] = y;
    }
    if (t == 0) text_s(title, (240 - measure(title) * 2) / 2, 56, 4, 5, 2);
    if (t == 60) text(sub, (240 - measure(sub)) / 2, 84, 3, 6);
    if (t > 120) { if ((frame >> 5) & 1) text("PRESS A", 100, 140, 7, 0); else px(1, 100, 140, 40, 8); }
    pen = TEXT_PAL; next_frame(); pen = SCENE_PAL;
    if (t > 120 && (pressed(KEY_A) || pressed(KEY_START))) break;
  }
  pen = TEXT_PAL;
  scene_end();
  load_map_gfx(); music(map_song()); REG_DISPCNT = world_dispcnt();
}

// game.js openTablet, wakeSleeper, ending: the end of chapter 3
static void open_tablet(int to) {
  say("The tablet begins to glow...");
  shake(30);
  blen = 0; cat("The three badges in "); cat(PLAYER_NAME); cat("'s case shine back at it!"); say(buf);
  say("The wall behind the tablet rumbles open!");
  static const Warp w = { 0, 0, MAP_HOLLOW, 6, 10, DIR_UP };
  surf_clear(); (void)to; warp(&w);
}
static void ending(void) {
  int caught = (S.caught[105 >> 3] >> (105 & 7)) & 1;
  say(caught ? "The great beast settles into its ball. The mountain goes quiet." : "The great beast sinks back into the earth, calm at last. The mountain goes quiet.");
  static const Warp w = { 0, 0, MAP_BASALT, 16, 9, DIR_LEFT };
  surf_clear(); warp(&w);
  static const char *const L[] = { "MAYA: JOSH! Was that you? The whole mountain shook!", "MAYA: Dad says the old stories were true. And you were the one the tablet meant.",
    "MAYA: ...OK, fine. You win this round.", 0 };
  say_lines(L); music(SONG_ASH);
  static const char *const L2[] = { "???: Heh. So the old stories were true after all.",
    "ASH GRUNT: The ASH CIRCLE thanks you, kid. You woke the mountain sleeper so we did not have to.",
    "ASH GRUNT: It was only the first. Our leader CINDRA wants the one sleeping under the sea.", "ASH GRUNT: Stay out of our way!",
    "MAYA: Who WERE those creeps? They ran south from SKYREACH, toward VOLTARA TOWN.", "MAYA: I'm going after them. Heal up and follow me, JOSH!", 0 };
  say_lines(L2);
  set_flag(FLAG_CH4);
  chapter_card("CHAPTER 4", "THE ASH CIRCLE");
}
static void wake_sleeper(int i) {
  const Npc *n = &mi->npcs[i];
  if (n->say) say_lines(n->say);
  say("..."); cry(n->legend, 0); shake(40); say(n->legend == 105 ? "GRRRAAOOOH!" : n->legend == 106 ? "KHYOOOOAAAHH!" : n->legend == 107 ? "KYAAAAAAAH!" : "KSSHHHRRRNN!"); surf_clear();
  make_mon(&B.foe, n->legend, n->lv); B.tr = 0;
  int r = battle();
  if (r == R_RAN || r == R_LOSE) return;
  if (n->legend == 105) { set_flag(FLAG_SLEEPER); ending(); return; }
  set_flag(n->flag); // the later sleepers: caught, or calmed and still out there to catch
  if (r == R_CAUGHT) set_flag(n->flag + 1);
  blen = 0; cat(SPECIES[n->legend].name);
  cat(r == R_CAUGHT ? (n->legend >= 105 ? " settles into its ball. The wind drops, and all is still." : " settles into its ball. The cave falls quiet.") : " is worn out, but it will not yield. It watches you, waiting."); say(buf);
  if (n->alt) say_lines(n->alt);
}

// The end of the LEAGUE: MAYA runs in after AURIC falls for one last battle, then the HALL OF FAME and the credits.
static void maya_arrives(void) {
  int i = 0; while (i < mi->nnpcs && mi->npcs[i].look != LOOK_MAYA) i++;
  if (i == mi->nnpcs) return;
  static const char *const L[] = { "???: JOSH! WAIT!", 0 }; say_lines(L);
  music(SONG_SPOTTED);
  while (ny[i] > me_y + 1) npc_step(i, DIR_UP);
  me_dir = DIR_DOWN;
  static const char *const L2[] = { "MAYA: Phew... I came straight up VICTORY ROAD after you.", "MAYA: You beat the CHAMPION?! ...Of course you did.",
    "MAYA: Then there's one trainer left you haven't beaten for good. Me!", 0 };
  say_lines(L2);
  trainer_talk(i);
}
static void save_game(void);
static void hof_wait(int frames) { for (int t = 0; frames < 0 || t < frames; t++) { next_frame(); if (t > 30 && (pressed(KEY_A) || pressed(KEY_START))) break; } } // frames < 0: until A
static void hall_of_fame(void) {
  static const char *const L[] = { "AURIC: Come, both of you. It is time to record the new CHAMPION.", "AURIC: This is the HALL OF FAME. Every CHAMPION and every partner who stood with them is honored here.", 0 };
  say_lines(L);
  fade(16);
  static const u32 C[] = { 0x101830, 0x283868, 0xf8d040, 0xf8f8f8, 0x8890c8, 0x504020 };
  scene_begin(); scene_cols(C, 6); music(SONG_HOF);
  pen = SCENE_PAL; px(1, 0, 0, 240, 160); px(3, 0, 8, 240, 2); px(3, 0, 150, 240, 2);
  text_s("HALL OF FAME", (240 - measure("HALL OF FAME") * 2) / 2, 18, 3, 6, 2);
  pen = TEXT_PAL; fade(0); pen = SCENE_PAL;
  for (int k = 0; k < S.nparty; k++) {
    const Mon *m = &S.party[k];
    load_art(0, 0, ART_MON(m->id, 0), 0); scene_sprite(0, 88, 44, 0, 0); scene_n = 1;
    px(1, 0, 112, 240, 30);
    blen = 0; cat(mon_name(m)); cat("   Lv"); cat_num(m->level);
    text(buf, (240 - measure(buf)) / 2, 116, 4, 2);
    blen = 0; cat("No. "); if (m->id < 100) cat("0"); if (m->id < 10) cat("0"); cat_num(m->id); text(buf, (240 - measure(buf)) / 2, 130, 5, 2);
    pen = TEXT_PAL; hof_wait(150); pen = SCENE_PAL;
  }
  scene_n = 0; px(1, 0, 40, 240, 108);
  blen = 0; cat("CHAMPION "); cat(PLAYER_NAME);
  text_s(buf, (240 - measure(buf) * 2) / 2, 70, 3, 6, 2);
  text("Welcome to the HALL OF FAME!", (240 - measure("Welcome to the HALL OF FAME!")) / 2, 100, 4, 2);
  pen = TEXT_PAL; hof_wait(180); pen = SCENE_PAL;
  static const char *const CRED[] = { "JOSHEMON", "A monster-collecting adventure", "WORLD, STORY AND CREATURES", "JOSH and CLAUDE", "MUSIC", "JOSH and CLAUDE",
    "BATTLE RULES", "from the pokeruby decompilation", "FONT", "Adafruit GFX", "THANK YOU", "FOR PLAYING!", 0 };
  for (int k = 0; CRED[k]; k += 2) { // a heading and a line, one page at a time
    px(1, 0, 40, 240, 108);
    text(CRED[k], (240 - measure(CRED[k])) / 2, 74, 3, 2); text(CRED[k + 1], (240 - measure(CRED[k + 1])) / 2, 92, 4, 2);
    pen = TEXT_PAL; for (int t = 0; t < 150; t++) next_frame(); pen = SCENE_PAL;
  }
  px(1, 0, 40, 240, 108);
  text_s("THE END", (240 - measure("THE END") * 2) / 2, 80, 3, 6, 2);
  pen = TEXT_PAL; hof_wait(-1); fade(16);
  scene_end();
  set_flag(FLAG_CHAMPION); heal_all();
  S.heal[0] = HOME.map; S.heal[1] = HOME.x; S.heal[2] = HOME.y; S.heal[3] = HOME.dir;
  static const Warp home = { 0, 0, MAP_CEDAR, 5, 14, DIR_DOWN };
  surf_clear(); warp(&home);
  save_game();
  static const char *const L3[] = { "The game was saved. A new journey begins...", 0 }; say_lines(L3);
}

// After the HALL OF FAME, PROF. CEDAR hands over the two starters you did not pick (so all 105 can be had), and the DEX
// DIPLOMA once every one of the 105 is caught. The late legends (106 on) are a bonus and do not count.
static void give_mon(int id, int lv) {
  Mon m; make_mon(&m, id, lv); mark(S.seen, id); mark(S.caught, id);
  blen = 0; cat(PLAYER_NAME); cat(" received "); cat(mon_name(&m)); cat("!"); sfx(SFX_BUY); say(buf);
  if (S.nparty < 6) S.party[S.nparty++] = m;
  else if (S.nbox < 30) { S.box[S.nbox++] = m; blen = 0; cat(mon_name(&m)); cat(" was sent to the BOX."); say(buf); }
}
#include "story_2.inc"
