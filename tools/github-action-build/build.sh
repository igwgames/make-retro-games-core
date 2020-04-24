#!/bin/sh -l

cd /
CI_BUILD=true make
ls -lah /rom
ls -lah /github
ls -lah /github/workspace/rom
cp rom/* $GITHUB_WORKDSPACE