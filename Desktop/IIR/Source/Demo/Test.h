#pragma once

#include <JuceHeader.h>

#include "../IAudioProcessor.h"

class Test : public IAudioProcessor {
 public:
  Test();
  ~Test();
  void audioInit(const float *knob, float sampleRate);
  void audioProcess(const float *inDataL, const float *inDataR, float *outDataL,
                    float *outDataR, int frameSize, const float *knob);

  static const int PARAM_NUM = 3;

  // clang-format off
  Param param[PARAM_NUM] = {
    {.label = "Level", .type = Slider, .range = {0, 100, 1}, .defaultValue = 50},
    {.label = "Pan", .type = Slider, .range = {0, 100, 1}, .defaultValue = 50},
    {.label = "Mute", .type = ComboBox, .range = {0, 1}, .rangeLabel={"OFF", "ON"}, .defaultValue = 0},
  };
  // clang-format on

  float knobLevel, knobLevelOld;
  float knobPan, knobPanOld;
  float knobMute, knobMuteOld;
};
