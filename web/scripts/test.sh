#!/bin/bash
set -e

yarn bsb -make-world

while getopts "w" opt; do
  case $opt in
  w) JEST_MODE=watch ;;
  esac
done

case "$JEST_MODE" in
watch)
  yarn concurrently --names bsb,jest --prefix-colors blue.bold,yellow.bold --kill-others --kill-others-on-fail "yarn bsb -w" "NODE_ENV=test yarn jest --watchAll --colors"
  ;;
*)
  NODE_ENV=test yarn jest --colors
  ;;
esac
