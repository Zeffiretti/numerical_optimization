// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef OPTIMIZER_LBFGS_H_
#define OPTIMIZER_LBFGS_H_

#include "optimizer/optimizer.h"

#include "third_party/eigen/Eigen/Dense"

class LBFGSOptimizer : public OptimizerBase {
  // L-BFGS optimizer for possibly non-convex non-smooth functions
 public:
  LBFGSOptimizer() = default;
  explicit LBFGSOptimizer(int n) : OptimizerBase(n) {}
  ~LBFGSOptimizer() override = default;

  void optimize() override;

 private:
  void updateHessian(const Eigen::VectorXd& s, const Eigen::VectorXd& y);
  Eigen::MatrixXd B;

  // the number of previous iterations to store
  int m = 10;

  // Lewis and Nocedal method parameters c1 and c2
  double c1 = 1e-4;
  double c2 = 0.9;
};
#endif  // OPTIMIZER_LBFGS_H_
