# encoding: utf-8

## Copyright 2022 Beijing Institute of Technology
## This script is used to configure the environment for the smurosim project.
## Usage: ./configure.sh [-h] [-t [all|main|ros]] [-c] [-b [0|1]]
## Options:
##   -h, --help: print this help message
##   -t, --test: test the program by executing main_node or/and ros_test
##        all, execute both test nodes (default)
##        main, only execute main_node to test non-ros functions
##        ros, only execute ros_test to test ros-related functions
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
arg_test_main="0"
arg_test_ros="0"
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
      arg_test_ros="1"
      arg_test_main="1"
      arg_test_google="1"
      args2=2
      ;;
    google)
      arg_test_ros="0"
      arg_test_main="0"
      arg_test_google="1"
      args2=2
      ;;
    main)
      arg_test_main="1"
      arg_test_ros="0"
      arg_test_google="0"
      args2=2
      ;;
    ros)
      arg_test_main="0"
      arg_test_ros="1"
      arg_test_google="0"
      args2=2
      ;;
    "" | -*)
      arg_test_ros="1"
      arg_test_main="1"
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

. /opt/ros/$ROS_DISTRO/setup.sh

set -ex
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECT_ROOT=$SCRIPTPATH/..

cd $PROJECT_ROOT
if [ "$arg_clean" != "0" ]; then
  if [ -d "build" ]; then
    rm -rf build
  fi
  if [ -f "platform/model/urdf/smuro.convert.urdf" ]; then
    rm platform/model/urdf/smuro.convert.urdf
  fi
fi

# [Deprecated] generate_urdf script calling has been moved into platform/model/urdf/CMakeLists.txt
# generate urdf file
# cd $SCRIPTPATH/../platform/model/urdf
# $SCRIPTPATH/generate_urdf.sh\
#   -x smuro.urdf.xacro\
#   -u smuro.convert.urdf\
#   -a ../config/xacro.config
# cd $SCRIPTPATH/..

if [ "$arg_build" != "1" ]; then
  exit 0
fi

if [ ! -d "build" ]; then
  mkdir build
fi
cd build

unit_tests=
if [ "$arg_test_google" != "0" ]; then
  unit_tests="
  load_data_file_test
  string_split_test
  scoped_temporary_workspace_test
  yaml_tree_node_test
  yaml_tree_reader_test
  yaml_test_utils_test
  yaml_eigen_conversions_test
  scoped_parameter_test
  dynamic_scoped_parameter_test
  time_test
  spsc_ring_test
  triple_buffer_test
  thread_liveness_flag_test
  shared_memory_file_topic_test
  shared_memory_file_simple_topic_test
  shared_memory_file_spsc_ring_topic_test
  shared_memory_file_triple_buffer_topic_test
  cycle_test
  interpolation_test
  quartic_spline_pair_test
  triangle_angles_test
  quartic_spline_interpolation_test
  eigen_indexing_test
  pseudoinverse_test
  smoothed_axis_joystick_test
  "
fi
# build and compile the project
targets="
  $unit_tests
  main_node
  ros_test
  platform_smuro_gazebo_controller_plugin
  robotis_math
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
if [ "$arg_test_main" != "0" ]; then
  $PROJECT_ROOT/build/platform/func_test/main_node $SCRIPTPATH/../platform/model/urdf/smuro.convert.urdf
fi
if [ "$arg_test_ros" != "0" ]; then
  $PROJECT_ROOT/build/platform/ros_test/ros_test
fi
