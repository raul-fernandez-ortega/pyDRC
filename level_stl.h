#ifndef Level_stl_h
#define Level_stl_h

#include "stl_type.h"
#include "level.h"
using namespace std;


double STL_GetRMSLevel(STLvectorReal *Sig);

double STL_GetBLRMSLevel(STLvectorReal *Sig, int SampleFreq, double StartFreq, double EndFreq, int MExp);

double STL_GetBLFFTRMSLevel(STLvectorComplex *FFTArray, int SampleFreq, DLReal StartFreq, DLReal EndFreq);

void STL_SigNormalize(STLvectorReal *Sig, DLReal NormFactor, NormType TNorm);

bool STL_LPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_C1LPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);


bool STL_HMPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_C1HMPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_LPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_C1LPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, const DLReal EndFreq, int MExp);

bool STL_HMPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_C1HMPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp);

bool STL_LPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp);

bool STL_CMPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp);

bool STL_HMPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp);

#endif
