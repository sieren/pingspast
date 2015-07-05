//  Created by Matthias Frick (c) 2015. All rights reserved.

#pragma once
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include "AudioGenerator.h"

// Data Handler for pipe
//
namespace wiplate
{
namespace data
{
  class DataHandler
  {
    using Callback = std::function<void(WifiClient)>;

    public:
      DataHandler(boost::asio::strand* strand, std::string iface, Callback handlerCallback);
    ~DataHandler() = default;
      void start();
      void stop();

    private:
      boost::asio::strand* mStrand;
      void parsePacket(std::string result);
      void sendResult(WifiClient result);
      bool mIsRunning;
      std::string interFace;
      std::shared_ptr<audio::AudioGenerator> audioGen;
      FILE* pipe;
      WifiClient oldClient;
      Callback mDataCallback;
  };
} // end of data
} // end of wiplate
