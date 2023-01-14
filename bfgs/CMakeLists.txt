# Copyright 2023 HELLION XIE. All rights reserved.
add_executable(bfgs main.cc)
target_link_libraries(bfgs
  PUBLIC
    eigen
  PRIVATE
    optimizer
)
target_include_directories(lsgd
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)
