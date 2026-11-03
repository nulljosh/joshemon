static void pc(void) {
  sfx(SFX_SELECT); blen = 0; cat(PLAYER_NAME); cat(" booted up the PC."); say(buf);
  static const char *const OPTS[] = { "WITHDRAW", "DEPOSIT", "LOG OFF" };
  for (;;) {
    say_now("What would you like to do?");
    int i = choose(OPTS, 3, 130, 2, 108, 62, 16, 10, 1);
    if (i == 0) {
      if (!S.nbox) { say("There are no creatures in the BOX."); continue; }
      if (S.nparty >= 6) { say("Your team is full!"); continue; }
      int j = pick_from(S.box, S.nbox, "Withdraw which one?");
      if (j >= 0) { Mon m = S.box[j]; for (int k = j; k < S.nbox - 1; k++) S.box[k] = S.box[k + 1]; S.nbox--; S.party[S.nparty++] = m; sfx(SFX_BUY); blen = 0; cat(mon_name(&m)); cat(" joined your team."); say(buf); }
    } else if (i == 1) {
      if (S.nparty <= 1) { say("That's your last creature! You can't deposit it."); continue; }
      if (S.nbox >= 30) { say("The BOX is full."); continue; } // ponytail: 30 in the BOX
      int j = pick_from(S.party, S.nparty, "Deposit which one?");
      if (j >= 0) { Mon m = S.party[j]; for (int k = j; k < S.nparty - 1; k++) S.party[k] = S.party[k + 1]; S.nparty--; S.box[S.nbox++] = m; sfx(SFX_BUY); blen = 0; cat(mon_name(&m)); cat(" was stored in the BOX."); say(buf); }
    } else break;
  }
  sfx(SFX_BACK); surf_clear();
}

// ---------- OPTION (game.js options): text speed and sound, saved
static void options(void) {
  static const char *const SPD[3] = { "SLOW", "MID", "FAST" };
  for (;;) {
    char a[20], b[20]; const char *labels[3];
    blen = 0; cat("TEXT  "); cat(SPD[S.speed]); for (int k = 0; k <= blen; k++) a[k] = buf[k];
    blen = 0; cat("SOUND  "); cat(S.mute ? "OFF" : "ON"); for (int k = 0; k <= blen; k++) b[k] = buf[k];
    labels[0] = a; labels[1] = b; labels[2] = "DONE";
    int i = choose(labels, 3, 96, 2, 142, 62, 16, 10, 1);
    if (i == 0) S.speed = (S.speed + 1) % 3;
    else if (i == 1) { S.mute ^= 1; sound_mute(S.mute); if (!S.mute) music(map_song()); }
    else return;
  }
}

// ---------- TOWN MAP (game.js regionMap, drawMap, NODES, LINKS): a sketch of the region, you blink where you are.
// With fly set it picks a town you have been to and returns its FLY index (-1: backed out).
enum { N_TOWN, N_ROUTE, N_CAVE };
static const struct { u8 x, y, kind; const char *name; } NODES[34] = {
  { 36, 126, N_TOWN, "CEDAR TOWN" }, { 36, 98, N_ROUTE, "ROUTE 1" }, { 36, 66, N_TOWN, "HARBOR CITY" }, { 74, 66, N_ROUTE, "ROUTE 2" },
  { 112, 66, N_CAVE, "GRANITE HOLLOW" }, { 112, 46, N_CAVE, "DEEP TUNNEL" }, { 96, 34, N_CAVE, "SLEEPER'S HOLLOW" }, { 140, 46, N_TOWN, "BASALT CITY" },
  { 172, 46, N_ROUTE, "ROUTE 3" }, { 204, 46, N_TOWN, "SKYREACH TOWN" }, { 204, 82, N_ROUTE, "ROUTE 4" }, { 204, 118, N_TOWN, "VOLTARA TOWN" },
  { 204, 126, N_ROUTE, "ROUTE 5" }, { 152, 126, N_ROUTE, "ROUTE 6" }, { 100, 126, N_TOWN, "MIREWOOD CITY" }, // JS had these at 138, under the box
  { 218, 126, N_ROUTE, "ROUTE 7" }, { 232, 126, N_TOWN, "LUMEN ISLE" }, { 232, 100, N_ROUTE, "ROUTE 8" }, { 232, 74, N_TOWN, "FROSTGATE" }, { 232, 50, N_CAVE, "ICE CAVE" }, { 218, 74, N_ROUTE, "FROZEN BAY" }, { 218, 92, N_CAVE, "SUNKEN SHRINE" }, { 172, 92, N_TOWN, "DRAKESPIRE" }, { 172, 110, N_CAVE, "VICTORY ROAD" }, { 140, 110, N_TOWN, "JOSHEMON LEAGUE" },
  { 36, 46, N_ROUTE, "ROUTE 10" }, { 36, 28, N_TOWN, "COVEWICK TOWN" }, { 20, 28, N_ROUTE, "ROUTE 11" }, { 58, 130, N_ROUTE, "ROUTE 12" }, { 76, 120, N_CAVE, "ELDER GROTTO" }, { 172, 30, N_ROUTE, "ROUTE 13" }, { 172, 22, N_CAVE, "SUN RUINS" }, { 172, 62, N_ROUTE, "ROUTE 14" }, { 232, 134, N_ROUTE, "ROUTE 15" },
};
static const u8 LINKS[25][2] = { { 16, 33 }, { 8, 32 }, { 8, 30 }, { 30, 31 }, { 0, 28 }, { 28, 29 }, { 2, 25 }, { 25, 26 }, { 26, 27 }, { 0, 2 }, { 2, 4 }, { 4, 5 }, { 5, 7 }, { 7, 9 }, { 9, 11 }, { 11, 12 }, { 12, 14 }, { 12, 16 }, { 16, 18 }, { 18, 19 }, { 18, 20 }, { 20, 21 }, { 21, 22 }, { 22, 23 }, { 23, 24 } };
static void map_draw(int fly, int here) {
  static const u32 C[] = { 0x2c58a8, 0x3462b4, 0x5a8a40, 0x88c068, 0x78b058, 0xa89070, 0xc0a888, 0xf0e0a0, 0x283040, 0xe85840, 0xf8a088, 0x806040, 0x181010 };
  surf_clear(); scene_cols(C, 13);
  pen = SCENE_PAL;
  px(1, 0, 14, 240, 146); for (int y = 18; y < 160; y += 6) for (int x = (y / 6) % 2 ? 3 : 0; x < 240; x += 6) px(2, x, y, 2, 1);
  px(3, 12, 22, 212, 124); px(4, 14, 20, 212, 124); for (int i = 0; i < 60; i++) px(5, 14 + hash(i, 9) % 212, 20 + hash(9, i) % 124, 3, 2);
  px(6, 92, 22, 48, 34); px(7, 96, 24, 40, 28); // the mountain
  for (int k = 0; k < (int)(sizeof LINKS / sizeof LINKS[0]); k++) {
    int x1 = NODES[LINKS[k][0]].x, y1 = NODES[LINKS[k][0]].y, x2 = NODES[LINKS[k][1]].x, y2 = NODES[LINKS[k][1]].y;
    px(8, (x1 < x2 ? x1 : x2) - 1, (y1 < y2 ? y1 : y2) - 1, (x2 > x1 ? x2 - x1 : x1 - x2) + 3, (y2 > y1 ? y2 - y1 : y1 - y2) + 3);
  }
  for (int i = 0; i < (int)(sizeof NODES / sizeof NODES[0]); i++) {
    int x = NODES[i].x, y = NODES[i].y;
    if (i == 6 && badge_count() < 3) continue; // the hollow opens with three badges
    if (NODES[i].kind == N_TOWN) { px(9, x - 5, y - 5, 11, 11); px(10, x - 4, y - 4, 9, 9); px(11, x - 3, y - 3, 3, 2); }
    if (NODES[i].kind == N_CAVE) { px(9, x - 5, y - 5, 11, 11); px(12, x - 4, y - 4, 9, 9); px(13, x - 2, y - 1, 5, 5); }
  }
  pen = 11; { static const u32 H[] = { 0x203860, 0xf8f8f8, 0x102040 }; for (int i = 0; i < 3; i++) PAL_BG[11 * 16 + 1 + i] = HEX(H[i]); }
  px(1, 0, 0, 240, 14); text(fly ? "FLY: pick a town" : "TOWN MAP", 8, 3, 2, 3);
  pen = TEXT_PAL; box(2, 136, 236, 22); text(NODES[here].name, 12, 143, C_INK, C_SHADOW);
}
static int region_map(int fly) {
  u16 dispcnt = REG_DISPCNT;
  int towns[FLY_COUNT], nt = 0, pick = 0;
  for (int i = 0; i < FLY_COUNT; i++) if (i == 0 || flag(FLY[i].been)) { if (FLY[i].map == cur || FLY[i].node == mi->node) pick = nt; towns[nt++] = i; }
  // the cursor: a white square frame as a sprite (16x16 at tile 520), blinking where you are
  volatile u32 *t = (volatile u32 *)OBJ_TILES + 520 * 8;
  for (int q = 0; q < 4; q++) for (int r = 0; r < 8; r++) {
    int tx = q & 1, ty = q >> 1, y = ty * 8 + r; u32 w = 0;
    for (int x = 0; x < 8; x++) { int X = tx * 8 + x; if (X < 15 && y < 15 && (X < 2 || X > 12 || y < 2 || y > 12)) w |= 1u << (x * 4); }
    t[q * 8 + r] = w;
  }
  PAL_OBJ[15 * 16 + 1] = HEX(0xf8f8f8);
  scene_begin(); sfx(SFX_SELECT);
  int here = fly ? FLY[towns[pick]].node : mi->node, drawn = -1, to = -1;
  for (;;) {
    if (here != drawn) { map_draw(fly, here); drawn = here; }
    int on = fly || ((frame >> 4) & 1);
    scene_obj[0] = (Obj){ (NODES[here].y - 7) & 255, ((NODES[here].x - 7) & 511) | (1 << 14), 520 | (15 << 12), 0 }; scene_n = on;
    next_frame();
    if (fly && (pressed(KEY_RIGHT) || pressed(KEY_DOWN))) { pick = (pick + 1) % nt; here = FLY[towns[pick]].node; sfx(SFX_CURSOR); }
    else if (fly && (pressed(KEY_LEFT) || pressed(KEY_UP))) { pick = (pick + nt - 1) % nt; here = FLY[towns[pick]].node; sfx(SFX_CURSOR); }
    else if (fly && pressed(KEY_A)) { to = towns[pick]; break; }
    else if (pressed(KEY_A) || pressed(KEY_B) || pressed(KEY_START)) break;
  }
  sfx(to >= 0 ? SFX_SELECT : SFX_BACK);
  scene_end(); REG_DISPCNT = dispcnt;
  if (!in_battle) load_map_gfx();
  return to;
}

// ---------- field skills (game.js FIELD, obstacle, clearObstacle, shore, startSurf, fish): unlocked by badges, used by
// any creature on the team, from the team menu or by pressing A at the obstacle
static const char *const SKILL_NAME[SK_COUNT] = { "CUT", "ROCK SMASH", "FLY", "SURF", "FLASH", "STRENGTH" };
static const char *const SKILL_BADGE[SK_COUNT] = { "TIDE", "STONE", "FEATHER", "VOLT", "VENOM", "MIND" };
static int has_skill(int k) { // the badge may not exist yet (MIND comes later in the story)
  for (int b = 0; b < BADGE_COUNT; b++) { const char *a = BADGE_NAME[b], *c = SKILL_BADGE[k]; while (*a && *a == *c) a++, c++; if (!*a && !*c) return flag(BADGE_FLAG[b]); }
  return 0;
}
static void facing(int *x, int *y) { *x = me_x + DX[me_dir]; *y = me_y + DY[me_dir]; }
static Mon *first_able(void) { for (int i = 0; i < S.nparty; i++) if (S.party[i].hp) return &S.party[i]; return 0; }
static int clear_obstacle(Mon *m, int skill) {
  int x, y; facing(&x, &y); char t = tile_at(x, y);
  int want = t == 'K' ? SK_CUT : t == 'Y' ? SK_SMASH : t == 'O' ? SK_STRENGTH : -1;
  if (want != skill) { say("Can't use that here."); return 0; }
  blen = 0; cat(mon_name(m)); cat(" used "); cat(SKILL_NAME[skill]); cat("!"); say(buf);
  if (skill == SK_STRENGTH) { map_strength = 1; sfx(SFX_BUMP); blen = 0; cat(mon_name(m)); cat("'s STRENGTH made it possible to move boulders around."); say(buf); return 1; }
  sfx(skill == SK_CUT ? SFX_HOP : SFX_BUMP); set_tile(x, y, ground()); return 1;
}
static void obstacle(int x, int y) {
  static const char *const YN[] = { "YES", "NO" };
  char t = tile_at(x, y); int skill = t == 'K' ? SK_CUT : t == 'Y' ? SK_SMASH : SK_STRENGTH;
  say(skill == SK_CUT ? "This tree looks like it can be CUT down!" : skill == SK_SMASH ? "It's a cracked rock. A creature may be able to smash it." : "It's a big boulder. A creature may be able to push it aside.");
  Mon *m = first_able();
  if (!has_skill(skill) || (skill == SK_STRENGTH && map_strength) || !m) return;
  blen = 0; cat("Would you like to use "); cat(SKILL_NAME[skill]); cat("?"); say_now(buf);
  if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) == 0) clear_obstacle(m, skill);
}
static void start_surf(Mon *m) {
  blen = 0; cat(mon_name(m)); cat(" used SURF!"); say(buf); surf_clear();
  S.bike = 0; S.surf = m->id; load_look(LOOK_SURF); music(SONG_SURF); sfx(SFX_HOP);
  moving = 1; hop = 0; speed = 2; // hop on and glide one tile out
}
static void shore(void) {
  static const char *const YN[] = { "YES", "NO" };
  if (!(mi->flags & (MF_OUTDOOR | MF_CAVE))) return; // pools indoors are just pools; caves and the shrine flood
  say("The water is dyed a deep blue...");
  Mon *m = first_able();
  if (!has_skill(SK_SURF) || !m) return;
  say_now("Would you like to SURF?");
  if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) == 0) start_surf(m);
}
// returns 1 when the menus should close
static int field_skill(int k, Mon *m) {
  int x, y; facing(&x, &y);
  if (k == SK_CUT || k == SK_SMASH || k == SK_STRENGTH) return clear_obstacle(m, k);
  if (k == SK_FLY) {
    if ((mi->flags & MF_CAVE) || !(mi->flags & MF_OUTDOOR)) { say("Can't use that here."); return 0; }
    int to = region_map(1); if (to < 0) return 0;
    blen = 0; cat(mon_name(m)); cat(" used FLY!"); say(buf); surf_clear();
    Warp w = { 0, 0, FLY[to].map, FLY[to].x, FLY[to].y, DIR_DOWN }; warp(&w); return 1;
  }
  if (k == SK_SURF) {
    if (S.surf || tile_at(x, y) != '~' || npc_at(x, y) >= 0) { say("Can't use that here."); return 0; }
    start_surf(m); return 1;
  }
  if (k == SK_FLASH) {
    if (!(mi->flags & MF_DARK) || map_flash) { say("Can't use that here."); return 0; }
    map_flash = 1; sfx(SFX_HEAL); blen = 0; cat(mon_name(m)); cat(" used FLASH! The darkness lifted."); say(buf); dark_setup(); return 1;
  }
  say("Can't use that here.");
  return 0;
}
// the OLD ROD: cast, wait, and press A while the line bobs; too slow and it gets away
static void fish(void) {
  int x, y; facing(&x, &y);
  if (!(S.surf || tile_at(x, y) == '~') || !mi->fish[0]) { say("Can't use that here."); return; }
  blen = 0; cat(PLAYER_NAME); cat(" cast the line."); say_now(buf); sfx(SFX_SELECT);
  for (int i = 0, n = 2 + ri(3); i < n; i++) { wait(30); cat(" ."); say_now(buf); }
  if (ri(4) == 0) { say("Not even a nibble..."); return; }
  say_now("Oh! A bite!"); sfx(SFX_LOWHP);
  int sx = MAP_PADX * 16 - camx + me_x * 16 + 3, sy = MAP_PADY * 16 - camy + me_y * 16 - 17, got = 0;
  for (int t = 0; t < 60 && !got; t++) { // a real second to press A, the "!" blinking over you
    if ((frame >> 3) & 1) { box(sx, sy, 12, 14); text("!", sx + 3, sy + 3, C_RED, 0); } else px(0, sx, sy, 12, 14);
    next_frame(); got = pressed(KEY_A);
  }
  px(0, sx, sy, 12, 14);
  if (!got) { say("The creature got away..."); return; }
  say("Landed a creature!"); surf_clear();
  wild_kind(2);
}
static int ride_bike(void) {
  if (S.surf || !(mi->flags & (MF_OUTDOOR | MF_CAVE))) { say("Can't use that here."); return 0; }
  S.bike ^= 1; if (S.bike) load_look(LOOK_BIKE); music(map_song()); sfx(SFX_SELECT);
  blen = 0; cat(PLAYER_NAME); cat(S.bike ? " got on the BIKE." : " got off the BIKE."); say(buf); return 1;
}

// ---------- GIVE: a held item goes to a creature; what it held comes back to the BAG
static void give_item(int key) {
  int i = team_screen(1, 1, "Give to which creature?");
  if (i < 0) return;
  Mon *m = &S.party[i];
  if (m->held) BAG(m->held - 1)++;
  m->held = key + 1; BAG(key)--;
  blen = 0; cat(mon_name(m)); cat(" is holding the "); cat(ITEM_NAME[key]); cat("."); say(buf);
}
