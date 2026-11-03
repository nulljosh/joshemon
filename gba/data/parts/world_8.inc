  [MAP_ROUTE7] = { .name = "ROUTE 7",
    .rows =
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~TTTT~~~~~~~~~~~~~~TTT~~~~"
    "~~~~T,,,,T~~~~~~~~~~~~T...T~~~"
    "~~~~T,,,,.~~~~~~~~~~~~T.S..~~~"
    "~~~~~T...~~~~~~~~~~~~~~TT~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~TTTT~~~~~~~~~~~"
    "~~~~~~~~~~~~~~T....~~~~~~~~~~~"
    "~~~~~~~~~~~~~~T....~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~TTTT~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
    .w = 30,
    .h = 14,
    .flags = MF_OUTDOOR,
    .nwarps = 11,
    .nnpcs = 4,
    .nsigns = 1,
    .warps = W6_route7,
    .npcs = N6_route7,
    .signs = S6_route7,
    .song = SONG_LUMEN,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 15,
    .grass = {49,56,49,25,56,49,102,56,25,49,102,26},
    .lvlo = 34,
    .lvhi = 37,
    .rate = 20,
    .water = {78,78,33,78,35,33,78,79,35,78,81,79},
    .wlo = 33,
    .whi = 37,
    .wrate = 4,
    .fish = {32,32,78,32,78,28,78,32,81,78,32,81},
    .flo = 20,
    .fhi = 30,
    .border = '~' },
  [MAP_LUMEN] = { .name = "LUMEN ISLE",
    .rows =
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~RRRR~~~~~~~~~~"
    "~~~~~~~~~~HWWH~~~~~~~~~~"
    "~~~~~~TTTTHHHHTTTT~~~~~~"
    "~~~~TT....HDHH....TT~~~~"
    "~~~T...f...p.....,,,T~~~"
    "~~T..RRRRR.p.BBBBB,,,T~~"
    "~~T..RRRRR.p.BBBBB,,.T~~"
    "~~T..HWHWH.p.HWHWH...T~~"
    "~~T..HHDHH.p.HHDHH...T~~"
    "~~T....p...p...p...S.T~~"
    "~~T....ppppppppppp...T~~"
    "~~T.,,,....p.........T~~"
    "~~T.,,,..GGGGGGG..f..T~~"
    "~~T......GGGGGGG.....T~~"
    "~~T..S...HHWHWHH.....p~~"
    "~~T......HHHDHHH.....pp~"
    "~~~T.f......p.......p~~~"
    "~~~~TTT.....ppppppppp~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~",
    .w = 24,
    .h = 20,
    .flags = MF_OUTDOOR,
    .nwarps = 33,
    .nnpcs = 4,
    .nsigns = 2,
    .warps = W6_lumen,
    .npcs = N6_lumen,
    .signs = S6_lumen,
    .song = SONG_LUMEN,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 16,
    .grass = {49,56,25,49,56,102,25,49,56,102,26,57},
    .lvlo = 34,
    .lvhi = 37,
    .rate = 20,
    .water = {78,78,33,78,35,33,78,79,35,78,81,79},
    .wlo = 33,
    .whi = 37,
    .wrate = 4,
    .fish = {32,32,78,32,78,28,78,32,81,78,32,81},
    .flo = 20,
    .fhi = 30,
    .border = '~' },
  [MAP_REST_LUMEN] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "yyoouuoooP"
    "oooouuoooo"
    "Poooooooyy"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W6_rest_lumen,
    .npcs = N6_rest_lumen,
    .signs = S6_rest_lumen,
    .song = SONG_REST,
    .nstock = 2,
    .stock = K6_rest,
    .town = "LUMEN",
    .node = 16 },
  [MAP_MART_LUMEN] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "hhoooooohh"
    "hhoouuoohh"
    "PooooooooP"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W6_mart_lumen,
    .npcs = N6_mart_lumen,
    .signs = S6_mart_lumen,
    .song = SONG_MART,
    .nstock = 6,
    .stock = K6_mart_lumen,
    .town = "LUMEN",
    .node = 16 },
  [MAP_LIGHTHOUSE1] = { .name = "LIGHTHOUSE",
    .rows =
    "bbbbbbbbbbbbbb"
    "bgggggbbgggUgb"
    "bgggggbbggggbb"
    "bggbggggggbggb"
    "bggbggggggbggb"
    "bggggbbbbggggb"
    "bggggbggbggggb"
    "bgggggggggbbgb"
    "bbbggggggggggb"
    "bggggggggbbggb"
    "bggggggggggggb"
    "bbbbbbmmbbbbbb",
    .w = 14,
    .h = 12,
    .flags = MF_OUTDOOR | MF_CAVE | MF_DARK,
    .nwarps = 3,
    .nnpcs = 3,
    .nsigns = 0,
    .warps = W6_lighthouse1,
    .npcs = N6_lighthouse1,
    .signs = S6_lighthouse1,
    .song = SONG_CAVE,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 16,
    .grass = {84,84,86,86,84,86,54,54,84,86,85,87},
    .lvlo = 34,
    .lvhi = 37,
    .rate = 8,
    .border = 'b' },
  [MAP_LIGHTHOUSE2] = { .name = "LIGHTHOUSE",
    .rows =
    "bbbbbbbbbbbbbb"
    "bUggggggggggbb"
    "bgggbbbbbgggbb"
    "bgggbgggbgggbb"
    "bgggbgggbggggb"
    "bggggggggggggb"
    "bbbbgggbbbgggb"
    "bgggggggbggggb"
    "bgggggggbggggb"
    "bggbbbbbbggggb"
    "bggggggggggggb"
    "bbbbbbbbbbbbbb",
    .w = 14,
    .h = 12,
    .flags = MF_OUTDOOR | MF_CAVE | MF_DARK,
    .nwarps = 1,
    .nnpcs = 5,
    .nsigns = 0,
    .warps = W6_lighthouse2,
    .npcs = N6_lighthouse2,
    .signs = S6_lighthouse2,
    .song = SONG_CAVE,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 16,
    .grass = {84,86,84,86,54,84,86,54,85,84,87,85},
    .lvlo = 35,
    .lvhi = 38,
    .rate = 8,
    .border = 'b' },
  [MAP_GYM6] = { .name = 0,
    .rows =
    "wvwwwwwwwwvw"
    "wwwwwwwwwwww"
    "oooooooooooo"
    "oPoooooooPoo"
    "oooooooooooo"
    "ouuuuooouuuo"
    "oooooooooooo"
    "oooPoooooPoo"
    "oooooooooooo"
    "oooooooooooo"
    "ouuuoooouuuo"
    "oooooooooooo"
    "PooooooooooP"
    "wwwwwmmwwwww",
    .w = 12,
    .h = 14,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 4,
    .nsigns = 0,
    .warps = W6_gym6,
    .npcs = N6_gym6,
    .signs = S6_gym6,
    .song = SONG_GYM,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 16 },
  [MAP_ROUTE8] = { .name = "ROUTE 8",
    .rows =
    "FFFFFFFFFFFppFFFFFFFFFFF"
    "FFFFFFFFFFFppFFFFFFFFFFF"
    "FFFFFFFFFFFppFFFFFFFFFFF"
    "FFFssssssssppsAAAAAAsFFF"
    "FFFssAAAAAsppsAAAAAAsFFF"
    "FFFssAAAAAssssAAAAAAsFFF"
    "FFFssAAAAAssssAAAAAAsFFF"
    "FFFssAAAAAsssssssssssFFF"
    "FFFssssssssssssXXXsFFFFF"
    "FFFFFsssssFFFssssssFFFFF"
    "FFFFFsssssFFFssssssFFFFF"
    "FFFFFssssssssssssssssFFF"
    "FFFssssssssssssAAAAAsFFF"
    "FFFsAAAAAssssssAAAAAsFFF"
    "FFFsAAAAAssssssAAAAAsFFF"
    "FFFsAAAAAsssXXsAAAAAsFFF"
    "FFFsAAAAAssssXsssssssFFF"
    "FFFssssssssssXsssssssFFF"
    "FFFXXXXXXXXXOXXXXXXXXFFF"
    "FFFssssssssssssssssssFFF"
    "FFFsssssssssssssSssssFFF"
    "~~~ssssssssssssssssss~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~",
    .w = 24,
    .h = 28,
    .flags = MF_OUTDOOR,
    .nwarps = 26,
    .nnpcs = 4,
    .nsigns = 1,
    .warps = W7_route8,
    .npcs = N7_route8,
    .signs = S7_route8,
    .song = SONG_FROST,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 17,
    .grass = {74,76,74,98,76,74,40,98,74,76,75,99},
    .lvlo = 36,
    .lvhi = 40,
    .rate = 20,
    .water = {78,78,33,78,35,33,78,79,35,78,81,79},
    .wlo = 33,
    .whi = 37,
    .wrate = 4,
    .fish = {32,32,78,32,78,28,78,32,81,78,32,81},
    .flo = 20,
    .fhi = 30,
    .border = 'F' },
  [MAP_FROSTGATE] = { .name = "FROSTGATE",
    .rows =
    "FFFFFFFFFFFEFFFFFFFFFFFF"
    "FFFFFFFFFFFpFFFFFFFFFFFF"
    "FFsssssssssppsssssssssFF"
    "FFssRRRRRssppsBBBBBsssFF"
    "FFssRRRRRssppsBBBBBsssFF"
    "FFssHWHWHssppsHWHWHsssFF"
    "FFssHHDHHssppsHHDHHsssFF"
    "sssssspppppppppppsssssFF"
    "sssssspppppppppppsssssFF"
    "FFssssssGGGGGGGsssssssFF"
    "FFssssssGGGGGGGsBBBBBsFF"
    "FFssssssHHWHWHHsBBBBBsFF"
    "FFssssssHHHDHHHsHWHWHsFF"
    "FFsssssssssppsssHHWHHsFF"
    "FFsssssssSsppsssssssssFF"
    "FFsAAAAssssppsSssAAAAsFF"
    "FFsAAAAssssppssssAAAAsFF"
    "FFsssssssssppsssssssssFF"
    "FFFFFFFFFFFppFFFFFFFFFFF"
    "FFFFFFFFFFFppFFFFFFFFFFF",
    .w = 24,
    .h = 20,
    .flags = MF_OUTDOOR,
    .nwarps = 8,
    .nnpcs = 4,
    .nsigns = 2,
    .warps = W7_frost,
    .npcs = N7_frost,
    .signs = S7_frost,
    .song = SONG_FROST,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 18,
    .border = 'F' },
  [MAP_REST_FROST] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "yyoouuoooP"
    "oooouuoooo"
    "Poooooooyy"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W7_rest_frost,
    .npcs = N7_rest_frost,
    .signs = S7_rest_frost,
    .song = SONG_REST,
    .nstock = 2,
    .stock = K6_rest,
    .town = "FROSTGATE",
    .node = 18 },
  [MAP_MART_FROST] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "hhoooooohh"
    "hhoouuoohh"
    "PooooooooP"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W7_mart_frost,
    .npcs = N7_mart_frost,
    .signs = S7_mart_frost,
    .song = SONG_MART,
    .nstock = 7,
    .stock = K7_mart_frost,
    .town = "FROSTGATE",
    .node = 18 },
  [MAP_ICECAVE] = { .name = "ICE CAVE",
    .rows =
    "QQQQQQQQQQQQQQQQQQQQ"
    "QQeeeeeeeeeeeeeeeeQQ"
    "QQeeeeeeeeeeeeeeeeQQ"
    "QQQQQQeQQQQQQQQQQQQQ"
    "QQIIIIIIIIIIQIIIIIQQ"
    "QQIQIIIIIIIIIIIIIIQQ"
    "QQIIIIIQIIIIIIIIIIQQ"
    "QQIIIIIIIIIIIQIIIIQQ"
    "QQIIIIIIIIQIIIIQIIQQ"
    "QQIIQIIIIIIIIIIIIIQQ"
    "QQIIIIQIIIIIIIIIQIQQ"
    "QQIIIIIIIQQIIIIIIIQQ"
    "QQeeeeeeeeeeeeeeeeQQ"
    "QQeeeeeeeeeeeeeeeeQQ"
    "QQeeeeeeeeeeeeeeeeQQ"
    "QQQQQQQQQmmQQQQQQQQQ",
    .w = 20,
    .h = 16,
    .flags = MF_CAVE,
    .nwarps = 2,
    .nnpcs = 4,
    .nsigns = 0,
    .warps = W7_icecave,
    .npcs = N7_icecave,
    .signs = S7_icecave,
    .song = SONG_CAVE,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 19,
    .grass = {74,74,76,44,75,74,76,45,74,75,51,77},
    .lvlo = 37,
    .lvhi = 41,
    .rate = 10,
    .border = 'Q' },
  [MAP_GYM7] = { .name = 0,
    .rows =
    "wvwwwwwwwwwwvw"
    "wwwwwwwwwwwwww"
    "oooooooooooooo"
    "PooooooooooooP"
    "QQQQQIIIIQQQQQ"
    "IIIIIIIIIIIQII"
    "IIQIIIIIIIQIII"
    "IIIIIIQIIIIIII"
    "IIIQIIIIIIIIQI"
    "ooIIIIIIIQIIII"
    "IQIIIIIIIIIIoo"
    "IIIIQIIIQIIIoo"
    "oooooooooooooo"
    "oooooooooooooo"
    "PooooooooooooP"
    "wwwwwwmmwwwwww",
    .w = 14,
    .h = 16,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 4,
    .nsigns = 0,
    .warps = W7_gym7,
    .npcs = N7_gym7,
    .signs = S7_gym7,
    .song = SONG_GYM,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 18 },
  [MAP_ROUTE9] = { .name = "FROZEN BAY",
    .rows =
    "FFFFFFFFFFFFFFFFFFFFFFFF"
    "FFFFFFFFFFFFFFFFFFFFFFFF"
    "FFFFFFFFFFFFFFFFFFFFFFFF"
    "FFFFFFFFFFFsssssssFFFFFF"
    "FFFFFFFFFssssAAAAsssFFFF"
    "FFFFFFFsssssAAAAAAsssFFF"
    "FFFFFsssAAAAsssssssssFFF"
    "FFFFssssAAAAssssssSsssss"
    "FFFsssssssssssssssssssss"
    "FFssssssssAAAAssssssFFFF"
    "FsssssssssAAAAsssssFFFFF"
    "Fsssss~~~~~~~~ssssssFFFF"
    "Fsss~~~~~~~~~~~~sssssFFF"
    "Fss~~~~~~~~~~~~~~~sssFFF"
    "Fs~~~~~~~~~~~~~~~~~ssFFF"
    "F~~~~~~~~~~~~~~~~~~~sFFF"
    "~~~~~~~~~~~~~~~~~~~~~FFF"
    "~~~~XXXX~~~~~~~~~~~~~~FF"
    "~~~XXXXXX~~~~~~~~~~~~~~~"
    "~~~XXXEXX~~~~~~~~~~~~~~~"
    "~~~~sssss~~~~~~~~~~~~~~~"
    "~~~~~sss~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~"
    "XXXXXXXXXXXXXXXXXXXXX~XX",
    .w = 24,
    .h = 24,
    .flags = MF_OUTDOOR,
    .nwarps = 4,
    .nnpcs = 8,
    .nsigns = 1,
    .warps = W8_route9,
    .npcs = N8_route9,
    .signs = S8_route9,
    .song = SONG_FROST,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 20,
    .grass = {76,75,74,76,75,74,77,75,76,99,77,75},
    .lvlo = 38,
    .lvhi = 42,
    .rate = 20,
    .water = {28,29,104,78,34,29,28,79,34,104,29,79},
    .wlo = 36,
    .whi = 41,
    .wrate = 4,
    .fish = {32,32,104,33,32,104,28,33,32,104,33,104},
    .flo = 25,
    .fhi = 38,
    .border = 'F' },
  [MAP_SHRINE] = { .name = "SUNKEN SHRINE",
    .rows =
    "bbbbbbbbbbbbbbbbbbbb"
    "bbbbbbbb~~~~bbbbbbbb"
    "bbggSggg~~~~ggggggbb"
    "bbggggggggggggggggbb"
    "bbggggggggggggggggbb"
    "bbbbggggggggggggbbbb"
    "bbbbbbbbbgbbbbbbbbbb"
    "bggggggggggggggggggb"
    "b~~~~~~~~~~~~~~~~~~b"
    "b~~~~~~~~~~~~~~~~~~b"
    "b~~~bb~~~~~~~~bb~~~b"
    "b~~~bb~~~~~~~~bb~~~b"
    "b~~~~~~~~~~~~~~~~~~b"
    "b~~~~~~~~~~~~~~~~~~b"
    "bggggggggggggggggggb"
    "bggggggggggggggggggb"
    "bbbbbbbbbggbbbbbbbbb"
    "bbbbbbbbbggbbbbbbbbb",
    .w = 20,
    .h = 18,
    .flags = MF_CAVE,
    .nwarps = 2,
    .nnpcs = 5,
    .nsigns = 1,
    .warps = W8_shrine,
    .npcs = N8_shrine,
    .signs = S8_shrine,
    .song = SONG_CAVE,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 21,
    .grass = {85,42,43,84,85,42,86,43,85,87,42,86},
    .lvlo = 40,
    .lvhi = 44,
    .rate = 10,
    .water = {104,29,104,28,29,104,29,104,28,104,29,104},
    .wlo = 40,
    .whi = 44,
    .wrate = 4,
    .border = 'b' },
  [MAP_DRAKE] = { .name = "DRAKESPIRE",
    .rows =
    "XXXXXXXXXX~~~~XXXXXXXXXX"
    "XXXXXXXXX~~~~~~XXXXXXXXX"
    "XXXTTTT~~~~~~~~~~TTTTXXX"
    "XXT,,,..~~~~~~~~..,,,TXX"
    "XT,,,....pppppp....,,,TX"
    "XT..RRRRR..pp..BBBBB..TX"
    "XT..RRRRR..pp..BBBBB..TX"
    "XT..HWHWH..pp..HWHWH..TX"
    "XT..HHDHH..pp..HHDHH..TX"
    "XT....p.S..pp....p....TX"
    "XT....pppppppppppp....TX"
    "XT.f...p............f.TX"
    "XT.....p.GGGGGGG......TX"
    "XT.....p.GGGGGGG......TX"
    "XT..S..p.HHWHWHH..,,,.TX"
    "XT.....p.HHHDHHH..,,,.TX"
    "XT.....pppppp.....,,,.TX"
    "XTT.........pp......TTXX"
    "XXXTTT......pp....TTTXXX"
    "XXXXXXX.....pp...XXXXXXX"
    "XXXXXXXXXXXXEXXXXXXXXXXX"
    "XXXXXXXXXXXXXXXXXXXXXXXX",
    .w = 24,
    .h = 22,
    .flags = MF_OUTDOOR,
    .nwarps = 8,
    .nnpcs = 3,
    .nsigns = 2,
    .warps = W8_drake,
    .npcs = N8_drake,
    .signs = S8_drake,
    .song = SONG_DRAKE,
    .nstock = 2,
    .stock = K6_rest,
    .town = "DRAKESPIRE",
    .node = 22,
    .grass = {88,58,89,59,88,66,58,88,69,59,89,90},
    .lvlo = 40,
    .lvhi = 44,
    .rate = 15,
    .border = 'X' },
  [MAP_REST_DRAKE] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "yyoouuoooP"
    "oooouuoooo"
    "Poooooooyy"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W8_rest_drake,
    .npcs = N8_rest_drake,
    .signs = S8_rest_drake,
    .song = SONG_REST,
    .nstock = 2,
    .stock = K6_rest,
    .town = "DRAKESPIRE",
    .node = 22 },
  [MAP_MART_DRAKE] = { .name = 0,
    .rows =
    "wvwwrrwwvw"
    "wwwwwwwwww"
    "oooccccooo"
    "oooooooooo"
    "hhoooooohh"
    "hhoouuoohh"
    "PooooooooP"
    "wwwwmmwwww",
    .w = 10,
    .h = 8,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 2,
    .nsigns = 0,
    .warps = W8_mart_drake,
    .npcs = N8_mart_drake,
    .signs = S8_mart_drake,
    .song = SONG_MART,
    .nstock = 7,
    .stock = K7_mart_frost,
    .town = "DRAKESPIRE",
    .node = 22 },
  [MAP_GYM8] = { .name = 0,
    .rows =
    "wvwwwwwwwwwwvw"
    "wwwwwwwwwwwwww"
    "PooooooooooooP"
    "oooooooooooooo"
    "oooooooooooooo"
    "hhhhhhOhhhhhhh"
    "oooooooooooooo"
    "oPooooooooPooo"
    "oooooooooooooo"
    "hhhhhhhhOhhhhh"
    "oooooooooooooo"
    "ouuuoooooouuuo"
    "oooooooooooooo"
    "PooooooooooooP"
    "oooooooooooooo"
    "wwwwwwmmwwwwww",
    .w = 14,
    .h = 16,
    .flags = 0,
    .nwarps = 2,
    .nnpcs = 4,
    .nsigns = 0,
    .warps = W8_gym8,
    .npcs = N8_gym8,
    .signs = S8_gym8,
    .song = SONG_GYM,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 22 },
  [MAP_VICTORY1] = { .name = "VICTORY ROAD",
    .rows =
    "XXXXXXXXXXXXEXXXXXXXXXXX"
    "XXXXqqqqqqqqqqqqqqqqXXXX"
    "XXqqqqqqqqqqqqqqqqqqqqXX"
    "XqqqqXXXqqqqqqqqXXXqqqqX"
    "XqqqqXXXqqqqqqqqXXXqqqqX"
    "XXXXXXXXXXXXYXXXXXXXXXXX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "X~~~~~~~~~~~~~~~~~~~~~~X"
    "X~~~~~XX~~~~~~~~~XX~~~~X"
    "X~~~~~XX~~~~~~~~~XX~~~~X"
    "X~~~~~~~~~~~~~~~~~~~~~~X"
    "X~~~~~~~~~~~~~~~~~~~~~~X"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XqqXXXXXXXXqXXXXXXXXqqqX"
    "XXXXXXXXXXXOXXXXXXXXXXXX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XqqXXXqqqqqqqqqqqXXqqqqX"
    "XqqqqqqqqqqqqqqqqqqqqqqX"
    "XXXXXXXXXXXXXXXXXXXXEXXX",
    .w = 24,
    .h = 22,
    .flags = MF_CAVE,
    .nwarps = 2,
    .nnpcs = 5,
    .nsigns = 0,
    .warps = W9_vr1,
    .npcs = N9_vr1,
    .signs = S9_vr1,
    .song = SONG_VICTORY,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 23,
    .grass = {46,48,53,52,64,63,61,96,92,83,46,48},
    .lvlo = 44,
    .lvhi = 49,
    .rate = 10,
    .water = {104,33,29,104,33,29,104,33,29,104,33,104},
    .wlo = 42,
    .whi = 48,
    .wrate = 4,
    .border = 'X' },
  [MAP_VICTORY2] = { .name = "VICTORY ROAD",
    .rows =
    "XXXXXXXXXXXXXXXXEXXX"
    "XqqqqqqXXXXqqqqqqqqX"
    "XqXXXXqXqqqqXXXXXqqX"
    "XqXqqqqXqXXqqqqqXqqX"
    "XqXqXXXXqXqqXXXqXXqX"
    "XqqqXqqqqXqXqqXqqqqX"
    "XXXqXqXXXXqXqXXXXXqX"
    "XqqqqqXqqqqXqqqqqXqX"
    "XqXXXXXqXXXXXXXXqXqX"
    "XqXqqqqqqqqqqqqXqXqX"
    "XqXqXXXXXXXXXXqXqqqX"
    "XqqqXqqqqqqqqXqXXXqX"
    "XXXXXqXXXXXXqXqqqqqX"
    "XqqqqqXqqqqXqXXXXXqX"
    "XqXXXXXqXXqXqqqqqXqX"
    "XqqqqqqqqqqqqqXqqqqX"
    "XXXXXXXXXqXXXXXXXXXX"
    "XXXXXXXXXEXXXXXXXXXX",
    .w = 20,
    .h = 18,
    .flags = MF_CAVE | MF_DARK,
    .nwarps = 2,
    .nnpcs = 3,
    .nsigns = 0,
    .warps = W9_vr2,
    .npcs = N9_vr2,
    .signs = S9_vr2,
    .song = SONG_VICTORY,
    .nstock = 2,
    .stock = K6_rest,
    .town = 0,
    .node = 23,
    .grass = {52,53,64,46,48,92,63,61,96,83,52,64},
    .lvlo = 45,
    .lvhi = 50,
    .rate = 10,
    .border = 'X' },
