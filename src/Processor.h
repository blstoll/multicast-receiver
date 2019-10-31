#ifndef Processor_h_
#define Processor_h_

template<typename T>
class Processor
{
public:
   virtual ~Processor(){}
   virtual void process(const T& data) = 0;
};

#endif
