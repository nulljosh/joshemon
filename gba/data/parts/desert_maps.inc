  [MAP_ROUTE13] = { .name = "ROUTE 13",
    .rows =
    "XXXXXXXXXXXXXXXXXXXXXXXX"
    "XXXXXXXXXXXEXXXXXXXXXXXX"
    "XXaaaaaaaaapaaaaaaaaaaXX"
    "XaaaCaaddddpddddaaaCaaaX"
    "XaaaaaadddapadddaaaaaaaX"
    "XaaddaaaaaapaaaaaaddaaaX"
    "XaadddaCaaapaaaCaadddaaX"
    "XaaddaaaaaappppppaaddaaX"
    "XaaaaaaaaaaaaaaapaaaaaaX"
    "XaCaaaaddddaaaaapaaaCaaX"
    "XaaaaadddddaTT~~paaaaaaX"
    "XaaaaaaddddaT~~~paaaaaaX"
    "XaaCaaaaaaaaaTT.paaaCaaX"
    "XaaaaaaaapppppppaaaaaaaX"
    "XaaddaaaapaaaaaaaaddaaaX"
    "XadddaaaapaaaCaaaddddaaX"
    "XaaddaaaapaaaaaaaaddaaaX"
    "XaaaaaCaapaaaaaaCaaaaaaX"
    "XaaaaaaaapaaaaaaaaaaaaaX"
    "XaaddddaapaadddddaaaaaaX"
    "XaadddaaapaaddddaaaCaaaX"
    "XaaaaaaaapaaaaaaaaaaaaaX"
    "XaaCaaaaapaaaaaCaaaaaaaX"
    "XaaaaaaaSpaaaaaaaaaaaaaX"
    "XXXXXXXXXpXXXXXXXXXXXXXX"
    "XXXXXXXXXpXXXXXXXXXXXXXX",
    .w = 24, .h = 26, .flags = MF_OUTDOOR, .nwarps = 2, .nnpcs = 6, .nsigns = 1,
    .warps = Wde_route13, .npcs = Nde_route13, .signs = Sde_route13,
    .song = SONG_DUNES, .nstock = 2, .stock = K6_rest, .town = 0, .node = 30,
    .grass = {100,82,38,100,60,82,44,38,100,82,60,86}, .lvlo = 16, .lvhi = 20, .rate = 22,
    .water = {30,28,30,28,30,28,30,28,30,28,32,32}, .wlo = 15, .whi = 19, .wrate = 4,
    .fish = {32,32,30,32,30,28,32,30,32,28,32,104}, .flo = 10, .fhi = 16,
    .border = 'a' },
  [MAP_RUINS] = { .name = "SUN RUINS",
    .rows =
    "bbbbbbbbbbbbbbbb"
    "bbbbbbbggbbbbbbb"
    "bbbggggggggggbbb"
    "bbgggbbggbbgggbb"
    "bbggbbbggbbbggbb"
    "bbggggggggggggbb"
    "bbbbggbbbbggbbbb"
    "bbggggbbbbggggbb"
    "bbggbbggggbbggbb"
    "bbggggggggggggbb"
    "bbbbbbbggbbbbbbb"
    "bbbbbbbggbbbbbbb"
    "bbbbbbbggbbbbbbb"
    "bbbbbbbggbbbbbbb",
    .w = 16, .h = 14, .flags = MF_CAVE | MF_SUN, .nwarps = 2, .nnpcs = 4, .nsigns = 0,
    .warps = Wru_ruins, .npcs = Nru_ruins, .signs = Sru_ruins,
    .song = SONG_CAVE, .nstock = 2, .stock = K6_rest, .town = 0, .node = 31,
    .grass = {84,49,86,42,25,84,49,86,42,25,56,84}, .lvlo = 17, .lvhi = 21, .rate = 10,
    .border = 'b' },
