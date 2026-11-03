// gba/tools/paint.c : the map and people painter, a host tool (cc). It paints every map from the world data, cuts it
// into 8x8 tiles for the cartridge, and bakes the people, things and trainer portraits into OBJ tiles.
// Ported line for line from the JS game (game.js tile(), edges(), drawTiles(), exportMap(), bakePerson(),
// bakePortrait(), drawThing() and gba/tools/maps.mjs), so the cartridge needs no JavaScript to grow new maps.
// Built and run by the Makefile: cc -O2 -I data tools/paint.c data/world.c -o build/paint && build/paint build
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"

// ---------- a canvas: RGBA, source-over blending with Skia's rounding (what Chrome's 2D canvas does)
typedef struct { int w, h; unsigned char *p; } Cv;
static Cv cv(int w, int h) { Cv c = { w, h, calloc((size_t)w * h, 4) }; return c; }
static int clip_y1 = 1 << 30; // only the canopy pass clips, and only from below
static unsigned mul255(unsigned a, unsigned b) { unsigned p = a * b + 128; return (p + (p >> 8)) >> 8; }
static void blend(unsigned char *d, unsigned r, unsigned g, unsigned b, unsigned a) {
  if (a == 255) { d[0] = r; d[1] = g; d[2] = b; d[3] = 255; return; }
  if (!a) return;
  if (d[3] == 255) { // over an opaque pixel: Skia's blit, src premultiplied then dst * (256 - a) >> 8 (measured equal to Chrome)
    d[0] = (r * a + 127) / 255 + ((d[0] * (256 - a)) >> 8); d[1] = (g * a + 127) / 255 + ((d[1] * (256 - a)) >> 8); d[2] = (b * a + 127) / 255 + ((d[2] * (256 - a)) >> 8);
    return;
  }
  // over a clear or part-clear pixel: premultiply the backing store, blend, unpremultiply like getImageData
  unsigned da = d[3], dr = mul255(d[0], da), dg = mul255(d[1], da), db = mul255(d[2], da);
  unsigned oa = a + mul255(da, 255 - a);
  unsigned orr = mul255(r, a) + mul255(dr, 255 - a), og = mul255(g, a) + mul255(dg, 255 - a), ob = mul255(b, a) + mul255(db, 255 - a);
  d[3] = oa;
  if (oa == 255) { d[0] = orr; d[1] = og; d[2] = ob; }
  else { d[0] = (orr * 255 + oa / 2) / oa; d[1] = (og * 255 + oa / 2) / oa; d[2] = (ob * 255 + oa / 2) / oa; }
}
// colour as "#rrggbb" or "#rrggbbaa" in the JS, here 0xRRGGBB plus an alpha
static void fill_a(Cv *c, unsigned rgb, unsigned a, int x, int y, int w, int h) {
  for (int yy = y; yy < y + h; yy++) for (int xx = x; xx < x + w; xx++) {
    if (xx < 0 || yy < 0 || xx >= c->w || yy >= c->h || yy >= clip_y1) continue;
    blend(c->p + ((size_t)yy * c->w + xx) * 4, rgb >> 16, (rgb >> 8) & 255, rgb & 255, a);
  }
}
static void fill(Cv *c, unsigned rgb, int x, int y, int w, int h) { fill_a(c, rgb, 255, x, y, w, h); }
static void draw(Cv *dst, const Cv *src, int sx, int sy, int sw, int sh, int dx, int dy) { // drawImage, no scaling
  for (int y = 0; y < sh; y++) for (int x = 0; x < sw; x++) {
    int X = dx + x, Y = dy + y; if (X < 0 || Y < 0 || X >= dst->w || Y >= dst->h || Y >= clip_y1) continue;
    const unsigned char *s = src->p + ((size_t)(sy + y) * src->w + sx + x) * 4;
    blend(dst->p + ((size_t)Y * dst->w + X) * 4, s[0], s[1], s[2], s[3]);
  }
}
static int opaque(const Cv *c, int x, int y) { return x >= 0 && y >= 0 && x < c->w && y < c->h && c->p[((size_t)y * c->w + x) * 4 + 3] > 0; }
static unsigned shade(unsigned rgb, double f) { // game.js shade: each channel times f, rounded
  unsigned r = (unsigned)lround((rgb >> 16) * f), g = (unsigned)lround(((rgb >> 8) & 255) * f), b = (unsigned)lround((rgb & 255) * f);
  return r << 16 | g << 8 | b;
}
static int hash(int x, int y) { return (int)((((unsigned)x * 73856093u) ^ ((unsigned)y * 19349663u)) % 997); }
static int has(const char *s, char c) { return c && strchr(s, c); }

// ---------- tiles (game.js 85-199): our own pixel art, GBA palette
enum { GRASS = 0x73c6a5, GRASS_HI = 0x8ad4b4, GRASS_LO = 0x5fb898, PATH = 0xa5d6c6, PATH_HI = 0xc4e8da, SPECK = 0xb5ff84, LEAF_HI = 0xb5ff84, LEAF = 0x84c663,
  LEAF_MID = 0x398c31, LEAF_DK = 0x395200, SNOW = 0xf0f4f8, SNOW_LO = 0xd0dcec, SNOW_SH = 0xa8b8d4, TALL = 0x18a56b, TALL_HI = 0x42b584, WATER = 0x5aa0e0, WATER_HI = 0x9cd0f8, SHADOW = 0x4aa88a };
static Cv T_SAND, T_DUNE, T_CACTUS, T_PATH_SAND, T_S_SAND, T_BRICK_SUN, T_SLATE_SUN; enum { SAND = 0xecd49c, SAND_LO = 0xd4b478 };
static Cv T_DOT, T_F[2], T_FERN, T_PATH, T_TREE_GROUND, T_W[2], T_R, T_H, T_WIN, T_D, T_B, T_S, T_G, T_K_, T_X[3], T_Q, T_Z, T_J, T_CUT, T_Y, T_O, T_E, T_N, T_L, T_WALL, T_V, T_RR, T_FLOOR, T_U, T_P, T_YY, T_C, T_SH, T_M, T_BRICK, T_SLATE, T_STAIRS, TREE, T_SNOW, T_DRIFT, TREE_SNOW, T_S_SNOW, T_PATH_SNOW, T_ICE, T_ICEWALL, T_FROSTFLOOR;
static Cv *t; static void p(unsigned c, int x, int y, int w, int h) { fill(t, c, x, y, w, h); }
static void p1(unsigned c, int x, int y) { p(c, x, y, 1, 1); }
static void grass(void) {
  p(GRASS, 0, 0, 16, 16);
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) {
    if ((x + y * 3) % 11 == 0 && (x * y) % 3 != 1) p1(GRASS_HI, x, y);
    else if ((x * 5 + y) % 13 == 0) p1(GRASS_LO, x, y);
  }
  static const int T[4][2] = { { 2, 3 }, { 10, 6 }, { 5, 11 }, { 13, 13 } };
  for (int i = 0; i < 4; i++) { int x = T[i][0], y = T[i][1]; p1(GRASS_LO, x, y); p1(GRASS_LO, x + 2, y); p1(GRASS_LO, x + 1, y + 1); p1(GRASS_HI, x + 1, y - 1); }
}
static void wall(void) { p(0xf0e4c4, 0, 0, 16, 9); p(0xe0d0a8, 0, 8, 16, 1); p(0xc8a070, 0, 9, 16, 6); for (int x = 1; x < 16; x += 5) p(0xb08860, x, 9, 1, 6); p(0x7a5838, 0, 15, 16, 1); }
static void floor_(void) { p(0xf8ecb8, 0, 0, 16, 16); p(0xecd898, 0, 0, 16, 1); p(0xecd898, 0, 8, 16, 1); p(0xecd898, 0, 0, 1, 16); p(0xecd898, 8, 0, 1, 16); p1(0xf0e0a8, 4, 4); p1(0xf0e0a8, 12, 12); }
static void roof(unsigned base, unsigned line) { p(base, 0, 0, 16, 16); for (int y = 2; y < 16; y += 4) { p(line, 0, y, 16, 1); for (int x = (y / 2) % 4; x < 16; x += 4) p(line, x, y - 2, 1, 2); } }
static void slab(int x, int y, int w, int h) {
  p(0x3a2a1c, x, y, w, h); p(0x7a5c40, x, y, w - 1, h - 1); p(0x8a6c4c, x + 1, y, w - 3, 2); p(0xa08060, x + 1, y, (w >> 1) > 1 ? w >> 1 : 1, 1); p(0x5a4430, x, y + h - 3, w - 1, 2);
  p1(0x3a2a1c, x, y); p1(0x3a2a1c, x + w - 2, y); p1(0x3a2a1c, x, y + h - 2); p(0x4a3626, x + 1, y + h - 1, w - 2, 1);
}
static double sq(double v) { return v * v; }
static void make_tiles(void) {
  t = &T_DOT; *t = cv(16, 16); grass();
  for (int k = 0; k < 2; k++) {
    t = &T_F[k]; *t = cv(16, 16); grass();
    static const int F[2][3] = { { 4, 4, 0xf87070 }, { 11, 9, 0xf8f8f8 } };
    for (int i = 0; i < 2; i++) { int x = F[i][0], y = F[i][1], c = F[i][2], s = k; p(LEAF_MID, x - 1, y + 3, 3, 1); p(TALL, x, y + 2, 1, 2); p(c, x - 1 + s, y, 3, 1); p(c, x + s, y - 1, 1, 3); p1(0xf8d048, x + s, y); }
  }
  static const char *FERN[16] = { "..d....dd....d..", ".dad..dabd..dad.", ".dabd.dabd.dbad.", "..dabddabddbad..", "d..dabdabdbad..d", "ad..dabbbbad..da", "bad..dabbad..dab", "abad.dabbad.daba",
    ".dbaddabbaddbad.", "..dbbdabbadbbd..", "d..dbbabbabbd..d", "ad..dbbbbbbd..da", "bad..dbbbbd..dab", "dbad..dbbd..dabd", ".dbbd.dbbd.dbbd.", "..dd...dd...dd.." };
  t = &T_FERN; *t = cv(16, 16); grass();
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) { char ch = FERN[y][x]; if (ch != '.') p1(ch == 'd' ? LEAF_DK : ch == 'a' ? TALL_HI : TALL, x, y); }
  t = &T_PATH; *t = cv(16, 16); p(PATH, 0, 0, 16, 16); for (int i = 0; i < 10; i++) p1(i % 3 ? SPECK : PATH_HI, hash(i, 9) % 16, hash(9, i) % 16);
  t = &T_TREE_GROUND; *t = cv(16, 16); grass();
  // conifer: four stacked leaf tiers, lit from the top left
  t = &TREE; *t = cv(22, 28);
  for (int y = 22; y < 28; y++) for (int x = 1; x < 21; x++) if (sq((x - 10.5) / 9) + sq((y - 25) / 3.0) <= 1) p1(SHADOW, x, y);
  static const double TI[3][4] = { { 10.5, 4.5, 4.2, 4 }, { 10.5, 10, 7, 5 }, { 10.5, 16.5, 9.4, 6 } };
#define INSIDE(X, Y) ({ int _k = -1; for (int _i = 0; _i < 3; _i++) if (sq(((X) + 0.5 - TI[_i][0]) / TI[_i][2]) + sq(((Y) + 0.5 - TI[_i][1]) / TI[_i][3]) <= 1) { _k = _i; break; } _k; })
  for (int y = 0; y < 24; y++) for (int x = 0; x < 22; x++) {
    int k = -1; for (int i = 2; i >= 0; i--) if (sq((x + 0.5 - TI[i][0]) / TI[i][2]) + sq((y + 0.5 - TI[i][1]) / TI[i][3]) <= 1) { k = i; break; }
    if (k < 0) continue;
    double nx = (x + 0.5 - TI[k][0]) / TI[k][2], ny = (y + 0.5 - TI[k][1]) / TI[k][3];
    int edge = INSIDE(x + 1, y) < 0 || INSIDE(x, y + 1) < 0 || INSIDE(x - 1, y) < 0;
    double light = -nx * 0.5 - ny * 0.7 + ((hash(x, y) % 9) - 4) * 0.1;
    p1(edge || ny > 0.72 ? LEAF_DK : light > 0.8 ? LEAF_HI : light > 0.25 ? LEAF : light > -0.45 ? LEAF_MID : LEAF_DK, x, y);
  }
  static const int TR[5][2] = { { 9, 22 }, { 10, 22 }, { 11, 22 }, { 10, 23 }, { 11, 23 } };
  for (int i = 0; i < 5; i++) p1(0x4a3a2a, TR[i][0], TR[i][1]);
  for (int k = 0; k < 2; k++) { t = &T_W[k]; *t = cv(16, 16); p(WATER, 0, 0, 16, 16); for (int y = 2 + k * 3; y < 16; y += 6) { int x = (y * 5 + k * 6) % 10; p(WATER_HI, x, y, 4, 1); p(WATER_HI, x + 1, y - 1, 2, 1); } }
  t = &T_R; *t = cv(16, 16); roof(0xd06048, 0xa83c2c);
  t = &T_H; *t = cv(16, 16); p(0xf0e8d0, 0, 0, 16, 16); p(0xd8ccb0, 0, 0, 16, 1); p(0xc8b898, 0, 14, 16, 2); for (int x = 3; x < 16; x += 8) p(0xe0d4b8, x, 1, 1, 13);
  t = &T_WIN; *t = cv(16, 16); p(0xf0e8d0, 0, 0, 16, 16); p(0xc8b898, 0, 14, 16, 2); p(0x706050, 2, 2, 12, 10); p(0x90c0e8, 3, 3, 10, 8); p(0xd8f0f8, 4, 4, 3, 2); p(0x706050, 7, 3, 2, 8); p(0x706050, 3, 7, 10, 1);
  t = &T_D; *t = cv(16, 16); p(0xf0e8d0, 0, 0, 16, 16); p(0x584030, 2, 0, 12, 16); p(0x987048, 3, 1, 10, 15); p(0xb08858, 4, 2, 3, 13); p(0xf8d048, 11, 8, 1, 2); p(0xa07850, 3, 15, 10, 1);
  t = &T_B; *t = cv(16, 16); roof(0x5888d8, 0x3860a8);
  t = &T_S; *t = cv(16, 16); grass(); p(0x604020, 7, 9, 2, 6); p(0x503018, 2, 2, 12, 8); p(0xc89858, 3, 3, 10, 6); p(0xa87840, 4, 5, 8, 1); p(0xa87840, 4, 7, 6, 1);
  t = &T_G; *t = cv(16, 16); roof(0x9868c0, 0x704898);
  t = &T_K_; *t = cv(16, 16); p(0xb08858, 0, 0, 16, 16); for (int y = 3; y < 16; y += 4) p(0x806040, 0, y, 16, 1); for (int x = 0; x < 16; x += 8) p(0x987048, x + (x ? 3 : 5), 0, 1, 16); p(0x584030, 0, 0, 1, 16); p(0x584030, 15, 0, 1, 16);
  static const int RK[3][3][4] = { { { 1, 1, 14, 7 }, { 0, 8, 8, 8 }, { 8, 9, 8, 7 } }, { { 0, 0, 16, 9 }, { 2, 9, 13, 7 }, { 0 } }, { { 0, 0, 7, 7 }, { 7, 1, 9, 7 }, { 1, 8, 15, 8 } } };
  for (int k = 0; k < 3; k++) { t = &T_X[k]; *t = cv(16, 16); p(0x4a3626, 0, 0, 16, 16); for (int r = 0; r < 3; r++) if (RK[k][r][2]) slab(RK[k][r][0], RK[k][r][1], RK[k][r][2], RK[k][r][3]); }
  t = &T_Q; *t = cv(16, 16); p(0xb89870, 0, 0, 16, 16); for (int i = 0; i < 8; i++) p(i % 2 ? 0xa08060 : 0xc8aa82, hash(i, 5) % 15, hash(5, i) % 15, 2, 1);
  t = &T_Z; *t = cv(16, 16); p(0x687080, 0, 0, 16, 16); p(0x7a8292, 1, 1, 14, 6); p(0x7a8292, 1, 9, 14, 6); p(0x4a5262, 0, 7, 16, 2); p(0x4a5262, 7, 0, 1, 7); p(0x4a5262, 11, 9, 1, 7);
  p1(0xa0a8b8, 2, 2); p1(0xa0a8b8, 13, 2); p1(0xa0a8b8, 2, 13); p1(0xa0a8b8, 13, 13);
  t = &T_J; *t = cv(16, 16); p(0x5a7a48, 0, 0, 16, 16); for (int i = 0; i < 10; i++) p(i % 2 ? 0x4a6a3c : 0x6a8a54, hash(i, 11) % 15, hash(11, i) % 15, 2, 1);
  p(0x3a6878, 2, 9, 6, 3); p(0x5a90a0, 3, 9, 3, 1); p(0x3a6878, 10, 3, 4, 2); p(0x5a90a0, 11, 3, 2, 1);
  { static const int R[4][2] = { { 9, 7 }, { 11, 9 }, { 13, 6 }, { 4, 5 } }; for (int i = 0; i < 4; i++) { int x = R[i][0], h = R[i][1]; p(0x2a4a20, x, 15 - h, 1, h); p(0x8aa060, x, 15 - h, 1, 1); } }
  t = &T_CUT; *t = cv(16, 16); grass(); p(0x2a4a10, 3, 2, 10, 11); p(0x395200, 2, 4, 12, 7); p(0x398c31, 4, 3, 8, 8); p(0x84c663, 5, 3, 4, 3); p(0xb5ff84, 6, 4, 2, 1); p(0x398c31, 3, 6, 10, 3);
  p(0x604020, 7, 12, 2, 3); p(0x2a4a10, 6, 15, 4, 1); p(0x2a4a10, 7, 7, 1, 3);
  t = &T_Y; *t = cv(16, 16); p(0xb89870, 0, 0, 16, 16); p(0x3a2a1c, 2, 3, 12, 12); p(0x8a7a6a, 3, 3, 10, 10); p(0xb0a090, 4, 4, 5, 3); p(0x605040, 3, 12, 10, 2);
  { static const int C[10][2] = { { 8, 4 }, { 7, 5 }, { 7, 6 }, { 8, 7 }, { 9, 8 }, { 9, 9 }, { 5, 8 }, { 6, 9 }, { 11, 6 }, { 10, 7 } }; for (int i = 0; i < 10; i++) p1(0x3a2a1c, C[i][0], C[i][1]); }
  t = &T_O; *t = cv(16, 16); p(0xb89870, 0, 0, 16, 16); p(0x6a5038, 2, 13, 13, 3); p(0x3a2a1c, 2, 1, 12, 14); p(0x3a2a1c, 1, 3, 14, 10); p(0x9a9088, 2, 2, 12, 12); p(0x9a9088, 2, 3, 12, 9);
  p(0xc8c0b8, 4, 3, 6, 3); p(0xf0e8e0, 5, 4, 2, 1); p(0x6a6460, 3, 11, 11, 2); p(0x7a7470, 12, 5, 2, 7);
  t = &T_E; *t = cv(16, 16); p(0x6a5038, 0, 0, 16, 16); p(0x181010, 2, 3, 12, 13); p(0x281c14, 3, 2, 10, 2); p(0x4a3626, 1, 3, 1, 13); p(0x4a3626, 14, 3, 1, 13);
  t = &T_N; *t = cv(16, 16); grass(); p(SHADOW, 0, 14, 16, 2);
  p(0xa8b0b8, 0, 6, 16, 2); p(0xa8b0b8, 0, 11, 16, 2); p(0xf8f8f0, 0, 5, 16, 2); p(0xf8f8f0, 0, 10, 16, 2);
  for (int x = 2; x <= 10; x += 8) { p(0x788088, x, 2, 4, 13); p(0xf8f8f8, x, 1, 3, 13); p(0xd0d8e0, x + 2, 3, 1, 11); }
  t = &T_L; *t = cv(16, 16); grass(); p(GRASS_HI, 0, 8, 16, 1); p(TALL_HI, 0, 9, 16, 2); p(TALL, 0, 11, 16, 2); p(LEAF_DK, 0, 13, 16, 1); p(GRASS_LO, 0, 14, 16, 1);
  t = &T_WALL; *t = cv(16, 16); wall();
  t = &T_V; *t = cv(16, 16); wall(); p(0x7a5838, 3, 1, 10, 8); p(0xa8d8f8, 4, 2, 8, 6); p(0xf8f8f8, 5, 3, 3, 1); p(0x7a5838, 7, 2, 1, 6);
  t = &T_RR; *t = cv(16, 16); wall(); p(0x5878b8, 3, 1, 10, 7); p(0xf8d048, 5, 3, 6, 3); p(0xf8f8f8, 7, 4, 2, 1);
  t = &T_FLOOR; *t = cv(16, 16); floor_();
  t = &T_U; *t = cv(16, 16); p(0xd85848, 0, 0, 16, 16); p(0xf8c868, 1, 1, 14, 1); p(0xf8c868, 1, 14, 14, 1); for (int x = 3; x < 14; x += 4) p(0xb83c30, x, 4, 2, 8);
  t = &T_P; *t = cv(16, 16); floor_(); p(0x2f7a3a, 3, 1, 10, 8); p(0x58b048, 4, 1, 4, 3); p(0x58b048, 9, 3, 3, 3); p(0xa8e070, 5, 2, 1, 1); p(0xc86838, 4, 9, 8, 6); p(0xe08848, 5, 10, 3, 1); p(0x8a4020, 4, 14, 8, 1);
  t = &T_YY; *t = cv(16, 16); floor_(); p(0x783848, 1, 2, 14, 13); p(0xe87888, 2, 3, 12, 6); p(0xf8a8b0, 3, 4, 10, 1); p(0xc85868, 2, 9, 12, 5);
  t = &T_C; *t = cv(16, 16); p(0xf0e0b8, 0, 0, 16, 16); p(0x784828, 0, 3, 16, 13); p(0xa87048, 0, 3, 16, 4); p(0xc89868, 0, 3, 16, 1);
  t = &T_SH; *t = cv(16, 16); p(0xf0e0b8, 0, 0, 16, 16); p(0x684020, 1, 0, 14, 16);
  { static const int S[3][2] = { { 2, 0xe85848 }, { 7, 0x48a8e8 }, { 12, 0xf8c838 } }; for (int i = 0; i < 3; i++) { int y = S[i][0]; p(0x3a2410, 2, y + 3, 12, 1); for (int x = 3; x < 13; x += 3) p(S[i][1], x, y, 2, 3); } }
  // the LIGHTHOUSE (chapter 6): grey-blue stone bricks, slate floor, stairs going up
  t = &T_BRICK; *t = cv(16, 16); p(0x626878, 0, 0, 16, 16);
  for (int r = 0; r < 4; r++) { int y = r * 4; p(0x7e8496, 0, y, 16, 1); p(0x3e4452, 0, y + 3, 16, 1); for (int x = r % 2 ? 4 : 0; x < 16; x += 8) p(0x3e4452, x, y, 1, 3); }
  t = &T_SLATE; *t = cv(16, 16); p(0x9c9488, 0, 0, 16, 16); p(0x7a7266, 0, 0, 16, 1); p(0x7a7266, 0, 8, 16, 1); p(0x7a7266, 0, 0, 1, 16); p(0x7a7266, 8, 0, 1, 16);
  for (int i = 0; i < 6; i++) p(0xaca498, 1 + hash(i, 13) % 14, 1 + hash(13, i) % 14, 2, 1);
  t = &T_STAIRS; *t = cv(16, 16); p(0x9c9488, 0, 0, 16, 16);
  for (int y = 0; y < 16; y += 4) { p(0xb8b0a4, 2, y, 12, 1); p(0x5a5248, 2, y + 3, 12, 1); }
  p(0x4a4238, 0, 0, 2, 16); p(0x4a4238, 14, 0, 2, 16);
  // the north (chapter 7): packed snow with blue shadows, deep snow drifts where wild creatures hide, firs under snow
  t = &T_SNOW; *t = cv(16, 16); p(SNOW, 0, 0, 16, 16);
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) { if ((x * 7 + y * 3) % 17 == 0) p1(SNOW_LO, x, y); else if ((x + y * 5) % 23 == 0) p1(0xffffff, x, y); }
  t = &T_DRIFT; *t = cv(16, 16); p(SNOW, 0, 0, 16, 16); // tall grass poking through the snow, capped white
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) { char ch = FERN[y][x]; if (ch != '.') p1(ch == 'd' ? 0x2f5a48 : ch == 'a' ? 0xffffff : 0x5a8a70, x, y); }
  for (int x = 0; x < 16; x++) if (FERN[0][x] == 'd' || FERN[1][x] == 'a') p1(0xffffff, x, 0);
  t = &T_PATH_SNOW; *t = cv(16, 16); p(0xdce4f0, 0, 0, 16, 16); for (int i = 0; i < 10; i++) p1(i % 3 ? 0xc4cee0 : 0xe8eef8, hash(i, 9) % 16, hash(9, i) % 16); // trodden snow
  // the ICE CAVE: glassy ice with light streaks, frosted floor, blue ice walls
  t = &T_ICE; *t = cv(16, 16); p(0x98cce8, 0, 0, 16, 16); p(0x88bce0, 0, 15, 16, 1); p(0x88bce0, 15, 0, 1, 16);
  for (int i = 0; i < 4; i++) { p(0xd8f0ff, 2 + i, 9 - i, 1, 1); p(0xd8f0ff, 9 + i, 5 - i, 2, 1); } p(0xffffff, 3, 8, 1, 1); p1(0x78acd0, 12, 11); p1(0x78acd0, 13, 12);
  t = &T_FROSTFLOOR; *t = cv(16, 16); p(0xc8d8e8, 0, 0, 16, 16); for (int i = 0; i < 9; i++) p(i % 2 ? 0xb0c4d8 : 0xe4eef8, hash(i, 7) % 15, hash(7, i) % 15, 2, 1);
  t = &T_ICEWALL; *t = cv(16, 16); p(0x2a4468, 0, 0, 16, 16);
  { static const int B[3][4] = { { 1, 1, 14, 7 }, { 0, 8, 8, 8 }, { 8, 9, 8, 7 } };
    for (int i = 0; i < 3; i++) { int x = B[i][0], y = B[i][1], w = B[i][2], h = B[i][3]; p(0x5a80b0, x, y, w - 1, h - 1); p(0x8ab4dc, x + 1, y, w - 3, 2); p(0xc8e4f8, x + 1, y, w / 2, 1); p(0x3e5e88, x, y + h - 3, w - 1, 2); } }
  t = &T_S_SNOW; *t = cv(16, 16); p(SNOW, 0, 0, 16, 16); p(SNOW_SH, 4, 14, 9, 2); p(0x604020, 7, 9, 2, 6); p(0x503018, 2, 2, 12, 8); p(0xc89858, 3, 3, 10, 6); p(0xa87840, 4, 5, 8, 1); p(0xa87840, 4, 7, 6, 1); p(0xffffff, 2, 1, 12, 2);
  t = &TREE_SNOW; *t = TREE; t->p = malloc((size_t)TREE.w * TREE.h * 4); memcpy(t->p, TREE.p, (size_t)TREE.w * TREE.h * 4);
  for (int y = 0; y < TREE.h; y++) for (int x = 0; x < TREE.w; x++) { // snow on the lit tops of each tier, shadow blue under the trunk
    unsigned char *q = t->p + ((size_t)y * t->w + x) * 4; if (!q[3]) continue; unsigned c = q[0] << 16 | q[1] << 8 | q[2], n = c;
    if (c == LEAF_HI) n = 0xffffff; else if (c == LEAF) n = SNOW; else if (c == SHADOW) n = SNOW_SH; else if (c == LEAF_MID) n = 0x2f6a3a;
    q[0] = n >> 16; q[1] = (n >> 8) & 255; q[2] = n & 255;
  }
  t = &T_M; *t = cv(16, 16); p(0xf0e0b8, 0, 0, 16, 16); p(0xc04838, 1, 4, 14, 10); for (int x = 3; x < 14; x += 3) p(0xe07058, x, 5, 1, 8);
#include "parts/desert.inc"
}
// game.js TILES[key]: '~' and 'f' have two frames, 'X' three layouts picked per tile
static const MapInfo *M; // the map being painted
#include "parts/paint_2.inc"
#include "parts/paint_3.inc"
#include "parts/paint_4.inc"
