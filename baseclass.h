#ifndef baseclass_h
#define baseclass_h

#include <string.h>
#include <malloc.h>
#include "baselib_stl.h"
#include "stl_type.h"
#include "level_stl.h"
#include "dspwind_stl.h"
#include "toeplitz.h"
#include "kirkebyfd.h"
#include "slprefilt.h"

class DRCSignal
{

 private:
  unsigned int PreWindowLen;
  unsigned int PSStart;
  unsigned int PSEnd;
  unsigned int WStart;
  unsigned int WLen;
  int SampleRate;
  char *Path;

 public:
  STLvectorReal *Data;
  DRCSignal();
  ~DRCSignal() {};
  DRCSignal(DRCSignal *SCopy);
  void setParams(DRCSignal *SCopy);
  void setData(STLvectorReal *InData, int psstart, int psend);
  void setData(STLvectorReal *InData);
  void clearData(void);
  void setPreWindowLen(int prewindowlen);
  void setPSStart(int psstart);
  void setPSEnd(int psend);
  void setWStart(int wstart);
  void setWLen(int wlen);
  void setSampleRate(int srate);
  void setPath(char *path);
  STLvectorReal *getData(void);
  int getPreWindowLen(void);
  int getPSStart(void);
  int getPSEnd(void);
  int getWStart(void);
  int getWLen(void);
  int getSampleRate(void);
  char *getPath(void);
  void Normalize(DLReal normfactor, char* normtype);
  bool WriteSignal(char* outfile, char* outfiletype);
};

STLvectorReal *STL_ToeplitzSolve(STLvectorReal *A, STLvectorReal *B, unsigned int N);

STLvectorReal *STL_KirkebyFDInvert(STLvectorReal *InSig, unsigned int InSigLen, 
				  unsigned int InvFilterSigLen, STLvectorReal *EffortSig, 
				  DLReal EffortFactor, int MExp);

STLvectorReal *STL_PEISMPKirkebyFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig, 
					unsigned int InvFilterSigLen, DLReal EffortFactor, 
					int MExp);

STLvectorReal *STL_PEMSMPKirkebyFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig,
					unsigned int InvFilterLen, DLReal EffortFactor, 
					int PEStart, int PETransition, int MExp);

STLvectorReal *STL_PETFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig, 
			      unsigned int InvFilterLen, char PEType, int PELowerWindow, 
			      int PEUpperWindow, int PEStartFreq, int PEEndFreq, 
			      int PEFilterLen, DLReal FSharpness, int PEBandSplit, 
			      DLReal PEWindowExponent,const DLReal OGainFactor, int SampleRate, 
			      int MExp, const SLPPrefilteringType SLPType);

#endif
