#pragma once

#include <JuceHeader.h>

#include "../IAudioProcessor.h"
#include "../lib/ht_include.h"

class TenTapDelay : public IAudioProcessor {
 public:
  TenTapDelay();
  ~TenTapDelay();
  void audioInit(const float *knob, float sampleRate);
  void audioProcess(const float *inDataL, const float *inDataR, float *outDataL,
                    float *outDataR, int frameSize, const float *knob);

  static const int PARAM_NUM = 9;

  // clang-format off
  Param param[PARAM_NUM] = {
   {.label = "Time L", .type = Slider, .range = {10, 1000, 1.0}, .defaultValue = 500},
   {.label = "Feedback L", .type = Slider, .range = {0.0, 1.0, 0.01}, .defaultValue = 0.3},
   {.label = "Time R", .type = Slider, .range = {10, 1000, 1.0}, .defaultValue = 500},
   {.label = "Feedback R", .type = Slider, .range = {0.0, 1.0, 0.01}, .defaultValue = 0.3},
   {.label = "Mix", .type = Slider, .range = {0.0, 1.0, 0.01}, .defaultValue = 0.5},
   {.label = "Level", .type = Slider, .range = {0.0, 1.0, 0.01}, .defaultValue = 0.5},
   {.label = "Change", .type = ComboBox, .range = {0, 1},.rangeLabel={"Stereo","Mono"},.defaultValue = 0},
   {.label = "Tap", .type = Slider, .range = {0.0, 10.0, 1.00}, .defaultValue = 0.0},
   {.label = "Pan Alpha", .type = Slider, .range = {-1.0, 1.0, 0.00}, .defaultValue = 0.0},

   
  };
  // clang-format on

  float knobTimeL, knobTimeLOld;
  float knobFeedbackL, knobFeedbackLOld;
  float knobTimeR, knobTimeROld;
  float knobFeedbackR, knobFeedbackROld;
  float knobMix, knobMixOld;
  float knobLevel, knobLevelOld;
  float knobChange, knobChangeOld;
  float knobTap, knobTapOld;
  float knobPan, knobPanOld;

  // 2 阶 IIR 滤波器
  static const int DELAY_NUM = 2;
  static const int TAPS_NUM = 10;
  static const int TAPS_DELAY_LENGTH_MAX = DELAY_LENGTH_MAX * TAPS_NUM;
  float length[DELAY_NUM], lengthOld[DELAY_NUM];
  float delayBuffer[DELAY_NUM][TAPS_DELAY_LENGTH_MAX + 1];
  int delayWriteIndex[DELAY_NUM];
  int delayReadIndex[TAPS_NUM];
  float delayOut[DELAY_NUM];

  float delay(float input, int length, float buffer[], int *writeIndex,
              int *readIndex, int times);
  float Tap(float input, float length, float buffer[], int *writeIndex,
            int readIndex[]);
};
