# Copyright 2023 HELLION XIE. All rights reserved.
add_library(path_smoother STATIC
  path_smoother.cc
  path_smoother.h
)
target_link_libraries(path_smoother
  PUBLIC
    eigen
  PRIVATE
    optimizer
)
target_include_directories(path_smoother
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)