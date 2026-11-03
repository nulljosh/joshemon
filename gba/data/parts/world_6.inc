static const Npc N9_vr1[] = {
  { .x = 8, .y = 2, .dir = DIR_RIGHT, .look = LOOK_ACEF, .flags = NF_TRAINER, .say = L9_a1_in, .alt = L9_a1_after, .tr = &T9_a1 },
  { .x = 17, .y = 7, .dir = DIR_LEFT, .look = LOOK_ACE, .flags = NF_TRAINER, .say = L9_a2_in, .alt = L9_a2_after, .tr = &T9_a2 },
  { .x = 6, .y = 14, .dir = DIR_RIGHT, .look = LOOK_ACEF, .flags = NF_TRAINER, .say = L9_a3_in, .alt = L9_a3_after, .tr = &T9_a3 },
  { .x = 16, .y = 20, .dir = DIR_UP, .look = LOOK_ACE, .flags = NF_TRAINER, .say = L9_a4_in, .alt = L9_a4_after, .tr = &T9_a4 },
  { .x = 21, .y = 15, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 2, .flag = FLAG_GOT_VR1ITEM }
};
static const Sign S9_vr1[] = {{0}};
static const Warp W9_vr2[] = {{9, 17, MAP_VICTORY1, 20, 20, DIR_UP}, {16, 0, MAP_LEAGUE, 9, 10, DIR_UP}};
static const Npc N9_vr2[] = {
  { .x = 12, .y = 13, .dir = DIR_DOWN, .look = LOOK_ACEF, .flags = NF_TRAINER, .say = L9_a5_in, .alt = L9_a5_after, .tr = &T9_a5 },
  { .x = 16, .y = 5, .dir = DIR_RIGHT, .look = LOOK_ACE, .flags = NF_TRAINER, .say = L9_a6_in, .alt = L9_a6_after, .tr = &T9_a6 },
  { .x = 8, .y = 13, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_REVIVE, .count = 2, .flag = FLAG_GOT_VR2ITEM }
};
static const Sign S9_vr2[] = {{0}};
// THE JOSHEMON LEAGUE: past VICTORY ROAD. A lobby to heal and shop, the ELITE FOUR one room after another (each door opens once its member falls), then the CHAMPION.
static const char *const L10_sealed[] = {"The way forward is sealed. It opens when this room's trainer falls.", 0};
static const char *const L10_gate[] = {"Welcome to the JOSHEMON LEAGUE.", "Beyond the doors wait the ELITE FOUR, then the CHAMPION. Once you go in, there is no coming back out until it's over.", 0};
static const char *const L10_lobbykid[] = {"I came all the way here just to watch. The ELITE FOUR use dark, ghost, steel and fighting creatures, in that order!", 0};
static const char *const L10_nox_in[] = {"NOX: Welcome to the LEAGUE. I am NOX of the ELITE FOUR.", "NOX: My creatures live in the dark. Let us see if you can see at all.", 0};
static const char *const L10_nox_after[] = {"NOX: Go. The dark lets you pass.", 0};
static const unsigned char TP10_nox[] = {13,49,22,49,97,50,54,50,104,52};
static const Trainer T10_nox = {"ELITE FOUR NOX", 0, 200, 5, 1, 0, 255, 0, FLAG_BEAT_NOX, 0, 0, TP10_nox, L10_nox_in, L10_nox_after, 0, "Into the light..."};
static const unsigned short C10_nox[] = {FLAG_BEAT_NOX | 0x8000, 0};
static const char *const L10_ves_in[] = {"VESPERA: Shh. Do you hear them? My friends are all around us.", "VESPERA: They are curious about you. Let them have a closer look!", 0};
static const char *const L10_ves_after[] = {"VESPERA: They like you. How rare. Go on.", 0};
static const unsigned char TP10_ves[] = {85,50,87,51,54,52,87,53};
static const Trainer T10_ves = {"ELITE FOUR VESPERA", 0, 200, 4, 1, 0, 255, 0, FLAG_BEAT_VESPERA, 0, 0, TP10_ves, L10_ves_in, L10_ves_after, 0, "They fade..."};
static const unsigned short C10_ves[] = {FLAG_BEAT_VESPERA | 0x8000, 0};
static const char *const L10_fer_in[] = {"FERRAND: Steel does not bend. It does not tire. It does not lose.", "FERRAND: Well. It has not lost yet. Come!", 0};
static const char *const L10_fer_after[] = {"FERRAND: Steel bent. I will have to think about that.", 0};
static const unsigned char TP10_fer[] = {92,51,66,52,52,52,53,54};
static const Trainer T10_fer = {"ELITE FOUR FERRAND", 0, 200, 4, 1, 0, 255, 0, FLAG_BEAT_FERRAND, 0, 0, TP10_fer, L10_fer_in, L10_fer_after, 0, "Bent... not broken."};
static const unsigned short C10_fer[] = {FLAG_BEAT_FERRAND | 0x8000, 0};
static const char *const L10_bra_in[] = {"BRAKKA: HA! The last of the FOUR! I train on the mountain every morning before dawn!", "BRAKKA: Show me your fighting spirit!", 0};
static const char *const L10_bra_after[] = {"BRAKKA: Your spirit is real. The CHAMPION is through that door. Give it everything!", 0};
static const unsigned char TP10_bra[] = {48,52,63,53,37,53,64,55};
static const Trainer T10_bra = {"ELITE FOUR BRAKKA", 0, 200, 4, 1, 0, 255, 0, FLAG_BEAT_BRAKKA, 0, 0, TP10_bra, L10_bra_in, L10_bra_after, 0, "What a fight!"};
static const unsigned short C10_bra[] = {FLAG_BEAT_BRAKKA | 0x8000, 0};
static const char *const L10_aur_in[] = {"AURIC: So you are the one who beat the ELITE FOUR.", "AURIC: I watched you come up through VICTORY ROAD. You never once turned back.", "AURIC: I am AURIC, the CHAMPION. Everything you've learned, show it to me now!", 0};
static const char *const L10_aur_after[] = {"AURIC: You are the CHAMPION now. Wear it well.", 0};
static const unsigned char TP10_aur[] = {99,54,33,55,61,55,27,56,101,56,90,58};
static const Trainer T10_aur = {"CHAMPION AURIC", 0, 200, 6, 1, 0, 255, 0, FLAG_BEAT_AURIC, 0, 0, TP10_aur, L10_aur_in, L10_aur_after, 0, "Magnificent."};
static const unsigned short C10_aur[] = {FLAG_BEAT_AURIC | 0x8000, 0};
static const Warp W10_out[] = {{9, 5, MAP_LOBBY, 6, 8, DIR_UP}, {9, 11, MAP_VICTORY2, 16, 1, DIR_DOWN}, {20, 5, MAP_HALL, 5, 7, DIR_UP}};
static const Npc N10_out[] = {{ .x = 12, .y = 8, .dir = DIR_LEFT, .look = LOOK_ACE, .flags = NF_WANDER, .say = L9_a6_after }};
static const Sign S10_out[] = {{11, 8, "THE JOSHEMON LEAGUE. Only the strongest trainers may enter."}, {22, 6, "BATTLE HALL. How many wins in a row can you take?"}};
static const Warp W10_lobby[] = {{6, 9, MAP_LEAGUE, 9, 6, DIR_DOWN}, {7, 9, MAP_LEAGUE, 9, 6, DIR_DOWN}, {6, 0, MAP_E4A, 5, 9, DIR_UP}, {7, 0, MAP_E4A, 5, 9, DIR_UP}};
static const Npc N10_lobby[] = {
  { .x = 3, .y = 1, .dir = DIR_DOWN, .look = LOOK_NURSE, .flags = NF_HEAL },
  { .x = 10, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_SHOP },
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_SAILOR, .flags = 0, .say = L10_gate },
  { .x = 11, .y = 6, .dir = DIR_LEFT, .look = LOOK_BOY, .flags = NF_WANDER, .say = L10_lobbykid }
};
static const Sign S10_lobby[] = {{0}};
static const unsigned char K10_lobby[] = {ITEM_ULTRA, ITEM_HYPER, ITEM_MAXPOT, ITEM_FULLHEAL, ITEM_REVIVE, ITEM_REPEL};
static const Warp W10_nox[] = {{5, 0, MAP_E4B, 5, 9, DIR_UP}};
static const Npc N10_nox[] = {
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_NOX, .flags = NF_TRAINER, .say = L10_nox_in, .alt = L10_nox_after, .tr = &T10_nox },
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .say = L10_sealed, .cond = C10_nox }
};
static const Sign S10_nox[] = {{0}};
static const Warp W10_ves[] = {{5, 0, MAP_E4C, 5, 9, DIR_UP}};
static const Npc N10_ves[] = {
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_VESPERA, .flags = NF_TRAINER, .say = L10_ves_in, .alt = L10_ves_after, .tr = &T10_ves },
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .say = L10_sealed, .cond = C10_ves }
};
static const Sign S10_ves[] = {{0}};
static const Warp W10_fer[] = {{5, 0, MAP_E4D, 5, 9, DIR_UP}};
static const Npc N10_fer[] = {
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_FERRAND, .flags = NF_TRAINER, .say = L10_fer_in, .alt = L10_fer_after, .tr = &T10_fer },
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .say = L10_sealed, .cond = C10_fer }
};
static const Sign S10_fer[] = {{0}};
static const Warp W10_bra[] = {{5, 0, MAP_CHAMP, 5, 9, DIR_UP}};
static const Npc N10_bra[] = {
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_BRAKKA, .flags = NF_TRAINER, .say = L10_bra_in, .alt = L10_bra_after, .tr = &T10_bra },
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .say = L10_sealed, .cond = C10_bra }
};
static const Sign S10_bra[] = {{0}};
static const char *const L10_mayaf_in[] = {"MAYA: We started this together in CEDAR TOWN. Let's finish it together. Give it everything you've got!", 0};
static const char *const L10_mayaf_after[] = {"MAYA: ...Yeah. You're the real thing, JOSH.", "MAYA: Go on. Your name belongs in the HALL OF FAME. I'll be right behind you. Someday.", 0};
static const unsigned char TP10_mayaf[] = {69,55,71,55,103,56,102,56,35,57,6,59,69,55,71,55,103,56,102,56,35,57,9,59,69,55,71,55,103,56,102,56,35,57,3,59};
static const Trainer T10_mayaf = {"MAYA", 0, 120, 6, 3, 0, 255, 0, FLAG_BEAT_MAYAF, 0, 0, TP10_mayaf, L10_mayaf_in, L10_mayaf_after, 0, "Ha... you really are the CHAMPION."};
static const unsigned short C10_mayaf[] = {FLAG_BEAT_AURIC, FLAG_BEAT_MAYAF | 0x8000, 0};
static const Warp W10_aur[] = {{0}};
static const Npc N10_aur[] = {
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_AURIC, .flags = NF_TRAINER, .say = L10_aur_in, .alt = L10_aur_after, .tr = &T10_aur },
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .say = L10_sealed, .cond = C10_aur },
  { .x = 5, .y = 9, .dir = DIR_UP, .look = LOOK_MAYA, .flags = NF_TRAINER, .say = L10_mayaf_in, .alt = L10_mayaf_after, .tr = &T10_mayaf, .cond = C10_mayaf }
};
static const Sign S10_aur[] = {{0}};
// SKYREACH SUMMIT: the post-game climb above SKYREACH TOWN, where CAELITH (#107) sleeps
static const char *const L11_cael[] = {"The wind stops. Far above, the clouds part...", "CAELITH, the sleeper in the sky, descends!", 0};
static const char *const L11_cael_after[] = {"The clouds drift apart. From the summit you can see the whole region, all the way to the sea.", 0};
static const unsigned short C11_cael[] = {FLAG_CHAMPION, FLAG_CAEL_CAUGHT | 0x8000, 0}; // FLAG_CAEL_CAUGHT must follow FLAG_SKY_CALM
static const Warp W11_summit[] = {{7, 13, MAP_SKY, 9, 1, DIR_DOWN}, {8, 13, MAP_SKY, 9, 1, DIR_DOWN}};
static const Npc N11_summit[] = {
  { .x = 7, .y = 4, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .flag = FLAG_SKY_CALM, .say = L11_cael, .alt = L11_cael_after, .cond = C11_cael, .legend = 107, .lv = 70 }
};
static const Sign S11_summit[] = {{0}};
// BATTLE HALL: next to the LEAGUE, for the CHAMPION. Win streaks against random trainers at your own level (story.inc battle_hall).
static const char *const L12_hall_vet[] = {"I've watched every streak in this hall. The record? Let's just say it isn't yours yet.", 0};
static const Warp W12_hall[] = {{5, 8, MAP_LEAGUE, 20, 6, DIR_DOWN}, {6, 8, MAP_LEAGUE, 20, 6, DIR_DOWN}};
static const Npc N12_hall[] = {
  { .x = 5, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_HALL },
  { .x = 9, .y = 5, .dir = DIR_LEFT, .look = LOOK_OLD, .flags = 0, .say = L12_hall_vet }
};
static const Sign S12_hall[] = {{0}};
const unsigned short SQ_RATE[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,157,263,363,457,547,631,711,786,856,923,986,1046,1102,1155,1205,1253,1297,1339,1379,1417,1452,1486,1517,1547,1575,1602,1627,1650,1673,1694,1714,1732,1750,1767,1783,1798,1812,1825,1837,1849,1860,1871,1881,1890,1899,1907,1915,1923,1930,1936,1943,1949,1954,1959,1964,1969,1974,1978,1982,1985,1989,1992,1995,1998,2001,2004,2006,2009,2011,2013,2015,2017,2018,2020,2022,2023,2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,2035,2036,2036,2037,2038};
const unsigned short WV_RATE[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,157,263,363,457,547,631,711,786,856,923,986,1046,1102,1155,1205,1253,1297,1339,1379,1417,1452,1486,1517,1547,1575,1602,1627,1650,1673,1694,1714,1732,1750,1767,1783,1798,1812,1825,1837,1849,1860,1871,1881,1890,1899,1907,1915,1923,1930,1936,1943,1949,1954,1959,1964,1969,1974,1978,1982,1985,1989,1992,1995,1998,2001,2004,2006,2009,2011,2013,2015,2017,2018,2020,2022,2023,2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,2035,2036,2036,2037,2038,2038,2039,2039,2040,2040,2041,2041,2041,2042,2042,2042,2043};
// ROUTE 10, THE SEAWALK: a boardwalk north from HARBOR CITY's pier over the sea, open once you hold the TIDE BADGE
static const char *const Lcv_gate[] = {"SAILOR: The SEAWALK runs north over open sea to COVEWICK.", "SAILOR: The planks are slick and the swimmers are rough. Only trainers with HARBOR GYM's badge may cross.", 0};
static const char *const Lcv_fish_in[] = {"FISHER NED: Shh! You'll scare the fish. Fine, a quick battle!", 0};
static const char *const Lcv_fish_after[] = {"FISHER NED: The big ones bite at dusk, off the end of the boards.", 0};
static const char *const Lcv_swim_in[] = {"SWIMMER LIA: I swim the whole SEAWALK every day. Race you!", 0};
static const char *const Lcv_swim_after[] = {"SWIMMER LIA: You beat me without even getting wet. Not fair!", 0};
static const char *const Lcv_sail_in[] = {"SAILOR KANE: Steady on the planks, landlubber!", 0};
static const char *const Lcv_sail_after[] = {"SAILOR KANE: COVEWICK has the best chowder on the coast. Tell them KANE sent you.", 0};
static const char *const Lcv_bird_in[] = {"BIRDER TESS: The gulls nest on this islet. So do my creatures!", 0};
static const char *const Lcv_bird_after[] = {"BIRDER TESS: Flying types love the sea wind. Watch for SEASKIP diving.", 0};
static const unsigned short Ccv_gate[] = {FLAG_BADGE_TIDE | 0x8000, 0};
static const unsigned char TPcv_fish[] = {32,12,32,13};
static const Trainer Tcv_fish = {"FISHER NED", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R10FISH, 0, 0, TPcv_fish, Lcv_fish_in, Lcv_fish_after, 0, "Snapped my line!"};
static const unsigned char TPcv_swim[] = {30,12,28,13};
static const Trainer Tcv_swim = {"SWIMMER LIA", 1, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R10SWIM, 0, 0, TPcv_swim, Lcv_swim_in, Lcv_swim_after, 0, "Glub..."};
static const unsigned char TPcv_sail[] = {34,13,10,12,10,13};
static const Trainer Tcv_sail = {"SAILOR KANE", 4, 50, 3, 1, 0, 255, 0, FLAG_BEAT_R10SAILOR, 0, 0, TPcv_sail, Lcv_sail_in, Lcv_sail_after, 0, "Man overboard!"};
static const unsigned char TPcv_bird[] = {67,13,23,14};
static const Trainer Tcv_bird = {"BIRDER TESS", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R10BIRDER, 0, 0, TPcv_bird, Lcv_bird_in, Lcv_bird_after, 0, "They flew off!"};
static const Warp Wcv_route10[] = {{11, 29, MAP_HARBOR, 11, 1, DIR_DOWN}, {12, 29, MAP_HARBOR, 12, 1, DIR_DOWN}, {9, 0, MAP_COVE, 9, 18, DIR_UP}, {10, 0, MAP_COVE, 10, 18, DIR_UP}};
static const Npc Ncv_route10[] = {
  { .x = 11, .y = 27, .dir = DIR_DOWN, .look = LOOK_SAILOR, .flags = NF_SHOW, .say = Lcv_gate, .cond = Ccv_gate },
  { .x = 13, .y = 18, .dir = DIR_LEFT, .look = LOOK_FISHER, .flags = NF_TRAINER, .say = Lcv_fish_in, .alt = Lcv_fish_after, .tr = &Tcv_fish },
  { .x = 5, .y = 12, .dir = DIR_LEFT, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lcv_swim_in, .alt = Lcv_swim_after, .tr = &Tcv_swim },
  { .x = 5, .y = 9, .dir = DIR_RIGHT, .look = LOOK_SAILOR, .flags = NF_TRAINER, .say = Lcv_sail_in, .alt = Lcv_sail_after, .tr = &Tcv_sail },
  { .x = 14, .y = 4, .dir = DIR_LEFT, .look = LOOK_BIRDER, .flags = NF_TRAINER, .say = Lcv_bird_in, .alt = Lcv_bird_after, .tr = &Tcv_bird },
  { .x = 1, .y = 14, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_GREAT, .count = 2, .flag = FLAG_GOT_R10BALL },
  { .x = 5, .y = 4, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_SUPER, .count = 1, .flag = FLAG_GOT_R10POT }
};
static const Sign Scv_route10[] = {{13, 24, "ROUTE 10, THE SEAWALK. North: COVEWICK. South: HARBOR CITY."}};
// COVEWICK TOWN: a fishing village on an island at the top of the SEAWALK. A gift creature, woods to the west
static const char *const Lcv_cold[] = {"Before the SEAWALK, the only way here was by rowboat. Took all morning.", "Now trainers come every day. The chowder pot never cools.", 0};
static const char *const Lcv_ckid[] = {"There's a big rock in the woods! Mom says only a really strong creature could shove it.", 0};
static const char *const Lcv_cnet[] = {"The fish run thick off COVEWICK. Got a rod? Try the water by the boards.", 0};
static const char *const Lcv_gift_say[] = {"MARIS: Oh! A trainer! Can I ask you something?", "MARIS: This little one washed up in a crate from a ship last spring. It chimes like a bell when it's happy.", "MARIS: It never chimes for me. I think it wants to see the world. Will you take it?", 0};
static const char *const Lcv_gift_alt[] = {"MARIS: PIXIEBELL is chiming already! I knew it. Take good care of it.", 0};
static const char *const Lcv_hgran[] = {"MARIS found that creature on the beach. She talks to it every night.", 0};
static const char *const Lcv_cshop[] = {"Heading into COVEWICK WOODS? Bug creatures everywhere in there. Stock up on ANTIDOTES.", 0};
static const Warp Wcv_cove[] = {{9, 19, MAP_ROUTE10, 9, 1, DIR_DOWN}, {10, 19, MAP_ROUTE10, 10, 1, DIR_DOWN}, {0, 7, MAP_ROUTE11, 22, 7, DIR_LEFT}, {0, 8, MAP_ROUTE11, 22, 8, DIR_LEFT}, {5, 13, MAP_REST_COVE, 4, 6, DIR_UP}, {15, 13, MAP_MART_COVE, 4, 6, DIR_UP}, {6, 5, MAP_HOUSE_COVE, 4, 6, DIR_UP}};
static const Npc Ncv_cove[] = {
  { .x = 13, .y = 9, .dir = DIR_DOWN, .look = LOOK_OLD, .flags = NF_WANDER, .say = Lcv_cold },
  { .x = 8, .y = 7, .dir = DIR_LEFT, .look = LOOK_BOY, .flags = NF_WANDER, .say = Lcv_ckid },
  { .x = 8, .y = 15, .dir = DIR_DOWN, .look = LOOK_FISHER, .flags = 0, .say = Lcv_cnet }
};
static const Sign Scv_cove[] = {{12, 5, "COVEWICK TOWN. Nets out, doors open."}};
static const Warp Wcv_rest_cove[] = {{4, 7, MAP_COVE, 5, 14, DIR_DOWN}, {5, 7, MAP_COVE, 5, 14, DIR_DOWN}};
static const Npc Ncv_rest_cove[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_NURSE, .flags = NF_HEAL },
  { .x = 8, .y = 1, .dir = DIR_DOWN, .look = LOOK_PC, .flags = NF_PC }
};
static const Sign Scv_rest_cove[] = {{0}};
static const Warp Wcv_mart_cove[] = {{4, 7, MAP_COVE, 15, 14, DIR_DOWN}, {5, 7, MAP_COVE, 15, 14, DIR_DOWN}};
static const Npc Ncv_mart_cove[] = {
  { .x = 4, .y = 1, .dir = DIR_DOWN, .look = LOOK_CLERK, .flags = NF_SHOP },
  { .x = 7, .y = 6, .dir = DIR_UP, .look = LOOK_LASS, .flags = 0, .say = Lcv_cshop }
};
static const Sign Scv_mart_cove[] = {{0}};
static const unsigned char Kcv_mart_cove[] = {ITEM_BALL, ITEM_GREAT, ITEM_POTION, ITEM_SUPER, ITEM_ANTIDOTE, ITEM_PARLYZ, ITEM_REPEL};
static const Warp Wcv_house_cove[] = {{4, 7, MAP_COVE, 6, 6, DIR_DOWN}, {5, 7, MAP_COVE, 6, 6, DIR_DOWN}};
