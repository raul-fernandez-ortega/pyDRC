#ifndef IS_stage_h
#define IS_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "convol.h"

typedef struct
{
  unsigned int PCOutWindow; 
  DLReal       PCNormFactor;
  char*        PCNormType; 
  char*        PCOutFile;
  char*        PCOutFileType;
  char*        ISType;
  char *       ISPETType;
  char *       ISPrefilterFctn;
  int          ISPELowerWindow;
  int          ISPEUpperWindow;
  int          ISPEStartFreq;
  int          ISPEEndFreq;
  int          ISPEFilterLen;
  DLReal       ISPEFSharpness;
  int          ISPEBandSplit;
  DLReal       ISPEWindowExponent;
  DLReal       ISPEOGainFactor;
  int          ISSMPMultExponent;
  int          ISOutWindow;
  DLReal       ISNormFactor;
  char *       ISNormType;
  char *       ISOutFile;
  char *       ISOutFileType;
} ISParmsType;

class ISstage
{
 private:
  ISParmsType Cfg;
  DRCSignal *MPInSig;
  DRCSignal *EPInSig;
  DRCSignal *PCOutSig;
  DRCSignal *ISOutSig;

 public:
  ISstage(DRCSignal *MPInputSig, DRCSignal *EPInputSig, ISParmsType InCfg);
  void NewInputSignal(DRCSignal *MPInputSig, DRCSignal *EPInputSig);
  void NewInCfg(ISParmsType InCfg);
  ~ISstage();
  void process(void);
  ISParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getPCOutSig() { return PCOutSig; };
  DRCSignal *getISOutSig() { return ISOutSig; };
};
#endif
