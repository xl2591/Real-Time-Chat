#pragma once

#include <JuceHeader.h>

const int MAX_PARAM_NUM = 50;

typedef juce::String String;
typedef juce::Array<float> FloatArray;
typedef juce::Array<juce::String> StringArray;

enum ParamType {
  Slider,
  ComboBox,
};

typedef struct Param {
  String label;
  ParamType type;
  FloatArray range;
  StringArray rangeLabel;
  float defaultValue;
} Param;

// ui样式更改 可以加一个

class IAudioProcessor {
 public:
  IAudioProcessor(){};
  virtual ~IAudioProcessor(){};

  virtual void audioInit(const float *knob, float sampleRate) = 0;
  virtual void audioProcess(const float *inDataL, const float *inDataR,
                            float *outDataL, float *outDataR, int frameSize,
                            const float *knob) = 0;

 private:
  int _paramNum;
  Param *_param;

 public:
  float *knob;

  void initParam(int paramNum, const Param *p) {
    jassert(paramNum <= MAX_PARAM_NUM);
    _paramNum = paramNum;
    knob = (float *)malloc(paramNum * sizeof(float));
    _param = (Param *)malloc(paramNum * sizeof(Param));
    memcpy(&_param, &p, sizeof(p));
  };

  int getParamNum() { return _paramNum; }

  String getParamLabel(int index) {
    jassert(index < _paramNum);
    return _param[index].label;
  }

  ParamType getParamType(int index) {
    jassert(index < _paramNum);
    return _param[index].type;
  }

  FloatArray getParamRange(int index) {
    jassert(index < _paramNum);
    return _param[index].range;
  }

  StringArray getParamRangeLabel(int index) {
    jassert(index < _paramNum);
    return _param[index].rangeLabel;
  }

  float getParamDefaultValue(int index) {
    jassert(index < _paramNum);
    return _param[index].defaultValue;
  }

  void setParam(int index, float value) {
    jassert(index < _paramNum);
    knob[index] = value;
  }
};
