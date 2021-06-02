#ifndef HT_NONLINEAR_H_
#define HT_NONLINEAR_H_

#include "ht_const.h"
#include "ht_util.h"

/**
 * 指数非线性曲线
 *
 * @param upLimit: 正半轴限幅（正数）
 * @param downLimit: 负半轴限幅（正数）
 * @param upCurvature: 正半轴曲率（正数）
 * @param downCurvature: 负半轴曲率（正数）
 * @param x: 输入信号
 *
 * @return 输出信号
 */
inline float expCurve(float upLimit, float downLimit, float upCurvature,
                      float downCurvature, float x) {
  float y;

  if (x > 0) {
    y = upLimit * (1 - expf(-upCurvature * x));
  } else {
    y = -downLimit * (1 - expf(downCurvature * x));
  }

  return y;
}

/**
 * 过采样
 *
 * @param x: 当前信号
 * @param *x_prev: 上一点的信号(地址)
 * @param osr: 过采样倍数
 * @param y[osr]: 过采样信号
 *
 * @return void
 */
inline void upsampling(float x, float* x_prev, int osr, float y[]) {
  // 线性插值
  linspace(*x_prev, x, osr, y);
  // 保存上一点数据
  *x_prev = x;
}

/**
 * 降采样 + fir 抗混叠
 *
 * @param x[osr]: 过采样的输入信号
 * @param osr: 过采样倍数
 * @param coeffs[taps+1]: 滤波器系数
 * @param state[taps+1]: 滤波器状态
 * @param taps: 滤波器阶数
 *
 * @return 输出信号
 */
inline float downsampling(const float x[], int osr, const float coeffs[],
                          float state[], int taps) {
  // 初始化累加变量
  float output = 0.f;

  // 反向写入新数据
  for (int i = 0; i < osr; i++) state[osr - 1 - i] = x[i];
  // 计算 fir (state 反向)
  // ! 这里不能用 size_t，因为 size_t 是无符号的，永远不会小于 0
  for (int i = taps; i >= 0; i--) output += coeffs[i] * state[i];
  // state 后移 osr 格
  for (int i = taps; i >= osr; i--) state[i] = state[i - osr];

  return output;
}

#endif
