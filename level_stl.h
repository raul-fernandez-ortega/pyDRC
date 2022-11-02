#ifndef Level_stl_h
#define Level_stl_h

#include "stl_type.h"
#include "level.h"
using namespace std;


double STL_GetRMSLevel(const STLvectorReal Sig);

double STL_GetBLRMSLevel(const STLvectorReal Sig, const int SampleFreq, const double StartFreq,
			 const double EndFreq, const DLReal W, const int MExp);

double STL_GetBLFFTRMSLevel(const STLvectorComplex FFTArray, const int SampleFreq, 
			    const DLReal StartFreq, const DLReal EndFreq, const DLReal W);

void STL_SigNormalize(STLvectorReal &Sig, const DLReal NormFactor, const NormType TNorm);

bool STL_LPDipLimit(STLvectorReal& Sig, const DLReal MinGain, const DLReal DLStart, 
		    const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		    const int MExp);

bool STL_C1LPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		      const int MExp);


bool STL_HMPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		     const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		     const int MExp);

bool STL_C1HMPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		       const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		       const int MExp);



bool STL_LPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
		     const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		     const int MExp);

bool STL_C1LPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain,const DLReal PLStart, 
		       const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, 
		       const int MExp);

bool STL_HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
		      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
		      const int MExp);

bool STL_C1HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
			const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,
			const int MExp);

bool STL_LPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, 
		    const int MExp);


bool STL_CMPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, 
		     const int MExp);

bool STL_HMPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, 
		     const int MExp);
#endif
