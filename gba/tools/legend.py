# python3 tools/legend.py [--png DIR]: draws the late legends (#106 THALASSOR, the sea sleeper; #107 CAELITH, the sky sleeper) as 64x64 sprites, front and back,
# from layered shapes: each part has a 3-step colour ramp, light from the top left, a dark outline round the whole creature
# and a darker seam where a part overlaps the one behind it. Writes them into data/art.c.gz (the art tables, kept gzipped) after the first 105 creatures.
import gzip
import math, re, sys
from PIL import Image, ImageDraw

OUT = (16, 22, 48)  # outline
RAMPS = {
    'body': [(28, 52, 112), (44, 92, 176), (86, 146, 222)],
    'belly': [(170, 160, 120), (232, 222, 184), (232, 222, 184)],
    'fin': [(22, 128, 140), (54, 196, 196), (150, 236, 228)],
    'horn': [(150, 136, 110), (226, 216, 190), (226, 216, 190)],
    'foam': [(150, 200, 230), (226, 242, 250), (226, 242, 250)],
    'plume': [(168, 176, 200), (232, 236, 244), (255, 255, 255)],
    'gold': [(176, 120, 32), (232, 184, 56), (252, 228, 120)],
    'sky': [(48, 112, 184), (96, 168, 232), (168, 216, 248)],
}

def bez(p, t):  # quadratic or cubic bezier point
    if len(p) == 3: (a, b), (c, d), (e, f) = p; u = 1 - t; return (u*u*a + 2*u*t*c + t*t*e, u*u*b + 2*u*t*d + t*t*f)
    (a, b), (c, d), (e, f), (g, h) = p; u = 1 - t
    return (u**3*a + 3*u*u*t*c + 3*u*t*t*e + t**3*g, u**3*b + 3*u*u*t*d + 3*u*t*t*f + t**3*h)

class Sprite:
    def __init__(s): s.layers = []  # (ramp, mask)
    def mask(s): return Image.new('1', (64, 64), 0)
    def add(s, ramp, draw):
        m = s.mask(); draw(ImageDraw.Draw(m)); s.layers.append((ramp, m)); return m
    def tube(s, ramp, pts, r0, r1, n=40):  # a thick curve that tapers from r0 to r1
        def d(g):
            for i in range(n + 1):
                t = i / n; x, y = bez(pts, t); r = r0 + (r1 - r0) * t
                g.ellipse((x - r, y - r, x + r, y + r), fill=1)
        return s.add(ramp, d)
    def poly(s, ramp, pts): return s.add(ramp, lambda g: g.polygon(pts, fill=1))
    def oval(s, ramp, box): return s.add(ramp, lambda g: g.ellipse(box, fill=1))
    def render(s, glow=()):
        px = [[None] * 64 for _ in range(64)]; who = [[-1] * 64 for _ in range(64)]
        for k, (ramp, m) in enumerate(s.layers):
            for y in range(64):
                for x in range(64):
                    if m.getpixel((x, y)): who[y][x] = k
        def inside(k, x, y): return 0 <= x < 64 and 0 <= y < 64 and s.layers[k][1].getpixel((x, y))
        for y in range(64):
            for x in range(64):
                k = who[y][x]
                if k < 0: continue
                ramp = RAMPS[s.layers[k][0]]
                if not inside(k, x + 2, y + 2) or not inside(k, x + 1, y + 1): c = ramp[0]   # shade on the bottom right
                elif not inside(k, x - 1, y - 1) or not inside(k, x - 2, y - 1): c = ramp[2]  # light on the top left
                else: c = ramp[1]
                if any(0 <= x + dx < 64 and 0 <= y + dy < 64 and who[y + dy][x + dx] >= 0 and who[y + dy][x + dx] < k and not inside(k, x + dx, y + dy)
                       for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))) and s.layers[k][0] != 'foam':
                    c = ramp[0] if c != ramp[0] else OUT  # a seam where this part sits over the one behind
                px[y][x] = c
        for (x, y, c) in glow: px[y][x] = c
        img = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
        for y in range(64):
            for x in range(64):
                if px[y][x]: img.putpixel((x, y), px[y][x] + (255,))
                elif any(0 <= x + dx < 64 and 0 <= y + dy < 64 and px[y + dy][x + dx] for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    img.putpixel((x, y), OUT + (255,))
        return img

EYE, GLOW, WHITE = (250, 250, 240), (120, 250, 240), (250, 250, 240)
GOLD_LINE = (252, 228, 120)

def thalassor_front():
    s = Sprite()
    s.poly('fin', [(46, 38), (63, 14), (62, 26), (64, 30), (58, 42)])               # far pectoral fin, raised behind like a wing
    for (x, y) in ((40, 20), (47, 25), (53, 32), (57, 40)):                           # the crest down its back, only the tips show
        s.poly('fin', [(x - 4, y + 3), (x + 7, y - 3), (x + 2, y + 6)])
    s.poly('horn', [(26, 12), (40, 3), (36, 10), (30, 16)])                           # swept-back horn
    s.poly('fin', [(14, 12), (19, 3), (23, 10), (28, 5), (30, 14)])                   # crown fin
    s.tube('body', [(46, 60), (62, 44), (46, 26), (24, 20)], 11, 6)                   # body and neck in one S
    s.tube('belly', [(38, 60), (52, 46), (40, 32), (22, 24)], 6, 3)                   # pale throat and belly
    s.oval('body', (11, 11, 31, 26))                                                  # head
    s.poly('body', [(2, 20), (14, 15), (16, 25), (4, 24)])                            # long snout
    s.poly('fin', [(34, 40), (22, 44), (16, 53), (28, 50), (38, 47)])                 # near flipper
    s.oval('foam', (4, 55, 30, 64)); s.oval('foam', (24, 57, 64, 64))                 # the wave it rides
    glow = [(17, 16, OUT), (18, 16, GLOW), (19, 16, EYE), (17, 17, OUT), (18, 17, OUT), (19, 17, GLOW)]       # a narrow glowing eye
    glow += [(x, 23, OUT) for x in range(4, 12)] + [(12, 22, OUT), (13, 22, OUT)]                              # the mouth line
    glow += [(x, y, GLOW) for (x, y) in ((50, 54), (53, 50), (55, 46), (55, 42), (54, 38))]                    # glowing marks on the flank
    return s.render(glow)

def thalassor_back():  # seen from behind and a little left: the crest is on top now, the face turned away
    s = Sprite()
    s.poly('fin', [(18, 40), (0, 16), (2, 30), (0, 34), (8, 46)])                    # far fin, raised
    s.poly('horn', [(40, 12), (26, 0), (30, 9), (36, 16)])                            # horn
    s.tube('body', [(20, 70), (-2, 46), (18, 24), (42, 18)], 14, 8)                   # body and neck, mirrored S, bigger
    s.oval('body', (34, 6, 58, 26))                                                   # back of the head
    s.oval('body', (50, 13, 64, 24))                                                  # the snout, turned away
    for (x, y) in ((24, 18), (15, 24), (8, 32), (5, 41), (6, 50)):                    # crest along the near side of the neck
        s.poly('fin', [(x + 4, y + 4), (x - 6, y - 3), (x - 1, y + 7)])
    s.poly('fin', [(38, 6), (42, 0), (46, 6), (52, 2), (52, 10)])                     # crown fin
    s.poly('fin', [(26, 46), (46, 44), (62, 54), (44, 58), (28, 56)])                 # near flipper, swept back
    glow = [(x, y, GLOW) for (x, y) in ((14, 48), (13, 44), (14, 40), (17, 36), (21, 33))]
    return s.render(glow)

def fan(s, root, tips, r0=3, tip_sky=2):  # a wing: one tapered feather per tip, the outermost in sky blue
    for k, (x, y) in enumerate(tips):
        mx, my = (root[0] + x) / 2, (root[1] + y) / 2 - 3
        s.tube('sky' if k >= len(tips) - tip_sky else 'plume', [root, (mx, my), (x, y)], r0, 1)

def caelith_front():  # #107, the sky sleeper: a great white bird, gold crest, sky-blue wingtips and streamers
    s = Sprite()
    fan(s, (36, 28), [(48, 34), (54, 26), (58, 16), (60, 6), (56, 0)])                        # far wing, raised behind
    for (x, y) in ((58, 63), (50, 64), (42, 63)):                                              # tail streamers
        s.tube('sky', [(38, 46), (44, 54), (x - 6, y - 4), (x, y)], 3, 1)
    for (x, y) in ((30, 0), (34, 4), (36, 9)):                                                 # crest plumes, swept back
        s.tube('gold', [(20, 10), (x - 6, y + 2), (x, y)], 2, 1)
    s.tube('plume', [(36, 48), (36, 36), (26, 24)], 9, 8)                                       # body, a teardrop
    s.tube('plume', [(26, 26), (22, 18), (18, 14)], 6, 5)                                       # neck
    s.oval('plume', (10, 6, 26, 21))                                                           # head
    s.poly('gold', [(2, 15), (10, 11), (12, 17), (6, 19)])                                     # hooked beak
    s.tube('gold', [(22, 26), (28, 30), (36, 30)], 1, 1)                                       # gold collar
    s.poly('gold', [(30, 54), (28, 60), (32, 58), (35, 62), (36, 54)])                          # talons
    fan(s, (32, 32), [(20, 44), (12, 42), (5, 36), (1, 28), (2, 20)], 4)                       # near wing, spread low and out
    glow = [(15, 12, OUT), (16, 12, GLOW), (15, 13, OUT), (16, 13, OUT)]                       # eye
    return s.render(glow)

def caelith_back():
    s = Sprite()
    for (x, y) in ((22, 64), (32, 66), (42, 64)):
        s.tube('sky', [(32, 50), (32, 56), (x, y)], 3, 1)
    fan(s, (26, 30), [(16, 44), (8, 38), (2, 28), (0, 16), (4, 4)], 4)                          # left wing
    fan(s, (38, 30), [(48, 44), (56, 38), (62, 28), (64, 16), (60, 4)], 4)                      # right wing
    for (x, y) in ((26, 0), (32, -2), (38, 0)):
        s.tube('gold', [(32, 12), ((32 + x) / 2, 6), (x, y)], 2, 1)
    s.tube('plume', [(32, 56), (33, 42), (32, 26)], 10, 6)                                      # back, tapering to the neck
    s.oval('plume', (25, 8, 39, 24))                                                           # back of the head
    glow = [(32, y, OUT) for y in range(32, 54, 3)]                                            # spine feathers
    return s.render(glow)

def to_gba(img):  # 64 4bpp tiles, 16 BGR555 colours (0 clear), and the lowest inked row
    cols = sorted({img.getpixel((x, y))[:3] for y in range(64) for x in range(64) if img.getpixel((x, y))[3]})
    assert len(cols) <= 15, len(cols)
    idx = {c: i + 1 for i, c in enumerate(cols)}
    tiles = []
    for t in range(64):
        tx, ty = t % 8, t // 8
        for r in range(8):
            w = 0
            for x in range(8):
                p = img.getpixel((tx * 8 + x, ty * 8 + r)); w |= (idx[p[:3]] if p[3] else 0) << (4 * x)
            tiles.append(w)
    pal = [0] + [(r >> 3) | (g >> 3) << 5 | (b >> 3) << 10 for (r, g, b) in cols] + [0] * (15 - len(cols))
    feet = max(y for y in range(64) for x in range(64) if img.getpixel((x, y))[3])
    return tiles, pal, feet

def patch(art, name, n_old, items, fmt):  # keep the first n_old entries, then ours
    i = art.index(name); j = art.index('{', i); k = art.index('};', j)
    vals = re.findall(r'0x[0-9a-fA-F]+|\d+', art[j + 1:k])[:n_old] + [fmt(v) for v in items]
    head = re.sub(r'\[\d+\]', '[%d]' % len(vals), art[i:j], count=1)
    body = ',\n'.join(','.join(vals[a:a + 16]) for a in range(0, len(vals), 16))
    return art[:i] + head + '{\n' + body + '\n' + art[k:]

if __name__ == '__main__':
    mons = [(thalassor_front(), thalassor_back()), (caelith_front(), caelith_back())]
    if '--png' in sys.argv:
        d = sys.argv[sys.argv.index('--png') + 1]; sheet = Image.new('RGBA', (128 * len(mons), 64), (255, 0, 255, 255))
        for i, (f, b) in enumerate(mons): sheet.paste(f, (128 * i, 0), f); sheet.paste(b, (128 * i + 64, 0), b)
        sheet.resize((sheet.width * 4, 256), Image.NEAREST).save(d + '/legends.png'); sys.exit()
    tiles, pals, feet = [], [], []
    for f, b in mons:
        for img in (f, b): t, p, ft = to_gba(img); tiles += t; pals += p
        feet.append(to_gba(f)[2])
    art = gzip.open('data/art.c.gz', 'rt').read()
    art = patch(art, 'ART_TILES[', 105 * 2 * 512, tiles, hex)
    art = patch(art, 'ART_PAL[', 105 * 2 * 16, pals, str)
    art = patch(art, 'MON_FEET[', 105, feet, str)
    gzip.open('data/art.c.gz', 'wt').write(art)
