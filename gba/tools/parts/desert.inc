  // the desert (ROUTE 13 and the SUN RUINS): warm sand with speckles, dry grass in drifts where wild creatures hide,
  // saguaro cacti you walk around, packed sand on the path. A map with border 'a' is a desert map.
  t = &T_SAND; *t = cv(16, 16); p(SAND, 0, 0, 16, 16);
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) { if ((x * 5 + y * 3) % 19 == 0) p1(SAND_LO, x, y); else if ((x * 3 + y * 7) % 29 == 0) p1(0xfcf0d0, x, y); }
  t = &T_DUNE; *t = cv(16, 16); p(SAND, 0, 0, 16, 16);
  for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) { char ch = FERN[y][x]; if (ch != '.') p1(ch == 'd' ? 0x7a5a28 : ch == 'a' ? 0xe0c070 : 0xb08a40, x, y); }
  t = &T_PATH_SAND; *t = cv(16, 16); p(0xf4e6c0, 0, 0, 16, 16); for (int i = 0; i < 10; i++) p1(i % 3 ? 0xe2cfa0 : 0xfff4d8, hash(i, 11) % 16, hash(11, i) % 16);
  t = &T_CACTUS; *t = cv(16, 16); p(SAND, 0, 0, 16, 16); p(SAND_LO, 4, 14, 9, 2); // a shadow, a trunk and two arms
  p(0x2c5a28, 6, 1, 5, 14); p(0x4a9040, 7, 1, 3, 14); p(0x78c060, 7, 2, 1, 12);
  p(0x2c5a28, 2, 5, 4, 3); p(0x2c5a28, 2, 3, 3, 4); p(0x4a9040, 3, 3, 1, 4); p(0x4a9040, 3, 6, 3, 1);
  p(0x2c5a28, 11, 7, 3, 3); p(0x2c5a28, 12, 4, 3, 5); p(0x4a9040, 13, 4, 1, 5); p(0x4a9040, 11, 8, 2, 1);
  for (int i = 0; i < 7; i++) p1(0xf8f0c8, 6 + (i % 2) * 4, 2 + i * 2);
  t = &T_S_SAND; *t = cv(16, 16); p(SAND, 0, 0, 16, 16); p(SAND_LO, 4, 14, 9, 2); p(0x604020, 7, 9, 2, 6); p(0x503018, 2, 2, 12, 8); p(0xc89858, 3, 3, 10, 6); p(0xa87840, 4, 5, 8, 1); p(0xa87840, 4, 7, 6, 1);
  // the SUN RUINS (flag MF_SUN): the lighthouse's brick and slate in warm sandstone
  t = &T_BRICK_SUN; *t = cv(16, 16); p(0xb88a58, 0, 0, 16, 16);
  for (int r = 0; r < 4; r++) { int y = r * 4; p(0xd8ac78, 0, y, 16, 1); p(0x7a5430, 0, y + 3, 16, 1); for (int x = r % 2 ? 4 : 0; x < 16; x += 8) p(0x7a5430, x, y, 1, 3); }
  t = &T_SLATE_SUN; *t = cv(16, 16); p(0xe0c898, 0, 0, 16, 16); p(0xc4a878, 0, 0, 16, 1); p(0xc4a878, 0, 8, 16, 1); p(0xc4a878, 0, 0, 1, 16); p(0xc4a878, 8, 0, 1, 16);
  for (int i = 0; i < 6; i++) p(0xf0dcb0, 1 + hash(i, 17) % 14, 1 + hash(17, i) % 14, 2, 1);
