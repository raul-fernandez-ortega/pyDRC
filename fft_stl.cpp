#include "fft_stl.h"


STLvectorComplex STL_Fftw(STLvectorReal Sig)
{
  int I,SigLen = Sig.size();
  DLReal *InSig = new DLReal[SigLen];
  DLComplex *OutSig = new DLComplex[SigLen];
  STLvectorComplex RetSig(Sig.size());
  for(I = 0; I < SigLen; I++)
      InSig[I] = Sig[I];
  if( Fftw(InSig, OutSig, SigLen) == true) {
    for(I = 0; I < SigLen; I++)
      RetSig[I] = OutSig[I];
  }
  delete InSig;
  delete OutSig;
  return RetSig;
}

STLvectorReal STL_IFftw(STLvectorComplex Sig)
{
  int I,SigLen = Sig.size();
  DLComplex *InSig = new DLComplex[SigLen];
  DLReal *OutSig = new DLReal[SigLen];
  STLvectorReal RetSig(Sig.size());
  for(I = 0; I < SigLen; I++)
    InSig[I] = Sig[I];
  if( IFftw(InSig, OutSig, SigLen) == true) {
    for(I = 0; I < SigLen; I++)
      RetSig[I] = OutSig[I];
  }
  delete InSig;
  delete OutSig;

  return RetSig;
}




