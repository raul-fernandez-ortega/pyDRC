#ifndef SLprefilt_stl_h
#define SLprefilt_stl_h

#include "stl_type.h"
#include "dsplib.h"
#include "slprefilt.h"
using namespace std;

STLvectorReal STL_SLPreFilt(STLvectorReal Sig, int IBS, int FBS, int FilterLen, 
			    int BandSplit, DLReal WindowExponent, int SampleFreq, 
			    DLReal StartFreq, DLReal EndFreq, int WindowGap, 
			    DLReal FSharpness,const WindowType WType,SLPPrefilteringType SLPType);



#endif
