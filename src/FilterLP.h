//
//  FilterLP.h
//  Multitude
//
//  Created by Matthias Frick on 13/05/15.
//  Copyright (c) 2015 Matthias Frick. All rights reserved.
//


#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <assert.h>
namespace mfk
{
namespace Filter
{
template <typename T>
class FilterLP
{

  public:
    FilterLP() { }
    ~FilterLP() { }
    FilterLP(float freq, float reso) {  b0 = b1 = b2 = b3 = b4 = t1 = t2 = 0;
      setCutoff(freq); setReso(reso); }
    void setCutoff(float freq)
    {
      assert(freq >= 0.f && freq <= 1.f);
      mFreq = freq;
    }

    void setReso(float reso)
    {
      assert(reso >= 0.f && reso <= 1.f);
      mReso = reso;
    }

    void setFs(float fs)
    {
      mFs = fs;
    }

    T processSample(T smpl)
    {
      q = 1.0f - mFreq;
      p = mFreq + 0.8f * mFreq * q;
      frq = p + p - 1.0f;
      q = mReso * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
      T in = smpl;
      in -= q * b4; //feedback
      t1 = b1; b1 = (in + b0) * p - b1 * frq;
      t2 = b2; b2 = (b1 + t1) * p - b2 * frq;
      t1 = b3; b3 = (b2 + t2) * p - b3 * frq;
      b4 = (b3 + t1) * p - b4 * frq;
      b4 = b4 - b4 * b4 * b4 * 0.166667f; //clipping
      b0 = in;
      // Lowpass output: b4
      // Highpass output: in - b4;
      // Bandpass output: 3.0f * (b3 - b4);
      smpl = b4;
      return smpl;
    }

    std::vector<T> processSamples(std::vector<T> smpls)
    {
      // Moog 24 dB/oct resonant lowpass VCF
      // References: CSound source code, Stilson/Smith CCRMA paper.
      std::vector<T> outBuff(smpls.size());
      std::fill(outBuff.begin(), outBuff.end(), 0);

      for (int i = 0; i<smpls.size(); i++)
      {
        outBuff.at(i) = processSample(smpls.at(i));
      }
      return outBuff;
    }

  private:
    float mFreq = 0.999;
    float mReso = 0.001;
    float mFs = 44100;
    float frq, p, q; //filter coefficients
    float b0, b1, b2, b3, b4; //filter buffers (beware denormals!)
    float t1, t2; //temporary buffers
};
} // filter
} // mfk

