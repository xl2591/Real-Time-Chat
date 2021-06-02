#ifndef HT_UTIL_H_
#define HT_UTIL_H_

#include "ht_const.h"

/**
 * 生成数据间隔均匀的数组
 *
 * @param start: 起始数
 * @param end: 结尾数
 * @param n: 数组长度
 * @param y[n]: 生成的数组
 *
 * @return void
 */
inline void linspace(float start, float end, int n, float y[]) {
  float step = (end - start) / (float)(n - 1);
  y[0] = start;
  for (int i = 1; i < n; i++) y[i] = y[i - 1] + step;
  y[n - 1] = end;
}

/**
 * 计算 Hamming 窗
 *
 * @param n: 窗长度
 * @param symmetric: true 生成 symmetric 窗，false 生成 periodic 窗
 * @param window[n]: 窗数据
 *
 * @return void
 */
inline void hamming(int n, bool symmetric, float window[]) {
  if (symmetric) {
    // symmetric window for fitler design
    for (size_t i = 0; i < n; i++) {
      window[i] = 0.54f - 0.46f * cosf((TWO_PI * i) / (n - 1));
    }
  } else {
    // periodic window for spectral analysis
    // computes a window of length n + 1 and returns the first n points.
    for (size_t i = 0; i < n; i++) {
      window[i] = 0.54f - 0.46f * cosf((TWO_PI * i) / (n));
    }
  }
}

// 计算 sinc 函数 (输入弧度)
inline float sinc(float t) {
  if (fabsf(t) < 1.0e-7f)
    return 1.0f;
  else
    return sinf(PI * t) / (PI * t);
}

// db_gain 转换到 linear_gain
inline float db2gain(float db_gain) { return powf(10.0f, db_gain * 0.05f); }

// linear_gain 转换到 db_gain
inline float gain2db(float linear_gain) { return 20.0f * log10f(linear_gain); }

// 符号函数
inline float sign(float x) { return (x > 0) ? 1 : -1; }

// 取整，不考虑负数
inline int round2int(float x) { return (int)(x + 0.5); }

// 快速 tanh 算法 (查表)
inline float fastTanh1(float beta) {
  if (beta > 2) {
    return 1;
  } else if (beta <= -2) {
    return -1;
  } else {
    int index = 128 + 64 * beta;
    return FAST_TANH_TABLE[index];
  }
}

// 快速 tanh 算法 (exp)
inline float fastTanh2(float beta) {
  float z = expf(2 * beta);
  return (z - 1) / (z + 1);
}

// 快速 tanh 算法 (级数)
inline float fastTanh3(float beta) {
  if (beta > 4.8) {
    return 1;
  } else if (beta <= -4.8) {
    return -1;
  } else {
    float x2 = beta * beta;
    float a = beta * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
    float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
    return a / b;
  }
}

// 快速 exp 算法 (8阶)
inline float fastExp1(float x) {
  // x = 1 + x / pow(2, 8);
  x = 1 + x * 0.00390625;

  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;

  return x;
}

// 快速 exp 算法 (10阶)
inline float fastExp2(float x) {
  // x = 1 + x / pow(2, 10);
  x = 1 + x * 0.000976562;

  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;
  x *= x;

  return x;
}

#endif
