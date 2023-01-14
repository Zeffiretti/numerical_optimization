# encoding: utf-8

## Copyright 2022 HELLION XIE
## This script is used to configure the environment for the numerical optimization project.
## Usage: ./configure.sh [-h] [-t [all|main|ros]] [-c] [-b [0|1]]
## Options:
##   -h, --help: print this help message
##   -t, --test: test the program by executing coressponding nodes
##        all, execute both test nodes (default)
##        lsgd, linear search gradient descent
##        google, run google unit tests
##        if the param is left blank, the script will execute both test nodes.
##   -c, --clean: clean the build directory
##   -b, --build:
##        1, build the project (default)
##        0, do not build the project

help() {
  sed -rn 's/^## ?//;T;p' "$0"
  exit 0
}

arg_help="0"
arg_test_lsgd="0"
arg_test_bfgs="0"
arg_test_google="0"
arg_clean="0"
arg_build="1"
extra_args=""

while true; do
  key="$1"
  case $key in
  -h | --help)
    help
    ;;
  -t | --test)
    args2=1
    case $2 in
    all)
      arg_test_lsgd="1"
      arg_test_bfgs="1"
      arg_test_google="1"
      args2=2
      ;;
    lsgd)
      arg_test_lsgd="1"
      arg_test_google="0"
      args2=2
      ;;
    bfgs)
      arg_test_bfgs="1"
      arg_test_google="0"
      args2=2
      ;;
    google)
      arg_test_google="1"
      args2=2
      ;;
    "" | -*)
      arg_test_google="0"
      args2=1
      ;;
    *)
      echo "Unkown test option: $2"
      args2=1
      ;;
    esac
    shift $args2
    ;;
  -c | --clean)
    arg_clean="1"
    shift
    ;;
  -b | --build)
    arg_build="$2"
    shift 2
    ;;
  "")
    break
    ;;
  *)
    extra_args="$extra_args $1"
    shift 1
    ;;
  esac
done
echo "extra_args: $extra_args"

if [ "$arg_help" != "0" ]; then
  help
  exit 1
fi

set -ex
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECT_ROOT=$SCRIPTPATH/..

cd $PROJECT_ROOT
if [ "$arg_clean" != "0" ]; then
  if [ -d "build" ]; then
    rm -rf build
  fi
  if [ -d "data" ]; then
    rm -rf data
  fi
fi

if [ "$arg_build" != "1" ]; then
  exit 0
fi

if [ ! -d "build" ]; then
  mkdir build
fi
cd build

unit_tests="
"
if [ "$arg_test_google" != "0" ]; then
  unit_tests="
  "
fi
# build and compile the project
targets="
  $unit_tests
  lsgd
  bfgs
"
cmake ..
make -j $(nproc) $targets

echo "Project configuration successfully."

# if pass '-test', run the main_node
if [ "$arg_test_google" != "0" ]; then
  cd $PROJECT_ROOT/build
  make test
  cd $PROJECT_ROOT
fi
if [ "$arg_test_lsgd" != "0" ]; then
  $SCRIPTPATH/test.sh lsgd
fi
if [ "$arg_test_bfgs" != "0" ]; then
  $SCRIPTPATH/test.sh bfgs
fi
