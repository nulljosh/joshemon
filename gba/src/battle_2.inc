static void contact_fx(int side) { // the holder's STATIC, POISON POINT, EFFECT SPORE, ROUGH SKIN, after each contact hit
  int o = side ^ 1, ab = abil(o), ail = 0;
  if (ab == AB_STATIC && ri(3) == 0) ail = AIL_PAR;
  if (ab == AB_POISON_POINT && ri(3) == 0) ail = AIL_PSN;
  if (ab == AB_EFFECT_SPORE && ri(10) == 0) ail = (const u8[]){ AIL_PSN, AIL_PAR, AIL_SLP }[ri(3)];
  if (ail && can_status(side, ST_OF[ail])) { ab_say(o, "!"); inflict(side, ail, 0, 0); }
  if (ab == AB_ROUGH_SKIN) { int r = B.m[side]->stats[0] / 16; hit(side, r < 1 ? 1 : r); blen = 0; label(side); cat(" was hurt by "); label(o); cat("'s ROUGH SKIN!"); bsay(buf); }
}
#include "battle_moves.inc" // PROTECT, REST, LEECH SEED, MAGNITUDE and the rest
static int use_move(int side, Slot *mv) {
  Mon *a = B.m[side], *d = B.m[side ^ 1]; int dside = side ^ 1; const Move *M = &MOVES[mv->id];
#if !defined QA_FIELD || defined QA_REAL
  if (side == ME && mv->id != MV_STRUGGLE) { mv->pp--; if (abil(FOE) == AB_PRESSURE && !MOVES[mv->id].tgt && mv->pp) mv->pp--; }
#endif // the QA build's one L80 runs whole gauntlets (the LEAGUE) on mashed A, so it never runs out
  blen = 0; label(side); cat(" used "); cat(M->name); cat("!"); bsay(buf);
  s8 *st = B.st[side], *dst = B.st[dside];
  B.fx[side].last = mv->id; if (mv->id != MV_PROTECT && mv->id != MV_DETECT && mv->id != MV_ENDURE) B.fx[side].protn = 0;
  if (B.fx[dside].protect && !M->tgt && mv->id != MV_MIRROR_MOVE && mv->id != MV_NATURE_POWER && mv->id != MV_PSYCH_UP) { bsayf(dside, " protected itself!"); return R_NONE; }
  int vp = M->power ? M->power : var_power(mv->id, a);
  if (M->type == TYPE_FIRE && !M->tgt && (M->power || M->ail) && abil(dside) == AB_FLASH_FIRE && d->hp && d->status != ST_FRZ) { // Gen 3: any FIRE move aimed at it, WILL-O-WISP too
    ab_say(dside, B.ffire[dside] ? " made it ineffective!" : " raised its FIRE power!"); B.ffire[dside] = 1; return R_NONE;
  }
  int fixed = !M->power && !vp ? fixed_damage(mv->id, a, d) : 0; // drawn once: PSYWAVE rolls
  int ohko = !M->power && M->acc == 30 && fixed == d->hp; // ponytail: the four OHKO moves are the only acc-30 fixed hits
  if (M->acc != 255 && !M->tgt) {
    int stage = st[6] + 6 - (B.fx[dside].fore && dst[7] > 6 ? 6 : dst[7]); if (stage < 0) stage = 0; if (stage > 12) stage = 12; // FORESIGHT ignores raised evasion
    int p = ohko ? (d->level > a->level ? 0 : M->acc + a->level - d->level) : M->acc * (abil(side) == AB_COMPOUNDEYES ? 13 : 10) / 10 * ACC[stage][0] / ACC[stage][1];
    if (ri(100) >= p) { sfx(SFX_MISS); bsayf(side, "'s attack missed!"); return R_NONE; }
  }
  move_anim(side, M, vp || fixed);
  { int r = special_move(side, mv->id); if (r >= 0) return r; }
  int did = 0;
  if (vp || fixed) {
    did = 1;
    int phys = is_physical(M->type), ai = phys ? 1 : 3, di = phys ? 2 : 4;
    const Species *ds = &SPECIES[d->id];
    int eff[2], ne = 0, immune = 0;
    for (int k = 0; k < 2; k++) if (ds->types[k] != 255) {
      eff[ne] = TYPE_CHART[M->type][ds->types[k]];
      if (mv->id == MV_STRUGGLE || (!eff[ne] && B.fx[dside].fore && ds->types[k] == TYPE_GHOST && (M->type == TYPE_NORMAL || M->type == TYPE_FIGHTING))) eff[ne] = 10; // STRUGGLE has no type; FORESIGHT lets NORMAL and FIGHTING hit GHOSTs
      immune |= !eff[ne]; ne++;
    }
    int dab = abil(dside);
    if (dab == AB_LEVITATE && M->type == TYPE_GROUND) { ab_say(dside, " makes GROUND moves miss!"); return R_NONE; }
    if (!fixed && ((dab == AB_WATER_ABSORB && M->type == TYPE_WATER) || (dab == AB_VOLT_ABSORB && M->type == TYPE_ELECTRIC))) {
      if (d->hp >= d->stats[0]) { ab_say(dside, " made it useless!"); return R_NONE; }
      int g = d->stats[0] / 4; d->hp = d->hp + g > d->stats[0] ? d->stats[0] : d->hp + g; drain(dside);
      blen = 0; label(dside); cat(" restored HP using its "); cat(ABILITY_NAME[dab]); cat("!"); bsay(buf); return R_NONE;
    }
    if (immune) { blen = 0; cat("It doesn't affect "); label(dside); cat("..."); bsay(buf); return R_NONE; }
    int over = eff[0] * (ne > 1 ? eff[1] : 10); // x100: >100 super effective, <100 not very
    int hits = !M->hitsLo ? 1 : M->hitsLo == M->hitsHi ? M->hitsLo : (const u8[]){ 2, 2, 2, 3, 3, 3, 4, 5 }[ri(8)];
    int dealt = 0, n = 0, aab = abil(side), pw = vp, atk = a->stats[ai], burn = a->status == ST_BRN;
    static const u8 PINCH[] = { AB_OVERGROW, TYPE_GRASS, AB_BLAZE, TYPE_FIRE, AB_TORRENT, TYPE_WATER, AB_SWARM, TYPE_BUG };
    for (int k = 0; k < 8; k += 2) if (aab == PINCH[k] && M->type == PINCH[k + 1] && a->hp * 3 <= a->stats[0]) pw = pw * 15 / 10;
    if (B.ffire[side] && M->type == TYPE_FIRE) pw = pw * 15 / 10;
    int sport = B.fx[0].sport | B.fx[1].sport; if ((sport & 1 && M->type == TYPE_ELECTRIC) || (sport & 2 && M->type == TYPE_FIRE)) pw /= 2; // MUD SPORT, WATER SPORT
    if ((weather == 1 && M->type == TYPE_WATER) || (weather == 2 && M->type == TYPE_FIRE)) pw = pw * 15 / 10; // ponytail: Gen 3 scales the damage, not the power; same 1.5x and 0.5x
    else if ((weather == 1 && M->type == TYPE_FIRE) || (weather == 2 && M->type == TYPE_WATER)) pw /= 2;
    if (a->held && ITEMS[a->held - 1].kind == 6 && ITEMS[a->held - 1].steps == M->type + 1) pw = pw * 11 / 10; // CHARCOAL, MYSTIC WATER, MIRACLE SEED
    if (aab == AB_GUTS && a->status && phys) { atk = atk * 15 / 10; burn = 0; }
    if (dab == AB_THICK_FAT && (M->type == TYPE_FIRE || M->type == TYPE_ICE)) atk /= 2;
    int contact = makes_contact(mv->id); // Gen 3's F_MAKES_CONTACT, SEISMIC TOSS and SUPER FANG too
    for (; n < hits && d->hp && a->hp; n++) {
      int crit = !fixed && dab != AB_SHELL_ARMOR && ri(crit_chance(M->crit + (B.fx[side].focus ? 2 : 0))) == 0;
      int dmg = fixed ? fixed : damage(a->level, pw, M->type, atk, d->stats[di], st[ai], dst[di], crit, burn, mv->id != MV_STRUGGLE && has_type(a, M->type), eff, ne, 100 - ri(16));
      sfx(fixed || over == 100 ? SFX_HIT : over > 100 ? SFX_SUPER : SFX_WEAK);
      if (!crit && !fixed && (phys ? B.fx[dside].refl : B.fx[dside].light) && dmg > 1) dmg /= 2; // REFLECT, LIGHT SCREEN
      int endured = B.fx[dside].endure && dmg >= d->hp; if (endured) dmg = d->hp - 1;
      int before = d->hp; hit(dside, dmg); dealt += before - d->hp; if (phys) B.fx[dside].took += before - d->hp;
      if (crit) bsay("A critical hit!");
      if (endured) bsayf(dside, " endured the hit!");
      if (contact && before > d->hp && a->hp) contact_fx(side);
    }
    if (ohko && !d->hp) bsay("It's a one-hit KO!");
    if (hits > 1) { blen = 0; cat("Hit "); cat_num(n); cat(n > 1 ? " times!" : " time!"); bsay(buf); }
    if (!fixed && over > 100) bsay("It's super effective!");
    if (!fixed && over < 100) bsay("It's not very effective...");
    if (M->drain > 0 && dealt) { int g = dealt * M->drain / 100; a->hp += g < 1 ? 1 : g; if (a->hp > a->stats[0]) a->hp = a->stats[0]; drain(side); blen = 0; label(dside); cat(" had its energy drained!"); bsay(buf); }
    if (M->drain < 0 && dealt && (aab != AB_ROCK_HEAD || mv->id == MV_STRUGGLE)) { int r = dealt * -M->drain / 100; hit(side, r < 1 ? 1 : r); bsayf(side, " is hit with recoil!"); }
    if (!d->hp) { int r = faint(dside); if (r || a->hp) return r; }
    if (!a->hp) return faint(side);
    if (M->flinch && dab != AB_INNER_FOCUS && ri(100) < M->flinch) B.flinch[dside] = 1;
  }
  if (M->cat == CAT_HEAL && M->heal) {
    did = 1;
    if (a->hp >= a->stats[0]) bsayf(side, "'s HP is full!");
    else { a->hp += a->stats[0] * M->heal / 100; if (a->hp > a->stats[0]) a->hp = a->stats[0]; drain(side); bsayf(side, " regained health!"); }
  }
  if (M->ail) {
    did = 1;
    int p = M->power ? (M->ailP ? M->ailP : M->chance) : 100;
    if (d->hp && ri(100) < p) inflict(dside, M->ail, mv->id, !M->power);
  }
  if (M->nsc && (!M->power || ri(100) < (M->statP ? M->statP : M->chance))) {
    did = 1;
    int tgt = M->tgt || (M->cat == CAT_RAISE && M->power) ? side : dside; // GROWL, LEER: the foe. METAL CLAW, SUPERPOWER: the user
    for (int k = 0; k < M->nsc; k++) {
      int i = M->sc[k][0], ch = M->sc[k][1]; s8 *s = B.st[tgt];
      if (tgt != side && ch < 0 && B.fx[tgt].mist) { if (!M->power) side_note(tgt, " is protected by MIST!"); continue; }
      if (tgt != side && ch < 0 && (abil(tgt) == AB_CLEAR_BODY || (abil(tgt) == AB_KEEN_EYE && i == 6))) { if (!M->power) ab_say(tgt, " prevents stat loss!"); continue; }
      blen = 0; label(tgt); cat("'s "); cat(STAT_NAME[i]);
      if ((ch > 0 && s[i] >= 12) || (ch < 0 && s[i] <= 0)) { cat(ch > 0 ? " won't go higher!" : " won't go lower!"); bsay(buf); continue; }
      s[i] += ch; if (s[i] < 0) s[i] = 0; if (s[i] > 12) s[i] = 12;
      cat(ch > 0 ? (ch > 1 ? " sharply rose!" : " rose!") : ch < -1 ? " harshly fell!" : " fell!"); bsay(buf);
    }
  }
  if (!did) bsay(mv->id == 150 ? "But nothing happened!" : "But it failed!"); // ponytail: trapping, two-turn moves come later
  return R_NONE;
}
static int take_turn(int side, Slot *mv) {
  Mon *a = B.m[side]; B.fx[side].acted = 1;
  if (!a->hp) return R_NONE;
  if (a->status == ST_SLP) { int dec = abil(side) == AB_EARLY_BIRD ? 2 : 1; a->sleep = a->sleep > dec ? a->sleep - dec : 0; if (a->sleep) { bsayf(side, " is fast asleep."); return R_NONE; } a->status = 0; hud(side); bsayf(side, " woke up!"); }
  if (a->status == ST_FRZ) { if (ri(5)) { bsayf(side, " is frozen solid!"); return R_NONE; } a->status = 0; hud(side); bsayf(side, " was defrosted!"); }
  if (B.flinch[side]) { B.flinch[side] = 0; bsayf(side, " flinched!"); return R_NONE; }
  if (B.conf[side]) {
    if (--B.conf[side] == 0) bsayf(side, " snapped out of confusion!");
    else {
      bsayf(side, " is confused!");
      if (ri(2) == 0) {
        bsay("It hurt itself in its confusion!");
        hit(side, base_damage(a->level, 40, TYPE_NORMAL, a->stats[1], a->stats[2], B.st[side][1], B.st[side][2], 0, 0));
        return a->hp ? R_NONE : faint(side);
      }
    }
  }
  if (a->status == ST_PAR && ri(4) == 0) { bsayf(side, " is paralyzed! It can't move!"); return R_NONE; }
  return use_move(side, mv);
}
static int end_of_turn(void) {
  for (int side = 0; side < 2; side++) {
    Mon *m = B.m[side]; B.flinch[side] = 0; int fresh = B.fresh[side]; B.fresh[side] = 0;
    if (m->hp && m->status && abil(side) == AB_SHED_SKIN && ri(3) == 0) {
      static const char *const WHAT[] = { "", "paralysis", "sleep", "poison", "poison", "burn", "ice" };
      blen = 0; label(side); cat("'s SHED SKIN cured its "); cat(WHAT[m->status]); cat(" problem!"); m->status = 0; hud(side); bsay(buf);
    }
    if (m->hp && abil(side) == AB_SPEED_BOOST && !fresh && B.st[side][5] < 12) { B.st[side][5]++; ab_say(side, " raised its SPEED!"); }
    if (!m->hp || !(m->status == ST_PSN || m->status == ST_TOX || m->status == ST_BRN)) continue;
    int dmg = m->stats[0] * (m->status == ST_TOX ? ++B.tox[side] : 2) / 16; if (dmg < 1) dmg = 1;
    bsayf(side, m->status == ST_BRN ? " is hurt by its burn!" : " is hurt by poison!");
    hit(side, dmg);
    if (!m->hp) { int r = faint(side); if (r) return r; }
  }
  return moves_end_of_turn();
}


#include "battle_team.inc" // the team screen
// ---------- items (game.js useItem, throwBall)
static const char *const CURED[] = { "", "was cured of paralysis.", "woke up.", "was cured of its poisoning.", "was cured of its poisoning.", "had its burn healed.", "was thawed out." };
static int use_medicine(int key) {
  const Item *it = &ITEMS[key];
  int i = team_screen(1, 1, "Use on which creature?");
  if (i < 0) return 0;
  Mon *m = &S.party[i];
  blen = 0;
  if (it->heal && m->hp && m->hp < m->stats[0]) { int n = m->stats[0] - m->hp; if (n > it->heal && it->heal < 255) n = it->heal; /* 255: MAX POTION, all of it */ m->hp += n; cat(mon_name(m)); cat("'s HP was restored by "); cat_num(n); cat(n > 1 ? " points." : " point."); }
  else if (it->cure && m->hp && (it->cure >> m->status) & 1 && m->status) { int was = m->status; m->status = 0; if (in_battle && m == B.m[ME]) B.tox[ME] = 0; cat(mon_name(m)); cat(" "); cat(CURED[was]); }
  else if (it->revive && !m->hp) { m->hp = m->stats[0] / 2; m->status = 0; cat(mon_name(m)); cat(" recovered from fainting!"); }
  else { (in_battle ? bsay : say)("It won't have any effect."); return 0; }
  BAG(key)--; sfx(SFX_HEAL);
  if (in_battle && m == B.m[ME]) { drain(ME); hud(ME); }
  (in_battle ? bsay : say)(buf);
  return 1;
}
static int throw_ball(int key) {
  const Item *it = &ITEMS[key]; const char *nm = ITEM_NAME[key];
  S.bag[key]--;
  blen = 0; cat(PLAYER_NAME); cat(has("AEIOU", nm[0]) ? " threw an " : " threw a "); cat(nm); cat("!"); bsay(buf);
  copy32((volatile u32 *)OBJ_TILES + T_BALL * 8, SPRITE_TILES + LOOK_BALL * SLOT_TILES * 8, 8 * 8);
  for (int i = 0; i < 16; i++) PAL_OBJ[P_BALL * 16 + i] = SPRITE_PAL[LOOK_BALL * 16 + i];
  if (B.tr) { bsay("The TRAINER blocked the BALL!"); bsay("Don't be a thief!"); return R_NONE; }
  ball_on = 1; sfx(SFX_BALL);
  static const s8 ARC[25] = { 0,9,17,26,34,41,48,54,59,63,67,69,70,69,67,63,59,54,48,41,34,26,17,9,0 }; // round(sin(i/24*pi)*70)
  for (int i = 0; i <= 24; i++) { ball_x = 40 + i * 136 / 24; ball_y = 90 - ARC[i] - i * 52 / 24; next_frame(); }
  B.hide[FOE] = 1; hud(FOE); wait(10);
  for (int i = 0; i < 12; i++) { ball_y += 4; if (ball_y > 48) ball_y = 48; next_frame(); }
  Mon *f = B.m[FOE];
  int st = f->status == ST_SLP || f->status == ST_FRZ ? 1 : f->status && f->status != ST_TOX ? 2 : 0; // Gen 3 bug kept: badly poisoned gets no bonus
  int shakes = expected_shakes(catch_odds(SPECIES[f->id].catchRate, it->ball, f->stats[0], f->hp, st), rnd16);
  for (int i = 0; i < shakes && i < 3; i++) {
    wait(20); sfx(SFX_SHAKE);
    static const s8 DXS[8] = { -2, -3, -2, 0, 2, 3, 2, 0 };
    int x0 = ball_x; for (int k = 0; k < 8; k++) { ball_x = x0 + DXS[k]; wait(2); } ball_x = x0;
  }
  wait(20);
  if (shakes == 4) {
    music(SONG_CAUGHT);
    blen = 0; cat("Gotcha! "); cat(mon_name(f)); cat(" was caught!"); bsay(buf);
    int is_new = !((S.caught[f->id >> 3] >> (f->id & 7)) & 1); mark(S.caught, f->id);
    if (is_new) { blen = 0; cat(mon_name(f)); cat("'s data was added to the DEX."); bsay(buf); }
    if (S.nparty < 6) S.party[S.nparty++] = *f;
    else { if (S.nbox < 30) S.box[S.nbox++] = *f; blen = 0; cat(mon_name(f)); cat(" was sent to the BOX."); bsay(buf); } // ponytail: 30 in the BOX until the PC pages arrive (C7)
    ball_on = 0;
    return R_CAUGHT;
  }
  ball_on = 0; B.hide[FOE] = 0; hud(FOE); music(B.tr ? SONG_TRAINER : SONG_BATTLE);
  static const char *const MISS[] = { "Oh, no! It broke free!", "Aww! It appeared to be caught!", "Aargh! Almost had it!", "Shoot! It was so close, too!" };
  bsay(MISS[shakes]);
  return R_NONE;
}


