// Copyright 2023 HELLION XIE. All rights reserved.
#include "optimizer/optimizer.h"

#include <cstdio>

void OptimizerBase::log(const Eigen::VectorXd& x, double f, const Eigen::VectorXd& g, int iter, double t, double c,
                        double sigma) {
  if (fp == nullptr) {
    return;
  }
  if (iter == 0) {
    fprintf(fp, "iter, x, f, g, t, c, sigma\n");
  }
  fprintf(fp, "%d, ", iter);
  for (int i = 0; i < x.size(); i++) {
    fprintf(fp, "%f, ", x(i));
  }
  fprintf(fp, "%f, ", f);
  for (int i = 0; i < g.size(); i++) {
    fprintf(fp, "%f, ", g(i));
  }
  fprintf(fp, "%f, %f, %f\n", t, c, sigma);
}

OptimizerBase::~OptimizerBase() {
  if (fp != nullptr) {
    fclose(fp);
  }
}