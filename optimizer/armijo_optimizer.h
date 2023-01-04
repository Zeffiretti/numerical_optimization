// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef OPTIMIZER_ARMIJO_OPTIMIZER_H_
#define OPTIMIZER_ARMIJO_OPTIMIZER_H_

#include "optimizer/optimizer.h"

class ArmijoOptimizer : public OptimizerBase {
 public:
  ArmijoOptimizer() = default;
  explicit ArmijoOptimizer(int n) : OptimizerBase(n) {}
  ~ArmijoOptimizer() override = default;

  void optimize() override;
};

#endif  // OPTIMIZER_ARMIJO_OPTIMIZER_H_