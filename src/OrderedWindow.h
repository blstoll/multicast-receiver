#ifndef OrderedWindow_h_
#define OrderedWindow_h_

#include <set>
#include <stdlib.h>

#include "Processor.h"

class OrderedWindow : public Processor<unsigned short>
{
public:
   OrderedWindow(Processor<unsigned short>& processor, size_t windowSize = 1000);
   virtual ~OrderedWindow();

   virtual void process(const unsigned short& sequence); 

private:
   void release();
   bool isOld(unsigned short sequence);
private:
   // Divide the numeric range into thirds to help detect when a sequence roll-over has occurred
   const static unsigned short JUST_ROLLED = 0XFFFF / 3;
   const static unsigned short NEAR_ROLLOVER = JUST_ROLLED * 2;

   Processor<unsigned short>& myProcessor;

   std::set<unsigned short> myWindow;
   size_t myWindowSize;

   unsigned short mySequence;
   bool myIsFirstPacket;
   
};

#endif
