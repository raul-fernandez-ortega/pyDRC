#ifndef HD_stage_h
#define HD_stage_h
#include "stl_type.h"
#include "baselib_stl.h"
#include "baseclass.h"
#include "level_stl.h"
#include "hd.h"

typedef struct
{
  int HDMultExponent;
  DLReal HDMPNormFactor;
  char * HDMPNormType;
  char * HDMPOutFile;
  char * HDMPOutFileType;
  DLReal HDEPNormFactor;
  char * HDEPNormType;
  char * HDEPOutFile;
  char * HDEPOutFileType;
} HDParmsType;

class HDstage
{
 private:
  HDParmsType Cfg;
  DRCSignal *InSignal;
  DRCSignal *MPSignal;
  DRCSignal *EPSignal;

 public:
  HDstage(DRCSignal *InputSig, HDParmsType InCfg);
  ~HDstage() {};
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(HDParmsType InCfg);
  void process(void);
  void Normalize(void);
  void WriteOutput(void);
  HDParmsType getHDParms(void) { return Cfg; };
  DRCSignal *getMPSig(void) { return MPSignal; };
  DRCSignal *getEPSig(void) { return EPSignal; };
};
#endif
