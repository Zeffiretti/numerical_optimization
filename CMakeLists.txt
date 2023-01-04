# Copyright 2023 HELLION XIE. All rights reserved.
cmake_minimum_required(VERSION 3.16)

option(BUILD_TESTING "Build tests" ON)

project(numerical_optimization)
include(CTest)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/cmake
)
include(CtestAddTest)
include(CxxOptions)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Reduces build size while retaining basic debug info.
add_compile_options(-ggdb1)

# Required for Eigen alignment fix
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TARGET_ARCH "skylake" CACHE STRING "Target architecture")
if(TARGET_ARCH)
  add_compile_options(-march=${TARGET_ARCH} -mtune=${TARGET_ARCH})
endif()

# For VS Code integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

list(APPEND OPTIM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(OPTIM_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(third_party)
add_subdirectory(optimizer)
add_subdirectory(lsgd)

