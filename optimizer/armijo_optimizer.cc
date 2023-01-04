// Copyright 2023 HELLION XIE. All rights reserved.
#include "optimizer/armijo_optimizer.h"

#include <cstddef>
#include <iostream>

#include "third_party/eigen/Eigen/Core"

void ArmijoOptimizer::optimize() {
  Eigen::VectorXd x = x_;
  std::cout << "Initial point: " << x.transpose() << std::endl;
  auto delta = g(x, n);
  double sigma_square = options_.sigma * options_.sigma;
  size_t iter = 0;
  while (delta.transpose() * delta > sigma_square) {
    double dd = delta.transpose() * delta;
    while (f(x - options_.t * delta, n) > f(x, n) - options_.c * options_.t * dd) {
      options_.t *= 0.5;
    }
    x = x - options_.t * delta;
    delta = g(x, n);
    iter++;
  }
  std::cout << "Final point: " << x.transpose() << std::endl;
  std::cout << "Final value: " << f(x, n) << std::endl;
  std::cout << "Iterations: " << iter << std::endl;
}
