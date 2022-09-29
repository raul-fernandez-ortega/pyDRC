#include "dspwind_stl.h"

void STL_HammingWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  HammingWindow(&ARef.data()[Start], Len);
  return;
}
void STL_HanningWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  HanningWindow(&ARef.data()[Start], Len);
  return;
}
void STL_BlackmanWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  BlackmanWindow(&ARef.data()[Start], Len);
  return;
}
void STL_BartlettWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)
{
  BartlettWindow(&ARef.data()[Start], Len);
  return;
}
void STL_BlackmanHarrisWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len)	
{
  BlackmanHarrisWindow(&ARef.data()[Start], Len);  
  return;
}
void STL_ExponentialWindow(STLvectorReal &ARef, DLReal P, unsigned int Start, unsigned int Len)
{
  ExponentialWindow(&ARef.data()[Start], Len, P);
  return;
}

// Finestratura parziale e spaziata
void STL_SpacedBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  SpacedBlackmanWindow(ARef.data(), Len, Space, WType);
  return;
}

void STL_HalfBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType)
{
  HalfBlackmanWindow(ARef.data(), Len, Space, WType);
  return;
}

STLvectorReal STL_Ones(unsigned int Size)
{ 
  STLvectorReal RetSig(Size, 1);
  return RetSig;
}

STLvectorReal STL_Delta(unsigned int Size, unsigned int delay)
{
  STLvectorReal RetSig(Size, 0);
  if(delay < Size)
    RetSig[delay] = 1.0;
  return RetSig;
  

}
