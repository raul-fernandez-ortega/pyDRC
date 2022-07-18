#ifndef MS_stage_h
#define MS_stage_h
#include "stl_type.h"
#include "baselib.h"
#include "baseclass.h"
#include "level.h"
#include "hd.h"

typedef struct
{
  /* Minimum phase filter extraction stage */
  int    MSMultExponent;
  int    MSOutWindow;
  DLReal MSNormFactor;
  char * MSNormType;
  char * MSOutFile;
  char * MSOutFileType;
} MSParmsType;


class MSstage
{
 private:
  MSParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;

 public:
  MSstage(DRCSignal *InputSig, MSParmsType InCfg);
  void NewInputSignal(DRCSignal *InputSig);
  void NewInCfg(MSParmsType InCfg);
  ~MSstage() {};
  void process(void);
  MSParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
};
#endif
