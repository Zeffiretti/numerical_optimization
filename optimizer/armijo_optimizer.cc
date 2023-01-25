// Copyright 2023 HELLION XIE. All rights reserved.
#include "optimizer/armijo_optimizer.h"

#include <cstddef>
#include <iostream>

#include "third_party/eigen/Eigen/Core"

void ArmijoOptimizer::optimize() {
  Eigen::VectorXd x = x_;
  auto delta = g(x);
  const double sigma_square = options_.sigma * options_.sigma;
  size_t iter = 0;
  while (delta.transpose() * delta > sigma_square) {
    const double dd = delta.transpose() * delta;
    double t = options_.t;
    while (f(x - t * delta) > f(x) - options_.c * t * dd) {
      t *= 0.5;
    }
    x = x - t * delta;
    delta = g(x);
    if (iter % 10 == 0) {
      log(x, f(x), delta, iter, options_.t, options_.c, options_.sigma);
    }
    iter++;
  }
}
