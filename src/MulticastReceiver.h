#ifndef MulticastReceiver_h_
#define MulticastReceiver_h_

#include <vector>
#include <boost/asio.hpp>

#include "Processor.h"

class MulticastReceiver
{
public:
   MulticastReceiver(const boost::asio::ip::address& multicastAddr,
                     const boost::asio::ip::address& bindAddress,
                     const unsigned short port,
                     Processor< std::vector<unsigned char> >& processor,
                     unsigned rxBufferBytes = 16777216);

   virtual ~MulticastReceiver();

   void start();
   void stop();

protected:
   void onPacket(const boost::system::error_code& error, size_t bytesRx);
private:
   static const size_t MAX_PACKET_SIZE = 9000;

   Processor< std::vector<unsigned char> >& myProcessor;

   boost::asio::io_service myService;
   boost::asio::ip::udp::socket mySocket;
   boost::asio::ip::udp::endpoint mySenderEndpoint;
   std::vector<unsigned char> myBuffer;
};

#endif
