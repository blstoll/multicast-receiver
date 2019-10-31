#ifndef SequenceExtractor_h_
#define SequenceExtractor_h_

#include <vector>

#include "Processor.h"

class SequenceExtractor : public Processor< std::vector<unsigned char>>
{
public:
   SequenceExtractor(Processor<unsigned short>& processor);
   virtual ~SequenceExtractor();

   void process(const std::vector<unsigned char>& packet);

private:
   Processor<unsigned short>& myProcessor;
};
#endif
