#include "SequenceChecker.h"

#include <iostream>
#include <arpa/inet.h>


SequenceChecker::SequenceChecker():
   mySequence(0),
   myIsFirstPacket(true)
{
}

SequenceChecker::~SequenceChecker()
{
}

void SequenceChecker::process(const unsigned short& sequence)
{
   if(!myIsFirstPacket)
   {
      if(sequence != mySequence)
      {
         std::cerr << "Unexpected sequence: " << sequence
                   << " Expected: " << mySequence << std::endl;
      }
      else
      {
         //std::cout << "Successfully processed: " << sequence << std::endl;
      }
   }
   else
   {
      myIsFirstPacket = false;
   }

   mySequence = sequence + 1;
}
