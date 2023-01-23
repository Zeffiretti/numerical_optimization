// Copyright 2023 HELLION XIE. All rights reserved.
#ifndef MATH_CUBIN_SPLINE_H_
#define MATH_CUBIN_SPLINE_H_

#include <ostream>

#include "third_party/eigen/Eigen/Dense"
#include "third_party/eigen/Eigen/src/Core/Matrix.h"

namespace optim_math {
typedef struct CubicSplineParamType {
  int N;
  Eigen::MatrixXd ai;
  Eigen::MatrixXd bi;
  Eigen::MatrixXd ci;
  Eigen::MatrixXd di;
  double stretch_energy;
  Eigen::MatrixXd inner_gradient;
  friend std::ostream& operator<<(std::ostream& os, const CubicSplineParamType& param) {
    os << "=====Cubic Spline Param BEGIN=====" << std::endl;
    os << "N:\n" << param.N << std::endl;
    os << "-------------------" << std::endl;
    os << "ai:\n" << param.ai << std::endl;
    os << "-------------------" << std::endl;
    os << "bi:\n" << param.bi << std::endl;
    os << "-------------------" << std::endl;
    os << "ci:\n" << param.ci << std::endl;
    os << "-------------------" << std::endl;
    os << "di:\n" << param.di << std::endl;
    os << "-------------------" << std::endl;
    os << "stretch_energy:\n" << param.stretch_energy << std::endl;
    os << "-------------------" << std::endl;
    os << "inner_gradient:\n" << param.inner_gradient << std::endl;
    os << "=====Cubic Spline Param END=====" << std::endl;
    return os;
  }
} CubicSplineParam;

// Cubic Spline Param Calculation
// parameters: input x, output param
// x: all key points of the cubic spline, including initial and terminal points, with the shape of (N+1, 2)
//    N is the number of segments. Each row is a point with the shape of (1, 2)
// param: the cubic spline parameters
void calcCubicSplineParam(const Eigen::MatrixXd& x, CubicSplineParam* param);

// Cubic Spline Points Calculation
// parameters: input param, output x
// param: the cubic spline parameters
// x: the calculated points of the cubic spline, with the shape of (100*N, 2). N is the number of segments.
//    Each row is a point with the shape of (1, 2)
void calcCubicSplinePoints(const CubicSplineParam& param, Eigen::MatrixXd* x);

}  // namespace optim_math

#endif  // MATH_CUBIN_SPLINE_H_
