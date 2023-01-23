// Copyright 2023 HELLION XIE. All rights reserved.
#include "bfgs/path_smoother/path_smoother.h"

#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>

#include "math/cubic_spline.h"
#include "optimizer/lbfgs.h"
#include "third_party/eigen/Eigen/Eigen"

void PathSmoother::smooth(const Eigen::MatrixXd& x, Eigen::MatrixXd* x_smooth) {
  // todo(@zeffiretti): add path smoother
  cubic_spline.setConditions(initial_point, terminal_point, num_segments);

  const int nop = num_segments - 1;   // number of via points
  LBFGSOptimizer optimizer(nop * 2);  // we run the optimizer on a 2-d map
  // set this.getEnergy as the objective function
  std::function<double(const Eigen::VectorXd&)> energy_func =
      std::bind(&PathSmoother::getEnergy, this, std::placeholders::_1);
  optimizer.setObjectiveFunction(energy_func);
  // set this.getGradient as the gradient function
  std::function<Eigen::VectorXd(const Eigen::VectorXd&)> gradient_func =
      std::bind(&PathSmoother::getGradient, this, std::placeholders::_1);
  optimizer.setGradientFunction(gradient_func);
  OptimizerOptions options;
  options.t = 1.0;
  options.c = 1e-4;
  options.sigma = 1e-5;
  optimizer.setOptimizerOption(options);
  Eigen::VectorXd x_init = Eigen::VectorXd::Zero(nop * 2);
  // linearly interpolate between the initial and terminal points as the initial guess
  for (int i = 0; i < nop; ++i) {
    x_init(i * 2) = initial_point(0) + (terminal_point(0) - initial_point(0)) / (nop + 1) * (i + 1);
    x_init(i * 2 + 1) = initial_point(1) + (terminal_point(1) - initial_point(1)) / (nop + 1) * (i + 1);
  }
  optimizer.setInitialPoint(x_init);
  optimizer.optimize();
  Eigen::VectorXd x_opt;
  optimizer.getOptimalPoint(&x_opt);

  // resize x_opt to Matrix2xd, with each row being a point
  Eigen::Map<const Eigen::Matrix2Xd> x_optimal_map(x_opt.data(), 2, x_opt.size() / 2);
  optimal_sol = x_optimal_map.transpose();

  // reshape x_opt to Matrix2xd, with each row being a point
  Eigen::Map<const Eigen::Matrix2Xd> xMap(x_opt.data(), 2, x_opt.size() / 2);
  // add the initial and terminal points
  Eigen::MatrixXd points(num_segments + 1, 2);
  points.row(0) = initial_point.transpose();
  points.block(1, 0, num_segments - 1, 2) = xMap.transpose();
  points.row(num_segments) = terminal_point.transpose();
  // calculate the spline parameters
  optim_math::calcCubicSplineParam(points, &spline_param);
  // calculate the smooth path
  optim_math::calcCubicSplinePoints(spline_param, x_smooth);
  // std::cout << "smoothed path [in class]: " << x_smooth->transpose() << std::endl;
}

double PathSmoother::getEnergy(const Eigen::VectorXd& x) {
  Eigen::Map<const Eigen::Matrix2Xd> xMap(x.data(), 2, x.size() / 2);
  double energy = 0;
  cubic_spline.setInnerPoints(xMap);
  cubic_spline.getStretchEnergy(energy);
  if (std::isnan(energy)) {
    std::cout << "energy is nan" << std::endl;
    std::cout << "x: " << x.transpose() << std::endl;
  }

  for (int i = 0; i < num_segments - 1; i++) {
    const Eigen::Vector2d point = xMap.col(i);
    for (int j = 0; j < obstacles.rows(); j++) {
      const Eigen::Vector3d obstacle = obstacles.row(j).transpose();
      const Eigen::Vector2d delta = point - obstacle.head<2>();
      const double dist = delta.norm() + kDBL_EPSILON;
      const double signdist = dist - obstacle(2);
      if (signdist < 0.0) {
        energy -= signdist * penalty_weight;
      }
    }
  }
  return energy;
}

double PathSmoother::getStretchEnergy(const Eigen::VectorXd& x) { return 0; }

double PathSmoother::getPotentialEnergy(const Eigen::VectorXd& x) { return 0; }

Eigen::VectorXd PathSmoother::getGradient(const Eigen::VectorXd& x) {
  Eigen::Map<const Eigen::Matrix2Xd> xMap(x.data(), 2, x.size() / 2);
  // std::cout << "xMap: " << xMap << std::endl;
  Eigen::MatrixXd gradient = Eigen::MatrixXd::Zero(2, x.size() / 2);
  // std::cout << "x: " << x.transpose() << std::endl;
  // std::cout << "xMap: " << xMap << std::endl;
  cubic_spline.setInnerPoints(xMap);
  cubic_spline.getGrad(gradient);

  for (int i = 0; i < num_segments - 1; i++) {
    const Eigen::Vector2d point = xMap.col(i);
    for (int j = 0; j < obstacles.rows(); j++) {
      const Eigen::Vector3d obstacle = obstacles.row(j).transpose();
      const Eigen::Vector2d delta = point - obstacle.head<2>();
      const double dist = delta.norm() + kDBL_EPSILON;
      const double signdist = dist - obstacle(2);
      if (signdist < 0.0) {
        gradient.col(i) -= penalty_weight * delta / dist;
      }
    }
  }
  // map gradient to Eigen::VectorXd
  gradient.resize(gradient.size(), 1);
  return gradient;
}
