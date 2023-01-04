// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef OPTIMIZER_OPTIMIZER_H_
#define OPTIMIZER_OPTIMIZER_H_

#include <cstdlib>

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
    srand(time(NULL));
    x_ = Eigen::VectorXd::Random(dim);
    x_ = x_.array() * 20 - 10;
  }
  virtual ~OptimizerBase() = default;

  virtual void optimize() = 0;
  void setOptimizerOption(const OptimizerOptions& option) { options_ = option; }
  void setObjectiveFunction(double (*f_)(const Eigen::VectorXd& x, size_t n)) { f = f_; }
  void setGradientFunction(Eigen::VectorXd (*g_)(const Eigen::VectorXd& x, size_t n)) { g = g_; }
  void setInitialPoint(const Eigen::VectorXd& x) { x_ = x; }

 protected:
  OptimizerOptions options_;
  Eigen::VectorXd x_;
  int n = 0;
  // objective function instance
  double (*f)(const Eigen::VectorXd& x, size_t n);
  // gradient function instance
  Eigen::VectorXd (*g)(const Eigen::VectorXd& x, size_t n);
};

#endif  // OPTIMIZER_OPTIMIZER_H_