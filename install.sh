#!/bin/bash

set -e

echo "Installing VoidFetch..."

# Compile VoidFetch
gcc main.c -o voidfetch

# Create the user's binary directory
mkdir -p "$HOME/.local/bin"

# Install the executable
install -m 755 voidfetch "$HOME/.local/bin/voidfetch"

echo "VoidFetch installed!"
echo "Run it with: voidfetch"
