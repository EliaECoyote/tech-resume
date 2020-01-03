#!/bin/bash
set -e

[ -f ".bsb.lock" ] && rm .bsb.lock
yarn clean
yarn "yarn bsb -make-world"

yarn concurrently \
  --names bsb,node \
  --prefix-colors blue.bold,yellow.bold \
  --kill-others \
  --kill-others-on-fail \
  "yarn bsb -w" "yarn nodemon --inspect=4001 lib/js/src/index.bs.js"
