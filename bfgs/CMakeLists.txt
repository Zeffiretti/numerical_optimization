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
add_executable(path_smoother_node path_smoother_node.cc)
target_link_libraries(path_smoother_node
  PUBLIC
    eigen
    raylib
  PRIVATE
    path_smoother
    optimizer
    2dvisualizer
    optim_math
)
target_include_directories(path_smoother_node
  PUBLIC SYSTEM
    ${OPTIM_INCLUDE_DIRS}
)
add_subdirectory(path_smoother)
add_subdirectory(visualizer)
