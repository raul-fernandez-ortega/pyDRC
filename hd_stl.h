#ifndef Hd_stl_h
#define Hd_stl_h

#include "stl_type.h"
#include "hd.h"
#include "baseclass.h"
using namespace std;

class HD
{
  public:
  int MExp;
  unsigned int hd_len;
  unsigned int hd_start;
  DRCSignal *InSig;
  DRCSignal *MPSig;
  DRCSignal *EPSig;  
  HD(DRCSignal *InputSig, unsigned int start, unsigned int len, int InMExp);
  ~HD() {};
  void SetInputSignal(DRCSignal *InputSig, unsigned int start, unsigned int len);
  void SetMExp(int InMExp);
  void process(void);
  DRCSignal *getInSig() { return InSig; };
  DRCSignal *getMPSig() { return MPSig; };
  DRCSignal *getEPSig() { return EPSig; };

};

#endif
