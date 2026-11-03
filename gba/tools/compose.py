# python3 tools/compose.py : composes every song in music/songs*.py (in file order) and writes them, with the SONGS table,
# to build/songs.inc, which data/world.c includes. make runs it. A spec sets NAME, BPM, LOOP, STYLE, CHORDS ("C:4 Am:4", beats per chord)
# and MELODY ("e5:1 g5:.5 r:1"), optionally LEAD (duty, decay, vib, vol), HARMONY ('arp8', 'arp16', 'echo', 'pad',
# 'off') and DRUMS (a list of 4-beat bars). It writes the lead, a square-2 part from the chords or an echo of the lead,
# a triangle bass in the style, and drums. Notes are kept in quarter beats.
import os, re, runpy
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
NOTE = {'c': 0, 'd': 2, 'e': 4, 'f': 5, 'g': 7, 'a': 9, 'b': 11}
DRUM = {'k': 36, 's': 38, 'h': 42, 'o': 46, 'c': 49}
QUAL = {'': [0, 4, 7], 'm': [0, 3, 7], '7': [0, 4, 7, 10], 'm7': [0, 3, 7, 10], 'maj7': [0, 4, 7, 11], 'dim': [0, 3, 6], 'sus': [0, 5, 7], 'aug': [0, 4, 8]}
NAMES = ['c', 'c#', 'd', 'd#', 'e', 'f', 'f#', 'g', 'g#', 'a', 'a#', 'b']
def name(m): return NAMES[m % 12] + str(m // 12 - 1)
def midi(n): return 0 if n == 'r' else DRUM[n] if n in DRUM else 12 * (int(n[-1]) + 1) + NOTE[n[0]] + (1 if '#' in n else 0)
def chord(c):
    r = c[0].lower(); i = 1
    if len(c) > 1 and c[1] in '#b': r += c[1]; i = 2
    root = NOTE[r[0]] + (1 if r[1:] == '#' else -1 if r[1:] == 'b' else 0)  # r[1:]: a plain B is not a flat
    return root % 12, QUAL[c[i:]]
def parse(seq): return [(n.split(':')[0], float(n.split(':')[1])) for n in seq.split()]
DR = {'town': ['h:.5 h:.5 s:.5 h:.5 h:.5 h:.5 s:.5 h:.5'], 'route': ['k:.5 h:.5 s:.5 h:.5 k:.5 k:.5 s:.5 h:.5'],
      'battle': ['k:.5 h:.25 h:.25 s:.5 h:.5 k:.5 k:.5 s:.5 h:.25 h:.25'], 'march': ['k:1 s:.5 s:.5 k:1 s:1'],
      'cave': ['k:2 h:2'], 'ballad': ['h:1 h:1 s:1 h:1'], 'fanfare': None}
INST = ['brass', 'strings', 'piano', 'timpani', 'flute', 'bass', 'kick', 'snare', 'crash']  # the order in tools/samples.py
FILL = 'k:.5 s:.5 s:.5 s:.25 s:.25 s:.5 s:.5 o:1'  # every eighth bar, into the next phrase

def compose(spec):
    STYLE = spec.get('STYLE', 'town')
    chords = [(chord(c.split(':')[0]), float(c.split(':')[1])) for c in spec['CHORDS'].split()]
    total = sum(b for _, b in chords)
    mel = parse(spec['MELODY']); ml = sum(b for _, b in mel)
    assert abs(ml - total) < 1e-6, (spec['NAME'], 'melody beats', ml, 'chord beats', total)
    t = 0; edges = []
    for c, b in chords: edges.append((t, t + b, c)); t += b
    t = 0
    for n, b in mel:
        if n != 'r' and ((t % 2 == 0 and b >= 1) or b >= 1.5):  # passing eighths are fine
            root, q = next(c for s0, e0, c in edges if s0 <= t < e0)
            if (midi(n) - root) % 12 not in q and not spec.get('COLOR'): print('  %s: %s at beat %g is outside its chord' % (spec['NAME'], n, t))
        t += b
    lead = dict(duty=1, decay=0, vib=3, vol=.09); lead.update(spec.get('LEAD', {}))
    parts = [('square', lead['vol'], lead, mel)]
    if 'LEAD_INST' in spec:  # the lead doubled (or carried) by a sampled instrument: brass, strings, flute...
        name_, vol_ = spec['LEAD_INST']; parts.append(('sample', vol_, dict(duty=INST.index(name_), vib=lead['vib']), mel))
    if 'PAD_INST' in spec:  # held chords on sampled voices, one voice per chord tone
        name_, vol_ = spec['PAD_INST']; poct = spec.get('PAD_OCT', 4)
        for k in range(3):
            parts.append(('sample', vol_, dict(duty=INST.index(name_)), [(name(12 * (poct + 1) + root + q[k]), b) for (root, q), b in chords]))
    H = spec.get('HARMONY', {'town': 'arp8', 'route': 'arp8', 'battle': 'arp16', 'cave': 'pad', 'fanfare': 'pad', 'ballad': 'arp8', 'march': 'arp8'}[STYLE])
    oct2 = spec.get('HARM_OCT', 4)
    if H == 'echo':  # the lead again half a beat later, quieter, like the GBA's echo voices
        out = []; left = total
        for n, b in [('r', spec.get('ECHO', .5))] + mel:
            if left <= 0: break
            b = min(b, left); out.append((n, b)); left -= b
        parts.append(('square', lead['vol'] * .45, dict(duty=2, decay=0, vib=0), out))
    elif H != 'off':
        out = []
        for (root, q), b in chords:
            tones = [12 * (oct2 + 1) + root + i for i in q]
            if H == 'pad': out.append((name(tones[1]), b)); continue
            step = .5 if H == 'arp8' else .25
            pat = tones + [tones[1] + 12] if H == 'arp16' else [tones[0], tones[2], tones[1], tones[2]]
            t = 0; k = 0
            while t < b - 1e-9: out.append((name(pat[k % len(pat)]), step)); t += step; k += 1
        parts.append(('square', spec.get('HARM_VOL', .04), dict(duty=2 if H == 'pad' else 0, decay=0 if H == 'pad' else 2, vib=0), out))
    bass = []; boct = spec.get('BASS_OCT', 2)
    for (root, q), b in chords:
        r = 12 * (boct + 1) + root; f = r + 7; o = r + 12
        pat = {'battle': [(r, .5), (o, .5)], 'route': [(r, 1), (f, 1), (o, 1), (f, 1)], 'cave': [(r, 2), (f, 2)],
               'ballad': [(r, 2), (f, 2)], 'march': [(r, 1), (f, 1)], 'fanfare': [(r, b)]}.get(STYLE, [(r, 1), (r + q[1], 1), (f, 1), (r + q[1], 1)])
        t = 0; k = 0
        while t < b - 1e-9:
            m, d = pat[k % len(pat)]; d = min(d, b - t); bass.append((name(m), d)); t += d; k += 1
    parts.append(('triangle', spec.get('BASS_VOL', .18), {}, bass))
    if 'BASS_INST' in spec:  # the bass line doubled on a sampled voice
        name_, vol_ = spec['BASS_INST']; parts.append(('sample', vol_, dict(duty=INST.index(name_)), bass))
    drums = spec.get('DRUMS', DR[STYLE])
    if drums:
        out = []; bars = int(total // 4); rest = total - bars * 4
        for i in range(bars): out += parse(FILL if i % 8 == 7 and STYLE != 'cave' else drums[i % len(drums)])
        if rest: out.append(('r', rest))
        kit = spec.get('KIT', .6)  # kick, snare and crash on the sampled kit, hats stay on the noise channel
        if kit:
            ks = [(n if n in 'ksc' else 'r', b) for n, b in out]
            for i in range(0, bars, 8):  # a crash on the first beat of every phrase
                t = 0
                for j, (n, b) in enumerate(ks):
                    if abs(t - i * 4) < 1e-9: ks[j] = ('c', b); break
                    t += b
            parts.append(('sample', kit, dict(duty=INST.index('kick')), ks))
            out = [(n if n in 'ho' else 'r', b) for n, b in out]
        parts.append(('noise', spec.get('DRUM_VOL', .05), {}, out))
    N = spec['NAME']; lines = []; chs = []
    for i, (wave, vol, opt, ns) in enumerate(parts):
        assert abs(sum(b for _, b in ns) - total) < 1e-6, (N, wave, 'part length')
        for _, b in ns: assert (b * 4) % 1 == 0 and b * 4 < 256, (N, b)
        lines.append('static const unsigned char SN_%s_%d[] = {%s};' % (N, i, ','.join('%d,%d' % (midi(n), int(b * 4)) for n, b in ns)))
        chs.append('{%d,%d,%d,SN_%s_%d,%d,%d,%d}' % (['square', 'triangle', 'noise', 'sample'].index(wave), int(vol * 1000), len(ns), N, i, opt.get('duty', 2), opt.get('decay', 0), opt.get('vib', 0)))
    lines.append('static const SongCh SC_%s[] = {%s};' % (N, ','.join(chs)))
    return '\n'.join(lines), '{%d,%d,%d,SC_%s}' % (spec['BPM'], spec.get('LOOP', 1), len(parts), N)

import glob
songs = sum((runpy.run_path(f)['SONGS'] for f in sorted(glob.glob(os.path.join(ROOT, 'music', 'songs*.py')))), [])
done = [compose(s) for s in songs]
block = '// songs: written by tools/compose.py from music/songs.py, do not edit by hand\n' + '\n'.join(c for c, _ in done) + '\n// songs end\n'
table = 'const Song SONGS[%d] = {%s};' % (len(done), ','.join(e for _, e in done))
path = os.path.join(ROOT, 'build', 'songs.inc'); os.makedirs(os.path.dirname(path), exist_ok=True)
open(path, 'w').write(block + table + '\n')
print('%d songs' % len(done))
