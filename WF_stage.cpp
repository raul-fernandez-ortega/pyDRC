#include "WF_stage.h"

WFstage::WFstage(DRCSignal *InputSig, WFParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  Cfg = InCfg;
  //process();

}
void WFstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  //process();
}
void WFstage::NewInCfg(WFParmsType InCfg)
{
  Cfg = InCfg;
  //process();
}
bool WFstage::process(void)
{
  unsigned int WFSigLen;
  unsigned int InWinLen;
  
  unsigned int I;

  STLvectorReal BufSig;

  SLPPrefilteringType SLPType;
  BWPPrefilteringType BWPType;

  sputs("DRC: Window Filtering stage (WF).");
    
  OutSig->clearData();
  //InWinLen = InSig->getData().size();
  InWinLen = InSig->getWLen();


  sputs("WF stage: allocating filtering array.");
  WFSigLen = Cfg.WFLowerWindow + Cfg.WFFilterLen - 1;

  if (Cfg.WFRTFlag[0] == 'Y')
      OutSig->setWStart((InSig->getWLen() - Cfg.WFLowerWindow) / 2);
  else
      OutSig->setWStart((InWinLen - Cfg.WFLowerWindow) / 2);
  for(I = OutSig->getWStart(); I <  InSig->Data.size(); I++)
    BufSig.push_back(InSig->Data[I]);

  if (Cfg.WFfilterFctn[0] == 'P')
    {
      SLPType = SLPProportional;
      BWPType = BWPProportional;
    }
  else
    {
      SLPType = SLPBilinear;
      BWPType = BWPBilinear;
    }
  switch (Cfg.WFfilterType[0])
    {
    case 'B':
      sputs("WF stage: component band windowing.");
      
      /* Verifica che la finestratura sia corretta */
      if (((Cfg.WFPreWindowLen == 0) && (OutSig->getWStart() < OutSig->getPSStart())) 
	  || ((OutSig->getWStart() + Cfg.WFLowerWindow) > OutSig->getPSEnd()))
      	sputs("WF stage: !!Warning: input signal too short for correct signal filtering, spurios spikes may be generated.");

      OutSig->setData(STL_BWPreFilt(BufSig,Cfg.WFLowerWindow,Cfg.WFUpperWindow,
				    Cfg.WFFilterLen,Cfg.WFBandSplit,Cfg.WFWindowExponent,
				    InSig->getSampleRate(),Cfg.WFStartFreq,Cfg.WFEndFreq,
				    Cfg.WFWindowGap,WFull,BWPType),
		      0, Cfg.WFLowerWindow + Cfg.WFFilterLen - 1);
      break;
      
    case 'b':
      sputs("WF stage: component single side band windowing.");
      /* Verifica che la finestratura sia corretta */
      if ((OutSig->getWStart() + Cfg.WFLowerWindow) > OutSig->getPSEnd())
      	sputs("WF stage: !!Warning: input signal too short for correct signal filtering, spurios spikes may be generated.");
      
      OutSig->setData(STL_BWPreFilt(BufSig,Cfg.WFLowerWindow,Cfg.WFUpperWindow,
				    Cfg.WFFilterLen,Cfg.WFBandSplit,Cfg.WFWindowExponent,
				    InSig->getSampleRate(),Cfg.WFStartFreq,Cfg.WFEndFreq,
				    Cfg.WFWindowGap,WRight,BWPType),
		      0, Cfg.WFLowerWindow + Cfg.WFFilterLen - 1);
      break;
      
    case 'S':
      sputs("WF stage: component sliding lowpass filtering.");
      
      /* Verifica che la finestratura sia corretta */
      if (((OutSig->getPreWindowLen() == 0) && (OutSig->getWStart() < OutSig->getPSStart()))
	  || ((OutSig->getWStart() + Cfg.WFLowerWindow) > OutSig->getPSEnd()))
	sputs("WF stage: !!Warning: input signal too short for correct signal filtering, spurios spikes may be generated.");
      
      OutSig->setData(STL_SLPreFilt(BufSig,Cfg.WFLowerWindow,Cfg.WFUpperWindow,
				    Cfg.WFFilterLen,Cfg.WFBandSplit,Cfg.WFWindowExponent,
				    InSig->getSampleRate(),Cfg.WFStartFreq,Cfg.WFEndFreq,
				    Cfg.WFWindowGap,Cfg.WFFSharpness,WFull,SLPType),
		      0, Cfg.WFLowerWindow + Cfg.WFFilterLen - 1);
      break;
      
    case 's':
      sputs("WF stage: component single side sliding lowpass filtering.");
      
      /* Verifica che la finestratura sia corretta */
      if ((OutSig->getWStart() + Cfg.WFLowerWindow) > OutSig->getPSEnd())
      	sputs("WF stage: !!Warning: input signal too short for correct signal filtering, spurios spikes may be generated.");
      OutSig->setData(STL_SLPreFilt(BufSig,Cfg.WFLowerWindow,Cfg.WFUpperWindow,
				    Cfg.WFFilterLen,Cfg.WFBandSplit,Cfg.WFWindowExponent,
				    InSig->getSampleRate(),Cfg.WFStartFreq,Cfg.WFEndFreq,
				    Cfg.WFWindowGap,Cfg.WFFSharpness,WRight,SLPType),
		      0, Cfg.WFLowerWindow + Cfg.WFFilterLen - 1);
      break;
    }
  /* Calcola la dimensione per la finestratura finale */
  if (Cfg.WFFinalWindow > 0)
    {
      OutSig->setWStart((WFSigLen - Cfg.WFFinalWindow) / 2);
      OutSig->setWLen(Cfg.WFFinalWindow);
    }
  else
    {
      OutSig->setWStart(0);
      OutSig->setWLen(WFSigLen);
    }
  /* Verifica se si deve effettuare la finestratura finale */
  if (Cfg.WFRTFlag[0] == 'Y')
    {
      sputs("WF stage: ringing truncation final windowing.");
      STL_BlackmanWindow(OutSig->Data, OutSig->getWStart(),OutSig->getWLen());
    }
  sputs("DRC: Window Filtering stage (WF).");
  return true;
}

void WFstage::Normalize(void)
{
  if (Cfg.NormFactor > 0) {
    sputs("WF stage: output component normalization.");
    OutSig->Normalize(Cfg.NormFactor,Cfg.NormType);
  }
}

void WFstage::WriteOutput(void)
{
  if (Cfg.OutFile != NULL) {
    sputsp("WF stage: saving output component: ",Cfg.OutFile);
    if (OutSig->WriteSignal(Cfg.OutFile, Cfg.OutFileType) == false) {
      sputs("WF stage: output component save failed.");
    }
  }
}

