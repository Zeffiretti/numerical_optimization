// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef OPTIMIZER_OPTIMIZER_H_
#define OPTIMIZER_OPTIMIZER_H_

#include <stdio.h>

#include <cstdlib>
#include <functional>
#include <ostream>

#include "third_party/eigen/Eigen/Dense"
#include "third_party/eigen/Eigen/src/Core/Matrix.h"

struct OptimizerOptions {
  double t = 1.0;
  double c = 0.5;
  double sigma = 1e-5;
};

class OptimizerBase {
 public:
  OptimizerBase() = default;
  explicit OptimizerBase(int dim) : n(dim) {
    // initialize x with distributed random numbers: [-100, 100]
    srand(time(nullptr));
    x_ = Eigen::VectorXd::Random(dim);
    x_ = x_.array() * 50;
  }
  virtual ~OptimizerBase();

  virtual void optimize() = 0;
  void setOptimizerOption(const OptimizerOptions& option) { options_ = option; }
  void setObjectiveFunction(double (*f_)(const Eigen::VectorXd& x)) { f = f_; }
  void setObjectiveFunction(std::function<double(const Eigen::VectorXd& x)> f_) { f = std::move(f_); }
  void setGradientFunction(Eigen::VectorXd (*g_)(const Eigen::VectorXd& x)) { g = g_; }
  void setGradientFunction(std::function<Eigen::VectorXd(const Eigen::VectorXd& x)> g_) { g = std::move(g_); }
  void setInitialPoint(const Eigen::VectorXd& x) { x_ = x; }
  void getOptimalPoint(Eigen::VectorXd* x) { *x = x_; }

  void setLogPath(const char* path) {
    log_path = const_cast<char*>(path);
    fp = fopen(log_path, "w");  // NOLINT
  }
  void log(const Eigen::VectorXd& x, double f, const Eigen::VectorXd& g, int iter, double t, double c, double sigma);

 protected:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  OptimizerOptions options_;
  Eigen::VectorXd x_;
  int n = 0;

  // funciton pointer to objective function
  std::function<double(const Eigen::VectorXd& x)> f;
  // function pointer to gradient function
  std::function<Eigen::VectorXd(const Eigen::VectorXd& x)> g;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

 private:
  char* log_path = nullptr;
  FILE* fp = nullptr;
};

#endif  // OPTIMIZER_OPTIMIZER_H_
