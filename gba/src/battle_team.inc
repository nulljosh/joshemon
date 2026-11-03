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

