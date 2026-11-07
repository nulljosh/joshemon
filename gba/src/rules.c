// Gen 3 battle rules, ported line for line from src/rules.mjs. tools/rules_test.c checks it with the same numbers.
#include "rules.h"
#include "../data/dex.h"

const unsigned char STAGE[13][2] = {{10,40},{10,35},{10,30},{10,25},{10,20},{10,15},{10,10},{15,10},{20,10},{25,10},{30,10},{35,10},{40,10}};
// Gen 3 has no physical/special flag on moves: the move's type decides.
int is_physical(int t) {
  return t == TYPE_NORMAL || t == TYPE_FIGHTING || t == TYPE_FLYING || t == TYPE_POISON || t == TYPE_GROUND || t == TYPE_ROCK || t == TYPE_BUG || t == TYPE_GHOST || t == TYPE_STEEL;
}
int stage_mod(int v, int s) { return v * STAGE[s][0] / STAGE[s][1]; }
int calc_stat(int base, int iv, int ev, int level, int hp) {
  int core = (2 * base + iv + ev / 4) * level / 100;
  return hp ? core + level + 10 : core + 5; // the nature's x110/100 or x90/100 comes after, in nature_pct
}
// Gen 3 natures (pokeruby gNatureStatTable): nature n raises stat n/5 and lowers stat n%5 of ATK DEF SPE SPA SPD,
// and does nothing when the two match. Our stats run HP ATK DEF SPA SPD SPE, hence the remap.
const char *const NATURE_NAME[25] = {"HARDY","LONELY","BRAVE","ADAMANT","NAUGHTY","BOLD","DOCILE","RELAXED","IMPISH","LAX","TIMID","HASTY","SERIOUS","JOLLY","NAIVE","MODEST","MILD","QUIET","BASHFUL","RASH","CALM","GENTLE","SASSY","CAREFUL","QUIRKY"};
int nature_pct(int nature, int stat) {
  static const unsigned char OURS[5] = {1, 2, 5, 3, 4};
  int up = OURS[nature / 5 % 5], down = OURS[nature % 5];
  return up == down || stat == 0 ? 100 : stat == up ? 110 : stat == down ? 90 : 100;
}
int base_damage(int level, int power, int type, int atk, int def, int as, int ds, int crit, int burn) {
  int a = crit ? (as > 6 ? stage_mod(atk, as) : atk) : stage_mod(atk, as);
  int d = crit ? (ds < 6 ? stage_mod(def, ds) : def) : stage_mod(def, ds);
  int dmg = a * power * (2 * level / 5 + 2);
  dmg = dmg / d / 50;
  if (is_physical(type)) { if (burn) dmg /= 2; if (dmg == 0) dmg = 1; }
  return dmg + 2;
}
int damage(int level, int power, int type, int atk, int def, int as, int ds, int crit, int burn, int stab, const int *eff10, int n, int roll) {
  int dmg = base_damage(level, power, type, atk, def, as, ds, crit, burn) * (crit ? 2 : 1);
  if (stab) dmg = dmg * 15 / 10;
  for (int i = 0; i < n; i++) dmg = dmg * eff10[i] / 10;
  if (dmg == 0) return 0;
  dmg = dmg * roll / 100;
  return dmg < 1 ? 1 : dmg;
}
int crit_chance(int s) { static const unsigned char N[5] = { 16, 8, 4, 3, 2 }; return N[s > 4 ? 4 : s]; }
int catch_odds(int rate, int ball10, int max_hp, int hp, int status) {
  int odds = (rate * ball10 / 10) * (max_hp * 3 - hp * 2) / (3 * max_hp);
  if (status == 1) odds *= 2; else if (status == 2) odds = odds * 15 / 10;
  return odds;
}
static unsigned isqrt(unsigned v) { unsigned r = 0; for (unsigned b = 1u << 30; b; b >>= 2) { if (v >= r + b) { v -= r + b; r = (r >> 1) + b; } else r >>= 1; } return r; }
int shake_odds(int odds) { return 1048560 / isqrt(isqrt(16711680 / odds)); }
int expected_shakes(int odds, unsigned (*rng16)(void)) {
  if (odds > 254) return 4;
  int s = shake_odds(odds), n = 0;
  while (n < 4 && (int)rng16() < s) n++;
  return n;
}
int exp_for_level(int g, int n) {
  int c = n * n * n, v;
  switch (g) {
    case GROWTH_FAST: v = 4 * c / 5; break;
    case GROWTH_MEDIUM_SLOW: v = 6 * c / 5 - 15 * n * n + 100 * n - 140; break;
    case GROWTH_SLOW: v = 5 * c / 4; break;
    case GROWTH_SLOW_THEN_VERY_FAST: // erratic
      v = n <= 50 ? c * (100 - n) / 50 : n <= 68 ? c * (150 - n) / 100 : n <= 98 ? (int)((long long)c * ((1911 - 10 * n) / 3) / 500) : c * (160 - n) / 100; break;
    case GROWTH_FAST_THEN_VERY_SLOW: // fluctuating
      v = n <= 15 ? c * ((n + 1) / 3 + 24) / 50 : n <= 35 ? c * (n + 14) / 50 : c * (n / 2 + 32) / 50; break;
    default: v = c;
  }
  return v < 0 ? 0 : v;
}
int level_for_exp(int g, int exp) { int l = 1; while (l < 100 && exp_for_level(g, l + 1) <= exp) l++; return l; }
int exp_gain(int base_exp, int foe_level) { return base_exp * foe_level / 7; }
int pick_slot(int r100) {
  static const unsigned char W[12] = { 20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1 };
  for (int i = 0, acc = 0; i < 12; i++) { acc += W[i]; if (r100 < acc) return i; }
  return 11;
}

// pokeruby: TAUNT stops any move whose power is 0. Gen 3 gives COUNTER, SEISMIC TOSS and the other self-scaling hits
// power 1, but this table holds 0 for them, so they're listed to stay usable.
int taunt_blocks(int id) {
  static const unsigned short HITS[] = { 12, 32, 49, 68, 69, 82, 90, 101, 117, 149, 162, 175, 179, 217, 222, 243, 255, 283, 329 };
  if (!id || MOVES[id].power) return 0;
  for (unsigned i = 0; i < sizeof HITS / sizeof *HITS; i++) if (HITS[i] == id) return 0;
  return 1;
}
