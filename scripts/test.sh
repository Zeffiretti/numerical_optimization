## Copyright 2022 HELLION XIE. All rights reserved.

# 获取当前日期时间作为时间戳 yyyy-mm-dd-hh-mm-ss
timestamp=$(date "+%Y-%m-%d-%H-%M-%S")
# 获取当前目录
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECT_ROOT=$SCRIPTPATH/..
test_node=$1

cd $PROJECT_ROOT
if [ ! -d "data" ]; then
  mkdir data
fi

cd data
if [ ! -d "$test_node" ]; then
  mkdir $test_node
fi
cd $PROJECT_ROOT
log_file="$PROJECT_ROOT/data/$test_node/$timestamp.log"
$PROJECT_ROOT/build/$test_node/$test_node $log_file 2
python3 $PROJECT_ROOT/scripts/paint.py $log_file
