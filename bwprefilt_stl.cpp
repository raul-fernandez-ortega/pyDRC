#include "bwprefilt_stl.h"

STLvectorReal *STL_BWPreFilt(STLvectorReal *Sig, int IBS, int FBS,
			     int FilterLen, int BandSplit, DLReal WindowExponent, 
			     int SampleFreq, DLReal StartFreq, DLReal EndFreq, 
			     int WindowGap, WindowType WType, BWPPrefilteringType BWPType)
{
  int MPPFSigLen = IBS + FilterLen - 1;
  STLvectorReal *RetSig = new STLvectorReal();
  int I,SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  DLReal *OutSig = new DLReal[MPPFSigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig->at(I);
  for(I = 0; I < MPPFSigLen; I++)
    OutSig[I] = 0;
  BWPreFilt(InSig, IBS, FBS, FilterLen, BandSplit, WindowExponent, SampleFreq, StartFreq, 
  	    EndFreq, WindowGap, OutSig, WType, BWPType);
  for(I = 0; I < MPPFSigLen; I++)
    RetSig->push_back(OutSig[I]);
  delete InSig;
  delete OutSig;
  return RetSig;
}

