// Battles, included into main.c (shares its text surface, menus and frame loop). Port of game.js 955-1480:
// Gen 3 order of checks, damage from rules.c, the Sapphire screen: backdrop on BG1 (8bpp), pads and creatures as
// sprites, HUD boxes on the text surface (palette banks 13 and 14), the dark text box at the bottom.

// ---------- creatures (game.js makeMon, statsOf)
static int ri(int n) { return (int)(rnd() % (u32)n); }
static unsigned rnd16(void) { return rnd() & 0xFFFF; }
static void calc_stats(Mon *m) { const Species *s = &SPECIES[m->id]; for (int i = 0; i < 6; i++) m->stats[i] = calc_stat(s->stats[i], m->ivs[i], 0, m->level, i == 0) * nature_pct(m->nature, i) / 100; }
static void make_mon(Mon *m, int id, int level) {
  memset(m, 0, sizeof *m);
  m->id = id; m->level = level;
  for (int i = 0; i < 6; i++) m->ivs[i] = ri(32);
  m->nature = ri(25);
  const Species *s = &SPECIES[id];
  u16 ids[64]; int n = 0; // unique moves learnt by now, first-seen order, then the last four
  for (int i = 0; i < s->nlearn; i++) {
    int lv = s->learn[i] >> 9, mv = s->learn[i] & 511, dup = 0;
    if (lv > level) continue;
    for (int j = 0; j < n; j++) dup |= ids[j] == mv;
    if (!dup && n < 64) ids[n++] = mv;
  }
  for (int i = n > 4 ? n - 4 : 0, k = 0; i < n; i++, k++) { m->moves[k].id = ids[i]; m->moves[k].pp = MOVES[ids[i]].pp; }
  m->exp = exp_for_level(s->growth, level);
  calc_stats(m); m->hp = m->stats[0];
}
static const char *mon_name(const Mon *m) { return SPECIES[m->id].name; }
static int has_type(const Mon *m, int t) { return SPECIES[m->id].types[0] == t || SPECIES[m->id].types[1] == t; }
static void mark(u8 *bits, int id) { bits[id >> 3] |= 1 << (id & 7); }

// ---------- battle state
enum { ME, FOE };
enum { ST_NONE, ST_PAR, ST_SLP, ST_PSN, ST_TOX, ST_BRN, ST_FRZ };
enum { R_NONE, R_WIN, R_LOSE, R_RAN, R_CAUGHT };
static int hall_mode;
static const char *const ST_NAME[] = { "", "PAR", "SLP", "PSN", "PSN", "BRN", "FRZ" };
static const u32 ST_COL[] = { 0, 0xc8a808, 0x8890a0, 0xa040a0, 0xa040a0, 0xe05830, 0x58a8d8 };
static const char *const STAT_NAME[] = { "", "ATTACK", "DEFENSE", "SP. ATK", "SP. DEF", "SPEED", "accuracy", "evasiveness" };
static const u8 ACC[13][2] = {{33,100},{36,100},{43,100},{50,100},{60,100},{75,100},{1,1},{133,100},{166,100},{2,1},{233,100},{133,50},{3,1}};
static const s8 SINE10[11] = { 0, 3, 6, 8, 10, 10, 10, 8, 6, 3, 0 }; // round(sin(t/10 * pi) * 10), the lunge

typedef struct {
  Mon foe; Mon *m[2]; int mi, scene, tries;
  int shown[2];     // HP on the bars, x16 so the drain can move in fractions
  int exp;          // EXP bar fill, 0..1024
  s8 st[2][8]; u8 conf[2], flinch[2], tox[2], swapped, ffire[2], pend[2], fresh[2];
  u8 hide[2], fy[2]; int lunge_side, lunge_t, pop_side, pop_t;
  int intro;        // 30..0: pads and creatures slide in
  int me_tr;        // 12..0: your back slides off as the ball goes out
  u8 grew;          // party slots that levelled up, for evolution after the battle
  const Trainer *tr; int tlook, ti, show_tr; Mon tparty[6]; // a trainer's team, the one out now, their portrait on screen
} Battle;
static Battle B;
static void fx_obj(void);
static void move_anim(int side, const Move *M, int fixed);
static int fishing; static int wild_kind(int kind);
static int story_battle; // the PROF's rescue: no BAG, no running
static int ball_x, ball_y, ball_on;
static int team_screen(int pick, int cancel, const char *msg);
static int bag_screen(void);
static void summary(const Mon *m);
static int can_learn(const Mon *m, int id), knows(const Mon *m, int id);
static int field_skill(int k, Mon *m), has_skill(int k);
enum { SK_CUT, SK_SMASH, SK_FLY, SK_SURF, SK_FLASH, SK_STRENGTH, SK_COUNT };
static const char *const SKILL_NAME[6];
static int team_able, team_start; // team_start: where the cursor opens (the first creature still standing, after a faint) // a TM's move: the team cards say ABLE / NOT ABLE instead of HP

// OBJ VRAM in battle: foe 0, yours 64, your back portrait 128, pads 256 (5 frames of 32), ball 448.
enum { T_FOE = 0, T_ME = 64, T_TRAINER = 128, T_FTR = 192, T_PADS = 256, T_BALL = 448 };
enum { P_FOE, P_ME, P_TRAINER, P_PADS, P_BALL, P_FTR };
static const u16 *art_pal(int art) { return art >= 1000 ? PORTRAIT_PAL + (art - 1000) * 16 : ART_PAL + art * 16; }
static void load_art(int tile, int pal, int art, int dy) { // dy: shift the picture down, clipped at its own bottom (fainting)
  volatile u32 *t = (volatile u32 *)OBJ_TILES + tile * 8;
  const u32 *src = art >= 1000 ? PORTRAIT_TILES + (art - 1000) * 64 * 8 : ART_TILES + art * 64 * 8;
  for (int ty = 0; ty < 8; ty++) for (int tx = 0; tx < 8; tx++) for (int r = 0; r < 8; r++) {
    int y = ty * 8 + r - dy;
    t[(ty * 8 + tx) * 8 + r] = y < 0 ? 0 : src[((y >> 3) * 8 + tx) * 8 + (y & 7)];
  }
  if (pal >= 0) for (int i = 0; i < 16; i++) PAL_OBJ[pal * 16 + i] = art_pal(art)[i];
}
static void white(int pal, int art, int on) { for (int i = 1; i < 16; i++) PAL_OBJ[pal * 16 + i] = on ? HEX(0xf8f8f8) : art_pal(art)[i]; }
static int art_of(int side) { return ART_MON(B.m[side]->id, side == ME); }

// ---------- HUD (game.js statusBox): Sapphire-shaped box, HP bar, your EXP strip. Colours of banks 13 (foe), 14 (you).
enum { H_OUT = 1, H_CREAM, H_LIP, H_HI, H_TAG, H_HPT, H_HPS, H_FRAME, H_BARBG, H_BAR1, H_BAR2, H_EXP, H_INK, H_INKS, H_ST };
static void hud_pal(int bank) {
  static const u32 cols[] = { 0x384048, 0xf8f8e0, 0xe0d8b0, 0xfffff0, 0x506070, 0xf8b838, 0xa85808, 0x506858, 0x303830, 0x70f8a8, 0x58d080, 0x40c8f8, 0x404040, 0xd0d0c8, 0 };
  for (int i = 0; i < 15; i++) PAL_BG[bank * 16 + 1 + i] = HEX(cols[i]);
}
// The parts that move each frame: just the bar (and your numbers), so a redraw never shows half a box.
static void hud_hp(int side) {
  const Mon *m = B.m[side]; int mine = side == ME, x = mine ? 124 : 6, y = mine ? 70 : 8, w = 112, bank = 13 + mine;
  if (B.hide[side]) return;
  pen = bank;
  int bx = x + 56, by = y + 17, cur = B.shown[side], max = m->stats[0] * 16; // hpBar
  px(H_FRAME, bx - 1, by - 1, 50, 5); px(H_BARBG, bx, by, 48, 3);
  int bw = cur > 0 ? (48 * cur + max - 1) / max : 0; if (cur > 0 && bw < 1) bw = 1;
  static const u32 bar[3][2] = { { 0x70f8a8, 0x58d080 }, { 0xf8e038, 0xc8a808 }, { 0xf85838, 0xa84048 } };
  int k = cur * 2 > max ? 0 : cur * 5 > max ? 1 : 2;
  PAL_BG[bank * 16 + H_BAR1] = HEX(bar[k][0]); PAL_BG[bank * 16 + H_BAR2] = HEX(bar[k][1]);
  px(H_BAR1, bx, by, bw, 2); px(H_BAR2, bx, by + 2, bw, 1);
  if (mine) { px(H_CREAM, x + 50, y + 23, w - 56, 9); blen = 0; cat_num((cur + 15) / 16); cat("/"); cat_num(m->stats[0]); text(buf, rtext_w(buf, x + w - 7), y + 23, H_INK, H_INKS); }
  pen = TEXT_PAL;
}
static void hud_exp(void) { int x = 124, y = 70 + 38; pen = 14; px(H_OUT, x + 37, y - 4, 68, 1); px(H_EXP, x + 37, y - 4, (68 * B.exp + 512) / 1024, 1); pen = TEXT_PAL; }
static void hud(int side) {
  const Mon *m = B.m[side]; int mine = side == ME, x = mine ? 124 : 6, y = mine ? 70 : 8, w = 112, h = mine ? 38 : 28, bank = 13 + mine;
  if (B.hide[side] || B.intro > 18) { pen = bank; px(0, x, y, w, h); pen = TEXT_PAL; return; }
  pen = bank;
  px(H_OUT, x + 2, y, w - 4, h); px(H_OUT, x + 1, y + 1, w - 2, h - 2); px(H_OUT, x, y + 2, w, h - 4);
  px(H_CREAM, x + 2, y + 1, w - 4, h - 2); px(H_CREAM, x + 1, y + 2, w - 2, h - 4);
  px(H_LIP, x + 2, y + h - 4, w - 4, 3); px(H_LIP, mine ? x + 1 : x + w - 4, y + 2, 3, h - 4);
  px(H_HI, x + 3, y + 2, w - 6, 1);
  text(mon_name(m), x + 7, y + 4, H_INK, H_INKS);
  blen = 0; cat("Lv"); cat_num(m->level); text(buf, rtext_w(buf, x + w - 7), y + 4, H_INK, H_INKS);
  if (m->status) { PAL_BG[bank * 16 + H_ST] = HEX(ST_COL[m->status]); px(H_OUT, x + 6, y + 15, 23, 10); px(H_ST, x + 7, y + 16, 21, 8); text(ST_NAME[m->status], x + 8, y + 16, H_HI, 0); }
  px(H_OUT, x + 36, y + 15, 66, 7); px(H_TAG, x + 37, y + 16, 17, 5);
  text("HP", x + 38, y + 14, H_HPT, H_HPS);
  if (mine) px(H_OUT, x + 36, y + h - 5, 70, 3);
  pen = TEXT_PAL;
  hud_hp(side); if (mine) hud_exp();
}

static void hud_again(void) { if (in_battle && !team_open) { hud(FOE); hud(ME); } }

// ---------- drawing (game.js drawBattle)
static int oam_n;
static void obj(int x, int y, int w, int h, u16 a0, u16 a1, u16 a2) {
  if (oam_n >= 128 || x <= -w || x >= 240 || y <= -h || y >= 160) return;
  oam[oam_n].a0 = (y & 255) | a0; oam[oam_n].a1 = (x & 511) | a1; oam[oam_n++].a2 = a2; // .pad holds the affine matrix
}
static int lunge(int side) { return B.lunge_side == side && B.lunge_t <= 10 ? SINE10[B.lunge_t] : 0; }
static void mon_obj(int side, int x, int y, int tile, int pal) {
  if (B.hide[side]) return;
  if (B.pop_side == side) { // grows out of the ball: affine scale about the sprite's centre, feet kept on the ground
    int s = B.pop_t * 256 / 14; if (s < 31) s = 31;
    oam[0].pad = oam[3].pad = 65536 / s; oam[1].pad = oam[2].pad = 0;
    int w = 64 * s / 256;
    obj(x, y + 32 - w / 2, 64, 64, 1 << 8, 3 << 14, tile | (1 << 10) | (pal << 12));
    return;
  }
  obj(x, y, 64, 64, 0, 3 << 14, tile | (1 << 10) | (pal << 12));
}
static void render_battle(void) {
  if (B.m[ME] && B.m[ME]->hp && B.m[ME]->hp * 4 <= B.m[ME]->stats[0] && !B.hide[ME] && frame % 36 == 0 && fx_t < 0) sfx(SFX_LOWHP);
  int slide = B.intro * 140 / 30;
  oam_n = 0;
  if (ball_on) obj(ball_x - 4, ball_y - 21, 16, 32, 2 << 14, 2 << 14, T_BALL | (1 << 10) | (P_BALL << 12));
  mon_obj(ME, 24 - slide + lunge(ME), 48 - lunge(ME) / 2, T_ME, P_ME);
  if (B.me_tr) obj(16 - slide - (12 - B.me_tr) * 90 / 12, 48, 64, 64, 0, 3 << 14, T_TRAINER | (1 << 10) | (P_TRAINER << 12));
  if (B.show_tr) obj(144 + slide, 8, 64, 64, 0, 3 << 14, T_FTR | (1 << 10) | (P_FTR << 12));
  else mon_obj(FOE, 144 + slide - lunge(FOE), 70 - MON_FEET[B.m[FOE]->id - 1] + lunge(FOE) / 2, T_FOE, P_FOE);
  for (int i = 0; i < 2; i++) obj(124 + slide + i * 64, 54, 64, 32, 1 << 14, 3 << 14, (T_PADS + i * 32) | (2 << 10) | (P_PADS << 12));
  for (int i = 0; i < 3; i++) obj(-8 - slide + i * 64, 94, 64, 32, 1 << 14, 3 << 14, (T_PADS + (2 + i) * 32) | (2 << 10) | (P_PADS << 12));
  fx_obj();
  for (int i = oam_n; i < 128; i++) oam[i].a0 = 1 << 9;
}
static void wait(int n) { while (n--) next_frame(); }

// ---------- the moving parts (game.js hit, drain, popIn, faint)
static void drain(int side) {
  const Mon *m = B.m[side]; int to = m->hp * 16, step = m->stats[0] * 16 / 60; if (step < 6) step = 6;
  while (B.shown[side] != to) {
    B.shown[side] = B.shown[side] > to ? (B.shown[side] - step < to ? to : B.shown[side] - step) : (B.shown[side] + step > to ? to : B.shown[side] + step);
    hud_hp(side); next_frame();
  }
}
static int pre(const char *s, const char *p) { while (*p) if (*s++ != *p++) return 0; return 1; }
static void hit(int side, int dmg) {
  for (int i = 0; i < 8; i++) { B.hide[side] = i % 2 == 0; wait(4); }
  B.hide[side] = 0;
#if defined QA_FIELD && !defined QA_REAL
  if (side == ME && B.tr && ((cur >= MAP_E4A && cur <= MAP_CHAMP) || cur == MAP_HALL || cur == MAP_ROUTE12 || cur == MAP_GROTTO)) dmg = 0; // the QA build's lone L80 walks the LEAGUE and the post-game coast in one go
#endif
  Mon *m = B.m[side]; m->hp = m->hp > dmg ? m->hp - dmg : 0;
  drain(side);
}
static void on_entry(int side);
static void pop_in(int side) {
  int tile = side == ME ? T_ME : T_FOE, pal = side == ME ? P_ME : P_FOE;
  load_art(tile, pal, art_of(side), 0);
  sfx(SFX_BALL); B.hide[side] = 0; B.pop_side = side; white(pal, art_of(side), 1);
  for (B.pop_t = 0; B.pop_t <= 14; B.pop_t++) { if (B.pop_t * 100 >= 85 * 14) white(pal, art_of(side), 0); next_frame(); }
  B.pop_side = -1; white(pal, art_of(side), 0); cry(B.m[side]->id, 0);
  hud(side); on_entry(side); if (B.pend[side ^ 1]) on_entry(side ^ 1);
}
static void label(int side) { if (side == FOE) cat(B.tr ? "Foe " : "Wild "); cat(mon_name(B.m[side])); }
static void bsayf(int side, const char *rest) { blen = 0; label(side); cat(rest); bsay(buf); }
static int abil(int side) { return ABILITY[B.m[side]->id]; }
static void ab_say(int side, const char *rest) { blen = 0; label(side); cat("'s "); cat(ABILITY_NAME[abil(side)]); cat(rest); bsay(buf); }
static void on_entry(int side) { // INTIMIDATE and PRESSURE speak once both creatures are out
  int o = side ^ 1; if (!B.m[side]->hp) return;
  B.pend[side] = B.hide[o] || !B.m[o]->hp; if (B.pend[side]) return; // speaks once the other side is out
  if (abil(side) == AB_PRESSURE) ab_say(side, "!");
  if (abil(side) != AB_INTIMIDATE) return;
  if (abil(o) == AB_CLEAR_BODY) { ab_say(o, " prevents stat loss!"); return; }
  if (B.st[o][1] <= 0) return;
  B.st[o][1]--; blen = 0; label(side); cat("'s INTIMIDATE cuts "); label(o); cat("'s ATTACK!"); bsay(buf);
}

static int first_alive(void) { for (int i = 0; i < S.nparty; i++) if (S.party[i].hp) return i; return -1; }
static void new_side(int side) { for (int i = 0; i < 8; i++) B.st[side][i] = 6; B.conf[side] = B.flinch[side] = B.tox[side] = B.ffire[side] = B.pend[side] = 0; B.fresh[side] = 1; }

static void anim_exp(Mon *m, u32 to) {
  int g = SPECIES[m->id].growth; u32 lo = exp_for_level(g, m->level), hi = exp_for_level(g, m->level + 1);
  int target = to >= hi ? 1024 : (int)((to - lo) * 1024 / (hi - lo));
  while (B.exp < target) { B.exp += 20; if (B.exp > target) B.exp = target; hud_exp(); next_frame(); }
  if (target >= 1024) B.exp = 0;
}
static void learn_move(Mon *m, int id) {
  blen = 0;
  for (int i = 0; i < 4; i++) if (!m->moves[i].id) { m->moves[i].id = id; m->moves[i].pp = MOVES[id].pp; cat(mon_name(m)); cat(" learned "); cat(MOVES[id].name); cat("!"); say(buf); return; }
  static const char *const YN[] = { "YES", "NO" };
  const char *N = mon_name(m), *mv = MOVES[id].name;
  for (;;) {
    blen = 0; cat(N); cat(" is trying to learn "); cat(mv); cat("."); say(buf);
    blen = 0; cat("But "); cat(N); cat(" can't learn more than four moves."); say(buf);
    blen = 0; cat("Delete a move to make room for "); cat(mv); cat("?"); say_now(buf);
    if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) == 0) {
      say_now("Which move should be forgotten?");
      const char *names[4]; char cut[4][13];
      for (int i = 0; i < 4; i++) { const char *s = MOVES[m->moves[i].id].name; int k = 0; for (; s[k] && k < 12; k++) cut[i][k] = s[k]; cut[i][k] = 0; names[i] = cut[i]; }
      int i = choose(names, 4, 120, 2, 118, 76, 16, 10, 1);
      if (i >= 0) {
        blen = 0; cat("1, 2, and... ... Poof! "); cat(N); cat(" forgot "); cat(MOVES[m->moves[i].id].name); cat("."); say(buf);
        m->moves[i].id = id; m->moves[i].pp = MOVES[id].pp;
        blen = 0; cat("And... "); cat(N); cat(" learned "); cat(mv); cat("!"); say(buf);
        return;
      }
    }
    blen = 0; cat("Stop learning "); cat(mv); cat("?"); say_now(buf);
    if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) == 0) { blen = 0; cat(N); cat(" did not learn "); cat(mv); cat("."); say(buf); return; }
  }
}
static void give_exp(Mon *m, int amt) {
  const Species *s = &SPECIES[m->id];
  blen = 0; cat(mon_name(m)); cat(" gained "); cat_num(amt); cat(" EXP. Points!"); bsay(buf);
  m->exp += amt;
  while (m->level < 100 && (u32)exp_for_level(s->growth, m->level + 1) <= m->exp) {
    anim_exp(m, exp_for_level(s->growth, m->level + 1));
    B.grew |= 1 << (m - S.party); music(SONG_LEVELUP);
    int old = m->stats[0]; m->level++; calc_stats(m); m->hp += m->stats[0] - old; B.shown[ME] = m->hp * 16; hud(ME);
    blen = 0; cat(mon_name(m)); cat(" grew to LV. "); cat_num(m->level); cat("!"); bsay(buf);
    for (int i = 0; i < s->nlearn; i++) if ((s->learn[i] >> 9) == m->level) {
      int id = s->learn[i] & 511, known = 0;
      for (int k = 0; k < 4; k++) known |= m->moves[k].id == id;
      if (!known) learn_move(m, id);
    }
  }
  anim_exp(m, m->exp);
}
static int faint(int side) {
  cry(B.m[side]->id, 1);
  for (int i = 0; i <= 16; i++) { B.fy[side] = i * 4; load_art(side == ME ? T_ME : T_FOE, -1, art_of(side), B.fy[side]); next_frame(); }
  B.hide[side] = 1; B.fy[side] = 0; hud(side);
  bsayf(side, " fainted!");
  if (side == FOE) {
    int gain = exp_gain(SPECIES[B.m[FOE]->id].exp, B.m[FOE]->level); if (B.tr) gain = gain * 3 / 2;
    give_exp(B.m[ME], gain);
    if (!B.tr || ++B.ti >= B.tr->nparty) return R_WIN;
    B.m[FOE] = &B.tparty[B.ti]; B.shown[FOE] = B.m[FOE]->hp * 16; new_side(FOE); B.hide[FOE] = 1; B.swapped = 1; mark(S.seen, B.m[FOE]->id);
    blen = 0; cat(B.tr->name); cat(" sent out "); cat(mon_name(B.m[FOE])); cat("!"); bsay(buf); pop_in(FOE);
    return R_NONE;
  }
  if (first_alive() < 0) {
    blen = 0; cat(PLAYER_NAME); cat(" is out of usable creatures!"); bsay(buf);
    int lost = S.money / 2; S.money -= lost;
    if (lost) { blen = 0; cat(PLAYER_NAME); cat(" panicked and dropped $"); cat_num(lost); cat("..."); bsay(buf); }
    blen = 0; cat(PLAYER_NAME); cat(" whited out!"); bsay(buf);
    return R_LOSE;
  }
  int i = -1; while (i < 0 || !S.party[i].hp) { team_start = first_alive(); i = team_screen(1, 0, "Choose a creature."); }
  B.mi = i; B.m[ME] = &S.party[i]; B.shown[ME] = B.m[ME]->hp * 16; new_side(ME); B.hide[ME] = 1; B.swapped = 1;
  blen = 0; cat("Go! "); cat(mon_name(B.m[ME])); cat("!"); bsay(buf); pop_in(ME);
  return R_NONE;
}

// ---------- moves (game.js useMove, takeTurn, endOfTurn)
static int stat_of(int side, int i) { return B.m[side]->stats[i]; }
static int mon_speed(int side) { int v = stage_mod(stat_of(side, 5), B.st[side][5]); return B.m[side]->status == ST_PAR ? v / 4 : v; }
static int fixed_damage(int id, const Mon *a, const Mon *d) { // the no-power moves that still hurt
  if (id == 12 || id == 32 || id == 90 || id == 329) return d->hp; // GUILLOTINE, HORN DRILL, FISSURE, SHEER COLD
  switch (id) { case 49: return 20; case 82: return 40; case 69: case 101: return a->level; case 162: return d->hp / 2 > 1 ? d->hp / 2 : 1; }
  return 0;
}
static const u8 ST_OF[] = { 0, ST_PAR, ST_SLP, ST_PSN, ST_BRN, ST_FRZ }; // AIL_* to ST_*
static int can_status(int side, int st) { // types and abilities that keep a status off
  const Mon *m = B.m[side]; int ab = abil(side);
  if (m->status || !m->hp) return 0;
  if (st == ST_PSN || st == ST_TOX) return !has_type(m, TYPE_POISON) && !has_type(m, TYPE_STEEL) && ab != AB_IMMUNITY;
  if (st == ST_BRN) return !has_type(m, TYPE_FIRE) && ab != AB_WATER_VEIL;
  if (st == ST_FRZ) return !has_type(m, TYPE_ICE) && ab != AB_MAGMA_ARMOR;
  if (st == ST_SLP) return ab != AB_INSOMNIA;
  if (st == ST_PAR) return ab != AB_LIMBER;
  return 1;
}
static void inflict(int side, int ail, int mv_id, int loud) {
  Mon *m = B.m[side];
  if (ail == AIL_CONF) {
    if (B.conf[side]) { if (loud) bsayf(side, " is already confused!"); return; }
    if (abil(side) == AB_OWN_TEMPO) { if (loud) ab_say(side, " prevents confusion!"); return; }
    B.conf[side] = 2 + ri(4); bsayf(side, " became confused!"); return;
  }
  if (ail == AIL_OTHER || ail > AIL_FRZ) { if (loud) bsay("But it failed!"); return; } // ponytail: leech seed, trapping, yawn... later
  int st = ST_OF[ail]; if (mv_id == 92 || mv_id == 305) st = ST_TOX; // TOXIC, POISON FANG
  if (!can_status(side, st)) { if (loud) { if (m->status) bsayf(side, m->status == ST_SLP ? " is already asleep!" : " is already affected!"); else { blen = 0; cat("It doesn't affect "); label(side); cat("..."); bsay(buf); } } return; }
  m->status = st; if (st == ST_SLP) m->sleep = 2 + ri(4); if (st == ST_TOX) B.tox[side] = 0;
  hud(side);
  static const char *const TXT[] = { "", " is paralyzed! It may be unable to move!", " fell asleep!", " was poisoned!", " was badly poisoned!", " was burned!", " was frozen solid!" };
  bsayf(side, TXT[st]);
}
#include "battle_2.inc"
#include "battle_3.inc"
