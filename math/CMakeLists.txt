# Copyright 2023 HELLION XIE. All rights reserved.
add_library(optim_math STATIC
  cubic_spline.cc
  cubic_spline.h
  cubic_curve.h
)
target_link_libraries(optim_math
  PUBLIC
    eigen
)
target_include_directories(optim_math
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)
