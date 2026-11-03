// what a hit looks like where it lands, by type (all particles: c 0/1 picks the type's two colours, s the shape)
static int cosr(int a, int r) { return SIN[(a + 64) & 255] * r / 127; }
static int sinr(int a, int r) { return SIN[a & 255] * r / 127; }
static void target_fx(int type, int tx, int ty) {
  int f, i;
  switch (type) {
  case TYPE_FIRE: // flames licking up off the target
    for (f = 0; f < 26; f++) { for (i = 0; i < 3; i++) spark((tx - 14 + ri(28)) * 16, (ty + 12 - ri(6)) * 16, ri(7) - 3, -(14 + ri(14)), -1, 12 + ri(8), ri(3) ? 0 : 1, ri(2) ? 3 : 2); fx_step(); next_frame(); }
    fx_ticks(10); break;
  case TYPE_WATER: // a splash that falls back
    for (i = 0; i < 22; i++) spark((tx - 4 + ri(8)) * 16, (ty + 4) * 16, ri(48) - 24, -(22 + ri(22)), 3, 26, i % 3 ? 0 : 1, ri(2) ? 3 : 2);
    fx_ticks(26); break;
  case TYPE_ICE: // a ring of frost closing in, then shattering
    for (i = 0; i < 16; i++) { int a = i * 16; spark((tx + cosr(a, 30)) * 16, (ty + sinr(a, 20)) * 16, -cosr(a, 30) * 16 / 14, -sinr(a, 20) * 16 / 14, 0, 14, i % 2, 3); }
    fx_ticks(14);
    for (i = 0; i < 18; i++) { int a = i * 256 / 18, v = 26 + ri(16); spark(tx * 16, ty * 16, cosr(a, v), sinr(a, v), 1, 12, i % 2, 4); }
    fx_ticks(12); break;
  case TYPE_GRASS: case TYPE_BUG: // leaves whirling in (BUG: a buzzing swarm)
    for (f = 0; f < 26; f++) {
      nsparks = 0;
      for (i = 0; i < 10; i++) {
        int a = f * (type == TYPE_BUG ? 13 : 9) + i * 26, r = 30 - f;
        int jx = type == TYPE_BUG ? ri(5) - 2 : 0, jy = type == TYPE_BUG ? ri(5) - 2 : 0;
        spark((tx + cosr(a, r) + jx) * 16, (ty + sinr(a, r * 2 / 3) + jy) * 16, 0, 0, 0, 2, i % 2, type == TYPE_GRASS ? 3 : 2);
      }
      next_frame();
    }
    nsparks = 0; break;
  case TYPE_ROCK: // rocks dropping out of the sky
    for (i = 0; i < 8; i++) spark((tx - 18 + ri(36)) * 16, (ty - 44 - i * 9) * 16, 0, 18, 3, 24 + i, i % 2, 3);
    fx_ticks(32);
    for (i = 0; i < 10; i++) spark((tx - 10 + ri(20)) * 16, (ty + 12) * 16, ri(24) - 12, -(6 + ri(8)), 1, 12, 1, 2);
    fx_ticks(12); break;
  case TYPE_GROUND: // the ground bucks and dust boils up
    for (f = 0; f < 22; f++) { for (i = 0; i < 2; i++) spark((tx - 22 + ri(44)) * 16, (ty + 16) * 16, ri(9) - 4, -(4 + ri(8)), 0, 18, ri(2), ri(3) ? 3 : 2); fx_step(); next_frame(); }
    fx_ticks(12); break;
  case TYPE_POISON: // bubbles welling up and popping
    for (f = 0; f < 24; f++) { if (f % 2 == 0) spark((tx - 14 + ri(28)) * 16, (ty + 10) * 16, (f % 4 ? 3 : -3), -(8 + ri(6)), 0, 18 + ri(6), ri(2), 3); fx_step(); next_frame(); }
    fx_ticks(14); break;
  case TYPE_GHOST: case TYPE_DARK: case TYPE_DRAGON: // shadows spiralling in (DRAGON: rising round it)
    for (f = 0; f < 28; f++) {
      nsparks = 0;
      for (i = 0; i < 8; i++) {
        int a = f * 10 + i * 32, r = type == TYPE_DRAGON ? 22 : 34 - f;
        int y = type == TYPE_DRAGON ? ty + 18 - f * 36 / 28 + sinr(a, 6) : ty + sinr(a, r / 2);
        spark((tx + cosr(a, r)) * 16, y * 16, 0, 0, 0, 2, i % 2, type == TYPE_DRAGON ? 3 : 2);
      }
      next_frame();
    }
    nsparks = 0;
    for (i = 0; i < 12; i++) { int a = i * 256 / 12; spark(tx * 16, ty * 16, cosr(a, 30), sinr(a, 30), 0, 10, i % 2, 2); }
    fx_ticks(10); break;
  case TYPE_FLYING: // gusts streaking across
    for (f = 0; f < 18; f++) { if (f % 3 == 0) for (i = 0; i < 4; i++) spark((tx - 40 + i * 6) * 16, (ty - 16 + ri(32)) * 16, 64, -2, 0, 16, i % 2, 4); fx_step(); next_frame(); }
    fx_ticks(10); break;
  case TYPE_STEEL: case TYPE_ELECTRIC: // a hard clang of sparks (ELECTRIC: the crackle after the bolt)
    for (i = 0; i < 20; i++) { int a = ri(256), v = 30 + ri(30); spark(tx * 16, ty * 16, cosr(a, v), sinr(a, v), 0, 6 + ri(6), i % 2, 4); }
    fx_ticks(12); break;
  default: // NORMAL, FIGHTING, PSYCHIC: the burst on impact, a star for a body blow
    for (i = 0; i < 16; i++) { int a = i * 256 / 16, v = i % 2 ? 34 : 18; spark(tx * 16, ty * 16, cosr(a, v), sinr(a, v), 0, 12, i % 2 ? 0 : 1, i % 2 ? 3 : 2); }
    fx_ticks(14);
  }
}
