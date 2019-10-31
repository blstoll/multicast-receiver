#include "SequenceExtractor.h"

#include <iostream>
#include <arpa/inet.h>
#include <sys/time.h>

SequenceExtractor::SequenceExtractor(Processor<unsigned short>& processor):
   myProcessor(processor)
{
}

SequenceExtractor::~SequenceExtractor()
{
}

void SequenceExtractor::process(const std::vector<unsigned char>& packet)
{
   if(packet.size() > sizeof(unsigned short))
   {
      const unsigned short sequence(ntohs(*reinterpret_cast<const unsigned short*>(packet.data())));

      if(sequence % 10000 == 0)
      {
         struct timeval tv;
         gettimeofday(&tv, NULL);

         std::cout << "Received sequence: " << sequence << " at: " << tv.tv_sec << std::endl;
      }
      myProcessor.process(sequence);
   }
}
