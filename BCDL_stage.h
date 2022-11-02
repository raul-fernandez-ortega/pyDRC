#ifndef BCDL_stage_h
#define BCDL_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "fir.h"
#include "hd.h"
#include "convol.h"

typedef struct
{
  char *BCDLType;
  DLReal BCDLMinGain;
  int BCDLStartFreq;
  int BCDLEndFreq;
  DLReal BCDLStart;
  int BCDLMultExponent;
  DLReal NormFactor;
  char *NormType;
  char *OutFile;
  char *OutFileType;
} BCDLParmsType;

class BCDLstage
{
 private:
  BCDLParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;

 public:
  BCDLstage(DRCSignal *InputSig, BCDLParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(BCDLParmsType InCfg);
  ~BCDLstage() {};
  bool process(void);
  BCDLParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
  void Normalize(void);
  void WriteOutput(void);
};
#endif
