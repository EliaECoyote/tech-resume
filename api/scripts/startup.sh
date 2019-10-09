#!/bin/bash
set -e

echo $PWD;

DIR="/lib"

[ ! -d "$DIR" ] && yarn build-bsb

yarn concurrently 
  --names bsb,node 
  --prefix-colors blue.bold,yellow.bold 
  --kill-others 
  --kill-others-on-fail 
    "yarn watch-bsb" "yarn watch-node"
