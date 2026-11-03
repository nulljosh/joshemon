static int dex_caught_105(void) { int n = 0; for (int id = 1; id <= 105; id++) n += (S.caught[id >> 3] >> (id & 7)) & 1; return n; }
static void diploma(void) {
  static const u32 C[] = { 0xf0e8d0, 0xc0a060, 0x403020, 0xa03028, 0x806040 };
  scene_begin(); scene_cols(C, 5); music(SONG_CAUGHT);
  pen = SCENE_PAL; px(1, 0, 0, 240, 160); px(5, 6, 6, 228, 3); px(5, 6, 151, 228, 3); px(5, 6, 6, 3, 148); px(5, 231, 6, 3, 148); px(2, 11, 11, 218, 1); px(2, 11, 148, 218, 1); px(2, 11, 11, 1, 138); px(2, 228, 11, 1, 138);
  text_s("DIPLOMA", (240 - measure("DIPLOMA") * 2) / 2, 24, 4, 5, 2);
  blen = 0; cat("This certifies that "); cat(PLAYER_NAME);
  text(buf, (240 - measure(buf)) / 2, 64, 3, 0);
  text("caught every one of the 105 creatures", (240 - measure("caught every one of the 105 creatures")) / 2, 80, 3, 0);
  text("and completed the DEX.", (240 - measure("and completed the DEX.")) / 2, 96, 3, 0);
  text("PROF. CEDAR", 150, 124, 4, 0);
  pen = TEXT_PAL; for (int t = 0; t < 600 && !(t > 30 && pressed(KEY_A)); t++) next_frame();
  scene_end(); load_map_gfx(); music(map_song()); REG_DISPCNT = world_dispcnt();
}
static void postgame_prof(void) {
  if (!flag(FLAG_GOT_STARTERS)) {
    static const char *const L[] = { "PROF. CEDAR: JOSH! The new CHAMPION, back in little CEDAR TOWN!", "PROF. CEDAR: I have a favor to ask. Two creatures in my bag have been waiting a long time for a trainer.",
      "PROF. CEDAR: They are the two you did not choose that first day. Will you take them?", 0 };
    say_lines(L);
    for (int k = 0; k < 3; k++) if (STARTERS[k] != S.starter) give_mon(STARTERS[k], 5);
    set_flag(FLAG_GOT_STARTERS);
    static const char *const L2[] = { "PROF. CEDAR: There. Now every creature in the region can be found. I'd love to see a full DEX one day!", 0 };
    say_lines(L2); return;
  }
  int n = dex_caught_105();
  if (n >= 105 && !flag(FLAG_DIPLOMA)) {
    static const char *const L[] = { "PROF. CEDAR: What's this? Your DEX... all 105?! Every single one!", "PROF. CEDAR: I never thought I'd see it. This calls for a DIPLOMA!", 0 };
    say_lines(L); diploma(); set_flag(FLAG_DIPLOMA);
    static const char *const L2[] = { "PROF. CEDAR: Congratulations, JOSH. You've done what no one else has.", 0 }; say_lines(L2); return;
  }
  if (flag(FLAG_DIPLOMA)) { say("PROF. CEDAR: A complete DEX... I still can't believe it. Thank you, JOSH."); return; }
  blen = 0; cat("PROF. CEDAR: Your DEX shows "); cat_num(n); cat(" of the 105 caught. Keep going!"); say(buf);
}

// The BATTLE HALL: the CHAMPION takes on trainer after trainer. Each one brings three fully evolved creatures at the
// level of your strongest; every seventh win they get two levels stronger. A loss only ends the streak.
static Trainer hall_t; static u8 hall_party[6]; static char hall_name[28];
static void battle_hall(void) {
  static const char *const YN[] = { "YES", "NO" };
  static const char *const CLASS[] = { "ACE TRAINER ", "ACE TRAINER ", "DRAGON TAMER ", "SKIER ", "HIKER ", "SWIMMER ", "PSYCHIC ", "BIRDER " };
  static const u8 LOOKS[] = { LOOK_ACE, LOOK_ACEF, LOOK_TAMER, LOOK_SKIER, LOOK_HIKER, LOOK_SWIMMER, LOOK_PSYCHIC, LOOK_BIRDER };
  static const char *const NAMES[] = { "ROWAN", "ISLA", "TEO", "MIRA", "BODHI", "SAGE", "FINN", "NOVA", "ODA", "LUCA", "WREN", "ARLO" };
  if (!flag(FLAG_CHAMPION)) { say("Welcome to the BATTLE HALL! I'm sorry, the HALL is open only to the CHAMPION."); return; }
  blen = 0; cat("Welcome to the BATTLE HALL! Your best streak is "); cat_num(S.hall_best); cat(S.hall_best == 1 ? " win." : " wins."); say(buf);
  say_now("Will you start a streak?");
  if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) != 0) { say("Come back any time!"); return; }
  int top = 50; for (int i = 0; i < S.nparty; i++) if (S.party[i].level > top) top = S.party[i].level;
  u8 finals[110]; int nf = 0; for (int id = 1; id < 105; id++) if (!SPECIES[id].nevo) finals[nf++] = id;
  heal_all();
  int streak = 0;
  for (;;) {
    int c = ri(8), lv = top + streak / 7 * 2; if (lv > 100) lv = 100;
    for (int k = 0; k < 3; k++) { hall_party[k * 2] = finals[ri(nf)]; hall_party[k * 2 + 1] = lv; }
    blen = 0; cat(CLASS[c]); cat(NAMES[ri(12)]); for (int k = 0; k <= blen; k++) hall_name[k] = buf[k];
    hall_t = (Trainer){ hall_name, 0, 0, 3, 1, 0, 255, 0, 0, 0, 0, hall_party, 0, 0, 0, "Well fought!" };
    blen = 0; cat("Battle "); cat_num(streak + 1); cat(": "); cat(hall_name); cat(" steps up!"); say(buf);
    hall_mode = 1; int r = trainer_battle(&hall_t, LOOKS[c]); hall_mode = 0;
    if (r != R_WIN) { blen = 0; cat("The streak ends at "); cat_num(streak); cat(streak == 1 ? " win." : " wins."); say(buf); break; }
    streak++; if (streak > S.hall_best) S.hall_best = streak;
    heal_all();
    blen = 0; cat(PLAYER_NAME); cat(" has won "); cat_num(streak); cat(streak == 1 ? " battle" : " battles"); cat(" in a row! Your team is healed."); say(buf);
    say_now("Keep the streak going?");
    if (choose(YN, 2, 176, 66, 60, 44, 16, 10, 1) != 0) break;
  }
  heal_all();
  blen = 0; cat("Your best streak is "); cat_num(S.hall_best); cat(". Thanks for battling!"); say(buf);
}
