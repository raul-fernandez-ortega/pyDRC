#include "IS_stage.h"

ISstage::ISstage(DRCSignal *MPInputSig, DRCSignal *EPInputSig, ISParmsType InCfg)
{
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  PCOutSig = new DRCSignal(*MPInputSig);
  ISOutSig = new DRCSignal(*MPInputSig);
  Cfg = InCfg;
  //process();

}

ISstage::~ISstage()
{
}

void ISstage::NewInputSignal(DRCSignal *MPInputSig, DRCSignal *EPInputSig)
{
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  delete PCOutSig;
  delete ISOutSig;
  PCOutSig = new DRCSignal(*MPInputSig);
  ISOutSig = new DRCSignal(*MPInputSig);
}

void ISstage::NewInCfg(ISParmsType InCfg)
{
  Cfg = InCfg;
}

bool ISstage::process(void)
{
  DLReal *MPEPSig;
  DLReal *MPPFSig;
  DLReal *EPPFSig;
  DLReal *ISRevSig;
  DLReal *ISRevOut;
  unsigned int MPEPSigLen, I, MPDataSize, EPDataSize;
  SLPPrefilteringType SLPType;

  sputs("DRC: Prefilter Completion stage (PC).");
    
  PCOutSig->clearData();
  ISOutSig->clearData();

  MPDataSize = MPInSig->getData().size();
  EPDataSize = EPInSig->getData().size();
  MPPFSig = new DLReal[MPDataSize];
  EPPFSig = new DLReal[EPDataSize];  

  for(I = 0; I < MPDataSize; I++)
    MPPFSig[I]=MPInSig->Data[I];
  for(I = 0; I < EPDataSize; I++)
    EPPFSig[I]=EPInSig->Data[I];

  /* Controlla se si deve attuare la fase PC */
  //  if (Cfg.ISType[0] == 'L' || Cfg.PCOutFile != NULL)
  //    {
  /* Alloca l'array per la convoluzione MP/EP */
  sputs("IS stage: allocating MP/EP convolution array.");
  MPEPSigLen = MPInSig->getWLen() + EPInSig->getWLen() - 1;
  MPEPSig = new DLReal[MPEPSigLen];
  if (MPEPSig == NULL) {
    sputs("PC stage: memory allocation failed.");
    return false;
  }
  /* Convoluzione MP/EP */
  sputs("PC stage: MP/EP Convolution...");
  if (FftwConvolve(&MPPFSig[MPInSig->getWStart()],MPInSig->getWLen(), &EPPFSig[EPInSig->getWStart()],EPInSig->getWLen(),MPEPSig) == false) {
    sputs("PC stage: convolution failed.");
    return false;
  }
  /* Dealloca gli array MP/EP finestrati */
  if (Cfg.ISType[0] == 'L') {
    delete MPPFSig;
    delete EPPFSig;
  }
  
  /* Finestratura segnale risultante */
  if (Cfg.PCOutWindow > 0) {
    sputs("PC stage: MP/EP signal windowing.");
    PCOutSig->setWStart((MPEPSigLen - Cfg.PCOutWindow) / 2);
    PCOutSig->setWLen(Cfg.PCOutWindow);
    BlackmanWindow(&MPEPSig[PCOutSig->getWStart()],PCOutSig->getWLen());
  } else {
    PCOutSig->setWStart(0);
    PCOutSig->setWLen(MPEPSigLen);
  }
  
  /* Normalizzazione segnale risultante */
  /*if (Cfg.PCNormFactor > 0) {
    sputs("IS stage: MP/EP normalization.");
    SigNormalize(&MPEPSig[PCOutSig->getWStart()],PCOutSig->getWLen(),Cfg.PCNormFactor, (NormType) Cfg.PCNormType[0]);
    }*/
  for(int K = 0; K < PCOutSig->getWStart() + PCOutSig->getWLen(); K++)
    PCOutSig->Data.push_back(MPEPSig[K]);

  //PCOutSig->WriteSignal(Cfg.PCOutFile, Cfg.PCOutFileType);
  sputs("DRC: Finished Prefilter Completion stage (PC).");
  
  sputs("DRC: Inversion stage (IS).");
  /* Verifica tipo inversione */
  ISOutSig->setWStart(PCOutSig->getWStart());
  ISOutSig->setWLen(PCOutSig->getWLen());
  
  /* Fase lineare con matrice Toeplitz */
  switch (Cfg.ISType[0]) {
  case 'L':
    /* Alloca l'array per l'inversione segnale */
    sputs("IS stage: allocating delay/reverse array.");
    ISRevSig = new DLReal[ISOutSig->getWLen()];
    if (ISRevSig == NULL) {
      sputs("IS stage: memory allocation failed.");
      return false;
    }
    
    /* Inversione e ritardo segnale */
    sputs("IS stage: signal delay/reverse.");
    for (int K = 0;K < ISOutSig->getWLen(); K++)
      ISRevSig[ISOutSig->getWLen() - (K + 1)] = MPEPSig[ISOutSig->getWStart() + K];
    
    /* Calcolo autocorrelazione e setup inversione */
    sputs("IS stage: autocorrelation computation and reverse setup...");
    if (AutoCorrelation(&MPEPSig[ISOutSig->getWStart()],ISOutSig->getWLen()) == false) {
      sputs("IS stage: autocorrelation computation failed.");
      return false;
    }
    for (int K = ISOutSig->getWLen() / 2; K < ISOutSig->getWLen(); K++)
      MPEPSig[ISOutSig->getWStart() + K] = 0;
    
    /* Alloca l'array per l'inversione segnale */
    sputs("IS stage: allocating inversion array.");
    ISRevOut = new DLReal[ISOutSig->getWLen()];
    if (ISRevOut == NULL) {
      sputs("IS stage: memory allocation failed.");
      return false;
    }
    
    /* Effettua l'inversione del segnale */
    sputs("IS stage: Toeplitz least square inversion...");
    if (ToeplitzSolve(&MPEPSig[ISOutSig->getWStart()],ISRevSig,ISRevOut, ISOutSig->getWLen()) != 0) {
      sputs("IS stage: inversion failed.");
      return false;
    }
    
    /* Dealloca gli array */
    delete ISRevSig;
    delete MPEPSig;
    break;
    
    /* A fase minima con pre-echo truncation */
  case 'T':
    /* Verifica la dimensione filtro richiesta */
    if (Cfg.ISOutWindow > 0)
      ISOutSig->setWLen(Cfg.ISOutWindow);
    else
      ISOutSig->setWLen(MPInSig->getWLen() + EPInSig->getWLen() - 1);
    
    /* Alloca l'array per l'inversione segnale */
    sputs("IS stage: allocating inversion array.");
    ISRevOut = new DLReal[ISOutSig->getWLen()];
    if (ISRevOut == NULL) {
      sputs("IS stage: memory allocation failed.");
      return false;
    }
    /* Verifica il tipo di funzione di prefiltratura */
    if (Cfg.ISPrefilterFctn[0] == 'P')
      /* Proporzionale */
      SLPType = SLPProportional;
    else
      /* Bilineare */
      SLPType = SLPBilinear;
    
    /* Inversione a fase minima selettiva */
    /* BlackmanWindow(EPRef,Cfg.BCInitWindow); */
    sputs("IS stage: pre-echo truncation fast deconvolution...");
    if (PETFDInvert(&MPPFSig[MPInSig->getWStart()], MPInSig->getWLen(), 
		    &EPPFSig[EPInSig->getWStart()], EPInSig->getWLen(), 
		    ISRevOut,ISOutSig->getWLen(), Cfg.ISPETType[0],
		    Cfg.ISPELowerWindow,Cfg.ISPEUpperWindow,Cfg.ISPEStartFreq,
		    Cfg.ISPEEndFreq,Cfg.ISPEFilterLen,Cfg.ISPEFSharpness,Cfg.ISPEBandSplit,
		    Cfg.ISPEWindowExponent,SLPType,Cfg.ISPEOGainFactor, ISOutSig->getSampleRate(),
		    Cfg.ISSMPMultExponent) == false) {
      sputs("IS stage: inversion failed.");
      return false;
    }
    
    /* Dealloca gli array MP/EP finestrati */
    delete MPPFSig;
    delete EPPFSig;
    break;
  }
  
  /* Finestratura segnale risultante */
  if (Cfg.ISOutWindow > 0) {
    sputs("IS stage: inverted signal windowing.");
    ISOutSig->setWStart((ISOutSig->getWLen() - Cfg.ISOutWindow) / 2);
    ISOutSig->setWLen(Cfg.ISOutWindow);
    BlackmanWindow(&ISRevOut[ISOutSig->getWStart()],ISOutSig->getWLen());
  } else {
    ISOutSig->setWStart(0);
  }
  for(int K = ISOutSig->getWStart(); K < ISOutSig->getWLen(); K++)
    ISOutSig->Data.push_back(ISRevOut[K]);
  
  sputs("DRC: Finished Inversion stage (IS).");
  return true;
}

void ISstage::Normalize(void)
{
  if (Cfg.ISNormFactor > 0) {
    sputs("PC stage: output component normalization.");
    PCOutSig->Normalize(Cfg.PCNormFactor,Cfg.PCNormType);
  }
  if (Cfg.ISNormFactor > 0) {
    sputs("IS stage: output component normalization.");
    ISOutSig->Normalize(Cfg.ISNormFactor,Cfg.ISNormType);
  }
}

void ISstage::WriteOutput(void)
{
  if (Cfg.PCOutFile != NULL) {
    sputsp("PC stage: saving output component: ",Cfg.ISOutFile);
    if (PCOutSig->WriteSignal(Cfg.PCOutFile, Cfg.PCOutFileType) == false) {
      sputs("PC stage: output component save failed.");
    }
  }
  if (Cfg.ISOutFile != NULL) {
    sputsp("IS stage: saving output component: ",Cfg.ISOutFile);
    if (ISOutSig->WriteSignal(Cfg.ISOutFile, Cfg.ISOutFileType) == false) {
      sputs("IS stage: output component save failed.");
    }
  }
}
