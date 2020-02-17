#!/bin/bash
set -e

echo firebase token is: $FIREBASE_TOKEN

firebase deploy --debug --debug --token "$FIREBASE_TOKEN" --only hosting
