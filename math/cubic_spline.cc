// Copyright 2023 HELLION XIE. All rights reserved.
#include "math/cubic_spline.h"

#include <iostream>

#include "third_party/eigen/Eigen/Dense"

namespace optim_math {
void calcCubicSplineParam(const Eigen::MatrixXd& x, CubicSplineParam* param) {
  const int N = x.rows() - 1;  // x contains initial and terminal points, the number of segments is x.rows()-1
  param->N = N;
  Eigen::MatrixXd& ai = param->ai;
  Eigen::MatrixXd& bi = param->bi;
  Eigen::MatrixXd& ci = param->ci;
  Eigen::MatrixXd& di = param->di;

  ai = x.block(0, 0, N, 2);
  bi.resize(N, 2);
  ci.resize(N, 2);
  di.resize(N, 2);

  // step 1: compute the middle variables: Di
  Eigen::MatrixXd Hi(N - 1, N - 1);
  Hi.diagonal() = Eigen::MatrixXd::Ones(N - 1, 1) * 4;
  Hi.diagonal(1) = Eigen::MatrixXd::Ones(N - 2, 1) * 1;
  Hi.diagonal(-1) = Eigen::MatrixXd::Ones(N - 2, 1) * 1;
  Eigen::MatrixXd Xi(N - 1, 2);
  for (int i = 1; i <= N - 1; ++i) {
    Xi.row(i - 1) = 3 * (x.row(i + 1) - x.row(i - 1));
  }
  // add zero to the head and tail of Di
  const int dim = x.cols();
  Eigen::MatrixXd Di_head = Eigen::MatrixXd::Zero(1, dim);
  Eigen::MatrixXd Di_tail = Eigen::MatrixXd::Zero(1, dim);
  Eigen::MatrixXd Di = Eigen::MatrixXd::Zero(N + 1, dim);
  Di.block(1, 0, N - 1, 2) = Hi.inverse() * Xi;

  bi.setZero();
  bi.block(1, 0, N - 1, 2) = Di.block(1, 0, N - 1, 2);
  // todo(@Zeffiretti): add the energy and the gradient of the energy
  param->stretch_energy = 0;
  param->inner_gradient = Eigen::MatrixXd::Zero(N, 2);
  for (int i = 0; i < N; i++) {
    ci.row(i) = 3 * (x.row(i + 1) - x.row(i)) - 2 * Di.row(i) - Di.row(i + 1);
    di.row(i) = 2 * (x.row(i) - x.row(i + 1)) + Di.row(i) + Di.row(i + 1);
  }
  for (int i = 0; i < N; ++i) {
    param->stretch_energy += (x.row(i + 1) - x.row(i)).squaredNorm();
    param->inner_gradient.row(i) = 2 * (x.row(i + 1) - x.row(i));
  }
}

void calcCubicSplinePoints(const CubicSplineParam& param, Eigen::MatrixXd* x) {
  const int N = param.N;
  const Eigen::MatrixXd& ai = param.ai;
  const Eigen::MatrixXd& bi = param.bi;
  const Eigen::MatrixXd& ci = param.ci;
  const Eigen::MatrixXd& di = param.di;

  *x = Eigen::MatrixXd::Zero(N * 100, 2);
  for (int i = 0; i < N; ++i) {
    double s = 0;
    for (int j = 0; j < 100; ++j) {
      s += 0.01;
      Eigen::MatrixXd p = ai.row(i) + bi.row(i) * s + ci.row(i) * s * s + di.row(i) * s * s * s;
      x->row(i * 100 + j) = p;
    }
  }
}
}  // namespace optim_math