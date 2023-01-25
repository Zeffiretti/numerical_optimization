// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_
#define BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_

#include "math/cubic_spline.h"
#include "third_party/eigen/Eigen/Dense"
#include "third_party/eigen/Eigen/src/Core/Matrix.h"

class PathSmoother {
 public:
  PathSmoother() = default;
  explicit PathSmoother(int spline_segment_num) : num_segments(spline_segment_num) {}
  PathSmoother(int spline_segment_num, const Eigen::MatrixXd& obstacles)
      : num_segments(spline_segment_num), obstacles(obstacles) {}
  ~PathSmoother() = default;

  [[nodiscard]] Eigen::Vector2d getInitailPoint() const { return initial_point; }
  [[nodiscard]] Eigen::Vector2d getTerminalPoint() const { return terminal_point; }
  void setInitailPoint(const Eigen::Vector2d& initail_point) { this->initial_point = initail_point; }
  void setTerminalPoint(const Eigen::Vector2d& terminal_point) { this->terminal_point = terminal_point; }
  void setObstacles(const Eigen::MatrixXd& obstacles) { this->obstacles = obstacles; }
  [[nodiscard]] Eigen::MatrixXd getOptimal() const { return optimal_sol; }

  void smooth();

 private:
  // the number of spline segments
  int num_segments = 3;

  // the initial and terminal points
  Eigen::Vector2d initial_point, terminal_point;

  // obstacles
  // The obstacles are defined as circles, with the shape of (M, 3), where M is the number of obstacles,
  // with one row being one obstacle. The first two columns are the centers of the circles, and the third column is the
  // radius.
  Eigen::MatrixXd obstacles;

  // the weight of the potential energy
  double penalty_weight = 1000.0;

  // the epsilon for the distance between the line and the obstacle
  const double kDBL_EPSILON = 1e-6;

  // cubic spline cacalator
  optim_math::CubicSpline cubic_spline;

  // optimal solutions
  Eigen::MatrixXd optimal_sol;

  // object and gradient function for path smoothing
  double getEnergy(const Eigen::VectorXd& x);
  Eigen::VectorXd getGradient(const Eigen::VectorXd& x);
};

#endif  // BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_
