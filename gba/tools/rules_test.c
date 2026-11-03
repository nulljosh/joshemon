// make test (cc tools/rules_test.c src/rules.c data/dex.c)
// The C rules against the same hand-worked numbers as src/rules.test.mjs, plus every exp curve dumped for a diff with JS.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/rules.h"
#include "../data/dex.h"
static unsigned max16(void) { return 65535; }
int main(int argc, char **argv) {
  if (argc > 1 && !strcmp(argv[1], "curves")) { for (int g = 0; g < 6; g++) for (int n = 1; n <= 100; n++) printf("%d\n", exp_for_level(g, n)); return 0; }
  assert(calc_stat(40, 31, 0, 5, 1) == 20);
  int e20[] = { 20 }, e205[] = { 20, 5 }, e0[] = { 0 };
  assert(base_damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 0) == 37);
  assert(base_damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 1, 0) == 37);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 1, 0, 0, 0, 0, 100) == 74);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 0, 1, e20, 1, 100) == 110);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 0, 1, e20, 1, 85) == 93);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 0, 0, e205, 2, 100) == 37);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 0, 0, e0, 1, 100) == 0);
  assert(damage(50, 80, TYPE_NORMAL, 100, 100, 6, 6, 0, 1, 0, 0, 0, 100) == 19);
  assert(base_damage(50, 80, TYPE_NORMAL, 100, 100, 8, 6, 0, 0) == 72);
  assert(base_damage(50, 80, TYPE_NORMAL, 100, 100, 4, 6, 1, 0) == 37);
  assert(base_damage(50, 80, TYPE_NORMAL, 100, 100, 4, 6, 0, 0) == 2 + (50 * 80 * 22 / 100) / 50);
  assert(is_physical(TYPE_GHOST) && !is_physical(TYPE_DARK));
  assert(catch_odds(45, 10, 100, 100, 0) == 15);
  assert(shake_odds(15) == 32767);
  assert(catch_odds(45, 10, 100, 100, 1) == 30);
  assert(catch_odds(255, 10, 100, 1, 0) == 253);
  assert(catch_odds(255, 20, 100, 1, 0) == 506);
  assert(expected_shakes(506, max16) == 4);
  assert(expected_shakes(catch_odds(3, 10, 100, 100, 0), max16) == 0);
  assert(exp_for_level(GROWTH_MEDIUM_SLOW, 5) == 135 && exp_for_level(GROWTH_MEDIUM, 10) == 1000 && exp_for_level(GROWTH_SLOW, 10) == 1250 && exp_for_level(GROWTH_FAST, 10) == 800 && exp_for_level(GROWTH_MEDIUM_SLOW, 1) == 0);
  assert(level_for_exp(GROWTH_MEDIUM, 1000) == 10);
  assert(exp_gain(64, 7) == 64);
  assert(pick_slot(0) == 0 && pick_slot(99) == 11);
  for (int i = 1; i <= SPECIES_COUNT; i++) assert(SPECIES[i].growth < 6);
  puts("rules ok");
  return 0;
}
