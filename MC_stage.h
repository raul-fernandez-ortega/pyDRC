#ifndef MC_stage_h
#define MC_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "level.h"
#include "fir.h"
#include "hd.h"
#include "convol.h"

typedef struct
{
  char * MCFilterType;
  char * MCInterpolationType;
  int    MCMultExponent;
  int    MCFilterLen;
  int    MCNumPoints;
  char * MCPointsFile;
  char * MCMagType;
  char * MCFilterFile;
  char * MCFilterFileType;
  int    MCOutWindow;
  DLReal MCNormFactor;
  char * MCNormType;
  char * MCOutFile;
  char * MCOutFileType;
} MCParmsType;

class MCstage
{
 private:
  MCParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;
  DRCSignal *MCOutF;

 public:
  MCstage(DRCSignal *InputSig, MCParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(MCParmsType InCfg);
  ~MCstage() {};
  bool process(void);
  MCParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
};
#endif
