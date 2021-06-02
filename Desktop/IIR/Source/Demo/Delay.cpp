#include "./Delay.h"

Delay::Delay() { initParam(PARAM_NUM, param); }

Delay::~Delay() {}

void Delay::delay(float in, int length, float feedback) {
  buffer[writeIndex] = in + delayOut * feedback;

  readIndex = writeIndex - length;
  if (readIndex < 0) {
    readIndex = readIndex + DELAY_LENGTH_MAX + 1;  //为0时 返回最大值
  }
  delayOut = buffer[readIndex];

  writeIndex++;
  if (writeIndex > DELAY_LENGTH_MAX) {
    writeIndex = 0;
  }
  readIndex++;
  if (readIndex > DELAY_LENGTH_MAX) {
    readIndex = 0;
  }
}

void Delay::audioInit(const float *knob, float sampleRate) {
  // 初始化旋钮
  knobTimeOld = knob[0];
  knobFeedbackOld = knob[1];
  knobMixOld = knob[2];
  knobLevelOld = knob[3];

  memset(buffer, 0, sizeof(buffer));
  writeIndex = 0;
  readIndex = 0;
}

void Delay::audioProcess(const float *inDataL, const float *inDataR,
                         float *outDataL, float *outDataR, int frameSize,
                         const float *knob) {
  float input, inputL, inputR;
  float output, outputL, outputR;

  // 旋钮
  knobTime = knob[0];
  knobFeedback = knob[1];
  knobMix = knob[2];
  knobLevel = knob[3];

  // main loop
  for (int i = 0; i < frameSize; i++) {
    // 左右声道输入数据
    knobFeedbackOld = knobRotate(knobFeedbackOld, knobFeedback);
    knobMixOld = knobRotate(knobMixOld, knobMix);

    inputL = inDataL[i];
    inputR = inDataR[i];
    input = inputL;

    int length = (int)(knobTime * 0.001 * FS);
    delay(inputL, length, knobFeedbackOld);

    // mix
    output = inputL * (1 - knobMixOld) + delayOut * knobMixOld;

    // Level
    output = output * 2 * knobLevelOld;

    outDataL[i] = output;
    outDataR[i] = output;
  }
}