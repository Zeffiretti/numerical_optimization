#!/bin/bash
## Copyright 2022 Beijing Institute of Technology

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
echo "executing: static check"

cd $SCRIPTPATH/..
# third_party libraries are not checked, for speeding up the code check
cpplint --recursive --exclude=third_party/* --exclude=build/* .
