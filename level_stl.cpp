#include "level_stl.h"

/* RMS level calculation */
double STL_GetRMSLevel(STLvectorReal *Sig)
{
  unsigned int i, SigLen = Sig->size();
  double RMSLevel;
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  RMSLevel = (double) GetRMSLevel(InSig,SigLen);
  delete InSig;
  return RMSLevel;
}
 
/* Frequency band filtered RMS level calculation */
double STL_GetBLRMSLevel(STLvectorReal *Sig, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  double BLRMSLevel;
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  BLRMSLevel = (double) GetBLRMSLevel(InSig, SigLen, SampleFreq, StartFreq, EndFreq, MExp);
  delete InSig;
  return BLRMSLevel;
}

/* Frequency band filtered RMS level calculation. FFT input */
double STL_GetBLFFTRMSLevel(STLvectorComplex *FFTArray, int SampleFreq, DLReal StartFreq, DLReal EndFreq)
{
  unsigned int i, SigLen = FFTArray->size();
  double BLRMSLevel;
  DLComplex *InFFTArray = new DLComplex[SigLen];
  for(i = 0; i < SigLen; i++)
    InFFTArray[i] = (STLComplex) FFTArray->at(i);
  BLRMSLevel = (double) GetBLFFTRMSLevel(InFFTArray, SigLen, SampleFreq, StartFreq, EndFreq);
  delete InFFTArray;
  return BLRMSLevel;
}

/* Signal normalization */
void STL_SigNormalize(STLvectorReal *Sig, DLReal NormFactor, NormType TNorm)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  SigNormalize(InSig, SigLen, NormFactor, TNorm);
  for(i = 0; i < SigLen; i++)
    Sig->at(i) = InSig[i];
  delete InSig;
  return;
}

/* Linear phase Dip Limiting, with frequency band filtered RMS calculation */
bool STL_LPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if (LPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Linear phase Dip Limiting, with frequency band filtered RMS calculation.*/ 
/* Keeping first derivative continuity on the limiting point */
bool STL_C1LPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if (C1LPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
bool STL_HMPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if (HMPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
/* Keeping first derivative continuity on the limiting point */
bool STL_C1HMPDipLimit(STLvectorReal *Sig, DLReal MinGain, DLReal DLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if (C1HMPDipLimit(InSig, SigLen, MinGain, DLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}     

/* Linear phase Peak Limiting, with frequency band filtered RMS calculation */
bool STL_LPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if (LPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Linear phase Peak Limiting, with frequency band filtered RMS calculation.*/ 
/* Keeping first derivative continuity on the limiting point */
bool STL_C1LPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(C1LPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Peak Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
bool STL_HMPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(HMPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Minimum phase Dip Limiting with frequency band filtered RMS calculation. Hilbert Transform version */
/* Keeping first derivative continuity on the limiting point */
bool STL_C1HMPPeakLimit(STLvectorReal *Sig, DLReal MaxGain, DLReal PLStart, int SampleFreq, DLReal StartFreq, DLReal EndFreq, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(C1HMPPeakLimit(InSig, SigLen, MaxGain, PLStart, SampleFreq, StartFreq, EndFreq, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Livellazione norma a fase lineare */
bool STL_LPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(LPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

bool STL_CMPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(CMPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}

/* Livellazione norma a fase minima */
/* Versione basata sulla trasformata di Hilbert */
bool STL_HMPNormFlat(STLvectorReal *Sig, DLReal Gain, DLReal OGainFactor, int MExp)
{
  unsigned int i,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  for(i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  if(HMPNormFlat(InSig, SigLen, Gain, OGainFactor, MExp) == true) {
    Sig->clear();
    for(i = 0; i < SigLen; i++)
      Sig->push_back(InSig[i]);
    delete InSig;
    return true;
  } else {
    delete InSig;
    return false;
  }
}
