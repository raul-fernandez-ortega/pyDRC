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

bool HRstage::process(void)
{
  int I, J;
  int MPEPSigLen, MPDataLen, EPDataLen;
  DLReal *MPPFSig;
  DLReal *MPSig;
  DLReal *MPEPSig;
  DLReal *EPSig;
  DLReal *EPPFSig;

  sputs("DRC: Homomorphic Recovery stage (HR).");
    
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
    sputs("HR stage: allocating homomorphic deconvolution arrays.");
    MPSig = new DLReal[2 * MPInSig->getWLen()];
    if (MPSig == NULL) {
      sputs("HR stage: memory allocation failed.");
      return false;
    }
      
    /* Azzera gli array */
    for (I = 0;I < 2 * MPInSig->getWLen();I++)
      MPSig[I] = 0;
    
    /* Controlla se si deve preservare la componente EP della fase minima */
    if (Cfg.HRMPEPPreserve[0] == 'Y') {
      MPEPSig = new DLReal[MPInSig->getWLen()];
      if (MPEPSig == NULL) {
	sputs("HR stage: memory allocation failed.");
	return false;
      }
	  
      /* Azzera gli array */
      for (I = 0;I < MPInSig->getWLen();I++)
	MPEPSig[I] = 0;
    } else
      MPEPSig = NULL;
    
    /* Effettua la deconvoluzione omomorfa*/
    sputs("HR stage: MP Recover homomorphic deconvolution stage...");
    if (CepstrumHD(&MPPFSig[MPInSig->getWStart()], &MPSig[MPInSig->getWLen() / 2 - (1 - (MPInSig->getWLen() % 2))],MPEPSig,
		   MPInSig->getWLen(),Cfg.HRMPHDMultExponent) == false) {
      sputs("HR stage: Homomorphic deconvolution failed.");
      return false;
    }
      
    /* Ricopia la componente MP nell'array originale */
    for (I = 0,J = MPInSig->getWStart();I < MPInSig->getWLen();I++,J++)
      MPPFSig[J] = MPSig[I];
    
    /* Dealloca l'array deconvoluzione */
    delete MPSig;
  }
  
  /* Verifica se si deve effettuare la finestratura finale */
  if (Cfg.HRMPFinalWindow > 0) {
    sputs("HR stage: minimum phase component final windowing.");
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
    sputs("HR stage: allocating minimum phase EP recovering arrays.");
    MPEPSigLen = MPInSig->getWLen() + EPInSig->getWLen() - 1;
    EPPFSig = new DLReal[MPEPSigLen];
    if (EPSig == NULL) {
      sputs("HR stage: memory allocation failed.");
      return false;
    }
    
    /* Effettua la convoluzione */
    sputs("HR stage: minimum phase EP recovering...");
    if (FftwConvolve(MPEPSig,MPInSig->getWLen(),EPSig,EPInSig->getWLen(),EPPFSig) == false) {
      sputs("HR stage: convolution failed.");
      return false;
    }
    /* Recupera la componente EP */
    for (I = 0, J= MPInSig->getWLen() / 2; I < MPInSig->getWLen();I++, J++) {
      EPSig[I] = EPPFSig[J];
    }
    /* Dealloca l'array temporaneo convoluzione */
    delete[] MPEPSig;
    delete[] EPSig;
  }
  /* Recupera la componente EP */
  
  for(I = MPInSig->getWLen() / 2; I < MPEPSigLen; I++)
    EPSignal->Data.push_back(EPPFSig[I]);
  EPSignal->setWStart(EPInSig->getWStart());
  EPSignal->setWLen(EPInSig->getWLen());
  MPSignal->setWStart(MPInSig->getWStart());
  MPSignal->setWLen(MPInSig->getWLen());

  sputs("DRC: Finished Homomorphic Recovery stage (HR).");
  return true;
}

void HRstage::Normalize(void)
{
  if (Cfg.HRMPNormFactor > 0) {
    sputs("HR stage: Minimum phase component normalization.");
    MPSignal->Normalize(Cfg.HRMPNormFactor,Cfg.HRMPNormType);
  }
  if (Cfg.HREPNormFactor > 0) {
    sputs("HR stage: Excess phase component normalization.");
    EPSignal->Normalize(Cfg.HREPNormFactor,Cfg.HREPNormType);
  }
}

void HRstage::WriteOutput(void)
{
  if (Cfg.HRMPOutFile != NULL) {
    sputsp("HR stage: Saving minimum phase component: ",Cfg.HRMPOutFile);
    if (MPSignal->WriteSignal(Cfg.HRMPOutFile, Cfg.HRMPOutFileType) == false) {
      sputs("HR stage: Minimum phase component save failed.");
    }
  }
  if (Cfg.HREPOutFile != NULL) {
    sputsp("HR stage: Saving excess phase component: ",Cfg.HREPOutFile);
    EPSignal->setWStart(0);
    if (EPSignal->WriteSignal(Cfg.HREPOutFile, Cfg.HREPOutFileType) == false) {
      sputs("HR stage: Excess phase component save failed.");
    }
  }
}
