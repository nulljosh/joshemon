// ---------- evolution (game.js evolve): after the battle, dark sky, sparkles circling, the shape flashes
// between old and new faster and faster; hold B to stop it.
static const s8 SIN[256] = {0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,-40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,-81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,-109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,-123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,-127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,-120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,-102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,-68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,-25,-22,-19,-16,-12,-9,-6,-3};
static int evo_t, evo_show, evo_from, evo_to;
static void render_evolve(void) {
  int n = 0;
  for (int i = 0; i < 24; i++) {
    int a = (i * 256 / 24 + evo_t * 256 / 188) & 255, r = 30 + ((evo_t * 2 + i * 13) % 50);
    int x = 120 + SIN[(a + 64) & 255] * r / 127, y = 56 + SIN[a] * r * 7 / 1270;
    oam[n].a0 = (y & 255); oam[n].a1 = (x & 511); oam[n++].a2 = (T_BALL + 8) | ((i % 3 ? 2 : 3) << 12);
  }
  int on_new = evo_show == evo_to, sil = evo_t > 0 && evo_t < 159;
  white(on_new, ART_MON(on_new ? evo_to : evo_from, 0), sil);
  oam[n].a0 = 24; oam[n].a1 = 88 | (3 << 14); oam[n++].a2 = (on_new ? 64 : 0) | (on_new << 12);
  for (int i = n; i < 128; i++) oam[i].a0 = 1 << 9;
}
static void evolve(Mon *m, int to) {
  evo_from = m->id; evo_to = to; evo_show = m->id; evo_t = 0;
  load_art(0, 0, ART_MON(m->id, 0), 0); load_art(64, 1, ART_MON(to, 0), 0);
  volatile u32 *t = (volatile u32 *)OBJ_TILES + (T_BALL + 8) * 8; for (int i = 0; i < 8; i++) t[i] = i < 2 ? 0x11 : 0; // a 2x2 spark
  PAL_OBJ[2 * 16 + 1] = HEX(0xf8f0a0); PAL_OBJ[3 * 16 + 1] = HEX(0xa0d8f8);
  surf_clear(); PAL_BG[0] = HEX(0x101830);
  REG_DISPCNT = MODE0 | BG0_ON | OBJ_ON | OBJ_1D;
  evo_open = 1; music(SONG_EVO);
  blen = 0; cat("What? "); cat(mon_name(m)); cat(" is evolving!"); say_auto = 50; say(buf); say_auto = 0;
  int stopped = 0;
  for (int i = 0; i < 160; i++) {
    evo_t = i; int period = 24 - i / 7; if (period < 2) period = 2;
    evo_show = (i / period) % 2 ? to : m->id;
    if (keys & KEY_B) { stopped = 1; break; }
    next_frame();
  }
  evo_show = stopped ? m->id : to; evo_t = 0;
  if (stopped) { blen = 0; cat("Huh? "); cat(mon_name(m)); cat(" stopped evolving!"); say(buf); }
  else {
    char old[16]; { const char *s = mon_name(m); int k = 0; for (; s[k] && k < 15; k++) old[k] = s[k]; old[k] = 0; }
    int old_max = m->stats[0]; m->id = to; calc_stats(m); m->hp += m->stats[0] - old_max;
    mark(S.seen, to); mark(S.caught, to); music(SONG_CAUGHT);
    blen = 0; cat("Congratulations! Your "); cat(old); cat(" evolved into "); cat(mon_name(m)); cat("!"); say(buf);
    const Species *s = &SPECIES[m->id];
    for (int i = 0; i < s->nlearn; i++) if ((s->learn[i] >> 9) == m->level) {
      int id = s->learn[i] & 511, known = 0; for (int k = 0; k < 4; k++) known |= m->moves[k].id == id;
      if (!known) learn_move(m, id);
    }
  }
  evo_open = 0; surf_clear();
}


// ---------- move animations (game.js moveAnim, drawFx): one look per type, all particles, each an 8x8 sprite
static const u32 FX_COL[17][2] = { // by TYPE_COL order (type_col maps dex.json's order onto it)
  { 0xf8f8f8, 0xf8e070 }, { 0xe86040, 0xf8b090 }, { 0xe8f0f8, 0xa8c8f0 }, { 0xa050c0, 0xd090e8 }, { 0xc0a060, 0x806040 }, { 0xa89060, 0x605040 },
  { 0xa8c830, 0xe0f070 }, { 0x705898, 0xa888d0 }, { 0xc0c0d8, 0xf8f8f8 }, { 0xf8a030, 0xf84020 }, { 0x58a8f8, 0xc0e8f8 }, { 0x58c848, 0xa8f070 },
  { 0xf8e040, 0xfff8a0 }, { 0xf878b0, 0xf8c0e0 }, { 0xa8f0f8, 0xf8f8f8 }, { 0x7058f8, 0xa898f8 }, { 0x504058, 0x887098 } };
enum { T_FX = 464 }; // three 8x8 tiles: a 2x2 dot, a 3x3 dot, a 3x2 bolt dash
typedef struct { s16 x, y, vx, vy, g; u8 life, c, s; } Spark; // x, y, v in 1/16 px
static Spark sparks[100]; static int nsparks;
static int fx_x[2] = { 56, 176 }, fx_y[2] = { 84, 44 };
static int type_col(int t);
static void spark(int x16, int y16, int vx, int vy, int g, int life, int c, int sz) { if (nsparks < 100) sparks[nsparks++] = (Spark){ x16, y16, vx, vy, g, life, c, sz }; }
static void fx_obj(void) { // appended after the battle sprites by render_battle
  for (int i = 0; i < nsparks && oam_n < 128; i++) {
    Spark *p = &sparks[i]; int t = p->s == 3 ? 1 : p->s == 4 ? 2 : 0;
    obj(p->x >> 4, p->y >> 4, 8, 8, 0, 0, (T_FX + t) | (0 << 10) | ((8 + p->c) << 12));
  }
}
static void fx_step(void) {
  int k = 0;
  for (int i = 0; i < nsparks; i++) { Spark *p = &sparks[i]; p->x += p->vx; p->y += p->vy; p->vy += p->g; if (--p->life) sparks[k++] = *p; }
  nsparks = k;
}
static void fx_ticks(int n) { while (n--) { fx_step(); next_frame(); } }
#include "battle_fx.inc" // target_fx: what each type looks like where it lands
static void move_anim(int side, const Move *M, int fixed) {
  int ts = M->tgt ? side : side ^ 1, ax = fx_x[side], ay = fx_y[side], tx = fx_x[ts], ty = fx_y[ts], tc = type_col(M->type);
  volatile u32 *t = (volatile u32 *)OBJ_TILES + T_FX * 8;
  for (int r = 0; r < 8; r++) { t[r] = r < 2 ? 0x11 : 0; t[8 + r] = r < 3 ? 0x111 : 0; t[16 + r] = r < 2 ? 0x111 : 0; }
  PAL_OBJ[8 * 16 + 1] = HEX(FX_COL[tc][0]); PAL_OBJ[9 * 16 + 1] = HEX(FX_COL[tc][1]);
  nsparks = 0;
  if (!M->power && !fixed) { // status moves: a shower falling on the foe, or rising off yourself
    int up = M->tgt;
    for (int f = 0; f < 34; f++) { if (f % 2 == 0) spark((tx - 22 + ri(44)) * 16, (ty + (up ? 18 : -22)) * 16, 0, up ? -21 : 21, 0, 22, f % 4 ? 0 : 1, 2); fx_step(); next_frame(); }
  } else {
    int phys = is_physical(M->type), projectile = M->type == TYPE_FIRE || M->type == TYPE_WATER || M->type == TYPE_GRASS || M->type == TYPE_ICE || M->type == TYPE_POISON || M->type == TYPE_BUG || M->type == TYPE_DRAGON || M->type == TYPE_PSYCHIC || M->type == TYPE_GHOST;
    if (phys && !projectile) { B.lunge_side = side; for (B.lunge_t = 0; B.lunge_t <= 10; B.lunge_t++) next_frame(); B.lunge_side = -1; }
    else if (M->type == TYPE_ELECTRIC) { // a bolt that flickers down onto the target
      for (int f = 0; f < 20; f++) {
        nsparks = 0;
        if (f % 6 < 4) { int cx = tx - 6; for (int i = 0; i < 6; i++) { int nx_ = tx + (i % 2 ? 6 : -6); for (int k = 0; k < 6; k++) spark((cx + (nx_ - cx) * k / 6) * 16, (ty - 30 + i * 8 + k) * 16, 0, 0, 0, 2, 0, 4); cx = nx_; } }
        next_frame();
      }
      nsparks = 0;
    } else if (M->type == TYPE_PSYCHIC) { // two rings widening round the target
      for (int f = 0; f < 24; f++) {
        nsparks = 0;
        for (int q = 0; q < 2; q++) { int r = 6 + ((f + q * 6) % 12) * 3; for (int a = 0; a < 40; a += 2) spark((tx + SIN[(a * 256 / 40 + 64) & 255] * r / 127) * 16, (ty + SIN[(a * 256 / 40) & 255] * r * 6 / 1270) * 16, 0, 0, 0, 2, 0, 2); }
        next_frame();
      }
      nsparks = 0;
    } else for (int f = 0; f < 18; f++) { // a trail from you to the target, arcing up
      int k1024 = f * 1024 / 18, arc = SIN[(k1024 * 128 / 1024) & 255] * 14 / 127;
      for (int i = 0; i < 2; i++) spark((ax + (tx - ax) * k1024 / 1024 + ri(9) - 4) * 16, (ay + (ty - ay) * k1024 / 1024 + ri(9) - 4 - arc) * 16, 0, 0, 0, 8, i ? 0 : 1, M->type == TYPE_GRASS ? 3 : 2);
      fx_step(); next_frame();
    }
    target_fx(M->type, tx, ty);
  }
  nsparks = 0;
}

// ---------- a turn (game.js turn)
static void show_pp(int i) {
  const Slot *mv = &B.m[ME]->moves[i]; const Move *M = &MOVES[mv->id];
  box(160, 112, 80, 48);
  text("PP", 168, 124, C_INK, C_SHADOW);
  blen = 0; cat_num(mv->pp); cat("/"); cat_num(M->pp); text(buf, rtext_w(buf, 232), 124, C_INK, C_SHADOW);
  text(TYPE_NAME[M->type], 172, 140, C_INK, C_SHADOW);
}
static int battle_turn(void) {
  Mon *me = B.m[ME];
  blen = 0; cat("What will "); cat(mon_name(me)); cat(" do?"); say_now(buf);
  static const char *const CMD[] = { "FIGHT", "BAG", "TEAM", "RUN" };
  int cmd = choose_grid(CMD, 4, 120, 112, 120, 48, 18, 13, 2, 54, 0, 0);
  bbox();
  static Slot struggle = { 165, 99, 0 };
  Slot *act = 0; int run = 0;
  if (cmd == 0) {
    int any = 0; for (int i = 0; i < 4; i++) any |= me->moves[i].id && me->moves[i].pp;
    if (!any) { bsayf(ME, " has no moves left!"); act = &struggle; }
    else {
      const char *names[4]; int n = 0; char cut[4][13];
      for (int i = 0; i < 4 && me->moves[i].id; i++) { const char *s = MOVES[me->moves[i].id].name; int k = 0; for (; s[k] && k < 12; k++) cut[i][k] = s[k]; cut[i][k] = 0; names[n++] = cut[i]; }
      int i = choose_grid(names, n, 0, 112, 160, 48, 14, 13, 2, 72, 1, show_pp);
      bbox();
      if (i < 0) return R_NONE;
#ifdef QA_REAL
      while (!me->moves[i].pp) i = (i + 1) % n; // the script only mashes A: fall through to the next move with PP, as a player would
#endif
      if (!me->moves[i].pp) { say("There is no PP left for this move!"); return R_NONE; }
      act = &me->moves[i];
    }
  } else if (cmd == 3) run = 1;
  else if (cmd == 1 && story_battle) { bsay("PROF. CEDAR: No time for that! Fight!"); return R_NONE; }
  else if (cmd == 1) {
    int key = bag_screen(); if (key < 0) return R_NONE;
    if (ITEMS[key].ball) { int r = throw_ball(key); if (r) return r; }
    else if (!(ITEMS[key].heal || ITEMS[key].cure || ITEMS[key].revive)) { bsay("That can't be used right now."); return R_NONE; }
    else if (!use_medicine(key)) return R_NONE;
  } else {
    int i = team_screen(1, 1, "Choose a creature.");
    if (i < 0 || i == B.mi) return R_NONE;
    if (B.fx[ME].trap) { bsayf(ME, " can't be switched out!"); return R_NONE; }
    blen = 0; cat(mon_name(me)); cat(", come back!"); bsay(buf); if (abil(ME) == AB_NATURAL_CURE) { me->status = me->sleep = 0; B.tox[ME] = 0; }
    B.hide[ME] = 1; B.mi = i; B.m[ME] = &S.party[i]; B.shown[ME] = B.m[ME]->hp * 16; new_side(ME); hud(ME);
    blen = 0; cat("Go! "); cat(mon_name(B.m[ME])); cat("!"); bsay(buf); pop_in(ME);
  }

  int nf = 0; while (nf < 4 && B.m[FOE]->moves[nf].id) nf++;
  Slot *foe_mv = &B.m[FOE]->moves[ri(nf ? nf : 1)];
  if (run && story_battle) { bsay("There's no running away now!"); return R_NONE; }
  if (run && B.tr) { bsay("No! There's no running from a TRAINER battle!"); return R_NONE; }
  if (run) {
    int a = mon_speed(ME), b = mon_speed(FOE); B.tries++;
    int f = a * 128 / (b ? b : 1) + 30 * B.tries;
    int trapped = abil(ME) != AB_RUN_AWAY && B.fx[ME].trap; // MEAN LOOK; RUN AWAY slips past it
    if (!trapped && (abil(ME) == AB_RUN_AWAY || a > b || f > 255 || ri(256) < f)) { bsay("Got away safely!"); return R_RAN; }
    bsay("Can't escape!");
  }
  int r;
  if (act) {
    int pa = MOVES[act->id].pri, pb = MOVES[foe_mv->id].pri;
    int me_first = pa != pb ? pa > pb : mon_speed(ME) != mon_speed(FOE) ? mon_speed(ME) > mon_speed(FOE) : ri(2) == 0;
    int order[2] = { me_first ? ME : FOE, me_first ? FOE : ME };
    for (int k = 0; k < 2; k++) {
      if ((r = take_turn(order[k], order[k] == ME ? act : foe_mv))) return r;
      if (B.swapped) { B.swapped = 0; break; } // a fresh creature came in; the old one's queued move is void
    }
  } else if ((r = take_turn(FOE, foe_mv))) return r;
  return end_of_turn();
}

// ---------- in and out (game.js battle)
static u16 snowy(u16 c) { // up north the grass scene's greens turn to snow and frosted firs
  int r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31, l = (r + g + b) / 3;
  if (g < r || g < b) return c;
  int k = l / 2; return RGB15(k + 14 > 31 ? 31 : k + 14, k + 15 > 31 ? 31 : k + 15, k + 17 > 31 ? 31 : k + 17);
}
static u16 sandy(u16 c) { // in the desert the grass scene's greens turn to warm sand
  int r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31, l = (r + g + b) / 3;
  if (g < r || g < b) return c;
  int k = l / 2; return RGB15(k + 17 > 31 ? 31 : k + 17, k + 13 > 31 ? 31 : k + 13, k + 6);
}
static void scene_in(void) {
  const Scene *sc = &SCENES[B.scene]; int snow = B.scene == SCENE_GRASS && mi->border == 'F', sand = B.scene == SCENE_GRASS && mi->border == 'a';
  for (int i = 0; i < sc->npal; i++) PAL_BG[i] = snow ? snowy(sc->pal[i]) : sand ? sandy(sc->pal[i]) : sc->pal[i];
  copy32((volatile u32 *)CHARBLOCK(CBB_MAP), sc->tiles, sc->ntiles * 16);
  volatile u16 *m = SCREENBLOCK(SBB_GROUND);
  for (int y = 0; y < 32; y++) for (int x = 0; x < 32; x++) m[y * 32 + x] = y < 14 && x < 30 ? sc->map[y * 30 + x] : 0;
  REG_BG1HOFS = REG_BG1VOFS = 0;
  copy32((volatile u32 *)OBJ_TILES + T_PADS * 8, sc->pad, 5 * 32 * 8);
  for (int i = 0; i < 16; i++) PAL_OBJ[P_PADS * 16 + i] = snow ? snowy(sc->padpal[i]) : sand ? sandy(sc->padpal[i]) : sc->padpal[i];
  load_art(T_TRAINER, P_TRAINER, ART_PLAYER_BACK, 0);
  hud_pal(13); hud_pal(14);
}
static int tr_song(const Trainer *t) { // a leader, MAYA and the ASH CIRCLE each have their own battle music
  if (pre(t->name, "ELITE ")) return SONG_ELITE;
  if (pre(t->name, "CHAMPION ")) return SONG_CHAMPION;
  if (t->badge) return SONG_LEADER;
  if (pre(t->name, "MAYA")) return SONG_RIVAL;
  if (pre(t->name, "ASH ") || pre(t->name, "ADMIN ")) return SONG_ASHBATTLE;
  return SONG_TRAINER;
}
static int battle(void) {
  music(story_battle ? SONG_BATTLE : B.tr ? tr_song(B.tr) : B.foe.id >= 105 ? SONG_LEGEND : SONG_BATTLE); sfx(SFX_WIPE);
  for (wipe = 0; wipe <= 20; wipe++) next_frame();
  kill_popup(); surf_clear();
  B.m[ME] = &S.party[B.mi = first_alive()]; B.m[FOE] = B.tr ? &B.tparty[0] : &B.foe; B.ti = 0; B.show_tr = !!B.tr;
  B.shown[ME] = B.m[ME]->hp * 16; B.shown[FOE] = B.m[FOE]->hp * 16; B.tries = 0; B.grew = 0; B.swapped = 0;
  B.hide[ME] = 1; B.pop_side = B.lunge_side = -1; B.fy[0] = B.fy[1] = 0;
  { const Species *s = &SPECIES[B.m[ME]->id]; u32 lo = exp_for_level(s->growth, B.m[ME]->level), hi = exp_for_level(s->growth, B.m[ME]->level + 1); B.exp = B.m[ME]->level >= 100 ? 0 : (int)((B.m[ME]->exp - lo) * 1024 / (hi - lo)); }
  memset(B.fx, 0, sizeof B.fx); new_side(ME); new_side(FOE);
  B.scene = S.surf || fishing ? SCENE_WATER : (mi->flags & MF_CAVE) ? SCENE_CAVE : (mi->flags & MF_OUTDOOR) ? SCENE_GRASS : SCENE_INDOOR;
  mark(S.seen, B.m[FOE]->id);
  in_battle = 1; B.intro = 30; B.me_tr = 12;
  REG_DISPCNT = MODE0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D | (1 << 13);
  scene_in();
  load_art(T_FOE, P_FOE, art_of(FOE), 0);
  if (B.tr) load_art(T_FTR, P_FTR, ART_PORTRAIT(B.tlook), 0);
  B.hide[FOE] = !!B.tr;
  bbox();
  for (; wipe > 0; wipe -= 2) next_frame();
  wipe = 0;
  for (; B.intro > 0; B.intro--) next_frame();
  if (B.tr) {
    blen = 0; cat(B.tr->name); cat(" would like to battle!"); bsay(buf); B.show_tr = 0;
    blen = 0; cat(B.tr->name); cat(" sent out "); cat(mon_name(B.m[FOE])); cat("!"); bsay(buf); pop_in(FOE);
  } else { hud(FOE); cry(B.foe.id, 0); blen = 0; cat("Wild "); cat(mon_name(&B.foe)); cat(" appeared!"); bsay(buf); }
#if defined QA_FIELD && !defined QA_REAL
  if (B.tr) { B.m[ME]->hp = B.m[ME]->stats[0]; B.m[ME]->status = 0; } // QA scripts test the story, not the dice: a fresh lead for every trainer
#endif
  blen = 0; cat("Go! "); cat(mon_name(B.m[ME])); cat("!"); bsay(buf);
  for (; B.me_tr > 0; B.me_tr--) next_frame();
  if (!B.tr) B.pend[FOE] = 1;
  B.fresh[ME] = B.fresh[FOE] = 0; // leads count their first turn (SPEED BOOST)
  pop_in(ME);
  wturns = 0; if ((weather = cur == MAP_ROUTE13 ? 3 : 0)) bsay("A sandstorm is raging."); // the DUNE ROAD never clears
#ifdef QA_FX
  for (int t = 0; t < 17; t++) { static Move m; m.type = t; m.power = 60; m.tgt = 0; move_anim(ME, &m, 0); for (int w = 0; w < 30; w++) next_frame(); } // every type's look in turn, for a recording
#endif
  int r = R_NONE;
  while (!r) r = battle_turn();
  if (r == R_WIN) music(SONG_WIN);
  if (r == R_WIN && B.tr) {
    B.show_tr = 1; B.hide[FOE] = 1; hud(FOE);
    blen = 0; cat(PLAYER_NAME); cat(" defeated "); cat(B.tr->name); cat("!"); bsay(buf);
    blen = 0; cat(B.tr->name); cat(": "); cat(B.tr->lose); bsay(buf);
    int prize = B.tr->money * B.m[FOE]->level; S.money += prize;
    blen = 0; cat(PLAYER_NAME); cat(" got $"); cat_num(prize); cat(" for winning!"); bsay(buf);
  }
  for (wipe = 0; wipe <= 20; wipe++) next_frame();
  in_battle = 0; surf_clear();
  if (r != R_LOSE) for (int i = 0; i < S.nparty; i++) { // creatures that grew may evolve now
    Mon *m = &S.party[i]; const Species *s = &SPECIES[m->id]; int opts[4], n = 0;
    if (!((B.grew >> i) & 1) || !m->hp) continue;
    for (int k = 0; k < s->nevo && n < 4; k++) if (m->level >= s->evo[k * 2 + 1]) opts[n++] = s->evo[k * 2];
    if (n) { wipe = 0; evolve(m, opts[ri(n)]); for (wipe = 0; wipe <= 20; wipe++) next_frame(); }
  }
  if (r == R_LOSE && !hall_mode) { // back to the last Rest House, team healed (a BATTLE HALL loss just ends the streak)
    for (int i = 0; i < S.nparty; i++) { Mon *m = &S.party[i]; m->hp = m->stats[0]; m->status = 0; for (int k = 0; k < 4; k++) if (m->moves[k].id) m->moves[k].pp = MOVES[m->moves[k].id].pp; }
    load_map(S.heal[0]); me_x = S.heal[1]; me_y = S.heal[2]; me_dir = S.heal[3];
  } else load_map_gfx();
  REG_DISPCNT = world_dispcnt() | (1 << 13);
  music(map_song());
  for (wipe = 20; wipe > 0; wipe--) next_frame();
  return r;
}
// game.js wild: kind 0 tall grass and caves, 1 surfing, 2 a rod. A hooked creature ignores REPEL.
static int wild_kind(int kind) { // returns 1 when a battle happened (REPEL can turn it away)
  const u8 *list = kind == 1 ? mi->water : kind == 2 ? mi->fish : mi->grass;
  int lo = kind == 1 ? mi->wlo : kind == 2 ? mi->flo : mi->lvlo, hi = kind == 1 ? mi->whi : kind == 2 ? mi->fhi : mi->lvhi;
  int id = list[pick_slot(ri(100))], lv = lo + ri(hi - lo + 1);
#ifdef QA_WILD
  id = QA_WILD; // a QA build that meets one kind on every grass step, to see an ability fire
#endif
  if (kind != 2 && S.repel && lv < S.party[0].level) return 0; // Gen 3 REPEL: only weaker wild creatures are kept away
  make_mon(&B.foe, id, lv); B.tr = 0;
  fishing = kind == 2; battle(); fishing = 0;
  return 1;
}
static int wild(void) { return wild_kind(0); }
// game.js trainerTalk's battle: the team for your starter when it is the rival's
static int trainer_battle(const Trainer *t, int look) {
  int var = 0; for (int k = 0; k < 3 && t->nvar > 1; k++) if (STARTERS[k] == S.starter) var = k;
  const u8 *p = t->party + var * t->nparty * 2;
  for (int i = 0; i < t->nparty; i++) make_mon(&B.tparty[i], p[i * 2], p[i * 2 + 1]);
  B.tr = t; B.tlook = look;
  int r = battle(); B.tr = 0;
  return r;
}
