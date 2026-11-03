// Menus and field items (C7): the BAG, using items outside battle, TMs. Included into main.c after story.inc.

// ---------- BAG (game.js bagScreen, drawBag): orange stripes, a satchel whose flap changes colour per pocket,
// the item list on the right, the description underneath. Left and right change pockets.
static const char *const POCKET_NAME[4] = { "ITEMS", "BALLS", "TM CASE", "KEY ITEMS" };
static int bag_pocket, bag_sel, bag_top;
static int bag_keys(int *out) { int n = 0; for (int k = 0; k < ITEM_COUNT; k++) if (ITEMS[k].pocket == bag_pocket && S.bag[k]) out[n++] = k; return n; }
static void bag_satchel(void) {
  static const u32 FLAP[4] = { 0xc87840, 0xd05040, 0x58a058, 0x5878c0 };
  PAL_BG[SCENE_PAL * 16 + 5] = HEX(FLAP[bag_pocket]);
  pen = SCENE_PAL;
  px(1, 0, 90, 98, 30); for (int y = 90; y < 120; y += 6) px(2, 0, y, 98, 2); // clear the pocket name strip
  px(3, 14, 30, 72, 62); px(4, 16, 32, 68, 58); px(5, 18, 34, 64, 22); px(3, 16, 56, 68, 3);
  px(6, 46, 60, 8, 8); px(3, 48, 62, 4, 4); px(3, 30, 18, 4, 14); px(3, 66, 18, 4, 14); px(3, 30, 16, 40, 4);
  const char *name = POCKET_NAME[bag_pocket]; int l = 0; while (name[l]) l++;
  text(name, 50 - l * 3, 98, 7, 8);
  for (int j = 0; j < 4; j++) px(j == bag_pocket ? 7 : 8, 36 + j * 8, 108, 5, 3);
  pen = TEXT_PAL;
}
static void bag_list(void) {
  int keys_[ITEM_COUNT], n = bag_keys(keys_);
  if (bag_sel > n) bag_sel = n;
  if (bag_top > bag_sel) bag_top = bag_sel;
  if (bag_top < bag_sel - 5) bag_top = bag_sel - 5;
  box(98, 4, 140, 104);
  for (int r = 0; r < 6 && bag_top + r <= n; r++) {
    int j = bag_top + r, y = 14 + r * 16;
    text(j == n ? "CANCEL" : ITEMS[keys_[j]].label, 114, y, C_INK, C_SHADOW);
    if (j < n && ITEMS[keys_[j]].kind != 5 && ITEMS[keys_[j]].pocket != 3) { blen = 0; cat("x"); cat_num(S.bag[keys_[j]]); text(buf, rtext_w(buf, 228), y, C_INK, C_SHADOW); }
    if (j == bag_sel) arrow(C_INK, 104, y);
  }
  box(2, 112, 236, 46);
  const char *d = bag_sel < n ? ITEMS[keys_[bag_sel]].desc : "Close the BAG.";
  int ib = in_battle; in_battle = 0; say_now_box(d); in_battle = ib; // the BAG's own white box, even mid-battle
}
static int bag_screen(void) {
  static const u32 C[] = { 0xf0a848, 0xe89838, 0x603818, 0xa86030, 0, 0xf8d048, 0xf8f8f8, 0xa05818, 0xd05040 };
  u16 dispcnt = REG_DISPCNT;
  scene_begin(); scene_cols(C, 9);
  pen = SCENE_PAL; for (int y = 0; y < 160; y += 6) { px(1, 0, y, 240, 6); px(2, 0, y, 240, 2); } pen = TEXT_PAL;
  bag_satchel(); bag_list();
  int out = -1;
  for (;;) {
    next_frame();
    pen = SCENE_PAL; // blinking pocket arrows and scroll marks
    if (frame % 40 < 28) { text("<", 6, 98, 7, 8); text(">", 88, 98, 7, 8); } else { px(1, 6, 98, 8, 10); px(1, 88, 98, 8, 10); }
    pen = TEXT_PAL;
    int keys_[ITEM_COUNT], n = bag_keys(keys_);
    px(bag_top > 0 && frame % 30 < 20 ? C_RED : C_PAPER, 165, 6, 6, 2);
    px(bag_top + 6 <= n && frame % 30 < 20 ? C_RED : C_PAPER, 165, 104, 6, 2);
    if (pressed(KEY_UP) && bag_sel > 0) { bag_sel--; sfx(SFX_CURSOR); bag_list(); }
    else if (pressed(KEY_DOWN) && bag_sel < n) { bag_sel++; sfx(SFX_CURSOR); bag_list(); }
    else if (pressed(KEY_LEFT) || pressed(KEY_RIGHT)) { bag_pocket = (bag_pocket + (pressed(KEY_LEFT) ? 3 : 1)) & 3; bag_sel = bag_top = 0; sfx(SFX_CURSOR); bag_satchel(); bag_list(); }
    else if (pressed(KEY_A)) { sfx(SFX_SELECT); out = bag_sel < n ? keys_[bag_sel] : -1; break; }
    else if (pressed(KEY_B)) { sfx(SFX_BACK); break; }
  }
  scene_end(); REG_DISPCNT = dispcnt;
  if (in_battle) { hud_pal(13); hud_pal(14); bbox(); hud(FOE); hud(ME); }
  return out;
}

// ---------- TMs (game.js teachTM, canLearn): its own move list, a shared type, or any NORMAL move
static int can_learn(const Mon *m, int id) {
  const Species *s = &SPECIES[m->id];
  for (int i = 0; i < s->nlearn; i++) if ((s->learn[i] & 511) == id) return 1;
  return has_type(m, MOVES[id].type) || MOVES[id].type == TYPE_NORMAL;
}
static int knows(const Mon *m, int id) { for (int k = 0; k < 4; k++) if (m->moves[k].id == id) return 1; return 0; }
static void teach_tm(int key) {
  int mv = ITEMS[key].tm; const char *name = MOVES[mv].name;
  static const char *const YN[] = { "YES", "NO" };
  blen = 0; cat("Booted up a TM. It contained "); cat(name); cat("."); say(buf);
  blen = 0; cat("Teach "); cat(name); cat(" to a creature?"); say_now(buf);
  if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) != 0) return;
  team_able = mv;
  int i = team_screen(1, 1, "Teach which creature?"); team_able = 0;
  if (i < 0) return;
  Mon *m = &S.party[i];
  blen = 0; cat(mon_name(m));
  if (knows(m, mv)) { cat(" already knows "); cat(name); cat("."); say(buf); return; }
  if (!can_learn(m, mv)) { cat(" can't learn "); cat(name); cat("."); say(buf); return; }
  learn_move(m, mv);
  if (knows(m, mv)) S.bag[key]--;
}

// ---------- using an item outside battle (game.js useField). Returns 1 when the START menu should close.
static void escape_rope(void);
static int region_map(int fly);
static int ride_bike(void);
static void fish(void);
static int use_field(int key) {
  const Item *it = &ITEMS[key];
  if (it->heal || it->cure || it->revive) { if (S.nparty) use_medicine(key); return 0; }
  if (it->ball) { say("There's nothing to catch here!"); return 0; }
  if (it->tm) { teach_tm(key); return 0; }
  if (it->kind == 1) { S.bag[key]--; S.repel = it->steps; sfx(SFX_HEAL); blen = 0; cat(PLAYER_NAME); cat(" used the "); cat(ITEM_NAME[key]); cat("."); say(buf); return 1; }
  if (it->kind == 2) { if (!(mi->flags & MF_CAVE) || !S.rope[0]) { say("It can't be used here."); return 0; } S.bag[key]--; blen = 0; cat(PLAYER_NAME); cat(" used the "); cat(ITEM_NAME[key]); cat("."); say(buf); escape_rope(); return 1; }
  if (it->kind == 5) { region_map(0); return 0; }
  if (it->kind == 3) return ride_bike();
  if (it->kind == 4) { fish(); return 1; }
  return 0;
}
static void escape_rope(void) { Warp w = { 0, 0, S.rope[0] - 1, S.rope[1], S.rope[2], S.rope[3] }; surf_clear(); warp(&w); }

// ---------- shared bits of the info screens (game.js typeChip, ball)
static const u32 TYPE_COL[17] = { 0xa8a878, 0xc03028, 0xa890f0, 0xa040a0, 0xe0c068, 0xb8a038, 0xa8b820, 0x705898, 0xb8b8d0, 0xf08030, 0x6890f0, 0x78c850, 0xf8d030, 0xf85888, 0x98d8d8, 0x7038f8, 0x705848 };
static int type_col(int t) { // TYPE_NAME order is dex.json's; TYPE_COL above is in the same order as game.js TYPE_COL keys
  static const char *const K[17] = { "NORMAL", "FIGHTING", "FLYING", "POISON", "GROUND", "ROCK", "BUG", "GHOST", "STEEL", "FIRE", "WATER", "GRASS", "ELECTRIC", "PSYCHIC", "ICE", "DRAGON", "DARK" };
  for (int k = 0; k < 17; k++) { const char *a = K[k], *b = TYPE_NAME[t]; while (*a && *a == *b) a++, b++; if (!*a && !*b) return k; }
  return 0;
}
static void type_chip(int x, int y, int t, int idx_out, int idx_col, int idx_white) { // colours are indexes in the current pen's bank
  PAL_BG[pen * 16 + idx_col] = HEX(TYPE_COL[type_col(t)]);
  px(idx_out, x, y, 50, 12); px(idx_col, x + 1, y + 1, 48, 10);
  char nm[7]; int k = 0; for (; TYPE_NAME[t][k] && k < 6; k++) nm[k] = TYPE_NAME[t][k]; nm[k] = 0;
  text(nm, x + 1 + (48 - k * 8) / 2, y + 2, idx_white, idx_out);
}
static void ball_icon(int x, int y) { // game.js ball(): 8x8, in bank 15
  static const char *const R0[8] = { "..####..", ".#rrrr#.", "#rrrrrr#", "#rr##rr#", "####w###", "#ww##ww#", ".#wwww#.", "..####.." };
  for (int j = 0; j < 8; j++) for (int i = 0; i < 8; i++) { char c = R0[j][i]; if (c != '.') px(c == '#' ? C_BALLDK : c == 'r' ? C_BALLRED : C_PAPER, x + i, y + j, 1, 1); }
}
static int owns(const u8 *bits, int id) { return (bits[id >> 3] >> (id & 7)) & 1; }
static int count_bits(const u8 *bits) { int n = 0; for (int id = 1; id <= SPECIES_COUNT; id++) n += owns(bits, id); return n; }
static void pad3(int n) { if (n < 100) cat("0"); if (n < 10) cat("0"); cat_num(n); }

// ---------- DEX (game.js dexScreen, drawDex): red case, the list on the left, a picture on the right, A for the entry
static int dex_i;
static void dex_draw(int page) {
  static const u32 C[] = { 0xd84838, 0xb03028, 0xf8f8f8, 0x702018, 0x404040 };
  surf_clear(); scene_cols(C, 5); scene_n = 0;
  pen = SCENE_PAL; px(1, 0, 0, 240, 160); px(2, 0, 0, 240, 20); text("DEX", 8, 6, 3, 4);
  blen = 0; cat("SEEN "); { int n = count_bits(S.seen); if (n < 100) cat(" "); if (n < 10) cat(" "); cat_num(n); } cat("  OWN "); { int n = count_bits(S.caught); if (n < 100) cat(" "); if (n < 10) cat(" "); cat_num(n); }
  text(buf, 88, 6, 3, 4);
  int id = dex_i + 1; const Species *s = &SPECIES[id];
  if (page) {
    pen = TEXT_PAL; box(4, 24, 232, 132);
    load_art(0, 0, ART_MON(id, 0), 0); scene_sprite(0, 12, 30, 0, 0); scene_n = 1;
    blen = 0; cat("No."); pad3(id); text(buf, 88, 36, C_INK, C_SHADOW); text(s->name, 88, 50, C_INK, C_SHADOW);
    pen = 11; // the text palette again, with the chip's outline and type colours in 11..13, so the box around them stays white
    for (int i = 1; i < 16; i++) PAL_BG[11 * 16 + i] = PAL_BG[TEXT_PAL * 16 + i];
    PAL_BG[11 * 16 + 11] = HEX(0x404040);
    for (int j = 0; j < 2 && s->types[j] != 255; j++) type_chip(88 + j * 54, 64, s->types[j], 11, 12 + j, C_PAPER);
    pen = TEXT_PAL;
    text("HT ?  WT ?", 88, 80, C_GRAY2, 0);
    const char *d = owns(S.caught, id) ? s->dex : "Catch it to read its entry.";
    // up to five lines wrapped at 212px, 11px apart
    for (int line = 0; line < 5 && *d; line++) {
      const char *p = d, *end = d; int w = 0;
      while (*p) { const char *e = p; while (*e == ' ') e++; while (*e && *e != ' ') e++; int ww = 0; for (const char *q = d; q < e; q++) ww += FONT_W[glyph((u8)*q)]; if (ww > 212 && end > d) break; end = e; p = e; w = ww; }
      char l[64]; int k = 0; for (const char *q = d; q < end && k < 63; q++) l[k++] = *q; l[k] = 0;
      text(l, 14, 100 + line * 11, C_INK, C_SHADOW); d = end; while (*d == ' ') d++; (void)w;
    }
    return;
  }
  pen = TEXT_PAL; box(4, 24, 140, 132);
  int top = dex_i - 3; if (top > SPECIES_COUNT - 8) top = SPECIES_COUNT - 8; if (top < 0) top = 0;
  for (int r = 0; r < 8; r++) {
    int j = top + r, y = 32 + r * 15, seen = owns(S.seen, j + 1);
    if (j == dex_i) px(C_HILITE, 8, y - 3, 132, 14);
    if (owns(S.caught, j + 1)) ball_icon(12, y);
    blen = 0; pad3(j + 1); text(buf, 24, y, C_INK, C_SHADOW);
    if (seen) { char nm[11]; int k = 0; for (; SPECIES[j + 1].name[k] && k < 10; k++) nm[k] = SPECIES[j + 1].name[k]; nm[k] = 0; text(nm, 56, y, C_INK, C_SHADOW); }
    else text("----------", 56, y, C_GRAY, 0);
  }
  box_c(148, 24, 88, 96, C_PAPER, C_EDGE, C_INNER);
  if (owns(S.seen, id)) { load_art(0, 0, ART_MON(id, 0), 0); scene_sprite(0, 160, 40, 0, 0); scene_n = 1; }
  else text_s("?", 188, 68, C_GRAY, 0, 2);
  pen = SCENE_PAL;
  if (owns(S.seen, id)) text("A: ENTRY", 156, 128, 3, 4);
  text("B: BACK", 156, 142, 3, 4);
  pen = TEXT_PAL;
}
static void dex_screen(void) {
  u16 dispcnt = REG_DISPCNT;
  scene_begin(); dex_draw(0);
  for (;;) {
    next_frame();
    int old = dex_i;
    if (pressed(KEY_B)) { sfx(SFX_BACK); break; }
    if (pressed(KEY_UP) && dex_i > 0) dex_i--;
    if (pressed(KEY_DOWN) && dex_i < SPECIES_COUNT - 1) dex_i++;
    if (pressed(KEY_LEFT)) dex_i = dex_i > 8 ? dex_i - 8 : 0;
    if (pressed(KEY_RIGHT)) dex_i = dex_i + 8 < SPECIES_COUNT - 1 ? dex_i + 8 : SPECIES_COUNT - 1;
    if (old != dex_i) { sfx(SFX_CURSOR); dex_draw(0); }
    if ((pressed(KEY_A) || pressed(KEY_START)) && owns(S.seen, dex_i + 1)) {
      sfx(SFX_SELECT); dex_draw(1);
      do next_frame(); while (!(pressed(KEY_A) || pressed(KEY_B) || pressed(KEY_START)));
      sfx(SFX_BACK); dex_draw(0);
    }
  }
  scene_end(); REG_DISPCNT = dispcnt;
  if (!in_battle) load_map_gfx();
}

// ---------- SUMMARY (game.js summary, drawSummary): everything on one page
static void summary(const Mon *m) {
  u16 dispcnt = REG_DISPCNT; const Species *s = &SPECIES[m->id];
  static const u32 C[] = { 0xf0e8d8, 0x3888c0, 0xf8f8f8, 0x204060, 0xd8e8f0, 0xa8c8d8, 0x404040, 0xd0d0c8, 0x606060, 0x404040, 0, 0, 0, 0xd84838 };
  scene_begin(); scene_cols(C, 14); cry(m->id, 0);
  pen = SCENE_PAL;
  px(1, 0, 0, 240, 160); px(2, 0, 0, 240, 16); text("CREATURE INFO", 8, 4, 3, 4);
  px(5, 4, 20, 76, 76); px(6, 4, 80, 76, 16);
  load_art(0, 0, ART_MON(m->id, 0), 0); scene_sprite(0, 10, 92 - MON_FEET[m->id - 1] - 4, 0, 0); scene_n = 1;
  text(mon_name(m), 86, 22, 7, 8); blen = 0; cat("Lv"); cat_num(m->level); text(buf, 86, 34, 7, 8);
  for (int j = 0; j < 2 && s->types[j] != 255; j++) type_chip(86 + j * 52, 46, s->types[j], 10, 11 + j, 3);
  if (m->status) { PAL_BG[SCENE_PAL * 16 + 13] = HEX(ST_COL[m->status]); px(10, 190, 34, 23, 10); px(13, 191, 35, 21, 8); text(ST_NAME[m->status], 192, 35, 3, 0); }
  int to_next = m->level >= 100 ? 0 : exp_for_level(s->growth, m->level + 1) - (int)m->exp;
  text("EXP NEXT", 86, 62, 9, 0); blen = 0; cat_num(to_next); text(buf, rtext_w(buf, 236), 62, 7, 8);
  blen = 0; cat(NATURE_NAME[m->nature % 25]); cat(" nature."); text(buf, 86, 76, 7, 8); // the raised stat's name shows red, the lowered one blue
  static const char *const N[6] = { "HP", "ATTACK", "DEFENSE", "SP. ATK", "SP. DEF", "SPEED" };
  for (int j = 0; j < 6; j++) {
    int y = 100 + (j % 3) * 12, x = j < 3 ? 6 : 122;
    int pct = nature_pct(m->nature % 25, j); text(N[j], x, y, pct > 100 ? 14 : pct < 100 ? 2 : 9, 0);
    blen = 0; if (!j) { cat_num(m->hp); cat("/"); } cat_num(m->stats[j]); text(buf, rtext_w(buf, x + 110), y, 7, 8);
  }
  pen = 11; // the move rows: their own bank for four type colours
  { static const u32 K[] = { 0xf0e8d8, 0xc8bca8, 0x404040, 0xd0d0c8, 0x606060 }; for (int i = 0; i < 5; i++) PAL_BG[11 * 16 + 1 + i] = HEX(K[i]); }
  px(1, 0, 136, 240, 24); px(2, 4, 136, 232, 1);
  for (int j = 0; j < 4 && m->moves[j].id; j++) {
    const Move *M = &MOVES[m->moves[j].id]; int x = j % 2 ? 122 : 6, y = 140 + (j / 2) * 10;
    PAL_BG[11 * 16 + 6 + j] = HEX(TYPE_COL[type_col(M->type)]);
    px(6 + j, x, y + 1, 3, 7);
    char nm[12]; int k = 0; for (; M->name[k] && k < 11; k++) nm[k] = M->name[k]; nm[k] = 0;
    text(nm, x + 6, y, 3, 4);
    blen = 0; cat_num(m->moves[j].pp); cat("/"); cat_num(M->pp); text(buf, rtext_w(buf, x + 110), y, 5, 0);
  }
  pen = TEXT_PAL;
  do next_frame(); while (!(pressed(KEY_A) || pressed(KEY_B)));
  sfx(SFX_BACK);
  scene_end(); REG_DISPCNT = dispcnt;
}

// ---------- marts (game.js shop, quantity): MONEY box, the stock with prices, how many, YES/NO
static void money_box(int price) {
  box(2, 2, 96, 30); text("MONEY", 10, 8, C_INK, C_SHADOW);
  blen = 0; cat("$"); cat_num(S.money); text(buf, rtext_w(buf, 90), 18, C_INK, C_SHADOW);
  if (price >= 0) { box(2, 34, 96, 22); blen = 0; cat("$"); cat_num(price); text(buf, rtext_w(buf, 90), 41, C_INK, C_SHADOW); }
  else px(0, 2, 34, 96, 22);
}
static const u8 *shop_stock;
static void shop_move(int j) { money_box(j < mi->nstock ? ITEMS[shop_stock[j]].price : -1); }
static int quantity(int price, int max) {
  int n = 1, old = -1;
  for (;;) {
    if (n != old) { box(140, 70, 98, 36); blen = 0; cat("x"); if (n < 10) cat("0"); cat_num(n); text(buf, 152, 76, C_INK, C_SHADOW); blen = 0; cat("$"); cat_num(n * price); text(buf, rtext_w(buf, 230), 90, C_INK, C_SHADOW); old = n; }
    next_frame();
    int was = n;
    if (pressed(KEY_UP)) n = n >= max ? 1 : n + 1;
    else if (pressed(KEY_DOWN)) n = n <= 1 ? max : n - 1;
    else if (pressed(KEY_RIGHT)) n = n + 10 > max ? max : n + 10;
    else if (pressed(KEY_LEFT)) n = n - 10 < 1 ? 1 : n - 10;
    else if (pressed(KEY_A) || pressed(KEY_START)) { sfx(SFX_SELECT); px(0, 140, 70, 98, 36); return n; }
    else if (pressed(KEY_B)) { sfx(SFX_BACK); px(0, 140, 70, 98, 36); return 0; }
    if (n != was) sfx(SFX_CURSOR);
  }
}
static void shop(void) {
  static const char *const YN[] = { "YES", "NO" };
  blen = 0; cat("Welcome to "); cat(mi->town ? mi->town : "the"); cat(" MART! How may I serve you?"); say(buf);
  shop_stock = mi->stock;
  for (;;) {
    const char *labels[9]; int n = mi->nstock;
    for (int i = 0; i < n; i++) labels[i] = ITEM_NAME[shop_stock[i]];
    labels[n] = "QUIT";
    say_now("What would you like?");
    int i = choose_grid(labels, n + 1, 140, 2, 98, 14 + (n + 1) * 16, 16, 10, 1, 0, 1, shop_move);
    if (i < 0 || i == n) break;
    int key = shop_stock[i], price = ITEMS[key].price, max = (int)(S.money / price); if (max > 99) max = 99;
    if (!max) { say("You don't have enough money."); continue; }
    blen = 0; cat(ITEM_NAME[key]); cat("? How many would you like?"); say_now(buf);
    int q = quantity(price, max); if (!q) continue;
    blen = 0; cat(ITEM_NAME[key]); cat(", and you want "); cat_num(q); cat(". That will be $"); cat_num(price * q); cat(". OK?"); say_now(buf);
    if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) != 0) continue;
    S.money -= price * q; S.bag[key] += q; sfx(SFX_BUY); money_box(-1);
    say("Here you go! Thank you very much.");
  }
  surf_clear();
}

// ---------- the Rest House PC (game.js pc, pickFrom): WITHDRAW, DEPOSIT, pages of six
static int pick_from(Mon *list, int count, const char *title) {
  static char lines[8][20]; const char *labels[8];
  for (int page = 0;;) {
    int n = 0, more = count > (page + 1) * 6;
    for (int i = page * 6; i < count && i < page * 6 + 6; i++) {
      blen = 0; { const char *s = mon_name(&list[i]); for (int k = 0; s[k] && k < 10; k++) { char c[2] = { s[k], 0 }; cat(c); } } cat(" Lv"); cat_num(list[i].level);
      int k = 0; for (; buf[k] && k < 19; k++) lines[n][k] = buf[k]; lines[n][k] = 0; labels[n] = lines[n]; n++;
    }
    int shown = n;
    if (more) labels[n++] = "NEXT";
    labels[n++] = "CANCEL";
    say_now(title);
    int i = choose(labels, n, 96, 2, 142, 14 + n * 16, 16, 10, 1);
    if (i < 0 || i == n - 1) return -1;
    if (more && i == shown) { page++; continue; }
    return page * 6 + i;
  }
}
#include "menus_2.inc"
