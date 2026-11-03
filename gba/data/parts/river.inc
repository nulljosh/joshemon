// ROUTE 14, the BASALT RIVER: under ROUTE 3, two paths drop to the banks and the river between them needs SURF.
// Swimmers in the current, a fisher on the old pier, and a quiet south bank you only reach by water.
static const char *const Lri_sw1_in[] = {"SWIMMER JUNE: The current runs all the way to SKYREACH. Race me!", 0};
static const char *const Lri_sw1_after[] = {"SWIMMER JUNE: You swim like you were born in a river.", 0};
static const char *const Lri_sw2_in[] = {"SWIMMER PAX: Nobody comes down here. Finally, a challenger!", 0};
static const char *const Lri_sw2_after[] = {"SWIMMER PAX: Back to floating, then.", 0};
static const char *const Lri_fish_in[] = {"FISHER GUS: Shh! You scared the fish. Now you owe me a battle!", 0};
static const char *const Lri_fish_after[] = {"FISHER GUS: The big ones bite at the end of the pier.", 0};
static const char *const Lri_bird_in[] = {"BIRDER IVY: River birds dive for their dinner. Watch this!", 0};
static const char *const Lri_bird_after[] = {"BIRDER IVY: They're still hungry. So am I.", 0};
static const unsigned char TPri_sw1[] = {30,27,28,28};
static const Trainer Tri_sw1 = {"SWIMMER JUNE", 2, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R14SWIM1, 0, 0, TPri_sw1, Lri_sw1_in, Lri_sw1_after, 0, "Glub!"};
static const unsigned char TPri_sw2[] = {34,28,33,28};
static const Trainer Tri_sw2 = {"SWIMMER PAX", 2, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R14SWIM2, 0, 0, TPri_sw2, Lri_sw2_in, Lri_sw2_after, 0, "Swept away!"};
static const unsigned char TPri_fish[] = {32,27,32,28,35,28};
static const Trainer Tri_fish = {"FISHER GUS", 3, 50, 3, 1, 0, 255, 0, FLAG_BEAT_R14FISH, 0, 0, TPri_fish, Lri_fish_in, Lri_fish_after, 0, "Snapped my line!"};
static const unsigned char TPri_bird[] = {67,27,23,29};
static const Trainer Tri_bird = {"BIRDER IVY", 3, 50, 2, 1, 0, 255, 0, FLAG_BEAT_R14BIRD, 0, 0, TPri_bird, Lri_bird_in, Lri_bird_after, 0, "They flew off!"};
static const Warp Wri_route14[] = {{3, 0, MAP_ROUTE3, 3, 12, DIR_UP}, {24, 0, MAP_ROUTE3, 24, 12, DIR_UP}};
static const Npc Nri_route14[] = {
  { .x = 8, .y = 5, .dir = DIR_DOWN, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lri_sw1_in, .alt = Lri_sw1_after, .tr = &Tri_sw1 },
  { .x = 21, .y = 8, .dir = DIR_LEFT, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lri_sw2_in, .alt = Lri_sw2_after, .tr = &Tri_sw2 },
  { .x = 13, .y = 10, .dir = DIR_UP, .look = LOOK_FISHER, .flags = NF_TRAINER, .say = Lri_fish_in, .alt = Lri_fish_after, .tr = &Tri_fish },
  { .x = 19, .y = 11, .dir = DIR_LEFT, .look = LOOK_BIRDER, .flags = NF_TRAINER, .say = Lri_bird_in, .alt = Lri_bird_after, .tr = &Tri_bird },
  { .x = 1, .y = 12, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 2, .flag = FLAG_GOT_R14BALL },
  { .x = 25, .y = 12, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_HYPER, .count = 1, .flag = FLAG_GOT_R14POT }
};
static const Sign Sri_route14[] = {{2, 3, "ROUTE 14, the BASALT RIVER. Both paths lead back up to ROUTE 3."}};
