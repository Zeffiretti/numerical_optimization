// Copyright 2023 HELLION XIE. All rights reserved.
#include <cstddef>
#include <iostream>

#include "optimizer/armijo_optimizer.h"
#include "optimizer/optimizer.h"
#include "third_party/eigen/Eigen/Dense"

double rosenbrock(const Eigen::VectorXd& x, size_t n) {
  double res = 0.0;
  for (size_t i = 0; i < n / 2; ++i) {
    double odd = x(2 * i), even = x(2 * i + 1);
    res += 100 * (odd * odd - even) * (odd * odd - even) + (odd - 1) * (odd - 1);
  }
  return res;
}

Eigen::VectorXd gradient(const Eigen::VectorXd& x, size_t n) {
  Eigen::VectorXd diff = Eigen::VectorXd::Zero(n);
  for (size_t i = 0; i < n / 2; ++i) {
    double odd = x(2 * i), even = x(2 * i + 1);
    diff(2 * i) = 400 * odd * (odd * odd - even) + 2 * (odd - 1);
    diff(2 * i + 1) = -200 * (odd * odd - even);
  }
  return diff;
}

int main(int argc, char* argv[]) {
  int n = 100;
  if (argc == 2) {
    n = atoi(argv[1]);
  }
  ArmijoOptimizer optim(n);
  optim.setObjectiveFunction(rosenbrock);
  optim.setGradientFunction(gradient);
  OptimizerOptions options;
  options.t = 1.0;
  options.c = 1e-4;
  options.sigma = 1e-5;
  optim.setOptimizerOption(options);
  Eigen::VectorXd x = Eigen::VectorXd::Ones(n) * 10;  // start from (10, 10, ..., 10)
  optim.setInitialPoint(x);
  optim.optimize();
  return 0;
}
