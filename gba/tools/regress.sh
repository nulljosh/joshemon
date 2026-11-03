#!/bin/sh
# tools/regress.sh: builds the ROMs and plays every regression script headless. Prints "script fails" per line; all 0 is a pass.
# Screenshots land in build/reg/<script>.
cd "$(dirname "$0")/.." || exit 1
make build/joshemon.gba build/qa_field.gba build/qa_dip.gba build/gbarun >/dev/null 2>&1 || { echo "build failed"; exit 1; }
run() { mkdir -p "build/reg/$2"; printf '%s %s\n' "$2" "$(build/gbarun "build/$1" "scripts/$2.run" "build/reg/$2" 2>&1 | grep -c FAIL)"; }
for s in c6d c6h c7a c7b c7c c7g c8a c8b c8c c9a c10a c10b c11a g1 g2 g3 g4 g5 cv1 cv2; do run qa_field.gba $s; done
for s in c11b c12a c12b co1; do run qa_dip.gba $s; done
run joshemon.gba co2
