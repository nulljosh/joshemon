static const char *const L7_bor_in[] = {"BOREAL: So you made it across my floor.", "BOREAL: Cold teaches patience. Let us see what you have learned!", 0};
static const char *const L7_bor_win[] = {"BOREAL: Warm work. You earned this.", "JOSH received the FROST BADGE from BOREAL!", "BOREAL: Now strong creatures will follow you anywhere.", 0};
static const char *const L7_bor_after[] = {"BOREAL: The ASH CIRCLE went down the frozen bay to the sunken shrine.", "BOREAL: If they wake the sea sleeper, the ice will not hold. Go, JOSH.", 0};
static const unsigned char TP7_gs1[] = {75,41,77,41};
static const Trainer T7_gs1 = {"SKIER INGA", 4, 44, 2, 1, 0, 255, 0, FLAG_BEAT_GYMSKI1, 0, 0, TP7_gs1, L7_gs1_in, L7_gs1_after, 0, "Off the edge!"};
static const unsigned char TP7_gs2[] = {76,41,74,42};
static const Trainer T7_gs2 = {"SKIER TORV", 4, 44, 2, 1, 0, 255, 0, FLAG_BEAT_GYMSKI2, 0, 0, TP7_gs2, L7_gs2_in, L7_gs2_after, 0, "Too slippery!"};
static const unsigned char TP7_bor[] = {74,42,77,43,75,45};
static const Trainer T7_bor = {"LEADER BOREAL", 0, 110, 3, 1, 7, ITEM_TM16, 0, FLAG_BEAT_BOREAL, 0, 0, TP7_bor, L7_bor_in, L7_bor_after, L7_bor_win, "The thaw comes for everyone."};
static const Warp W7_gym7[] = {{6, 15, MAP_FROSTGATE, 11, 13, DIR_DOWN}, {7, 15, MAP_FROSTGATE, 11, 13, DIR_DOWN}};
static const Npc N7_gym7[] = {
  { .x = 9, .y = 13, .dir = DIR_LEFT, .look = LOOK_OLD, .flags = 0, .say = L7_gguide, .alt = L7_gguide2, .flag = FLAG_BADGE_FROST },
  { .x = 0, .y = 9, .dir = DIR_RIGHT, .look = LOOK_SKIER, .flags = NF_TRAINER, .say = L7_gs1_in, .alt = L7_gs1_after, .tr = &T7_gs1 },
  { .x = 13, .y = 11, .dir = DIR_LEFT, .look = LOOK_SKIER, .flags = NF_TRAINER, .say = L7_gs2_in, .alt = L7_gs2_after, .tr = &T7_gs2 },
  { .x = 6, .y = 2, .dir = DIR_DOWN, .look = LOOK_BOREAL, .flags = NF_TRAINER, .say = L7_bor_in, .alt = L7_bor_after, .tr = &T7_bor }
};
static const Sign S7_gym7[] = {{0}};
static const unsigned char K7_mart_frost[] = {ITEM_ULTRA, ITEM_SUPER, ITEM_HYPER, ITEM_ICEHEAL, ITEM_FULLHEAL, ITEM_REVIVE, ITEM_REPEL};

// ==================== Chapter 8: FROZEN BAY (ROUTE 9, west of FROSTGATE), the SUNKEN SHRINE, CINDRA ====================
static const char *const L8_ski_in[] = {"SKIER: The bay freezes at the edges. Don't try it on skis!", 0};
static const char *const L8_ski_after[] = {"SKIER: GLACIELK cross the deep snow here at dusk. Antlers made of ice!", 0};
static const char *const L8_g1_in[] = {"ASH GRUNT: Nobody goes down to the bay. CINDRA's orders!", 0};
static const char *const L8_g1_after[] = {"ASH GRUNT: The shrine is on the rock island out west. You'd have to SURF.", 0};
static const char *const L8_fish_in[] = {"FISHER: Ice fishing! Hold still, you'll scare the bites!", 0};
static const char *const L8_fish_after[] = {"FISHER: Something big swims under the shrine island. My line snapped twice.", 0};
static const char *const L8_swim_in[] = {"SWIMMER: Cold water? Good for the heart! Let's battle!", 0};
static const char *const L8_swim_after[] = {"SWIMMER: The current runs out of the shrine, like the place is breathing.", 0};
static const char *const L8_g2_in[] = {"ASH GRUNT: Lookout duty on an ice rock. Worst job in the CIRCLE.", 0};
static const char *const L8_g2_after[] = {"ASH GRUNT: CINDRA went in an hour ago. Something down there is breathing.", 0};
static const unsigned char TP8_ski[] = {75,40,76,41};
static const Trainer T8_ski = {"SKIER LISE", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R9SKI, 0, 0, TP8_ski, L8_ski_in, L8_ski_after, 0, "Off-piste!"};
static const unsigned char TP8_g1[] = {80,41,85,42};
static const Trainer T8_g1 = {"ASH GRUNT", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_BAYGRUNT1, 0, 0, TP8_g1, L8_g1_in, L8_g1_after, 0, "CINDRA won't like this."};
static const unsigned char TP8_fish[] = {32,39,104,40,33,41};
static const Trainer T8_fish = {"FISHER BRAM", 1, 40, 3, 1, 0, 255, 0, FLAG_BEAT_R9FISH, 0, 0, TP8_fish, L8_fish_in, L8_fish_after, 0, "My line!"};
static const unsigned char TP8_swim[] = {28,40,29,41};
static const Trainer T8_swim = {"SWIMMER UNA", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R9SWIM, 0, 0, TP8_swim, L8_swim_in, L8_swim_after, 0, "Brr!"};
static const unsigned char TP8_g2[] = {43,42,81,42};
static const Trainer T8_g2 = {"ASH GRUNT", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_BAYGRUNT2, 0, 0, TP8_g2, L8_g2_in, L8_g2_after, 0, "Off the rock I go..."};
static const Warp W8_route9[] = {{23, 7, MAP_FROSTGATE, 1, 7, DIR_RIGHT}, {23, 8, MAP_FROSTGATE, 1, 8, DIR_RIGHT}, {6, 19, MAP_SHRINE, 9, 16, DIR_UP}, {21, 23, MAP_DRAKE, 11, 1, DIR_DOWN}};
static const char *const L8_thal[] = {"Rain lashes the bay. The water heaves...", "THALASSOR, the sea sleeper, rises from the waves!", 0};
static const char *const L8_thal_after[] = {"Far to the south, the clouds part over the peaks of DRAKESPIRE.", 0};
static const unsigned short C8_thal[] = {FLAG_BEAT_CINDRA, FLAG_THAL_CAUGHT | 0x8000, 0}; // FLAG_THAL_CAUGHT must follow FLAG_SEA_CALM
static const char *const L8_storm[] = {"SWIMMER: The storm out past the reef is wild! Nobody gets through until the sea calms down.", 0};
static const unsigned short C8_storm[] = {FLAG_SEA_CALM | 0x8000, 0};
static const Npc N8_route9[] = {
  { .x = 6, .y = 9, .dir = DIR_RIGHT, .look = LOOK_SKIER, .flags = NF_TRAINER, .say = L8_ski_in, .alt = L8_ski_after, .tr = &T8_ski },
  { .x = 15, .y = 10, .dir = DIR_UP, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L8_g1_in, .alt = L8_g1_after, .tr = &T8_g1 },
  { .x = 18, .y = 13, .dir = DIR_LEFT, .look = LOOK_FISHER, .flags = NF_TRAINER, .say = L8_fish_in, .alt = L8_fish_after, .tr = &T8_fish },
  { .x = 12, .y = 15, .dir = DIR_DOWN, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = L8_swim_in, .alt = L8_swim_after, .tr = &T8_swim },
  { .x = 8, .y = 20, .dir = DIR_RIGHT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L8_g2_in, .alt = L8_g2_after, .tr = &T8_g2 },
  { .x = 11, .y = 3, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 3, .flag = FLAG_GOT_R9BALL },
  { .x = 15, .y = 21, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .flag = FLAG_SEA_CALM, .say = L8_thal, .alt = L8_thal_after, .cond = C8_thal, .legend = 106, .lv = 50 },
  { .x = 21, .y = 22, .dir = DIR_UP, .look = LOOK_SWIMMER, .flags = NF_SHOW, .say = L8_storm, .cond = C8_storm }
};
static const Sign S8_route9[] = {{18, 7, "FROZEN BAY. West: the open sea. East: FROSTGATE."}};
// SUNKEN SHRINE: a flooded hall you SURF across, EMBER at the steps, CINDRA at the sleeper's pool
static const char *const L8_s1_in[] = {"ASH GRUNT: This place drips. And it hums. Do you hear it humming?", 0};
static const char *const L8_s1_after[] = {"ASH GRUNT: Water up to my knees and a sleeper underneath. I want a raise.", 0};
static const char *const L8_s2_in[] = {"ASH GRUNT: You SURFED in here? Bold. Dumb, but bold!", 0};
static const char *const L8_s2_after[] = {"ASH GRUNT: Up the steps. The boss is at the pool.", 0};
static const char *const L8_em_in[] = {"EMBER: You again! CINDRA is at the pool. She is not to be disturbed.", "EMBER: Third time pays for all. Burn!", 0};
static const char *const L8_em_after[] = {"EMBER: Go on, then. See for yourself. You are already too late.", 0};
static const char *const L8_cin_in[] = {"CINDRA: So. The child from the mountain.", "CINDRA: I am CINDRA. I lead the ASH CIRCLE. Fire made this land, and fire will make it new.", "CINDRA: The sleeper under the mountain answered you. This one will answer me.", "CINDRA: Show me what you are!", 0};
static const char *const L8_cin_win[] = {"CINDRA: Hm. Stronger than EMBER said.", "CINDRA: But listen. You are too late.", "The pool behind CINDRA begins to churn...", "Something huge rises from the deep, then dives away through the sea gate and out to the open water!", "CINDRA: The sea sleeper is awake. Wherever it swims, storms follow.", "CINDRA: Chase it if you like. The ASH CIRCLE will be waiting.", "CINDRA vanished into the steam!", 0};
static const unsigned short C8_cindra[] = {FLAG_BEAT_CINDRA | 0x8000, 0};
static const unsigned char TP8_s1[] = {85,42,87,43};
static const Trainer T8_s1 = {"ASH GRUNT", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_SHGRUNT1, 0, 0, TP8_s1, L8_s1_in, L8_s1_after, 0, "Soaked and beaten."};
static const unsigned char TP8_s2[] = {60,42,83,43};
static const Trainer T8_s2 = {"ASH GRUNT", 5, 60, 2, 1, 0, 255, 0, FLAG_BEAT_SHGRUNT2, 0, 0, TP8_s2, L8_s2_in, L8_s2_after, 0, "Washed out!"};
static const unsigned char TP8_em[] = {87,43,61,44,43,44};
static const Trainer T8_em = {"ADMIN EMBER", 3, 160, 3, 1, 0, 255, 0, FLAG_BEAT_EMBER3, 0, 0, TP8_em, L8_em_in, L8_em_after, 0, "Not... again!"};
static const unsigned char TP8_cin[] = {87,45,101,45,81,46,61,48};
static const Trainer T8_cin = {"ASH LEADER CINDRA", 0, 200, 4, 1, 0, 255, 0, FLAG_BEAT_CINDRA, 0, 0, TP8_cin, L8_cin_in, L8_cin_in, L8_cin_win, "Impressive. Too late."};
static const Warp W8_shrine[] = {{9, 17, MAP_ROUTE9, 6, 20, DIR_DOWN}, {10, 17, MAP_ROUTE9, 6, 20, DIR_DOWN}};
static const Npc N8_shrine[] = {
  { .x = 13, .y = 14, .dir = DIR_LEFT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L8_s1_in, .alt = L8_s1_after, .tr = &T8_s1 },
  { .x = 4, .y = 7, .dir = DIR_RIGHT, .look = LOOK_GRUNT, .flags = NF_TRAINER, .say = L8_s2_in, .alt = L8_s2_after, .tr = &T8_s2 },
  { .x = 10, .y = 5, .dir = DIR_LEFT, .look = LOOK_EMBER, .flags = NF_TRAINER, .say = L8_em_in, .alt = L8_em_after, .tr = &T8_em },
  { .x = 9, .y = 3, .dir = DIR_UP, .look = LOOK_CINDRA, .flags = NF_TRAINER, .say = L8_cin_in, .alt = L8_cin_in, .tr = &T8_cin, .cond = C8_cindra },
  { .x = 16, .y = 3, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_REVIVE, .count = 1, .flag = FLAG_GOT_SHITEM }
};
static const Sign S8_shrine[] = {{4, 2, "Old letters in the stone: WHILE THE TIDE SLEEPS, THE DEEP ONE DREAMS. WHEN IT WAKES, THE SEA RISES."}};
// DRAKESPIRE: the town under the dragon peaks, south over the calmed sea. GYM 8 LEADER WYRMOND, the DRAGON BADGE
static const char *const L8_dguard[] = {"The cave behind me is VICTORY ROAD. It leads to the JOSHEMON LEAGUE.", "Only trainers with all eight badges may pass.", 0};
static const char *const L8_dold[] = {"Dragons nest up on the peaks. On clear days they circle the town.", "The storm kept them hidden for days. Somebody calmed the sea, I hear.", 0};
static const char *const L8_dkid[] = {"WYRMOND is the oldest trainer in the whole region! He rode a dragon here when he was ten.", 0};
static const char *const L8_dshop[] = {"Heading for VICTORY ROAD? Buy REVIVES. Lots of them.", 0};
static const char *const L8_gguide[] = {"WYRMOND's dragons guard his hall. Push the statues aside with STRENGTH!", "Dragons fear ice. And other dragons. Good luck!", 0};
static const char *const L8_gguide2[] = {"The DRAGON BADGE! That makes eight. VICTORY ROAD awaits!", 0};
static const char *const L8_t1_in[] = {"DRAGON TAMER: My dragons hatched on the peaks above town. Behold!", 0};
static const char *const L8_t1_after[] = {"DRAGON TAMER: The statues are heavy. Good thing your creature is strong.", 0};
static const char *const L8_t2_in[] = {"DRAGON TAMER: Only a steady heart reaches WYRMOND!", 0};
static const char *const L8_t2_after[] = {"DRAGON TAMER: You have one. Go on.", 0};
static const char *const L8_wyr_in[] = {"WYRMOND: Hm? A visitor in my hall, with seven badges in the case.", "WYRMOND: I have flown with dragons for sixty years. They respect one thing: a steady heart.", "WYRMOND: Show me yours!", 0};
static const char *const L8_wyr_win[] = {"WYRMOND: Ha! The dragons bow to you, young one.", "JOSH received the DRAGON BADGE from WYRMOND!", "WYRMOND: Eight badges. The JOSHEMON LEAGUE lies south, through VICTORY ROAD.", 0};
static const char *const L8_wyr_after[] = {"WYRMOND: VICTORY ROAD starts at the cave south of town. Go well, and keep your heart steady.", 0};
static const unsigned short C8_vguard[] = {FLAG_BADGE_DRAGON | 0x8000, 0};
static const unsigned char TP8_t1[] = {88,44,89,45};
static const Trainer T8_t1 = {"DRAGON TAMER IVO", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_TAMER1, 0, 0, TP8_t1, L8_t1_in, L8_t1_after, 0, "My dragons..."};
static const unsigned char TP8_t2[] = {89,45,69,45};
static const Trainer T8_t2 = {"DRAGON TAMER RHEA", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_TAMER2, 0, 0, TP8_t2, L8_t2_in, L8_t2_after, 0, "Steady... steady..."};
static const unsigned char TP8_wyr[] = {89,47,101,47,69,48,90,50};
static const Trainer T8_wyr = {"LEADER WYRMOND", 0, 120, 4, 1, 8, ITEM_TM17, 0, FLAG_BEAT_WYRMOND, 0, 0, TP8_wyr, L8_wyr_in, L8_wyr_after, L8_wyr_win, "The old dragon rests."};
static const Warp W8_drake[] = {{10, 0, MAP_ROUTE9, 21, 22, DIR_UP}, {11, 0, MAP_ROUTE9, 21, 22, DIR_UP}, {12, 0, MAP_ROUTE9, 21, 22, DIR_UP}, {13, 0, MAP_ROUTE9, 21, 22, DIR_UP}, {6, 8, MAP_REST_DRAKE, 4, 6, DIR_UP}, {17, 8, MAP_MART_DRAKE, 4, 6, DIR_UP}, {12, 15, MAP_GYM8, 6, 14, DIR_UP}, {12, 20, MAP_VICTORY1, 12, 1, DIR_DOWN}};
static const Npc N8_drake[] = {
  { .x = 12, .y = 19, .dir = DIR_DOWN, .look = LOOK_SAILOR, .flags = NF_SHOW, .say = L8_dguard, .cond = C8_vguard },
  { .x = 3, .y = 10, .dir = DIR_DOWN, .look = LOOK_OLD, .flags = NF_WANDER, .say = L8_dold },
  { .x = 19, .y = 12, .dir = DIR_LEFT, .look = LOOK_BOY, .flags = NF_WANDER, .say = L8_dkid }
};
static const Sign S8_drake[] = {{8, 9, "DRAKESPIRE. The town under the dragon peaks."}, {4, 14, "DRAKESPIRE GYM. LEADER: WYRMOND. The oldest dragon master."}};
static const Warp W8_rest_drake[] = {{4, 7, MAP_DRAKE, 6, 9, DIR_DOWN}, {5, 7, MAP_DRAKE, 6, 9, DIR_DOWN}};
static const Npc N8_rest_drake[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_NURSE, .flags = NF_HEAL },
  { .x = 8, .y = 1, .dir = DIR_DOWN, .look = LOOK_PC, .flags = NF_PC }
};
static const Sign S8_rest_drake[] = {{0}};
static const Warp W8_mart_drake[] = {{4, 7, MAP_DRAKE, 17, 9, DIR_DOWN}, {5, 7, MAP_DRAKE, 17, 9, DIR_DOWN}};
static const Npc N8_mart_drake[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_SHOP },
  { .x = 7, .y = 6, .dir = DIR_UP, .look = LOOK_HIKER, .flags = 0, .say = L8_dshop }
};
static const Sign S8_mart_drake[] = {{0}};
static const Warp W8_gym8[] = {{6, 15, MAP_DRAKE, 12, 16, DIR_DOWN}, {7, 15, MAP_DRAKE, 12, 16, DIR_DOWN}};
static const Npc N8_gym8[] = {
  { .x = 9, .y = 13, .dir = DIR_LEFT, .look = LOOK_OLD, .flags = 0, .say = L8_gguide, .alt = L8_gguide2, .flag = FLAG_BADGE_DRAGON },
  { .x = 2, .y = 12, .dir = DIR_RIGHT, .look = LOOK_TAMER, .flags = NF_TRAINER, .say = L8_t1_in, .alt = L8_t1_after, .tr = &T8_t1 },
  { .x = 2, .y = 6, .dir = DIR_RIGHT, .look = LOOK_TAMER, .flags = NF_TRAINER, .say = L8_t2_in, .alt = L8_t2_after, .tr = &T8_t2 },
  { .x = 7, .y = 2, .dir = DIR_DOWN, .look = LOOK_WYRMOND, .flags = NF_TRAINER, .say = L8_wyr_in, .alt = L8_wyr_after, .tr = &T8_wyr }
};
static const Sign S8_gym8[] = {{0}};
// VICTORY ROAD: south of DRAKESPIRE. Floor 1 needs ROCK SMASH, SURF and STRENGTH, floor 2 is a dark maze (FLASH). Ace trainers.
static const char *const L9_a1_in[] = {"ACE TRAINER CORA: Eight badges and still standing? Prove it!", 0};
static const char *const L9_a1_after[] = {"ACE TRAINER CORA: The rock below is cracked. Smash it.", 0};
static const char *const L9_a2_in[] = {"ACE TRAINER DEV: Everyone who reaches the LEAGUE came through this lake. Not everyone came out.", 0};
static const char *const L9_a2_after[] = {"ACE TRAINER DEV: Fine. SURF on, then.", 0};
static const char *const L9_a3_in[] = {"ACE TRAINER NOOR: I've trained in this cave for a year. Let's see your year!", 0};
static const char *const L9_a3_after[] = {"ACE TRAINER NOOR: That boulder only moves for real strength.", 0};
static const char *const L9_a4_in[] = {"ACE TRAINER KAI: The road goes down into the dark from here. You'll want FLASH.", "ACE TRAINER KAI: But first, me!", 0};
static const char *const L9_a4_after[] = {"ACE TRAINER KAI: Light the way and keep right. That's all I'll say.", 0};
static const char *const L9_a5_in[] = {"ACE TRAINER LIV: Lost in the dark? Me neither. Battle!", 0};
static const char *const L9_a5_after[] = {"ACE TRAINER LIV: The exit is up and to the right. Probably.", 0};
static const char *const L9_a6_in[] = {"ACE TRAINER OREN: One step from the LEAGUE. I won't be the one who stops you... or will I?", 0};
static const char *const L9_a6_after[] = {"ACE TRAINER OREN: Go. The ELITE FOUR are waiting.", 0};
static const unsigned char TP9_a1[] = {46,46,53,47,59,47};
static const Trainer T9_a1 = {"ACE TRAINER CORA", 4, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE1, 0, 0, TP9_a1, L9_a1_in, L9_a1_after, 0, "Proven."};
static const unsigned char TP9_a2[] = {63,47,66,47,61,48};
static const Trainer T9_a2 = {"ACE TRAINER DEV", 5, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE2, 0, 0, TP9_a2, L9_a2_in, L9_a2_after, 0, "Sunk!"};
static const unsigned char TP9_a3[] = {96,48,71,48,29,48};
static const Trainer T9_a3 = {"ACE TRAINER NOOR", 5, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE3, 0, 0, TP9_a3, L9_a3_in, L9_a3_after, 0, "A whole year..."};
static const unsigned char TP9_a4[] = {64,48,90,48,77,49};
static const Trainer T9_a4 = {"ACE TRAINER KAI", 3, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE4, 0, 0, TP9_a4, L9_a4_in, L9_a4_after, 0, "Lights out."};
static const unsigned char TP9_a5[] = {33,49,87,49,92,49};
static const Trainer T9_a5 = {"ACE TRAINER LIV", 3, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE5, 0, 0, TP9_a5, L9_a5_in, L9_a5_after, 0, "Lost after all."};
static const unsigned char TP9_a6[] = {52,49,89,50,26,50};
static const Trainer T9_a6 = {"ACE TRAINER OREN", 2, 80, 3, 1, 0, 255, 0, FLAG_BEAT_ACE6, 0, 0, TP9_a6, L9_a6_in, L9_a6_after, 0, "...I won't."};
static const Warp W9_vr1[] = {{12, 0, MAP_DRAKE, 12, 19, DIR_UP}, {20, 21, MAP_VICTORY2, 9, 16, DIR_UP}};
