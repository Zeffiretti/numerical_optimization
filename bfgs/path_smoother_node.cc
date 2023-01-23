// Copyright 2023 HELLION XIE. All rights reserved.
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>

#include "bfgs/path_smoother/path_smoother.h"
#include "bfgs/visualizer/map.h"
#include "math/cubic_curve.h"
#include "math/cubic_spline.h"
#include "math/cubic_splines.h"

namespace raylib {
#include "third_party/raylib/src/raylib.h"
}

const int kWindowWidth = 1000;
const int kWindowHeight = 1000;

void logic(TwoDMap& map, PathSmoother& smoother) {
  int num_point = 0;
  while (!map.isClosed()) {
    // listen to mouse click
    if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON)) {
      Eigen::Vector2d point(raylib::GetMouseX(), raylib::GetMouseY());
      map.addMarker(point);
      num_point++;
      if (num_point == 1) {
        smoother.setInitailPoint(point);
      }
      if (num_point >= 2) {
        map.clearMarkers();
        smoother.setTerminalPoint(point);
        map.addMarker(smoother.getInitailPoint(), Eigen::Vector3d(0, 0, 255));
        map.addMarker(smoother.getTerminalPoint(), Eigen::Vector3d(0, 0, 255));
        Eigen::MatrixXd x_smooth;
        Eigen::MatrixXd x;
        std::cout << "Path finding..." << std::endl;
        smoother.smooth(x, &x_smooth);
        std::cout << "Path found!" << std::endl;
        auto path = smoother.getOptimal();
        // std::cout << "path: " << path << std::endl;

        // map.addMarkers(x_smooth * 1000, Eigen::Vector3d(255, 0, 0));
        map.addMarkers(path, Eigen::Vector3d(0, 255, 0));

        num_point = 0;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main(int argc, char* argv[]) {
  TwoDMap map(kWindowWidth, kWindowHeight);
  PathSmoother smoother(50);
  smoother.setObstacles(map.getObstacles());

  std::thread logic_thread(logic, std::ref(map), std::ref(smoother));
  logic_thread.detach();

  map.update();
  map.close();

  // sleep 1s to wait for the logic thread to finish
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}
