#include "BC_stage.h"

BCstage::BCstage(BCParmsType InCfg)
{
  OutSignal = new DRCSignal();
  NewCfgParms(InCfg);
  OutSignal->setPreWindowLen(Cfg.BCPreWindowLen);
  OutSignal->setSampleRate(Cfg.BCSampleRate);
  //process();
}

void BCstage::NewCfgParms(BCParmsType InCfg)
{
  Cfg = InCfg;
  OutSignal->setPreWindowLen(Cfg.BCPreWindowLen);
  OutSignal->setSampleRate(Cfg.BCSampleRate);
}

bool BCstage::process(void)
{
  int I, PSStart, PSEnd;
  DLReal* InSig;

  OutSignal->clearData();
  DLReal SRMSValue;

  /* Controlla il tipo ricerca centro impulso */
  if (Cfg.BCImpulseCenterMode[0] == 'A')  {
    /* Ricerca il centro impulso */
    sputsp("Seeking impulse center on: ", Cfg.BCInFile);
    //Cfg.BCImpulseCenter = FindMaxPcm(Cfg.BCInFile,(IFileType) Cfg.BCInFileType[0]);
    Cfg.BCImpulseCenter = SND_FindMaxPcm(Cfg.BCInFile);
    if (Cfg.BCImpulseCenter < 0)
      return false;
    printf("Impulse center found at sample %i.\n",Cfg.BCImpulseCenter);
    fflush(stdout);
  }  
  /* Alloca l'array per il segnale in ingresso */
  sputs("Allocating input signal array.");
  InSig = new DLReal[Cfg.BCInitWindow];
  if (InSig == NULL) {
    sputs("Memory allocation failed.");
    return false;
  }  
  /* Legge il file di ingresso */
  sputsp("Reading input signal: ",Cfg.BCInFile);
  /*if (ReadSignal(Cfg.BCInFile,InSig,Cfg.BCInitWindow,Cfg.BCImpulseCenter, (IFileType) Cfg.BCInFileType[0],&PSStart,&PSEnd) == False) {
    sputs("Error reading input signal.");
    return false;
    }*/
  if (SND_ReadSignal(Cfg.BCInFile,InSig,Cfg.BCInitWindow,Cfg.BCImpulseCenter,&PSStart,&PSEnd) == false) {
    sputs("Error reading input signal.");
    return false;
  }
  sputs("Input signal read.");
  
  /* Effettua la prefinestratura del segnale */
  if (Cfg.BCPreWindowLen > 0) {
    sputs("Input signal prewindowing.");
    
    if ((Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen) < PSStart)
      sputs("!!Warning: input signal too short for correct signal prewindowing, spurios spikes may be generated.");
    
    for (I = 0;I < Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen;I++)
      InSig[I] = 0;
    SpacedBlackmanWindow(&InSig[Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen],Cfg.BCPreWindowLen,Cfg.BCPreWindowGap,WLeft);
  }
  
  SRMSValue = GetRMSLevel(InSig,Cfg.BCInitWindow);
  if (SRMSValue > 0)
    printf("Input signal RMS level %f (%f dB).\n",(double) SRMSValue, (double) (20 * log10((double) SRMSValue)));
  else
    printf("Input signal RMS level %f (-inf dB).\n",(double) SRMSValue);
  fflush(stdout); 
  OutSignal->setPSStart(PSStart);
  OutSignal->setPSEnd(PSEnd);
  
  /* Verifica se si deve effettuare il dip limiting */
  if (Cfg.BCDLMinGain > 0) {
    switch (Cfg.BCDLType[0])
      {
	/* Fase lineare */
      case 'L':
	sputs("Input signal linear phase dip limiting...");
	if (C1LPDipLimit(InSig,Cfg.BCInitWindow,Cfg.BCDLMinGain,Cfg.BCDLStart,
			 Cfg.BCSampleRate,Cfg.BCDLStartFreq,Cfg.BCDLEndFreq,Cfg.BCDLMultExponent) == false) {
	  sputs("Dip limiting failed.");
	  return 1;
	}
	break;
	  
	/* Fase minima */
      case 'M':
	sputs("Input signal minimum phase dip limiting...");
	if (C1HMPDipLimit(InSig,Cfg.BCInitWindow,Cfg.BCDLMinGain,Cfg.BCDLStart,
			  Cfg.BCSampleRate,Cfg.BCDLStartFreq,Cfg.BCDLEndFreq,Cfg.BCDLMultExponent) == false) {
	  sputs("Dip limiting failed.");
	  return 1;
	}
	break;
      }
  }
  for (I = 0; I < Cfg.BCInitWindow; I++)
    OutSignal->Data.push_back(InSig[I]);
  OutSignal->setWStart(0);
  OutSignal->setWLen(Cfg.BCInitWindow);
  OutSignal->setPath(Cfg.BCBaseDir);
  OutSignal->Normalize(Cfg.BCNormFactor,Cfg.BCNormType);
  OutSignal->WriteSignal(Cfg.BCOutFile, Cfg.BCOutFileType);
  return true;
}

bool BCstage::SeekImpulseCenter(void)
{
  printf("Seeking impulse center on: %s\n", Cfg.BCInFile);
  Cfg.BCImpulseCenter = STL_FindMaxPcm(Cfg.BCInFile,(IFileType) Cfg.BCInFileType[0]);
  if (Cfg.BCImpulseCenter < 0)
    return false;
  printf("Impulse center found at sample %i.\n",Cfg.BCImpulseCenter);
  return true;
}
bool BCstage::ReadImpulseFile(void)
{
  int PSStart = 0, PSEnd = 0;
  printf("Reading input signal file: %s\n ",Cfg.BCInFile);
  OutSignal->setData(STL_ReadSignal(Cfg.BCInFile, Cfg.BCInitWindow, 
				    Cfg.BCImpulseCenter,
				    (IFileType) Cfg.BCInFileType[0],
				    PSStart,PSEnd),
		     0,Cfg.BCInitWindow);
  OutSignal->setPSStart(PSStart);
  OutSignal->setPSEnd(PSEnd);
  if (OutSignal->Data.size()>0)
    return true;
  else
    return false;
}
