#!/bin/bash
## Copyright 2022 HELLION XIE

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
format_file=$SCRIPTPATH/../.clang-format
echo format file is $format_file

# check clang-format version, if not 12, print instructions
# for installing correct version and exit
clang_format_version=$(
  clang-format --version | grep -oP '\d+\.\d+\.\d+' | awk 'NR==1{print $1}'
)
if [[ ! $clang_format_version =~ ^12.* ]]; then
  echo "Your system clang-format version is $clang_format_version"
  echo "clang-format version 12 is required to format code."
  echo "Please install clang-format-12 and run this script again."
  echo "For Ubuntu 20.04, you can install clang-format-12 by running:"
  echo "apt-install clang-format-12"
  echo "Set the system default clang-format to clang-format-12 by running:"
  echo "update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-12 100"
  echo "(You may need to run this command with sudo.)"
  exit 1
fi

cd $SCRIPTPATH/..
$SCRIPTPATH/copyright.sh
echo "executing: format code"
source_file_list=$(find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\|cc\)')
# exclude third_party
source_file_list=$(echo "$source_file_list" | grep -v third_party)
clang-format -i -style=file $source_file_list
$SCRIPTPATH/static_check.sh
