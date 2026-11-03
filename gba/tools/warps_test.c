// make test: every warp sits on its map and lands on a tile you can stand on (the monkey finds hangs, not dead ends).
// Maps join only through warps (off the edge is trees, main.c tile_at), so checking warps covers the edges too.
#include <stdio.h>
#include <string.h>
#include "world.h"
static const char SOLID[] = "KYOnT~RBGHWSLwvrPychXbFQC"; // src/main.c SOLID
int main(void) {
  int bad = 0;
  for (int m = 0; m < MAP_COUNT; m++) {
    const MapInfo *mi = &MAP_INFO[m];
    for (int i = 0; i < mi->nwarps; i++) {
      const Warp *w = &mi->warps[i];
      if (w->x >= mi->w || w->y >= mi->h) { printf("%s warp %d at %d,%d is off the map\n", mi->name, i, w->x, w->y); bad++; continue; }
      if (w->map >= MAP_COUNT) { printf("%s warp %d goes to map %d, past MAP_COUNT\n", mi->name, i, w->map); bad++; continue; }
      const MapInfo *to = &MAP_INFO[w->map];
      if (w->tx >= to->w || w->ty >= to->h) { printf("%s warp %d lands off %s at %d,%d\n", mi->name, i, to->name, w->tx, w->ty); bad++; continue; }
      char t = to->rows[w->ty * to->w + w->tx];
      // warp() keeps you surfing onto '~' only when you were riding, i.e. stood on water
      if (t == '~' && mi->rows[w->y * mi->w + w->x] == '~') continue;
      if (strchr(SOLID, t)) { printf("%s warp %d lands on '%c' in %s at %d,%d\n", mi->name, i, t, to->name, w->tx, w->ty); bad++; }
    }
  }
  if (bad) return 1;
  puts("warps ok");
  return 0;
}
