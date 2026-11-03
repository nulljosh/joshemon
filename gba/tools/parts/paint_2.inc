static const Cv *tile_img(char ch, int tx, int ty, int frame) {
  switch (ch) {
    case '.': return &T_DOT; case 'f': return &T_F[((frame >> 5) + tx) & 1]; case ',': return &T_FERN; case 'p': return M && M->border == 'F' ? &T_PATH_SNOW : M && M->border == 'a' ? &T_PATH_SAND : &T_PATH; case 'T': return &T_TREE_GROUND;
    case '~': return &T_W[(frame >> 5) & 1]; case 'R': return &T_R; case 'H': return &T_H; case 'W': return &T_WIN; case 'D': return &T_D; case 'B': return &T_B;
    case 'S': return M && M->border == 'F' ? &T_S_SNOW : M && M->border == 'a' ? &T_S_SAND : &T_S; case 'G': return &T_G; case 'k': return &T_K_; case 'X': return &T_X[hash(tx & 255, ty & 255) % 3]; case 'q': return &T_Q; case 'z': return &T_Z;
    case 'j': return &T_J; case 'K': return &T_CUT; case 'Y': return &T_Y; case 'O': return &T_O; case 'E': return &T_E; case 'n': return &T_N; case 'L': return &T_L;
    case 'w': return &T_WALL; case 'v': return &T_V; case 'r': return &T_RR; case 'o': return &T_FLOOR; case 'u': return &T_U; case 'P': return &T_P; case 'y': return &T_YY;
    case 'c': return &T_C; case 'h': return &T_SH; case 'm': return &T_M; case 'b': return M && (M->flags & MF_SUN) ? &T_BRICK_SUN : &T_BRICK; case 'g': return M && (M->flags & MF_SUN) ? &T_SLATE_SUN : &T_SLATE; case 'U': return &T_STAIRS;
    case 'a': return &T_SAND; case 'd': return &T_DUNE; case 'C': return &T_CACTUS;
    case 's': case 'F': return &T_SNOW; case 'A': return &T_DRIFT; case 'I': return &T_ICE; case 'Q': return &T_ICEWALL; case 'e': return &T_FROSTFLOOR;
  }
  fprintf(stderr, "no tile art for '%c'\n", ch); exit(1);
}

// ---------- one map at one animation frame (game.js exportMap, drawTiles, edges)
static char ROWS[40 * 40];
static char tile_at(int x, int y) { return x < 0 || y < 0 || x >= M->w || y >= M->h ? 'T' : ROWS[y * M->w + x]; }
static char tile_pad(int tx, int ty) { // the padded border: paths keep going, the rest is the border (trees, rock or sea); rooms are black
  if (tx >= 0 && ty >= 0 && tx < M->w && ty < M->h) return ROWS[ty * M->w + tx];
  if (!(M->flags & MF_OUTDOOR)) return 0;
  int cx = tx < 0 ? 0 : tx >= M->w ? M->w - 1 : tx, cy = ty < 0 ? 0 : ty >= M->h ? M->h - 1 : ty;
  if (ROWS[cy * M->w + cx] == 'p') return 'p';
  if (ROWS[cy * M->w + cx] == '~') return '~'; // sea at the edge runs on past it (ROUTE 8 into LUMEN)
  return M->border ? M->border : (M->flags & MF_CAVE) ? 'X' : 'T';
}
static Cv *g; static void px(unsigned c, int x, int y, int w, int h) { fill(g, c, x, y, w, h); }
static void pxa(unsigned c, unsigned a, int x, int y, int w, int h) { fill_a(g, c, a, x, y, w, h); }
#define BLD "RBGHWD"
// Neighbours past the map edge are its border (sea, trees, rock), so the sea outside a sea route has no shoreline
// grid and a path that leaves the map keeps going. (The JS painter looked at a tree there.)
static void edges(char ch, int water_frame, int tx, int ty, int x, int y) {
  (void)water_frame;
  if (ch == 'p') {
    for (int i = 0; i < 16; i++) {
      int a = (i + tx * 3 + ty) % 3 == 0, b2 = (i + tx + ty * 2) % 4 == 1;
#define FRAY(dx, dy) ({ char _t = tile_pad(tx + (dx), ty + (dy)); _t != 'p' && !has(BLD, _t) && _t != 'm'; })
#define FC(dx, dy) (has("sAF", tile_pad(tx + (dx), ty + (dy))) ? SNOW : GRASS) // fray into snow up north
      if (FRAY(0, -1)) { if (a || i % 2) px(FC(0, -1), x + i, y, 1, 1); if (b2) px(FC(0, -1), x + i, y + 1, 1, 1); }
      if (FRAY(0, 1)) { if (a || i % 2) px(FC(0, 1), x + i, y + 15, 1, 1); if (b2) px(FC(0, 1), x + i, y + 14, 1, 1); }
      if (FRAY(-1, 0)) { if (a || i % 2) px(FC(-1, 0), x, y + i, 1, 1); if (b2) px(FC(-1, 0), x + 1, y + i, 1, 1); }
      if (FRAY(1, 0)) { if (a || i % 2) px(FC(1, 0), x + 15, y + i, 1, 1); if (b2) px(FC(1, 0), x + 14, y + i, 1, 1); }
    }
  }
  if (has(BLD, ch)) {
    int roof = ch == 'R' || ch == 'B' || ch == 'G'; unsigned out = 0x4a3c34, hi = ch == 'R' ? 0xf09878 : ch == 'B' ? 0x98c0f8 : 0xc8a0e8, eave = ch == 'R' ? 0x8a3424 : ch == 'B' ? 0x284a88 : 0x503070;
    if (!has(BLD, tile_pad(tx - 1, ty))) px(out, x, y, 1, 16);
    if (!has(BLD, tile_pad(tx + 1, ty))) px(out, x + 15, y, 1, 16);
    if (roof && !has(BLD, tile_pad(tx, ty - 1))) { px(out, x, y, 16, 1); px(hi, x + 1, y + 1, 14, 1); }
    if (roof && tile_pad(tx, ty + 1) != ch) { px(eave, x, y + 12, 16, 3); px(out, x, y + 15, 16, 1); }
  }
  if (has("wvr", ch) && has("wvr", tile_pad(tx, ty + 1))) { px(0xf0e4c4, x, y + 9, 16, 7); if (ch == 'w') px(0xe8dab8, x, y + 12, 16, 1); }
  if (ch == '~') {
    unsigned lip = (M->flags & MF_CAVE) ? 0x4a3626 : (M->flags & MF_OUTDOOR) ? (M->border == 'F' ? 0x6878a0 : LEAF_DK) : 0xa88858, dark = 0x3c78c0;
    if (tile_pad(tx, ty - 1) != '~') { px(lip, x, y, 16, 1); px(dark, x, y + 1, 16, 3); }
    if (tile_pad(tx - 1, ty) != '~') { px(lip, x, y, 1, 16); px(dark, x + 1, y, 2, 16); }
    if (tile_pad(tx + 1, ty) != '~') { px(lip, x + 15, y, 1, 16); px(dark, x + 13, y, 2, 16); }
    if (tile_pad(tx, ty + 1) != '~') { px(WATER_HI, x, y + 13, 16, 1); px(0xf8f8f8, x + ((tx * 5) % 7), y + 13, 3, 1); px(lip, x, y + 15, 16, 1); }
  }
}
static void draw_tiles(int tx0, int ty0, int tx1, int ty1, int cx, int cy, int frame) {
  for (int ty = ty0; ty <= ty1; ty++) for (int tx = tx0; tx <= tx1; tx++) {
    char ch = tile_pad(tx, ty); if (!ch) continue;
    int x = tx * 16 - cx, y = ty * 16 - cy;
    draw(g, tile_img(ch, tx, ty, frame), 0, 0, 16, 16, x, y); edges(ch, frame, tx, ty, x, y);
    if (has("RBG", ch)) { // roofs: pale ridge on top, dark trim at the gables, deeper shade on the overhang
#define ROOF(c) ({ char _c = (c); _c && has("RBG", _c); })
      if (!ROOF(tile_pad(tx, ty - 1))) { pxa(0xffffff, 0x50, x, y, 16, 2); pxa(0, 0x30, x, y + 2, 16, 1); }
      if (!ROOF(tile_pad(tx - 1, ty))) { pxa(0, 0x48, x, y, 2, 16); pxa(0xffffff, 0x30, x + 2, y, 1, 16); }
      if (!ROOF(tile_pad(tx + 1, ty))) pxa(0, 0x48, x + 14, y, 2, 16);
      if (!ROOF(tile_pad(tx, ty + 1))) { pxa(0, 0x40, x, y + 13, 16, 3); pxa(0xffffff, 0x28, x, y + 12, 16, 1); }
    }
#define WALLC(c) ({ char _c = (c); _c && has("HWD", _c); })
    if (WALLC(ch) && !WALLC(tile_pad(tx + 1, ty))) pxa(0, 0x28, x + 13, y, 3, 16);
    if (has(".pfs", ch) && WALLC(tile_pad(tx, ty - 1))) pxa(0, 0x30, x, y, 16, 2);
    if (has("HWD", ch) && has("RBG", tile_pad(tx, ty - 1))) { pxa(0, 0x38, x, y, 16, 3); pxa(0, 0x20, x, y + 3, 16, 1); }
  }
}
// the whole map with its border, ground layer and the canopy that covers people (only tree tops above their own tile)
static void export_map(int id, int frame, int padx, int pady, Cv *ground, Cv *over) {
  M = &MAP_INFO[id];
  for (int i = 0; i < M->w * M->h; i++) { char c = M->rows[i]; ROWS[i] = has("KYO", c) ? ((M->flags & MF_CAVE) ? 'q' : !(M->flags & MF_OUTDOOR) ? 'o' : M->border == 'F' ? 's' : M->border == 'a' ? 'a' : '.') : c; } // obstacles are sprites on the cartridge
  int w = (M->w + padx * 2) * 16, h = (M->h + pady * 2) * 16, cx = -padx * 16, cy = -pady * 16;
  *ground = cv(w, h); *over = cv(w, h);
  g = ground; px(0, 0, 0, w, h);
  draw_tiles(-padx, -pady, M->w + padx - 1, M->h + pady, cx, cy, frame);
  for (int ty = -pady; ty <= M->h + pady; ty++) for (int tx = -padx - 1; tx <= M->w + padx; tx++) if (has("TF", tile_pad(tx, ty))) {
    int x = tx * 16 - cx - 3, y = ty * 16 - cy - 12; const Cv *tr = tile_pad(tx, ty) == 'F' ? &TREE_SNOW : &TREE;
    draw(ground, tr, 0, 0, 22, 28, x, y);
    clip_y1 = ty * 16 - cy; draw(over, tr, 0, 0, 22, 28, x, y); clip_y1 = 1 << 30;
  }
}

// ---------- people (game.js bakePerson, bakePortrait, GEAR) and things (drawThing)
enum { D_DOWN, D_UP, D_LEFT, D_RIGHT };
// the player, drawn by hand: down, up, left; standing and mid-step. k outline, R/r cap, W white, H/h hair, S/s skin, E eyes, B/b/L shirt, G/g pack, P pants, O shoes
static const char *const HERO[6][20] = {
  {".....kkkkkk.....", "...kkRRRRRRkk...", "..kRRRRWWRRRRk..", "..kRRRRWWRRRRk..", ".krrrrrrrrrrrrk.", ".kkHSSSSSSSSHkk.", "..kHSESSSSESHk..", "..kHSESSSSESHk..", "..kkSSSSSSSSkk..", "...kksSSSSskk...", "..kBBkksskkBBk..", ".kBBBBLBBLBBBBk.", ".kBkBBBBBBBBkBk.", ".kSkBBBBBBBBkSk.", ".kskbbbbbbbbksk.", "..kkPPPPPPPPkk..", "...kPPPkkPPPk...", "...kPPPkkPPPk...", "...kOOOkkOOOk...", "....kkk..kkk...."},
  {".....kkkkkk.....", "...kkRRRRRRkk...", "..kRRRRWWRRRRk..", "..kRRRRWWRRRRk..", ".krrrrrrrrrrrrk.", ".kkHSSSSSSSSHkk.", "..kHSESSSSESHk..", "..kHSESSSSESHk..", "..kkSSSSSSSSkk..", "...kksSSSSskk...", "..kBBkksskkBBk..", ".kBBBBLBBLBBBBk.", ".kBkBBBBBBBBkBk.", ".kSkBBBBBBBBkSk.", ".kskbbbbbbbbksk.", "..kkPPPPPPPPkk..", "...kPPPkkPPPk...", "...kOOOkkPPPk...", "....kkk.kOOOk...", ".........kkk...."},
  {".....kkkkkk.....", "...kkRRRRRRkk...", "..kRRRRRRRRRRk..", "..kRRRRRRRRRRk..", ".krrrrrrrrrrrrk.", ".kHHHHHHHHHHHHk.", "..kHHHHHHHHHHk..", "..khHHHHHHHHhk..", "..kkhhhhhhhhkk..", "...kksSSSSskk...", "..kBkGGGGGGkBk..", ".kBBkGGGGGGkBBk.", ".kBkGGgggGGGkBk.", ".kSkGGGGGGGGkSk.", ".kskkggggggkksk.", "..kkPPPPPPPPkk..", "...kPPPkkPPPk...", "...kPPPkkPPPk...", "...kOOOkkOOOk...", "....kkk..kkk...."},
  {".....kkkkkk.....", "...kkRRRRRRkk...", "..kRRRRRRRRRRk..", "..kRRRRRRRRRRk..", ".krrrrrrrrrrrrk.", ".kHHHHHHHHHHHHk.", "..kHHHHHHHHHHk..", "..khHHHHHHHHhk..", "..kkhhhhhhhhkk..", "...kksSSSSskk...", "..kBkGGGGGGkBk..", ".kBBkGGGGGGkBBk.", ".kBkGGgggGGGkBk.", ".kSkGGGGGGGGkSk.", ".kskkggggggkksk.", "..kkPPPPPPPPkk..", "...kPPPkkPPPk...", "...kOOOkkPPPk...", "....kkk.kOOOk...", ".........kkk...."},
  {"......kkkkkk....", "....kkRRRRRRkk..", "...kRRRRRRRRRRk.", "...kRRRWWRRRRRk.", ".kkrrrrrrrrrrrk.", "krrrrrkHHHHHHHk.", ".kkSSSSSHHHHHk..", "..kSESSSSHHHHk..", "..kSSSSSsHHHk...", "...kssSSSkkk....", "...kkBBBBBkk....", "...kBBBBBBBGk...", "...kBBSBBBkGk...", "...kBBSBBBkGk...", "...kbbsbbbkgk...", "....kPPPPPkk....", "....kPPPPPk.....", "....kPPkPPk.....", "....kOOkOOk.....", ".....kk.kk......"},
  {"......kkkkkk....", "....kkRRRRRRkk..", "...kRRRRRRRRRRk.", "...kRRRWWRRRRRk.", ".kkrrrrrrrrrrrk.", "krrrrrkHHHHHHHk.", ".kkSSSSSHHHHHk..", "..kSESSSSHHHHk..", "..kSSSSSsHHHk...", "...kssSSSkkk....", "...kkBBBBBkk....", "...kBBBBBBBGk...", "...kBBSBBBkGk...", "...kBBSBBBkGk...", "...kbbsbbbkgk...", "....kPPPPPkk....", "...kPPPkPPPk....", "..kPPPk.kPPPk...", "..kOOOk.kOOOk...", "...kkk...kkk...."},
};
static const char *const HERO_BACK[32] = {"................................", "...........kkkkkkkkkk...........", ".........kkRRRRRRRRRRkk.........", "........kRRRRRRRRRRRRRRk........", ".......kRRRRRRRRRRRRRRRRk.......", "......kRRRRRRRRRRRRRRRRRRk......", "......kRRRRRRRRRRRRRRRRRRk......", ".....krrrrrrrrrrrrrrrrrrrrk.....", ".....kWWWWWWWWWWWWWWWWWWWWk.....", ".....kHHHHHHHHHHHHHHHHHHHHk.....", ".....kHHHHHHHHHHHHHHHHHHHHk.....", ".....kHHhHHHHHHHHHHHHHHhHHk.....", "....kSkHHHHHHHHHHHHHHHHHHkSk....", "....kSkHhHHHHHHHHHHHHHHhHkSk....", ".....kkhHHHHHHHHHHHHHHHHhkk.....", "......khhHHHHHHHHHHHHHHhhk......", ".......kkhhhhhhhhhhhhhhkk.......", ".........kksssssssssskk.........", "......kkkkBBBBBBBBBBBBkkkk......", "....kkBBBBBBBBBBBBBBBBBBBBkk....", "...kBBBBBGkBBBBBBBBBBkGBBBBBk...", "..kBBBBBBGkBBBBBBBBBBkGBBBBBBk..", "..kBBBBBBGGkBBBBBBBBkGGBBBBBBk..", ".kBBBBBBBkGGGGGGGGGGGGkBBBBBBBk.", ".kBBBBBBBkGGGGGGGGGGGGkBBBBBBBk.", ".kBbBBBBBkGgggggggggGGkBBBBBbBk.", ".kBbBBBBBkGGGGWWGGGGGGkBBBBBbBk.", "kBBbBBBBBkGGGGWWGGGGGGkBBBBBbBBk", "kBBbBBBBBkGGGGGGGGGGGGkBBBBBbBBk", "kbbbbbbbbkggggggggggggkbbbbbbbbk", "kbbbbbbbbbkkkkkkkkkkkkbbbbbbbbbk", "kbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbk"};
static unsigned hero_col(char ch) {
  switch (ch) { case 'k': return 0x282430; case 'R': return 0xd83030; case 'r': return 0x982028; case 'W': return 0xf8f8f8; case 'H': return 0x503020; case 'h': return 0x382018;
    case 'S': return 0xf8c8a0; case 's': return 0xd8a078; case 'E': return 0x302830; case 'B': return 0x3060b8; case 'b': return 0x204890; case 'L': return 0x5888d8;
    case 'G': return 0x58a048; case 'g': return 0x387030; case 'P': return 0x404868; case 'O': return 0x484050; }
  fprintf(stderr, "hero: bad colour %c\n", ch); exit(1);
}
static Cv bake_grid(const char *const *rows, int w, int h) {
  Cv c = cv(w, h); t = &c;
  for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) if (rows[y][x] != '.') p1(hero_col(rows[y][x]), x, y);
  return c;
}
static const char *const BARE[3][10] = {{".....kkkkkk.....", "...kkHHHHHHkk...", "..kHHHHHHHHHHk..", "..kHHHHHHHHHHk..", ".kHHHHHHHHHHHHk.", ".kHHhSSSSSShHHk.", "..kHSESSSSESHk..", "..kHSESSSSESHk..", "..kkSSSSSSSSkk..", "...kksSSSSskk..."}, {".....kkkkkk.....", "...kkHHHHHHkk...", "..kHHHHHHHHHHk..", "..kHHHHHHHHHHk..", ".kHHHHHHHHHHHHk.", ".kHHHHHHHHHHHHk.", "..kHHHHHHHHHHk..", "..khHHHHHHHHhk..", "..kkhhhhhhhhkk..", "...kksSSSSskk..."}, {"......kkkkkk....", "....kkHHHHHHkk..", "...kHHHHHHHHHHk.", "...kHHHHHHHHHHk.", "..kHHHHHHHHHHHk.", "..kHHSSHHHHHHHk.", "..kSSSSSHHHHHk..", "..kSESSSSHHHHk..", "..kSSSSSsHHHk...", "...kssSSSkkk...."}}; // heads without a cap, for everyone else
// everyone else is drawn from the player's frames, in their own colours: a cap or bare head, long hair, a lab coat, no pack
static Cv bake_look(const Look *L, int d, int step) {
  Cv c = cv(16, 20); t = &c;
  const char *const *f = HERO[d * 2 + (step ? 1 : 0)];
  unsigned shirt = L->coat ? 0xe8e8e0 : L->shirt, shirtS = L->coat ? 0xc0c0b8 : shade(L->shirt, 0.72), legs = L->legs ? L->legs : 0x404868;
  for (int y = 0; y < 20; y++) for (int x = 0; x < 16; x++) {
    char ch = y < 10 && !L->cap ? BARE[d][y][x] : f[y][x]; unsigned col;
    switch (ch) {
      case '.': continue;
      case 'R': col = L->cap; break; case 'r': col = shade(L->cap, 0.7); break;
      case 'H': col = L->hair; break; case 'h': col = shade(L->hair, 0.7); break;
      case 'B': case 'G': col = shirt; break; case 'b': case 'g': col = shirtS; break;
      case 'L': col = L->coat ? L->shirt : shade(shirt, 1.25); break;
      case 'P': col = legs; break; case 'O': col = 0x383038; break;
      default: col = hero_col(ch);
    }
    p1(col, x, y);
  }
  if (L->long_hair) { unsigned hs = shade(L->hair, 0.7);
    if (d == D_DOWN) { p(L->hair, 1, 5, 2, 7); p(hs, 1, 11, 2, 1); p(L->hair, 13, 5, 2, 7); p(hs, 13, 11, 2, 1); }
    else if (d == D_UP) { p(L->hair, 3, 9, 10, 4); p(hs, 3, 12, 10, 1); }
    else { p(L->hair, 10, 8, 4, 5); p(hs, 10, 12, 4, 1); }
    Cv snap = cv(16, 20); memcpy(snap.p, c.p, 16 * 20 * 4);
    for (int y = 0; y < 20; y++) for (int x = 0; x < 16; x++) if (!opaque(&snap, x, y) && (opaque(&snap, x - 1, y) || opaque(&snap, x + 1, y) || opaque(&snap, x, y - 1) || opaque(&snap, x, y + 1))) p1(0x282430, x, y);
    free(snap.p); }
  return c;
}
static Cv bake_person(const Look *L, int d, int step) {
  return L == &LOOK_DATA[LOOK_PLAYER] ? bake_grid(HERO[d * 2 + (step ? 1 : 0)], 16, 20) : bake_look(L, d, step);
}
static void ell(unsigned col, double cx, double cy, double rx, double ry, int y0, int y1) {
  for (int y = (int)ceil(cy - ry); y <= cy + ry; y++) if (y >= y0 && y <= y1) {
    double v = 1 - sq((y - cy) / ry); int w = (int)lround(rx * sqrt(v > 0 ? v : 0));
    p(col, (int)floor(cx - w + 0.5), y, w * 2, 1);
  }
}
