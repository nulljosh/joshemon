static void render(void) {
  if (scene_open) { render_scene(); return; }
  if (team_open) { render_team(); return; }
  if (evo_open) { render_evolve(); return; }
  if (in_battle) { render_battle(); return; }
  if (!mi) { for (int i = 0; i < 128; i++) oam[i].a0 = 1 << 9; return; } // title screen: no map yet
  int ox = me_x * 16 + DX[me_dir] * off, oy = me_y * 16 + DY[me_dir] * off;
  camx = fit(ox, mi->w * 16, 240) + MAP_PADX * 16 - shake_x;
  camy = fit(oy, mi->h * 16, 160) + MAP_PADY * 16;
  int bx = MAP_PADX * 16 - camx, by = MAP_PADY * 16 - camy;
  Thing th[40]; int n = 0;
  for (int y = (camy >> 4) - MAP_PADY - 1; y <= (camy >> 4) - MAP_PADY + 11; y++) for (int x = (camx >> 4) - MAP_PADX - 1; x <= (camx >> 4) - MAP_PADX + 16; x++) { // obstacles in view
    char c = tile_at(x, y); if (n < 30 && x >= 0 && y >= 0 && x < mi->w && y < mi->h && (c == 'K' || c == 'Y' || c == 'O')) th[n++] = (Thing){ y * 16, bx + x * 16, by + y * 16, LOOK_OBSK + (c == 'Y') + (c == 'O') * 2, 0, 0 };
  }
  for (int i = 0; i < mi->nnpcs; i++) {
    const Npc *p = &mi->npcs[i];
    if (!visible(i) || (p->flags & NF_NOSPRITE)) continue;
    int wx = nx[i] * 16 + DX[ndir[i]] * noff[i], wy = ny[i] * 16 + DY[ndir[i]] * noff[i];
    th[n++] = (Thing){ wy, bx + wx, by + wy, p->look, ndir[i], p->look == LOOK_PC ? (frame >> 4) & 1 : noff[i] ? (noff[i] >> 3) & 1 : 0 };
  }
  if (tile_at(me_x, me_y) == ',' && !moving) th[n++] = (Thing){ oy + 1, bx + ox, by + oy, LOOK_GRASS, 0, 0 }; // over your feet
  static const s8 BOB12[8] = { 0, 1, 1, 1, 0, -1, -1, -1 }; // round(sin(frame / 12)), riding the water
  int bob = S.surf ? BOB12[(frame / 10) & 7] : 0;
  th[n++] = (Thing){ oy, bx + ox, by + oy - (hop ? HOP[off] : 0) + bob, LOOK_PLAYER, me_dir, moving && !S.surf && ((walk >> 3) & 1) };
  if (S.surf) th[n++] = (Thing){ oy, bx + ox, by + oy + bob, LOOK_SURF, me_dir, 0 };
  if (S.bike) th[n++] = (Thing){ oy, bx + ox, by + oy - (hop ? HOP[off] : 0), LOOK_BIKE, me_dir, (walk >> 1) & 1 };
  for (int i = 1; i < n; i++) for (int j = i; j > 0 && th[j].y >= th[j - 1].y; j--) { Thing t = th[j]; th[j] = th[j - 1]; th[j - 1] = t; }
  nobj = 0;
  for (int i = 0; i < n; i++) {
    int d = th[i].dir, tile = slot_of[th[i].look] * SLOT_TILES + ((d == DIR_RIGHT ? DIR_LEFT : d) * 2 + th[i].step) * 8;
    put(th[i].sx, th[i].sy - 16, 2 << 14, (2 << 14) | (d == DIR_RIGHT ? 1 << 12 : 0), tile | (1 << 10) | (slot_of[th[i].look] << 12));
  }
  if (!fade_level) for (int i = 0; i < n; i++) if (th[i].look < LOOK_BALL) put(th[i].sx, th[i].sy + 8 + (th[i].look == LOOK_PLAYER && hop ? HOP[off] : 0), (1 << 14) | (1 << 10), 0, SHADOW_TILE | (1 << 10) | (SHADOW_PAL << 12));
  if (frame - rustle_t < 18) { // game.js ui.rustle: four bits of grass fly up and fall
    static const s8 DXR[4] = { 2, 6, 10, 13 }, SXR[4] = { -2, -1, 1, 2 }; int k = frame - rustle_t;
    for (int i = 0; i < 4; i++) put(bx + rustle_x * 16 + DXR[i] + SXR[i] * k * 3 / 10, by + rustle_y * 16 + 4 - k / 2 + k * k / 36, 0, 0, 900 | (1 << 10) | (13 << 12));
  }
  for (int i = 0; i < mi->nnpcs && legend_on; i++) if (mi->npcs[i].legend && visible(i)) { // the sleeper, bobbing
    static const s8 BOB[8] = { 0, 1, 2, 1, 0, -1, -2, -1 };
    put(bx + nx[i] * 16 - 24, by + ny[i] * 16 - 40 + BOB[(frame / 16) & 7], 0, 3 << 14, 768 | (1 << 10) | (14 << 12));
  }
  for (int i = nobj; i < 128; i++) oam[i].a0 = 1 << 9;
}
static u32 lag_mark; // the VBlank count at the last present(); loops that wait on vsync() alone move it (lag_rebase)
static void lag_rebase(void) { lag_mark = vblanks; }
static void present(void) {
  { u32 now = vblanks, n = now - lag_mark; if (lag_mark && n > 1) QA.lag = QA.lag + n - 1 > 255 ? 255 : QA.lag + n - 1; lag_mark = now; } // QA: frames missed, read mid-frame before the wait so the VBlank interrupt has settled
  vsync();
  copy32((volatile u32 *)OAM, (const u32 *)oam, 256);
  if (!in_battle) { REG_BG1HOFS = REG_BG2HOFS = camx; REG_BG1VOFS = REG_BG2VOFS = camy; }
  REG_BLDY = fade_level;
  if (wipe) { REG_DISPCNT |= 1 << 13; REG_WIN0H = 240; REG_WIN0V = (wipe * 80 / 20) << 8 | (160 - wipe * 80 / 20); } else REG_DISPCNT &= ~(1 << 13);
  if (popup >= 0) { // slides down, waits, slides back up (game.js ui.mapName)
    int t = popup++, y = t < 12 ? -26 + t * 2 : t > 130 ? -2 - (t - 130) * 2 : -2;
    REG_BG0VOFS = -2 - y;
    if (t > 145) { popup = -1; surf_clear(); REG_BG0VOFS = 0; }
  }
  if (mi && !in_battle) { stream(); animate(); }
  QA.battle = in_battle; QA.song = song; QA.map = cur; QA.x = me_x; QA.y = me_y; QA.dir = me_dir;
  frame++;
}
static void next_frame(void) { render(); present(); read_keys(); }
static void fade(int to) { int from = fade_level; for (int i = 1; i <= 12; i++) { fade_level = from + (to - from) * i / 12; next_frame(); } }

static void kill_popup(void) { if (popup >= 0) { popup = -1; surf_clear(); REG_BG0VOFS = 0; } }

// ---------- dialog (game.js say): bottom box, two lines word-wrapped to 216px, letters at 0.75 a frame,
// A or B shows the rest, then waits for A or B with the red arrow blinking.
// One page of a line: returns where the next page starts (Sapphire keeps going when text runs past two rows).
static int wrap_w = 216; // line width; the battle prompt next to its menu wraps at 104 (game.js)
static int say_auto; // frames before battle text moves on by itself (game.js bsay: 60), 0 waits for A
static void bbox(void) { box_c(0, 112, 240, 48, C_BFILL, C_BEDGE, C_BINNER); } // the battle's dark text box
static const char *say_page(const char *s, int instant) {
  QA.talking = 1;
  if (in_battle) bbox(); else box(2, 112, 236, 46);
  int ink = in_battle ? C_PAPER : C_INK, shadow = in_battle ? C_BSHADOW : C_SHADOW, paper = in_battle ? C_BFILL : C_PAPER, waited = 0;
  const char *line[2] = { s, 0 }, *stop = s;
  { // wrap: break before the word that would pass 216px
    const char *p = s, *start = s; int li = 0;
    while (*p) {
      const char *w = p; while (*w == ' ') w++;
      const char *e = w; while (*e && *e != ' ') e++;
      int width = 0; for (const char *q = start; q < e; q++) width += FONT_W[glyph((u8)*q)];
      if (width > wrap_w && p > start) { if (++li == 2) break; line[li] = start = w; }
      p = e;
    }
    stop = p;
  }
  const char *end0 = line[1] ? line[1] : stop; while (end0 > s && end0[-1] == ' ') end0--;
  int len0 = end0 - s, len1 = line[1] ? stop - line[1] : 0, total = len0 + len1, shown = 0, acc = 0, x = 12;
  for (;;) {
    int target = shown;
    if (instant) target = total;
    else if (pressed(KEY_A) || pressed(KEY_B)) { if (shown == total) break; target = total; }
    else { static const u8 SPD[3] = { 8, 15, 40 }; acc += SPD[S.speed]; target = shown + acc / 20; acc %= 20; if (target > total) target = total; }
    for (; shown < target; shown++) {
      int li = shown >= len0; char one[2] = { li ? line[1][shown - len0] : s[shown], 0 };
      if (shown == len0) x = 12;
      text(one, x, 122 + li * 16, ink, shadow); x += FONT_W[glyph((u8)one[0])];
    }
    if (instant) break;
    if (shown == total && say_auto && ++waited >= say_auto) break;
    if (shown == total && !say_auto) { int on = (frame >> 4) & 1; for (int i = 0; i < 4; i++) px(on ? C_RED : paper, 222 + i, 144 + i, 7 - i * 2, 1); }
    next_frame();
  }
  while (*stop == ' ') stop++;
  QA.talking = 0;
  return stop;
}
static void say(const char *s) { kill_popup(); do s = say_page(s, 0); while (*s); }
static void bsay(const char *s) { say_auto = 60; say(s); say_auto = 0; }
static void say_now_box(const char *s) { say_page(s, 1); }
static void say_now(const char *s) { kill_popup(); if (in_battle) wrap_w = 104; say_page(s, 1); wrap_w = 216; } // drawn at once, no wait (the prompt under a menu)
static void say_lines(const char *const *l) { for (; l && *l; l++) say(*l); }
// game.js choose: a box of labels, arrow on the pick, A picks, B backs out (-1) when allowed
static void arrow(int c, int x, int y) { for (int i = 0; i < 4; i++) px(c, x + i, y + i, 1, 7 - i * 2); }
static void hud_again(void); // a battle menu that covered a status box puts it back
// cols > 1 lays labels out in a grid cw apart (the battle menus); on_move hears every new pick
static int choose_grid(const char *const *labels, int n, int bx, int by, int bw, int bh, int padx, int pady, int cols, int cw, int cancel, void (*on_move)(int)) {
  kill_popup();
  box(bx, by, bw, bh);
  for (int i = 0; i < n; i++) text(labels[i], bx + padx + (i % cols) * cw, by + pady + (i / cols) * 16, C_INK, C_SHADOW);
  int sel = 0;
  if (on_move) on_move(0);
  for (;;) {
    arrow(C_INK, bx + padx - 9 + (sel % cols) * cw, by + pady + (sel / cols) * 16);
    next_frame();
    int to = sel;
    if (pressed(KEY_UP) && sel - cols >= 0) to -= cols;
    if (pressed(KEY_DOWN) && sel + cols < n) to += cols;
    if (pressed(KEY_LEFT) && cols > 1 && sel % cols > 0) to--;
    if (pressed(KEY_RIGHT) && cols > 1 && sel % cols < cols - 1 && sel + 1 < n) to++;
    if (pressed(KEY_A) || pressed(KEY_START)) { sfx(SFX_SELECT); px(0, bx, by, bw, bh); hud_again(); return sel; }
    if (pressed(KEY_B) && cancel) { sfx(SFX_BACK); px(0, bx, by, bw, bh); hud_again(); return -1; }
    if (to != sel) { sfx(SFX_CURSOR); arrow(C_PAPER, bx + padx - 9 + (sel % cols) * cw, by + pady + (sel / cols) * 16); sel = to; if (on_move) on_move(sel); }
  }
}
static int choose(const char *const *labels, int n, int bx, int by, int bw, int bh, int padx, int pady, int cancel) { return choose_grid(labels, n, bx, by, bw, bh, padx, pady, 1, 0, cancel, 0); }

static void warp(const Warp *w);
#include "battle.inc"
#include "story.inc"
#include "menus.inc"


static void warp(const Warp *w) {
  sfx(SFX_DOOR); fade(16);
  int changed = w->map != cur;
  popup = -1; surf_clear(); REG_BG0VOFS = 0;
  int was_cave = mi->flags & MF_CAVE, from = cur, fx_ = me_x, fy_ = me_y, fd = me_dir ^ 1;
  int riding = S.surf; S.surf = 0;
  if (!(MAP_INFO[w->map].flags & (MF_OUTDOOR | MF_CAVE))) S.bike = 0; // no riding indoors
  load_map(w->map); me_x = w->tx; me_y = w->ty; me_dir = w->dir;
  REG_DISPCNT = world_dispcnt() | (REG_DISPCNT & (1 << 13)); // the darkness layer comes and goes with the map
  if (riding && tile_at(me_x, me_y) == '~') { S.surf = riding; load_look(LOOK_SURF); music(map_song()); }
  for (int i = 0; i < FLY_COUNT; i++) if (FLY[i].map == cur) set_flag(FLY[i].been);
  if ((mi->flags & MF_CAVE) && !was_cave) { S.rope[0] = from + 1; S.rope[1] = fx_; S.rope[2] = fy_; S.rope[3] = fd; }
  fade(0);
  if (changed) show_name();
}
static void arrive(void) {
  for (int i = 0; i < mi->nwarps; i++) if (mi->warps[i].x == me_x && mi->warps[i].y == me_y) { warp(&mi->warps[i]); return; }
  char t = tile_at(me_x, me_y);
  if (t == ',') { rustle_x = me_x; rustle_y = me_y; rustle_t = frame; }
  if (cur == MAP_CEDAR && me_y == 2 && (me_x == 9 || me_x == 10) && !flag(FLAG_STARTER)) { prof_scene(); return; }
  if (t == 'D') { if (cur == MAP_CEDAR) home_rest(); else { say("The door is locked."); surf_clear(); } me_y++; me_dir = DIR_DOWN; return; }
  if (S.surf && t != '~') { S.surf = 0; music(map_song()); }
  if (S.surf && mi->water[0] && S.nparty && (int)(rnd() % 2880) < mi->wrate * 16 && wild_kind(1)) return; // a REPELled roll still lets trainers see you
  if (S.repel && --S.repel == 0) { say("REPEL's effect wore off..."); surf_clear(); return; }
  if (check_trainers()) return;
  // wild creatures: tall grass and marsh outdoors, any cave floor; rate x16 out of 2880 a step (rules.mjs encounterHit)
  if (mi->grass[0] && S.nparty && ((mi->flags & MF_CAVE) ? t == 'q' || t == 'g' || t == 'e' : t == ',' || t == 'j' || t == 'A' || t == 'd') && (QA_WILD_ON || (int)(rnd() % 2880) < mi->rate * 16)) wild();
}
static void world_step(void) {
  for (int i = 0; i < mi->nnpcs; i++) if ((mi->npcs[i].flags & NF_WANDER) && frame % 90 == (nx[i] * 7 + ny[i] * 3) % 90 && visible(i)) ndir[i] = rnd() & 3;
  if (moving) {
    off += speed; walk++;
    if (off >= (hop ? 32 : 16)) { int n = hop ? 2 : 1; me_x += DX[me_dir] * n; me_y += DY[me_dir] * n; off = 0; moving = hop = 0; arrive(); }
    return;
  }
  if (tile_at(me_x, me_y) == 'I' && !S.surf) { // ice: you keep sliding the way you were going until something stops you
    int sx = me_x + DX[me_dir], sy = me_y + DY[me_dir];
    if (!solid(sx, sy) && tile_at(sx, sy) != '~') { moving = 1; speed = 2; walk = 0; return; }
  }
  if (turn > 0) { turn--; return; }
  int k = keys & KEY_UP ? DIR_UP : keys & KEY_DOWN ? DIR_DOWN : keys & KEY_LEFT ? DIR_LEFT : keys & KEY_RIGHT ? DIR_RIGHT : -1;
  if (k < 0) { walk = 0; return; }
  if (me_dir != k) { me_dir = k; turn = 4; return; }
  int nx = me_x + DX[k], ny = me_y + DY[k];
  if (k == DIR_DOWN && tile_at(nx, ny) == 'L' && !solid(nx, ny + 1)) { moving = hop = 1; speed = 2; sfx(SFX_HOP); return; }
  if (tile_at(nx, ny) == 'O' && map_strength && !solid(nx + DX[k], ny + DY[k]) && has(".pqso", tile_at(nx + DX[k], ny + DY[k]))) { // STRENGTH pushes the boulder
    set_tile(nx + DX[k], ny + DY[k], 'O'); set_tile(nx, ny, ground()); sfx(SFX_BUMP); turn = 8; return;
  }
  int blocked = S.surf ? (tile_at(nx, ny) == '~' ? npc_at(nx, ny) >= 0 : solid(nx, ny) || tile_at(nx, ny) == 'L') : solid(nx, ny);
  if (blocked) { if (frame % 20 == 0) sfx(SFX_BUMP); walk++; return; }
  moving = 1; speed = S.bike ? 4 : keys & KEY_B ? 2 : 1;
}

// game.js talk: whatever you face, through a counter
static void talk(void) {
  int x = me_x + DX[me_dir], y = me_y + DY[me_dir];
  if (tile_at(x, y) == 'c') { x += DX[me_dir]; y += DY[me_dir]; }
  int i = npc_at(x, y);
  blen = 0;
  if (i < 0 && has("KYO", tile_at(x, y))) { obstacle(x, y); surf_clear(); return; }
  if (i < 0 && tile_at(x, y) == '~' && !S.surf) { shore(); surf_clear(); return; }
  if (i >= 0 && mi->npcs[i].opens && badge_count() >= 3) { open_tablet(mi->npcs[i].opens - 1); return; }
  if (i >= 0 && mi->npcs[i].legend) { wake_sleeper(i); return; }
  if (i >= 0 && (mi->npcs[i].flags & NF_POSTGAME) && flag(FLAG_CHAMPION)) { ndir[i] = me_dir ^ 1; postgame_prof(); return; }
  if (i >= 0 && (mi->npcs[i].flags & NF_HALL)) { battle_hall(); return; }
  if (i >= 0) {
    const Npc *n = &mi->npcs[i];
    if (n->flags & NF_ITEM) {
      set_flag(n->flag);
      cat(PLAYER_NAME); cat(" found "); if (n->count > 1) { cat_num(n->count); cat(" "); } else cat("a "); cat(ITEM_NAME[n->item]); if (n->count > 1) cat("S"); cat("!");
      sfx(SFX_BUY); say(buf); S.bag[n->item] += n->count;
    } else {
      if (n->look < LOOK_BALL) ndir[i] = me_dir ^ 1; // turn to face you (down<->up, left<->right)
      if (n->flags & NF_GIFT) {
        if (flag(n->flag)) say_lines(n->alt);
        else {
          if ((n->flags & NF_MON) && S.nparty >= 6 && S.nbox >= 30) { say("Your team and BOX are both full. Make some room first!"); surf_clear(); return; }
          say_lines(n->say);
          set_flag(n->flag);
          if (n->flags & NF_MON) { give_mon(n->item, n->count); say_lines(n->alt); surf_clear(); return; }
          S.bag[n->item] += n->count;
          const char *nm = ITEM_NAME[n->item];
          cat(PLAYER_NAME); cat(" received "); if (n->count > 1) { cat_num(n->count); cat(" "); } else cat(has("AEIOU", nm[0]) ? "an " : "a "); cat(nm); if (n->count > 1) cat("S"); cat("!");
          say(buf);
        }
      } else if (n->flags & NF_HEAL) nurse();
      else if (n->flags & NF_SHOP) shop();
      else if (n->flags & NF_PC) pc();
      else if (n->tr) { if (!flag(n->tr->beat)) trainer_talk(i); else say_lines(n->alt); }
      else say_lines(n->flag && flag(n->flag) ? n->alt : n->say);
    }
  } else for (int k = 0; k < mi->nsigns; k++) if (mi->signs[k].x == x && mi->signs[k].y == y) say(mi->signs[k].text);
  surf_clear();
}
static void save_game(void) { S.magic = SAVE_MAGIC; S.map = cur; S.x = me_x; S.y = me_y; S.dir = me_dir; sram_write(); }
static int count_bits(const u8 *bits);
static void start_menu(void) {
  // ponytail: DEX, BAG, MAP, OPTION join as C7 lands
  for (;;) {
    const char *labels[8]; int n = 0, dex = flag(FLAG_STARTER), team = S.nparty > 0;
    if (dex) labels[n++] = "DEX";
    if (team) labels[n++] = "TEAM";
    labels[n++] = "BAG"; labels[n++] = "MAP"; labels[n++] = PLAYER_NAME; labels[n++] = "SAVE"; labels[n++] = "OPTION"; labels[n++] = "EXIT";
    int i = choose(labels, n, 160, 2, 78, 14 + n * 16, 18, 9, 1);
    if (i >= 0 && !dex) i++;
    if (i >= 1 && !team) i++;
    blen = 0;
    if (i == 0) { dex_screen(); continue; }
    if (i == 1) { if (team_screen(0, 1, "Choose a creature.") == -2) break; continue; }
    if (i == 2) { int it = bag_screen(); if (it >= 0 && use_field(it)) break; surf_clear(); continue; }
    if (i == 3) { region_map(0); continue; }
    i -= 4;
    if (i == 0) {
      int nb = 0;
      cat(PLAYER_NAME); cat(". MONEY $"); cat_num(S.money); cat(". DEX: seen "); cat_num(count_bits(S.seen)); cat(", caught "); cat_num(count_bits(S.caught)); cat(". BADGES: ");
      for (int b = 0; b < BADGE_COUNT; b++) if (flag(BADGE_FLAG[b])) { if (nb++) cat(", "); cat(BADGE_NAME[b]); }
      cat(nb ? "." : "none yet.");
      say(buf);
    } else if (i == 1) { save_game(); blen = 0; cat(PLAYER_NAME); cat(" saved the game."); say(buf); }
    else if (i == 2) { surf_clear(); options(); }
    else break;
    surf_clear();
  }
  surf_clear();
}

static void release(void) { do { vsync(); read_keys(); } while (keys); lag_rebase(); } // a press that ended one screen must not start the next

// game.js drawTitle: the painted sea and rock (BG1), a creature on the rock that changes every 128 frames,
// JOSHEMON big in gold with a dark outline, PRESS START blinking.
