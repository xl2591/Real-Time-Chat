#ifndef HT_FILTER_DESIGN_
#define HT_FILTER_DESIGN_

#include "ht_const.h"
#include "ht_util.h"

// references：
// 1. DAFX: Digital Audio Effects - Second Edition，page [49-50]
// 2. 《Cookbook formulae for audio equalizer biquad filter coefficients》
// https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

/**
 * 计算一阶低通滤波器系数
 *
 * @param fc: 中心频率
 * @param fs: 采样率
 * @param b_coeffs[2]: 滤波器分子系数
 * @param a_coeffs[1]: 滤波器分母系数
 *
 * @return void
 */
void lowpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算一阶高通滤波器系数
 *
 * @param fc: 中心频率
 * @param fs: 采样率
 * @param b_coeffs[2]: 滤波器分子系数
 * @param a_coeffs[1]: 滤波器分母系数
 *
 * @return void
 */
void highpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算一阶全通滤波器系数
 *
 * @param fc: 中心频率
 * @param fs: 采样率
 * @param b_coeffs[2]: 滤波器分子系数
 * @param a_coeffs[1]: 滤波器分母系数
 *
 * @return void
 */
void allpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶低通滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void lowpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶高通滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void highpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶全通滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void allpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶带通滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void bandpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶带阻滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void bandreject(float fc, float Q, float fs, float b_coeffs[],
                float a_coeffs[]);

/**
 * 计算二阶峰值滤波器系数（变化 Q）
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param db_gain: dB 增益值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void peaking(float fc, float Q, float db_gain, float fs, float b_coeffs[],
             float a_coeffs[]);

/**
 * 计算二阶峰值滤波器系数（固定 Q）
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param db_gain: dB 增益值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void peaking_const_q(float fc, float Q, float db_gain, float fs,
                     float b_coeffs[], float a_coeffs[]);

/**
 * 计算二阶低架滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param db_gain: dB 增益值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void lowshelf(float fc, float Q, float db_gain, float fs, float b_coeffs[],
              float a_coeffs[]);

/**
 * 计算二阶高架滤波器系数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param db_gain: dB 增益值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void highshelf(float fc, float Q, float db_gain, float fs, float b_coeffs[],
               float a_coeffs[]);

/**
 * 计算二阶倾斜滤波器系数
 *
 * @param fc_low: 低频中心频率
 * @param db_gain_low: 低频 dB 增益值
 * @param fc_high: 高频中心频率
 * @param db_gain_high: 高频 dB 增益值
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void tilt(float fc_low, float db_gain_low, float fc_high, float db_gain_high,
          float fs, float b_coeffs[], float a_coeffs[]);

/**
 * 为二阶滤波器添加一个 line_gain 参数
 *
 * @param fc: 中心频率
 * @param Q: Q 值
 * @param line_gain: 线性增益
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 * @param filter: 计算滤波器系数的函数指针
 *
 * @return void
 */
void gain_decorator(float fc, float Q, float line_gain, float fs,
                    float b_coeffs[], float a_coeffs[],
                    void (*filter)(float, float, float, float[], float[]));

/**
 * 计算 wah 滤波器系数
 *
 * @param control: 归一化的控制信号，[0, 1]
 * @param freq_min: 最小中心频率, [100Hz, 1000Hz]
 * @param freq_max: 最大中心频率 [500Hz, 5000Hz]
 * @param Q: Q 值，[1, 10]
 * @param tracking: Q 随 control 的变化比例, [0, 1]
 * @param fs: 采样率
 * @param b_coeffs[3]: 滤波器分子系数
 * @param a_coeffs[2]: 滤波器分母系数
 *
 * @return void
 */
void wah_filter(float control, float freq_min, float freq_max, float Q,
                float tracking, float fs, float *b_coeffs, float *a_coeffs);

/**
 * 计算 fir 低通滤波器系数
 *
 * @param fc: 中心频率
 * @param fs: 采样率
 * @param taps: 滤波器阶数
 * @param window[taps+1]: 窗数据
 * @param coeffs[taps+1]: 滤波器系数
 *
 * @return void
 */
void fir_lowpass(float fc, float fs, int taps, float window[], float coeffs[]);

#endif
