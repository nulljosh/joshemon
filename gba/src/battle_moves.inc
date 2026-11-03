// ---------- moves with rules of their own, after Gen 3 (pret/pokeruby src/battle_script_commands.c)
// Per-side state lives in B.fx: protect, focus, seeds and the like belong to the creature out and clear when it
// leaves (new_side); MIST, REFLECT, LIGHT SCREEN and SAFEGUARD belong to the side and run out after 5 turns.
enum { MV_MIST = 54, MV_SURF = 57, MV_LEECH_SEED = 73, MV_STUN_SPORE = 78, MV_EARTHQUAKE = 89, MV_TELEPORT = 100, MV_LIGHT_SCREEN = 113,
  MV_HAZE = 114, MV_REFLECT = 115, MV_FOCUS_ENERGY = 116, MV_MIRROR_MOVE = 119, MV_SWIFT = 129, MV_REST = 156,
  MV_STRUGGLE = 165, MV_SPIDER_WEB = 169, MV_PROTECT = 182, MV_BELLY_DRUM = 187, MV_FORESIGHT = 193, MV_DETECT = 197,
  MV_ENDURE = 203, MV_MEAN_LOOK = 212, MV_HEAL_BELL = 215, MV_SAFEGUARD = 219, MV_MAGNITUDE = 222, MV_PSYCH_UP = 244,
  MV_SHADOW_BALL = 247, MV_NATURE_POWER = 267, MV_REFRESH = 287, MV_MUD_SPORT = 300, MV_AROMATHERAPY = 312,
  MV_ODOR_SLEUTH = 316, MV_BLOCK = 335, MV_WATER_SPORT = 346, MV_FLAIL = 175, MV_REVERSAL = 179 };

static int fixed_damage(int id, const Mon *a, const Mon *d) { // the no-power moves that still hurt
  if (id == 12 || id == 32 || id == 90 || id == 329) return d->hp; // GUILLOTINE, HORN DRILL, FISSURE, SHEER COLD
  if (id == 68) return B.fx[a == B.m[ME] ? ME : FOE].took * 2; // COUNTER: twice the physical damage taken this turn
  if (id == 149) { int v = a->level * (ri(11) * 10 + 50) / 100; return v < 1 ? 1 : v; } // PSYWAVE
  if (id == 283) return d->hp > a->hp ? d->hp - a->hp : 0; // ENDEAVOR
  switch (id) { case 49: return 20; case 82: return 40; case 69: case 101: return a->level; case 162: return d->hp / 2 > 1 ? d->hp / 2 : 1; }
  return 0;
}
// MAGNITUDE rolls its power, FLAIL and REVERSAL grow as HP falls (sFlailHpScaleToPowerTable)
static int var_power(int id, const Mon *a) {
  if (id == MV_MAGNITUDE) {
    static const u8 P[] = { 10, 30, 50, 70, 90, 110, 150 }, C[] = { 5, 15, 35, 65, 85, 95, 100 };
    int r = ri(100), k = 0; while (r >= C[k]) k++;
    blen = 0; cat("MAGNITUDE "); cat_num(k + 4); cat("!"); bsay(buf); return P[k];
  }
  if (id == MV_FLAIL || id == MV_REVERSAL) { int n = 48 * a->hp / a->stats[0]; return n <= 1 ? 200 : n <= 4 ? 150 : n <= 9 ? 100 : n <= 16 ? 80 : n <= 32 ? 40 : 20; }
  return 0;
}
static int use_move(int side, Slot *mv);
static int call_move(int side, int id) { static Slot s; s.id = id; s.pp = 99; return use_move(side, &s); } // MIRROR MOVE, NATURE POWER
// PROTECT, DETECT, ENDURE: each success in a row halves the next one's odds, and moving last always fails
static int guard_ok(int side) {
  int n = B.fx[side].protn; if (n > 3) n = 3;
  int ok = !B.fx[side ^ 1].acted && (n == 0 || ri(1 << n) == 0);
  B.fx[side].protn = ok ? B.fx[side].protn + 1 : 0;
  return ok;
}
static void side_note(int side, const char *what) { blen = 0; label(side); cat(what); bsay(buf); }
// returns -1 when the move isn't one of these, else the battle result
static int special_move(int side, int id) {
  Mon *a = B.m[side], *d = B.m[side ^ 1]; int o = side ^ 1; Fx *f = &B.fx[side], *g = &B.fx[o];
  switch (id) {
  case MV_PROTECT: case MV_DETECT: if (!guard_ok(side)) break; f->protect = 1; bsayf(side, " protected itself!"); return R_NONE;
  case MV_ENDURE: if (!guard_ok(side)) break; f->endure = 1; bsayf(side, " braced itself!"); return R_NONE;
  case MV_FOCUS_ENERGY: if (f->focus) break; f->focus = 1; bsayf(side, " is getting pumped!"); return R_NONE;
  case MV_REST:
    if (a->hp >= a->stats[0]) { bsayf(side, "'s HP is full!"); return R_NONE; }
    if (abil(side) == AB_INSOMNIA) break;
    a->status = ST_SLP; a->sleep = 3; B.tox[side] = 0; a->hp = a->stats[0]; drain(side); hud(side);
    bsayf(side, " slept and became healthy!"); return R_NONE;
  case MV_TELEPORT: if (B.tr || story_battle || f->trap) break; bsayf(side, " fled from battle!"); return R_RAN;
  case MV_HAZE: for (int s = 0; s < 2; s++) for (int i = 0; i < 8; i++) B.st[s][i] = 6; bsay("All STAT changes were eliminated!"); return R_NONE;
  case MV_BELLY_DRUM:
    if (a->hp <= a->stats[0] / 2 || B.st[side][1] >= 12) break;
    hit(side, a->stats[0] / 2); B.st[side][1] = 12; bsayf(side, " cut its own HP and maximized ATTACK!"); return R_NONE;
  case MV_PSYCH_UP: for (int i = 0; i < 8; i++) B.st[side][i] = B.st[o][i]; blen = 0; label(side); cat(" copied "); label(o); cat("'s stat changes!"); bsay(buf); return R_NONE;
  case MV_MIST: if (f->mist) break; f->mist = 5; side_note(side, " became shrouded in MIST!"); return R_NONE;
  case MV_REFLECT: if (f->refl) break; f->refl = 5; bsay("REFLECT raised DEFENSE!"); return R_NONE;
  case MV_LIGHT_SCREEN: if (f->light) break; f->light = 5; bsay("LIGHT SCREEN raised SP. DEF!"); return R_NONE;
  case MV_SAFEGUARD: if (f->safe) break; f->safe = 5; side_note(side, " is covered by a veil!"); return R_NONE;
  case MV_LEECH_SEED:
    if (g->seed || has_type(d, TYPE_GRASS)) { bsayf(o, " evaded the attack!"); return R_NONE; }
    g->seed = 1; bsayf(o, " was seeded!"); return R_NONE;
  case MV_FORESIGHT: case MV_ODOR_SLEUTH: g->fore = 1; blen = 0; label(side); cat(" identified "); label(o); cat("!"); bsay(buf); return R_NONE;
  case MV_MEAN_LOOK: case MV_SPIDER_WEB: case MV_BLOCK: if (g->trap) break; g->trap = 1; bsayf(o, " can't escape now!"); return R_NONE;
  case MV_MUD_SPORT: case MV_WATER_SPORT: f->sport |= id == MV_MUD_SPORT ? 1 : 2; bsay(id == MV_MUD_SPORT ? "ELECTRIC's power was weakened!" : "FIRE's power was weakened!"); return R_NONE;
  case MV_HEAL_BELL: case MV_AROMATHERAPY: {
    bsay(id == MV_HEAL_BELL ? "A bell chimed!" : "A soothing aroma wafted through the area!");
    Mon *team = side == ME ? S.party : B.tr ? B.tparty : &B.foe; int n = side == ME ? S.nparty : B.tr ? B.tr->nparty : 1;
    for (int i = 0; i < n; i++) team[i].status = team[i].sleep = 0;
    B.tox[side] = 0; hud(side); return R_NONE; }
  case MV_REFRESH:
    if (!(a->status == ST_PSN || a->status == ST_TOX || a->status == ST_PAR || a->status == ST_BRN)) break;
    a->status = 0; hud(side); bsayf(side, "'s status returned to normal!"); return R_NONE;
  case MV_MIRROR_MOVE: { int m = g->last; if (!m || m == MV_MIRROR_MOVE || MOVES[m].tgt) break; return call_move(side, m); }
  case MV_NATURE_POWER: { // sNaturePowerMoves: what the place turns it into
    int m = B.scene == SCENE_WATER ? MV_SURF : B.scene == SCENE_CAVE ? MV_SHADOW_BALL : B.scene == SCENE_INDOOR ? MV_SWIFT : mi->border == 'a' ? MV_EARTHQUAKE : mi->border == 'F' ? MV_SWIFT : MV_STUN_SPORE;
    blen = 0; cat("NATURE POWER turned into "); cat(MOVES[m].name); cat("!"); bsay(buf); return call_move(side, m); }
  default: return -1;
  }
  bsay("But it failed!"); return R_NONE;
}
// end of turn: LEECH SEED drains, the 5-turn side effects run out, this turn's guards drop
static int moves_end_of_turn(void) {
  for (int s = 0; s < 2; s++) {
    Fx *f = &B.fx[s]; f->protect = f->endure = f->acted = 0; f->took = 0;
    static const char *const OFF[] = { "'s MIST wore off!", "'s REFLECT wore off!", "'s LIGHT SCREEN wore off!", "'s SAFEGUARD wore off!" };
    u8 *t[4] = { &f->mist, &f->refl, &f->light, &f->safe };
    for (int k = 0; k < 4; k++) if (*t[k] && !--*t[k]) side_note(s, OFF[k]);
    Mon *m = B.m[s], *o = B.m[s ^ 1];
    if (f->seed && m->hp && o->hp) {
      int dmg = m->stats[0] / 8; if (dmg < 1) dmg = 1; if (dmg > m->hp) dmg = m->hp;
      hit(s, dmg); o->hp = o->hp + dmg > o->stats[0] ? o->stats[0] : o->hp + dmg; drain(s ^ 1);
      bsayf(s, "'s health is sapped by LEECH SEED!");
      if (!m->hp) { int r = faint(s); if (r) return r; }
    }
  }
  return R_NONE;
}
