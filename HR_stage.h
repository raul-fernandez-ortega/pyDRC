#ifndef HR_stage_h
#define HR_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "hd_stl.h"
#include "dspwind_stl.h"
#include "convol.h"

typedef struct
{
  char*  HRMPHDRecover;
  char*  HRMPEPPreserve;
  int    HRMPHDMultExponent;
  int    HRMPFinalWindow;
  DLReal HRMPNormFactor;
  char * HRMPNormType;
  char * HRMPOutFile;
  char * HRMPOutFileType;
  DLReal HREPNormFactor;
  char * HREPNormType;
  char * HREPOutFile;
  char * HREPOutFileType;
} HRParmsType;

class HRstage
{
 private:
  HRParmsType Cfg;
  DRCSignal *MPInSig;
  DRCSignal *EPInSig;
  DRCSignal *MPSignal;
  DRCSignal *EPSignal; 

 public:
  HRstage(DRCSignal *MPInputSig, DRCSignal *EPInputSignal, HRParmsType InCfg);
  void NewInputSignal(DRCSignal *MPInputSig, DRCSignal *EPInputSignal);
  void NewInCfg(HRParmsType InCfg);
  ~HRstage() {};
  void process(void);
  void Normalize(void);
  void WriteOutput(void);
  HRParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getMPSig() { return MPSignal; };
  DRCSignal *getEPSig() { return EPSignal; };
};
#endif
