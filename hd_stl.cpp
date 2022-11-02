#include "hd_stl.h"

bool STL_CepstrumHD(const STLvectorReal In, STLvectorReal& MPOut, STLvectorReal& EPOut, const int N, const int MExp)
{
  unsigned int I;
  unsigned int SigLen = In.size();
  DLReal *hd_MPSig = new DLReal[SigLen];
  DLReal *hd_EPSig = new DLReal[SigLen];
  
  if (CepstrumHD(In.data(), hd_MPSig, hd_EPSig, SigLen, MExp) == true) {
    MPOut.clear();
    EPOut.clear();
    for(I = 0; I < SigLen; I++) {
      MPOut.push_back(hd_MPSig[I]);
      EPOut.push_back(hd_EPSig[I]);
    }
    return true;
  } else {
    return false;
  }
}
bool STL_HilbertHD(const STLvectorReal In, STLvectorReal& MPOut, STLvectorReal& EPOut, const int N, const int MExp)
{
  unsigned int I;
  unsigned int SigLen = In.size();
  DLReal *hd_MPSig = new DLReal[SigLen];
  DLReal *hd_EPSig = new DLReal[SigLen];
  
  if (HilbertHD(In.data(), hd_MPSig, hd_EPSig, SigLen, MExp) == true) {
    MPOut.clear();
    EPOut.clear();
    for(I = 0; I < SigLen; I++) {
      MPOut.push_back(hd_MPSig[I]);
      EPOut.push_back(hd_EPSig[I]);
    }
    return true;
  } else {
    return false;
  }
}
