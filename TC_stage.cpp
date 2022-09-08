#include "TC_stage.h"

TCstage::TCstage(DRCSignal *MsInputSig, DRCSignal *FtInputSig, TCParmsType InCfg)
{
  MsInSig = MsInputSig;
  FtInSig = FtInputSig;
  OutSig = new DRCSignal(FtInSig);
  Cfg = InCfg;
}
TCstage::~TCstage()
{
}
void TCstage::NewInputSignal(DRCSignal *MsInputSig)
{
  MsInSig = MsInputSig;
  OutSig->setParams(MsInputSig);
}
void TCstage::NewFilterSignal(DRCSignal *FtInputSig)
{
  FtInSig = FtInputSig;
}
void TCstage::NewInCfg(TCParmsType InCfg)
{
  Cfg = InCfg;
}

void TCstage::process(void)
{
  DLReal SRMSValue;

  /* Effettua la convoluzione */
  sputs("Convolving input signal with target response signal...");
  OutSig->setData(STL_Convolve_part(MsInSig->getData(), MsInSig->getWStart(), MsInSig->getWLen(),
				    FtInSig->getData(), FtInSig->getWStart(), FtInSig->getWLen()));
  /*OutSig->setData(STL_Convolve_part(MsInSig->getData(), MsInSig->getWStart(), 
				    MsInSig->getWLen(),
				    FtInSig->getData(), FtInSig->getWStart(), 
				    FtInSig->getWLen()),
				    0, MsInSig->getWLen()+FtInSig->getWLen()+1);*/
  /* Calcola il valore RMS del segnale dopo la filtratura */
  SRMSValue = STL_GetRMSLevel(OutSig->getData());
  if (SRMSValue >= 0)
    printf("Filtered signal RMS level %f (%f dB).\n",(double) SRMSValue, (double) (20 * log10((double) SRMSValue)));
  else
    printf("Filtered signal RMS level %f (-inf dB).\n",(double) SRMSValue);
  fflush(stdout);

  //Normalize();
  
  /* Calcola la dimensione in uscita */
  if (Cfg.PSFilterType[0] == 'T')
    OutSig->setWLen(Cfg.BCInitWindow + 2 * Cfg.ISPELowerWindow);
  else
    OutSig->setWLen(MsInSig->getWLen()+FtInSig->getWLen()+1);
      
  //WriteOutput();
  
  sputs("Test convolution done.");
}

void TCstage::Normalize(void)
{
  if (Cfg.TCNormFactor > 0) {
    sputs("Test convolution signal normalization.");
    OutSig->Normalize(Cfg.TCNormFactor, Cfg.TCNormType);
  }
}

void TCstage::WriteOutput(void)
{
   if (Cfg.TCOutFile != NULL) {
     sputs("Saving test convolution signal.");
     if (OutSig->WriteSignal(Cfg.TCOutFile, Cfg.TCOutFileType)== false) {
       sputs("Test convolution save failed.");
       return;
     }
   }
}
