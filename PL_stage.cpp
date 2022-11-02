#include "PL_stage.h"

PLstage::PLstage(DRCSignal *InputSig, PLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
  //process();

}
void PLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  //process();
}
void PLstage::NewInCfg(PLParmsType InCfg)
{
  Cfg = InCfg;
  //process();
}

bool PLstage::process(void)
{
  int I, J;
  STLvectorReal BufSig;


  sputs("DRC: Peak Limiting stage (PL).");  
  OutSig->clearData();
  for(I = InSig->getWStart(); I <  InSig->getWLen(); I++)
      BufSig.push_back(InSig->Data[I]);

  if (Cfg.PLMaxGain > 0)
    {
      switch (Cfg.PLType[0])
	{
	  /* Fase lineare */
	case 'L':
	  sputs("PL stage: linear phase peak limiting...");
	  if (STL_C1LPPeakLimit(BufSig,Cfg.PLMaxGain,Cfg.PLStart, InSig->getSampleRate(),
				Cfg.PLStartFreq, Cfg.PLEndFreq, Cfg.PLType[0] == 'P', Cfg.PLMultExponent) == false) {
	    sputs("PL stage: peak limiting failed.");
	    return false;
	  }
	  break;
	  
	  /* Fase minima */
	case 'M':
	  sputs("PL stage: minimum phase peak limiting...");
	  if (STL_C1HMPPeakLimit(BufSig,Cfg.PLMaxGain,Cfg.PLStart,InSig->getSampleRate(),
				 Cfg.PLStartFreq,Cfg.PLEndFreq,Cfg.PLType[0] == 'W',Cfg.PLMultExponent) == false) {
	    sputs("PL stage: peak limiting failed.");
	    return false;
	  }
	  break;
	}
    }
  
  /* Effettua la finestratura finale */
  if (Cfg.PLOutWindow > 0)
    {
      OutSig->setWStart((InSig->getWLen() - Cfg.PLOutWindow) / 2);
      OutSig->setWLen(Cfg.PLOutWindow);
      sputs("PL stage: peak limited signal final windowing.");
      STL_BlackmanWindow(BufSig,OutSig->getWStart(),OutSig->getWLen());
    }
  for(J=0; J < OutSig->getWStart() + OutSig->getWLen(); J++)
    OutSig->Data.push_back(BufSig[J]);
  
  sputs("DRC: Finished Peak Limiting stage (PL).");
  return true;
  
}

void PLstage::Normalize(void)
{
  if (Cfg.PLNormFactor > 0) {
    sputs("PL stage: output component normalization.");
    OutSig->Normalize(Cfg.PLNormFactor,Cfg.PLNormType);
  }
}

void PLstage::WriteOutput(void)
{
  if (Cfg.PLOutFile != NULL) {
    sputsp("PL stage: saving output component: ",Cfg.PLOutFile);
    if (OutSig->WriteSignal(Cfg.PLOutFile, Cfg.PLOutFileType) == false) {
      sputs("PL stage: output component save failed.");
    }
  }
}
