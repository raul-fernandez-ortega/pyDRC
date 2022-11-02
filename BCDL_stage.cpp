#include "BCDL_stage.h"

BCDLstage::BCDLstage(DRCSignal *InputSig, BCDLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
}

void BCDLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
}

void BCDLstage::NewInCfg(BCDLParmsType InCfg)
{
  Cfg = InCfg;
}

bool BCDLstage::process(void)
{


  sputs("DRC: Base Configuration Dip Limiting Stage.");
  OutSig->Data.clear();
  OutSig->setParams(*InSig);
  OutSig->setData(InSig->getData(), 0, InSig->getData().size());
  
  
  /* Verifica se si deve effettuare il dip limiting */
  if (Cfg.BCDLMinGain > 0) {
    switch (Cfg.BCDLType[0]) {
      /* Fase lineare */
    case 'L':
    case 'P':
      sputs("BCDL stage: input signal linear phase dip limiting...");
      if (C1LPDipLimit(OutSig->getData().data(),OutSig->getWLen(),Cfg.BCDLMinGain,Cfg.BCDLStart,
		       OutSig->getSampleRate(),Cfg.BCDLStartFreq,Cfg.BCDLEndFreq,Cfg.BCDLType[0] == 'P',Cfg.BCDLMultExponent) == false) {
	sputs("BCDL stage: dip limiting failed.");
	return false;
      }
      break;
      
      /* Fase minima */
    case 'M':
    case 'W':
      sputs("DCDL stage: input signal minimum phase dip limiting...");
      if (C1HMPDipLimit(OutSig->getData().data(),OutSig->getWLen(),Cfg.BCDLMinGain,Cfg.BCDLStart,
			OutSig->getSampleRate(),Cfg.BCDLStartFreq,Cfg.BCDLEndFreq,Cfg.BCDLType[0] == 'W',Cfg.BCDLMultExponent) == false)
	{
	  sputs("BCDL stage: dip limiting failed.");
	  return false;
	}
      break;
    }
  }
  sputs("DRC: Finished Base Configuration Dip Limiting Stage.");
  
  return true;
}

void BCDLstage::Normalize(void)
{
  if (Cfg.NormFactor > 0) {
    sputs("BCDL stage: output component normalization.");
    OutSig->Normalize(Cfg.NormFactor,Cfg.NormType);
  }
}

void BCDLstage::WriteOutput(void)
{
  if (Cfg.OutFile != NULL) {
    sputsp("BCDL stage: saving output component: ",Cfg.OutFile);
    if (OutSig->WriteSignal(Cfg.OutFile, Cfg.OutFileType) == false) {
      sputs("BCDL stage: output component save failed.");
    }
  }
}
