#!/usr/bin/env python3
# tools/gen3_moves.py: compares our move table (data/dex.c, data/parts/dex_2.inc) with pret/pokeruby's
# src/data/battle_moves.c and move names, and prints the MOVE_CONTACT bits for data/parts/dex_4.inc.
# Needs gh (fetches pokeruby). Known, kept on purpose: SPIT UP (needs STOCKPILE), variable-power moves stored as power 0 (LOW KICK keeps 50,
# our creatures have no weights), and self or never-miss moves stored as accuracy 255.
import os, re, subprocess
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
def fetch(path):
    return subprocess.run(['gh', 'api', 'repos/pret/pokeruby/contents/' + path, '-H', 'Accept: application/vnd.github.raw'], capture_output=True, text=True, check=True).stdout
G=fetch('src/data/battle_moves.c')
TYPES=["NORMAL","FIGHTING","FLYING","POISON","GROUND","ROCK","BUG","GHOST","STEEL","FIRE","WATER","GRASS","ELECTRIC","PSYCHIC","ICE","DRAGON","DARK"]
gm=[]
for m in re.finditer(r'\[MOVE_(\w+)\] = \{(.*?)\n    \}',G,re.S):
    b=m.group(2); g=lambda k: re.search(r'\.'+k+r' = ([^,\n]+)',b).group(1)
    t=g('type').replace('TYPE_',''); t='GHOST' if t=='GHOST' else t
    gm.append(dict(name=m.group(1),eff=g('effect'),power=int(g('power')),type=t,acc=int(g('accuracy')),pp=int(g('pp')),chance=int(g('secondaryEffectChance')),pri=int(g('priority')),contact='F_MAKES_CONTACT' in g('flags') or 'F_MAKES_CONTACT' in b,target=g('target')))
names=re.findall(r'\[MOVE_\w+\] = _\("([^"]*)"\)',fetch('src/data/text/move_names_en.h'))
D=''.join(open(os.path.join(ROOT,f)).read() for f in ['data/dex.c','data/parts/dex_2.inc'])
i=D.index('const Move MOVES')
ours=[]
for ln in D[i:].split('\n')[2:]:
    if ln.startswith('{0}'): ours.append(None); continue
    m=re.match(r'\{"([^"]+)",(-?\d+),(\d+),(\d+),(\d+),(-?\d+),(\d+),(\d+),(\d+),(-?\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),',ln)
    if m: ours.append(m.groups())
    elif ln.startswith('}'): break

bits = [0] * 45
for k, g in enumerate(gm):
    if g['contact']: bits[k >> 3] |= 1 << (k & 7)
print('MOVE_CONTACT', ','.join(map(str, bits)))
for k, o in enumerate(ours, 1):
    if o is None or k >= len(gm): continue
    g = gm[k]; ty = TYPES[int(o[1])] if 0 <= int(o[1]) < 17 else 'MYSTERY'; acc = int(o[3]); acc = 0 if acc == 255 else acc; d = []
    if ty != g['type']: d.append(f"type {ty}->{g['type']}")
    if int(o[2]) != g['power'] and g['power'] != 1: d.append(f"power {o[2]}->{g['power']}")
    if acc != g['acc'] and int(o[3]) != 255: d.append(f"acc {o[3]}->{g['acc']}")
    if int(o[4]) != g['pp']: d.append(f"pp {o[4]}->{g['pp']}")
    if int(o[5]) != g['pri']: d.append(f"pri {o[5]}->{g['pri']}")
    if names[k] != o[0]: d.append(f"name {o[0]}->{names[k]}")
    if d: print(k, o[0], '|', ', '.join(d), '|', g['eff'])
