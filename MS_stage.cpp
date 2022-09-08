#include "MS_stage.h"

MSstage::MSstage(DRCSignal *InputSig, MSParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(InputSig);
  Cfg = InCfg;
}

void MSstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  OutSig->setParams(InputSig);
}

void MSstage::NewInCfg(MSParmsType InCfg)
{
  Cfg = InCfg;
}

void MSstage::process(void)
{
  unsigned int I;
  DLReal *PSOutSig;
  DLReal *MPSig;
  OutSig->Data->clear();
  /* Alloca gli array per la deconvoluzione omomorfa */
  sputs("Allocating homomorphic deconvolution arrays.");
  MPSig = new DLReal[InSig->Data->size()];
  if (MPSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data->size();I++)
    MPSig[I] = 0;
  
  PSOutSig = new DLReal[InSig->Data->size()];
  if (PSOutSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data->size();I++)
    PSOutSig[I] = InSig->Data->at(I);    
  /* Effettua la deconvoluzione omomorfa*/
  sputs("MP filter extraction homomorphic deconvolution stage...");
  if (CepstrumHD(PSOutSig,MPSig,NULL, InSig->Data->size(),Cfg.MSMultExponent) == false) {
    sputs("Homomorphic deconvolution failed.");
    return ;
  }
  
  /* Verifica se si deve finestrare il filtro */
  sputs("MP filter extraction windowing.");
  if (Cfg.MSOutWindow > 0) {
    HalfBlackmanWindow(MPSig,Cfg.MSOutWindow,0,WRight);
    OutSig->setWLen(Cfg.MSOutWindow);
  } else
    OutSig->setWLen(InSig->getWLen());
  
  /* Normalizzazione segnale risultante */
  for(I = 0; I < (unsigned int)OutSig->getWLen(); I++)
    OutSig->Data->push_back(MPSig[I]);
  
  //Normalize();
  //WriteOutput();
  return;
}

void MSstage::Normalize(void)
{
  if (Cfg.MSNormFactor > 0) {
    sputs("Minimun phase filter normalization.");
    OutSig->Normalize(Cfg.MSNormFactor,Cfg.MSNormType);
  }
}

void MSstage::WriteOutput(void)
{
  if (Cfg.MSOutFile != NULL) {
    sputs("Saving Minimun phase filter.");
    OutSig->WriteSignal(Cfg.MSOutFile, Cfg.MSOutFileType);
  }
}

