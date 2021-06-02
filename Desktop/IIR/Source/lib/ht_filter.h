#ifndef HT_FILTER_H_
#define HT_FILTER_H_

#include "ht_const.h"

/**
 * 有限脉冲响应滤波器 (FIR)
 *
 * @param sample: 输入信号
 * @param coeffs[n]: 滤波器系数（反向）
 * @param state[n+1]: 滤波器状态
 * @param n: 滤波器系数长度，n = taps+1
 *
 * @return 输出信号
 */
inline float fir(float sample, const float coeffs[], float state[], int n) {
  // https://ccrma.stanford.edu/~jos/filters/Convolution_Representation_FIR_Filters.html
  // FIR 滤波器差分方程：
  // y[n] = b[0]*x[n] + b[1]*x[n-1] + b[2]*x[n-2] +... + b[M]*x[n-M]
  // 由于 b 是反向的：
  // y[n] = b[n]*x[n] + b[n-1]*x[n-1] + b[n-2]*x[n-2] +... + b[0]*x[0]
  // 而且 x 长度比 b 多 1：
  // y[n] = b[n]*x[n+1] + b[n-1]*x[n] + b[n-2]*x[n-1] +... + b[0]*x[1]

  // 初始化累加变量
  float output = 0.f;

  // 写入新数据
  state[n] = sample;

  // FIR 差分方程
  for (size_t i = 0; i < n; i++) {
    output += coeffs[i] * state[i + 1];
    // state 前移一格
    state[i] = state[i + 1];
  }

  return output;
}

/**
 * Direct-Form II 型，无限脉冲响应滤波器 (IIR)
 *
 * @param sample: 输入信号
 * @param b_coeffs[taps+1]: 滤波器分子系数
 * @param a_coeffs[taps]: 滤波器分母系数
 * @param state[taps+1]: 滤波器状态
 * @param taps: 滤波器阶数
 *
 * @return 输出信号
 */
inline float iir2(float sample, const float b_coeffs[], const float a_coeffs[],
                  float state[], int taps) {
  // https://ccrma.stanford.edu/~jos/filters/Direct_Form_II.html
  // Direct-Form II 型 IIR 滤波器差分方程：
  // v[n] = x[n] - a[1]*v[n-1] - a[2]*v[n-2]
  // y[n] = b[0]*v[n] + b[1]*v[n-1] + b[2]*v[n-2]
  // v 反向：
  // v[0] = x[n] - a[1]*v[1] - a[2]*v[2]
  // y[0] = b[0]*v[0] + b[1]*v[1] + b[2]*v[2]
  // 由于省略 a[0]，因此将 a 整体前移一格：
  // v[0] = x[n] - a[0]*v[1] - a[1]*v[2]
  // y[0] = b[0]*v[0] + b[1]*v[1] + b[2]*v[2]

  // 初始化累加变量
  float output = 0.f;

  // IIR 差分方程
  switch (taps) {
    // clang-format off
    case 1:
      state[0] = sample - a_coeffs[0] * state[1];
      output = b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[1] = state[0];
      break;
    case 2:
      state[0] = sample - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[2] = state[1]; state[1] = state[0]; 
      break;
    case 3:
      state[0] = sample - a_coeffs[2] * state[3] - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 4:
      state[0] = sample - a_coeffs[3] * state[4] - a_coeffs[2] * state[3] - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 5:
      state[0] = sample - a_coeffs[4] * state[5] - a_coeffs[3] * state[4] - a_coeffs[2] * state[3] - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 6:
      state[0] = sample - a_coeffs[5] * state[6] - a_coeffs[4] * state[5] - a_coeffs[3] * state[4] - a_coeffs[2] * state[3] - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 7:
      state[0] = sample - a_coeffs[6] * state[7] - a_coeffs[5] * state[6] - a_coeffs[4] * state[5] - a_coeffs[3] * state[4] - a_coeffs[2] * state[3] - a_coeffs[1] * state[2] - a_coeffs[0] * state[1];
      output = b_coeffs[7] * state[7] + b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[7] = state[6]; state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    default:
      break;
      // clang-format on
  }

  return output;
}

/**
 * Direct-Form II 型，无限脉冲响应滤波器 (IIR)，未归一化系数
 *
 * @param sample: 输入信号
 * @param b_coeffs[taps+1]: 滤波器分子系数
 * @param a_coeffs[taps+1]: 滤波器分母系数
 * @param state[taps+1]: 滤波器状态
 * @param taps: 滤波器阶数
 *
 * @return 输出信号
 */
inline float iir2va(float sample, const float b_coeffs[],
                    const float a_coeffs[], float state[], int taps) {
  // 初始化累加变量
  float output = 0.f;

  // IIR 差分方程
  switch (taps) {
    // clang-format off
    case 1:
      state[0] = sample - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[1] = state[0];
      break;
    case 2:
      state[0] = sample - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[2] = state[1]; state[1] = state[0]; 
      break;
    case 3:
      state[0] = sample - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 4:
      state[0] = sample - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 5:
      state[0] = sample - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 6:
      state[0] = sample - a_coeffs[6] * state[6] - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 7:
      state[0] = sample - a_coeffs[7] * state[7] - a_coeffs[6] * state[6] - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[7] * state[7] + b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[7] = state[6]; state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    default:
      break;
      // clang-format on
  }

  return output;
}

/**
 * Direct-Form II 型，无限脉冲响应滤波器 (IIR)，未归一化系数
 *
 * @param sample: 输入信号
 * @param b_coeffs[taps+1]: 滤波器分子系数
 * @param a_coeffs[taps+1]: 滤波器分母系数
 * @param state[taps+1]: 滤波器状态
 * @param taps: 滤波器阶数
 *
 * @return 输出信号
 */
inline double iir2vad(double sample, const double b_coeffs[],
                      const double a_coeffs[], double state[], int taps) {
  // 初始化累加变量
  double output = 0.0;

  // IIR 差分方程
  switch (taps) {
    // clang-format off
    case 1:
      state[0] = sample - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[1] = state[0];
      break;
    case 2:
      state[0] = sample - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[2] = state[1]; state[1] = state[0]; 
      break;
    case 3:
      state[0] = sample - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 4:
      state[0] = sample - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 5:
      state[0] = sample - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 6:
      state[0] = sample - a_coeffs[6] * state[6] - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    case 7:
      state[0] = sample - a_coeffs[7] * state[7] - a_coeffs[6] * state[6] - a_coeffs[5] * state[5] - a_coeffs[4] * state[4] - a_coeffs[3] * state[3] - a_coeffs[2] * state[2] - a_coeffs[1] * state[1];
      state[0] /= a_coeffs[0];
      output = b_coeffs[7] * state[7] + b_coeffs[6] * state[6] + b_coeffs[5] * state[5] + b_coeffs[4] * state[4] + b_coeffs[3] * state[3] + b_coeffs[2] * state[2] + b_coeffs[1] * state[1] + b_coeffs[0] * state[0];
      state[7] = state[6]; state[6] = state[5]; state[5] = state[4]; state[4] = state[3]; state[3] = state[2]; state[2] = state[1]; state[1] = state[0]; 
      break;
    default:
      break;
      // clang-format on
  }

  return output;
}

#endif