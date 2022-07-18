#include "dspwind_stl.h"

void STL_HammingWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  HammingWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}
void STL_HanningWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen]; 
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  HanningWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}
void STL_BlackmanWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  BlackmanWindow(&InSig[Start], Len);
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}
void STL_BartlettWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  BartlettWindow(&InSig[Start], Len);  
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}
void STL_BlackmanHarrisWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)	
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  BlackmanHarrisWindow(&InSig[Start], Len);  
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}
void STL_ExponentialWindow(STLvectorReal &ARef, DLReal P, unsigned int Start, unsigned int Len)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen]; 
  for(I = 0; I < SigLen ; I++)
    InSig[I] = ARef[I];
  ExponentialWindow(&InSig[Start], Len, P);
  for(I = 0; I < SigLen ; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}

// Finestratura parziale e spaziata
void STL_SpacedBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = ARef[I];
  SpacedBlackmanWindow(InSig, Len, Space, WType);
  for(I = 0; I < SigLen; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}

void STL_HalfBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  unsigned int I, SigLen = ARef.size();
  DLReal *InSig = new DLReal[SigLen];
  for(I = 0; I < SigLen; I++)
    InSig[I] = ARef[I];
  HalfBlackmanWindow(InSig, Len, Space, WType);
  for(I = 0; I < SigLen; I++)
    ARef[I] = InSig[I];
  delete InSig;
  return;
}

STLvectorReal STL_Ones(unsigned int Size)
{
  unsigned int I;
  DLReal *InSig = new DLReal[Size];
  STLvectorReal RetSig(Size);
  Ones(InSig, Size);
  for(I = 0; I < Size; I++)
    RetSig[I] = InSig[I];
  delete InSig;
  return RetSig;
}

STLvectorReal STL_Delta(unsigned int Size, unsigned int delay)
{
  unsigned int I;
  STLvectorReal RetSig(Size);
  for(I = 0; I < Size; I++)
    RetSig[I] = (double) 0;
  if(delay < Size)
    RetSig[delay] = 1.0;
  else
    RetSig[Size] = 1.0;
  return RetSig;
  

}
