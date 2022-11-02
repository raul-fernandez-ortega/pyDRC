#include "MC_stage.h"

MCstage::MCstage(DRCSignal *InputSig, MCParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  MCOutF = new DRCSignal(*InputSig);
  Cfg = InCfg;
  //process();

}

void MCstage::NewInputSignal(DRCSignal *InputSig)
{
  InSig = InputSig;
  //process();
}

void MCstage::NewInCfg(MCParmsType InCfg)
{
  Cfg = InCfg;
  //process();
}

bool MCstage::process(void)
{
  DLReal *MCFilterFreqs;
  DLReal *MCFilterM;
  DLReal *MCFilterP;
  DLReal *MCOutSig;
  DLReal *MCFilter;
  DLReal *MCSig;

  int I, MCOutSigLen;
  unsigned int WLen3;
  InterpolationType FIType;
  int MCMPFLen;

  sputs("MC: Microphone Compensation stage (MC).");
  
  OutSig->Data.clear();
  MCOutF->Data.clear();

  /* Verifica se abilitata */
  if (Cfg.MCFilterType[0] != 'N') {
    /* Verifica se si devono contare i punti filtro */
    if (Cfg.MCNumPoints == 0) {
      sputsp("MC stage: counting mic compensation definition file points: ",Cfg.MCPointsFile);
      Cfg.MCNumPoints = FLineCount(Cfg.MCPointsFile);
      printf("MC stage: mic compensation definition file points: %d\n",Cfg.MCNumPoints);
      fflush(stdout);
    }
    /* Alloca gli array per la generazione del filtro compensazione */
    sputs("MC stage: allocating mic compensation filter arrays.");
    MCFilterFreqs = new DLReal[Cfg.MCNumPoints];
    if (MCFilterFreqs == NULL) {
      sputs("MC stage: memory allocation failed.");
      return false;
    }
    MCFilterM = new DLReal[Cfg.MCNumPoints];
    if (MCFilterM == NULL) {
      sputs("MC stage: memory allocation failed.");
      return false;
    }
    MCFilterP = new DLReal[Cfg.MCNumPoints];
    if (MCFilterP == NULL) {
      sputs("MC stage: memory allocation failed.");
      return false;
    }
    
    MCOutSigLen = Cfg.MCFilterLen + InSig->getWLen() - 1;
    //MCOutSigLen = Cfg.MCFilterLen + Cfg.BCInitWindow - 1;
    MCOutSig = new DLReal[MCOutSigLen];
    if (MCOutSig == NULL) {
      sputs("MC stage: memory allocation failed.");
      return false;
    }
#ifdef DEBUG 
    printf("MC stage DEBUG: MCOutSigLen:%d\n",MCOutSigLen);
#endif
    /* Legge i punti del filtro */
    sputsp("MC stage: reading mic compensation definition file: ",Cfg.MCPointsFile);
    if (ReadPoints(Cfg.MCPointsFile,(TFMagType) Cfg.MCMagType[0],MCFilterFreqs,MCFilterM,MCFilterP,Cfg.MCNumPoints,InSig->getSampleRate()) == false) {
      sputs("MC stage: mic compensation file input failed.");
      return false;
    }
#ifdef DEBUG
    sputs("MC stage DEBUG:Values readed from file");
    for(I = 0;I < Cfg.MCNumPoints; I++) 
      printf("MC stage DEBUG: Frequency:%10.4f Hz. Magnitude:%10.4e Phase:%10.3f\n",InSig->getSampleRate()*MCFilterFreqs[I]/2,MCFilterM[I],MCFilterP[I]);
#endif
    /* Effettua l'inversione diretta */
    sputs("MC stage: mic compensation direct inversion.");
    for (I = 0;I < Cfg.MCNumPoints;I++) {
      MCFilterM[I] = ((DRCFloat) 1.0) / MCFilterM[I];
      MCFilterP[I] = -MCFilterP[I];
#ifdef DEBUG 
      printf("MC stage DEBUG: Frequency:%10.4f Hz. Magnitude:%10.4e Phase:%10.3f\n",InSig->getSampleRate()*MCFilterFreqs[I]/2,MCFilterM[I],MCFilterP[I]);
#endif
    }
    
    /* Verifica il tipo di interpolazione */
    switch(Cfg.MCInterpolationType[0]) {
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
    switch (Cfg.MCFilterType[0]) {
    case 'L':
      /* Alloca gli array per il filtro */
      sputs("MC stage: allocating mic compensation filter arrays.");
#ifdef DEBUG 
      printf("MC stage DEBUG: MCFilter length:%d\n",Cfg.MCFilterLen);
#endif
      MCFilter = new DLReal[Cfg.MCFilterLen];
      if (MCFilter == NULL) {
	sputs("MC stage: memory allocation failed.");
	return false;
      }
      for (I = 0; I < Cfg.MCFilterLen; I++)
	MCFilter[I] = 0;
      
      /* Calcola la dimensione richiesta per il calcolo del filtro */
      if (Cfg.MCMultExponent >= 0) {
	/* Calcola la potenza di due superiore a Cfg.MCFilterLen */
	for(I = 1;I <= Cfg.MCFilterLen;I <<= 1);
	I *= 1 << Cfg.MCMultExponent;
      }
      else
	I = Cfg.MCFilterLen;
      
#ifdef DEBUG 
      printf("MC stage DEBUG: I=%d.\n",I);
#endif
      /* Calcola il filtro */
      sputs("MC stage: mic compensation FIR Filter computation...");
      if (GenericFir(MCFilter,Cfg.MCFilterLen,MCFilterFreqs,MCFilterM,MCFilterP,Cfg.MCNumPoints,I,FIType) == false) {
	sputs("MC stage: FIR Filter computation failed.");
	return false;
      }
      
      /* Effettua la finestratura del filtro */
      BlackmanWindow(MCFilter,Cfg.MCFilterLen);
      break;
      
    case 'M':
      sputs("MC stage: minimun phase microphone compensation filter.");
      /* Alloca gli array per il filtro */
      sputs("MC stage: allocating mic compensation filter arrays.");
      MCMPFLen = 1 + 2 * Cfg.MCFilterLen;
      MCFilter = new DLReal[MCMPFLen];
      if (MCFilter == NULL) {
	sputs("MC stage: memory allocation failed.");
	return false;
      }
      for (I = 0; I < MCMPFLen; I++)
	MCFilter[I] = 0;
      
      /* Calcola la dimensione richiesta per il calcolo del filtro */
      if (Cfg.MCMultExponent >= 0) {
	/* Calcola la potenza di due superiore a Cfg.MCFilterLen */
	for(I = 1;I <= MCMPFLen;I <<= 1);
	I *= 1 << Cfg.MCMultExponent;
      }
      else
	I = MCMPFLen;
#ifdef DEBUG 
      printf("MC stage DEBUG: I=%d.\n",I);
#endif
      /* Calcola il filtro */
      sputs("MC stage: mic compensation FIR Filter computation...");
      if (GenericFir(MCFilter,MCMPFLen,MCFilterFreqs,MCFilterM,MCFilterP,Cfg.MCNumPoints,I,FIType) == false) {
	sputs("MC stage: FIR Filter computation failed.");
	return false;
      }
      
      /* Alloca gli array per la deconvoluzione omomorfa */
      sputs("MC stage: allocating homomorphic deconvolution arrays.");
      MCSig = new DLReal[MCMPFLen];
      if (MCSig == NULL) {
	sputs("MC stage: memory allocation failed.");
	return false;
      }
      
      /* Azzera gli array */
      for (I = 0;I < MCMPFLen;I++)
	MCSig[I] = 0;
      
      /* Effettua la deconvoluzione omomorfa*/
      sputs("MC stage: MP mic compensation filter extraction homomorphic deconvolution stage...");
      if (CepstrumHD(MCFilter,MCSig,NULL,MCMPFLen,Cfg.MCMultExponent) == false) {
	sputs("MC stage: Homomorphic deconvolution failed.");
	return false;
      }
      
      /* Effettua la finestratura del filtro a fase minima */
      HalfBlackmanWindow(MCSig,Cfg.MCFilterLen,0,WRight);
      
      /* Copia il filtro a fase minima nell'array filtro */
      for (I = 0;I < Cfg.MCFilterLen;I++)
	MCFilter[I] = MCSig[I];
      
      /* Dealloca l'array deconvoluzione */
      delete[] MCSig;
      break;
    }
    
    /* Verifica se si deve salvare il filtro psicoacustico */
    if (Cfg.MCFilterFile != NULL) {
      /* Salva la componente MP */
      sputsp("MC stage: saving mic compensation filter: ",Cfg.MCFilterFile);
      //if (WriteSignal(Cfg.MCFilterFile,MCFilter,Cfg.MCFilterLen,(IFileType) Cfg.MCFilterFileType[0]) == false) {
      if (SND_WriteSignal(Cfg.MCFilterFile,MCFilter,0,Cfg.MCFilterLen,InSig->getSampleRate(),(IFileType) Cfg.MCFilterFileType[0]) == false) {
	sputs("MC stage: mic compensation filter save failed.");
	return false;
      }
    }
    /* Convoluzione filtro segnale */
    sputs("MC stage: Mic compensation FIR Filter convolution...");
    //if (DFftConvolve(InSig,Cfg.BCInitWindow,MCFilter,Cfg.MCFilterLen,MCOutSig) == false) {
    //if (FftwConvolve(&InputS[InSig->getWStart()],InSig->getWLen(),MCFilter, Cfg.MCFilterLen,MCOutSig) == false) {
    if (FftwConvolve(&InSig->Data[InSig->getWStart()],InSig->getWLen(), MCFilter, Cfg.MCFilterLen, MCOutSig) == false) {
      perror("MC stage: Convolution failed.");
      return false;
    }
    
    /* Deallocazione array */
    //delete[] MCFilter;
    
    /* Determina la dimensione della finestra di uscita */
    if (Cfg.MCOutWindow > 0) {
      /* Verifica il tipo di filtro */
      switch (Cfg.MCFilterType[0]) {
      case 'L':
	/* Determina la finestratura filtro */ 
	OutSig->setWStart((MCOutSigLen - Cfg.MCOutWindow) / 2);
	OutSig->setWLen(Cfg.MCOutWindow);
	WLen3 = MCOutSigLen;
	
	/* Salva il filtro per la convoluzione test */
	// for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
	//	MCFilter[I] = MCOutSig[J];
	
	/* Effetua la finestratura filtro */
	sputs("Mic compensation signal windowing.");
	BlackmanWindow(&MCOutSig[OutSig->getWStart()],OutSig->getWLen());
	//BlackmanWindow(MCFilter,OutSig->getWLen());
	break;
	
      case 'M':
	/* Determina la finestratura filtro */
	OutSig->setWStart((InSig->getWLen() - Cfg.MCOutWindow) / 2);
	WLen3 = InSig->getWLen();
	OutSig->setWLen(Cfg.MCOutWindow);
	
	/* Salva il filtro per la convoluzione test */
	// for (I = 0,J = OutSig->getWStart();I < OutSig->getWLen();I++,J++)
	//	MCFilter[I] = MCOutSig[J];
	
	/* Effetua la finestratura filtro */
	sputs("Mic compensation signal windowing.");
	BlackmanWindow(&MCOutSig[OutSig->getWStart()],OutSig->getWLen());		
	break;
      }
    } else {
      /* Verifica il tipo di filtro */
      switch (Cfg.MCFilterType[0]) { 
      case 'L':
	/* Determina la finestratura filtro */
	OutSig->setWStart(0);
	OutSig->setWLen(MCOutSigLen);
	WLen3 = MCOutSigLen;
	
      case 'M':
	/* Determina la finestratura filtro */
	OutSig->setWStart(0);
	WLen3 = InSig->getWLen();
	break;
      }
    }
    
    for(I = 0; I < MCOutSigLen; I++)
      OutSig->Data.push_back(MCOutSig[I]);
    
    OutSig->Normalize(Cfg.MCNormFactor, Cfg.MCNormType);
    OutSig->WriteSignal(Cfg.MCOutFile, Cfg.MCOutFileType);
    
    OutSig->setWLen(WLen3);
    OutSig->setWStart(0);
  } else {
    OutSig->setData(InSig->getData(),0, InSig->getData().size());
    OutSig->setParams(*InSig);
  }
  sputs("DRC: Finished Microphone compensation stage (MC).");
  return true;
}
