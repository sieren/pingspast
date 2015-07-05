//  Created by Matthias Frick (c) 2015. All rights reserved.

#include "AudioOSC.h"
#include <vector>

float gPhaseAccumulator = 0.0f;
int nWraps;
float dPhi;
mfk::Smoother<float> fadeOut = 0;

namespace wiplate
{
namespace audio
{
namespace dsp
{

audio::SampleBuffer AudioOSC::process(size_t frames)
{
  std::vector<audio::Sample> outBuffer(frames);
  std::fill(outBuffer.begin(), outBuffer.end(), 0);
  if (mIsRunning)
  {
    const double twoPI = 2 * mPI;
    switch (mOscillatorMode) {
      case OSC_MODE_SINE:
        for (int i = 0; i < frames; i++) {
          outBuffer.at(i) = fadeOut.getUpdated() * sin(mPhase);
          mPhase += mPhaseIncrement;
          while (mPhase >= twoPI) {
            mPhase -= twoPI;
          }
        }
        break;
      case OSC_MODE_SAW:
        for (int i = 0; i < frames; i++) {
          outBuffer.at(i) = fadeOut.getUpdated() * (1.0 - (2.0 * mPhase / twoPI));
          mPhase += mPhaseIncrement;
          while (mPhase >= twoPI) {
            mPhase -= twoPI;
          }
        }
        break;
      case OSC_MODE_SQUARE:
        for (int i = 0; i < frames; i++) {
          if (mPhase <= mPI) {
            outBuffer.at(i) = fadeOut.getUpdated() * 1.0;
          } else {
            outBuffer.at(i) = fadeOut.getUpdated() * -1.0;
          }
          mPhase += mPhaseIncrement;
          while (mPhase >= twoPI) {
            mPhase -= twoPI;
          }
        }
        break;
      case OSC_MODE_TRIANGLE:
        for (int i = 0; i < frames; i++) {
          double value = -1.0 + (2.0 * mPhase / twoPI);
          outBuffer.at(i) = fadeOut.getUpdated() * 2.0 * (fabs(value) - 0.5);
          mPhase += mPhaseIncrement;
          while (mPhase >= twoPI) {
            mPhase -= twoPI;
          }
        }
        break;
    }
  }
  if (fadeOut.get() == 0)
  {
    mIsRunning = false;
  }
  return outBuffer;
}

void AudioOSC::updateIncrement() {
  mPhaseIncrement = mFreq * 2 * mPI / mFs;
}

void AudioOSC::fire(float length, float freq, float amp, OscModes mode)
{
  mFreq = freq;
  mAmp = amp;
  mLength = length * mFs;
  mIsRunning = true;
  mOscillatorMode = mode;
  gPhaseAccumulator = 0.0f;
  nWraps = 0;
  dPhi = 0;
  fadeOut.set(amp,  mLength);
  updateIncrement();
}
}
}
}