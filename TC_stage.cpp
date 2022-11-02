#include "TC_stage.h"

TCstage::TCstage(DRCSignal *MsInputSig, DRCSignal *FtInputSig, TCParmsType InCfg)
{
  MsInSig = MsInputSig;
  FtInSig = FtInputSig;
  OutSig = new DRCSignal(*FtInSig);
  Cfg = InCfg;
}
TCstage::~TCstage()
{
}
void TCstage::NewInputSignal(DRCSignal *MsInputSig)
{
  MsInSig = MsInputSig;
  delete OutSig;
  OutSig = new DRCSignal(*FtInSig);
}
void TCstage::NewFilterSignal(DRCSignal *FtInputSig)
{
  FtInSig = FtInputSig;
  delete OutSig;
  OutSig = new DRCSignal(*FtInSig);
}
void TCstage::NewInCfg(TCParmsType InCfg)
{
  Cfg = InCfg;
}

bool TCstage::process(void)
{
  DLReal SRMSValue;
  sputs("DRC: Test Convolution stage (TC).");

  /* Effettua la convoluzione */
  sputs("TC stage: convolving input signal with target response signal...");
  OutSig->setData(STL_Convolve_part(MsInSig->getData(), MsInSig->getWStart(), 
				    MsInSig->getWLen(),
				    FtInSig->getData(), FtInSig->getWStart(), 
				    FtInSig->getWLen()),
				    0, MsInSig->getWLen()+FtInSig->getWLen()+1);
  /* Calcola il valore RMS del segnale dopo la filtratura */
  SRMSValue = STL_GetRMSLevel(OutSig->getData());
  if (SRMSValue >= 0)
    printf("TC stage: filtered signal RMS level %f (%f dB).\n",(double) SRMSValue, (double) (20 * log10((double) SRMSValue)));
  else
    printf("TC stage: filtered signal RMS level %f (-inf dB).\n",(double) SRMSValue);
  fflush(stdout);
      
  /* Calcola la dimensione in uscita */
  if (Cfg.PSFilterType[0] == 'T')
    OutSig->setWLen(Cfg.BCInitWindow + 2 * Cfg.ISPELowerWindow);
  else
    OutSig->setWLen(MsInSig->getWLen()+FtInSig->getWLen()+1);
      
  sputs("TC stage: test convolution done.");
  sputs("DRC: Finished Test Convolution stage (TC).");
  return true;
}


void TCstage::Normalize(void)
{
  if (Cfg.TCNormFactor > 0) {
    sputs("TC stage: output component normalization.");
    OutSig->Normalize(Cfg.TCNormFactor,Cfg.TCNormType);
  }
}

void TCstage::WriteOutput(void)
{
  if (Cfg.TCOutFile != NULL) {
    sputsp("TC stage: saving output component: ",Cfg.TCOutFile);
    if (OutSig->WriteSignal(Cfg.TCOutFile, Cfg.TCOutFileType) == false) {
      sputs("TC stage: output component save failed.");
    }
  }
}

