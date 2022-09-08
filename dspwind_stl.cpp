#include "dspwind_stl.h"

STLvectorReal *STL_HammingWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  HammingWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}
STLvectorReal *STL_HanningWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  HanningWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}
STLvectorReal *STL_BlackmanWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  BlackmanWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}
STLvectorReal *STL_BartlettWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  BartlettWindow(&InSig[Start], Len);  
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}
STLvectorReal *STL_BlackmanHarrisWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len)	
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  BlackmanHarrisWindow(&InSig[Start], Len);  
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}
STLvectorReal *STL_ExponentialWindow(STLvectorReal *ARef, DLReal P, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef->at(I);
  ExponentialWindow(&InSig[Start], Len, P);
  for(I = 0; I < SigLen ; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}

// Finestratura parziale e spaziata
STLvectorReal *STL_SpacedBlackmanWindow(STLvectorReal *ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen; I++)
    InSig[I] = ARef->at(I);
  SpacedBlackmanWindow(InSig, SigLen, Space, WType);
  for(I = 0; I < SigLen; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}

STLvectorReal *STL_HalfBlackmanWindow(STLvectorReal *ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  unsigned int I, SigLen = ARef->size();
  DLReal *InSig = new DLReal[SigLen];
  STLvectorReal *OutSig = new STLvectorReal(SigLen);
  for(I = 0; I < SigLen; I++)
    InSig[I] = ARef->at(I);
  HalfBlackmanWindow(InSig, Len, Space, WType);
  for(I = 0; I < SigLen; I++)
    OutSig->at(I) = InSig[I];
  delete InSig;
  return OutSig;
}

STLvectorReal *STL_Ones(unsigned int Size)
{
  unsigned int I;
  DLReal *InSig = new DLReal[Size];
  STLvectorReal *RetSig = new STLvectorReal(Size);
  Ones(InSig, Size);
  for(I = 0; I < Size; I++)
    RetSig->at(I) = InSig[I];
  delete InSig;
  return RetSig;
}

STLvectorReal *STL_Delta(unsigned int Size, unsigned int delay)
{
  unsigned int I;
  STLvectorReal *RetSig = new STLvectorReal(Size);
  for(I = 0; I < Size; I++)
    RetSig->at(I) = (double) 0;
  if(delay < Size)
    RetSig->at(delay) = 1.0;
  else
    RetSig->at(Size) = 1.0;
  return RetSig;
  

}
