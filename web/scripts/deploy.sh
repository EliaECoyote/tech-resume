#!/bin/bash
set -e

firebase deploy --debug --token "$FIREBASE_TOKEN" --only hosting
