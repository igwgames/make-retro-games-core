#!/bin/sh -l

cd /
CI_BUILD=true make
ls -lah /rom
ls -lah /github/home
# cp rom/* $GITHUB_WORKDSPACE
mkdir /github/home/rom || echo "Dir exists"
cp rom/starter.nes /github/home/rom
ls -lah /github/home/rom