// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_
#define BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_

#include "math/cubic_spline.h"
#include "math/cubic_splines.h"
#include "third_party/eigen/Eigen/Dense"
#include "third_party/eigen/Eigen/src/Core/Matrix.h"

class PathSmoother {
 public:
  PathSmoother() = default;
  explicit PathSmoother(int spline_segment_num) : num_segments(spline_segment_num) {}
  PathSmoother(int spline_segment_num, const Eigen::MatrixXd& obstacles)
      : num_segments(spline_segment_num), obstacles(obstacles) {}
  ~PathSmoother() = default;

  Eigen::Vector2d getInitailPoint() const { return initial_point; }
  Eigen::Vector2d getTerminalPoint() const { return terminal_point; }
  void setInitailPoint(const Eigen::Vector2d& initail_point) { this->initial_point = initail_point; }
  void setTerminalPoint(const Eigen::Vector2d& terminal_point) { this->terminal_point = terminal_point; }
  void setObstacles(const Eigen::MatrixXd& obstacles) { this->obstacles = obstacles; }
  [[nodiscard]] Eigen::MatrixXd getOptimal() const { return optimal_sol; }

  void smooth(const Eigen::MatrixXd& x, Eigen::MatrixXd* x_smooth);

 private:
  // the number of spline segments
  int num_segments = 3;

  // the initial and terminal points
  Eigen::Vector2d initial_point, terminal_point;

  // cubic spline parameters
  optim_math::CubicSplineParam spline_param;

  // via points
  // Generallt, the via points are the control points of the cubic spline in a 2-d space.
  // For optimization purpose, we define it in vector form, with the shape of (2 * (N - 1), 1)
  // where N is the number of spline segments.
  Eigen::VectorXd via_points;

  // obstacles
  // The obstacles are defined as circles, with the shape of (M, 3), where M is the number of obstacles,
  // one row for one obstacle, with the first two columns for the center of the circle, and the third column for the
  // radius.
  Eigen::MatrixXd obstacles;

  // the result path after optimization
  Eigen::MatrixXd x_smooth;

  // the weight of the potential energy
  double penalty_weight = 1000.0;

  // the epsilon for the distance between the line and the obstacle
  const double kDBL_EPSILON = 1e-6;

  // cubic spline cacalator
  optim_math::CubicSpline cubic_spline;

  // optimal solutions
  Eigen::MatrixXd optimal_sol;

  double getEnergy(const Eigen::VectorXd& x);
  double getPotentialEnergy(const Eigen::VectorXd& x);
  double getStretchEnergy(const Eigen::VectorXd& x);
  double evaluate(const Eigen::VectorXd& x, Eigen::VectorXd* gradient);
  Eigen::VectorXd getGradient(const Eigen::VectorXd& x);
};

#endif  // BFGS_PATH_SMOOTHER_PATH_SMOOTHER_H_
