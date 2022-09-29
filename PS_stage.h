#ifndef PS_stage_h
#define PS_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "fir.h"
#include "hd.h"
#include "convol.h"

typedef struct
{
  char * PSFilterType;
  char * PSInterpolationType;
  int    PSMultExponent;
  int    PSFilterLen;
  int    PSNumPoints;
  char * PSPointsFile;
  char * PSMagType;
  int    PSOutWindow;
  DLReal PSNormFactor;
  char * PSNormType;
  char * PSOutFile;
  char * PSOutFileType;
  int    ISPELowerWindow;
} PSParmsType;

class PSstage
{
 private:
  PSParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;
  DRCSignal *PSOutF;

 public:
  PSstage(DRCSignal *InputSig, PSParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(PSParmsType InCfg);
  ~PSstage() {};
  void process(void);
  PSParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
  DRCSignal *getOutF() { return PSOutF; };
};
#endif
