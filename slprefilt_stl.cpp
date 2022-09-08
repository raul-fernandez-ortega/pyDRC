#include "slprefilt_stl.h"


STLvectorReal *STL_FastLowPassFir(const unsigned int Order,const DLReal Freq, STLvectorReal *Window, const unsigned int WSize)
{
  STLvectorReal *RetSig = new STLvectorReal();
  DLReal *Filter = new DLReal[Order];
  DLReal *FWindow = new DLReal[Window->size()];
  for(unsigned int i = 0; i < Window->size(); i++)
    FWindow[i] = Window->at(i);
  FastLowPassFir(Filter,Order,Freq, FWindow,WSize);
  for(unsigned int i = 0; i < Order; i++)
      RetSig->push_back(Filter[i]);
  delete Filter;
  delete FWindow;
  return RetSig;
}

STLvectorReal *STL_SLPreFilt(STLvectorReal *Sig, int IBS, int FBS, int FilterLen, int BandSplit, DLReal WindowExponent, int SampleFreq,
			     DLReal StartFreq, DLReal EndFreq, int WindowGap, DLReal FSharpness, WindowType WType, SLPPrefilteringType SLPType)
{
  int MPPFSigLen = IBS + FilterLen - 1;
  STLvectorReal *RetSig = new STLvectorReal();
  int SigLen = Sig->size();
  DLReal *InSig = new DLReal[SigLen];
  DLReal *OutSig = new DLReal[MPPFSigLen];
  for(int i = 0; i < SigLen; i++)
    InSig[i] = Sig->at(i);
  for(int i = 0; i < MPPFSigLen; i++)
    OutSig[i] = 0;
  SLPreFilt(InSig, IBS, FBS, FilterLen, BandSplit, WindowExponent, SampleFreq, StartFreq, EndFreq, WindowGap, FSharpness, OutSig, WType, SLPType);
  for(int i = 0; i < MPPFSigLen; i++)
      RetSig->push_back(OutSig[i]);
  delete InSig;
  delete OutSig;
  return RetSig;
}

