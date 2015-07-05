//  Created by Matthias Frick (c) 2015. All rights reserved.

#include <iostream>
#include <DataHandler.h>
#include <AEController.h>
#include <thread>
#include <AudioGenerator.h>
#include <AudioConfig.h>
#include <functional>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <vector>
#include <cstring>
#include <signal.h>

std::shared_ptr<wiplate::data::DataHandler> pHandler;
std::unique_ptr<wiplate::audio::AEController> audioController;
std::shared_ptr<wiplate::audio::AudioGenerator> audioGenerator;
boost::thread dHandlerThread;
boost::asio::io_service ios;
boost::asio::strand strand(ios);
boost::asio::io_service::work work(ios);

void shutdown()
{
  ios.stop();
  audioController->stop();
  do
  {
   usleep(1000);
  } // wait for audioengine to stop gracefully
  while (audioController->isRunning());
  pHandler->stop();
}

void sighandler(int sig)
{
  if (sig == SIGABRT)
  {
    shutdown();
  }

  if (sig == SIGTERM || sig == SIGINT)
  {
    std::cout << "\nShutting down down gracefully...\n";
    shutdown();
  }
}

void printUsage()
{
  std::cout << "Usage is -i <interface>.\n";
}

int main(int argc, char** argv)
{
  using namespace wiplate;
  if (argc <= 1)
  {
    printUsage();
    return 0;
  }
  // register sighandlers
  signal(SIGABRT, &sighandler);
  signal(SIGINT, &sighandler);
  signal(SIGTERM, &sighandler);

  // TODO: accept input for number of voices
  if (std::string(argv[1]) == "-i")
  {
    if (argv[1+1] == 0)
    {
      printUsage();
      return 0;
    }
    dHandlerThread = boost::thread(boost::bind(&boost::asio::io_service::run, &ios));
    audioGenerator = std::make_shared<audio::AudioGenerator>(44100);
    pHandler = std::make_shared<data::DataHandler>(&strand, std::string(argv[2]), std::bind(&audio::AudioGenerator::addNewClient, audioGenerator, std::placeholders::_1));
  }
  else
  {
    printUsage();
    return 0;
  }

  std::thread handThread(&data::DataHandler::start, pHandler);
  audioController = std::make_unique<audio::AEController>(44100);
  using Callback = std::function<void(audio::Sample* data, size_t frames_)>;

  audioController->start(audioGenerator->processAudio);
  dHandlerThread.join();
  handThread.join();

  return 0;
}



