#!/bin/sh -l

# Get dependencies for all npm-based tools
# cd tools/tmx2c/src && npm install && cd ../../../tools/sprite_def2img/src && npm install && cd ../../../tools/chr2img/src && npm install

cd /
ls -lah tools/tmx2c/src
CI_BUILD=true make