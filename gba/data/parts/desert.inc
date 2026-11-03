// ROUTE 13, the DUNE ROAD: a desert north of ROUTE 3, behind a cracked rock (ROCK SMASH). Dry-grass drifts hide ground
// and fire creatures, cacti you walk around, an oasis, and the SUN RUINS at the top. Border 'a' makes it a desert map.
static const char *const Lde_hik_in[] = {"HIKER DUSTIN: Sand in my boots, sand in my lunch. I love it out here!", 0};
static const char *const Lde_hik_after[] = {"HIKER DUSTIN: Drink at the oasis before you go north. The ruins get hot.", 0};
static const char *const Lde_pic_in[] = {"PICNICKER SAL: A picnic in the desert! Want to battle while the sandwiches bake?", 0};
static const char *const Lde_pic_after[] = {"PICNICKER SAL: Crunchy. The sandwiches, I mean.", 0};
static const char *const Lde_man_in[] = {"RUIN MANIAC OREN: The SUN RUINS are older than the region's name! I won't let you scratch them!", 0};
static const char *const Lde_man_after[] = {"RUIN MANIAC OREN: Read the tablet inside. Nobody knows who carved it.", 0};
static const char *const Lde_bird_in[] = {"BIRDER WYN: Desert birds fly highest. The hot air lifts them!", 0};
static const char *const Lde_bird_after[] = {"BIRDER WYN: Back to the thermals for us.", 0};
static const unsigned char TPde_hik[] = {44,18,45,19};
static const Trainer Tde_hik = {"HIKER DUSTIN", 3, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R13HIKER, 0, 0, TPde_hik, Lde_hik_in, Lde_hik_after, 0, "Buried!"};
static const unsigned char TPde_pic[] = {82,18,40,18};
static const Trainer Tde_pic = {"PICNICKER SAL", 4, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R13PIC, 0, 0, TPde_pic, Lde_pic_in, Lde_pic_after, 0, "My picnic!"};
static const unsigned char TPde_man[] = {38,19,100,20};
static const Trainer Tde_man = {"RUIN MANIAC OREN", 2, 60, 2, 1, 0, 255, 0, FLAG_BEAT_R13MANIAC, 0, 0, TPde_man, Lde_man_in, Lde_man_after, 0, "History repeats..."};
static const unsigned char TPde_bird[] = {67,18,24,20};
static const Trainer Tde_bird = {"BIRDER WYN", 2, 40, 2, 1, 0, 255, 0, FLAG_BEAT_R13BIRD, 0, 0, TPde_bird, Lde_bird_in, Lde_bird_after, 0, "Grounded!"};
static const Warp Wde_route13[] = {{9, 25, MAP_ROUTE3, 20, 2, DIR_DOWN}, {11, 1, MAP_RUINS, 7, 12, DIR_UP}};
static const Npc Nde_route13[] = {
  { .x = 12, .y = 16, .dir = DIR_LEFT, .look = LOOK_HIKER, .flags = NF_TRAINER, .say = Lde_hik_in, .alt = Lde_hik_after, .tr = &Tde_hik },
  { .x = 5, .y = 13, .dir = DIR_RIGHT, .look = LOOK_PICNIC, .flags = NF_TRAINER, .say = Lde_pic_in, .alt = Lde_pic_after, .tr = &Tde_pic },
  { .x = 14, .y = 5, .dir = DIR_DOWN, .look = LOOK_MANIAC, .flags = NF_TRAINER, .say = Lde_man_in, .alt = Lde_man_after, .tr = &Tde_man },
  { .x = 18, .y = 10, .dir = DIR_LEFT, .look = LOOK_BIRDER, .flags = NF_TRAINER, .say = Lde_bird_in, .alt = Lde_bird_after, .tr = &Tde_bird },
  { .x = 21, .y = 22, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_GREAT, .count = 3, .flag = FLAG_GOT_R13BALL },
  { .x = 2, .y = 2, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_SUPER, .count = 2, .flag = FLAG_GOT_R13POT }
};
static const Sign Sde_route13[] = {{8, 23, "ROUTE 13, the DUNE ROAD. North: the SUN RUINS. South: ROUTE 3."}};
// SUN RUINS: a small temple of old brick at the top of the DUNE ROAD. A psychic guards it, and a tablet tells of the sleepers.
static const char *const Lru_psy_in[] = {"PSYCHIC MIRA: The stones whisper to me. They say you should turn back!", 0};
static const char *const Lru_psy_after[] = {"PSYCHIC MIRA: The stones say you are stubborn. They are right.", 0};
static const char *const Lru_tab[] = {"Worn letters on the tablet:", "THREE SLEEP BENEATH THE LAND. ONE IN THE MOUNTAIN, ONE IN THE SEA, ONE ABOVE THE CLOUDS.", "WAKE THEM GENTLY, OR NOT AT ALL.", 0};
static const unsigned char TPru_psy[] = {25,19,49,20};
static const Trainer Tru_psy = {"PSYCHIC MIRA", 4, 60, 2, 1, 0, 255, 0, FLAG_BEAT_RUINPSY, 0, 0, TPru_psy, Lru_psy_in, Lru_psy_after, 0, "The stones were wrong!"};
static const Warp Wru_ruins[] = {{7, 13, MAP_ROUTE13, 11, 2, DIR_DOWN}, {8, 13, MAP_ROUTE13, 11, 2, DIR_DOWN}};
static const Npc Nru_ruins[] = {
  { .x = 12, .y = 9, .dir = DIR_LEFT, .look = LOOK_PSYCHIC, .flags = NF_TRAINER, .say = Lru_psy_in, .alt = Lru_psy_after, .tr = &Tru_psy },
  { .x = 7, .y = 1, .dir = DIR_DOWN, .look = LOOK_TABLET, .flags = 0, .say = Lru_tab },
  { .x = 2, .y = 8, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_TM05, .count = 1, .flag = FLAG_GOT_RUINTM },
  { .x = 13, .y = 3, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_SUPER, .count = 2, .flag = FLAG_GOT_RUINPOT }
};
static const Sign Sru_ruins[] = {{0}};
