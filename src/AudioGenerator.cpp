//  Created by Matthias Frick (c) 2015. All rights reserved.

#include "AudioGenerator.h"
#include <boost/range/algorithm/copy.hpp>
#include <AEController.h>
#include "AudioOSC.h"
#include <algorithm>
#include <functional>
#include <chrono>
#include <FilterLP.h>

namespace wiplate
{
namespace audio
{

std::vector<std::unique_ptr<dsp::AudioOSC>> voices(8);
std::unique_ptr<mfk::Filter::FilterLP<Sample>> filter;

AudioGenerator::AudioGenerator(size_t sampleRate) : mSampleRate(sampleRate)
{
  for (int i = 0; i < voices.size(); i++)
  {
    voices.at(i) = std::make_unique<dsp::AudioOSC>(sampleRate, 3, 220, 1.0);
  }
  std::cout << "VOICES: " << voices.size() << "\n";

  // filter output signal to make it less harsh
  filter = std::make_unique<mfk::Filter::FilterLP<Sample>>(0.18, 0.5);
}

void AudioGenerator::processAudio(audio::Sample* data, size_t frames)
{
  std::vector<Sample> buffer(frames * cfg::chanNumbers);
  std::vector<Sample> tempBuffer(frames * cfg::chanNumbers);
  if (buffer.size() != frames)
  {
    buffer.resize(frames * cfg::chanNumbers);
  }
  for (auto &voice : voices)
  {
    if (voice->isRunning()) // save some cpu cycles and only use active voices
    {
    std::vector<Sample> oscbuffer = voice->process(frames);
    std::transform(oscbuffer.begin(), oscbuffer.end(), buffer.begin(), tempBuffer.begin(), std::plus<Sample>());
    std::copy(tempBuffer.begin(), tempBuffer.end(), buffer.begin());
    std::fill(tempBuffer.begin(), tempBuffer.end(), 0);
    std::fill(oscbuffer.begin(), oscbuffer.end(), 0);
    }
  }
  buffer = filter->processSamples(buffer);
  boost::range::copy(buffer, data);

}

/*
 The incoming WIFI Client is being examined as to whether
 it is a duplicate request or not.
 The examination is taken care of by first 4 HEX of the MAC Adresse,
 and the probed AP.
 */
void AudioGenerator::addNewClient(WifiClient newClient)
{
  WifiClient oldClient = oldClientSafe;
  double timeDiff = std::chrono::duration<double, std::milli>(newClient.timestamp-oldClient.timestamp).count();

  // filter out probe bursts from roughly the same device manufacturer
  if (!oldClient.probeAp.compare(newClient.probeAp) && !(oldClient.mac.compare(newClient.mac)))
  {
    if (timeDiff > 500)
    {
      processClient(newClient);
    }
    else
    {
      std::cout << "Delta: " << timeDiff << " Got duplicate request, bailing...\n";
      std::cout << "MAC: " << newClient.mac << " AP: " << newClient.probeAp << "\nSIGNAL: " << newClient.snr << "\n\n";
    }
  }
  else
  {
    processClient(newClient);
  }
  oldClientSafe = newClient;
}

/*
 The clients data (MAC, SNR and Probed AP) is being translated
 into different parameters for the oscillators.
 */
void AudioGenerator::processClient(WifiClient newClient)
{
  std::cout << "RECEIVED HOMELESS DEVICE: \n";
  std::cout << "MAC: " << newClient.mac << " AP: " << newClient.probeAp << "\nSIGNAL: " << newClient.snr << "\n\n";
  int apLength = newClient.probeAp.length() == 0 ? 2 : newClient.probeAp.length();
  apLength = apLength > 5 ? 5 : apLength; // limit to 5 secs max
  dsp::AudioOSC::OscModes oscMode = dsp::AudioOSC::OSC_MODE_SINE;
  double amp = 0.5;

  // convert MAC to Freq
  std::string rawMac = newClient.mac.substr(0,2) + newClient.mac.substr(3,2);
  float freq;
  try
  {
    freq = (float)std::stol(rawMac, nullptr, 16);
    freq = (freq / 65535) * 800 + 150;
  }
  catch (std::exception &e) {
    freq = 120;
  }

  if (newClient.probeAp == "XX")
  {
    oscMode = dsp::AudioOSC::OSC_MODE_SAW;
    amp = 0.8;
  }
  for (auto &voice : voices)
  {
    if (!(voice->isRunning()))
    {
      voice->fire(apLength, freq, amp, oscMode);
      break;
    }
  }
}

// delete voices
AudioGenerator::~AudioGenerator()
{
  voices.erase(voices.begin());
}

} // end of audio
} // end of wiplate
