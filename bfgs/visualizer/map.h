// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef BFGS_VISUALIZER_MAP_H_
#define BFGS_VISUALIZER_MAP_H_

#include <cstddef>
#include <thread>

#include "third_party/eigen/Eigen/Dense"

namespace raylib {
#include "third_party/raylib/src/raylib.h"
}

class TwoDMap {
 public:
  TwoDMap() {
    clearMarkers();
    initMapWindow();
    generateObstacles();
  }
  TwoDMap(int w, int h) : width(w), height(h) {
    clearMarkers();
    initMapWindow();
    generateObstacles();
  }
  void update();
  void close() { is_closed = true; }
  bool isClosed() { return is_closed; }
  void addMarker(const Eigen::Vector2d& pos, const Eigen::Vector3d& color = Eigen::Vector3d(255, 0, 0)) {
    markers.conservativeResize(markers.rows() + 1, 5);
    markers.block(markers.rows() - 1, 0, 1, 2) = pos.transpose();
    markers.block(markers.rows() - 1, 2, 1, 3) = color.transpose();
  }

  // add markers to the 2-d map
  // param: pos, color
  //     pos: Eigen::MatrixXd, positions of the markders, with the shape of Nx3. Each row of pos is the coordinates of
  //     one marker.
  //   color: Eigen::Vector3d, colors of the markers, in the sequence of (R, G, B). The range of the value is [0, 255]
  void addMarkers(const Eigen::MatrixXd& pos, const Eigen::Vector3d& color = Eigen::Vector3d(255, 0, 0)) {
    markers.conservativeResize(markers.rows() + pos.rows(), 5);
    markers.block(markers.rows() - pos.rows(), 0, pos.rows(), 2) = pos;
    for (size_t i = markers.rows() - pos.rows(); i < markers.rows(); ++i) {
      markers.block(i, 2, 1, 3) = color.transpose();
    }
  }
  void clearMarkers() { markers.resize(0, 5); }

  // data getter
  [[nodiscard]] Eigen::MatrixXd getObstacles() const { return obstacles; }

 private:
  void render();
  void initMapWindow();
  void generateObstacles();

  int width = 1000;
  int height = 1000;
  int frame_rate = 60;
  bool is_closed = false;

  // obstacles: numbers * 3 matrix, each row is (x, y, r)
  Eigen::MatrixXd obstacles;
  Eigen::MatrixXd markers;
};

#endif  // BFGS_VISUALIZER_MAP_H_
