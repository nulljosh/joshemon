// Joshemon for the Game Boy Advance.
// C2: title, then the overworld: the player walks the maps on a 16px grid (B runs), hops ledges, takes doors and
// warps through a fade, NPCs and items stand on their tiles as hardware sprites, and named maps pop up their name.
// Ground layer (BG1, prio 2) under the sprites (prio 1), canopy (BG2, prio 0) over them, text surface (BG0) on top.
#include "gba.h"
#include "../data/font.h"
#include "../build/maps.h"
#include "../data/world.h"
#include "../data/dex.h"
#include "../data/art.h"
#include "rules.h"
// The hot paths (drawing, the sound mixer) run as ARM code from IWRAM: no ROM wait states, a 32-bit bus.
#define FAST __attribute__((section(".iwram.draw"), target("arm"), noinline, long_call))
#include "sound.inc"

#define SBB_TEXT 31
#define SBB_GROUND 29
#define SBB_OVER 30
#define CBB_MAP 0
#define CBB_TEXT 2
#define TEXT_PAL 15
#define SHADOW_PAL 15
#define SLOT_TILES 48 // 6 frames of a 16x32 sprite
#define SHADOW_TILE (15 * SLOT_TILES)
// gcc emits these for struct copies; there is no libc on the cartridge.
__attribute__((optimize("no-tree-loop-distribute-patterns"))) void *memcpy(void *d, const void *s, unsigned n) { u8 *a = d; const u8 *b = s; while (n--) *a++ = *b++; return d; }
__attribute__((optimize("no-tree-loop-distribute-patterns"))) void *memset(void *d, int c, unsigned n) { u8 *a = d; while (n--) *a++ = c; return d; }
#define HEX(v) RGB15(((v) >> 19) & 31, ((v) >> 11) & 31, ((v) >> 3) & 31)

static u16 keys, prev;
static inline int pressed(int k) { return (keys & k) && !(prev & k); }
static void read_keys(void) { prev = keys; keys = ~REG_KEYINPUT & 0x3FF; }
static void copy32(volatile u32 *dst, const u32 *src, int n) { while (n--) *dst++ = *src++; }
static u32 seed = 0x2545F491;
static u32 rnd(void) { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
static int frame;
// What the headless runner (tools/gbarun.c) reads to script play: in a battle, text on screen, where you stand.
volatile struct { u8 battle, talking, map, x, y, dir, song; } QA __attribute__((section(".data.0qa"))) = {0}; // always at 0x03000000 (gba.ld), for gbarun scripts

// ---------- text surface: BG0 tiles are handed out on first touch, so any pixel on screen can be drawn,
// like the JS canvas. Colours of palette 15: 1 paper, 2 box edge, 3 box inner line, 4 ink, 5 ink shadow.
enum { C_PAPER = 1, C_EDGE, C_INNER, C_INK, C_SHADOW, C_RED, C_BFILL, C_BEDGE, C_BINNER, C_BSHADOW, C_HILITE, C_GRAY, C_BALLDK, C_BALLRED, C_GRAY2 };
static int pen = TEXT_PAL; // palette bank for cells touched next (battle HUD boxes use 13 and 14)
#define TEXT_TILES ((volatile u32 *)CHARBLOCK(CBB_TEXT))
static u16 cell[20][30];
static int next_tile = 1;
static void surf_clear(void) {
  volatile u32 *m = (volatile u32 *)SCREENBLOCK(SBB_TEXT);
  for (int i = 0; i < 512; i++) m[i] = 0;
  for (int y = 0; y < 20; y++) for (int x = 0; x < 30; x++) cell[y][x] = 0;
  next_tile = 1;
}
// The drawing hot path runs as ARM code from IWRAM: no ROM wait states, a 32-bit bus (cartridge ROM is ~5x slower).
FAST static volatile u32 *cell_tile(int cx, int cy);
FAST static void px(int c, int x, int y, int w, int h);
FAST static void glyph_row(int bits, int c, int x, int y);
static volatile u32 *cell_tile(int cx, int cy) {
  int e = cell[cy][cx], t = e & 1023;
  if (!t) {
    if (next_tile >= 768) return TEXT_TILES; // tiles 512..767 run on into charblock 3 up to screenblock 28; past that, the blank tile
    t = next_tile++;
    for (int i = 0; i < 8; i++) TEXT_TILES[t * 8 + i] = 0;
  }
  if (e != (t | (pen << 12))) cell[cy][cx] = SCREENBLOCK(SBB_TEXT)[cy * 32 + cx] = t | (pen << 12);
  return TEXT_TILES + t * 8;
}
// Fill a rectangle: per tile cell, look the tile up once, then one masked write per pixel row inside it.
static void px(int c, int x, int y, int w, int h) {
  int x0 = x < 0 ? 0 : x, x1 = x + w > 240 ? 240 : x + w, y0 = y < 0 ? 0 : y, y1 = y + h > 160 ? 160 : y + h;
  if (x0 >= x1 || y0 >= y1) return;
  u32 fill = (u32)c * 0x11111111u;
  for (int cy = y0 >> 3; cy <= (y1 - 1) >> 3; cy++) {
    int r0 = cy * 8 < y0 ? y0 & 7 : 0, r1 = cy * 8 + 8 > y1 ? ((y1 - 1) & 7) + 1 : 8;
    for (int cx = x0 >> 3; cx <= (x1 - 1) >> 3; cx++) {
      int a = cx * 8 < x0 ? x0 & 7 : 0, b = cx * 8 + 8 > x1 ? ((x1 - 1) & 7) + 1 : 8;
      u32 mask = (b == 8 ? 0xFFFFFFFFu : (1u << (b * 4)) - 1) & ~((1u << (a * 4)) - 1);
      volatile u32 *t = cell_tile(cx, cy);
      if (mask == 0xFFFFFFFFu) for (int r = r0; r < r1; r++) t[r] = fill;
      else for (int r = r0; r < r1; r++) t[r] = (t[r] & ~mask) | (fill & mask);
    }
  }
}
static int glyph(int ch) { return ch > 0 && ch < FONT_GLYPHS ? ch : 63; }
static int measure(const char *s) { int w = 0; for (; *s; s++) w += FONT_W[glyph((u8)*s)]; return w; }
// One row of a glyph (bit b = pixel b) in colour c at x: spread the bits to nibbles, then one masked write per tile.
static void glyph_row(int bits, int c, int x, int y) {
  if (y < 0 || y >= 160 || !bits) return;
  u32 m = 0; for (int b = 0; b < 8; b++) if ((bits >> b) & 1) m |= 15u << (b * 4);
  u32 fill = (u32)c * 0x11111111u; int sh = (x & 7) * 4, cx = x >> 3;
  u32 lo = m << sh, hi = sh ? m >> (32 - sh) : 0;
  if (cx >= 0 && cx < 30 && lo) { volatile u32 *row = cell_tile(cx, y >> 3) + (y & 7); *row = (*row & ~lo) | (fill & lo); }
  if (cx + 1 >= 0 && cx + 1 < 30 && hi) { volatile u32 *row = cell_tile(cx + 1, y >> 3) + (y & 7); *row = (*row & ~hi) | (fill & hi); }
}
static void text_s(const char *s, int x, int y, int ink, int shadow, int sc) { // sc: pixel size (chapter titles are 2)
  for (int pass = shadow ? 0 : 1; pass < 2; pass++) {
    int c = pass ? ink : shadow, d = pass ? 0 : sc, cx = x;
    for (const char *p = s; *p; p++) {
      int k = glyph((u8)*p);
      for (int r = 0; r < 8; r++) {
        int bits = FONT_LO[k] >= 0 ? FONT_BITS[k * 8 + r] >> FONT_LO[k] : FONT_BITS[k * 8 + r] << -FONT_LO[k];
        if (sc == 1) { if (cx + d >= -8) glyph_row(bits, c, cx + d, y + r + d); }
        else for (int b = 0; b < 8; b++) if ((bits >> b) & 1) px(c, cx + b * sc + d, y + r * sc + d, sc, sc);
      }
      cx += FONT_W[k] * sc;
    }
  }
}
static void text(const char *s, int x, int y, int ink, int shadow) { text_s(s, x, y, ink, shadow, 1); }
// Gen 3 dialog box: white, navy edge, pale blue inner line, rounded corners (game.js box()).
static void box_c(int x, int y, int w, int h, int fill, int edge, int inner) {
  px(edge, x + 1, y, w - 2, h); px(edge, x, y + 1, w, h - 2);
  px(inner, x + 2, y + 1, w - 4, h - 2); px(inner, x + 1, y + 2, w - 2, h - 4);
  px(fill, x + 3, y + 2, w - 6, h - 4); px(fill, x + 2, y + 3, w - 4, h - 6);
  px(0, x, y, 1, 1); px(0, x + w - 1, y, 1, 1); px(0, x, y + h - 1, 1, 1); px(0, x + w - 1, y + h - 1, 1, 1); // rounded: the corners show through
}
static void box(int x, int y, int w, int h) { box_c(x, y, w, h, C_PAPER, C_EDGE, C_INNER); }
static int rtext_w(const char *s, int right) { return right - measure(s); }
static void load_text_pal(void) {
  static const u32 cols[] = { 0xf8f8f8, 0x48506c, 0xa0a8d0, 0x404040, 0xd0d0c8, 0xe05040, 0x384870, 0x283050, 0xa8b0d8, 0x605870, 0xf8d8a0, 0xa0a0a0, 0x303030, 0xe04038, 0x909090 };
  for (int i = 0; i < 15; i++) PAL_BG[TEXT_PAL * 16 + 1 + i] = HEX(cols[i]);
}

// ---------- game state, saved to cartridge SRAM (mGBA finds the save type by this string)
const char SAVE_TYPE[] = "SRAM_V113";
#define SRAM ((volatile u8 *)0x0E000000)
_Static_assert(FLAG_COUNT <= 512, "the save holds 512 story flags");
#define SAVE_MAGIC 0x3A4A4F54 // bump when the Save layout changes (HYPER and MAX POTION grew the bag)
// a creature you own or meet (game.js makeMon): moves are move ids with their PP left, id 0 empty
typedef struct { u16 id; u8 pp, pad; } Slot;
typedef struct { u8 id, level, status, sleep; u16 hp, stats[6]; u32 exp; u8 ivs[6]; Slot moves[4]; } Mon;
// rope: map+1, x, y, dir of the cave mouth you came in by (ESCAPE ROPE)
typedef struct { u32 magic, money; u8 map, x, y, dir, starter, repel, speed, mute, surf, bike; u8 heal[4], rope[4]; u8 flags[64]; /* 512 flags; FLAG_COUNT stays under */ u8 bag[ITEM_COUNT]; u8 nparty, nbox; Mon party[6], box[30]; u8 seen[16], caught[16]; u16 hall_best, hall_pad; } Save;
static Save S;
static int flag(int f) { return (S.flags[f >> 3] >> (f & 7)) & 1; }
static void set_flag(int f) { S.flags[f >> 3] |= 1 << (f & 7); }
static void sram_write(void) { const u8 *p = (const u8 *)&S; for (unsigned i = 0; i < sizeof S; i++) SRAM[i] = p[i]; }
static int sram_read(void) {
  (void)*(volatile const char *)SAVE_TYPE;
  Save t; u8 *p = (u8 *)&t; for (unsigned i = 0; i < sizeof t; i++) p[i] = SRAM[i];
  if (t.magic != SAVE_MAGIC) return 0;
  S = t; return 1;
}
// a small string builder for lines like "JOSH found a POTION!"
static char buf[200]; static int blen;
static void cat(const char *s) { while (*s && blen < 199) buf[blen++] = *s++; buf[blen] = 0; }
static void cat_num(int n) { char t[12]; int i = 0; do t[i++] = '0' + n % 10; while (n /= 10); while (i && blen < 199) buf[blen++] = t[--i]; buf[blen] = 0; }

// ---------- maps
static const MapInfo *mi;
static int cur = -1, camx, camy, lastcx, lastcy;
static void load_tile(int slot, int bankTile) { copy32((volatile u32 *)CHARBLOCK(CBB_MAP) + slot * 16, TILE_BANK + bankTile * 16, 16); }
static void stream(void) {
  const MapGfx *m = &MAP_GFX[cur];
  int cx = camx >> 3, cy = camy >> 3;
  if (cx == lastcx && cy == lastcy) return;
  lastcx = cx; lastcy = cy;
  volatile u16 *gm = SCREENBLOCK(SBB_GROUND), *om = SCREENBLOCK(SBB_OVER);
  for (int y = cy; y < cy + 21; y++) for (int x = cx; x < cx + 31; x++) {
    int in = x >= 0 && y >= 0 && x < m->cw && y < m->ch, i = y * m->cw + x, s = (y & 31) * 32 + (x & 31);
    gm[s] = in ? m->ground[i] : 0;
    om[s] = in ? m->over[i] : 0;
  }
}
static void animate(void) {
  if (frame & 31) return;
  const MapGfx *m = &MAP_GFX[cur];
  for (int i = 0; i < m->nanim; i++) load_tile(m->anim[i * 3], m->anim[i * 3 + 1 + ((frame >> 5) & 1)]);
}

// ---------- sprites: each look on the map gets a slot (6 frames of tiles and its own palette bank)
typedef struct { u16 a0, a1, a2, pad; } Obj;
static Obj oam[128];
static s8 slot_of[LOOK_COUNT];
static int nslots;
static int load_look(int look) {
  if (slot_of[look] >= 0) return slot_of[look];
  int s = nslots < 15 ? nslots++ : 0;
  slot_of[look] = s;
  copy32((volatile u32 *)OBJ_TILES + s * SLOT_TILES * 8, SPRITE_TILES + look * SLOT_TILES * 8, SLOT_TILES * 8);
  for (int i = 0; i < 16; i++) PAL_OBJ[s * 16 + i] = SPRITE_PAL[look * 16 + i];
  return s;
}
// The JS shadow is a 10x2 dark rect 14px under the tile top; here a semi-transparent sprite darkens the ground.
static void load_shadow(void) {
  volatile u32 *t = (volatile u32 *)OBJ_TILES + SHADOW_TILE * 8;
  for (int i = 0; i < 16; i++) t[i] = 0;
  t[6] = t[7] = 0x11111000; t[14] = t[15] = 0x00011111;
  PAL_OBJ[SHADOW_PAL * 16 + 1] = 0;
}

// ---------- the walk (game.js worldStep, arrive, warp)
static const s8 DX[4] = { 0, 0, -1, 1 }, DY[4] = { 1, -1, 0, 0 };
static const u8 HOP[33] = { 0,1,2,3,4,5,6,6,7,8,8,9,9,10,10,10,10,10,10,10,9,9,8,8,7,6,6,5,4,3,2,1,0 };
static const char SOLID[] = "KYOnT~RBGHWSLwvrPychXbFQ";
static int me_x, me_y, me_dir, moving, off, speed, turn, walk, hop, fade_level;
static u8 ndir[16], nx[16], ny[16], noff[16]; // people move in cutscenes and when a trainer walks up: their live place
static int shake_x, legend_on, rustle_x, rustle_y, rustle_t = -99; // rustle: tall grass you just stepped into shakes // shake_x: the whole world jolts sideways (game.js ui.shake)
static int popup = -1, wipe; // wipe 0..20: black bars closing from top and bottom (game.js ui.wipe)

static int has(const char *s, char c) { for (; *s; s++) if (*s == c) return 1; return 0; }
// the map's tiles, copied on entry so CUT, ROCK SMASH and STRENGTH can change them until you leave (game.js loadMap)
static char rows[40 * 40]; static int map_strength, map_flash;
static char tile_at(int x, int y) { return x < 0 || y < 0 || x >= mi->w || y >= mi->h ? 'T' : rows[y * mi->w + x]; }
static void set_tile(int x, int y, char c) { rows[y * mi->w + x] = c; }
static char ground(void) { return mi->flags & MF_CAVE ? 'q' : !(mi->flags & MF_OUTDOOR) ? 'o' : mi->border == 'F' ? 's' : '.'; } // what a cleared rock or moved boulder leaves
static int cond_ok(const u16 *c) { for (; c && *c; c++) if (flag(*c & 0x7FFF) == !!(*c & 0x8000)) return 0; return 1; }
static int visible(int i) { const Npc *n = &mi->npcs[i]; return cond_ok(n->cond) && !((n->flags & NF_ITEM) && flag(n->flag)); }
static int npc_at(int x, int y) { for (int i = 0; i < mi->nnpcs; i++) if (visible(i) && nx[i] == x && ny[i] == y) return i; return -1; }
static int solid(int x, int y) { return has(SOLID, tile_at(x, y)) || npc_at(x, y) >= 0; }

static void load_shadow(void);
// Dark caves (game.js darkness): black everywhere but a circle of light round you, a dithered ring at its edge; FLASH
// widens it. You are always mid-screen there, so the darkness is a still layer: BG3, 4bpp tiles from charblock 1
// slot 128 on (past the map's 8bpp tiles), screenblock 28, black from palette bank 13.
static int world_dispcnt(void) { return MODE0 | BG0_ON | BG1_ON | BG2_ON | OBJ_ON | OBJ_1D | ((mi && (mi->flags & MF_DARK)) ? BG3_ON : 0); }
static void dark_setup(void) {
  if (!(mi->flags & MF_DARK)) return;
  int r = map_flash ? 72 : 22, x0 = 120, y0 = 82, r6 = (r + 6) * (r + 6), r2 = r * r, n = 0;
  volatile u32 *tiles = (volatile u32 *)CHARBLOCK(1) + 128 * 8; volatile u16 *map = SCREENBLOCK(28);
  static u32 seen[200][8];
  for (int cy = 0; cy < 32; cy++) for (int cx = 0; cx < 32; cx++) {
    u32 t[8] = { 0 };
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
      int tx = (cx * 8 + x) & ~1, ty = (cy * 8 + y) & ~1, dx = tx - x0, dy = ty - y0, d = dx * dx + dy * dy;
      if (cx >= 30 || cy >= 20 || d > r6 || (d > r2 && ((tx + ty) >> 1) % 2)) t[y] |= 1u << (x * 4);
    }
    int k = 0; while (k < n && (seen[k][0] != t[0] || seen[k][1] != t[1] || seen[k][2] != t[2] || seen[k][3] != t[3] || seen[k][4] != t[4] || seen[k][5] != t[5] || seen[k][6] != t[6] || seen[k][7] != t[7])) k++;
    if (k == n && n < 200) { for (int i = 0; i < 8; i++) { seen[n][i] = t[i]; tiles[n * 8 + i] = t[i]; } n++; }
    map[cy * 32 + cx] = (128 + k) | (13 << 12);
  }
  PAL_BG[13 * 16 + 1] = 0;
  REG_BG3CNT = BG_CBB(1) | BG_SBB(28) | BG_PRIO(0); REG_BG3HOFS = REG_BG3VOFS = 0;
}
// Tiles, palette and sprite slots of the current map: on entering it, and again after a battle borrowed the VRAM.
static void load_map_gfx(void) {
  const MapGfx *m = &MAP_GFX[cur];
  for (int i = 0; i < MAP_PAL_SIZE; i++) PAL_BG[i] = MAP_PAL[i];
  for (int i = 0; i < m->ntiles; i++) load_tile(i, m->tiles[i]);
  lastcx = lastcy = -999;
  for (int i = 0; i < LOOK_COUNT; i++) slot_of[i] = -1;
  nslots = 0; load_look(LOOK_PLAYER); load_look(LOOK_GRASS);
  for (int i = 0; i < mi->nnpcs; i++) if (!(mi->npcs[i].flags & NF_NOSPRITE)) load_look(mi->npcs[i].look);
  load_shadow();
  { volatile u32 *t = (volatile u32 *)OBJ_TILES + 900 * 8; t[0] = t[1] = 0x11; t[2] = 0x22; for (int r = 3; r < 8; r++) t[r] = 0; PAL_OBJ[13 * 16 + 1] = HEX(0x42b584); PAL_OBJ[13 * 16 + 2] = HEX(0x395200); }
  for (int i = 0; i < mi->w * mi->h; i++) if (rows[i] == 'K' || rows[i] == 'Y' || rows[i] == 'O') load_look(LOOK_OBSK + (rows[i] == 'Y') + (rows[i] == 'O') * 2);
  if (S.surf) load_look(LOOK_SURF);
  if (S.bike) load_look(LOOK_BIKE);
  legend_on = 0;
  for (int i = 0; i < mi->nnpcs; i++) if (mi->npcs[i].legend) { // a 64x64 creature standing on the map
    const u32 *src = ART_TILES + ART_MON(mi->npcs[i].legend, 0) * 64 * 8; copy32((volatile u32 *)OBJ_TILES + 768 * 8, src, 64 * 8);
    for (int k = 0; k < 16; k++) PAL_OBJ[14 * 16 + k] = ART_PAL[ART_MON(mi->npcs[i].legend, 0) * 16 + k];
    legend_on = 1;
  }
  dark_setup();
}
static int map_song(void);
static void load_map(int id) {
  cur = id; mi = &MAP_INFO[id]; map_strength = map_flash = 0;
  for (int i = 0; i < mi->w * mi->h; i++) rows[i] = mi->rows[i];
  for (int i = 0; i < mi->nnpcs; i++) { ndir[i] = mi->npcs[i].dir; nx[i] = mi->npcs[i].x; ny[i] = mi->npcs[i].y; noff[i] = 0; }
  load_map_gfx();
  music(map_song());
}
static void show_name(void) {
  if (!(mi->flags & MF_OUTDOOR)) return;
  surf_clear(); box(4, -2, 104, 26); text(mi->name, 12, 9, C_INK, C_SHADOW);
  popup = 0;
}
// Outdoors the camera follows you past the edge (the border trees fill it); small rooms sit centred on black.
// game.js mapMusic: riding the water or the BIKE has its own tune
static int map_song(void) { return S.surf ? SONG_SURF : S.bike ? SONG_BIKE : mi->song; }
static int fit(int o, int size, int view) { return !(mi->flags & MF_OUTDOOR) && size <= view ? (size - view) / 2 : o + 8 - view / 2; }

static int nobj;
static void put(int sx, int sy, u16 a0, u16 a1, u16 a2) {
  if (nobj >= 128 || sx <= -64 || sx >= 240 || sy <= -64 || sy >= 160) return;
  oam[nobj++] = (Obj){ (sy & 255) | a0, (sx & 511) | a1, a2, 0 };
}
// One person or thing: depth order is screen y, nearer the bottom drawn on top.
typedef struct { int y, sx, sy, look, dir, step; } Thing;
static int in_battle, team_open, evo_open, scene_open;
static void render_battle(void), render_team(void), render_evolve(void), render_scene(void);
#include "main_2.inc"
#include "main_3.inc"
