#!/bin/bash
set -e

yarn bsb -make-world

case "$JEST_MODE" in
watch)
  yarn concurrently --names bsb,jest --prefix-colors blue.bold,yellow.bold --kill-others --kill-others-on-fail "yarn bsb -w" "NODE_ENV=test yarn jest --watchAll"
  ;;
*)
  NODE_ENV=test yarn jest
  ;;
esac
