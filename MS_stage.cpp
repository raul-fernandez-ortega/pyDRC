#include "MS_stage.h"

MSstage::MSstage(DRCSignal *InputSig, MSParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
  process();

}

void MSstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  process();
}

void MSstage::NewInCfg(MSParmsType InCfg)
{
  Cfg = InCfg;
  process();
}

bool MSstage::process(void)
{
  unsigned int I;
  DLReal *PSOutSig;
  DLReal *MPSig;

  sputs("DRC: Minimum Phase Filter Extraction stage (MS).");
  OutSig->Data.clear();
  /* Alloca gli array per la deconvoluzione omomorfa */
  sputs("MS stage: allocating homomorphic deconvolution arrays.");
  MPSig = new DLReal[InSig->Data.size()];
  if (MPSig == NULL) {
    sputs("MS stage: memory allocation failed.");
    return false;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data.size();I++)
    MPSig[I] = 0;
  
  PSOutSig = new DLReal[InSig->Data.size()];
  if (PSOutSig == NULL) {
    sputs("MS stage: memory allocation failed.");
    return false;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data.size();I++)
    PSOutSig[I] = InSig->Data[I];    
  /* Effettua la deconvoluzione omomorfa*/
  sputs("MS stage: MP filter extraction homomorphic deconvolution stage...");
  if (CepstrumHD(PSOutSig,MPSig,NULL, InSig->Data.size(),Cfg.MSMultExponent) == false) {
    sputs("MS stage: homomorphic deconvolution failed.");
    return false;
  }
  
  /* Verifica se si deve finestrare il filtro */
  sputs("MS stage: MP filter extraction windowing.");
  if (Cfg.MSOutWindow > 0) {
    HalfBlackmanWindow(MPSig,Cfg.MSOutWindow,0,WRight);
    OutSig->setWLen(Cfg.MSOutWindow);
  } else
    OutSig->setWLen(InSig->getWLen());

  for(I = 0; I < (unsigned int)OutSig->getWLen(); I++)
    OutSig->Data.push_back(MPSig[I]);

  sputs("DRC: Finished Minimum Phase Filter Extraction stage (MS).");
  return true;
}


void MSstage::Normalize(void)
{
  if (Cfg.MSNormFactor > 0) {
    sputs("MS stage: output component normalization.");
    OutSig->Normalize(Cfg.MSNormFactor,Cfg.MSNormType);
  }
}

void MSstage::WriteOutput(void)
{
  if (Cfg.MSOutFile != NULL) {
    sputsp("MS stage: saving output component: ",Cfg.MSOutFile);
    if (OutSig->WriteSignal(Cfg.MSOutFile, Cfg.MSOutFileType) == false) {
      sputs("MS stage: output component save failed.");
    }
  }
}
