// Copyright 2023 HELLION XIE. All rights reserved.
#include "bfgs/visualizer/map.h"

#include <algorithm>
#include <cmath>
#include <random>

#include "third_party/eigen/Eigen/Dense"

namespace raylib {
#include "third_party/raylib/src/raylib.h"
}

void TwoDMap::initMapWindow() {
  raylib::InitWindow(width, height, "2d map");
  raylib::SetTraceLogLevel(raylib::LOG_WARNING);
  raylib::SetTargetFPS(frame_rate);
}

void TwoDMap::update() {
  while (!is_closed && !raylib::WindowShouldClose()) {
    render();
  }
  raylib::MCloseWindow();
}

void TwoDMap::render() {
  raylib::BeginDrawing();
  raylib::ClearBackground(raylib::WHITE);
  // paint obstacles in the map window [black]
  for (int i = 0; i < obstacles.rows(); ++i) {
    raylib::DrawCircle(obstacles(i, 0), obstacles(i, 1), obstacles(i, 2), raylib::BLACK);
  }
  // paint markers in the map window
  for (int i = 0; i < markers.rows(); ++i) {
    const int r = static_cast<int>(markers(i, 2)), g = static_cast<int>(markers(i, 3)),
              b = static_cast<int>(markers(i, 4));
    raylib::Color color;
    color.r = std::min(255, std::max(r, 0));
    color.g = std::min(255, std::max(g, 0));
    color.b = std::min(255, std::max(b, 0));
    color.a = 255;
    raylib::DrawCircle(markers(i, 0), markers(i, 1), 5, color);
  }
  raylib::EndDrawing();
}

void TwoDMap::generateObstacles() {
  // radomly generate obstacles numbers, between 5 and 40
  srand(time(nullptr));
  int numbers = rand() % 35 + 5;
  // numbers = 1;
  obstacles = Eigen::MatrixXd::Zero(numbers, 3);
  for (int i = 0; i < numbers; ++i) {
    int x = rand() % width;
    int y = rand() % height;
    int r = rand() % 100;
    // if the obstacle is too close to another obstacle, regenerate it
    if (i > 0) {
      for (int j = 0; j < i; ++j) {
        if (std::sqrt(std::pow(x - obstacles(j, 0), 2) + std::pow(y - obstacles(j, 1), 2)) < r + obstacles(j, 2)) {
          x = rand() % width;
          y = rand() % height;
          r = rand() % 100;
          j = -1;
        }
      }
    }
    obstacles(i, 0) = x;
    obstacles(i, 1) = y;
    obstacles(i, 2) = r;
  }
}
