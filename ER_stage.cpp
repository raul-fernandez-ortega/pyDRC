#include "ER_stage.h"

ERstage::ERstage(DRCSignal *EPInputSig, ERParmsType InCfg)
{
  Cfg = InCfg;
  EPInSig = EPInputSig;
  EPSignal = new DRCSignal(*EPInputSig);
}

void ERstage::NewInputSignal(DRCSignal *EPInputSig)
{
  EPInSig = EPInputSig;
  delete EPSignal;
  EPSignal = new DRCSignal(*EPInputSig);
}

void ERstage::NewInCfg(ERParmsType InCfg)
{
  Cfg = InCfg;
}

void ERstage::process(void)
{
  int I, J;
  int EPDataLen;
  DLReal *EPSig;
  DLReal *EPPFSig;

  EPDataLen = EPInSig->getData().size();
  EPSignal->clearData();
  EPPFSig = new DLReal[EPDataLen];
  for(I = 0; I < EPDataLen; I++) {
    EPPFSig[I] = EPInSig->Data[I];
  }
  /* Verifica se si deve effettuare riappianamento */
  if (Cfg.EREPFlatGain > 0) {
    switch (Cfg.EREPFlatType[0]) {
    case 'L':
      sputs("Excess phase component linear phase flattening...");
      LPNormFlat(&EPPFSig[EPInSig->getWStart()],EPInSig->getWLen(),Cfg.EREPFlatGain, Cfg.EREPOGainFactor,Cfg.EREPFGMultExponent);
      break;
      
    case 'M':
      sputs("Excess phase component minimum phase flattening...");
      CMPNormFlat(&EPPFSig[EPInSig->getWStart()],EPInSig->getWLen(),Cfg.EREPFlatGain, Cfg.EREPOGainFactor,Cfg.EREPFGMultExponent);
      break;
      
    case 'D':
      /* Alloca gli array per la deconvoluzione omomorfa */
      sputs("Allocating homomorphic deconvolution arrays.");
      EPSig = new DLReal[EPInSig->getWLen()];
      if (EPSig == NULL) {
	sputs("Memory allocation failed.");
	return;
      }
      
      /* Azzera gli array per la deconvoluzione omomorfa */
      for (I = 0;I < EPInSig->getWLen();I++)
	EPSig[I] = 0;
      
      /* Effettua la deconvoluzione omomorfa*/
      sputs("Excess phase component homomorphic deconvolution flattening...");
      if (CepstrumHD(&EPPFSig[EPInSig->getWStart()],NULL,EPSig, EPInSig->getWLen(),Cfg.EREPFGMultExponent) == false) {
	sputs("Homomorphic deconvolution failed.");
	return;
      }
      
      /* Copia il risultato nell'array destinazione */
      for (I = 0,J = EPInSig->getWStart();I < EPInSig->getWLen();I++,J++)
	EPPFSig[J] = EPSig[I];
      
      /* Delloca gli array per la deconvoluzione omomorfa */
      delete EPSig;
      break;
    }
  }
  /* Verifica se si deve effettuare la finestratura finale */
  if (Cfg.EREPFinalWindow > 0) {
    sputs("Excess phase component final windowing.");
    BlackmanWindow(&EPPFSig[EPInSig->getWStart()],EPInSig->getWLen());
  }
  
  for(I=0; I < EPInSig->getWStart() + EPInSig->getWLen(); I++)
    EPSignal->Data.push_back(EPPFSig[I]);
  EPSignal->setWStart(EPInSig->getWStart());
  EPSignal->setWLen(EPInSig->getWLen());
  EPSignal->Normalize(Cfg.EREPNormFactor, Cfg.EREPNormType);
  EPSignal->WriteSignal(Cfg.EREPOutFile,  Cfg.EREPOutFileType);
}
