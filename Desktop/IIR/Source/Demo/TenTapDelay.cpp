#include "./TenTapDelay.h"

TenTapDelay::TenTapDelay() { initParam(PARAM_NUM, param); }

TenTapDelay::~TenTapDelay() {}

float TenTapDelay::delay(float input, int length, float buffer[],
                         int *writeIndex, int *readIndex, int times) {
  float output;

  // 延迟线长度的整数和小数部分
  int integer = (int)length;
  float frac = length - integer;

  // 输入信号写入 buffer
  buffer[*writeIndex] = input;

  // 读取输出
  *readIndex = (*writeIndex - integer);
  if (*readIndex < 0) {
    *readIndex += TAPS_DELAY_LENGTH_MAX + 1;
  }
  float currOut = buffer[*readIndex];

  // 读取上一个输出
  (*readIndex)--;
  if (*readIndex < 0) {
    *readIndex += TAPS_DELAY_LENGTH_MAX + 1;
  }
  float prevOut = buffer[*readIndex];

  // 线性差值
  output = frac * prevOut + (1 - frac) * currOut;

  // 更新 writeIndex
  (*writeIndex)++;
  if (*writeIndex > TAPS_DELAY_LENGTH_MAX) {
    *writeIndex = 0;
  }

  return output;
}

float TenTapDelay::Tap(float input, float length, float buffer[],
                       int *writeIndex, int readIndex[]) {
  float sumOutput = 0;
  float output[TAPS_NUM];
  int integer[TAPS_NUM];
  int frac[TAPS_NUM];
  int tapLength[TAPS_NUM];
  float currOut, prevOut;

  // 输入信号写入 buffer
  buffer[*writeIndex] = input;

  for (size_t i = 0; i < TAPS_NUM; i++) {
    tapLength[i] = (i + 1) * length;
    integer[i] = (int)tapLength[i];
    frac[i] = tapLength[i] - integer[i];

    // 读取输出
    readIndex[i] = (*writeIndex - integer[i]);
    if (readIndex[i] < 0) {
      readIndex[i] += TAPS_DELAY_LENGTH_MAX + 1;
    }
    currOut = buffer[readIndex[i]];

    // 读取上一个输出
    (readIndex[i])--;
    if (readIndex[i] < 0) {
      readIndex[i] += TAPS_DELAY_LENGTH_MAX + 1;
    }
    prevOut = buffer[readIndex[i]];

    // 线性差值
    output[i] = frac[i] * prevOut + (1 - frac[i]) * currOut;
  }

  // 更新 writeIndex
  (*writeIndex)++;
  if (*writeIndex > TAPS_DELAY_LENGTH_MAX) {
    *writeIndex = 0;
  }

  for (size_t i = 0; i < TAPS_NUM; i++) {
    sumOutput += output[i] / TAPS_NUM;
  }

  return sumOutput;
}

void TenTapDelay::audioInit(const float *knob, float sampleRate) {
  // 初始化旋钮
  knobTimeLOld = knob[0];
  knobFeedbackLOld = knob[1];
  knobTimeROld = knob[2];
  knobFeedbackROld = knob[3];
  knobMixOld = knob[4];
  knobLevelOld = knob[5];
  knobChangeOld = knob[6];
  knobTapOld = knob[7];
  knobPanOld = knob[8];

  lengthOld[0] = knobTimeLOld * 0.001 * FS;
  lengthOld[1] = knobTimeROld * 0.001 * FS;

  memset(delayBuffer, 0, sizeof(delayBuffer));
  memset(delayWriteIndex, 0, sizeof(delayWriteIndex));
  memset(delayReadIndex, 0, sizeof(delayReadIndex));
  memset(delayOut, 0, sizeof(delayOut));
}

void TenTapDelay::audioProcess(const float *inDataL, const float *inDataR,
                               float *outDataL, float *outDataR, int frameSize,
                               const float *knob) {
  float input[2];
  float output[2];
  float delta[2];
  float delayIn[2];

  // 旋钮
  knobTimeL = knob[0];
  knobFeedbackL = knob[1];
  knobTimeR = knob[2];
  knobFeedbackR = knob[3];
  knobMix = knob[4];
  knobLevel = knob[5];
  knobChange = knob[6];
  knobTap = knob[7];
  knobPan = knob[8];

  length[0] = knobTimeL * 0.001 * FS;
  length[1] = knobTimeR * 0.001 * FS;
  float OneOverSampleFrames = 2.2676e-04;  // 1 / (44100 * 0.1);
  delta[0] = (length[0] - lengthOld[0]) * OneOverSampleFrames;
  delta[1] = (length[1] - lengthOld[1]) * OneOverSampleFrames;

  // main loop
  for (int i = 0; i < frameSize; i++) {
    knobFeedbackLOld = knobRotate(knobFeedbackLOld, knobFeedbackL);
    knobFeedbackROld = knobRotate(knobFeedbackROld, knobFeedbackR);
    knobMixOld = knobRotate(knobMixOld, knobMix);
    knobLevelOld = knobRotate(knobLevelOld, knobLevel);
    knobChangeOld = knobRotate(knobChangeOld, knobChange);
    knobTapOld = knobRotate(knobTapOld, knobTap);

    lengthOld[0] += delta[0];
    lengthOld[1] += delta[1];

    // input
    input[0] = inDataL[i];
    input[1] = inDataR[i];

    // delay input and feedback
    delayIn[0] = input[0] + delayOut[0] * knobFeedbackLOld;
    delayIn[1] = input[1] + delayOut[1] * knobFeedbackROld;

    // delayline
//    delayOut[0] = delay(delayIn[0], lengthOld[0], delayBuffer[0],
//                        &delayWriteIndex[0], &delayReadIndex[0]);
//    delayOut[1] = delay(delayIn[1], lengthOld[1], delayBuffer[1],
//                        &delayWriteIndex[1], &delayReadIndex[1]);

    // mix
    output[0] = input[0] * (1 - knobMixOld) + delayOut[0] * knobMixOld;
    output[1] = input[1] * (1 - knobMixOld) + delayOut[1] * knobMixOld;

    // level
    output[0] = output[0] * 2 * knobLevelOld;
    output[1] = output[1] * 2 * knobLevelOld;

    //总输出

    outDataL[i] = output[0];
    outDataR[i] = output[1];

    //  Change 单声道双声道
    //单声道 = 左右声道加在一起

    if (knobChange == 1) {
      outDataL[i] = output[1] + output[2];

      outDataR[i] = output[1] + output[2];
    } else {
      outDataL[i] = output[0];
      outDataR[i] = output[1];
    };

    // Tap delay次数

//    (knobTap == TAPS_NUM) { outDataL = &output[3]; }

    // // 输入信号写入 buffer

    // //左右耳 乒乓效果

    // // output[0] * knobPanOld + output[1] *knobPanOld = 1;
    // // output[0] = ()

    float output = Tap(input[0], lengthOld[0], delayBuffer[0],
                       &delayWriteIndex[0], delayReadIndex);

    outDataL[i] = output;
    outDataR[i] = output;
  }
}
