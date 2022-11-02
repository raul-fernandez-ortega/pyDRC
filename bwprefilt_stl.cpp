#include "bwprefilt_stl.h"

STLvectorReal STL_BWPreFilt(STLvectorReal Sig, int IBS, int FBS,
			int FilterLen, int BandSplit, DLReal WindowExponent, 
			int SampleFreq, DLReal StartFreq, DLReal EndFreq, 
			int WindowGap, WindowType WType, BWPPrefilteringType BWPType)
{
  int MPPFSigLen = IBS + FilterLen - 1;
  int I;
  DLReal *OutSig = new DLReal[MPPFSigLen];
  for(I = 0; I < MPPFSigLen; I++)
    OutSig[I] = 0;
  BWPreFilt(Sig.data(), IBS, FBS, FilterLen, BandSplit, WindowExponent, SampleFreq, StartFreq, 
  	    EndFreq, WindowGap, OutSig, WType, BWPType);
  STLvectorReal RetSig(OutSig, OutSig + MPPFSigLen);
  delete OutSig;
  return RetSig;
}

