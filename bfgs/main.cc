// Copyright 2023 HELLION XIE. All rights reserved.
#include <cstddef>
#include <iostream>

#include "optimizer/lbfgs.h"
#include "optimizer/optimizer.h"
#include "third_party/eigen/Eigen/Dense"

double rosenbrock(const Eigen::VectorXd& x) {
  size_t n = x.size();
  double res = 0.0;
  for (size_t i = 0; i < n / 2; ++i) {
    double odd = x(2 * i), even = x(2 * i + 1);
    res += 100 * (odd * odd - even) * (odd * odd - even) + (odd - 1) * (odd - 1);
  }
  return res;
}

Eigen::VectorXd gradient(const Eigen::VectorXd& x) {
  size_t n = x.size();
  Eigen::VectorXd diff = Eigen::VectorXd::Zero(n);
  for (size_t i = 0; i < n / 2; ++i) {
    double odd = x(2 * i), even = x(2 * i + 1);
    diff(2 * i) = 400 * odd * (odd * odd - even) + 2 * (odd - 1);
    diff(2 * i + 1) = -200 * (odd * odd - even);
  }
  return diff;
}

// main function
// argv[1]: string, log file path prefix
// argv[2]: int, dimension of the problem, default 100
int main(int argc, char* argv[]) {
  int n = 100;
  char* log_path = nullptr;
  if (argc > 1) {
    log_path = argv[1];
  }
  // if log path does not end with .log, add .log suffix
  if (log_path != nullptr) {
    size_t len = strlen(log_path);
    if (len < 4 || strcmp(log_path + len - 4, ".log") != 0) {
      char* new_log_path = new char[len + 5];
      strcpy(new_log_path, log_path);
      strcat(new_log_path, ".log");
      log_path = new_log_path;
    }
  }
  if (argc > 2) {
    n = atoi(argv[2]);
  }
  LBFGSOptimizer optim(n);
  optim.setObjectiveFunction(rosenbrock);
  optim.setGradientFunction(gradient);
  OptimizerOptions options;
  options.t = 1.0;
  options.c = 1e-4;
  options.sigma = 1e-4;
  optim.setOptimizerOption(options);
  // Eigen::VectorXd x = Eigen::VectorXd::Ones(n) * 10;  // start from (10, 10, ..., 10)
  // optim.setInitialPoint(x);
  if (log_path != nullptr) {
    optim.setLogPath(log_path);
  }
  optim.optimize();
  Eigen::VectorXd x;
  optim.getOptimalPoint(&x);
  std::cout << "Optimal point: " << x.transpose() << std::endl;
  return 0;
}
