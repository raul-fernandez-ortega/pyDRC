#include "HR_stage.h"

HRstage::HRstage(DRCSignal *MPInputSig,DRCSignal *EPInputSig, HRParmsType InCfg)
{
  Cfg = InCfg;
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  MPSignal = new DRCSignal(*MPInputSig);
  EPSignal = new DRCSignal(*EPInputSig);
}

void HRstage::NewInputSignal(DRCSignal *MPInputSig, DRCSignal *EPInputSig)
{
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  delete MPSignal;
  delete EPSignal;
  MPSignal = new DRCSignal(*MPInputSig);
  EPSignal = new DRCSignal(*EPInputSig);
}

void HRstage::NewInCfg(HRParmsType InCfg)
{
  Cfg = InCfg;
}

void HRstage::process(void)
{
  int I, J;
  int MPEPSigLen, MPDataLen, EPDataLen;
  DLReal *MPPFSig;
  DLReal *MPSig;
  DLReal *MPEPSig;
  DLReal *EPSig;
  DLReal *EPPFSig;

  MPDataLen = MPInSig->getData().size();
  EPDataLen = EPInSig->getData().size();
  MPSignal->clearData();
  EPSignal->clearData();
  MPPFSig = new DLReal[MPDataLen];
  for(I = 0; I < MPDataLen; I++) {
    MPPFSig[I] = MPInSig->Data[I];
  }
  EPSig = new DLReal[EPDataLen];
  for(I = 0; I < EPDataLen; I++) {
    EPSig[I] = EPInSig->Data[I];
  }
  if (Cfg.HRMPHDRecover[0] == 'Y') {
    sputs("Allocating homomorphic deconvolution arrays.");
    MPSig = new DLReal[2 * MPInSig->getWLen()];
    if (MPSig == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
      
    /* Azzera gli array */
    for (I = 0;I < 2 * MPInSig->getWLen();I++)
      MPSig[I] = 0;
    
    /* Controlla se si deve preservare la componente EP della fase minima */
    if (Cfg.HRMPEPPreserve[0] == 'Y') {
      MPEPSig = new DLReal[MPInSig->getWLen()];
      if (MPEPSig == NULL) {
	sputs("Memory allocation failed.");
	return;
      }
	  
      /* Azzera gli array */
      for (I = 0;I < MPInSig->getWLen();I++)
	MPEPSig[I] = 0;
    } else
      MPEPSig = NULL;
    
    /* Effettua la deconvoluzione omomorfa*/
    sputs("MP Recover homomorphic deconvolution stage...");
    if (CepstrumHD(&MPPFSig[MPInSig->getWStart()], &MPSig[MPInSig->getWLen() / 2 - (1 - (MPInSig->getWLen() % 2))],MPEPSig,
		   MPInSig->getWLen(),Cfg.HRMPHDMultExponent) == false) {
      sputs("Homomorphic deconvolution failed.");
      return;
    }
      
    /* Ricopia la componente MP nell'array originale */
    for (I = 0,J = MPInSig->getWStart();I < MPInSig->getWLen();I++,J++)
      MPPFSig[J] = MPSig[I];
    
    /* Dealloca l'array deconvoluzione */
    delete MPSig;
  }
  
  /* Verifica se si deve effettuare la finestratura finale */
  if (Cfg.HRMPFinalWindow > 0) {
    sputs("Minimum phase component final windowing.");
    BlackmanWindow(&MPPFSig[MPInSig->getWStart()],MPInSig->getWLen());
    
    /* Controlla se si deve preservare la componente EP della fase minima */
    if (Cfg.HRMPHDRecover[0] == 'Y' && Cfg.HRMPEPPreserve[0] == 'Y')
      /* Effettua la finestratura della componente EP */
      BlackmanWindow(MPEPSig,MPInSig->getWLen());
  }
  for(I=0; I < MPDataLen; I++)
    MPSignal->Data.push_back(MPPFSig[I]);
  
  /* Controlla se si deve preservare la componente EP della fase minima */
  if (Cfg.HRMPHDRecover[0] == 'Y' && Cfg.HRMPEPPreserve[0] == 'Y') {
    /* Alloca l'array per la convoluzione */
    sputs("Allocating minimum phase EP recovering arrays.");
    MPEPSigLen = MPInSig->getWLen() + EPInSig->getWLen() - 1;
    EPPFSig = new DLReal[MPEPSigLen];
    if (EPSig == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Effettua la convoluzione */
    sputs("Minimum phase EP recovering...");
    if (FftwConvolve(MPEPSig,MPInSig->getWLen(),EPSig,EPInSig->getWLen(),EPPFSig) == false) {
      sputs("Convolution failed.");
      return ;
    }
    /* Recupera la componente EP */
    for (I = 0, J= MPInSig->getWLen() / 2; I < MPInSig->getWLen();I++, J++) {
      EPSig[I] = EPPFSig[J];
    }
    /* Dealloca l'array temporaneo convoluzione */
    //delete MPEPSig;
    //delete EPSig;
  }
  /* Recupera la componente EP */
  
  for(I = MPInSig->getWLen() / 2; I < MPEPSigLen; I++)
    EPSignal->Data.push_back(EPPFSig[I]);
  EPSignal->setWStart(EPInSig->getWStart());
  EPSignal->setWLen(EPInSig->getWLen());
  MPSignal->setWStart(MPInSig->getWStart());
  MPSignal->setWLen(MPInSig->getWLen());
  MPSignal->Normalize(Cfg.HRMPNormFactor, Cfg.HRMPNormType);
  MPSignal->WriteSignal(Cfg.HRMPOutFile,  Cfg.HRMPOutFileType);
  EPSignal->Normalize(Cfg.HREPNormFactor, Cfg.HREPNormType);
  EPSignal->WriteSignal(Cfg.HREPOutFile,  Cfg.HREPOutFileType);
}
