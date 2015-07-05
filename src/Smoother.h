//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include <assert.h>
#include <math.h>
#include <algorithm>

namespace mfk
{
  template <typename T>
  class Smoother
  {
  private:
    float kSampleRate = 441000;
  public:
    Smoother()
    : mOldValue(0), mNewValue(0), mUpdateCounter(0), mUpdateCounterMax(0.1 * kSampleRate)
    {}

    Smoother(float v)
    : mOldValue(v), mNewValue(v), mUpdateCounter(0),  mUpdateCounterMax(0.1 * kSampleRate)
    { assert(v >= 0.f && v <= 1.0); }

    Smoother(float v, float updateRate)
    : mOldValue(v), mNewValue(v), mUpdateCounter(0), kSampleRate(updateRate), mUpdateCounterMax(0.1 * updateRate)
    { assert(v >= 0.f && v <= 1.0); }

    void set(T v) {
      assert(v >= 0.f && v <= 1.0);
      mOldValue         = get();
      mNewValue         = v;
      mUpdateCounter    = (T)mUpdateCounterMax;
    }

    void set(T v, int updateRate) {
      mOldValue         = v;
      mNewValue         = 0;
      kSampleRate = updateRate;
      mUpdateCounterMax =  kSampleRate;
      mUpdateCounter    = (T)mUpdateCounterMax;
    }

    T get()
    {
      T f = factor();
      assert(f >= 0.f && f <= 1.f);
      return (mOldValue - mNewValue)*f + mNewValue;
    }

    T getRaw() {
      return mNewValue;
    }

    T getUpdated()
    {
      mUpdateCounter = std::max(mUpdateCounter-1, 0);
      return get();
    }

    T getUpdatedToZero()
    {
      if (mNewValue == 0) {
        mUpdateCounter = std::max(mUpdateCounter-1, 0);
        return get(); }
      else return mNewValue;
    }

  private:
    T factor()
    {
      assert(mUpdateCounterMax > 0);
      T f = (mUpdateCounter * M_PI) / ( 2* mUpdateCounterMax);
      return pow(sinf(f), 2);
    }

    T mOldValue;
    T mNewValue;
    int mUpdateCounter;
    int mUpdateCounterMax;
  };
} // namespace mfk
