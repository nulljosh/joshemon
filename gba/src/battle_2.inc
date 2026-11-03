static int use_move(int side, Slot *mv) {
  Mon *a = B.m[side], *d = B.m[side ^ 1]; int dside = side ^ 1; const Move *M = &MOVES[mv->id];
#if !defined QA_FIELD || defined QA_REAL
  if (side == ME) mv->pp--;
#endif // the QA build's one L80 runs whole gauntlets (the LEAGUE) on mashed A, so it never runs out
  blen = 0; label(side); cat(" used "); cat(M->name); cat("!"); bsay(buf);
  s8 *st = B.st[side], *dst = B.st[dside];
  if (M->acc != 255 && !M->tgt) {
    int stage = st[6] + 6 - dst[7]; if (stage < 0) stage = 0; if (stage > 12) stage = 12;
    if (ri(100) >= M->acc * ACC[stage][0] / ACC[stage][1]) { sfx(SFX_MISS); bsayf(side, "'s attack missed!"); return R_NONE; }
  }
  move_anim(side, M, !M->power && fixed_damage(mv->id, a, d));
  int did = 0, fixed = !M->power && fixed_damage(mv->id, a, d);
  if (M->power || fixed) {
    did = 1;
    int phys = is_physical(M->type), ai = phys ? 1 : 3, di = phys ? 2 : 4;
    const Species *ds = &SPECIES[d->id];
    int eff[2], ne = 0, immune = 0;
    for (int k = 0; k < 2; k++) if (ds->types[k] != 255) { eff[ne] = TYPE_CHART[M->type][ds->types[k]]; immune |= !eff[ne]; ne++; }
    if (immune) { blen = 0; cat("It doesn't affect "); label(dside); cat("..."); bsay(buf); return R_NONE; }
    int over = eff[0] * (ne > 1 ? eff[1] : 10); // x100: >100 super effective, <100 not very
    int hits = !M->hitsLo ? 1 : M->hitsLo == M->hitsHi ? M->hitsLo : (const u8[]){ 2, 2, 2, 3, 3, 3, 4, 5 }[ri(8)];
    int dealt = 0, n = 0;
    for (; n < hits && d->hp && a->hp; n++) {
      int crit = !fixed && ri(crit_chance(M->crit)) == 0;
      int dmg = fixed ? fixed_damage(mv->id, a, d) : damage(a->level, M->power, M->type, a->stats[ai], d->stats[di], st[ai], dst[di], crit, a->status == ST_BRN, has_type(a, M->type), eff, ne, 100 - ri(16));
      sfx(fixed || over == 100 ? SFX_HIT : over > 100 ? SFX_SUPER : SFX_WEAK);
      int before = d->hp; hit(dside, dmg); dealt += before - d->hp;
      if (crit) bsay("A critical hit!");
    }
    if (hits > 1) { blen = 0; cat("Hit "); cat_num(n); cat(n > 1 ? " times!" : " time!"); bsay(buf); }
    if (!fixed && over > 100) bsay("It's super effective!");
    if (!fixed && over < 100) bsay("It's not very effective...");
    if (M->drain > 0 && dealt) { int g = dealt * M->drain / 100; a->hp += g < 1 ? 1 : g; if (a->hp > a->stats[0]) a->hp = a->stats[0]; drain(side); blen = 0; label(dside); cat(" had its energy drained!"); bsay(buf); }
    if (M->drain < 0 && dealt) { int r = dealt * -M->drain / 100; hit(side, r < 1 ? 1 : r); bsayf(side, " is hit with recoil!"); }
    if (!d->hp) { int r = faint(dside); if (r || a->hp) return r; }
    if (!a->hp) return faint(side);
    if (M->flinch && ri(100) < M->flinch) B.flinch[dside] = 1;
  }
  if (M->cat == CAT_HEAL && M->heal) {
    did = 1;
    if (a->hp >= a->stats[0]) bsayf(side, "'s HP is full!");
    else { a->hp += a->stats[0] * M->heal / 100; if (a->hp > a->stats[0]) a->hp = a->stats[0]; drain(side); bsayf(side, " regained health!"); }
  }
  if (M->ail) {
    did = 1;
    int p = M->power ? (M->ailP ? M->ailP : M->chance) : 100;
    if (d->hp && ri(100) < p) inflict(dside, M->ail, mv->id, !M->power);
  }
  if (M->nsc && (!M->power || ri(100) < (M->statP ? M->statP : M->chance))) {
    did = 1;
    int tgt = M->tgt || M->cat == CAT_RAISE ? side : dside;
    for (int k = 0; k < M->nsc; k++) {
      int i = M->sc[k][0], ch = M->sc[k][1]; s8 *s = B.st[tgt];
      blen = 0; label(tgt); cat("'s "); cat(STAT_NAME[i]);
      if ((ch > 0 && s[i] >= 12) || (ch < 0 && s[i] <= 0)) { cat(ch > 0 ? " won't go higher!" : " won't go lower!"); bsay(buf); continue; }
      s[i] += ch; if (s[i] < 0) s[i] = 0; if (s[i] > 12) s[i] = 12;
      cat(ch > 0 ? (ch > 1 ? " sharply rose!" : " rose!") : ch < -1 ? " harshly fell!" : " fell!"); bsay(buf);
    }
  }
  if (!did) bsay(mv->id == 150 ? "But nothing happened!" : "But it failed!"); // ponytail: weather, trapping, two-turn moves come later
  return R_NONE;
}
static int take_turn(int side, Slot *mv) {
  Mon *a = B.m[side];
  if (!a->hp) return R_NONE;
  if (a->status == ST_SLP) { if (--a->sleep > 0) { bsayf(side, " is fast asleep."); return R_NONE; } a->status = 0; hud(side); bsayf(side, " woke up!"); }
  if (a->status == ST_FRZ) { if (ri(5)) { bsayf(side, " is frozen solid!"); return R_NONE; } a->status = 0; hud(side); bsayf(side, " was defrosted!"); }
  if (B.flinch[side]) { B.flinch[side] = 0; bsayf(side, " flinched!"); return R_NONE; }
  if (B.conf[side]) {
    if (--B.conf[side] == 0) bsayf(side, " snapped out of confusion!");
    else {
      bsayf(side, " is confused!");
      if (ri(2) == 0) {
        bsay("It hurt itself in its confusion!");
        hit(side, base_damage(a->level, 40, TYPE_NORMAL, a->stats[1], a->stats[2], B.st[side][1], B.st[side][2], 0, 0));
        return a->hp ? R_NONE : faint(side);
      }
    }
  }
  if (a->status == ST_PAR && ri(4) == 0) { bsayf(side, " is paralyzed! It can't move!"); return R_NONE; }
  return use_move(side, mv);
}
static int end_of_turn(void) {
  for (int side = 0; side < 2; side++) {
    Mon *m = B.m[side]; B.flinch[side] = 0;
    if (!m->hp || !(m->status == ST_PSN || m->status == ST_TOX || m->status == ST_BRN)) continue;
    int dmg = m->stats[0] * (m->status == ST_TOX ? ++B.tox[side] : 2) / 16; if (dmg < 1) dmg = 1;
    bsayf(side, m->status == ST_BRN ? " is hurt by its burn!" : " is hurt by poison!");
    hit(side, dmg);
    if (!m->hp) { int r = faint(side); if (r) return r; }
  }
  return R_NONE;
}


// ---------- team screen (game.js teamScreen, teamCard, drawTeam): lead card on the left, the rest stacked on the
// right, each card its own palette bank (9..14) so a new pick only swaps colours. Icons are scaled OBJ.
enum { K_OUT = 1, K_FILL, K_INNER, K_INK, K_INKS, K_HPT, K_HPS, K_FRAME, K_BARBG, K_BAR1, K_BAR2, K_ST, K_WHITE };
static int team_sel, team_moving = -1;
static void bar_cols(int bank, int i1, int cur, int max) {
  static const u32 bar[3][2] = { { 0x70f8a8, 0x58d080 }, { 0xf8e038, 0xc8a808 }, { 0xf85838, 0xa84048 } };
  int k = cur * 2 > max ? 0 : cur * 5 > max ? 1 : 2;
  PAL_BG[bank * 16 + i1] = HEX(bar[k][0]); PAL_BG[bank * 16 + i1 + 1] = HEX(bar[k][1]);
}
static void card_pal(int i) {
  const Mon *m = &S.party[i]; int on = i == team_sel || i == team_moving, bank = 9 + i;
  u32 fill = !m->hp ? 0xc86858 : team_moving >= 0 && on ? 0x58c878 : on ? 0xf8a038 : 0x3888c0;
  const u32 cols[] = { 0x203040, fill, on ? 0xf8e8c8 : 0xd8ecf8, 0x404040, 0xd0d0c8, 0xf89830, 0xa85808, 0x506858, 0x303830, 0, 0, ST_COL[m->status], 0xf8f8f8 };
  for (int k = 0; k < 13; k++) PAL_BG[bank * 16 + 1 + k] = HEX(cols[k]);
  bar_cols(bank, K_BAR1, m->hp, m->stats[0]);
}
static void team_card(int i) {
  const Mon *m = &S.party[i]; int big = i == 0, x = big ? 4 : 104, y = big ? 14 : 4 + (i - 1) * 23, w = big ? 96 : 132, h = big ? 70 : 22;
  pen = 9 + i; card_pal(i);
  px(K_OUT, x + 1, y, w - 2, h); px(K_OUT, x, y + 1, w, h - 2);
  px(K_FILL, x + 1, y + 1, w - 2, h - 2);
  px(K_INNER, x + 3, y + 4, w - 6, h - 7);
  int bx = big ? x + 30 : x + 30, by = big ? y + 46 : y + 15, bw = m->hp > 0 ? (48 * m->hp + m->stats[0] - 1) / m->stats[0] : 0;
  blen = 0; cat("Lv"); cat_num(m->level);
  if (big) { text(mon_name(m), x + 6, y + 6, K_INK, K_INKS); text(buf, x + 48, y + 20, K_INK, K_INKS); }
  else { text(mon_name(m), x + 28, y + 3, K_INK, K_INKS); text(buf, rtext_w(buf, x + w - 6), y + 3, K_INK, K_INKS); }
  int cx = big ? x + 48 : x + 84, cy = big ? y + 30 : y + 12;
  if (m->status) { px(K_OUT, cx, cy, 23, 10); px(K_ST, cx + 1, cy + 1, 21, 8); text(ST_NAME[m->status], cx + 2, cy + 1, K_WHITE, 0); }
  if (team_able) { const char *a = can_learn(m, team_able) && !knows(m, team_able) ? "ABLE" : "NOT ABLE"; text(a, rtext_w(a, x + w - 6), big ? y + 46 : y + 13, K_INK, K_INKS); pen = TEXT_PAL; return; }
  if (big) text("HP", x + 8, y + 44, K_HPT, K_HPS);
  px(K_FRAME, bx - 1, by - 1, 50, 5); px(K_BARBG, bx, by, 48, 3); px(K_BAR1, bx, by, bw, 2); px(K_BAR2, bx, by + 2, bw, 1);
  blen = 0; cat_num(m->hp); cat("/"); cat_num(m->stats[0]); text(buf, rtext_w(buf, x + w - 6), big ? y + 56 : y + 12, K_INK, K_INKS);
  pen = TEXT_PAL;
}
static void team_msg(const char *msg) { box(2, 120, 236, 38); text(msg, 12, 133, C_INK, C_SHADOW); }
static void render_team(void) {
  int n = 0;
  oam[0].pad = oam[3].pad = 410; oam[1].pad = oam[2].pad = 0; // 64 -> 40 px
  oam[4].pad = oam[7].pad = 745; oam[5].pad = oam[6].pad = 0; // 64 -> 22 px
  for (int i = 0; i < S.nparty; i++) {
    int big = i == 0, s = big ? 40 : 22, x = (big ? 7 : 107) - (64 - s) / 2, y = (big ? 18 : 4 + (i - 1) * 23) - (64 - s) / 2;
    oam[n].a0 = (y & 255) | (1 << 8); oam[n].a1 = (x & 511) | (3 << 14) | ((big ? 0 : 1) << 9); oam[n].a2 = (512 + i * 64) | (0 << 10) | ((6 + i) << 12); n++;
  }
  for (int i = n; i < 128; i++) oam[i].a0 = 1 << 9;
}
static void team_bg(void) { // teal with dots on an 8px grid, every other row shifted (BG1, two 8bpp tiles, screenblock 28)
  volatile u32 *t = (volatile u32 *)CHARBLOCK(CBB_MAP) + 510 * 16;
  for (int k = 0; k < 2; k++) for (int r = 0; r < 8; r++) for (int w = 0; w < 2; w++) {
    u32 v = 0; for (int b = 0; b < 4; b++) { int x = w * 4 + b, dot = r < 3 && x >= k * 4 && x < k * 4 + 3; v |= (u32)(dot ? 207 : 206) << (b * 8); }
    t[k * 16 + r * 2 + w] = v;
  }
  PAL_BG[206] = HEX(0x1e5868); PAL_BG[207] = HEX(0x24687a);
  volatile u16 *m = SCREENBLOCK(28); for (int i = 0; i < 1024; i++) m[i] = 510 + ((i >> 5) & 1);
}
static void team_draw(const char *msg) {
  surf_clear();
  for (int i = 0; i < S.nparty; i++) team_card(i);
  team_msg(msg);
}
static void restore_after_team(void);
// pick: return the chosen index (battle switch, item target). Otherwise A offers SWITCH. -1: backed out.
static int team_screen(int pick, int cancel, const char *msg) {
  u16 dispcnt = REG_DISPCNT, bg1 = REG_BG1CNT;
  team_open = 1; team_sel = team_start; team_start = 0; team_moving = -1;
  for (int i = 0; i < S.nparty; i++) { load_art(512 + i * 64, 6 + i, ART_MON(S.party[i].id, 0), 0); }
  team_bg(); REG_BG1CNT = BG_CBB(CBB_MAP) | BG_SBB(28) | BG_8BPP | BG_PRIO(3); REG_BG1HOFS = REG_BG1VOFS = 0;
  REG_DISPCNT = MODE0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D;
  team_draw(msg);
  int r = -1;
  for (;;) {
    next_frame();
    int to = team_sel;
    if (pressed(KEY_UP) && team_sel > 0) to--;
    if (pressed(KEY_DOWN) && team_sel < S.nparty - 1) to++;
    if (to != team_sel) { int was = team_sel; team_sel = to; card_pal(was); card_pal(to); continue; }
    if (pressed(KEY_B) && cancel) { if (team_moving >= 0) { int was = team_moving; team_moving = -1; card_pal(was); team_msg(msg); continue; } r = -1; break; }
    if (!pressed(KEY_A)) continue;
    if (pick) { r = team_sel; break; }
    if (team_moving >= 0) { // second press of SWITCH: swap the two
      if (team_moving != team_sel) { Mon t = S.party[team_moving]; S.party[team_moving] = S.party[team_sel]; S.party[team_sel] = t; for (int i = 0; i < S.nparty; i++) load_art(512 + i * 64, 6 + i, ART_MON(S.party[i].id, 0), 0); }
      team_moving = -1; team_draw(msg); continue;
    }
    blen = 0; cat("Do what with "); cat(mon_name(&S.party[team_sel])); cat("?"); team_msg(buf);
    const char *opts[9]; int sk[6], nsk = 0, no = 0;
    for (int k = 0; k < SK_COUNT; k++) if (has_skill(k)) { sk[nsk++] = k; opts[no++] = SKILL_NAME[k]; }
    opts[no++] = "SUMMARY"; opts[no++] = "SWITCH"; opts[no++] = "CANCEL";
    int a = choose(opts, no, 150, 146 - no * 16, 88, 14 + no * 16, 16, 10, 1);
    if (a >= 0 && a < nsk) { // a field skill: back to the world to use it
      team_open = 0; REG_BG1CNT = bg1; REG_DISPCNT = dispcnt; restore_after_team();
      if (field_skill(sk[a], &S.party[team_sel])) return -2;
      u16 d2 = REG_DISPCNT; (void)d2; team_open = 1;
      for (int i = 0; i < S.nparty; i++) load_art(512 + i * 64, 6 + i, ART_MON(S.party[i].id, 0), 0);
      team_bg(); REG_BG1CNT = BG_CBB(CBB_MAP) | BG_SBB(28) | BG_8BPP | BG_PRIO(3); REG_DISPCNT = MODE0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D; team_draw(msg); continue;
    }
    a -= nsk;
    if (a == 0) { summary(&S.party[team_sel]); team_bg(); REG_BG1CNT = BG_CBB(CBB_MAP) | BG_SBB(28) | BG_8BPP | BG_PRIO(3); REG_DISPCNT = MODE0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D; for (int i = 0; i < S.nparty; i++) load_art(512 + i * 64, 6 + i, ART_MON(S.party[i].id, 0), 0); team_draw(msg); }
    else if (a == 1 && S.nparty > 1) { team_moving = team_sel; card_pal(team_sel); team_msg("Move to where?"); }
    else team_draw(msg);
  }
  team_open = 0;
  REG_BG1CNT = bg1; REG_DISPCNT = dispcnt;
  restore_after_team();
  if (r >= 0 && in_battle && pick && !S.party[r].hp) { blen = 0; cat(mon_name(&S.party[r])); cat(" has no energy left to battle!"); say(buf); return -1; }
  return r;
}
static void restore_after_team(void) {
  surf_clear();
  if (!in_battle) { load_map_gfx(); return; } // the icons borrowed OBJ tiles from the map's people
  hud_pal(13); hud_pal(14); bbox(); hud(FOE); hud(ME);
}

// ---------- items (game.js useItem, throwBall)
static const char *const CURED[] = { "", "was cured of paralysis.", "woke up.", "was cured of its poisoning.", "was cured of its poisoning.", "had its burn healed.", "was thawed out." };
static int use_medicine(int key) {
  const Item *it = &ITEMS[key];
  int i = team_screen(1, 1, "Use on which creature?");
  if (i < 0) return 0;
  Mon *m = &S.party[i];
  blen = 0;
  if (it->heal && m->hp && m->hp < m->stats[0]) { int n = m->stats[0] - m->hp; if (n > it->heal && it->heal < 255) n = it->heal; /* 255: MAX POTION, all of it */ m->hp += n; cat(mon_name(m)); cat("'s HP was restored by "); cat_num(n); cat(n > 1 ? " points." : " point."); }
  else if (it->cure && m->hp && (it->cure >> m->status) & 1 && m->status) { int was = m->status; m->status = 0; if (in_battle && m == B.m[ME]) B.tox[ME] = 0; cat(mon_name(m)); cat(" "); cat(CURED[was]); }
  else if (it->revive && !m->hp) { m->hp = m->stats[0] / 2; m->status = 0; cat(mon_name(m)); cat(" recovered from fainting!"); }
  else { (in_battle ? bsay : say)("It won't have any effect."); return 0; }
  S.bag[key]--; sfx(SFX_HEAL);
  if (in_battle && m == B.m[ME]) { drain(ME); hud(ME); }
  (in_battle ? bsay : say)(buf);
  return 1;
}
static int throw_ball(int key) {
  const Item *it = &ITEMS[key]; const char *nm = ITEM_NAME[key];
  S.bag[key]--;
  blen = 0; cat(PLAYER_NAME); cat(has("AEIOU", nm[0]) ? " threw an " : " threw a "); cat(nm); cat("!"); bsay(buf);
  copy32((volatile u32 *)OBJ_TILES + T_BALL * 8, SPRITE_TILES + LOOK_BALL * SLOT_TILES * 8, 8 * 8);
  for (int i = 0; i < 16; i++) PAL_OBJ[P_BALL * 16 + i] = SPRITE_PAL[LOOK_BALL * 16 + i];
  if (B.tr) { bsay("The TRAINER blocked the BALL!"); bsay("Don't be a thief!"); return R_NONE; }
  ball_on = 1; sfx(SFX_BALL);
  static const s8 ARC[25] = { 0,9,17,26,34,41,48,54,59,63,67,69,70,69,67,63,59,54,48,41,34,26,17,9,0 }; // round(sin(i/24*pi)*70)
  for (int i = 0; i <= 24; i++) { ball_x = 40 + i * 136 / 24; ball_y = 90 - ARC[i] - i * 52 / 24; next_frame(); }
  B.hide[FOE] = 1; hud(FOE); wait(10);
  for (int i = 0; i < 12; i++) { ball_y += 4; if (ball_y > 48) ball_y = 48; next_frame(); }
  Mon *f = B.m[FOE];
  int st = f->status == ST_SLP || f->status == ST_FRZ ? 1 : f->status && f->status != ST_TOX ? 2 : 0; // Gen 3 bug kept: badly poisoned gets no bonus
  int shakes = expected_shakes(catch_odds(SPECIES[f->id].catchRate, it->ball, f->stats[0], f->hp, st), rnd16);
  for (int i = 0; i < shakes && i < 3; i++) {
    wait(20); sfx(SFX_SHAKE);
    static const s8 DXS[8] = { -2, -3, -2, 0, 2, 3, 2, 0 };
    int x0 = ball_x; for (int k = 0; k < 8; k++) { ball_x = x0 + DXS[k]; wait(2); } ball_x = x0;
  }
  wait(20);
  if (shakes == 4) {
    music(SONG_CAUGHT);
    blen = 0; cat("Gotcha! "); cat(mon_name(f)); cat(" was caught!"); bsay(buf);
    int is_new = !((S.caught[f->id >> 3] >> (f->id & 7)) & 1); mark(S.caught, f->id);
    if (is_new) { blen = 0; cat(mon_name(f)); cat("'s data was added to the DEX."); bsay(buf); }
    if (S.nparty < 6) S.party[S.nparty++] = *f;
    else { if (S.nbox < 30) S.box[S.nbox++] = *f; blen = 0; cat(mon_name(f)); cat(" was sent to the BOX."); bsay(buf); } // ponytail: 30 in the BOX until the PC pages arrive (C7)
    ball_on = 0;
    return R_CAUGHT;
  }
  ball_on = 0; B.hide[FOE] = 0; hud(FOE); music(B.tr ? SONG_TRAINER : SONG_BATTLE);
  static const char *const MISS[] = { "Oh, no! It broke free!", "Aww! It appeared to be caught!", "Aargh! Almost had it!", "Shoot! It was so close, too!" };
  bsay(MISS[shakes]);
  return R_NONE;
}


// ---------- evolution (game.js evolve): after the battle, dark sky, sparkles circling, the shape flashes
// between old and new faster and faster; hold B to stop it.
static const s8 SIN[256] = {0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,-40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,-81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,-109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,-123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,-127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,-120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,-102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,-68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,-25,-22,-19,-16,-12,-9,-6,-3};
static int evo_t, evo_show, evo_from, evo_to;
