#!/bin/bash
# the following command will replace copyright institute from Tencent to BIT
# set -ex
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd $SCRIPTPATH/..
echo "executing: set copyright to hellion.xie"

# Set the copyright information
copyright="Copyright $(date +%Y) HELLION XIE. All rights reserved."
copyright_reg="Copyright [0-9]{4} HELLION XIE. All rights reserved."

# Find all C++ source code files and CMakeLists files in the project folder, excluding files in the third_party folder
files=$(find . -path "./third_party" -prune -name "*.cpp" -o -name "*.cc" -o -name "*.h" -o -name "CMakeLists.txt")

for file in $files; do
  first_line=$(head -n 1 $file)

  if [[ "$file" =~ CMakeLists\.txt$ ]]; then
    # If it is a CMakeLists file, add '#' to the beginning of the copyright information line
    copyright_cmake="# $copyright"
    copyright_cmake_reg="# $copyright_reg"
    if [[ ! "$first_line" =~ $copyright_cmake_reg ]]; then
      sed -i "1i$copyright_cmake" $file
    fi
  else
    # f it is a C++ file, add '//' to the beginning of the copyright information line
    copyright_cc="\/\/ $copyright"
    copyright_cc_reg="\/\/ $copyright_reg"
    if [[ ! "$first_line" =~ $copyright_cc_reg ]]; then
      sed -i "1i$copyright_cc" $file
    fi
  fi
done
