#include "slprefilt_stl.h"

STLvectorReal STL_SLPreFilt(STLvectorReal Sig, int IBS, int FBS,
			int FilterLen, int BandSplit, DLReal WindowExponent, 
			int SampleFreq, DLReal StartFreq, DLReal EndFreq, 
			int WindowGap, DLReal FSharpness, WindowType WType, 
			SLPPrefilteringType SLPType)
{
  int MPPFSigLen = IBS + FilterLen - 1;
  int I;
  DLReal *OutSig = new DLReal[MPPFSigLen];
  for(I = 0; I < MPPFSigLen; I++)
    OutSig[I] = 0;
  SLPreFilt(Sig.data(), IBS, FBS, FilterLen, BandSplit, WindowExponent, SampleFreq, StartFreq, 
  	    EndFreq, WindowGap, FSharpness, OutSig, WType, SLPType);
  STLvectorReal RetSig(OutSig, OutSig + MPPFSigLen);
  delete OutSig;
  return RetSig;
}

