#ifndef HT_FX_H_
#define HT_FX_H_

#include "ht_const.h"
#include "ht_util.h"

// 干湿混合
inline float mix(float dry, float wet, float mix) {
  float output;

  if (mix < 0.5f) {
    output = dry + wet * (mix * 2);
  } else {
    output = dry * ((1 - mix) * 2) + wet;
  }

  return output;
}

// 旋钮渐变
inline float knobRotate(float prev, float cur) {
  if (prev != cur) {
    if (cur > prev) {
      prev = prev + 0.0001f;
      if (prev >= cur) {
        prev = cur;
      }
    } else {
      prev = prev - 0.0001f;
      if (prev <= cur) {
        prev = cur;
      }
    }
  } else {
    prev = cur;
  }

  return prev;
}

// 旋钮渐变
inline float knobRotateDelta(float prev, float cur, float delta) {
  if (prev != cur) {
    if (cur > prev) {
      prev = prev + delta;
      if (prev >= cur) {
        prev = cur;
      }
    } else {
      prev = prev - delta;
      if (prev <= cur) {
        prev = cur;
      }
    }
  } else {
    prev = cur;
  }

  return prev;
}

// 计算 LOG30A 电位器曲线
inline float getLog30A(float x) {
  float y;

  if (x < 0.75) {
    y = 0.5793 * x;
  } else {
    y = 0.4345 + 2.2620 * (x - 0.75);
  }

  return y;
}

/**
 * 音序器
 *
 * @param rate: 速率，[0.1Hz, 10Hz]
 * @param stepNum: step 个数
 * @param stepArr: step 数组，归一化，长度必须大于等于 stepNum
 * @param j: 计数器指针
 *
 * @return res: 归一化的音序器值
 */
inline float sequencer(float rate, int stepNum, const float* stepArr, float* j,
                       float fs) {
  float res = 0;

  // 计算周期
  float T = (1.f / rate) * fs;

  // 每个周期，换一个 step 值
  for (int i = 0; i < stepNum; i++) {
    if (*j >= i * T && *j < (i + 1) * T) {
      res = stepArr[i];
      break;
    }
  }

  (*j)++;
  if (*j >= stepNum * T) *j = 0;

  return res;
}

/**
 * 计算 LFO 频率
 *
 * @param knobValue: Rate 旋钮值，[0, 1]
 * @param isSync: 是否开启同步, 0 或 1
 * @param tapTempo: tapTempo 对应的毫秒（ms）值
 *
 * @return rate: LFO Rate
 */
inline float getLFORate(float knobValue, int isSync, int tapTempo) {
  float rate;

  if (isSync == 1) {
    // sync 开启，rate 旋钮为 Subdivision
    // rate 旋钮值转化：[0, 1] -> [0, 10]
    int tempoIndex = (int)(knobValue * 10);
    // T = tapTempo * 0.001 * multiple;
    rate = 1 / (tapTempo * 0.001f * TEMPO_MAP[tempoIndex]);
  } else {
    // sync 关闭，rate 旋钮为频率
    rate = LFO_RATE_MIN + (LFO_RATE_MAX - LFO_RATE_MIN) * knobValue;
  }

  return rate;
}

/**
 * 计算 Sine LFO
 *
 * @param rate: Rate 值, [0.1Hz, 10Hz]
 * @param phaseIndex: LFO 相位差，[0, 1, 2, 3, 4] map to [0, 0.25, 0.5, 0.75, 1]
 * @param j: LFO 计数器，需要初始化
 *
 * @return LFO: 归一化 LFO 值
 */
inline float sinLFOProcess(float rate, int phaseIndex, float* j, float fs) {
  // phase
  float phaseMap[5] = {
      0,         // 0 * pi
      0.785398,  // 1/4 * pi
      1.570796,  // 2/4 * pi
      2.356194,  // 3/4 * pi
      3.141593   // 4/4 * pi
  };
  float phase = phaseMap[phaseIndex];

  // period
  rate = rate / fs;
  float T = (1 / rate);

  // Sine
  float lfo = sinf(2 * PI * rate * (*j) - phase);

  // LFO counter
  (*j)++;
  if (*j >= T) *j = 0;

  // LFO normalization
  return (lfo + 1) * 0.5;
}

/**
 * 计算延时线长度
 *
 * @param knobValue: Rate 旋钮值，[0, 1]
 * @param isSync: 是否开启同步, 0 或 1
 * @param tapTempo: tapTempo 对应的毫秒（ms）值
 *
 * @return length: 延时线长度
 */
inline float getDelayLength(float knobValue, int isSync, int tapTempo,
                            float fs) {
  float length;

  if (isSync == 1) {
    // sync 开启，time 旋钮为 Subdivision
    // rate 旋钮值转化：[0, 1] -> [0, 10]
    int tempoIndex = (int)(knobValue * 10);
    // T = tapTempo * 0.001 * multiple;
    length = tapTempo * 0.001f * TEMPO_MAP[tempoIndex] * fs;

    // 超出范围
    if (length > DELAY_LENGTH_MAX) length = DELAY_LENGTH_MAX;
    if (length < DELAY_LENGTH_MIN) length = DELAY_LENGTH_MIN;
  } else {
    // sync 关闭，time 旋钮为时间
    length =
        DELAY_LENGTH_MIN + (DELAY_LENGTH_MAX - DELAY_LENGTH_MIN) * knobValue;
  }

  return length;
}

/**
 * 延时线
 *
 * @param input: 输入信号
 * @param length: 延时线长度，time * fs
 * @param maxLength: 延时线最大长度
 * @param buffer: buffer 数组
 * @param writeIndex: 写指针
 * @param readIndex: 读指针
 * @param i: 标识符
 *
 * @return output: 输出信号
 */
inline float delayline(float input, float length, int maxLength, float buffer[],
                       int* writeIndex, int* readIndex) {
  // 延迟线长度的整数和小数部分
  int integer = (int)length;
  float frac = length - integer;

  // 输入信号写入 buffer
  buffer[(*writeIndex)++] = input;

  // 判断写指针位置
  if (*writeIndex > maxLength - 1) *writeIndex = 0;

  // 计算读取的位置
  *readIndex = *writeIndex - integer;

  // 判断读指针位置
  if (*readIndex < 0) *readIndex += maxLength;

  // 读取输出
  float currOut = buffer[*readIndex];

  // 读取上一个输出
  *readIndex -= 1;

  // 再次判断读指针位置
  if (*readIndex < 0) *readIndex += maxLength;

  // buffer读取输出
  float prevOut = buffer[*readIndex];

  // 线性差值
  return prevOut * frac + currOut * (1 - frac);
}

/**
 * 按延时线长度比例(ratio) 计算延迟线反馈(feedback)
 *
 * @param feedback: 延迟 feedback
 * @param ratio: 延迟长度比例
 *
 * @return target: 期望 feedback
 */
inline float feedbackRatio(float feedback, float ratio) {
  float target;
  // -4.931 为拟合结果(R-square: 0.9988)
  // -4 为听感上的调试结果
  float a = -4;

  if (ratio == 1) {
    target = feedback;
  } else {
    target = feedback + (1 - feedback) * expf(a * ratio * ratio);
  }

  return target;
}

#endif
