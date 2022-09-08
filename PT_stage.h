#ifndef PT_stage_h
#define PT_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "slprefilt_stl.h"
#include "bwprefilt_stl.h"
#include "convol.h"
#include "psychoacoustic.h"

typedef struct
{
  char * PTType;
  int    PTReferenceWindow;
  char * PTDLType;
  DLReal PTDLMinGain;
  DLReal PTDLStartFreq;
  DLReal PTDLEndFreq;
  DLReal  PTDLStart;
  DLReal PTDLMultExponent;
  DLReal PTBandWidth;
  DLReal PTPeakDetectionStrength;
  int    PTMultExponent;
  int    PTFilterLen;
  char * PTFilterFile;
  char * PTFilterFileType;
  DLReal PTNormFactor;
  char * PTNormType;
  char * PTOutFile;
  char * PTOutFileType;
  int    PTOutWindow;
  int    BCInitWindow;
} PTParmsType;

class PTstage
{
 private:
  PTParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *RefSig;
  DRCSignal *OutSig;

 public:
  PTstage(DRCSignal* InputSig, DRCSignal* RefSignal, PTParmsType InCfg);
  void NewInputSignal(DRCSignal* InputSig, DRCSignal* RefSignal);
  void NewInCfg(PTParmsType InCfg);
  ~PTstage() {};
  int process(void);
  void Normalize(void);
  void WriteOutput(void);
  PTParmsType getPTParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
};
#endif
