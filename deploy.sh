#!/bin/sh
# Copies the latest ROM into web/ and deploys the landing to joshemon.jaybulb.com.
set -e
cd "$(dirname "$0")"
(cd gba && make)
cp gba/build/joshemon.gba web/joshemon.gba
npx wrangler deploy
