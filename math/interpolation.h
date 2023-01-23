// Copyright 2023 HELLION XIE. All rights reserved.
// Copyright 2022 Hellion Xie. All rights reserved.
#ifndef MATH_INTERPOLATION_H_
#define MATH_INTERPOLATION_H_

#include <type_traits>

namespace Eigen {
template <typename T>
class MatrixBase;
}

namespace math {

// Linearly interpolates from |start| and |end|. t ∈ [0, 1].
// |T| can be Eigen types.
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
T LinearInterpolate(T start, T end, double t) {
  return start + (end - start) * t;
}

template <typename T1, typename T2>
auto LinearInterpolate(const Eigen::MatrixBase<T1>& start, const Eigen::MatrixBase<T2>& end, double t) {
  return start + (end - start) * t;
}

// Interpolates on the line from |start| to |end| with zero velocity at endpoints.
// Returns the position at time |phase| in |period| during the movement.
// |period| must be > 0. |phase| must be in [0, |period|].
//
// Formulated as:
//   P(t) = P_start + (P_end - P_start) * r(t), t ∈ [0, 1],
//   r(t) = a t³ + b t² + c t + d.
// Constraints: r(0) = 0, r(1) = 1, r'(0) = 0, r'(1) = 0.
// Solved as a = -2, b = 3, c = 0, d = 0.
template <typename T>
void CubicInterpolate(const T& start, const T& end, double period, double phase, T& pos) {
  double t = phase / period;
  double r = (-2 * t + 3) * t * t;
  pos = start + (end - start) * r;
}

// Also returns the velocity at time |phase| in |period|.
template <typename T>
void CubicInterpolate(const T& start, const T& end, double period, double phase, T& pos, T& vel) {
  double t = phase / period;
  double r = (-2 * t + 3) * t * t;
  double rd = (-6 * t + 6) * t;
  pos = start + (end - start) * r;
  vel = (end - start) * (rd / period);
}

// Also returns the acceleration.
template <typename T>
void CubicInterpolate(const T& start, const T& end, double period, double phase, T& pos, T& vel, T& acc) {
  double t = phase / period;
  double r = (-2 * t + 3) * t * t;
  double rd = (-6 * t + 6) * t;
  double rdd = -12 * t + 6;
  pos = start + (end - start) * r;
  vel = (end - start) * (rd / period);
  acc = (end - start) * (rdd / period / period);
}

// Interpolates on the line from |start| to |end| with zero velocity and acceleration at endpoints.
// Returns the position at time |phase| in |period| during the movement.
// |period| must be > 0. |phase| must be in [0, |period|].
//
// Formulated as:
//   P(t) = P_start + (P_end - P_start) * r(t), |t| ∈ [0, 1],
//   r(t) = a t⁵ + b t⁴ + c t³ + d t² + e t + f.
// Constraints: r(0) = 0, r(1) = 1, r'(0) = 0, r'(1) = 0, r''(0) = 0, r''(1) = 0.
// Solved as: a = 6, b = -15, c = 10, d = e = f = 0.
template <typename T>
void QuinticInterpolate(const T& start, const T& end, double period, double phase, T& pos) {
  double t = phase / period;
  double r = ((6 * t - 15) * t + 10) * t * t * t;
  pos = start + (end - start) * r;
}

// Also returns the velocity at time |phase| in |period|.
template <typename T>
void QuinticInterpolate(const T& start, const T& end, double period, double phase, T& pos, T& vel) {
  double t = phase / period;
  double r = ((6 * t - 15) * t + 10) * t * t * t;
  double rd = ((30 * t - 60) * t + 30) * t * t;
  pos = start + (end - start) * r;
  vel = (end - start) * (rd / period);
}

// Also returns the acceleration.
template <typename T>
void QuinticInterpolate(const T& start, const T& end, double period, double phase, T& pos, T& vel, T& acc) {
  double t = phase / period;
  double r = ((6 * t - 15) * t + 10) * t * t * t;
  double rd = ((30 * t - 60) * t + 30) * t * t;
  double rdd = ((120 * t - 180) * t + 60) * t;
  pos = start + (end - start) * r;
  vel = (end - start) * (rd / period);
  acc = (end - start) * (rdd / period / period);
}

}  // namespace math
#endif  // MATH_INTERPOLATION_H_
