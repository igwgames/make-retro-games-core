#!/bin/sh -l

cd /
CI_BUILD=true make
ls -lah /rom
ls -lah /github/home
# cp rom/* $GITHUB_WORKSPACE
mkdir $GITHUB_WORKSPACE/rom || echo "Dir exists"
cp rom/starter.nes $GITHUB_WORKSPACE/rom/starter.nes
ls -lahr $GITHUB_WORKSPACE/rom