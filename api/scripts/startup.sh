#!/bin/bash
set -e

[ -f ".bsb.lock" ] && rm .bsb.lock
yarn clean
yarn build-bsb

yarn concurrently \
  --names bsb,node \
  --prefix-colors blue.bold,yellow.bold \
  --kill-others \
  --kill-others-on-fail \
  "yarn bsb -w" "yarn nodemon --inspect=3001 lib/js/src/index.bs.js"
