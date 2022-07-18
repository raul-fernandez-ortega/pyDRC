#include "HD_stage.h"

HDstage::HDstage(DRCSignal *InputSig, HDParmsType InCfg)
{
  Cfg = InCfg;
  InSignal = InputSig;
  MPSignal = new DRCSignal(*InputSig);
  EPSignal = new DRCSignal(*InputSig);
  //process();
}

void HDstage::NewInputSignal(DRCSignal *InputSig)
{
  InSignal = InputSig;
  MPSignal = new DRCSignal(*InputSig);
  EPSignal = new DRCSignal(*InputSig);
}

void HDstage::NewInCfg(HDParmsType InCfg)
{
  Cfg = InCfg;
}

void HDstage::process(void)
{
  unsigned int I;
  unsigned int inDataSize;
  DLReal *InSig;
  DLReal *MPSig;
  DLReal *EPSig;

  MPSignal->clearData();
  EPSignal->clearData();  
  inDataSize = InSignal->getData().size();

  sputs("Allocating homomorphic deconvolution arrays.");
  InSig = new DLReal[inDataSize];
  if (InSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  MPSig = new DLReal[2 * inDataSize];
  if (MPSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  EPSig = new DLReal[inDataSize];
  if (EPSig == NULL) {
    sputs("Memory allocation failed.");
    return ;
  }
  
  /* Azzera gli array */
  for(I = 0; I < inDataSize; I++)
      InSig[I] = InSignal->Data[I];
  for (I = 0;I < 2 * inDataSize; I++)
    MPSig[I] = 0;
  for (I = 0;I < inDataSize; I++)
    EPSig[I] = 0;

  /* Effettua la deconvoluzione omomorfa*/
  sputs("Homomorphic deconvolution stage...");
  if (CepstrumHD(InSig,&MPSig[inDataSize/2-(1-(inDataSize%2))], EPSig, inDataSize, Cfg.HDMultExponent) == false) {
    sputs("Homomorphic deconvolution failed.");
    return;
  }
  for(I=0; I < inDataSize; I++) {
    MPSignal->Data.push_back(MPSig[I]);
    EPSignal->Data.push_back(EPSig[I]);
  }
  Normalize();
  WriteOutput();
}

void HDstage::Normalize(void)
{
  if (Cfg.HDMPNormFactor > 0) {
    sputs("Minimum phase component normalization.");
    MPSignal->Normalize(Cfg.HDMPNormFactor,Cfg.HDMPNormType);
  }
  if (Cfg.HDEPNormFactor > 0) {
    sputs("Excess phase component normalization.");
    EPSignal->Normalize(Cfg.HDEPNormFactor,Cfg.HDEPNormType);
  }
}
void HDstage::WriteOutput(void)
{
  if (Cfg.HDMPOutFile != NULL) {
    sputsp("Saving minimum phase component: ",Cfg.HDMPOutFile);
    if (MPSignal->WriteSignal(Cfg.HDMPOutFile, Cfg.HDMPOutFileType) == false) {
      sputs("Minimum phase component save failed.");
    }
  }
  if (Cfg.HDEPOutFile != NULL) {
    sputsp("Saving excess phase component: ",Cfg.HDEPOutFile);
    EPSignal->setWStart(0);
    if (EPSignal->WriteSignal(Cfg.HDEPOutFile, Cfg.HDEPOutFileType) == false) {
      sputs("Excess phase component save failed.");
    }
  }
}
