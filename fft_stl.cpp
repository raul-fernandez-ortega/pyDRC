#include "fft_stl.h"


STLvectorComplex STL_Fftw(STLvectorReal Sig)
{
  DLComplex *OutSig = new DLComplex[Sig.size()];
  if( Fftw(Sig.data(), OutSig, Sig.size()) == true) {
    STLvectorComplex RetSig(OutSig, OutSig + Sig.size());
    delete OutSig;
    return RetSig;
  } else {
    STLvectorComplex RetSig;
    delete OutSig;
    return RetSig;
  }
}

STLvectorReal STL_IFftw(STLvectorComplex Sig)
{
  DLReal *OutSig = new DLReal[Sig.size()];
  if( IFftw(Sig.data(), OutSig, Sig.size()) == true) {
    STLvectorReal RetSig(OutSig, OutSig + Sig.size());
    delete OutSig;
    return RetSig;
  } else {
    STLvectorReal RetSig;
    delete OutSig;
    return RetSig;
  }
}




