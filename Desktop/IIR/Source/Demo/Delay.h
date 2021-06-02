#pragma once

#include <JuceHeader.h>

#include "../IAudioProcessor.h"
#include "../lib/ht_include.h"

class Delay : public IAudioProcessor {
 public:
  Delay();
  ~Delay();
  void audioInit(const float *knob, float sampleRate);
  void audioProcess(const float *inDataL, const float *inDataR, float *outDataL,
                    float *outDataR, int frameSize, const float *knob);

  static const int PARAM_NUM = 4;

  // clang-format off
  Param param[PARAM_NUM] = {
    {.label = "time", .type = Slider, .range = {10, 1000, 1.0}, .defaultValue = 0},
    {.label = "feedback", .type = Slider, .range = {0.0, 1,0, 0.1}, .defaultValue = 1000},
    {.label = "mix", .type = Slider, .range = {0.0, 1.0, 0.01}, .defaultValue = 0.0707},
    {.label = "level", .type = Slider, .range = {0.00, 1.0, 0.01}, .defaultValue = 0.5},

   
  };
  // clang-format on

  float knobTime, knobTimeOld;
  float knobFeedback, knobFeedbackOld;
  float knobMix, knobMixOld;
  float knobLevel, knobLevelOld;

  // 2 阶 IIR 滤波器
  float buffer[DELAY_LENGTH_MAX + 1];
  int writeIndex;
  int readIndex;
  float delayOut;

  void delay(float in, int length, float feedback);
};
