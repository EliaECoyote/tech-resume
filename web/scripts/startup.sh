#!/bin/bash
set -e

echo $PWD;

INDEX_FILE="/src/pages/index.bs.js"

[ ! -d "$INDEX_FILE" ] && yarn build-bsb

yarn concurrently  \
  --names bsb,gatsby  \
  --prefix-colors blue.bold,yellow.bold  \
  --kill-others  \
  --kill-others-on-fail  \
    "yarn watch-bsb" "yarn develop" \
