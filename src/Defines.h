//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include <chrono>
#ifndef MSGQUEUE
#define MSGQUEUE

namespace wiplate
{

const int kNoSNR = -200;
const std::string kNoName = "ff:ad";
struct WifiClient
{
  std::string mac;
  double snr;
  std::string probeAp;
  std::chrono::high_resolution_clock::time_point timestamp;
};

}
#endif