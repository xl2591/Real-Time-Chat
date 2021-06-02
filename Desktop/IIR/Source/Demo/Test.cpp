#include "./Test.h"

Test::Test() { initParam(PARAM_NUM, param); }

Test::~Test() {}

void Test::audioInit(const float *knob, float sampleRate) {
  // 初始化旋钮
  knobLevelOld = knob[0] * 0.01;
  knobPanOld = knob[1] * 0.01;
  knobMuteOld = knob[2];
}

void Test::audioProcess(const float *inDataL, const float *inDataR,
                        float *outDataL, float *outDataR, int frameSize,
                        const float *knob) {
  // 旋钮
  knobLevel = knob[0] * 0.01;
  knobPan = knob[1] * 0.01;
  knobMute = knob[2];

  // main loop
  for (int i = 0; i < frameSize; i++) {
    // 左右声道输入数据
    if (knobMute == 0) {
      outDataL[i] = inDataL[i];
      outDataR[i] = inDataR[i];
    } else {
      outDataL[i] = 0;
      outDataR[i] = 0;
    }
  }
}
