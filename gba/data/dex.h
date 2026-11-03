// Our 105 creatures plus the late legends (106 THALASSOR, 107 CAELITH), every move and the type chart (data/dex.c). Baked from mons.js and PokeAPI Gen 3 data.
#pragma once
#define SPECIES_COUNT 107
#define TYPE_COUNT 17
#define MOVE_MAX 354
enum { TYPE_NORMAL, TYPE_FIGHTING, TYPE_FLYING, TYPE_POISON, TYPE_GROUND, TYPE_ROCK, TYPE_BUG, TYPE_GHOST, TYPE_STEEL, TYPE_FIRE, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC, TYPE_PSYCHIC, TYPE_ICE, TYPE_DRAGON, TYPE_DARK };
enum { GROWTH_FAST, GROWTH_MEDIUM, GROWTH_MEDIUM_SLOW, GROWTH_SLOW, GROWTH_SLOW_THEN_VERY_FAST, GROWTH_FAST_THEN_VERY_SLOW };
enum { AIL_NONE, AIL_PAR, AIL_SLP, AIL_PSN, AIL_BRN, AIL_FRZ, AIL_CONF, AIL_OTHER };
enum { CAT_OTHER, CAT_HEAL, CAT_RAISE };
// acc 255: never misses. hits 0,0: one hit. tgt 1: the user. sc: stat index (1 atk .. 7 eva) and stages.
typedef struct { const char *name; unsigned char type, power, acc, pp; signed char pri; unsigned char cat, ail, ailP; signed char drain; unsigned char heal, flinch, crit, hitsLo, hitsHi, statP, chance, tgt, nsc; signed char sc[5][2]; } Move;
// learn: level << 9 | move id. evo: pairs of (to, level). types[1] 255: one type.
typedef struct { const char *name, *dex; unsigned char types[2], stats[6]; unsigned short exp; unsigned char catchRate, growth, nlearn, nevo; const unsigned short *learn; const unsigned char *evo; } Species;
extern const char *const TYPE_NAME[];
extern const unsigned char TYPE_CHART[TYPE_COUNT][TYPE_COUNT];
extern const Move MOVES[];
extern const Species SPECIES[];
