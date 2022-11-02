#ifndef PL_stage_h
#define PL_stage_h
#include "stl_type.h"
#include "baseclass.h"

typedef struct
{
  char *   PLType;
  DLReal   PLMaxGain;
  DLReal   PLStart;
  DLReal   PLStartFreq;
  DLReal   PLEndFreq;
  int      PLMultExponent;
  int      PLOutWindow;
  DLReal   PLNormFactor;
  char *   PLNormType;
  char *   PLOutFile;
  char *   PLOutFileType;
} PLParmsType;

class PLstage
{
 private:
  PLParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;

 public:
  PLstage(DRCSignal *InputSig, PLParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(PLParmsType InCfg);
  ~PLstage() {};
  bool process(void);
  PLParmsType getPLParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
  void Normalize(void);
  void WriteOutput(void);
  
};
#endif
