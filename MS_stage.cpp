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

void MSstage::process(void)
{
  unsigned int I;
  DLReal *PSOutSig;
  DLReal *MPSig;
  OutSig->Data.clear();
  /* Alloca gli array per la deconvoluzione omomorfa */
  sputs("Allocating homomorphic deconvolution arrays.");
  MPSig = new DLReal[InSig->Data.size()];
  if (MPSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data.size();I++)
    MPSig[I] = 0;
  
  PSOutSig = new DLReal[InSig->Data.size()];
  if (PSOutSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  
  /* Azzera gli array */
  for (I = 0;I < InSig->Data.size();I++)
    PSOutSig[I] = InSig->Data[I];    
  /* Effettua la deconvoluzione omomorfa*/
  sputs("MP filter extraction homomorphic deconvolution stage...");
  if (CepstrumHD(PSOutSig,MPSig,NULL, InSig->Data.size(),Cfg.MSMultExponent) == false) {
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
    OutSig->Data.push_back(MPSig[I]);
  
  if (Cfg.MSNormFactor > 0) {
    sputs("Minimum phase filter normalization.");
    OutSig->Normalize(Cfg.MSNormFactor, Cfg.MSNormType);
  }
  
  /* Salva il il filtro a fase minima */
  sputsp("Saving MP filter signal: ",Cfg.MSOutFile);
  /* Verifica se deve essere estratto il filtro a fase minima */
  if (Cfg.MSOutFile != NULL) {
    if (OutSig->WriteSignal(Cfg.MSOutFile, Cfg.MSOutFileType) == false)	{
      sputs("MP filter signal save failed.");
      return;
    }
  } 
}
