// Gen 3 battle rules, ported from src/rules.mjs (itself from pret/pokeruby). Integer maths that floors like the GBA.
#pragma once
extern const unsigned char STAGE[13][2];
int is_physical(int type);
int calc_stat(int base, int iv, int ev, int level, int hp);
int nature_pct(int nature, int stat); // 110, 100 or 90 for stat 0..5 (HP ATK DEF SPA SPD SPE); HP never changes
extern const char *const NATURE_NAME[25];
int stage_mod(int v, int stage);
// atk/def: the chosen pair (attack/defense or sp.atk/sp.def); stages 0..12, 6 neutral
int base_damage(int level, int power, int type, int atk, int def, int atk_stage, int def_stage, int crit, int burn);
// eff10: x10 effectiveness per defender type (0, 5, 10, 20); n of them. roll 85..100.
int damage(int level, int power, int type, int atk, int def, int atk_stage, int def_stage, int crit, int burn, int stab, const int *eff10, int n, int roll);
int crit_chance(int stage); // 1 in N
// status: 0 none, 1 sleep/freeze (x2), 2 poison/burn/paralysis (x1.5)
int catch_odds(int catch_rate, int ball10, int max_hp, int hp, int status);
int shake_odds(int odds);
int expected_shakes(int odds, unsigned (*rng16)(void)); // 4 = caught
int exp_for_level(int growth, int n);
int level_for_exp(int growth, int exp);
int exp_gain(int base_exp, int foe_level);
int pick_slot(int r100);
