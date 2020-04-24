#!/bin/sh -l

cd /
CI_BUILD=true make
ls -lah /rom
ls -lah /github
ls -lah /github/workspace
cp rom/* $GITHUB_WORKDSPACE