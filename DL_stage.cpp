#include "DL_stage.h"

DLstage::DLstage(DRCSignal *InputSig, DLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
}
void DLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  delete OutSig;
  OutSig = new DRCSignal(*InputSig);
}
void DLstage::NewInCfg(DLParmsType InCfg)
{
  Cfg = InCfg;
}

bool DLstage::process(void)
{
  unsigned int I;
  STLvectorReal BufSig;
  
  sputs("DRC: Dip Limiting stage (DL).");
  OutSig->clearData();
  for(I = 0; I < InSig->Data.size() ; I++)
      BufSig.push_back(InSig->Data[I]);

  /* Verifica se si deve effettuare il dip limiting */
  if (Cfg.DLMinGain > 0)
    {
      switch (Cfg.DLType[0])
	{
	  /* Fase lineare */
	case 'L':
	case 'P':
	  sputs("DL stage: MP signal linear phase dip limiting...");
	  if (STL_C1LPDipLimit(BufSig, Cfg.DLMinGain, Cfg.DLStart, InSig->getSampleRate(), Cfg.DLStartFreq, Cfg.DLEndFreq,
			       Cfg.DLType[0] == 'P', Cfg.DLMultExponent)== false)
	    {
	      sputs("DL stage: dip limiting failed.");
	      return false;
	    }
	  break;
	  
	  /* Fase minima */
	case 'M':
	  sputs("DL stage: MP signal minimum phase dip limiting...");
	  if (STL_C1HMPDipLimit(BufSig, Cfg.DLMinGain, Cfg.DLStart, InSig->getSampleRate(), Cfg.DLStartFreq, Cfg.DLEndFreq,
				Cfg.DLType[0] == 'W', Cfg.DLMultExponent) == false)
	    {
	      sputs("DL stage: dip limiting failed.");
	      return false;
	    }
	  break;
	}
      for(I = 0; I <  InSig->Data.size(); I++)
	OutSig->Data.push_back(BufSig[I]);
    } 
  sputs("DRC: Finished Dip Limiting stage (DL).");
  return true;
}

void DLstage::Normalize(void)
{
  if (Cfg.DLNormFactor > 0) {
    sputs("DL stage: output component normalization.");
    OutSig->Normalize(Cfg.DLNormFactor,Cfg.DLNormType);
  }
}

void DLstage::WriteOutput(void)
{
  if (Cfg.DLOutFile != NULL) {
    sputsp("DL stage: saving output component: ",Cfg.DLOutFile);
    if (OutSig->WriteSignal(Cfg.DLOutFile, Cfg.DLOutFileType) == false) {
      sputs("DL stage: output component save failed.");
    }
  }
}
