//  Created by Matthias Frick (c) 2015. All rights reserved.

#ifndef __wifiplate__AudioOSC__
#define __wifiplate__AudioOSC__
#pragma once
#include <stdio.h>
#include <AudioConfig.h>
#include <Smoother.h>

namespace wiplate
{
namespace audio
{
namespace dsp
{
  class AudioOSC
  {
    public:
      enum OscModes
      {
        OSC_MODE_SINE = 0,
        OSC_MODE_SAW,
        OSC_MODE_SQUARE,
        OSC_MODE_TRIANGLE,
      };
      AudioOSC() = default;
      AudioOSC(float fs, float length, float freq, float amp) :
        mFs(fs), mLength(length), mFreq(freq), mAmp(amp),
        mOscillatorMode(OSC_MODE_SINE),
        mIsRunning(false),
        mPI(2*acos(0.0)),
        twoPI(2 * mPI),
        mPhase(0.0){ updateIncrement(); };
      ~AudioOSC() = default;
      audio::SampleBuffer process(size_t frames);
      void fire(float length, float freq, float amp, OscModes mode);
      bool isRunning() { return mIsRunning; };

      void setMode(OscModes mode);
      void setFrequency(double frequency);
      void setSampleRate(double sampleRate);

    private:
      float mFs;
      float mLength;
      float mFreq;
      float mAmp;
      OscModes mOscillatorMode;
      bool mIsRunning;
      const double mPI;
      const double twoPI;
      float mPhase;
      float mPhaseIncrement;
      void updateIncrement();
  };
}
}
}
#endif /* defined(__wifiplate__AudioOSC__) */
