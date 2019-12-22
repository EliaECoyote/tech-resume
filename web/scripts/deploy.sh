#!/bin/bash
set -e

[ -f ".bsb.lock" ] && rm .bsb.lock

yarn clean
yarn build-bsb
yarn build

firebase deploy
