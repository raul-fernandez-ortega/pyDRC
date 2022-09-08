#include "PL_stage.h"

PLstage::PLstage(DRCSignal *InputSig, PLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(InputSig);
  Cfg = InCfg;
}

void PLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  OutSig->setParams(InputSig);
}

void PLstage::NewInCfg(PLParmsType InCfg)
{
  Cfg = InCfg;
}

void PLstage::process(void)
{
  int I, J;
  STLvectorReal *BufSig = new STLvectorReal();

  OutSig->clearData();
  for(I = InSig->getWStart(); I <  InSig->getWLen(); I++)
    BufSig->push_back(InSig->getData()->at(I));

  if (Cfg.PLMaxGain > 0)
    {
      switch (Cfg.PLType[0])
	{
	  /* Fase lineare */
	case 'L':
	  sputs("Linear phase peak limiting...");
	  if (STL_C1LPPeakLimit(BufSig,Cfg.PLMaxGain,Cfg.PLStart,
				InSig->getSampleRate(),Cfg.PLStartFreq,Cfg.PLEndFreq,
				Cfg.PLMultExponent) == false)
	    {
	      sputs("Peak limiting failed.");
	      return;
	    }
	  break;
	  
	  /* Fase minima */
	case 'M':
	  sputs("Minimum phase peak limiting...");
	  if (STL_C1HMPPeakLimit(BufSig,Cfg.PLMaxGain,Cfg.PLStart,
			     InSig->getSampleRate(),Cfg.PLStartFreq,Cfg.PLEndFreq,
				 Cfg.PLMultExponent) == false)
	    {
	      sputs("Peak limiting failed.");
	      return;
	    }
	  break;
	}
    }
  
  /* Effettua la finestratura finale */
  if (Cfg.PLOutWindow > 0)
    {
      OutSig->setWStart((InSig->getWLen() - Cfg.PLOutWindow) / 2);
      OutSig->setWLen(Cfg.PLOutWindow);
      sputs("Peak limited signal final windowing.");
      STL_BlackmanWindow(BufSig,OutSig->getWStart(),OutSig->getWLen());
    }
  for(J=0; J < OutSig->getWStart() + OutSig->getWLen(); J++)
    OutSig->Data->push_back(BufSig->at(J));

  //Normalize();
  //WriteOutput();
}

void PLstage::Normalize(void)
{
  if (Cfg.PLNormFactor > 0) {
    sputs("PL stage impulse normalization.");
    OutSig->Normalize(Cfg.PLNormFactor,Cfg.PLNormType);
  }
}

void PLstage::WriteOutput(void)
{
  if (Cfg.PLOutFile != NULL) {
    sputs("Saving PL stage impulse.");
    OutSig->WriteSignal(Cfg.PLOutFile, Cfg.PLOutFileType);
  }
}
