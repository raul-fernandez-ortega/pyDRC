#ifndef WF_stage_h
#define WF_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "slprefilt_stl.h"
#include "bwprefilt_stl.h"

typedef struct
{
  char *   WFfilterType;
  char *   WFfilterFctn;
  int      WFWindowGap;
  int      WFLowerWindow;
  int      WFUpperWindow;
  DLReal   WFStartFreq;
  DLReal   WFEndFreq;
  DLReal   WFWindowExponent;
  int      WFFilterLen;
  DLReal   WFFSharpness;
  int      WFBandSplit;
  DLReal   NormFactor;
  char *   NormType;
  char *   OutFile;
  char *   OutFileType;
  int      WFPreWindowLen;
  int      WFFinalWindow;
  char *   WFRTFlag;
} WFParmsType;

class WFstage
{
 private:
  WFParmsType Cfg;
  DRCSignal *InSig;
  DRCSignal *OutSig;

 public:
  WFstage(DRCSignal *InputSig, WFParmsType InCfg);
  void NewInputSignal(DRCSignal* InputSig);
  void NewInCfg(WFParmsType InCfg);
  ~WFstage() {};
  bool process(void);
  WFParmsType getWFParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
  void Normalize(void);
  void WriteOutput(void);
  
  
};
#endif
