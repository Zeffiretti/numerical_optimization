# Copyright 2023 HELLION XIE. All rights reserved.
add_library(optimizer STATIC
  optimizer.cc
  optimizer.h
  armijo_optimizer.cc
  armijo_optimizer.h
  lbfgs.cc
  lbfgs.h
)
target_link_libraries(optimizer
  PUBLIC
    eigen
)
target_include_directories(optimizer
  PUBLIC
    ${OPTIM_INCLUDE_DIRS}
)