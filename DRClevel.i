%module DRClevel
%{
#include "level_stl.h"
%}

%rename(GetRMSLevel)      STL_GetRMSLevel;
%rename(GetBLRMSLevel)    STL_GetBLRMSLevel;
%rename(GetBLFFTRMSLevel) STL_GetBLFFTRMSLevel;
%rename(SigNormalize)     STL_SigNormalize;
%rename(LPDipLimit)       STL_LPDipLimit;
%rename(C1LPDipLimit)     STL_C1LPDipLimit;
%rename(HMPDipLimit)      STL_HMPDipLimit;
%rename(C1HMPDipLimit)    STL_C1HMPDipLimit;
%rename(LPPeakLimit)      STL_LPPeakLimit;
%rename(C1LPPeakLimit)    STL_C1LPPeakLimit;
%rename(HMPPeakLimit)     STL_HMPPeakLimit;
%rename(C1HMPPeakLimit)   STL_C1HMPPeakLimit;
%rename(LPNormFlat)       STL_LPNormFlat;
%rename(CMPNormFlat)      STL_CMPNormFlat;
%rename(HMPNormFlat)      STL_HMPNormFlat;

%include typemaps.i
%include "stl_type.h"
%include level_stl.h

%feature("autodoc", "
Returns input signal RMS level.
") STL_GetRMSLevel;

%feature("autodoc", "
GetBLRMSLevel(const STLvectorReal Sig, const int SampleFreq, const double StartFreq,
	      const double EndFreq, const int MExp) --> double
Returns input signal RMS level on a given frequency band.
Input parameters: 
    - Sig: input signal as numarray.
    - SampleFrequency: Sig sample frequency.
    - StartFreq: frequency band beginning.
    - EndFreq: frequency band end.
    - MExp: FFT lenght as 2**MExp. If zero, FFT length equals Sig length.
") STL_GetBLRMSLevel;

%feature("autodoc", "
GetBLFFTRMSLevel(const STLvectorComplex FFTArray, const int SampleFreq, 
		 const DLReal StartFreq, const DLReal EndFreq) --> double
Returns input signal RMS level on a given frequency band.
Input parameters: 
    - Sig: FFT input signal as complex numbers numarray.
    - SampleFrequency: Sig sample frequency.
    - StartFreq: frequency band beginning.
    - EndFreq: frequency band end.
") STL_GetBLFFTRMSLevel;

%feature("autodoc", "
SigNormalize(STLvectorReal Sig, const DLReal NormFactor, const NormType TNorm) --> STLvectorReal
Signal normalization.
Input parameters:
    - Sig: Input signal as numarray.
    - NormFactor: Normalization factor.
    - Normtype: Normalization type. 'M' - maximum. 'S' - sum. 'E' - Euclidean.
") STL_SigNormalize;

%feature("autodoc", "
LPDipLimit(STLvectorReal Sig, const DLReal MinGain, const DLReal DLStart, 
	   const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	   const int MExp) --> STLvectorReal
Linear phase dip limit calculation. RMS calculated over a given frequency 
band as reference.
Input parameters:
   - Sig: input signal as numarray.
   - MinGain: Minimal gain limit as RMS factor.
   - DLStart:  >=1.0 Hard signal truncation. <1.0 soft signal truncation with
     DLStart factor.
   - StartFreq: starting frequency range point for RMS signal calculation.
   - EndFreq: final frequency range point for RMS signal calculation.
   - MExp: FFT lenght as 2**MExp. If zero, FFT length equals Sig length.
") STL_LPDipLimit;

%feature("autodoc", "
C1LPDipLimit(STLvectorReal Sig, const DLReal MinGain, const DLReal DLStart, 
	     const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	     const int MExp) --> STLvectorReal
Linear phase dip limit calculation. Similar as LPDipLimit but with first 
derivative continuity conservation over dip limit point.
") STL_C1LPDipLimit;

%feature("autodoc", "
HMPDipLimit(STLvectorReal Sig, const DLReal MinGain, const DLReal DLStart, 
	    const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	    const int MExp) --> STLvectorReal
Minimum phase dip limit calculation. RMS calculated over a given frequency 
band as reference.
Input parameters:
    - Sig: input signal as numarray.
    - MinGain: Minimal gain limit as RMS factor.
    - DLStart:  >=1.0 Hard signal truncation. <1.0 soft signal truncation with
      DLStart factor.
    - StartFreq: starting frequency range point for RMS signal calculation.
    - EndFreq: final frequency range point for RMS signal calculation.
    - MExp: FFT lenght as 2**MExp. If zero, FFT length equals Sig length.
") STL_HMPDipLimit;

%feature("autodoc", "
C1HMPDipLimit(STLvectorReal Sig, const DLReal MinGain, const DLReal DLStart, 
	      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	      const int MExp) --> STLvectorReal
Minimum phase dip limit calculation. Similar as HMPDipLimit but with first
derivative continuity conservation over dip limit point.
") STL_C1HMPDipLimit;

%feature("autodoc", "
LPPeakLimit(STLvectorReal Sig, const DLReal MaxGain, const DLReal PLStart, 
	    const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	    const int MExp) --> STLvectorReal
Linear phase peak limit calculation. RMS calculated over a given frequency 
band as reference.
Input parameters:
    - Sig: input signal as numarray.
    - MaxGain: Maximal gain limit as RMS factor.
    - DLStart:  >=1.0 Hard signal truncation. <1.0 soft signal truncation with
      DLStart factor.
    - StartFreq: starting frequency range point for RMS signal calculation.
    - EndFreq: final frequency range point for RMS signal calculation.
    - MExp: FFT lenght as 2**MExp. If zero, FFT length equals Sig length.
" )  STL_LPPeakLimit;

%feature("autodoc", "
C1LPPeakLimit(STLvectorReal Sig, const DLReal MaxGain,const DLReal PLStart, 
	      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	      const int MExp) --> STLvectorReal
Linear phase peak limit calculation. Similar as LPPeakLimit but with first
derivative continuity conservation over dip limit point.
") STL_C1LPPeakLimit;

%feature("autodoc", "
HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
	     const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
             const int MExp) --> STLvectorReal
Minimum phase peak limit calculation. RMS calculated over a given frequency 
band as reference.
Input parameters:
    - Sig: input signal as numarray.
    - MaxGain: Maximal gain limit as RMS factor.
    - DLStart:  >=1.0 Hard signal truncation. <1.0 soft signal truncation with
      DLStart factor.
    - StartFreq: starting frequency range point for RMS signal calculation.
    - EndFreq: final frequency range point for RMS signal calculation.
    - MExp: FFT lenght as 2**MExp. If zero, FFT length equals Sig length.
") STL_HMPPeakLimit;

%feature("autodoc", "
C1HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
	       const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
	       const int MExp) --> STLvectorReal
 Minimum phase peak limit calculation. Similar as HMPPeakLimit but with 
first derivative continuity conservation over dip limit point.
") STL_C1HMPPeakLimit;

%feature("autodoc", "1")  STL_LPNormFlat;
%feature("autodoc", "1") STL_CMPNormFlat;
%feature("autodoc", "1") STL_HMPNormFlat;
