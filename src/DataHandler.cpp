//  Created by Matthias Frick (c) 2015. All rights reserved.

#include "DataHandler.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <Defines.h>
#include <thread>

namespace wiplate
{
namespace data
{

DataHandler::DataHandler(boost::asio::strand *strand, std::string iface, Callback handlerCallback) :
                        mStrand(strand), interFace(iface), mDataCallback(handlerCallback) { }

void DataHandler::start()
{
  std::cout << "Got iface: " << interFace << "\n";
  std::string cmd = std::string("tshark -In -l -i " + interFace +
                                " -T fields -e wlan.sa_resolved -e wlan_mgt.ssid"
                                " -e radiotap.dbm_antsignal type mgt subtype probe-req &");
  if(!(pipe = popen(cmd.c_str(), "r")))
  {
    return;
  }
  int fd = fileno(pipe);
  int flags;
  flags = fcntl(fd, F_GETFL, 0);
  flags |= O_NONBLOCK; // make fgets non blocking
  fcntl(fd, F_SETFL, flags);
  mIsRunning = true;
  char buffer[256];
  std::string result;
  while(!feof(pipe) && mIsRunning)
  {
    if(fgets(buffer, 256, pipe) != NULL)
    {
      result = buffer;
      parsePacket(result);
    }
  usleep(200); // lets save some cpu cycles
  }
  mIsRunning = false;
  pclose(pipe);
}

void DataHandler::stop()
{
  mIsRunning = false;
}

void DataHandler::parsePacket(std::string result)
{
  std::stringstream myStream(result);
  std::string token;
  WifiClient client;
  std::getline(myStream, token, '\t');
  client.mac = token;
  if (client.mac == "")
  {
    client.mac = kNoName;
  }
  std::getline(myStream, token, '\t');
  client.probeAp = token;
  if (client.probeAp == "")
  {
    client.probeAp = "XX";
  }
  std::getline(myStream, token, '\t');
  try
  {
    client.snr = std::stoi(token,nullptr,0);
  }
  catch (std::exception &e)
  {
    client.snr = kNoSNR;
  }
  client.timestamp = std::chrono::high_resolution_clock::now();
  mStrand->post(mStrand->wrap(boost::bind(&DataHandler::sendResult, this, client)));
}


void DataHandler::sendResult(WifiClient result)
{
  mDataCallback(result);
}


} // data
} // wiplate
