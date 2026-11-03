// ROUTE 12, CEDAR COAST (its guard lives in world.c, next to CEDAR's people): east of CEDAR TOWN along the south shore, opened once you are CHAMPION. Strong trainers,
// strong wild creatures, and the ELDER GROTTO at the far end, where GEMSHADE sleeps on a seam of crystal.
static const char *const Lco_ace_in[] = {"ACE TRAINER DORIAN: The new CHAMPION, out on my coast? Show me the team that did it!", 0};
static const char *const Lco_ace_after[] = {"ACE TRAINER DORIAN: The ELDER GROTTO is at the end of the road. Something in there glitters.", 0};
static const char *const Lco_belt_in[] = {"BLACK BELT ROAN: I train against the waves. Today I train against you!", 0};
static const char *const Lco_belt_after[] = {"BLACK BELT ROAN: Hah! The sea is easier. It doesn't hit back as hard.", 0};
static const char *const Lco_bird_in[] = {"BIRDER NELL: The cliff winds carry my creatures higher than anywhere else!", 0};
static const char *const Lco_bird_after[] = {"BIRDER NELL: Grounded. Well played.", 0};
static const char *const Lco_swim_in[] = {"SWIMMER CORAL: I swim here from CEDAR every morning. Bet you can't keep up!", 0};
static const char *const Lco_swim_after[] = {"SWIMMER CORAL: Fine, you win. I'm still swimming home, though.", 0};
static const unsigned char TPco_ace[] = {64,58,87,58,66,59};
static const Trainer Tco_ace = {"ACE TRAINER DORIAN", 3, 90, 3, 1, 0, 255, 0, FLAG_BEAT_R12ACE, 0, 0, TPco_ace, Lco_ace_in, Lco_ace_after, 0, "A true CHAMPION."};
static const unsigned char TPco_belt[] = {63,57,64,59};
static const Trainer Tco_belt = {"BLACK BELT ROAN", 4, 80, 2, 1, 0, 255, 0, FLAG_BEAT_R12BELT, 0, 0, TPco_belt, Lco_belt_in, Lco_belt_after, 0, "Oof!"};
static const unsigned char TPco_bird[] = {69,58,35,58,59,60};
static const Trainer Tco_bird = {"BIRDER NELL", 4, 80, 3, 1, 0, 255, 0, FLAG_BEAT_R12BIRD, 0, 0, TPco_bird, Lco_bird_in, Lco_bird_after, 0, "Feathers everywhere!"};
static const unsigned char TPco_swim[] = {33,58,81,59};
static const Trainer Tco_swim = {"SWIMMER CORAL", 1, 80, 2, 1, 0, 255, 0, FLAG_BEAT_R12SWIM, 0, 0, TPco_swim, Lco_swim_in, Lco_swim_after, 0, "Glub!"};
static const Warp Wco_route12[] = {{0, 8, MAP_CEDAR, 18, 8, DIR_LEFT}, {21, 5, MAP_GROTTO, 9, 16, DIR_UP}};
static const Npc Nco_route12[] = {
  { .x = 12, .y = 4, .dir = DIR_DOWN, .look = LOOK_ACE, .flags = NF_TRAINER, .say = Lco_ace_in, .alt = Lco_ace_after, .tr = &Tco_ace },
  { .x = 17, .y = 9, .dir = DIR_LEFT, .look = LOOK_BELT, .flags = NF_TRAINER, .say = Lco_belt_in, .alt = Lco_belt_after, .tr = &Tco_belt },
  { .x = 6, .y = 12, .dir = DIR_RIGHT, .look = LOOK_BIRDER, .flags = NF_TRAINER, .say = Lco_bird_in, .alt = Lco_bird_after, .tr = &Tco_bird },
  { .x = 3, .y = 16, .dir = DIR_UP, .look = LOOK_SWIMMER, .flags = NF_TRAINER, .say = Lco_swim_in, .alt = Lco_swim_after, .tr = &Tco_swim },
  { .x = 6, .y = 1, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 3, .flag = FLAG_GOT_R12BALL },
  { .x = 22, .y = 14, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_MAXPOT, .count = 1, .flag = FLAG_GOT_R12POT }
};
static const Sign Sco_route12[] = {{2, 7, "ROUTE 12, CEDAR COAST. West: CEDAR TOWN. East: ELDER GROTTO."}};
// ELDER GROTTO: a cave around an underground lake. ROCK SMASH opens the west path and STRENGTH the east; both meet
// at the top, where GEMSHADE sleeps. Beat it to calm it, or catch it.
static const char *const Lgr_ace_in[] = {"ACE TRAINER SELA: I came for the glittering one. You'll have to get past me first!", 0};
static const char *const Lgr_ace_after[] = {"ACE TRAINER SELA: It's at the top. I couldn't wake it. Maybe you can.", 0};
static const char *const Lgr_hik_in[] = {"HIKER GUNNAR: These rocks are older than the league. So am I, nearly!", 0};
static const char *const Lgr_hik_after[] = {"HIKER GUNNAR: Smash the cracked rock, or shove the boulder. Either way gets you up.", 0};
static const unsigned char TPgr_ace[] = {75,60,77,60,61,61};
static const Trainer Tgr_ace = {"ACE TRAINER SELA", 3, 90, 3, 1, 0, 255, 0, FLAG_BEAT_GACE, 0, 0, TPgr_ace, Lgr_ace_in, Lgr_ace_after, 0, "Frozen solid."};
static const unsigned char TPgr_hik[] = {46,60,53,61};
static const Trainer Tgr_hik = {"HIKER GUNNAR", 2, 80, 2, 1, 0, 255, 0, FLAG_BEAT_GHIKER, 0, 0, TPgr_hik, Lgr_hik_in, Lgr_hik_after, 0, "Rockslide!"};
static const char *const Lgr_gem[] = {"Deep in the grotto, a crystal the size of a boulder glitters in the dark...", "The crystal opens its eyes!", 0};
static const char *const Lgr_gem_after[] = {"The grotto is quiet. Light from the crystals dances on the lake.", 0};
static const unsigned short Cgr_gem[] = {FLAG_GEM_CAUGHT | 0x8000, 0}; // FLAG_GEM_CAUGHT must follow FLAG_GEM_MET
static const Warp Wgr_grotto[] = {{9, 17, MAP_ROUTE12, 21, 6, DIR_DOWN}, {10, 17, MAP_ROUTE12, 21, 6, DIR_DOWN}};
static const Npc Ngr_grotto[] = {
  { .x = 13, .y = 12, .dir = DIR_LEFT, .look = LOOK_ACEF, .flags = NF_TRAINER, .say = Lgr_ace_in, .alt = Lgr_ace_after, .tr = &Tgr_ace },
  { .x = 5, .y = 10, .dir = DIR_RIGHT, .look = LOOK_HIKER, .flags = NF_TRAINER, .say = Lgr_hik_in, .alt = Lgr_hik_after, .tr = &Tgr_hik },
  { .x = 3, .y = 4, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_MAXPOT, .count = 2, .flag = FLAG_GOT_GMAX },
  { .x = 16, .y = 4, .dir = DIR_DOWN, .look = LOOK_BALL, .flags = NF_ITEM, .item = ITEM_ULTRA, .count = 5, .flag = FLAG_GOT_GULTRA },
  { .x = 9, .y = 1, .dir = DIR_DOWN, .look = 0, .flags = NF_SHOW | NF_NOSPRITE, .flag = FLAG_GEM_MET, .say = Lgr_gem, .alt = Lgr_gem_after, .cond = Cgr_gem, .legend = 54, .lv = 62 }
};
static const Sign Sgr_grotto[] = {{0}};
