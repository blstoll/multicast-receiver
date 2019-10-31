#ifndef SequenceChecker_h_
#define SequenceChecker_h_

#include<vector>

#include "Processor.h"


class SequenceChecker: public Processor<unsigned short>
{
public:
   SequenceChecker();
   virtual ~SequenceChecker();

   virtual void process(const unsigned short& sequence);
private:
   unsigned short mySequence;
   bool myIsFirstPacket;
};

#endif
