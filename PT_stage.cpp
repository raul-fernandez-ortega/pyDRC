#include "PT_stage.h"

PTstage::PTstage(DRCSignal *InputSig, DRCSignal *RefSignal, PTParmsType InCfg)
{
  InSig = InputSig;
  OutSig = new DRCSignal(*InputSig);
  RefSig = RefSignal;
  Cfg = InCfg;
  //process();
}

void PTstage::NewInputSignal(DRCSignal *InputSig,  DRCSignal *RefSignal)
{
  InSig = InputSig;
  RefSig = RefSignal;
  //process();
}

void PTstage::NewInCfg(PTParmsType InCfg)
{
  Cfg = InCfg;
  //process();
}

bool PTstage::process(void)
{

  unsigned int I, K;
  int WLen, WStart, WLen2, WStart2, L;
  unsigned int PTTConvLen, PTTRefLen;
  MKSETFType TFType;
  DLReal * OInSig;
  DLReal * PTTConv;
  DLReal * PTFilter;
  DLReal * ISRevOut;
  STLvectorReal buf;
  STLvectorReal buf2;

  sputs("DRC: Psychoacoustic Target stage (PT).");
  OutSig->clearData();
  
  if (Cfg.PTType[0] != 'N') {
    WLen = InSig->getWLen();
    WStart = InSig->getWStart();
    
    /* Alloca l'array per la convoluzione filtro e risposta */
    sputs("PT stage: allocating psychoacoustic target reference convolution array.");
    PTTConvLen = WLen + Cfg.BCInitWindow - 1;
    PTTConv = new DLReal[PTTConvLen];
    if (PTTConv == NULL) {
      sputs("PT stage: memory allocation failed.");
      return false;
    }
    OInSig = new DLReal[RefSig->getData().size()];
    if (OInSig == NULL) {
      sputs("PT stage: memory allocation failed.");
      return false;
    }
    buf = RefSig->getData();
    for(I=0; I < buf.size(); I++)
      OInSig[I] = buf[I];
    
    ISRevOut = new DLReal[InSig->getData().size()];
    if (ISRevOut == NULL) {
      sputs("PT stage: memory allocation failed.");
      return false;
    }	
    buf = InSig->getData();
    for(I = 0; I < buf.size(); I++)
      ISRevOut[I] = buf[I];
    
    /* Effettua la convoluzione tra filtro e risposta */
    sputs("PT stage: psychoacoustic target reference convolution...");
    if (FftwConvolve(OInSig,Cfg.BCInitWindow,&ISRevOut[WStart],WLen,PTTConv) == false) {
      sputs("PT stage: convolution failed.");
      return false;
    } 
    /* Effettua la finestratura della convoluzione di riferimento */
    PTTRefLen = (PTTConvLen - Cfg.PTReferenceWindow) / 2;
    for (I = 0;I < PTTRefLen;I++)
      PTTConv[I] = (DLReal) 0.0;
    for (I = PTTRefLen; I < InSig->getData().size();I++)
      PTTConv[I] = InSig->getData()[I];
    BlackmanWindow(&PTTConv[PTTRefLen],Cfg.PTReferenceWindow);
    for (I = (PTTRefLen + Cfg.PTReferenceWindow);I < PTTConvLen;I++)
      PTTConv[I] = (DLReal) 0.0;
    
    /* Verifica se si deve effettuare il dip limiting sulla risposta target */
    if (Cfg.PTDLMinGain > 0) {
      switch (Cfg.PTDLType[0]) {
	/* Fase lineare */
      case 'L':
	sputs("PT stage: target reference signal linear phase dip limiting...");
	if (C1LPDipLimit(PTTConv,PTTConvLen,Cfg.PTDLMinGain,Cfg.PTDLStart,
			 InSig->getSampleRate(),Cfg.PTDLStartFreq,Cfg.PTDLEndFreq, Cfg.PTDLType[0] == 'P', Cfg.PTDLMultExponent) == false) {
	  sputs("PT stage: dip limiting failed.");
	  return false;
	}
	break;
	
	/* Fase minima */
      case 'M':
	sputs("PT stage: target reference minimum phase dip limiting...");
	if (C1HMPDipLimit(PTTConv,PTTConvLen,Cfg.PTDLMinGain,Cfg.PTDLStart,
			  InSig->getSampleRate(),Cfg.PTDLStartFreq,Cfg.PTDLEndFreq, Cfg.PTDLType[0] == 'W', Cfg.PTDLMultExponent) == false) {
	  sputs("PT stage: dip limiting failed.");
	  return false;
	}
	break;
      }
    }
    
    /* Alloca l'array per il calcolo del filtro target */
    sputs("PT stage: allocating psychoacoustic target filter array.");
    PTFilter = new DLReal[Cfg.PTFilterLen];
    if (PTFilter == NULL) {
      sputs("PT stage: memory allocation failed.");
      return false;
    }
    
    /* Imposta il tipo filtro target */
    switch (Cfg.PTType[0]) {
    case 'L':
      TFType = MKSETFLinearPhase;
      break;
      
    case 'M':
      TFType = MKSETFMinimumPhase;
      break;
    }
    
    /* Calcola il filtro target psicoacustico */
    sputs("PT stage: computing psychoacoustic target filter...");
    /*if (P2MKSETargetFilter(PTTConv,PTTConvLen,InSig->getSampleRate(),
			   Cfg.PTBandWidth,Cfg.PTPeakDetectionStrength,PTFilter,TFType,
			   Cfg.PTMultExponent,Cfg.PTFilterLen) == false) {
      sputs("Psychoacoustic target filter computation failed.");
      return 1;
      }*/
    if (P2MKSETargetFilter(&PTTConv[PTTRefLen], Cfg.PTReferenceWindow, InSig->getSampleRate(), Cfg.PTBandWidth, Cfg.PTPeakDetectionStrength,
			   PTFilter,TFType, Cfg.PTMultExponent, Cfg.PTFilterLen, Cfg.PTDLMinGain, Cfg.PTDLStart,
			   InSig->getSampleRate(), Cfg.PTDLStartFreq, Cfg.PTDLEndFreq) == false) {
      sputs("PT stage: psychoacoustic target filter computation failed.");
      return false;
    }
    
    /* Dealloca l'array per la convoluzione target */
    delete[] PTTConv;
    
    /* Verifica se si deve salvare il filtro psicoacustico */
    if (Cfg.PTFilterFile != NULL) {
      /* Normalizzazione segnale risultante */
      if (Cfg.PTNormFactor > 0) {
	sputs("PT stage: psychoacoustic target filter normalization.");
	SigNormalize(PTFilter,Cfg.PTFilterLen,Cfg.PTNormFactor, (NormType) Cfg.PTNormType[0]);
      }
      
      /* Salva la componente MP */
      sputsp("PT stage: saving psychoacoustic target filter: ",Cfg.PTFilterFile);
      for(L = 0; L < Cfg.PTFilterLen; L++)
	buf2.push_back(PTFilter[L]);
      if (SND_WriteSignal(Cfg.PTFilterFile, buf2, 0, Cfg.PTFilterLen, InSig->getSampleRate(), (IFileType) Cfg.PTFilterFileType[0]) == false) {
	sputs("Psychoacoustic target filter save failed.");
	return false;
      }
      /*if (WriteSignal(Cfg.PTFilterFile,PTFilter,Cfg.PTFilterLen, (IFileType) Cfg.PTFilterFileType[0]) == false) {
	sputs("Psychoacoustic target filter save failed.");
	return false;
	}*/
    }
    
    /* Alloca l'array per la convoluzione filtro e target */
    sputs("PT stage: allocating psychoacoustic target correction filter convolution array.");
    PTTConvLen = Cfg.PTFilterLen  + WLen - 1;
    PTTConv = new DLReal[PTTConvLen];
    if (PTTConv == NULL) {
      sputs("PT stage: memory allocation failed.");
      return false;
    }
    
    /* Effettua la convoluzione tra filtro e target */
    sputs("PT stage: psychoacoustic target correction filter convolution...");
    if (FftwConvolve(PTFilter,Cfg.PTFilterLen,&ISRevOut[WStart],WLen,PTTConv) == false) {
      sputs("PT stage: convolution failed.");
      return false;
    }
    /* Dealloca il filtro target */
    delete PTFilter;
    
    /* Finestratura finale filtro risultante */
    if (Cfg.PTOutWindow > 0) {
      sputs("PT stage: psychoacoustic target correction filter windowing.");
      
      /* Verifica il tipo di filtro target */
      switch (TFType) {
      case MKSETFLinearPhase:
	WStart2 = (PTTConvLen - Cfg.PTOutWindow) / 2;
	WLen2 = Cfg.PTOutWindow;
	BlackmanWindow(&PTTConv[WStart2],WLen2);
	break;
	
      case MKSETFMinimumPhase:
	WStart2 = WStart;
	WLen2 = Cfg.PTOutWindow;
	BlackmanWindow(&PTTConv[WStart2],WLen2);
	break;
      }
    } else {
      /* Verifica il tipo di filtro target */
      switch (TFType) {
      case MKSETFLinearPhase:
	WStart2 = WStart;
	WLen2 = PTTConvLen;
	BlackmanWindow(&PTTConv[WStart2],WLen2);
	break;
	
      case MKSETFMinimumPhase:
	WStart2 = WStart;
	WLen2 = WLen;
	BlackmanWindow(&PTTConv[WStart2],WLen2);
	break;
      }
    }
    
    OutSig->setWStart(WStart2);
    OutSig->setWLen(WLen2);
    
    for(K = 0; K < PTTConvLen; K++)
      OutSig->Data.push_back(PTTConv[K]);
  }
  sputs("DRC: Finished Psychoacoustic Target stage (PT).");
  return true;
}

void PTstage::Normalize(void)
{
  if (Cfg.PTNormFactor > 0) {
    sputs("PT stage: output component normalization.");
    OutSig->Normalize(Cfg.PTNormFactor,Cfg.PTNormType);
  }
}

void PTstage::WriteOutput(void)
{
  if (Cfg.PTOutFile != NULL) {
    sputsp("PT stage: saving output component: ",Cfg.PTOutFile);
    if (OutSig->WriteSignal(Cfg.PTOutFile, Cfg.PTOutFileType) == false) {
      sputs("PT stage: output component save failed.");
    }
  }
}
