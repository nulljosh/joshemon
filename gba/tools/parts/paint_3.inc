static Cv bake_portrait(const Look *L, int back) {
  if (back) return bake_grid(HERO_BACK, 32, 32);
  Cv c = cv(32, 32); t = &c;
  unsigned skin = 0xf8c8a0, skinS = 0xe0a880, hair = L->hair, hairS = shade(L->hair, 0.7), shirt = L->coat ? 0xf0f0e8 : L->shirt, shirtS = L->coat ? 0xc8c8c0 : shade(L->shirt, 0.72);
  if (L->long_hair) { p(hair, 7, 8, 18, 18); p(hairS, 7, 20, 18, 6); }
  ell(shirt, 16, 31, 12, 10, 21, 31); p(shirtS, 22, 22, 5, 10); ell(skin, 16, 20, 2.5, 2, 18, 21);
  if (L->coat) { p(L->shirt == 0xf0f0e8 ? 0x88a0c8 : L->shirt, 13, 22, 6, 10); p(0x3858a8, 15, 22, 2, 7); }
  else { p(shirtS, 13, 21, 6, 1); p(shade(L->shirt, 0.85), 15, 22, 2, 3); }
  p(shirtS, 4, 24, 3, 8); p(shirt, 25, 24, 3, 8);
  ell(skin, 16, 11, 7.5, 8.5, -99, 99); p(skinS, 10, 17, 12, 1); ell(skinS, 16, 18, 5, 1, -99, 99);
  if (L->cap) {
    unsigned cs = shade(L->cap, 0.72);
    ell(hair, 16, 9, 8.5, 5, 6, 12); p(hair, 8, 9, 2, 6); p(hair, 22, 9, 2, 6);
    ell(L->cap, 16, 6, 8.5, 6, 0, 6); p(cs, 8, 6, 17, 2); p(shade(L->cap, 0.55), 7, 8, 18, 1);
    p(0xf8f8f8, 14, 2, 4, 3);
  } else {
    ell(hair, 16, 7, 8.5, 6.5, 0, 9); p(hair, 8, 7, 2, 8); p(hair, 22, 7, 2, 8);
    for (int x = 9; x < 23; x++) p(hair, x, 8, 1, 1 + ((x * 7) % 5 == 0 ? 3 : (x * 3) % 4 == 0 ? 2 : 1));
    p(hairS, 9, 6, 14, 1);
  }
  if (back) {
    for (int y = 3; y < 19; y++) { double v = 1 - sq((y - 11) / 8.5); int w = (int)lround(7.5 * sqrt(v > 0 ? v : 0)); p(y > 12 ? shade(L->hair, 0.85) : L->hair, 16 - w, y, w * 2, 1); }
    if (L->cap) { p(L->cap, 8, 1, 17, 6); p(shade(L->cap, 0.72), 8, 6, 17, 2); }
  } else {
    for (int ex = 11; ex <= 19; ex += 8) { p(hairS, ex, 10, 3, 1); p(0x302830, ex, 12, 3, 1); p(0x3c5aa0, ex, 13, 2, 2); p(0x302830, ex + 2, 13, 1, 2); p(0xf8f8f8, ex + 1, 13, 1, 1); }
    p(0xc06858, 15, 17, 2, 1); p(skinS, 16, 15, 1, 1);
    for (int gb = 0; gb < 17; gb++) if (L->gear >> gb & 1) switch (gb) {
      case 0: ell(shade(L->hair, 0.9), 16, 18, 6, 3, 16, 21); p(0xc06858, 15, 17, 2, 1); break; // beard
      case 1: for (int x = 9; x <= 21; x += 12) { p(0x5a3a1a, x, 22, 2, 10); p(0x7a5a30, x, 22, 1, 10); } break; // pack
      case 2: for (int ex = 10; ex <= 18; ex += 8) { p(0x202028, ex, 11, 5, 1); p(0x202028, ex, 15, 5, 1); p(0x202028, ex, 11, 1, 5); p(0x202028, ex + 4, 11, 1, 5); } p(0x202028, 15, 12, 3, 1); break; // glasses
      case 3: p(0x204070, 8, 6, 17, 2); for (int ex = 10; ex <= 18; ex += 8) { p(0x204070, ex, 4, 5, 4); p(0x80c8f0, ex + 1, 5, 3, 2); } break; // goggles
      case 4: p(0xe03030, 8, 7, 17, 2); p(0xe03030, 24, 8, 3, 1); p(0xe03030, 25, 9, 2, 2); break; // headband
      case 5: p(0xe85848, 23, 0, 2, 6); p(0xf8a088, 24, 1, 1, 3); break; // feather
      case 6: for (int y = 25; y < 32; y += 3) p(0xf8f8f8, 5, y, 22, 1); break; // stripes
      case 7: p(0xa07040, 27, 8, 1, 24); p(0xf8f8f8, 25, 2, 5, 1); p(0xf8f8f8, 25, 7, 5, 1); p(0xf8f8f8, 24, 3, 1, 4); p(0xf8f8f8, 30, 3, 1, 4); p(0xd8d8d8, 26, 4, 3, 2); break; // net
      case 8: p(0xd8d8d8, 12, 21, 2, 6); p(0xd8d8d8, 18, 21, 2, 6); p(0x202020, 5, 29, 22, 2); break; // gi
      case 9: p(0xf8c030, 8, 2, 17, 6); p(0xf8e070, 10, 2, 8, 2); p(0xc89020, 6, 7, 21, 2); break; // hardhat
      case 10: p(L->cap, 4, 7, 25, 2); p(shade(L->cap, 0.6), 4, 9, 25, 1); break; // brim
      case 11: p(0x283878, 2, 22, 4, 10); p(0x283878, 26, 22, 4, 10); p(0xe0c040, 12, 22, 8, 1); break; // cape
      case 12: p(0xf05020, 14, 25, 4, 4); p(0xf8b040, 15, 24, 2, 3); p(0xa02810, 15, 28, 2, 1); break; // ash
      case 13: p(0xa02818, 2, 22, 4, 10); p(0xa02818, 26, 22, 4, 10); p(0xe05030, 3, 22, 2, 2); p(0xe05030, 27, 22, 2, 2); break; // mantle
      case 14: p(0xf8e040, 14, 23, 3, 2); p(0xf8e040, 15, 25, 3, 2); p(0xf8e040, 14, 27, 3, 2); break; // bolt
      case 15: p(0xf05878, 20, 3, 6, 3); p(0xf05878, 22, 6, 2, 1); p(0xc03050, 22, 4, 2, 1); break; // bow
      case 16: p(0xf8f8f8, 7, 9, 3, 2); p(0xf8f8f8, 23, 9, 3, 2); p(0xa8d0f0, 8, 9, 1, 1); p(0xa8d0f0, 24, 9, 1, 1); break; // clips
    }
  }
  return c; // outlined at 64x64 by refine64
}
// a portrait is laid out at 32x32, doubled, then finished at the full 64x64: a 1 pixel outline, eyes with an iris and a shine, light on the hair, cheeks
static void refine64(Cv *c, const Look *L) {
  t = c;
  unsigned hs = 0; for (int k = 0; k < 24; k += 8) { unsigned ch = L->hair >> k & 255; hs |= (ch + (255 - ch) * 2 / 5) << k; } // the hair colour, lightened toward white
  if (!L->cap && !(L->gear & (1 << 9))) for (int x = 22; x < 36; x++) { int y = 4 + (x - 29) * (x - 29) / 14; p1(hs, x, y); p1(hs, x, y + 1); } // shine on the hair
  for (int ex = 22; ex <= 38; ex += 16) if (!(L->gear & (1 << 2))) { // eyes, unless glasses cover them
    p(0xf8f8f8, ex, 25, 6, 5); p(0x302830, ex, 24, 6, 1); p(0x302830, ex - 1, 25, 1, 1);
    p(0x2c4a90, ex + 2, 25, 3, 5); p(0x5a80d0, ex + 2, 28, 3, 2); p(0x181828, ex + 3, 26, 2, 2); p(0xf8f8f8, ex + 3, 25, 1, 1);
  }
  p(0xf8a8a0, 19, 31, 4, 2); p(0xf8a8a0, 41, 31, 4, 2); // cheeks
  Cv snap = cv(64, 64); memcpy(snap.p, c->p, 64 * 64 * 4);
  for (int y = 0; y < 64; y++) for (int x = 0; x < 64; x++) if (!opaque(&snap, x, y) && (opaque(&snap, x - 1, y) || opaque(&snap, x + 1, y) || opaque(&snap, x, y - 1) || opaque(&snap, x, y + 1))) p1(0x282430, x, y);
  free(snap.p);
}
// the player's back in battle, drawn at the full 64x64 (not doubled) so it has the detail of the Sapphire backs
static Cv bake_back64(void) {
  Cv c = cv(64, 64); t = &c;
  const unsigned R = 0xd83030, r = 0x982028, W = 0xf8f8f8, H = 0x503020, h = 0x382018, S = 0xf8c8a0, s = 0xd8a078, B = 0x3060b8, b = 0x204890, L = 0x5888d8, G = 0x58a048, g = 0x387030, K = 0x282430;
  ell(B, 32, 66, 30, 28, 38, 63);                                   // shoulders
  for (int y = 38; y < 64; y++) for (int x = 0; x < 64; x++) if (opaque(&c, x, y) && x > 44 - (y - 38) / 3) p1(b, x, y); // shade on the right
  for (int y = 40; y < 64; y++) { p1(L, 9 + (y - 40) / 4, y); p1(L, 10 + (y - 40) / 4, y); } // light on the left shoulder
  p(S, 27, 33, 11, 8); p(s, 33, 33, 5, 8);                           // neck
  ell(B, 32, 41, 8, 2, 39, 43); p(b, 27, 41, 11, 1);                 // collar
  for (int y = 40; y < 64; y++) { int dx = (y - 40) / 3; p(g, 18 + dx, y, 4, 1); p(G, 19 + dx, y, 2, 1); p(g, 42 - dx, y, 4, 1); p(G, 43 - dx, y, 2, 1); } // straps
  p(G, 21, 47, 22, 17); p(g, 37, 47, 6, 17); p(g, 21, 54, 22, 1); p(G, 22, 46, 20, 1); // pack, flap
  p(W, 30, 56, 4, 3); p(0xa8a8a8, 30, 58, 4, 1);                     // buckle
  ell(H, 32, 24, 15, 13, 18, 37);                                    // hair
  for (int x = 19; x < 46; x++) { int len = 2 + (x * 7) % 4; p(h, x, 34 - len + (x % 3 == 0), 1, 1 + (x % 2)); } // ends of the hair
  for (int y = 18; y < 33; y++) for (int x = 38; x < 48; x++) if (opaque(&c, x, y) && x > 41 - (y - 18) / 6) p1(h, x, y); // shade on the right
  ell(S, 16.5, 27, 2.5, 4, 0, 99); ell(S, 47.5, 27, 2.5, 4, 0, 99); p(s, 16, 26, 1, 3); p(s, 47, 26, 1, 3); // ears
  ell(R, 32, 19, 16, 14, 4, 21);                                     // cap
  for (int y = 4; y < 22; y++) for (int x = 36; x < 49; x++) if (opaque(&c, x, y) && x > 40 + (y < 12 ? 12 - y : 0) / 2) p1(r, x, y); // shade
  p(r, 16, 19, 32, 3); p(0x7a1820, 16, 21, 32, 1);                  // band
  p(W, 28, 15, 8, 5); p(0xc8c8c8, 28, 19, 8, 1); p(r, 29, 16, 6, 1);// the strap gap at the back
  p(0xf07070, 22, 8, 5, 2); p(0xf07070, 21, 10, 2, 2);               // shine
  Cv snap = cv(64, 64); memcpy(snap.p, c.p, 64 * 64 * 4);
  for (int y = 0; y < 64; y++) for (int x = 0; x < 64; x++) if (!opaque(&snap, x, y) && (opaque(&snap, x - 1, y) || opaque(&snap, x + 1, y) || opaque(&snap, x, y - 1) || opaque(&snap, x, y + 1))) p1(K, x, y);
  free(snap.p);
  return c;
}
// drawThing: kinds after the people, in LOOK_* order: ball, pc, tablet, grass, obsK, obsY, obsO, surf, bike
static void surf_mount(int x, int y, int d) {
  int side = d == D_LEFT || d == D_RIGHT, w = side ? 18 : 14, x0 = x + 8 - w / 2;
  fill_a(t, 0xffffff, 0x90, x0 - 1, y + 17, w + 2, 1);
  p(0x203850, x0, y + 10, w, 7); p(0x4888c8, x0 + 1, y + 10, w - 2, 6); p(0x78b8e8, x0 + 2, y + 11, w - 4, 2); p(0x305888, x0 + 1, y + 15, w - 2, 1);
#define HEAD(hx, hy) do { p(0x203850, hx, hy, 7, 6); p(0x4888c8, (hx) + 1, (hy) + 1, 5, 4); p(0x78b8e8, (hx) + 1, (hy) + 1, 3, 1); } while (0)
  if (side) { int fx = d == D_LEFT ? x0 + w - 3 : x0 + 1, hx = d == D_LEFT ? x0 - 4 : x0 + w - 3; p(0x203850, fx, y + 6, 3, 4); p(0x4888c8, fx + 1, y + 7, 1, 3); HEAD(hx, y + 7); p(0x101820, d == D_LEFT ? hx + 1 : hx + 5, y + 9, 1, 1); }
  if (d == D_DOWN) { HEAD(x0 + w / 2 - 3, y + 15); p(0x101820, x0 + w / 2 - 2, y + 17, 1, 1); p(0x101820, x0 + w / 2 + 2, y + 17, 1, 1); }
}
static void bike(int x, int y, int d, int spin) {
  if (d == D_LEFT || d == D_RIGHT) {
    for (int wx = x + 1; wx <= x + 10; wx += 9) { p(0x202028, wx, y + 13, 6, 6); p(0x808890, wx + 1, y + 14, 4, 4); p(0x202028, wx + 2, y + 15, 2, 2); p(0xc8d0d8, wx + 1 + spin * 3, y + 14, 1, 1); }
    p(0xc83030, x + 4, y + 14, 9, 2); p(0xc83030, x + (d == D_LEFT ? 3 : 11), y + 11, 2, 4); p(0x303038, x + (d == D_LEFT ? 2 : 11), y + 10, 3, 1);
  } else {
    int fy = d == D_DOWN ? y + 15 : y + 13;
    p(0x202028, x + 6, fy, 4, 6); p(0x808890, x + 7, fy + 1 + spin, 2, 3); p(0xc83030, x + 7, fy - 2, 2, 3); int by = d == D_DOWN ? y + 12 : y + 10; p(0x303038, x + 1, by, 14, 1); p(0xc83030, x, by - 1, 2, 3); p(0xc83030, x + 14, by - 1, 2, 3);
  }
}
static void ball(int x, int y) {
  static const char *R0[8] = { "..####..", ".#rrrr#.", "#rrrrrr#", "#rr##rr#", "####w###", "#ww##ww#", ".#wwww#.", "..####.." };
  for (int j = 0; j < 8; j++) for (int i = 0; i < 8; i++) { char c = R0[j][i]; if (c != '.') p1(c == '#' ? 0x303030 : c == 'r' ? 0xe04038 : 0xf8f8f8, x + i, y + j); }
}
static void draw_thing(int kind, int x, int y, int d, int step, int frame) {
  switch (kind) {
    case 0: ball(x + 4, y + 5); break;
    case 1: p(0x484858, x + 2, y + 2, 12, 11); p((frame >> 4) & 1 ? 0x78c8f0 : 0x58a8e0, x + 3, y + 3, 10, 7); p(0x686878, x + 5, y + 13, 6, 2); break;
    case 2: p(0x3a3038, x + 2, y - 4, 12, 19); p(0x8a8890, x + 3, y - 3, 10, 17); p(0xa8a8b0, x + 3, y - 3, 10, 2); for (int r = 0; r < 5; r++) p(0x5a5860, x + 5, y + 1 + r * 3, 6 - (r % 2) * 2, 1); break;
    case 3: draw(t, &T_FERN, 0, 10, 16, 6, x, y + 10); break;
    case 4: draw(t, &T_CUT, 0, 0, 16, 16, x, y); break;
    case 5: draw(t, &T_Y, 0, 0, 16, 16, x, y); break;
    case 6: draw(t, &T_O, 0, 0, 16, 16, x, y); break;
    case 7: surf_mount(x, y, d); break;
    case 8: bike(x, y, d, step ? 1 : 0); break; // P.walk = 2 gives spin (2 >> 1) & 1 = 1
  }
}

// ---------- the cutter and packers (gba/tools/maps.mjs)
static int c15(const unsigned char *d) { return d[3] < 128 ? -1 : (d[0] >> 3) | ((d[1] >> 3) << 5) | ((d[2] >> 3) << 10); }
static int pal[256], npal = 1;
static int colour(int c) { if (c < 0) return 0; for (int i = 1; i < npal; i++) if (pal[i] == c) return i; if (npal >= 256) { fprintf(stderr, "map palette over 256\n"); exit(1); } pal[npal] = c; return npal++; }
static unsigned char (*bank)[64]; static int nbank, capbank;
static int add_tile(const unsigned char *b) {
  for (int i = 0; i < nbank; i++) if (!memcmp(bank[i], b, 64)) return i;
  if (nbank == capbank) { capbank = capbank ? capbank * 2 : 1024; bank = realloc(bank, (size_t)capbank * 64); }
  memcpy(bank[nbank], b, 64); return nbank++;
}
static int cut(const Cv *c, int tx, int ty) { unsigned char b[64]; for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) b[y * 8 + x] = colour(c15(c->p + ((size_t)(ty * 8 + y) * c->w + tx * 8 + x) * 4)); return add_tile(b); }
// 4bpp OBJ frames in 1D tile order with one 16-colour palette per set; colours past 15 merge into their nearest
static int near_(int a, int b) { int d = 0; for (int k = 0; k < 15; k += 5) { int x = ((a >> k) & 31) - ((b >> k) & 31); d += x * x; } return d; }
static void pack(Cv *frames, int nf, int fw, int fh, FILE *words, int *nwords, int *pal16) {
  int cols[1024], cnt[1024], nc = 0, remap[1024];
  for (int f = 0; f < nf; f++) for (int i = 0; i < fw * fh; i++) { int c = c15(frames[f].p + i * 4); if (c < 0) continue; int k = 0; while (k < nc && cols[k] != c) k++; if (k == nc) { cols[nc] = c; cnt[nc++] = 0; } cnt[k]++; }
  int pl[1024], np = nc; for (int i = 0; i < nc; i++) { pl[i] = cols[i]; remap[i] = cols[i]; }
  while (np > 15) {
    int bi = 0, bj = 1, bd = 1 << 30;
    for (int i = 0; i < np; i++) for (int j = i + 1; j < np; j++) { int d = near_(pl[i], pl[j]); if (d < bd) { bd = d; bi = i; bj = j; } }
    int ci = 0, cj = 0; for (int k = 0; k < nc; k++) { if (cols[k] == pl[bi]) ci = cnt[k]; if (cols[k] == pl[bj]) cj = cnt[k]; }
    int keep = ci >= cj ? pl[bi] : pl[bj], drop = ci >= cj ? pl[bj] : pl[bi];
    for (int k = 0; k < nc; k++) if (remap[k] == drop) remap[k] = keep;
    for (int k = 0; k < nc; k++) if (cols[k] == keep) for (int q = 0; q < nc; q++) if (cols[q] == drop) cnt[k] += cnt[q];
    int w = 0; for (int i = 0; i < np; i++) if (pl[i] != drop) pl[w++] = pl[i]; np = w;
  }
  pal16[0] = 0; for (int i = 0; i < 15; i++) pal16[1 + i] = i < np ? pl[i] : 0;
  for (int f = 0; f < nf; f++) for (int ty = 0; ty < fh / 8; ty++) for (int tx = 0; tx < fw / 8; tx++) for (int y = 0; y < 8; y++) {
    unsigned w = 0;
    for (int x = 0; x < 8; x++) {
      int c = c15(frames[f].p + ((size_t)(ty * 8 + y) * fw + tx * 8 + x) * 4), idx = 0;
      if (c >= 0) { int k = 0; while (cols[k] != c) k++; int m = remap[k]; for (int i = 0; i < np; i++) if (pl[i] == m) idx = i + 1; }
      w |= (unsigned)idx << (x * 4);
    }
    fprintf(words, "%s0x%x", (*nwords)++ ? "," : "", w);
  }
}
