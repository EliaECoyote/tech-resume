#!/bin/bash
set -e

[ -f ".bsb.lock" ] && rm .bsb.lock

yarn clean
yarn bsb -make-world

yarn concurrently \
  --names bsb,gatsby \
  --raw \
  --prefix-colors blue.bold,yellow.bold \
  "yarn nodemon --exec \"bsb || exit 1\"" "yarn gatsby develop -H 0.0.0.0"
