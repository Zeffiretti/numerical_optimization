// Copyright 2023 HELLION XIE. All rights reserved.
#include "optimizer/lbfgs.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

#include "third_party/eigen/Eigen/Eigen"

void LBFGSOptimizer::updateHessian(const Eigen::VectorXd& s, const Eigen::VectorXd& y) {
  Eigen::MatrixXd const I = Eigen::MatrixXd::Identity(n, n);
  Eigen::MatrixXd const H = I - s * y.transpose() / (y.transpose() * s);
  Eigen::MatrixXd const G = I - y * s.transpose() / (y.transpose() * s);
  B = H * B * G + s * s.transpose() / (y.transpose() * s);
}

void LBFGSOptimizer::optimize() {
  Eigen::VectorXd x = x_;
  auto delta = g(x);
  double const sigma_square = options_.sigma * options_.sigma;
  size_t iter = 0;
  // initialize B = I
  B = Eigen::MatrixXd::Identity(n, n);
  // initialize d and t
  Eigen::VectorXd d = -B * delta;
  double t = options_.t;
  std::vector<Eigen::VectorXd> S, Y;
  S.clear();
  Y.clear();
  std::vector<double> RHO;
  RHO.clear();

  while (!d.hasNaN() && delta.transpose() * delta > sigma_square) {
    // update t using Lewis and Overton line search: weak Wolfe conditions
    t = options_.t;
    double lower_bound = 0, upper_bound = INFINITY;
    for (int i = 0; i < 50; ++i) {
      if (f(x) - f(x + t * d) < -c1 * t * d.transpose() * delta) {  // if S(t) condition fails
        upper_bound = t;
      } else if (d.transpose() * g(x + t * d) < c2 * d.transpose() * delta) {  // if C(t) condition fails
        lower_bound = t;
      } else {  // if both conditions hold, then we find a good t
        break;
      }
      if (upper_bound == INFINITY) {
        t = 2 * lower_bound;
      } else {
        t = (lower_bound + upper_bound) / 2;
      }
    }

    // update x: $x_{k+1} = x_k + t_k d_k$
    x = x + t * d;
    // update delta: $\delta_{k+1} = g(x_{k+1})$
    auto delta_new = g(x);
    // update s, y and rho at current point
    Eigen::VectorXd s = t * d;
    Eigen::VectorXd const y = delta_new - delta;
    delta = delta_new;
    double rho = 1 / ((s.transpose() * y));
    if (std::isnan(rho)) {
      rho = 1e6;
    }
    // update S, Y and RHO vectors, ensuring that the size of these vectors is no larger than m
    if (S.size() < m) {
      S.push_back(s);
      Y.push_back(y);
      RHO.push_back(rho);
    } else {
      S.erase(S.begin());
      Y.erase(Y.begin());
      RHO.erase(RHO.begin());
      S.push_back(s);
      Y.push_back(y);
      RHO.push_back(rho);
    }
    std::vector<double> Alpha(S.size(), 0.0);

    // Method 1
    // Here we use the formula in wikipedia: https://en.wikipedia.org/wiki/Limited-memory_BFGS,
    // which is different from the formula present in the course slides.
    auto q = delta;
    // limited-memory BFGS update, do not store B explicitly
    for (int i = S.size() - 1; i >= 0; --i) {
      Alpha[i] = RHO[i] * S[i].transpose() * q;
      q = q - Alpha[i] * Y[i];
    }
    auto gamma = static_cast<double>(S[S.size() - 1].transpose() * Y[S.size() - 1]) /
                 (static_cast<double>(Y[S.size() - 1].transpose() * Y[S.size() - 1]) + 1e-6);
    Eigen::MatrixXd const H0 = gamma * Eigen::MatrixXd::Identity(n, n);
    Eigen::VectorXd z = H0 * q;
    for (int i = 0; i < S.size(); ++i) {
      auto beta = RHO[i] * Y[i].transpose() * z;
      z = z + S[i] * (Alpha[i] - beta);
    }
    d = -z;

    // Method 2
    // Here we use the formula in the course slides, which does not store
    // B explicitly.
    // d = delta;
    // for (int i = 0; i < S.size(); ++i) {
    //   double alpha = RHO[i] * S[i].transpose() * d;
    //   d = d - alpha * Y[i];
    // }
    // double gamma = RHO[RHO.size() - 1] * Y[Y.size() - 1].transpose() * Y[Y.size() - 1];
    // d = d / gamma;
    // for (int i = S.size() - 1; i >= 0; --i) {
    //   double beta = RHO[i] * Y[i].transpose() * d;
    //   d = d + S[i] * (Alpha[i] - beta);
    // }
    // d = -d;

    // Method 3
    // Here we calculate B explicitly, and then use the formula in the course slides.
    // B = Eigen::MatrixXd::Identity(n, n);
    // for (int i = 0; i < S.size(); ++i) {
    //   updateHessian(S[i], Y[i]);
    // }
    // // update delta
    // d = -B * delta;
    log(x, f(x), delta, iter, options_.t, options_.c, options_.sigma);
    // std::cout << "Current point: " << x.transpose() << std::endl;
    iter++;
  }
  x_ = x;
}
