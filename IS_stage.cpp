#include "IS_stage.h"

ISstage::ISstage(DRCSignal *MPInputSig, DRCSignal *EPInputSig, ISParmsType InCfg)
{
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  PCOutSig = new DRCSignal(MPInputSig);
  ISOutSig = new DRCSignal(MPInputSig);
  Cfg = InCfg;
}

ISstage::~ISstage()
{
}

void ISstage::NewInputSignal(DRCSignal *MPInputSig, DRCSignal *EPInputSig)
{
  MPInSig = MPInputSig;
  EPInSig = EPInputSig;
  PCOutSig->setParams(MPInputSig);
  ISOutSig->setParams(MPInputSig);
}

void ISstage::NewInCfg(ISParmsType InCfg)
{
  Cfg = InCfg;
}

void ISstage::process(void)
{
  DLReal *MPEPSig;
  DLReal *MPPFSig;
  DLReal *EPPFSig;
  DLReal *ISRevSig;
  DLReal *ISRevOut;
  unsigned int MPEPSigLen, I, MPDataSize, EPDataSize;
  SLPPrefilteringType SLPType;

  PCOutSig->clearData();
  ISOutSig->clearData();

  MPDataSize = MPInSig->getData()->size();
  EPDataSize = EPInSig->getData()->size();
  MPPFSig = new DLReal[MPDataSize];
  EPPFSig = new DLReal[EPDataSize];  

  for(I = 0; I < MPDataSize; I++)
    MPPFSig[I]=MPInSig->getData()->at(I);
  for(I = 0; I < EPDataSize; I++)
    EPPFSig[I]=EPInSig->getData()->at(I);

  /* Controlla se si deve attuare la fase PC */
  //  if (Cfg.ISType[0] == 'L' || Cfg.PCOutFile != NULL)
  //    {
  /* Alloca l'array per la convoluzione MP/EP */
  sputs("Allocating MP/EP convolution array.");
  MPEPSigLen = MPInSig->getWLen() + EPInSig->getWLen() - 1;
  MPEPSig = new DLReal[MPEPSigLen];
  if (MPEPSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  /* Convoluzione MP/EP */
  sputs("MP/EP Convolution...");
  if (FftwConvolve(&MPPFSig[MPInSig->getWStart()],MPInSig->getWLen(), &EPPFSig[EPInSig->getWStart()],EPInSig->getWLen(),MPEPSig) == false) {
    sputs("Convolution failed.");
    return ;
  }
  /* Dealloca gli array MP/EP finestrati */
  if (Cfg.ISType[0] == 'L') {
    delete MPPFSig;
    delete EPPFSig;
  }
  
  /* Finestratura segnale risultante */
  if (Cfg.PCOutWindow > 0) {
    sputs("MP/EP signal windowing.");
    PCOutSig->setWStart((MPEPSigLen - Cfg.PCOutWindow) / 2);
    PCOutSig->setWLen(Cfg.PCOutWindow);
    BlackmanWindow(&MPEPSig[PCOutSig->getWStart()],PCOutSig->getWLen());
  } else {
    PCOutSig->setWStart(0);
    PCOutSig->setWLen(MPEPSigLen);
  }
  
  /* Normalizzazione segnale risultante */
  if (Cfg.PCNormFactor > 0) {
    sputs("MP/EP normalization.");
    SigNormalize(&MPEPSig[PCOutSig->getWStart()],PCOutSig->getWLen(),Cfg.PCNormFactor, (NormType) Cfg.PCNormType[0]);
  }
  for(int K = 0; K < PCOutSig->getWStart() + PCOutSig->getWLen(); K++)
    PCOutSig->Data->push_back(MPEPSig[K]);
  
  PCOutSig->WriteSignal(Cfg.PCOutFile, Cfg.PCOutFileType);

  /* Verifica tipo inversione */
  ISOutSig->setWStart(PCOutSig->getWStart());
  ISOutSig->setWLen(PCOutSig->getWLen());
  
  /* Fase lineare con matrice Toeplitz */
  switch (Cfg.ISType[0]) {
  case 'L':
    /* Alloca l'array per l'inversione segnale */
    sputs("Allocating delay/reverse array.");
    ISRevSig = new DLReal[ISOutSig->getWLen()];
    if (ISRevSig == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Inversione e ritardo segnale */
    sputs("Signal delay/reverse.");
    for (int K = 0;K < ISOutSig->getWLen(); K++)
      ISRevSig[ISOutSig->getWLen() - (K + 1)] = MPEPSig[ISOutSig->getWStart() + K];
    
    /* Calcolo autocorrelazione e setup inversione */
    sputs("Autocorrelation computation and reverse setup...");
    if (AutoCorrelation(&MPEPSig[ISOutSig->getWStart()],ISOutSig->getWLen()) == false) {
      sputs("Autocorrelation computation failed.");
      return;
    }
    for (int K = ISOutSig->getWLen() / 2; K < ISOutSig->getWLen(); K++)
      MPEPSig[ISOutSig->getWStart() + K] = 0;
    
    /* Alloca l'array per l'inversione segnale */
    sputs("Allocating inversion array.");
    ISRevOut = new DLReal[ISOutSig->getWLen()];
    if (ISRevOut == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Effettua l'inversione del segnale */
    sputs("Toeplitz least square inversion...");
    if (ToeplitzSolve(&MPEPSig[ISOutSig->getWStart()],ISRevSig,ISRevOut, ISOutSig->getWLen()) != 0) {
      sputs("Inversion failed.");
      return;
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
    sputs("Allocating inversion array.");
    ISRevOut = new DLReal[ISOutSig->getWLen()];
    if (ISRevOut == NULL) {
      sputs("Memory allocation failed.");
      return;
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
    sputs("Pre-echo truncation fast deconvolution...");
    if (PETFDInvert(&MPPFSig[MPInSig->getWStart()], MPInSig->getWLen(), 
		    &EPPFSig[EPInSig->getWStart()], EPInSig->getWLen(), 
		    ISRevOut,ISOutSig->getWLen(), Cfg.ISPETType[0],
		    Cfg.ISPELowerWindow,Cfg.ISPEUpperWindow,Cfg.ISPEStartFreq,
		    Cfg.ISPEEndFreq,Cfg.ISPEFilterLen,Cfg.ISPEFSharpness,Cfg.ISPEBandSplit,
		    Cfg.ISPEWindowExponent,SLPType,Cfg.ISPEOGainFactor, ISOutSig->getSampleRate(),
		    Cfg.ISSMPMultExponent) == false) {
      sputs("Inversion failed.");
      return ;
    }
    
    /* Dealloca gli array MP/EP finestrati */
    delete MPPFSig;
    delete EPPFSig;
    break;
  }
  
  /* Finestratura segnale risultante */
  if (Cfg.ISOutWindow > 0) {
    sputs("Inverted signal windowing.");
    ISOutSig->setWStart((ISOutSig->getWLen() - Cfg.ISOutWindow) / 2);
    ISOutSig->setWLen(Cfg.ISOutWindow);
    BlackmanWindow(&ISRevOut[ISOutSig->getWStart()],ISOutSig->getWLen());
  } else {
    ISOutSig->setWStart(0);
  }
  for(int K = ISOutSig->getWStart(); K < ISOutSig->getWLen(); K++)
    ISOutSig->Data->push_back(ISRevOut[K]);
  
  //Normalize();
  //WriteOutput();
}

void ISstage::Normalize(void)
{
  if (Cfg.ISNormFactor > 0) {
    sputs("IS stage impulse normalization.");
    ISOutSig->Normalize(Cfg.ISNormFactor,Cfg.ISNormType);
  }
}

void ISstage::WriteOutput(void)
{
  if (Cfg.ISOutFile != NULL) {
    sputs("Saving IS stage impulse.");
    ISOutSig->WriteSignal(Cfg.ISOutFile, Cfg.ISOutFileType);
  }
}
