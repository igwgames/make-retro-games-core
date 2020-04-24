#!/bin/sh -l

# Get dependencies for all npm-based tools, and build all at once.
cd tools/tmx2c/src && npm install && cd ../../../tools/sprite_def2img/src && npm install && cd ../../../tools/chr2img/src && npm install && cd / && CI_BUILD=true make