#include "./IIRFilter.h"

IIRFilter::IIRFilter() { initParam(PARAM_NUM, param); }

IIRFilter::~IIRFilter() {}

void IIRFilter::initFilterCoeffs() {
  int type = (int)knobTypeOld;
  float fc = knobFreqOld;
  float Q = knobQOld;
  float fs = 44100;

  if (type == 0) {
    // LP
    lowpass(fc, Q, fs, b, a);
  } else if (type == 1) {
    // HP
    highpass(fc, Q, fs, b, a);
  } else if (type == 2) {
    // BP
    bandpass(fc, Q, fs, b, a);
  } else if (type == 3) {
    // AP
    allpass(fc, Q, fs, b, a);
  }
}

void IIRFilter::audioInit(const float *knob, float sampleRate) {
  // 初始化旋钮
  knobTypeOld = knob[0];
  knobFreqOld = knob[1];
  knobQOld = knob[2];
  knobGainOld = knob[3];
  knobDelayOld = knob[4];

  memset(state, 0, sizeof(state));
  initFilterCoeffs();
}

void IIRFilter::audioProcess(const float *inDataL, const float *inDataR,
                             float *outDataL, float *outDataR, int frameSize,
                             const float *knob) {
  float input, inputL, inputR;
  float output, outputL, outputR;

  // 旋钮
  knobType = knob[0];
  knobFreq = knob[1];
  knobQ = knob[2];
  knobGain = knob[3];
  knobDelay = knob[4];
  // main loop
  for (int i = 0; i < frameSize; i++) {
    // 左右声道输入数据
    knobGainOld = knobRotate(knobGainOld, knobGain);

    if (knobTypeOld != knobType || knobFreqOld != knobFreq ||
        knobQOld != knobQ || knobDelayOld != knobDelay) {
      knobTypeOld = knobType;
      knobFreqOld = knobFreq;
      knobQOld = knobQ;
      knobDelayOld = knobDelay;
      initFilterCoeffs();
    }

    inputL = inDataL[i];
    inputR = inDataR[i];
    input = inputL;

    output = iir2(input, b, a, state, 2);

    output = output * knobGainOld;

    outDataL[i] = output;
    outDataR[i] = output;
  }
}
