#include "MulticastReceiver.h"

#include <vector>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;

MulticastReceiver::MulticastReceiver(const asio::ip::address& multicastGroup,
                                     const asio::ip::address& bindAddress,
                                     const unsigned short port,
                                     Processor< std::vector<unsigned char> >& processor,
                                     unsigned int rxBufferBytes):
   myProcessor(processor),
   myService(),
   mySocket(myService),
   myBuffer(MAX_PACKET_SIZE)
{
   asio::ip::udp::endpoint listenEndpoint(bindAddress, port);
   std::cout << "Binding to: " << bindAddress << ":" << port << std::endl;
   mySocket.open(listenEndpoint.protocol());

   mySocket.bind(asio::ip::udp::endpoint(multicastGroup, port));
   mySocket.set_option(asio::socket_base::receive_buffer_size(rxBufferBytes));

   std::cout << "Joining multicast group: " << multicastGroup << std::endl;
   mySocket.set_option(asio::ip::udp::socket::reuse_address(true));
   mySocket.set_option(asio::ip::multicast::enable_loopback(true));

   mySocket.set_option(asio::ip::multicast::join_group(multicastGroup.to_v4(), bindAddress.to_v4()));

   mySocket.async_receive_from(asio::buffer(myBuffer), mySenderEndpoint,
      boost::bind(&MulticastReceiver::onPacket, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

MulticastReceiver::~MulticastReceiver()
{
   stop();
}

void MulticastReceiver::start()
{
   myService.run();
}

void MulticastReceiver::stop()
{
   myService.stop();
}


void MulticastReceiver::onPacket(const boost::system::error_code& error, size_t bytesRx)
{
   if(!error)
   {
      std::vector<unsigned char> packet(myBuffer.begin(), myBuffer.begin() + bytesRx);
      myProcessor.process(packet);

      mySocket.async_receive_from(asio::buffer(myBuffer), mySenderEndpoint,
         boost::bind(&MulticastReceiver::onPacket, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
   }
   else
   {
      std::cerr << "Error: " << error << std::endl;
   }
}
