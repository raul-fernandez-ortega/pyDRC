#include "RMP_stage.h"

RMPstage::RMPstage(DRCSignal *InputSig, RMPParmsType InCfg)
{
  Cfg = InCfg;
  InSig = InputSig;
  MPSignal = new DRCSignal(InputSig);
  EPSignal = new DRCSignal(InputSig);
  Normalize(MPSignal, Cfg.MPPFNormFactor, Cfg.MPPFNormType);
  WriteSignal(MPSignal, Cfg.MPPFOutFile,  Cfg.MPPFOutFileType);
}
void RMPstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  MPSignal->setParams(InputSig);
  EPSignal->setParams(InputSig);
  Normalize(MPSignal, Cfg.MPPFNormFactor, Cfg.MPPFNormType);
  WriteSignal(MPSignal, Cfg.MPPFOutFile,  Cfg.MPPFOutFileType);
}

void RMPstage::NewInCfg(RMPParmsType InCfg)
{
  Cfg = InCfg;
  Normalize(MPSignal, Cfg.MPPFNormFactor, Cfg.MPPFNormType);
  WriteSignal(MPSignal, Cfg.MPPFOutFile,  Cfg.MPPFOutFileType);
}

void RMPstage::process(void)
{
  int I, J;
  DLReal *MPPFSig;
  DLReal *MPSig;
  DLReal *MPEPSig;

  MPSignal->Signal.clear();
  EPSignal->Signal.clear();
  MPPFSig = new DLReal[InSig->Signal->size()];
  for(I = 0; I < InSig->Signal->size(); I++)
    {
      MPPFSig[I] = InSig->Signal->at(I);
    }
  if (Cfg.MPHDRecover[0] == 'Y')
    {
      sputs("Allocating homomorphic deconvolution arrays.");
      MPSig = new DLReal[2 * InSig->WLen];
      if (MPSig == NULL)
	{
	  sputs("Memory allocation failed.");
	  return;
	}
      
      /* Azzera gli array */
      for (I = 0;I < 2 * InSig->WLen;I++)
	MPSig[I] = 0;
      
      /* Controlla se si deve preservare la componente EP della fase minima */
      if (Cfg.MPEPPreserve[0] == 'Y')
	{
	  MPEPSig = new DLReal[InSig->WLen];
	  if (MPEPSig == NULL)
	    {
	      sputs("Memory allocation failed.");
	      return;
	    }
	  
	  /* Azzera gli array */
	  for (I = 0;I < InSig->WLen;I++)
	    MPEPSig[I] = 0;
	}
      else
	MPEPSig = NULL;
      
      /* Effettua la deconvoluzione omomorfa*/
      sputs("MP Recover homomorphic deconvolution stage...");
      if (CepstrumHD(&MPPFSig[InSig->WStart],
		     &MPSig[InSig->WLen / 2 - (1 - (InSig->WLen % 2))],MPEPSig,
		     InSig->WLen,Cfg.MPHDMultExponent) == False)
	{
	  sputs("Homomorphic deconvolution failed.");
	  return;
	}
      
      /* Ricopia la componente MP nell'array originale */
      for (I = 0,J = InSig->WStart;I < InSig->WLen;I++,J++)
	MPPFSig[J] = MPSig[I];
      
      /* Dealloca l'array deconvoluzione */
      delete MPSig;
    }
  
  /* Verifica se si deve effettuare la finestratura finale */
  if (Cfg.MPPFFinalWindow > 0)
    {
      sputs("Minimum phase component final windowing.");
      BlackmanWindow(&MPPFSig[InSig->WStart],InSig->WLen);
      
      /* Controlla se si deve preservare la componente EP della fase minima */
      if (Cfg.MPHDRecover[0] == 'Y' && Cfg.MPEPPreserve[0] == 'Y')
	/* Effettua la finestratura della componente EP */
	BlackmanWindow(MPEPSig,InSig->WLen);
    }
  for(I=0; I < InSig->Signal.size(); I++)
    MPSignal->Signal->push_back(MPPFSig[I]);
  for(I=0; I < InSig->WLen; I++)
    EPSignal->Signal->push_back(MPEPSig[I]);
}
