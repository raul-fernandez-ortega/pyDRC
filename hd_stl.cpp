#include "hd_stl.h"

HD::HD(DRCSignal *InputSig, unsigned int start, unsigned int len, int InMExp )
{
  InSig = new DRCSignal(*InputSig);
  hd_start = start;
  hd_len = len;
  MExp = InMExp;
  MPSig = new DRCSignal(*InputSig);
  EPSig = new DRCSignal(*InputSig);
}
void HD::SetInputSignal(DRCSignal *InputSig, unsigned int start, unsigned int len)
{
  delete InSig;
  delete MPSig;
  delete EPSig;
  InSig = new DRCSignal(*InputSig);
  hd_start = start;
  hd_len = len;
  MPSig = new DRCSignal(*InputSig);
  EPSig = new DRCSignal(*InputSig);
}
void HD::SetMExp(int InMExp)
{
  MExp = InMExp;
}
void HD::process(void)
{
  STLvectorReal MPdata, EPdata;
  unsigned int I, J;
  unsigned int SigLen = InSig->Data.size();
  //MPSig->setData(MPSig->Data.clear());
  //EPSig->setData(EPSig->Data.clear());
  DLReal *hd_InSig = new DLReal[SigLen];
  DLReal *hd_MPSig = new DLReal[hd_len];
  DLReal *hd_EPSig = new DLReal[hd_len];
  for(I = 0; I < hd_start; I++) {
    hd_MPSig[I] = 0;
    hd_EPSig[I] = 0;
  }
  for(J =0, I = hd_start; J < hd_len; J++, I++) {
    hd_InSig[J] = InSig->Data[I];
    hd_MPSig[I] = 0;
    hd_EPSig[I] = 0;
  }
  for(I = hd_len; I < SigLen; I++) {
    hd_MPSig[I] = 0;
    hd_EPSig[I] = 0;
  }
  if (CepstrumHD(hd_InSig, hd_MPSig, hd_EPSig, hd_len, MExp) == true) {
    for(I = 0; I < SigLen; I++) {
      MPdata.push_back(hd_MPSig[I]);
      EPdata.push_back(hd_EPSig[I]);
    }
    MPSig->setData(MPdata,0,MPdata.size());
    EPSig->setData(EPdata,0,EPdata.size());
  }
}
