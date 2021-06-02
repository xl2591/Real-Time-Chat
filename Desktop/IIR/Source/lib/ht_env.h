#include <math.h>

#include "ht_util.h"

typedef struct Env {
  float attackCoeff;
  float releaseCoeff;
  float prevEnv;
} Env;

/**
 * 计算包络 attack 时间
 *
 * @param self: Env 结构体
 * @param attackTimeMS: attack 时间 (ms)
 * @param fs: 采样率
 *
 * @return void
 */
inline void envInitAttack(Env* self, float attackTimeMs, float fs) {
  self->attackCoeff = fastExp1(-1.f / (fs * attackTimeMs * 0.001f));
}

/**
 * 计算包络 release 时间
 *
 * @param self: Env 结构体
 * @param releaseTimeMs: release 时间 (ms)
 * @param fs: 采样率
 *
 * @return void
 */
inline void envInitRelease(Env* self, float releaseTimeMs, float fs) {
  self->releaseCoeff = fastExp1(-1.f / (fs * releaseTimeMs * 0.001f));
}

/**
 * 包络初始化
 *
 * @param self: Env 结构体
 * @param attackTimeMs: attack 时间 (ms)
 * @param releaseTimeMs: release 时间 (ms)
 * @param fs: 采样率
 *
 * @return void
 */
inline void envInit(Env* self, float attackTimeMs, float releaseTimeMs,
                    float fs) {
  envInitAttack(self, attackTimeMs, fs);
  envInitRelease(self, releaseTimeMs, fs);
  self->prevEnv = 0.f;
}

/**
 * 包络处理
 *
 * @param self: Env 结构体
 * @param input: 输入信号
 *
 * @return 包络信号 [0, 1]
 */
inline float envProecess(Env* self, float input) {
  float envOut;

  // calculate envelope value
  float envIn = fabsf(input);
  if (self->prevEnv < envIn) {
    envOut = envIn + self->attackCoeff * (self->prevEnv - envIn);
  } else {
    envOut = envIn + self->releaseCoeff * (self->prevEnv - envIn);
  }

  // save current envelope value
  self->prevEnv = envOut;

  if (envOut > 1) envOut = 1;
  if (envOut < 0) envOut = 0;

  return envOut;
}

inline void initEnvFilterCoeff(float ta, float fs, float b_coeffs[],
                               float a_coeffs[]) {
  // float a = expf(-log(9) / (fs * ta));
  float a = fastExp1(-2.197224577 / (fs * ta));

  b_coeffs[0] = (1 - a) * (1 - a);
  b_coeffs[1] = 0;
  b_coeffs[2] = 0;
  a_coeffs[0] = -2 * a;
  a_coeffs[1] = a * a;
}
