#!/bin/sh -l

cd /
CI_BUILD=true make

mkdir $GITHUB_WORKSPACE/rom || echo "Dir exists"
cp rom/starter.nes $GITHUB_WORKSPACE/rom/game.nes
