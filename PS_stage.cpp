#include "PS_stage.h"

PSstage::PSstage(DRCSignal *InputSig, PSParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  PSOutF = new DRCSignal(*InputSig);
  Cfg = InCfg;
}

void PSstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  OutSig->setParams(InputSig);
}

void PSstage::NewInCfg(PSParmsType InCfg)
{
  Cfg = InCfg;
}

void PSstage::process(void)
{
  DLReal *PSFilterFreqs;
  DLReal *PSFilterM;
  DLReal *PSFilterP;
  DLReal *PSOutSig;
  DLReal *PSFilter;
  DLReal *MPSig;
  DLReal *InputS;

  int I, J, PSOutSigLen;
  unsigned int WLen3;
  InterpolationType FIType;

  //DRCSignal PSOutF;

  OutSig->Data->clear();
  PSOutF->Data->clear();
  /* Verifica se si devono contare i punti filtro */
  if (Cfg.PSNumPoints == 0) {
    sputsp("Counting target response/mic compensation definition file points: ",Cfg.PSPointsFile);
    Cfg.PSNumPoints = FLineCount(Cfg.PSPointsFile);
    printf("Target response definition/mic compensation file points: %d\n",Cfg.PSNumPoints);
    fflush(stdout);
  }
  
  /* Alloca gli array per la generazione della risposta target */
  sputs("Allocating target response arrays.");
  PSFilterFreqs = new DLReal[Cfg.PSNumPoints];
  if (PSFilterFreqs == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  PSFilterM = new DLReal[Cfg.PSNumPoints];
  if (PSFilterM == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  PSFilterP = new DLReal[Cfg.PSNumPoints];
  if (PSFilterP == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  //PSOutSigLen = Cfg.PSFilterLen + InSig->getData().size() - 1;
  PSOutSigLen = Cfg.PSFilterLen + InSig->getWLen() - 1;
  PSOutSig = new DLReal[PSOutSigLen];
  if (PSOutSig == NULL) {
    sputs("Memory allocation failed.");
    return;
  }
  
  /* Legge i punti del filtro */
  sputsp("Reading target response/mic compensation definition file: ",Cfg.PSPointsFile);
  if (ReadPoints(Cfg.PSPointsFile,(TFMagType) Cfg.PSMagType[0],PSFilterFreqs, PSFilterM,PSFilterP,Cfg.PSNumPoints,InSig->getSampleRate()) == false) {
    sputs("Target response point file input failed.");
    return;
  }
  
  /* Verifica il tipo di interpolazione */
  switch(Cfg.PSInterpolationType[0]) {
  case 'L':
    FIType = Linear;
    break;
  case 'G':
    FIType = Logarithmic;
    break;
  case 'R':
    FIType = SplineLinear;
    break;
  case 'S':
    FIType = SplineLogarithmic;
    break;
  case 'P':
    FIType = PCHIPLinear;
    break;
  case 'H':
    FIType = PCHIPLogarithmic;
    break;
  }
  
  /* Verifica il tipo di filtro da utilizzare */
  switch (Cfg.PSFilterType[0]) {
  case 'L':
    /* Alloca gli array per il filtro */
    sputs("Allocating target filter arrays.");
    PSFilter = new DLReal[Cfg.PSFilterLen];
    if (PSFilter == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    for (I = 0; I < Cfg.PSFilterLen; I++)
      PSFilter[I] = 0;
    
    /* Calcola la dimensione richiesta per il calcolo del filtro */
    if (Cfg.PSMultExponent >= 0) {
      /* Calcola la potenza di due superiore a Cfg.PSFilterLen */
      for(I = 1;I <= Cfg.PSFilterLen;I <<= 1);
      I *= 1 << Cfg.PSMultExponent;
    } else
      I = Cfg.PSFilterLen;
    
    /* Calcola il filtro */
    sputs("FIR Filter computation...");
    if (GenericFir(PSFilter,Cfg.PSFilterLen, PSFilterFreqs,PSFilterM,PSFilterP,Cfg.PSNumPoints,I,FIType) == false) {
      sputs("FIR Filter computation failed.");
      return;
    }
    
    /* Effettua la finestratura del filtro */
    BlackmanWindow(PSFilter,Cfg.PSFilterLen);
 
    break;
  case 'M':
  case 'T':
    /* Alloca gli array per il filtro */
    sputs("Allocating target filter arrays.");
    PSFilter = new DLReal[2 * Cfg.PSFilterLen];
    if (PSFilter == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    for (I = 0; I < 2 * Cfg.PSFilterLen; I++)
      PSFilter[I] = 0;
    
    /* Calcola la dimensione richiesta per il calcolo del filtro */
    if (Cfg.PSMultExponent >= 0) {
      /* Calcola la potenza di due superiore a Cfg.PSFilterLen */
      for(I = 1;I <= 2 * Cfg.PSFilterLen;I <<= 1);
      I *= 1 << Cfg.PSMultExponent;
    }
    else
      I = 2 * Cfg.PSFilterLen;
    
    /* Calcola il filtro */
    sputs("FIR Filter computation...");
    if (GenericFir(PSFilter,2 * Cfg.PSFilterLen, PSFilterFreqs,PSFilterM,PSFilterP,Cfg.PSNumPoints,I,FIType) == false) {
      sputs("FIR Filter computation failed.");
      return;
    }
    
    /* Alloca gli array per la deconvoluzione omomorfa */
    sputs("Allocating homomorphic deconvolution arrays.");
    MPSig = new DLReal[2 * Cfg.PSFilterLen];
    if (MPSig == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Azzera gli array */
    for (I = 0;I < 2 * Cfg.PSFilterLen;I++)
      MPSig[I] = 0;
    
    /* Effettua la deconvoluzione omomorfa*/
    sputs("MP target response/mic compensation extraction homomorphic deconvolution stage...");
    if (CepstrumHD(PSFilter,MPSig,NULL,2 * Cfg.PSFilterLen, Cfg.PSMultExponent) == false) {
      sputs("Homomorphic deconvolution failed.");
      return;
    }      
    
    /* Effettua la finestratura del filtro a fase minima */
    HalfBlackmanWindow(MPSig,Cfg.PSFilterLen,0,WRight);
    
    /* Copia il filtro a fase minima nell'array filtro */
    for (I = 0;I < Cfg.PSFilterLen;I++)
      PSFilter[I] = MPSig[I];
      
    /* Dealloca l'array deconvoluzione */
    //delete MPSig;
    break;
  }
  InputS = new DLReal[InSig->Data->size()];
  for (unsigned int K = 0; K < InSig->Data->size(); K++)
    InputS[K] = InSig->Data->at(K);
  /* Convoluzione filtro segnale */
  sputs("Target response/mic compensation FIR Filter convolution...");
  if (FftwConvolve(&InputS[InSig->getWStart()],InSig->getWLen(),PSFilter, Cfg.PSFilterLen,PSOutSig) == false) {
    perror("Convolution failed.");
    return;
  }
  
  /* Deallocazione array */
  delete PSFilter;
  
  /* Determina la dimensione della finestra di uscita */
  if (Cfg.PSOutWindow > 0) {
    /* Alloca l'array temporaneo per il filtro */
    PSFilter = new DLReal[Cfg.PSOutWindow];
    if (PSFilter == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Verifica il tipo di filtro */
    switch (Cfg.PSFilterType[0]) {
    case 'L':
      /* Determina la finestratura filtro */
      OutSig->setWStart((PSOutSigLen - Cfg.PSOutWindow) / 2);
      OutSig->setWLen(Cfg.PSOutWindow);
      WLen3 = PSOutSigLen;
      
      /* Salva il filtro per la convoluzione test */
      for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
	PSFilter[I] = PSOutSig[J];
      
      /* Effetua la finestratura filtro */
      sputs("Target response/mic compensation signal windowing.");
      BlackmanWindow(PSFilter,OutSig->getWLen());
      break;
      
    case 'M':
      /* Determina la finestratura filtro */
      OutSig->setWStart((InSig->getWLen() - Cfg.PSOutWindow) / 2);
      WLen3 = InSig->getWLen();
      OutSig->setWLen(Cfg.PSOutWindow);
      
      /* Salva il filtro per la convoluzione test */
      for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
	PSFilter[I] = PSOutSig[J];
      
      /* Effetua la finestratura filtro */
      sputs("Target response/mic compensation signal windowing.");
      BlackmanWindow(PSFilter,OutSig->getWLen());
      break;
    case 'T':
      /* Determina la finestratura filtro */
      OutSig->setWStart((InSig->getWLen() / 2) - Cfg.ISPELowerWindow);
      WLen3 = InSig->getWLen();
      OutSig->setWLen(Cfg.PSOutWindow);
      
      /* Salva il filtro per la convoluzione test */
      for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
	PSFilter[I] = PSOutSig[J];
      
      /* Effetua la finestratura filtro */
      sputs("Target response/mic compensation signal windowing.");
      HalfBlackmanWindow(PSFilter,OutSig->getWLen(),Cfg.ISPELowerWindow,WRight);
      break;
    }
  } else {
    /* Verifica il tipo di filtro */
    switch (Cfg.PSFilterType[0]) {
      
    case 'L':
      /* Determina la finestratura filtro */
      OutSig->setWStart(0);
      OutSig->setWLen(PSOutSigLen);
      WLen3 = PSOutSigLen;
      
    case 'M':
      /* Determina la finestratura filtro */
      OutSig->setWStart(0);
      WLen3 = InSig->getWLen();
      break;
      
    case 'T':
      /* Determina la finestratura filtro */
      OutSig->setWStart((OutSig->getWLen() / 2) - Cfg.ISPELowerWindow);
      WLen3 = InSig->getWLen();
      OutSig->setWLen(PSOutSigLen - OutSig->getWStart());
      break;
    }
    
    /* Alloca l'array temporaneo per il filtro */
    PSFilter = new DLReal[OutSig->getWLen()];
    if (PSFilter == NULL) {
      sputs("Memory allocation failed.");
      return;
    }
    
    /* Salva il filtro per la convoluzione test */
    for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
      PSFilter[I] = PSOutSig[J];
  }
  
  PSOutF->setSampleRate(OutSig->getSampleRate());
  PSOutF->setWStart(0);
  PSOutF->setWLen(OutSig->getWLen());
  for(I = 0; I < PSOutF->getWLen(); I++)
    PSOutF->getData()->push_back(PSFilter[I]);
  for(I = 0; I < PSOutSigLen; I++)
    OutSig->getData()->push_back(PSOutSig[I]);

  OutSig->setWLen(WLen3);
  OutSig->setWStart(0);  
  //Normalize();
  //WriteOutput();
}

void PSstage::Normalize(void)
{
  if (Cfg.PSNormFactor > 0) {
    sputs("PS stage impulse normalization.");
    PSOutF->Normalize(Cfg.PSNormFactor,Cfg.PSNormType);
  }
}

void PSstage::WriteOutput(void)
{
  if (Cfg.PSOutFile != NULL) {
    sputs("Saving PS stage impulse.");
    PSOutF->WriteSignal(Cfg.PSOutFile, Cfg.PSOutFileType);
  }
}
