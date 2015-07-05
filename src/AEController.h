//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include "AudioConfig.h"
#include <functional>
#include <ciso646>
#include <portaudio.h>

namespace wiplate
{
namespace audio
{
  class AEController
  {
    using Callback = std::function<void(Sample* data, size_t frames_)>;

    public:
      AEController() : AEController(44100) { }
      AEController(size_t sampleRate);
      ~AEController();
      void  start(Callback f);
      void  start();
      void  stop();
      bool  isRunning() const { return mRunning; }

    private:
      bool      mRunning = false;
      PaStream* mStream;
      Callback  mPlaybackCallback;
  };

  namespace
  {
    template<class T> struct pa_format;
    template<> struct pa_format<float>
    { static const PaSampleFormat value = paFloat32; };
    template<> struct pa_format<int32_t>
    { static const PaSampleFormat value = paInt32;  };
    template<> struct pa_format<int16_t>
    { static const PaSampleFormat  value = paInt16;  };
  }
}
}

