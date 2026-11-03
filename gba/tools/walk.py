# python3 tools/walk.py "u3 r2 A60 shot:name w30 ..." : a gbarun key script from moves. u/d/l/r + tiles walks
# (a hold of 17n-4 frames is exactly n steps: 16 frames a step plus one to start the next, whether or not you were facing that way), A<n> mashes A n times,
# B<n> mashes B, w<n> waits, shot:name screenshots, S presses START, raw lines in [] pass through.
# fight: mash A through a battle, then B through any text after it (needs the QA address: --qa ELF).
# talk: press A once, then B until the text is gone. at:MAP,X,Y checks where you stand.
import sys, subprocess
out = []
args = sys.argv[1:]
if args[0] == '--qa':
    sym = subprocess.run(['arm-none-eabi-nm', args[1]], capture_output=True, text=True).stdout
    addr = next(l.split()[0] for l in sym.splitlines() if l.endswith(' QA'))
    out.append('qa ' + addr); args = args[2:]
for tok in args[0].split():
    if tok == 'fight': out += ['mash B battle 1', 'mash A battle', 'wait 50', 'mash B talking']; continue
    if tok == 'talk': out += ['press A 2', 'wait 10', 'mash B talking']; continue
    if tok.startswith('at:'): m, x, y = tok[3:].split(','); out += ['expect map ' + m, 'expect x ' + x, 'expect y ' + y]; continue
    if tok[0] in 'udlr' and tok[1:].isdigit():
        n = int(tok[1:]); out += ['press %s %d' % ({'u': 'UP', 'd': 'DOWN', 'l': 'LEFT', 'r': 'RIGHT'}[tok[0]], 17 * n - 4), 'wait 24']
    elif tok[0] in 'AB' and tok[1:].isdigit(): out += ['press %s 2\nwait 22' % tok[0]] * int(tok[1:])
    elif tok[0] == 'w': out.append('wait ' + tok[1:])
    elif tok.startswith('shot:'): out.append('shot ' + tok[5:])
    elif tok == 'S': out += ['press START 2', 'wait 20']
    elif tok.startswith('['): out.append(tok[1:-1].replace('_', ' '))
    else: sys.exit('bad move ' + tok)
print('\n'.join(out))
