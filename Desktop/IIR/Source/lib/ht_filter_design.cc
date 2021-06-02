#include "ht_filter_design.h"

void lowpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);

  b_coeffs[0] = K / (K + 1);
  b_coeffs[1] = b_coeffs[0];
  a_coeffs[0] = (K - 1) / (K + 1);
}

void highpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);

  b_coeffs[0] = 1.f / (K + 1);
  b_coeffs[1] = -b_coeffs[0];
  a_coeffs[0] = (K - 1) / (K + 1);
}

void allpass_first(float fc, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);

  b_coeffs[0] = (K - 1) / (K + 1);
  b_coeffs[1] = 1;
  a_coeffs[0] = b_coeffs[0];
}

void lowpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float K2 = K * K;
  float den = (K2 * Q + K + Q);

  b_coeffs[0] = (K2 * Q) / den;
  b_coeffs[1] = (2 * K2 * Q) / den;
  b_coeffs[2] = b_coeffs[0];
  a_coeffs[0] = (2 * Q * (K2 - 1)) / den;
  a_coeffs[1] = (K2 * Q - K + Q) / den;
}

void highpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float K2 = K * K;
  float den = (K2 * Q + K + Q);

  b_coeffs[0] = (Q) / den;
  b_coeffs[1] = (-2 * Q) / den;
  b_coeffs[2] = b_coeffs[0];
  a_coeffs[0] = (2 * Q * (K2 - 1)) / den;
  a_coeffs[1] = (K2 * Q - K + Q) / den;
}

void allpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float K2 = K * K;
  float den = (K2 * Q + K + Q);

  b_coeffs[0] = (K2 * Q - K + Q) / den;
  b_coeffs[1] = (2 * Q * (K2 - 1)) / den;
  b_coeffs[2] = 1;
  a_coeffs[0] = b_coeffs[1];
  a_coeffs[1] = b_coeffs[0];
}

void bandpass(float fc, float Q, float fs, float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float K2 = K * K;
  float den = (K2 * Q + K + Q);

  b_coeffs[0] = (K) / den;
  b_coeffs[1] = 0;
  b_coeffs[2] = -b_coeffs[0];
  a_coeffs[0] = (2 * Q * (K2 - 1)) / den;
  a_coeffs[1] = (K2 * Q - K + Q) / den;
}

void bandreject(float fc, float Q, float fs, float b_coeffs[],
                float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float K2 = K * K;
  float den = (K2 * Q + K + Q);

  b_coeffs[0] = (Q * (1 + K2)) / den;
  b_coeffs[1] = (2 * Q * (K2 - 1)) / den;
  b_coeffs[2] = b_coeffs[0];
  a_coeffs[0] = b_coeffs[1];
  a_coeffs[1] = (K2 * Q - K + Q) / den;
}

void peaking(float fc, float Q, float db_gain, float fs, float b_coeffs[],
             float a_coeffs[]) {
  float A = powf(10, (db_gain * 0.025f));  // powf(10, (db_gain / 40));
  float w0 = TWO_PI * (fc / fs);
  float a = sinf(w0) / (2 * Q);
  float den = (1 + a / A);

  b_coeffs[0] = (1 + a * A) / den;
  b_coeffs[1] = (-2 * cosf(w0)) / den;
  b_coeffs[2] = (1 - a * A) / den;
  a_coeffs[0] = b_coeffs[1];
  a_coeffs[1] = (1 - a / A) / den;
}

void peaking_const_q(float fc, float Q, float db_gain, float fs,
                     float b_coeffs[], float a_coeffs[]) {
  float K = tanf(PI * fc / fs);
  float V0 = powf(10, (db_gain * 0.05f));  // powf(10, (db_gain / 20));
  float K2 = K * K;

  if (db_gain > 0) {
    // boost
    float den = 1 + K / Q + K2;
    b_coeffs[0] = (1 + V0 / Q * K + K2) / den;
    b_coeffs[1] = (2 * (K2 - 1)) / den;
    b_coeffs[2] = (1 - V0 / Q * K + K2) / den;
    a_coeffs[0] = b_coeffs[1];
    a_coeffs[1] = (1 - K / Q + K2) / den;
  } else {
    // cut
    float den = 1 + K / (V0 * Q) + K2;
    b_coeffs[0] = (1 + K / Q + K2) / den;
    b_coeffs[1] = (2 * (K2 - 1)) / den;
    b_coeffs[2] = (1 - K / Q + K2) / den;
    a_coeffs[0] = b_coeffs[1];
    a_coeffs[1] = (1 - K / (V0 * Q) + K2) / den;
  }
}

void lowshelf(float fc, float Q, float db_gain, float fs, float b_coeffs[],
              float a_coeffs[]) {
  float w0 = TWO_PI * (fc / fs);
  float a = sinf(w0) / (2 * Q);
  float A = powf(10, db_gain * 0.025f);  // powf(10, db_gain / 40)
  float sqrtAa = sqrtf(A) * a;
  float den = (A + 1) + (A - 1) * cosf(w0) + 2 * sqrtAa;

  b_coeffs[0] = (A * ((A + 1) - (A - 1) * cosf(w0) + 2 * sqrtAa)) / den;
  b_coeffs[1] = (2 * A * ((A - 1) - (A + 1) * cosf(w0))) / den;
  b_coeffs[2] = (A * ((A + 1) - (A - 1) * cosf(w0) - 2 * sqrtAa)) / den;
  a_coeffs[0] = (-2 * ((A - 1) + (A + 1) * cosf(w0))) / den;
  a_coeffs[1] = ((A + 1) + (A - 1) * cosf(w0) - 2 * sqrtAa) / den;
}

void highshelf(float fc, float Q, float db_gain, float fs, float b_coeffs[],
               float a_coeffs[]) {
  float w0 = TWO_PI * (fc / fs);
  float a = sinf(w0) / (2 * Q);
  float A = powf(10, db_gain * 0.025f);  // powf(10, db_gain / 40)
  float sqrtAa = sqrtf(A) * a;
  float den = (A + 1) - (A - 1) * cosf(w0) + 2 * sqrtAa;

  b_coeffs[0] = (A * ((A + 1) + (A - 1) * cosf(w0) + 2 * sqrtAa)) / den;
  b_coeffs[1] = (-2 * A * ((A - 1) + (A + 1) * cosf(w0))) / den;
  b_coeffs[2] = (A * ((A + 1) + (A - 1) * cosf(w0) - 2 * sqrtAa)) / den;
  a_coeffs[0] = (2 * ((A - 1) - (A + 1) * cos(w0))) / den;
  a_coeffs[1] = ((A + 1) - (A - 1) * cosf(w0) - 2 * sqrtAa) / den;
}

void tilt(float fc_low, float db_gain_low, float fc_high, float db_gain_high,
          float fs, float b_coeffs[], float a_coeffs[]) {
  float boost_t = powf(10, db_gain_high * 0.05f);  // powf(10, db_gain / 20)
  float boost_b = powf(10, db_gain_low * 0.05f);

  float a_t = tanf(PI * fc_high / fs);
  float a_b = tanf(PI * fc_low / fs);
  float knum_t = 2 / (1 + (1 / boost_t));
  float kden_t = 2 / (1 + boost_t);
  float knum_b = 2 / (1 + (1 / boost_b));
  float kden_b = 2 / (1 + boost_b);

  float a10 = a_t + kden_t;
  float b10 = a_t + knum_t;
  float a11 = a_t - kden_t;
  float b11 = a_t - knum_t;
  float a20 = (a_b * kden_b) + 1;
  float b20 = (a_b * knum_b) - 1;
  float a21 = (a_b * kden_b) - 1;
  float b21 = (a_b * knum_b) + 1;
  float a0 = a10 * a20;

  b_coeffs[0] = (b10 * b20) / a0;
  b_coeffs[1] = ((b10 * b21) + (b11 * b20)) / a0;
  b_coeffs[2] = (b11 * b21) / a0;
  a_coeffs[0] = ((a10 * a21) + (a11 * a20)) / a0;
  a_coeffs[1] = a11 * a21 / a0;
}

void gain_decorator(float fc, float Q, float line_gain, float fs,
                    float b_coeffs[], float a_coeffs[],
                    void (*filter)(float, float, float, float[], float[])) {
  filter(fc, Q, fs, b_coeffs, a_coeffs);
  b_coeffs[0] *= line_gain;
  b_coeffs[1] *= line_gain;
  b_coeffs[2] *= line_gain;
}

void wah_filter(float control, float freq_min, float freq_max, float Q,
                float tracking, float fs, float *b_coeffs, float *a_coeffs) {
  // 计算 f0
  float f0 = freq_min + (freq_max - freq_min) * control;

  // 计算 Q
  float QMax = Q;
  float QMin = 1 + (QMax - 1) * (1 - tracking);     // [QMax, 1]
  float Q0 = QMin + (QMax - QMin) * (1 - control);  // QMax -> QMin

  // 计算 Gain, [13db, 15.5db]
  // float line_gain = 4.4668 + 1.4898 * control;

  // 计算 Gain, [18db, 20.5db]
  float line_gain = 7.9433 + 2.6492 * control;

  gain_decorator(f0, Q0, line_gain, fs, b_coeffs, a_coeffs, bandpass);
}

void fir_lowpass(float fc, float fs, int taps, float window[], float coeffs[]) {
  float sum = 0.f;
  float wc = 2.f * fc / fs;

  // calculate fir coeffs
  linspace(-taps * 0.5f, taps * 0.5f, taps + 1, coeffs);
  for (size_t i = 0; i < taps + 1; i++) {
    coeffs[i] = wc * sinc(wc * coeffs[i]);
    coeffs[i] *= window[i];
    sum += coeffs[i];
  }

  // normalize coeffs
  float inv_sum = 1.f / sum;
  for (size_t i = 0; i < taps + 1; i++) coeffs[i] *= inv_sum;
}
