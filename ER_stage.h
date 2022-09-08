#ifndef ER_stage_h
#define ER_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "hd_stl.h"
#include "dspwind_stl.h"
#include "convol.h"

typedef struct
{
  DLReal EREPFlatGain;
  DLReal EREPOGainFactor;
  char * EREPFlatType;
  int    EREPFGMultExponent;
  int    EREPFinalWindow;
  DLReal EREPNormFactor;
  char*  EREPNormType;
  char*  EREPOutFile;
  char*  EREPOutFileType;
} ERParmsType;

class ERstage
{
 private:
  ERParmsType Cfg;
  DRCSignal *EPInSig;
  DRCSignal *EPSignal; 

 public:
  ERstage(DRCSignal *EPInputSignal, ERParmsType InCfg);
  void NewInputSignal(DRCSignal *EPInputSignal);
  void NewInCfg(ERParmsType InCfg);
  ~ERstage() {};
  void process(void);
  void Normalize(void);
  void WriteOutput(void);
  ERParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return EPSignal; };
};
#endif
