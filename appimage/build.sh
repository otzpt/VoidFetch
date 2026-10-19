#!/bin/bash
# Builds VoidFetch-x86_64.AppImage. Needs gcc (with static libc) and appimagetool in PATH.
set -e
cd "$(dirname "$0")"
APPDIR=$(mktemp -d)/VoidFetch.AppDir
mkdir -p "$APPDIR/usr/bin"
gcc -Wall -Wextra -O2 -static ../main.c -o "$APPDIR/usr/bin/voidfetch"
cp AppRun voidfetch.desktop voidfetch.png "$APPDIR/"
ARCH=x86_64 appimagetool "$APPDIR" ../VoidFetch-x86_64.AppImage
