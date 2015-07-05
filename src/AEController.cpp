//  Created by Matthias Frick (c) 2015. All rights reserved.

#include "AEController.h"
#include <iostream>
#include <string>
#include <cassert>

namespace wiplate
{
namespace audio
{

namespace
{

  int audioCallBack(const void*, void *outBuff, unsigned long frames,
                    const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
                    void* userData)
  {
    using Callback = std::function<void(Sample* data, size_t frames)>;

    auto callback  = (Callback*)userData;

    (*callback)( (Sample*)outBuff, frames );
    return paContinue;
  }

} // anon

AEController::AEController(size_t sampleRate)
{
  auto errCode = Pa_Initialize();
  if (errCode != paNoError)
  {
    std::string msg = std::string("Failed to start stream: ") + Pa_GetErrorText(errCode);
    throw std::runtime_error(msg);
  }

  errCode = Pa_OpenDefaultStream( &mStream, 0, cfg::chanNumbers, pa_format<Sample>::value, double(sampleRate), cfg::prefFrameSize, audioCallBack, &mPlaybackCallback );

  if (errCode != paNoError)
  {
    std::string msg = std::string("Failed to start stream: ") + Pa_GetErrorText(errCode);
    throw std::runtime_error(msg);
  }

}

void AEController::start(Callback f)
{
  if (mRunning)
  {
    return;
  }

  mPlaybackCallback = f;
  PaError errCode = Pa_StartStream(mStream);
  if (errCode == paNoError)
  {
    mRunning = true;
  }
  else
  {
    std::string msg = std::string("Failed to start stream: ") + Pa_GetErrorText(errCode);
    throw std::runtime_error(msg);
  }

}

void AEController::start()
{
  if (mRunning)
  {
    return;
  }

  mPlaybackCallback = nullptr;
  PaError errCode = Pa_StartStream(mStream);

  if (errCode == paNoError)
  {
    mRunning = true;
  }
  else
  {
    std::string msg = std::string("Failed to start stream: ") + Pa_GetErrorText(errCode);
    throw std::runtime_error(msg);
  }

}

void AEController::stop()
{
  if (!mRunning)
  {
    return;
  }

  PaError errCode = Pa_StopStream(mStream);

  if (errCode == paNoError)
  {
    mRunning = false;
  }
  else
  {
    std::string msg = std::string("Failed to start stream: ") + Pa_GetErrorText(errCode);
    throw std::runtime_error(msg);
  }
  mRunning = false;
}

AEController::~AEController()
{
  Pa_CloseStream(mStream);
  Pa_Terminate();
}

} // end of audio
} // end of wiplate