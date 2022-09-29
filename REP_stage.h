#ifndef REP_stage_h
#define REP_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "hd_stl.h"
#include "dspwind_stl.h"

typedef struct
{
  /* Renormalization Minimum Phase */
  char* EPHDRecover;
  char* EPEPPreserve;
EPPFFlatGain = 1.0
EPPFOGainFactor = 0.0
EPPFFlatType = M
EPPFFGMultExponent = 3
  int EPHDMultExponent;
  int EPPFFinalWindow;
  DLReal MPPFNormFactor;
  char * MPPFNormType;
  char * MPPFOutFile;
  char * MPPFOutFileType;
} RMPParmsType;

class RMPstage
{
 private:
  RMPParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *MPSignal;
  DRCSignal *EPSignal; 
  void process(void);

 public:
  RMPstage(DRCSignal *InputSig, RMPParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(RMPParmsType InCfg);
  ~RMPstage() {};
  RMPParmsType getRMPParms(void) { return Cfg; };
  DRCSignal *getMPSig() { return MPSignal; };
  DRCSignal *getEPSig() { return EPSignal; };
};
#endif
