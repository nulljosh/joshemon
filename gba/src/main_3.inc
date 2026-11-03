static void title(void) {
  REG_DISPCNT = 0x80;
  sound_init(); music(SONG_TITLE);
#ifdef QA_JUKE
  for (int k = 0; k < SONG_COUNT; k++) { music(k); for (int f = SONGS[k].loop ? 1800 : 360; f--;) present(); } // every song in turn, for listening
#endif
  for (int i = 0; i < TITLE_NPAL; i++) PAL_BG[i] = TITLE_PAL[i];
  copy32((volatile u32 *)CHARBLOCK(CBB_MAP), TITLE_TILES, TITLE_NTILES * 16);
  volatile u16 *m = SCREENBLOCK(SBB_GROUND); for (int i = 0; i < 1024; i++) m[i] = (i & 31) < 30 && i < 640 ? TITLE_MAP[(i >> 5) * 30 + (i & 31)] : 0;
  REG_BG1CNT = BG_CBB(CBB_MAP) | BG_SBB(SBB_GROUND) | BG_8BPP | BG_PRIO(2); REG_BG1HOFS = REG_BG1VOFS = 0;
  load_text_pal(); surf_clear();
  REG_BG0CNT = BG_CBB(CBB_TEXT) | BG_SBB(SBB_TEXT) | BG_PRIO(0);
  static const u32 C[] = { 0x182048, 0xf8d040, 0xc06818, 0xf8f8f8, 0x183070, 0x102040, 0xa8c0e8 };
  for (int i = 0; i < 7; i++) PAL_BG[12 * 16 + 1 + i] = HEX(C[i]);
  pen = 12;
  int lx = (240 - measure("JOSHEMON") * 3) / 2;
  static const s8 OUT[8][2] = { { -2, 0 }, { 2, 0 }, { 0, -2 }, { 0, 2 }, { -2, -2 }, { 2, 2 }, { -2, 2 }, { 2, -2 } };
  for (int k = 0; k < 8; k++) text_s("JOSHEMON", lx + OUT[k][0], 17 + OUT[k][1], 1, 0, 3);
  text_s("JOSHEMON", lx, 17, 2, 3, 3);
  { // the acute on the E (JOSHÉMON): two font pixels on a slant, outlined, shadowed and filled like the letters
    static const s8 AC[6][2] = { { 4, -4 }, { 5, -4 }, { 3, -3 }, { 4, -3 }, { 2, -2 }, { 3, -2 } }; int ex = lx + measure("JOSH") * 3 - 16;
    for (int pass = 0; pass < 3; pass++) for (int a = 0; a < 6; a++) {
      int x = ex + AC[a][0] * 3, y = 17 + AC[a][1] * 3;
      if (pass == 0) for (int k = 0; k < 8; k++) px(1, x + OUT[k][0], y + OUT[k][1], 3, 3);
      else px(pass == 1 ? 3 : 2, x + (pass == 1 ? 3 : 0), y + (pass == 1 ? 3 : 0), 3, 3); } }
  const char *sub = "A MONSTER COLLECTING ADVENTURE", *cr = "(C) 2026 JOSHUA TROMMEL", *ps = "PRESS START";
  text(sub, (240 - measure(sub)) / 2, 44, 4, 5);
  text(cr, (240 - measure(cr)) / 2, 148, 7, 0);
  pen = TEXT_PAL;
  REG_DISPCNT = MODE0 | BG0_ON | BG1_ON | OBJ_ON | OBJ_1D;
  static const u8 SHOW[6] = { 1, 4, 7, 25, 70, 88 };
  for (int f = 0, shown = -1;; f++) {
    int id = SHOW[(f >> 7) % 6];
    if (id != shown) { load_art(0, 0, ART_MON(id, 0), 0); shown = id; }
    oam[0] = (Obj){ (108 - MON_FEET[id - 1] - 4 + ((f >> 4) & 1)) & 255, 88 | (3 << 14), 0 | (1 << 10), 0 };
    for (int i = 1; i < 128; i++) oam[i].a0 = 1 << 9;
    vsync(); copy32((volatile u32 *)OAM, (const u32 *)oam, 256);
    read_keys(); frame++;
    if ((f & 31) == 0) { pen = 12; if ((f >> 5) & 1) text(ps, (240 - measure(ps)) / 2, 132, 4, 6); else px(0, 0, 132, 240, 10); pen = TEXT_PAL; }
    if (pressed(KEY_START) || pressed(KEY_A)) break;
  }
  // the title stays up under CONTINUE / NEW GAME; the world or the intro replaces it
}

int main(void) {
  REG16(0x04000204) = 0x4317; // WAITCNT: ROM 3/1 wait states with the prefetch buffer (the BIOS default is 4/2, no prefetch); SRAM 8
  title();
  int qa = keys & KEY_L && keys & KEY_R, qsel = keys & KEY_SELECT, keys_at_title = keys; (void)qsel; (void)keys_at_title; // QA: START with L+R held skips the intro and the PROF scene: a L5 SPRIGLING, 5 BALLS, 2 POTIONS
  release();
  static const char *const pick[] = { "CONTINUE", "NEW GAME" };
  int cont = sram_read();
  if (cont) { cont = choose(pick, 2, 60, 92, 120, 44, 24, 10, 0) == 0; qa |= keys & KEY_L && keys & KEY_R; release(); }
  if (!cont) {
    for (unsigned i = 0; i < sizeof S; i++) ((u8 *)&S)[i] = 0;
    S.money = 3000; S.speed = 1; S.map = HOME.map; S.x = HOME.x; S.y = HOME.y; S.dir = HOME.dir;
    S.heal[0] = HOME.map; S.heal[1] = HOME.x; S.heal[2] = HOME.y; S.heal[3] = HOME.dir;
#ifdef QA_FIELD
    if (qa) { make_mon(&S.party[0], 1, 30); S.nparty = 1; S.starter = 1; set_flag(FLAG_STARTER); set_flag(FLAG_PROFOUT);
#ifdef QA_DIPLOMA
      for (int id = 1; id <= 105; id++) { mark(S.seen, id); mark(S.caught, id); } set_flag(FLAG_CHAMPION); set_flag(FLAG_GOT_STARTERS); for (int k = 0; k < FLY_COUNT; k++) set_flag(FLY[k].been);
#endif
      for (int b = 0; b < BADGE_COUNT - 1; b++) set_flag(BADGE_FLAG[b]);
      S.bag[ITEM_BIKE] = S.bag[ITEM_ROD] = S.bag[ITEM_MAP] = 1; S.bag[ITEM_BALL] = 5; S.map = MAP_HARBOR; S.x = 5; S.y = 3; S.dir = DIR_UP;
#ifdef QA_DIPLOMA
      S.map = MAP_CEDAR; S.x = 10; S.y = 1; // facing PROF. CEDAR
      if (keys_at_title & KEY_DOWN) { S.map = MAP_LEAGUE; S.x = 20; S.y = 6; make_mon(&S.party[0], 9, 80); S.party[0].moves[0].id = 58; } // START+L+R+DOWN: at the BATTLE HALL door
      if (keys_at_title & KEY_RIGHT) { S.map = MAP_CEDAR; S.x = 17; S.y = 8; S.dir = DIR_RIGHT; make_mon(&S.party[0], 9, 80); S.party[0].moves[0].id = 58; S.repel = 250; } // START+L+R+RIGHT: at CEDAR's east exit, the coast road
      if (keys_at_title & KEY_UP) { S.map = MAP_SKY; S.x = 9; S.y = 3; make_mon(&S.party[0], 9, 80); S.party[0].moves[0].id = 58; } // START+L+R+UP: SKYREACH, under the SUMMIT path
#endif
      if (qsel) { S.map = MAP_ROUTE2; S.x = 17; S.y = 4; S.dir = DIR_RIGHT; }
#ifdef QA_REAL
      { // a fair LEAGUE run: six fully evolved creatures at 54 to 56, damage and PP as in the real game, in the lobby under ELITE FOUR NOX's door.
        // Each creature's strongest move (power, same-type bonus, accuracy) goes first, as a player would pick it, since scripts mash the first move
        static const u8 TEAM[6][2] = {{9, 56}, {90, 55}, {103, 55}, {27, 55}, {64, 54}, {61, 54}};
        for (int i = 0; i < 6; i++) { Mon *m = &S.party[i]; make_mon(m, TEAM[i][0], TEAM[i][1]); mark(S.seen, m->id); mark(S.caught, m->id);
          for (int a = 0; a < 4; a++) for (int b = a + 1; b < 4; b++) {
            const Species *sp = &SPECIES[m->id]; int v[2]; Slot *x[2] = {&m->moves[a], &m->moves[b]};
            for (int k = 0; k < 2; k++) { const Move *M = &MOVES[x[k]->id]; v[k] = x[k]->id ? M->power * (M->type == sp->types[0] || M->type == sp->types[1] ? 3 : 2) * (M->acc > 100 ? 100 : M->acc) : -1; }
            if (v[1] > v[0]) { Slot t = *x[0]; *x[0] = *x[1]; *x[1] = t; } } }
        S.nparty = 6; for (int b = 0; b < BADGE_COUNT; b++) set_flag(BADGE_FLAG[b]); set_flag(FLAG_BEEN_LEAGUE);
        S.map = MAP_LOBBY; S.x = 6; S.y = 1; S.dir = DIR_UP;
        if (keys_at_title & KEY_RIGHT) { set_flag(FLAG_BEAT_NOX); set_flag(FLAG_BEAT_VESPERA); set_flag(FLAG_BEAT_FERRAND); set_flag(FLAG_BEAT_BRAKKA); S.map = MAP_CHAMP; S.x = 5; S.y = 9; } } // +RIGHT: fresh, at AURIC's door
#endif
#if !defined QA_DIPLOMA && !defined QA_REAL
      { // START+L+R plus UP, RIGHT, DOWN, LEFT or UP+LEFT: under the door of gym 1 to 5, with only the badges before it and a L80 on ICE BEAM
        int t = keys_at_title, gym = t & KEY_B ? 0 : t & KEY_UP ? (t & KEY_LEFT ? 5 : t & KEY_RIGHT ? 0 : 1) : t & KEY_RIGHT ? 2 : t & KEY_DOWN ? 3 : t & KEY_LEFT ? 4 : 0;
        if (!(t & KEY_B) && (t & KEY_UP) && (t & KEY_RIGHT)) { S.map = MAP_HARBOR; S.x = 11; S.y = 3; S.dir = DIR_UP; make_mon(&S.party[0], 9, 80); S.party[0].moves[0].id = 58; S.repel = 250; } // UP+RIGHT: under the SEAWALK pier
        static const u8 DOOR[5][3] = {{MAP_HARBOR, 8, 15}, {MAP_BASALT, 7, 14}, {MAP_SKY, 7, 14}, {MAP_VOLTARA, 7, 13}, {MAP_MIREWOOD, 7, 13}};
        if (gym) { S.map = DOOR[gym - 1][0]; S.x = DOOR[gym - 1][1]; S.y = DOOR[gym - 1][2]; S.dir = DIR_UP;
          for (int b = gym - 1; b < BADGE_COUNT; b++) S.flags[BADGE_FLAG[b] >> 3] &= ~(1 << (BADGE_FLAG[b] & 7));
          if (gym >= 4) set_flag(FLAG_PLANT);
          if (gym >= 5) set_flag(FLAG_HIDEOUT);
          make_mon(&S.party[0], 9, 80); S.party[0].moves[0].id = 58; S.repel = 250; } }
#endif
      if (keys_at_title & KEY_B) { S.map = MAP_LUMEN; S.x = 11; S.y = 6; S.dir = DIR_UP; make_mon(&S.party[0], 9, 80); set_flag(FLAG_MAYA9);
        S.party[0].moves[0].id = 58; S.party[0].moves[0].pp = 99; S.repel = 250;
        if (qsel) { S.map = MAP_ROUTE5; S.x = 1; S.y = 9; S.dir = DIR_LEFT; S.surf = 9; S.flags[FLAG_MAYA9 >> 3] &= ~(1 << (FLAG_MAYA9 & 7)); }
        if (keys_at_title & KEY_UP) { set_flag(FLAG_BADGE_MIND); set_flag(FLAG_LIGHTHOUSE); set_flag(FLAG_BEAT_SABLE); S.map = MAP_FROSTGATE; S.x = 11; S.y = 2; S.dir = DIR_UP;
          if (keys_at_title & KEY_RIGHT) { set_flag(FLAG_BOREAL_MET); S.y = 13; }
          if (keys_at_title & KEY_LEFT) { set_flag(FLAG_BOREAL_MET); set_flag(FLAG_BEAT_BOREAL); set_flag(FLAG_BADGE_FROST); S.x = 2; S.y = 8; S.dir = DIR_LEFT;
            if (qsel) { set_flag(FLAG_MAYA9); set_flag(FLAG_BEAT_CINDRA); set_flag(FLAG_SEA_CALM); S.surf = 0; S.map = MAP_DRAKE; S.x = 11; S.y = 4; S.dir = DIR_DOWN;
              if (keys_at_title & KEY_RIGHT) { set_flag(FLAG_BEAT_WYRMOND); set_flag(FLAG_BADGE_DRAGON); S.x = 12; S.y = 18; } } } } // B+UP: FROSTGATE, under the ICE CAVE; +RIGHT: at the gym door, BOREAL met; +LEFT: the west edge with the FROST BADGE; +LEFT+SELECT: DRAKESPIRE
        if (keys_at_title & KEY_DOWN) { set_flag(FLAG_BADGE_MIND); set_flag(FLAG_LIGHTHOUSE); set_flag(FLAG_BEAT_SABLE); S.x = 11; S.y = 1; S.surf = 9; } } } // B+DOWN: chapter 7, surfing north of LUMEN with the MIND BADGE // B+SELECT: surfing at ROUTE 5's west edge, MAYA waiting // ICE BEAM first: scripts mash the first move, and it lands on everything in chapter 6
#elif defined QA_EVO
    if (qa) { make_mon(&S.party[0], 1, 15); S.party[0].exp = exp_for_level(SPECIES[1].growth, 16) - 1; S.nparty = 1; }
#else
    if (qa) { make_mon(&S.party[0], 1, 5); S.nparty = 1; S.starter = 1; set_flag(FLAG_STARTER); mark(S.seen, 1); mark(S.caught, 1); S.bag[ITEM_BALL] = 5; S.bag[ITEM_POTION] = 2; }
#endif
  }
  sound_mute(S.mute);
  int fresh = !cont && !qa;
  if (fresh) intro_speech();
  REG_DISPCNT = 0x80;
  surf_clear();
  REG_BG1CNT = BG_CBB(CBB_MAP) | BG_SBB(SBB_GROUND) | BG_8BPP | BG_PRIO(2);
  REG_BG2CNT = BG_CBB(CBB_MAP) | BG_SBB(SBB_OVER) | BG_8BPP | BG_PRIO(0);
  REG_BLDCNT = 0x3F | (3 << 6) | (1 << 9) | (1 << 13); // fades darken everything; shadows blend onto the ground
  REG_BLDALPHA = 16 | (12 << 8);
  REG_WININ = 0x3F; REG_WINOUT = 0; // the wipe: only what is inside window 0 shows, the rest is black
  load_shadow();
  load_map(S.map); me_x = S.x; me_y = S.y; me_dir = S.dir;
  load_text_pal();
  render(); present();
  if (fresh) fade_level = 16;
  REG_DISPCNT = world_dispcnt();
  if (fresh) fade(0);
  show_name();
  if (fresh) { say("MOM: Here we are, JOSH! Our new home in CEDAR TOWN."); say("MOM: PROF. CEDAR lives just north of town. Why don't you go say hello?"); surf_clear(); }
  for (;;) {
    if (!moving && pressed(KEY_A)) talk();
    else if (!moving && pressed(KEY_START)) start_menu();
    world_step(); next_frame();
  }
}
