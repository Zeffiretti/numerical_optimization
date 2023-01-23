# Copyright 2023 HELLION XIE. All rights reserved.
add_library(2dvisualizer STATIC
  map.cc
  map.h
)
target_link_libraries(2dvisualizer
  PUBLIC
    eigen
    raylib
)
target_include_directories(2dvisualizer
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)
