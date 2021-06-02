#pragma

#include <JuceHeader.h>

const int MAX_PARAM_NUM = 50;  //输入（整数）不变量：最大参数值

typedef juce::String String;  //为一种数据类型定义一个新名字
typedef juce::Array<float> FloatArray;  // float为表示单精度类型 int表为整数
typedef juce::Array<juce::String> StringArray;

enum ParamType {
  Slider        //滑动组件,
      ComboBox  //组合框架,
};

typedef struct Param {  // struct 结构体名
  String label;         //字符串处理
  ParamType type;
  FloatArray range;
  StringArray rangeLabel;
  float defaultValue;
} Param;

class AudioProcessor {
 public:
  AudioProcessor(){};
  virtual ~AudioProcessor(){};

  virtual void audioInit(const float *knob,
                         float sampleRate) = 0;  // void 为输入无类型 空类型
  virtual void audioProcess(const float *inDataL, const float *inDataR,
                            float *outDataL, float *outDataR, int frameSize,
                            const float *knob) = 0;

 private:
  int _paraNum;
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
  int getPrarmNum() { return _paraNum; }

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
