#ifndef DL_stage_h
#define DL_stage_h
#include "stl_type.h"
#include "baseclass.h"

typedef struct
{
  /* Dip limiting stage */
  char *   DLType;
  DLReal   DLMinGain;
  DLReal   DLStart;
  DLReal   DLStartFreq;
  DLReal   DLEndFreq;
  int      DLMultExponent;
  DLReal   DLNormFactor;
  char *   DLNormType;
  char *   DLOutFile;
  char *   DLOutFileType;
} DLParmsType;

class DLstage
{
 private:
  DLParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;

 public:
  DLstage(DRCSignal *InputSig, DLParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(DLParmsType InCfg);
  ~DLstage() {};
  bool process(void);
  DLParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
  void Normalize(void);
  void WriteOutput(void);
  
};
#endif
