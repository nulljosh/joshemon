# python3 tools/samples.py > build/samples.c : the sampled instruments for the sample channel (Direct Sound A).
# Every sample is synthesized here from plain math, none are taken from anywhere. Each instrument is 8-bit signed at
# 13379 Hz, with PER samples to one cycle of its pitch (the player steps through it at f / 13379 * PER per output
# sample), a loop start (-1 plays once) and an envelope: attack frames, decay per frame, sustain level, release per frame.
import math, random
random.seed(7)
RATE = 13379
def lowpass(x, a):
    y = 0; out = []
    for _ in range(2):  # twice round so the loop point settles
        out = []
        for v in x: y += a * (v - y); out.append(y)
    return out
def norm(x, peak=120):
    m = max(abs(v) for v in x) or 1
    return [int(round(v * peak / m)) for v in x]
def saw(n, cycles, phase=0): return [((i * cycles / n + phase) % 1) * 2 - 1 for i in range(n)]

insts = []
# 0 BRASS: a bright saw rounded off, the GBA games' trumpet-like lead
insts.append(('brass', norm(lowpass(saw(64, 1), .55)), 64, 0, (2, 3, 200, 10)))
# 1 STRINGS: three saws a little apart (63, 64, 65 cycles in 4096 samples loop cleanly), soft, a slow bow
s = [a + b + c for a, b, c in zip(saw(4096, 63), saw(4096, 64, .3), saw(4096, 65, .6))]
insts.append(('strings', norm(lowpass(s, .35)), 64, 0, (8, 1, 210, 6)))
# 2 PIANO: a plucked string (Karplus-Strong), one period of 64, rings out once
buf = [random.uniform(-1, 1) for _ in range(64)]; out = []
for i in range(9000):
    v = buf[i % 64]; out.append(v); buf[i % 64] = .5 * (v + buf[(i + 1) % 64]) * .998
insts.append(('piano', norm(out), 64, -1, (0, 2, 160, 12)))
# 3 TIMPANI: a deep drum, a falling sine with a soft thump, once
out = []; ph = 0
for i in range(5000):
    t = i / RATE; f = RATE / 64 * (1 + .25 * math.exp(-t * 30)); ph += f / RATE
    out.append(math.sin(2 * math.pi * ph) * math.exp(-t * 5) + random.uniform(-1, 1) * .5 * math.exp(-t * 60))
insts.append(('timpani', norm(out), 64, -1, (0, 0, 255, 8)))
# 4 FLUTE: a sine with a little second harmonic and breath, looped
s = [math.sin(2 * math.pi * i / 64) + .25 * math.sin(4 * math.pi * i / 64) for i in range(64)]
insts.append(('flute', norm(s), 64, 0, (5, 2, 200, 8)))
# 5 BASS: a plucked electric bass, a rounded square that dies down, looped tail
s = lowpass([1 if i < 32 else -1 for i in range(64)], .3)
insts.append(('bass', norm(s), 64, 0, (0, 4, 150, 14)))

# 6 KICK, 7 SNARE, 8 CRASH: the sampled drum kit (one-shots at their own rate)
out = []; ph = 0
for i in range(2400):
    t = i / RATE; f = 45 + 110 * math.exp(-t * 35); ph += f / RATE
    out.append(math.sin(2 * math.pi * ph) * math.exp(-t * 14) + (random.uniform(-1, 1) * .4 if i < 40 else 0))
insts.append(('kick', norm(out, 126), 0, -1, (0, 0, 255, 255)))
n = [random.uniform(-1, 1) for _ in range(2600)]; lo = lowpass(n, .5); lo2 = lowpass(n, .08)
out = [((a - b) * 1.6 + .5 * math.sin(2 * math.pi * 190 * i / RATE) * math.exp(-i / RATE * 30)) * math.exp(-i / RATE * 16) for i, (a, b) in enumerate(zip(lo, lo2))]
insts.append(('snare', norm(out, 110), 0, -1, (0, 0, 255, 255)))
n = [random.uniform(-1, 1) for _ in range(14000)]; lo = lowpass(n, .3)
out = [(a - b) * math.exp(-i / RATE * 2.6) for i, (a, b) in enumerate(zip(n, lo))]
insts.append(('crash', norm(out, 90), 0, -1, (0, 0, 255, 255)))

print('// written by tools/samples.py, do not edit')
print('typedef struct { const signed char *d; int len, loop, per; unsigned char att, dec, sus, rel; } Inst;')
for n, d, per, loop, env in insts:
    print('static const signed char S_%s[%d] = {%s};' % (n, len(d), ','.join(map(str, d))))
print('const Inst INSTS[%d] = {%s};' % (len(insts), ','.join('{S_%s,%d,%d,%d,%d,%d,%d,%d}' % ((n, len(d), loop, per) + env) for n, d, per, loop, env in insts)))
# PER 0 marks the drum kit: the note picks KICK (36), SNARE (38) or CRASH (49), each played at its own rate
# the step through a sample for each midi note, 16.16, for PER = 1 (multiply by the instrument's PER)
print('const unsigned short DS_STEP[128] = {%s};' % ','.join(str(int(440 * 2 ** ((m - 69) / 12) / RATE * 65536)) for m in range(128)))
