static const char *const L6_sable_win[] = {"SABLE: The future was kind to you. Take the MIND BADGE.", "JOSH received the MIND BADGE from SABLE!", "SABLE: With it, STRENGTH will move boulders from your path.", 0};
static const char *const L6_sable_after[] = {"SABLE: I see snow in your future. The pass north of here leads to FROSTGATE.", "SABLE: And past the ice, the shrine under the sea. Be ready.", 0};
static const unsigned char TP6_swim1[] = {78,35,33,35};
static const Trainer T6_swim1 = {"SWIMMER LEA", 4, 20, 2, 1, 0, 255, 0, FLAG_BEAT_R7SWIM1, 0, 0, TP6_swim1, L6_swim1_in, L6_swim1_after, 0, "I sank!"};
static const unsigned char TP6_sailor[] = {29,36};
static const Trainer T6_sailor = {"SAILOR BEN", 3, 32, 1, 1, 0, 255, 0, FLAG_BEAT_R7SAILOR, 0, 0, TP6_sailor, L6_sailor_in, L6_sailor_after, 0, "Abandon ship!"};
static const unsigned char TP6_swim2[] = {81,36,78,35};
static const Trainer T6_swim2 = {"SWIMMER NOE", 4, 20, 2, 1, 0, 255, 0, FLAG_BEAT_R7SWIM2, 0, 0, TP6_swim2, L6_swim2_in, L6_swim2_after, 0, "Glub..."};
static const unsigned char TP6_maya[] = {69,37,71,37,6,40,69,37,71,37,9,40,69,37,71,37,3,40};
static const Trainer T6_maya = {"MAYA", 3, 80, 3, 3, 0, 255, 1, FLAG_BEAT_MAYA9, FLAG_MAYA9, 0, TP6_maya, L6_maya_in, L6_maya_after, 0, "Gah! You got stronger again!"};
static const unsigned char TP6_g1[] = {84,36,13,36};
static const Trainer T6_g1 = {"ASH GRUNT", 3, 60, 2, 1, 0, 255, 0, FLAG_BEAT_LHGRUNT1, 0, 0, TP6_g1, L6_g1_in, L6_g1_after, 0, "Lights out for me..."};
static const unsigned char TP6_g2[] = {86,37};
static const Trainer T6_g2 = {"ASH GRUNT", 4, 60, 1, 1, 0, 255, 0, FLAG_BEAT_LHGRUNT2, 0, 0, TP6_g2, L6_g2_in, L6_g2_after, 0, "I knew I should have brought a torch."};
static const unsigned char TP6_g3[] = {42,37,84,37};
static const Trainer T6_g3 = {"ASH GRUNT", 3, 60, 2, 1, 0, 255, 0, FLAG_BEAT_LHGRUNT3, 0, 0, TP6_g3, L6_g3_in, L6_g3_after, 0, "Not again!"};
static const unsigned char TP6_ember[] = {87,38,61,39,43,40};
static const Trainer T6_ember = {"ADMIN EMBER", 3, 160, 3, 1, 0, 255, 0, FLAG_BEAT_EMBER2, 0, FLAG_LIGHTHOUSE, TP6_ember, L6_ember_in, L6_ember_after, 0, "Burned out... again."};
static const unsigned char TP6_psy1[] = {26,38,102,39};
static const Trainer T6_psy1 = {"PSYCHIC ARIA", 4, 48, 2, 1, 0, 255, 0, FLAG_BEAT_PSY1, 0, 0, TP6_psy1, L6_psy1_in, L6_psy1_after, 0, "The dream lied."};
static const unsigned char TP6_psy2[] = {50,38,56,39};
static const Trainer T6_psy2 = {"PSYCHIC ODIN", 4, 48, 2, 1, 0, 255, 0, FLAG_BEAT_PSY2, 0, 0, TP6_psy2, L6_psy2_in, L6_psy2_after, 0, "I did not see that coming."};
static const unsigned char TP6_sable[] = {50,40,57,41,27,43};
static const Trainer T6_sable = {"LEADER SABLE", 0, 100, 3, 1, 6, ITEM_TM15, 0, FLAG_BEAT_SABLE, 0, 0, TP6_sable, L6_sable_in, L6_sable_after, L6_sable_win, "So this is the true one."};
static const unsigned short C6_maya[] = {FLAG_BADGE_VENOM, FLAG_MAYA9 | 0x8000, 0};
static const unsigned short C6_dark[] = {FLAG_LIGHTHOUSE | 0x8000, 0};
static const unsigned short C6_lit[] = {FLAG_LIGHTHOUSE, 0};
static const Warp W6_route7[] = {{29, 7, MAP_ROUTE5, 0, 7, DIR_RIGHT}, {29, 8, MAP_ROUTE5, 0, 8, DIR_RIGHT}, {29, 9, MAP_ROUTE5, 0, 9, DIR_RIGHT}, {29, 10, MAP_ROUTE5, 0, 10, DIR_RIGHT}, {29, 11, MAP_ROUTE5, 0, 11, DIR_RIGHT}, {29, 12, MAP_ROUTE5, 0, 12, DIR_RIGHT}, {0, 6, MAP_LUMEN, 23, 14, DIR_LEFT}, {0, 7, MAP_LUMEN, 23, 15, DIR_LEFT}, {0, 8, MAP_LUMEN, 23, 16, DIR_LEFT}, {0, 9, MAP_LUMEN, 23, 17, DIR_LEFT}, {0, 10, MAP_LUMEN, 23, 18, DIR_LEFT}};
static const Npc N6_route7[] = {
  { .x = 11, .y = 6, .dir = DIR_DOWN, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = L6_swim1_in, .alt = L6_swim1_after, .tr = &T6_swim1 },
  { .x = 17, .y = 10, .dir = DIR_LEFT, .look = LOOK_SAILOR, .flags = NF_TRAINER, .say = L6_sailor_in, .alt = L6_sailor_after, .tr = &T6_sailor },
  { .x = 22, .y = 11, .dir = DIR_LEFT, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = L6_swim2_in, .alt = L6_swim2_after, .tr = &T6_swim2 },
  { .x = 16, .y = 9, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 2, .flag = FLAG_GOT_L7BALL }
};
static const Sign S6_route7[] = {{24, 4, "ROUTE 7. West: LUMEN ISLE. Steer by the lighthouse."}};
static const Warp W6_lumen[] = {{23, 14, MAP_ROUTE7, 0, 6, DIR_RIGHT}, {23, 15, MAP_ROUTE7, 0, 7, DIR_RIGHT}, {23, 16, MAP_ROUTE7, 0, 8, DIR_RIGHT}, {23, 17, MAP_ROUTE7, 0, 9, DIR_RIGHT}, {23, 18, MAP_ROUTE7, 0, 10, DIR_RIGHT}, {7, 9, MAP_REST_LUMEN, 4, 6, DIR_UP}, {15, 9, MAP_MART_LUMEN, 4, 6, DIR_UP}, {12, 16, MAP_GYM6, 5, 12, DIR_UP}, {11, 4, MAP_LIGHTHOUSE1, 6, 10, DIR_UP}, {0, 0, MAP_ROUTE8, 0, 27, DIR_UP}, {1, 0, MAP_ROUTE8, 1, 27, DIR_UP}, {2, 0, MAP_ROUTE8, 2, 27, DIR_UP}, {3, 0, MAP_ROUTE8, 3, 27, DIR_UP}, {4, 0, MAP_ROUTE8, 4, 27, DIR_UP}, {5, 0, MAP_ROUTE8, 5, 27, DIR_UP}, {6, 0, MAP_ROUTE8, 6, 27, DIR_UP}, {7, 0, MAP_ROUTE8, 7, 27, DIR_UP}, {8, 0, MAP_ROUTE8, 8, 27, DIR_UP}, {9, 0, MAP_ROUTE8, 9, 27, DIR_UP}, {10, 0, MAP_ROUTE8, 10, 27, DIR_UP}, {11, 0, MAP_ROUTE8, 11, 27, DIR_UP}, {12, 0, MAP_ROUTE8, 12, 27, DIR_UP}, {13, 0, MAP_ROUTE8, 13, 27, DIR_UP}, {14, 0, MAP_ROUTE8, 14, 27, DIR_UP}, {15, 0, MAP_ROUTE8, 15, 27, DIR_UP}, {16, 0, MAP_ROUTE8, 16, 27, DIR_UP}, {17, 0, MAP_ROUTE8, 17, 27, DIR_UP}, {18, 0, MAP_ROUTE8, 18, 27, DIR_UP}, {19, 0, MAP_ROUTE8, 19, 27, DIR_UP}, {20, 0, MAP_ROUTE8, 20, 27, DIR_UP}, {21, 0, MAP_ROUTE8, 21, 27, DIR_UP}, {22, 0, MAP_ROUTE8, 22, 27, DIR_UP}, {23, 0, MAP_ROUTE8, 23, 27, DIR_UP}};
static const Npc N6_lumen[] = {
  { .x = 17, .y = 15, .dir = DIR_DOWN, .look = LOOK_MAYA, .flags = NF_TRAINER | NF_SHOW, .say = L6_maya_in, .alt = L6_maya_after, .tr = &T6_maya, .cond = C6_maya },
  { .x = 12, .y = 17, .dir = DIR_UP, .look = LOOK_SAILOR, .flags = NF_SHOW, .say = L6_guard, .cond = C6_dark },
  { .x = 4, .y = 11, .dir = DIR_RIGHT, .look = LOOK_LASS, .flags = 0, .say = L6_lass },
  { .x = 8, .y = 5, .dir = DIR_DOWN, .look = LOOK_OLD, .flags = NF_WANDER, .say = L6_old }
};
static const Sign S6_lumen[] = {{19, 10, "LUMEN ISLE. The light that guides ships home."}, {5, 15, "LUMEN GYM. LEADER: SABLE. The mind sees what the eye cannot."}};
static const Warp W6_rest_lumen[] = {{4, 7, MAP_LUMEN, 7, 10, DIR_DOWN}, {5, 7, MAP_LUMEN, 7, 10, DIR_DOWN}};
static const Npc N6_rest_lumen[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_NURSE, .flags = NF_HEAL },
  { .x = 8, .y = 1, .dir = DIR_DOWN, .look = LOOK_PC, .flags = NF_PC }
};
static const Sign S6_rest_lumen[] = {{0}};
static const Warp W6_mart_lumen[] = {{4, 7, MAP_LUMEN, 15, 10, DIR_DOWN}, {5, 7, MAP_LUMEN, 15, 10, DIR_DOWN}};
static const Npc N6_mart_lumen[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_SHOP },
  { .x = 7, .y = 6, .dir = DIR_UP, .look = LOOK_LASS, .flags = 0, .say = L6_shopper }
};
static const Sign S6_mart_lumen[] = {{0}};
static const Warp W6_lighthouse1[] = {{6, 11, MAP_LUMEN, 11, 5, DIR_DOWN}, {7, 11, MAP_LUMEN, 11, 5, DIR_DOWN}, {11, 1, MAP_LIGHTHOUSE2, 2, 1, DIR_RIGHT}};
static const Npc N6_lighthouse1[] = {
  { .x = 11, .y = 5, .dir = DIR_LEFT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L6_g1_in, .alt = L6_g1_after, .tr = &T6_g1 },
  { .x = 3, .y = 9, .dir = DIR_RIGHT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L6_g2_in, .alt = L6_g2_after, .tr = &T6_g2 },
  { .x = 12, .y = 9, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_FULLHEAL, .count = 2, .flag = FLAG_GOT_LHFULL }
};
static const Sign S6_lighthouse1[] = {{0}};
static const Warp W6_lighthouse2[] = {{1, 1, MAP_LIGHTHOUSE1, 10, 1, DIR_LEFT}};
static const Npc N6_lighthouse2[] = {
  { .x = 10, .y = 7, .dir = DIR_LEFT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L6_g3_in, .alt = L6_g3_after, .tr = &T6_g3 },
  { .x = 6, .y = 5, .dir = DIR_DOWN, .look = LOOK_EMBER, .flags = NF_TRAINER, .say = L6_ember_in, .alt = L6_ember_after, .tr = &T6_ember },
  { .x = 6, .y = 3, .dir = DIR_DOWN, .look = LOOK_KEEPER, .flags = NF_SHOW, .say = L6_keeper_help, .cond = C6_dark },
  { .x = 6, .y = 3, .dir = DIR_DOWN, .look = LOOK_KEEPER, .flags = NF_SHOW | NF_GIFT, .item = ITEM_ULTRA, .count = 3, .flag = FLAG_KEEPERGIFT, .say = L6_keeper_before, .alt = L6_keeper_after, .cond = C6_lit },
  { .x = 12, .y = 10, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_TM12, .count = 1, .flag = FLAG_GOT_LHTM }
};
static const Sign S6_lighthouse2[] = {{0}};
static const Warp W6_gym6[] = {{5, 13, MAP_LUMEN, 12, 17, DIR_DOWN}, {6, 13, MAP_LUMEN, 12, 17, DIR_DOWN}};
static const Npc N6_gym6[] = {
  { .x = 8, .y = 12, .dir = DIR_LEFT, .look = LOOK_OLD, .flags = 0, .say = L6_gguide, .alt = L6_gguide2, .flag = FLAG_BADGE_MIND },
  { .x = 2, .y = 9, .dir = DIR_RIGHT, .look = LOOK_PSYCHIC, .flags = NF_TRAINER, .say = L6_psy1_in, .alt = L6_psy1_after, .tr = &T6_psy1 },
  { .x = 9, .y = 6, .dir = DIR_LEFT, .look = LOOK_PSYCHIC, .flags = NF_TRAINER, .say = L6_psy2_in, .alt = L6_psy2_after, .tr = &T6_psy2 },
  { .x = 5, .y = 2, .dir = DIR_DOWN, .look = LOOK_SABLE, .flags = NF_TRAINER, .say = L6_sable_in, .alt = L6_sable_after, .tr = &T6_sable }
};
static const Sign S6_gym6[] = {{0}};
static const unsigned char K6_mart_lumen[] = {ITEM_ULTRA, ITEM_SUPER, ITEM_FULLHEAL, ITEM_REVIVE, ITEM_REPEL, ITEM_MIRACLE};
static const unsigned char K6_rest[] = {ITEM_BALL, ITEM_POTION};

// ==================== Chapter 7: ROUTE 8 (the snow pass north of LUMEN ISLE), FROSTGATE ====================
static const char *const L7_ski1_in[] = {"SKIER: Fresh powder all the way up to FROSTGATE! Race you!", 0};
static const char *const L7_ski1_after[] = {"SKIER: SNOWL nest in the firs. You hear them hoot before you see them.", 0};
static const char *const L7_ski2_in[] = {"SKIER: My goggles fogged up. Is that a trainer? Let's go!", 0};
static const char *const L7_ski2_after[] = {"SKIER: Men in grey coats skied past at dawn, heading north for the ICE CAVE.", 0};
static const char *const L7_hiker_in[] = {"HIKER: The mountain does not care how strong you are. But I do!", 0};
static const char *const L7_hiker_after[] = {"HIKER: That boulder on the beach keeps the tourists out. Good arms, kid.", 0};
static const char *const L7_nguard[] = {"The ICE CAVE lies north. The floor is pure ice: once you slide, you only stop at a rock.", "LEADER BOREAL went up there after the grey coats. She said someone had to.", 0};
static const char *const L7_gguard[] = {"The GYM is closed. LEADER BOREAL is up at the ICE CAVE.", "Men in grey coats came through yesterday, asking the way to the shrine under the sea.", 0};
static const char *const L7_old[] = {"FROSTGATE sits at the top of the old ice road.", "Under the ice, they say, the sea stays warm, and something sleeps there.", 0};
static const char *const L7_kid[] = {"If you hear hooting on ROUTE 8, look up! SNOWL sit in the firs.", 0};
static const char *const L7_shopper[] = {"ICE HEALS sell out every winter. You're lucky!", 0};
static const unsigned short C7_boreal[] = {FLAG_BOREAL_MET | 0x8000, 0};
static const unsigned char TP7_ski1[] = {76,39,74,40};
static const Trainer T7_ski1 = {"SKIER NILS", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R8SKI1, 0, 0, TP7_ski1, L7_ski1_in, L7_ski1_after, 0, "Wiped out!"};
static const unsigned char TP7_ski2[] = {74,40,77,41};
static const Trainer T7_ski2 = {"SKIER ELKE", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R8SKI2, 0, 0, TP7_ski2, L7_ski2_in, L7_ski2_after, 0, "Brr... you're good."};
static const unsigned char TP7_hiker[] = {45,40,52,41};
static const Trainer T7_hiker = {"HIKER ODD", 3, 48, 2, 1, 0, 255, 0, FLAG_BEAT_R8HIKER, 0, 0, TP7_hiker, L7_hiker_in, L7_hiker_after, 0, "Avalanche!"};
static const Warp W7_route8[] = {{11, 0, MAP_FROSTGATE, 11, 18, DIR_UP}, {12, 0, MAP_FROSTGATE, 12, 18, DIR_UP}, {0, 27, MAP_LUMEN, 0, 0, DIR_DOWN}, {1, 27, MAP_LUMEN, 1, 0, DIR_DOWN}, {2, 27, MAP_LUMEN, 2, 0, DIR_DOWN}, {3, 27, MAP_LUMEN, 3, 0, DIR_DOWN}, {4, 27, MAP_LUMEN, 4, 0, DIR_DOWN}, {5, 27, MAP_LUMEN, 5, 0, DIR_DOWN}, {6, 27, MAP_LUMEN, 6, 0, DIR_DOWN}, {7, 27, MAP_LUMEN, 7, 0, DIR_DOWN}, {8, 27, MAP_LUMEN, 8, 0, DIR_DOWN}, {9, 27, MAP_LUMEN, 9, 0, DIR_DOWN}, {10, 27, MAP_LUMEN, 10, 0, DIR_DOWN}, {11, 27, MAP_LUMEN, 11, 0, DIR_DOWN}, {12, 27, MAP_LUMEN, 12, 0, DIR_DOWN}, {13, 27, MAP_LUMEN, 13, 0, DIR_DOWN}, {14, 27, MAP_LUMEN, 14, 0, DIR_DOWN}, {15, 27, MAP_LUMEN, 15, 0, DIR_DOWN}, {16, 27, MAP_LUMEN, 16, 0, DIR_DOWN}, {17, 27, MAP_LUMEN, 17, 0, DIR_DOWN}, {18, 27, MAP_LUMEN, 18, 0, DIR_DOWN}, {19, 27, MAP_LUMEN, 19, 0, DIR_DOWN}, {20, 27, MAP_LUMEN, 20, 0, DIR_DOWN}, {21, 27, MAP_LUMEN, 21, 0, DIR_DOWN}, {22, 27, MAP_LUMEN, 22, 0, DIR_DOWN}, {23, 27, MAP_LUMEN, 23, 0, DIR_DOWN}};
static const Npc N7_route8[] = {
  { .x = 6, .y = 12, .dir = DIR_RIGHT, .look = LOOK_SKIER, .flags = NF_TRAINER, .say = L7_ski1_in, .alt = L7_ski1_after, .tr = &T7_ski1 },
  { .x = 17, .y = 7, .dir = DIR_LEFT, .look = LOOK_SKIER, .flags = NF_TRAINER, .say = L7_ski2_in, .alt = L7_ski2_after, .tr = &T7_ski2 },
  { .x = 9, .y = 3, .dir = DIR_DOWN, .look = LOOK_HIKER, .flags = NF_TRAINER, .say = L7_hiker_in, .alt = L7_hiker_after, .tr = &T7_hiker },
  { .x = 4, .y = 5, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 2, .flag = FLAG_GOT_R8BALL }
};
static const Sign S7_route8[] = {{16, 20, "ROUTE 8. North: FROSTGATE. Mind the drifts."}};
static const Warp W7_frost[] = {{11, 0, MAP_ICECAVE, 9, 14, DIR_UP}, {11, 19, MAP_ROUTE8, 11, 1, DIR_DOWN}, {12, 19, MAP_ROUTE8, 12, 1, DIR_DOWN}, {6, 6, MAP_REST_FROST, 4, 6, DIR_UP}, {16, 6, MAP_MART_FROST, 4, 6, DIR_UP}, {11, 12, MAP_GYM7, 6, 14, DIR_UP}, {0, 7, MAP_ROUTE9, 22, 7, DIR_LEFT}, {0, 8, MAP_ROUTE9, 22, 8, DIR_LEFT}};
static const Npc N7_frost[] = {
  { .x = 12, .y = 2, .dir = DIR_LEFT, .look = LOOK_OLD, .flags = 0, .say = L7_nguard },
  { .x = 11, .y = 13, .dir = DIR_DOWN, .look = LOOK_SAILOR, .flags = NF_SHOW, .say = L7_gguard, .cond = C7_boreal },
  { .x = 4, .y = 9, .dir = DIR_DOWN, .look = LOOK_OLD, .flags = NF_WANDER, .say = L7_old },
  { .x = 19, .y = 8, .dir = DIR_LEFT, .look = LOOK_BOY, .flags = NF_WANDER, .say = L7_kid }
};
static const Sign S7_frost[] = {{14, 15, "FROSTGATE. The town at the top of the ice road."}, {9, 14, "FROSTGATE GYM. LEADER: BOREAL. Cold hands, warm heart."}};
static const Warp W7_rest_frost[] = {{4, 7, MAP_FROSTGATE, 6, 7, DIR_DOWN}, {5, 7, MAP_FROSTGATE, 6, 7, DIR_DOWN}};
static const Npc N7_rest_frost[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_NURSE, .flags = NF_HEAL },
  { .x = 8, .y = 1, .dir = DIR_DOWN, .look = LOOK_PC, .flags = NF_PC }
};
static const Sign S7_rest_frost[] = {{0}};
static const Warp W7_mart_frost[] = {{4, 7, MAP_FROSTGATE, 16, 7, DIR_DOWN}, {5, 7, MAP_FROSTGATE, 16, 7, DIR_DOWN}};
static const Npc N7_mart_frost[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_SHOP },
  { .x = 7, .y = 6, .dir = DIR_UP, .look = LOOK_LASS, .flags = 0, .say = L7_shopper }
};
static const Sign S7_mart_frost[] = {{0}};
// ICE CAVE north of FROSTGATE: an ice field you slide across (stop against rocks), the ASH CIRCLE searching, BOREAL
static const char *const L7_ig1_in[] = {"ASH GRUNT: The boss said to search every cave for the shrine. This one is just cold!", 0};
static const char *const L7_ig1_after[] = {"ASH GRUNT: Fine, it's not here. The others went down the frozen bay.", 0};
static const char *const L7_ig2_in[] = {"ASH GRUNT: You again?! CINDRA is going to hear about this!", 0};
static const char *const L7_ig2_after[] = {"ASH GRUNT: The sea sleeper is ours. The shrine opens when the tide goes out.", 0};
static const char *const L7_boreal_meet[] = {"BOREAL: You crossed the ice on your own? Not bad at all.", "BOREAL: The ASH CIRCLE came through here, but the shrine is not in this cave. They went down the frozen bay, to the sea.", "BOREAL: Come to my GYM. I will show you what the cold can teach. Take these for the way back.", 0};
static const char *const L7_boreal_after[] = {"BOREAL: My GYM is open. Mind the ice!", 0};
static const unsigned char TP7_ig1[] = {86,39,43,40};
static const Trainer T7_ig1 = {"ASH GRUNT", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_ICEGRUNT1, 0, 0, TP7_ig1, L7_ig1_in, L7_ig1_after, 0, "Brr... and I lost."};
static const unsigned char TP7_ig2[] = {80,40,87,41};
static const Trainer T7_ig2 = {"ASH GRUNT", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_ICEGRUNT2, 0, 0, TP7_ig2, L7_ig2_in, L7_ig2_after, 0, "Frozen out!"};
static const Warp W7_icecave[] = {{9, 15, MAP_FROSTGATE, 11, 1, DIR_DOWN}, {10, 15, MAP_FROSTGATE, 11, 1, DIR_DOWN}};
static const Npc N7_icecave[] = {
  { .x = 15, .y = 13, .dir = DIR_LEFT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L7_ig1_in, .alt = L7_ig1_after, .tr = &T7_ig1 },
  { .x = 13, .y = 2, .dir = DIR_LEFT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L7_ig2_in, .alt = L7_ig2_after, .tr = &T7_ig2 },
  { .x = 3, .y = 1, .dir = DIR_DOWN, .look = LOOK_BOREAL, .flags = NF_SHOW | NF_GIFT, .item = ITEM_FULLHEAL, .count = 2, .flag = FLAG_BOREAL_MET, .say = L7_boreal_meet, .alt = L7_boreal_after, .cond = C7_boreal },
  { .x = 16, .y = 1, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_REVIVE, .count = 2, .flag = FLAG_GOT_ICEITEM }
};
static const Sign S7_icecave[] = {{0}};
// GYM 7: BOREAL's ice floor (slide and stop at the ice blocks), two skiers on islands, the FROST BADGE
static const char *const L7_gguide[] = {"BOREAL's floor is pure ice. You slide until something stops you.", "Fire and fighting moves melt her team. Good luck, champ-to-be!", 0};
static const char *const L7_gguide2[] = {"The FROST BADGE! Strong creatures will follow you anywhere now.", 0};
static const char *const L7_gs1_in[] = {"SKIER INGA: I've been stuck on this island since breakfast. Battle me!", 0};
static const char *const L7_gs1_after[] = {"SKIER INGA: Slide up from the middle. The blocks catch you.", 0};
static const char *const L7_gs2_in[] = {"SKIER TORV: BOREAL trains us on this floor. Balance is everything!", 0};
static const char *const L7_gs2_after[] = {"SKIER TORV: I slipped. On the ice and in battle.", 0};
