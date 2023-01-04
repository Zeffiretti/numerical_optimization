# Copyright 2023 HELLION XIE. All rights reserved.
add_executable(lsgd main.cc)
target_link_libraries(lsgd
  PUBLIC
    eigen
  PRIVATE
    optimizer
)
target_include_directories(lsgd
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)
