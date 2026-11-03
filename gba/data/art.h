// Creature art, battle scenes and the title picture (data/art.c).
#pragma once
#define ART_MON(id, back) (((id) - 1) * 2 + (back))
#define ART_PORTRAIT(look) (1000 + (look)) // trainer portraits come from the painter (build/maps.c, PORTRAIT_TILES)
#define ART_PLAYER_BACK (1000 + LOOK_DATA_COUNT)
extern const unsigned int ART_TILES[];   // 64 tiles per sprite
extern const unsigned short ART_PAL[];   // 16 colours per sprite
extern const unsigned char MON_FEET[];   // lowest inked row of each front, by id - 1
// battle scenes: backdrop 30x14 8bpp tiles, pads as 5 OBJ frames of 64x32 (foe 0-1, yours 2-4)
enum { SCENE_GRASS, SCENE_CAVE, SCENE_WATER, SCENE_INDOOR };
typedef struct { unsigned short npal, ntiles; const unsigned short *pal; const unsigned int *tiles; const unsigned short *map; const unsigned int *pad; const unsigned short *padpal; } Scene;
extern const Scene SCENES[];
#define TITLE_NPAL 28
#define TITLE_NTILES 218
extern const unsigned short TITLE_PAL[], TITLE_MAP[];
extern const unsigned int TITLE_TILES[];
