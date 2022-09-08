#ifndef TC_stage_h
#define TC_stage_h
#include "stl_type.h"
#include "baseclass.h"
#include "baselib_stl.h"
#include "level_stl.h"
#include "convol_stl.h"

typedef struct
{
  /* Test convolution stage */
  DLReal TCNormFactor;
  char * TCNormType;
  char * TCOutFile;
  char * TCOutFileType;
  char * TCOWFile;
  char * TCOWFileType;
  DLReal TCOWNormFactor;
  char * TCOWNormType;
  int    TCOWSkip;
  int    TCOWPrewindow;
  int    TCOWLength;
  int    BCInitWindow;
  int    ISPELowerWindow;
  char * PSFilterType;
} TCParmsType;

class TCstage
{
 private:
  TCParmsType Cfg;
  DRCSignal *MsInSig;
  DRCSignal *FtInSig;
  DRCSignal *OutSig;

 public:
  TCstage(DRCSignal *MsInputSig, DRCSignal *FtInputSig, TCParmsType InCfg);
  void NewInputSignal(DRCSignal *MsInputSig);
  void NewFilterSignal(DRCSignal *FtInputSig);
  void NewInCfg(TCParmsType InCfg);
  ~TCstage();
  void process(void);
  void Normalize(void);
  void WriteOutput(void);
  TCParmsType getCfgParms(void) { return Cfg; };
  DRCSignal *getOutSig() { return OutSig; };
};
#endif
