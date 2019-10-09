#!/bin/bash
set -e

echo $PWD;

INDEX_FILE="/lib/js/src/index.bs.js"

[ ! -d "$INDEX_FILE" ] && yarn build-bsb

yarn concurrently  \
  --names bsb,node  \
  --prefix-colors blue.bold,yellow.bold  \
  --kill-others  \
  --kill-others-on-fail  \
    "yarn watch-bsb" "yarn watch-node" \
