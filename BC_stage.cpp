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
  int samplerate;

  OutSignal->clearData();
  DLReal SRMSValue;

  sputs("DRC: Base Configuration stage (BC).");
  samplerate = SND_GetSampleRate(Cfg.BCInFile);
  if(samplerate > 0) 
    Cfg.BCSampleRate = samplerate;
  printf("BC stage: input signal sample rate:%i Hz.\n",Cfg.BCSampleRate);
  fflush(stdout);
  /* Controlla il tipo ricerca centro impulso */
  if (Cfg.BCImpulseCenterMode[0] == 'A')  {
    /* Ricerca il centro impulso */
        sputsp("BC stage: seeking impulse center on: ", Cfg.BCInFile);
    if(samplerate > 0) 
      Cfg.BCImpulseCenter = SND_FindMaxPcm(Cfg.BCInFile);
    else
      Cfg.BCImpulseCenter = FindMaxPcm(Cfg.BCInFile,(IFileType) Cfg.BCInFileType[0]);
    if (Cfg.BCImpulseCenter < 0)
      return false;
    printf("BC stage: impulse center found at sample %i.\n",Cfg.BCImpulseCenter);
    fflush(stdout);
  }
  /* Alloca l'array per il segnale in ingresso */
  InSig = new DLReal[Cfg.BCInitWindow];
  if (InSig == NULL) {
    sputs("BC Stage: memory allocation failed.");
    return false;
  }  
  /* Legge il file di ingresso */
  sputsp("BC stage: reading input signal: ",Cfg.BCInFile);

  if(samplerate > 0) {
    if (SND_ReadSignal(Cfg.BCInFile,InSig,Cfg.BCInitWindow,Cfg.BCImpulseCenter,&PSStart,&PSEnd) == false) {
      sputs("BC stage: error reading input signal.");
      return false;
    }
  }
  else {
    if (ReadSignal(Cfg.BCInFile,InSig,Cfg.BCInitWindow,Cfg.BCImpulseCenter,(IFileType) Cfg.BCInFileType[0],&PSStart,&PSEnd) == false) {
    sputs("BC stage: Error reading input signal.");
    return false;
    }
  }
  sputs("BC stage: Input signal read.");
  
  /* Effettua la prefinestratura del segnale */
  if (Cfg.BCPreWindowLen > 0) {
    sputs("BC stage: input signal prewindowing.");
    
    if ((Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen) < PSStart)
      sputs("BCstage: !!Warning: input signal too short for correct signal prewindowing, spurios spikes may be generated.");
    
    for (I = 0;I < Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen;I++)
      InSig[I] = 0;
    SpacedBlackmanWindow(&InSig[Cfg.BCInitWindow / 2 - Cfg.BCPreWindowLen],Cfg.BCPreWindowLen,Cfg.BCPreWindowGap,WLeft);
  }
  
  SRMSValue = GetRMSLevel(InSig,Cfg.BCInitWindow);
  if (SRMSValue > 0)
    printf("BC stage: Input signal RMS level %f (%f dB).\n",(double) SRMSValue, (double) (20 * log10((double) SRMSValue)));
  else
    printf("BC stage: Input signal RMS level %f (-inf dB).\n",(double) SRMSValue);
  fflush(stdout);
  
  OutSignal->setPSStart(PSStart);
  OutSignal->setPSEnd(PSEnd);
  
  for (I = 0; I < Cfg.BCInitWindow; I++)
    OutSignal->Data.push_back(InSig[I]);
  OutSignal->setWStart(0);
  OutSignal->setWLen(Cfg.BCInitWindow);
  OutSignal->setPath(Cfg.BCBaseDir);

  sputs("DRC: Finished Base Configuration stage (BC).");

  return true;
}

bool BCstage::SeekImpulseCenter(void)
{
  printf("BC stage: seeking impulse center on: %s\n", Cfg.BCInFile);
  Cfg.BCImpulseCenter = STL_FindMaxPcm(Cfg.BCInFile,(IFileType) Cfg.BCInFileType[0]);
  if (Cfg.BCImpulseCenter < 0)
    return false;
  printf("BC stage: impulse center found at sample %i.\n",Cfg.BCImpulseCenter);
  return true;
}
bool BCstage::ReadImpulseFile(void)
{
  int PSStart = 0, PSEnd = 0;
  printf("Reading input signal file: %s\n ",Cfg.BCInFile);
  OutSignal->setData(STL_ReadSignal(Cfg.BCInFile, Cfg.BCInitWindow, Cfg.BCImpulseCenter,(IFileType) Cfg.BCInFileType[0],PSStart,PSEnd),
		     0,Cfg.BCInitWindow);
  OutSignal->setPSStart(PSStart);
  OutSignal->setPSEnd(PSEnd);
  if (OutSignal->Data.size()>0)
    return true;
  else
    return false;
}

void BCstage::Normalize(void)
{
  if (Cfg.NormFactor > 0) {
    sputs("BC stage: output component normalization.");
    OutSignal->Normalize(Cfg.NormFactor,Cfg.NormType);
  }
}

void BCstage::WriteOutput(void)
{
  if (Cfg.OutFile != NULL) {
    sputsp("BC stage: saving output component: ",Cfg.OutFile);
    if (OutSignal->WriteSignal(Cfg.OutFile, Cfg.OutFileType) == false) {
      sputs("BC stage: output component save failed.");
    }
  }
}
