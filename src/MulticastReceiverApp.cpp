#include <iostream>
#include <unistd.h>
#include <csignal>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "SequenceChecker.h"
#include "OrderedWindow.h"
#include "SequenceExtractor.h"
#include "MulticastReceiver.h"

namespace
{
   int SUCCESS = 0;
   int ERROR = -1;

   MulticastReceiver* ourReceiver;
}

void signalHandler(int signum)
{
   std::cout << std::endl << "Iterrupt signal (" << signum << ") received." << std::endl;
   if(ourReceiver)
   {
      ourReceiver->stop();
   }
}

int main(int argc, char* argv[])
{
   namespace opts = boost::program_options;
   opts::options_description desc("Options");
   opts::variables_map values;

   desc.add_options()
   ("help,h", "Print help messages") 
   ("destination,d", opts::value<std::string>()->default_value("224.33.44.4"), "Multicast Destination Address") 
   ("port,p", opts::value<unsigned short>()->default_value(1234),"Port")
   ("bind,b", opts::value<std::string>()->default_value("127.0.0.1"),"Bind address")
   ("udp,u", opts::value<unsigned int>()->default_value(16777216),"UDP buffer size in bytes") // 16MB
   ("window,w", opts::value<unsigned int>()->default_value(100),"Packet ordering window");

   try
   {
      opts::store(opts::parse_command_line(argc, argv, desc), values);
      if(values.count("help"))
      {
         std::cout << "Multicast receiver application" << std::endl << desc << std::endl;
         return SUCCESS;
      }

      // Throws on command line error
      opts::notify(values);
   }
   catch(opts::error& e)
   {
      std::cerr << "Error: " << e.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;

      return ERROR;
   }

   // Register a signal for clean shutdown
   signal(SIGINT, signalHandler);

   std::string multicastAddress(values["destination"].as<std::string>());
   std::string bindIp(values["bind"].as<std::string>());
   unsigned short port(values["port"].as<unsigned short>());
   unsigned int windowSize(values["window"].as<unsigned int>());
   unsigned int udpRxBufferBytes(values["udp"].as<unsigned int>());

   boost::asio::ip::address multicastGroup(boost::asio::ip::address::from_string(multicastAddress));
   boost::asio::ip::address bindAddress(boost::asio::ip::address::from_string(bindIp));
   
   std::cout << "Receiving from: " << multicastGroup << ":" << port << " on interface: " << bindAddress << std::endl;
   std::cout << std::endl << "Type Ctrl+C to exit" << std::endl << std::endl;

   SequenceChecker sequenceChecker;
   OrderedWindow orderedWindow(sequenceChecker, windowSize);
   SequenceExtractor extractor(orderedWindow);

   MulticastReceiver receiver(multicastGroup, bindAddress, port, extractor, udpRxBufferBytes);
   ourReceiver = &receiver;
   receiver.start();

   return 0;
}
