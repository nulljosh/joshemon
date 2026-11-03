  [MAP_ROUTE12] = { .name = "ROUTE 12",
    .rows =
    "XXXXXXXXXXXXXXXXXXXXXXXX"
    "XXXXX,,,,XXXXXXXX,,,,XXX"
    "XXX,,,,,,,,XXXXX,,,,,,XX"
    "XX,,,..,,,,,.XX..,,,,,XX"
    "XX..TT...,,,...........X"
    "X...TT.......TT...XXXEXX"
    "X..........,,TT.....p..X"
    "X.S..,,,.......,,,..p..X"
    "ppppp,,,ppppppppppppp..X"
    "X....,,,....,,,,.......X"
    "XLLLLLL.....LLLLLLL..LLX"
    "X......................X"
    "X.f.....,,,,,,....f....X"
    "X..TT...,,,,,,....TT...X"
    "X......................X"
    ".....~~~~~~~~~~~~......."
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~",
    .w = 24, .h = 18, .flags = MF_OUTDOOR, .nwarps = 2, .nnpcs = 6, .nsigns = 1,
    .warps = Wco_route12, .npcs = Nco_route12, .signs = Sco_route12,
    .song = SONG_WORLD, .nstock = 2, .stock = K6_rest, .town = 0, .node = 28,
    .grass = {69,99,41,71,73,69,99,41,71,59,73,102}, .lvlo = 54, .lvhi = 58, .rate = 20,
    .water = {33,31,35,79,81,33,31,35,79,81,33,79}, .wlo = 52, .whi = 57, .wrate = 4,
    .fish = {33,32,33,79,32,33,81,79,33,32,104,81}, .flo = 40, .fhi = 50,
    .border = 'T' },
  [MAP_GROTTO] = { .name = "ELDER GROTTO",
    .rows =
    "XXXXXXXXXXXXXXXXXXXX"
    "XXXXXXXqqqqqqXXXXXXX"
    "XXXXXqqqqqqqqqqXXXXX"
    "XXXqqqqqXXXXqqqqqXXX"
    "XXqqqqqXX~~XXqqqqqXX"
    "XXqqqqXX~~~~XXqqqqXX"
    "XXXXqXX~~~~~~XXqXXXX"
    "XXXXYXX~~~~~~XXOXXXX"
    "XXqqqXX~~~~~~XXqqqXX"
    "XXqqqqXX~~~~XXqqqqXX"
    "XXXqqqqXX~~XXqqqqXXX"
    "XXXXqqqqXqqXqqqqXXXX"
    "XXXXXqqqqqqqqqqXXXXX"
    "XXXXXXqqqqqqqqXXXXXX"
    "XXXXXXXqqqqqqXXXXXXX"
    "XXXXXXXXqqqqXXXXXXXX"
    "XXXXXXXXXqqXXXXXXXXX"
    "XXXXXXXXXqqXXXXXXXXX",
    .w = 20, .h = 18, .flags = MF_CAVE, .nwarps = 2, .nnpcs = 5, .nsigns = 0,
    .warps = Wgr_grotto, .npcs = Ngr_grotto, .signs = Sgr_grotto,
    .song = SONG_CAVE, .nstock = 2, .stock = K6_rest, .town = 0, .node = 29,
    .grass = {46,53,85,43,89,46,85,53,101,43,89,97}, .lvlo = 55, .lvhi = 60, .rate = 12,
    .water = {33,79,81,33,79,81,33,79,81,33,79,104}, .wlo = 54, .whi = 58, .wrate = 4,
    .border = 'X' },
