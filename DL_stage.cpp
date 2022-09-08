#include "DL_stage.h"

DLstage::DLstage(DRCSignal *InputSig, DLParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(InputSig);
  Cfg = InCfg;
}
void DLstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  OutSig->setParams(InputSig);
}
void DLstage::NewInCfg(DLParmsType InCfg)
{
  Cfg = InCfg;
}

void DLstage::process(void)
{
  unsigned int i;
  STLvectorReal *BufSig = new STLvectorReal();

  OutSig->clearData();
  for(i = 0; i < InSig->Data->size() ; i++)
    BufSig->push_back(InSig->Data->at(i));

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
      OutSig->setData(BufSig, 0, BufSig->size());
    } 
  //Normalize();
  //WriteOutput();
}

void DLstage::Normalize(void)
{
  if (Cfg.DLNormFactor > 0) {
    sputs("DL stage impulse normalization.");
    OutSig->Normalize(Cfg.DLNormFactor,Cfg.DLNormType);
  }
}

void DLstage::WriteOutput(void)
{
  if (Cfg.DLOutFile != NULL) {
    sputs("Saving DL stage impulse.");
    OutSig->WriteSignal(Cfg.DLOutFile, Cfg.DLOutFileType);
  }
}
