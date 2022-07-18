#ifndef BWprefilt_stl_h
#define BWprefilt_stl_h

#include "stl_type.h"
#include "dsplib.h"
#include "bwprefilt.h"
#include "baselib.h"

using namespace std;

STLvectorReal STL_BWPreFilt(STLvectorReal Sig, int IBS, int FBS, int FilterLen, 
			    int BandSplit, DLReal WindowExponent, int SampleFreq, 
			    DLReal StartFreq, DLReal EndFreq, int WindowGap, 
			    WindowType WType,BWPPrefilteringType SLPType);



#endif
