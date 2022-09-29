#include "level_stl.h"

/* RMS level calculation */
double STL_GetRMSLevel(const STLvectorReal Sig)
{
  double RMSLevel;
  RMSLevel = (double) GetRMSLevel(Sig.data(),Sig.size());
  return RMSLevel;
  /*unsigned int I, SigLen = Sig.size();
  double RMSLevel;
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  RMSLevel = (double) GetRMSLevel(InSig,SigLen);
  delete InSig;
  return RMSLevel; */
}
 
/* Frequency band filtered RMS level calculation */
double STL_GetBLRMSLevel(const STLvectorReal Sig, const int SampleFreq, const DLReal StartFreq, 
			 const DLReal EndFreq, const int MExp)
{
  double BLRMSLevel;
  BLRMSLevel = (double) GetBLRMSLevel(Sig.data(), Sig.size(), SampleFreq, StartFreq, EndFreq, MExp);
  return BLRMSLevel;
  /*unsigned int I,SigLen = Sig.size();
  double BLRMSLevel;
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  BLRMSLevel = (double) GetBLRMSLevel(InSig, SigLen, SampleFreq, StartFreq, EndFreq, MExp);
  delete InSig;
  return BLRMSLevel; */
}

/* Frequency band filtered RMS level calculation. FFT input */
double STL_GetBLFFTRMSLevel(const STLvectorComplex FFTArray, const int SampleFreq, 
			    const DLReal StartFreq, const DLReal EndFreq)
{
  double BLRMSLevel;
  BLRMSLevel = (double) GetBLFFTRMSLevel(FFTArray.data(), FFTArray.size() , SampleFreq, StartFreq, EndFreq);
  return BLRMSLevel;
}

/* Signal normalization */
void STL_SigNormalize(STLvectorReal &Sig, const DLReal NormFactor, const NormType TNorm)
{
  SigNormalize(Sig.data(), Sig.size(), NormFactor, TNorm);
  return;
}

/* Linear phase Dip Limiting, with frequency band filtered RMS calculation */
bool STL_LPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		    const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
		    const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if (LPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Linear phase Dip Limiting, with frequency band filtered RMS calculation.*/ 
/* Keeping first derivative continuity on the limiting point */
bool STL_C1LPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
		      const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if (C1LPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
bool STL_HMPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		     const int SampleFreq, const DLReal StartFreq, 
		     const DLReal EndFreq, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if (HMPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
/* Keeping first derivative continuity on the limiting point */
bool STL_C1HMPDipLimit(STLvectorReal &Sig, const DLReal MinGain, const DLReal DLStart, 
		       const int SampleFreq, const DLReal StartFreq, 
		       const DLReal EndFreq, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if (C1HMPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}     

/* Linear phase Peak Limiting, with frequency band filtered RMS calculation */
bool STL_LPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
		     const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq,
		     const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if (LPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Linear phase Peak Limiting, with frequency band filtered RMS calculation.*/ 
/* Keeping first derivative continuity on the limiting point */
bool STL_C1LPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain,const DLReal PLStart, 
		       const int SampleFreq, const DLReal StartFreq, 
		       const DLReal EndFreq, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(C1LPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Peak Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
bool STL_HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
		      const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, 
		      const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(HMPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
/* Keeping first derivative continuity on the limiting point */
bool STL_C1HMPPeakLimit(STLvectorReal &Sig, const DLReal MaxGain, const DLReal PLStart, 
			const int SampleFreq, const DLReal StartFreq, 
			const DLReal EndFreq, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(C1HMPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Livellazione norma a fase lineare */
bool STL_LPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(LPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

bool STL_CMPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(CMPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Livellazione norma a fase minima */
/* Versione basata sulla trasformata di Hilbert */
bool STL_HMPNormFlat(STLvectorReal &Sig, const DLReal Gain, const DLReal OGainFactor, const int MExp)
{
  unsigned int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if(HMPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    for(I = 0; I < SigLen; I++)
      Sig[I] = InSig[I];
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}
