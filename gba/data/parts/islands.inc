// ROUTE 15, the LUMEN SHOALS: off LUMEN's south shore, five small islands you hop between on SURF.
// Two swimmers in the channels, a fisher and a sailor on the islands, and two items on the far ones.
static const char *const Lis_sw1_in[] = {"SWIMMER RAE: I swim to every island before breakfast. Keep up!", 0};
static const char *const Lis_sw1_after[] = {"SWIMMER RAE: Skip breakfast, I guess.", 0};
static const char *const Lis_sw2_in[] = {"SWIMMER OLA: The shoals are mine. Turn back or battle!", 0};
static const char *const Lis_sw2_after[] = {"SWIMMER OLA: Fine, share the shoals.", 0};
static const char *const Lis_fish_in[] = {"FISHER DOV: The tide brings the big ones in. You brought a battle.", 0};
static const char *const Lis_fish_after[] = {"FISHER DOV: Cast off the west point at dusk.", 0};
static const char *const Lis_sail_in[] = {"SAILOR KIT: My boat ran aground here. Battle me while I wait!", 0};
static const char *const Lis_sail_after[] = {"SAILOR KIT: The tide will float her off. Someday.", 0};
static const unsigned char TPis_sw1[] = {33,36,35,37};
static const Trainer Tis_sw1 = {"SWIMMER RAE", 3, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R15SWIM1, 0, 0, TPis_sw1, Lis_sw1_in, Lis_sw1_after, 0, "Glub!"};
static const unsigned char TPis_sw2[] = {78,36,34,37};
static const Trainer Tis_sw2 = {"SWIMMER OLA", 3, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R15SWIM2, 0, 0, TPis_sw2, Lis_sw2_in, Lis_sw2_after, 0, "Washed out!"};
static const unsigned char TPis_fish[] = {32,35,81,36,78,37};
static const Trainer Tis_fish = {"FISHER DOV", 3, 50, 3, 1, 0, 255, 0, FLAG_BEAT_R15FISH, 0, 0, TPis_fish, Lis_fish_in, Lis_fish_after, 0, "Line's tangled!"};
static const unsigned char TPis_sail[] = {79,37,35,38};
static const Trainer Tis_sail = {"SAILOR KIT", 3, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R15SAILOR, 0, 0, TPis_sail, Lis_sail_in, Lis_sail_after, 0, "Man overboard!"};
// ponytail: only the east half of the shore joins LUMEN; the rest of its south edge stays the sea
static const Warp Wis_route15[] = {{16, 0, MAP_LUMEN, 16, 18, DIR_UP}, {17, 0, MAP_LUMEN, 17, 18, DIR_UP}, {18, 0, MAP_LUMEN, 18, 18, DIR_UP}, {19, 0, MAP_LUMEN, 19, 18, DIR_UP}, {20, 0, MAP_LUMEN, 20, 18, DIR_UP}, {21, 0, MAP_LUMEN, 21, 18, DIR_UP}, {22, 0, MAP_LUMEN, 22, 18, DIR_UP}, {23, 0, MAP_LUMEN, 23, 18, DIR_UP}};
static const Npc Nis_route15[] = {
  { .x = 10, .y = 2, .dir = DIR_DOWN, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lis_sw1_in, .alt = Lis_sw1_after, .tr = &Tis_sw1 },
  { .x = 14, .y = 12, .dir = DIR_LEFT, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lis_sw2_in, .alt = Lis_sw2_after, .tr = &Tis_sw2 },
  { .x = 12, .y = 9, .dir = DIR_RIGHT, .look = LOOK_FISHER, .flags = NF_TRAINER, .say = Lis_fish_in, .alt = Lis_fish_after, .tr = &Tis_fish },
  { .x = 18, .y = 14, .dir = DIR_LEFT, .look = LOOK_SAILOR, .flags = NF_TRAINER, .say = Lis_sail_in, .alt = Lis_sail_after, .tr = &Tis_sail },
  { .x = 5, .y = 3, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_MYSTIC, .count = 1, .flag = FLAG_GOT_R15MYSTIC },
  { .x = 3, .y = 14, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_REVIVE, .count = 2, .flag = FLAG_GOT_R15REVIVE }
};
static const Sign Sis_route15[] = {{18, 6, "ROUTE 15, the LUMEN SHOALS. LUMEN ISLE is north across the water."}};
