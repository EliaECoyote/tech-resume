#!/bin/bash
set -e

[ -f ".bsb.lock" ] && rm .bsb.lock

yarn clean
yarn build-bsb

yarn concurrently \
  --names bsb,gatsby \
  --raw \
  --prefix-colors blue.bold,yellow.bold \
  "yarn watch-bsb" "yarn develop"
