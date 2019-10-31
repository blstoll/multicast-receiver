#include "OrderedWindow.h"

#include <iostream>

OrderedWindow::OrderedWindow(Processor<unsigned short>& processor, size_t windowSize):
   myProcessor(processor),
   myWindow(),
   myWindowSize(windowSize),
   mySequence(0),
   myIsFirstPacket(true)
{
}

OrderedWindow::~OrderedWindow()
{
}

void OrderedWindow::process(const unsigned short& sequence)
{
   if(mySequence == sequence || myIsFirstPacket)
   {
      myIsFirstPacket = false;
      myProcessor.process(sequence);
      mySequence = sequence + 1; 
   }
   else if(isOld(sequence))
   {
      // Give it the hook
      std::cout << "Dropping old packet: " << sequence
                << " expected: " << mySequence << std::endl;
   }
   else
   {
      // A packet from the future, just like on TV
      myWindow.insert(myWindow.end(), sequence);
   }

   release();
}

bool OrderedWindow::isOld(unsigned short sequence)
{
   if(mySequence > NEAR_ROLLOVER && sequence < JUST_ROLLED)
   {
      return false;
   }
   else if(mySequence < JUST_ROLLED && sequence > NEAR_ROLLOVER)
   {
      return true;
   }
   else
   {
      return sequence < mySequence;
   }
}

void OrderedWindow::release()
{
   if(myWindow.size() > myWindowSize)
   {
      unsigned short sequence(*myWindow.begin());
      myWindow.erase(myWindow.begin());

      myProcessor.process(sequence);
      mySequence = sequence + 1;
   }

   auto itr(myWindow.begin());
   while(myWindow.size() && (itr = myWindow.find(mySequence)) != myWindow.end())
   {
      std::cout << "Releasing queued sequence: " << *itr << std::endl;
      myProcessor.process(*itr);
      myWindow.erase(itr);

      mySequence = *itr + 1;
   }
}
