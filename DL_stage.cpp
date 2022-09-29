#include "DL_stage.h"

DLstage::DLstage(DRCSignal *InputSig, DLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
  //process();

}
void DLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  delete OutSig;
  OutSig = new DRCSignal(*InputSig);
  //process();
}
void DLstage::NewInCfg(DLParmsType InCfg)
{
  Cfg = InCfg;
  //process();
}

void DLstage::process(void)
{
  unsigned int I;
  STLvectorReal BufSig;

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
	  sputs("MP signal linear phase dip limiting...");
	  if (STL_C1LPDipLimit(BufSig, Cfg.DLMinGain, Cfg.DLStart,
			       InSig->getSampleRate(), Cfg.DLStartFreq, Cfg.DLEndFreq,
			       Cfg.DLMultExponent)== false)
	    {
	      sputs("Dip limiting failed.");
	      return;
	    }
	  break;
	  
	  /* Fase minima */
	case 'M':
	  sputs("MP signal minimum phase dip limiting...");
	  if (STL_C1HMPDipLimit(BufSig, Cfg.DLMinGain, Cfg.DLStart,
			    InSig->getSampleRate(), Cfg.DLStartFreq, Cfg.DLEndFreq,
			    Cfg.DLMultExponent) == false)
	    {
	      sputs("Dip limiting failed.");
	      return;
	    }
	  break;
	}
      for(I = 0; I <  InSig->Data.size(); I++)
	OutSig->Data.push_back(BufSig[I]);
    } 
  OutSig->Normalize(Cfg.DLNormFactor, Cfg.DLNormType);
  OutSig->WriteSignal(Cfg.DLOutFile, Cfg.DLOutFileType);
}
