#ifndef HT_CONST_H_
#define HT_CONST_H_

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PI (3.141592654f)                // pi
#define ONE_OVER_PI (0.3183098862f)      // 1 / pi
#define TWO_PI (6.283185307f)            // 2 * pi
#define FS (44100.f)                     // 采样率
#define ONE_OVER_FS (0.00002267573696f)  // 1 / 采样率
#define ONE_OVER_100 (0.01f)             // 1 / 100
#define ONE_OVER_255 (0.003921569f)      // 1 / 255
#define LIMIT_VALUE (2.5f)               // 最大限幅幅度
#define LFO_RATE_MIN (0.1f)              // 最小 LFO 频率
#define LFO_RATE_MAX (10.f)              // 最大 LFO 频率
#define DELAY_TIME_MIN (20e-3)           // 最小延时时间 20ms
#define DELAY_TIME_MAX (1000e-3)         // 最大延时时间 1000ms
#define DELAY_LENGTH_MIN (882)           // 最小延时线长度 20ms
#define DELAY_LENGTH_MAX (44100)         // 最大延时线长度 1000ms
#define OVERSAMPLING_MAX (16)            // 最大过采样倍数
// #define AA_TAPS (49)                     // 抗混叠 fir 滤波器阶数
// #define AA_FC (20000)                    // 抗混叠 fir 滤波器截止频率
// FAS
#define AA_TAPS (149)  // 抗混叠 fir 滤波器阶数
#define AA_FC (19000)  // 抗混叠 fir 滤波器截止频率

extern const float d100;

// Tap Tempo 映射
extern const float TEMPO_MAP[11];

// 旋钮速率表
extern const float pow0_1[101];
extern const float pow0_2[101];
extern const float pow0_3[101];
extern const float pow0_4[101];
extern const float pow0_5[101];
extern const float pow0_7[101];
extern const float pow1[101];
extern const float pow1_3[101];
extern const float pow1_5[101];
extern const float pow2[101];
extern const float pow2_5[101];
extern const float pow3[101];
extern const float pow3_5[101];
extern const float pow4[101];
extern const float pow4_5[101];
extern const float pow5[101];
extern const float pow6[101];

// 抗混叠 FIR 滤波器系数
extern const float coeffAntiAliasing2x15k[40];
extern const float coeffAntiAliasing2x18k[40];
extern const float coeffAntiAliasing4x15k[40];
extern const float coeffAntiAliasing4x18k[40];

extern const float FAST_TANH_TABLE[256];

#endif
