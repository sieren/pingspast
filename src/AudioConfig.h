//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include <vector>
#include <stdlib.h>

#ifndef AUDIOCONF
#define AUDIOCONF

namespace wiplate
{
namespace audio
{
  using Sample = float;
  using SampleBuffer = std::vector<Sample>;
  using SampleBufferList = std::vector<SampleBuffer>;

  namespace cfg
  {
    const size_t chanNumbers = 1;
    const size_t  sampleRate = 44100.0f;
    const size_t prefFrameSize = 256;
  }
}
}


#endif
