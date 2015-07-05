//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include <vector>
#include <AudioConfig.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <Defines.h>
#include <AudioOSC.h>

namespace wiplate
{
namespace audio
{
class AudioGenerator
  {

  public:
    AudioGenerator(size_t sampleRate);
    ~AudioGenerator();
    static void processAudio(audio::Sample* data, size_t frames);
    void fireResults();
    void addNewClient(WifiClient newClient);

  private:
    float mSampleRate;
    void processClient(WifiClient newClient);
    WifiClient oldClientSafe;

  };
} // end of audio
} // end of wiplagte

