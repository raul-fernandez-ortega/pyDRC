#include "slprefilt_stl.h"

STLvectorReal STL_SLPreFilt(STLvectorReal Sig, int IBS, int FBS,
			int FilterLen, int BandSplit, DLReal WindowExponent, 
			int SampleFreq, DLReal StartFreq, DLReal EndFreq, 
			int WindowGap, DLReal FSharpness, WindowType WType, 
			SLPPrefilteringType SLPType)
{
  int MPPFSigLen = IBS + FilterLen - 1;
  STLvectorReal RetSig;
  int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  DLReal *OutSig = new DLReal[MPPFSigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  for(I = 0; I < MPPFSigLen; I++)
    OutSig[I] = 0;
  SLPreFilt(InSig, IBS, FBS, FilterLen, BandSplit, WindowExponent, SampleFreq, StartFreq, 
  	    EndFreq, WindowGap, FSharpness, OutSig, WType, SLPType);
  for(I = 0; I < MPPFSigLen; I++)
      RetSig.push_back(OutSig[I]);
  return RetSig;
}

