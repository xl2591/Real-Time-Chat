#pragma once

#include <JuceHeader.h>

#include "../IAudioProcessor.h"
#include "../lib/ht_include.h"

class IIRFilter : public IAudioProcessor {
 public:
  IIRFilter();
  ~IIRFilter();
  void audioInit(const float *knob, float sampleRate);
  void audioProcess(const float *inDataL, const float *inDataR, float *outDataL,
                    float *outDataR, int frameSize, const float *knob);

  static const int PARAM_NUM = 6;

  // clang-format off
  Param param[PARAM_NUM] = {
    {.label = "IIR", .type = ComboBox, .range = {0, 1, 2}, .rangeLabel = {"LP", "HP", "BP", "AP"}, .defaultValue = 0},
    {.label = "Freq", .type = Slider, .range = {20, 20000, 1}, .defaultValue = 1000},
    {.label = "Q", .type = Slider, .range = {0.01, 10, 0.01}, .defaultValue = 0.0707},
    {.label = "Gain", .type = Slider, .range = {0.00, 1.0, 0.01}, .defaultValue = 0.5},

    {.label = "Delay", .type = Slider, .range = {0.00, 1.0, 0.01}, .defaultValue = 0.5},
    {.label = "FIR", .type = ComboBox, .range = {0, 1, 2}, .rangeLabel = {"LP", "HP", "BP", "AP"}, .defaultValue = 0}

  };
  // clang-format on

  float knobType, knobTypeOld;
  float knobFreq, knobFreqOld;
  float knobQ, knobQOld;
  float knobGain, knobGainOld;
  float knobDelay, knobDelayOld;
  // 2 阶 IIR 滤波器
  float b[3], a[2], state[3];
  void initFilterCoeffs();
};
