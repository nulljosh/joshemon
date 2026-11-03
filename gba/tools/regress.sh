#!/bin/sh
# tools/regress.sh: builds the ROMs and plays every regression script headless. Prints "script fails" per line; all 0 is a pass.
# Screenshots land in build/reg/<script>. tools/regress.sh 1 or 2 runs one half (each well under 5 minutes); no argument runs both.
# Every script gets a hard 120 s cap.
cd "$(dirname "$0")/.." || exit 1
make build/joshemon.gba build/qa_field.gba build/qa_dip.gba build/gbarun >/dev/null 2>&1 || { echo "build failed"; exit 1; }
run() { # a script that dies or hits the cap mid-way reports INCOMPLETE, not a pass
  mkdir -p "build/reg/$2"; o=$(timeout 120 build/gbarun "build/$1" "scripts/$2.run" "build/reg/$2" 2>&1 | grep -E "^(ok|FAIL)")
  want=$(grep -c "^expect" "scripts/$2.run"); got=$(printf '%s\n' "$o" | grep -c -E "^(ok|FAIL)")
  if [ "$got" -lt "$want" ]; then echo "$2 INCOMPLETE $got/$want"; else printf '%s %s\n' "$2" "$(printf '%s\n' "$o" | grep -c FAIL)"; fi; }
H=${1:-12}
case $H in *1*) for s in c6d c6h c7a c7b c7c c7g c8a c8b c8c c9a c10a c10b; do run qa_field.gba $s; done;; esac
case $H in *2*) for s in c11a g1 g2 g3 g4 g5 cv1 cv2 de1; do run qa_field.gba $s; done
  for s in c11b c12a c12b co1; do run qa_dip.gba $s; done; run joshemon.gba co2;; esac
