#ifndef BC_stage_h
#define BC_stage_h
#include "baselib.h"
#include "baseclass.h"
#include "stl_type.h"
#include "level.h"
#include "dspwind.h"

typedef struct
{
  /* Base configuration */
  char *    BCBaseDir;
  char *    BCInFile;
  char *    BCInFileType;
  int       BCSampleRate;
  char *    BCImpulseCenterMode;
  int       BCImpulseCenter;
  int       BCInitWindow;
  int       BCPreWindowLen;
  int       BCPreWindowGap;
  char *    BCDLType;
  DLReal    BCDLMinGain;
  DLReal    BCDLStartFreq;
  DLReal    BCDLEndFreq;
  DLReal    BCDLStart;
  DLReal    BCDLMultExponent;
  DLReal    NormFactor;
  char *    NormType;
  char *    OutFile;
  char *    OutFileType;
} BCParmsType;

class BCstage
{
 private:

  BCParmsType Cfg;
  DRCSignal *OutSignal;
  
 public:
  BCstage(BCParmsType InCfg);
  ~BCstage() {};
  void NewCfgParms(BCParmsType InCfg);
  bool process(void);
  bool SeekImpulseCenter(void);
  bool ReadImpulseFile(void);
  BCParmsType getCfgParms() { return Cfg; };
  DRCSignal* getOutSig() { return OutSignal; };
  void Normalize(void);
  void WriteOutput(void);
};

#endif
