## Copyright 2022 HELLION XIE. All rights reserved.

# 获取当前日期时间作为时间戳 yyyy-mm-dd-hh-mm-ss
timestamp=$(date "+%Y-%m-%d-%H-%M-%S")
# 获取当前目录
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECT_ROOT=$SCRIPTPATH/..

cd $PROJECT_ROOT
# 检查data目录是否存在，不存在则创建
if [ ! -d "data" ]; then
  mkdir data
fi

log_file="$PROJECT_ROOT/data/$timestamp.log"
$PROJECT_ROOT/build/lsgd/lsgd $log_file 2
python $PROJECT_ROOT/scripts/paint.py $log_file
