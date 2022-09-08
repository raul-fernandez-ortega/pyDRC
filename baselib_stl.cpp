#include "baselib_stl.h"

/* Signal reading from a raw file. Data length InitWindow, centered at ImpulseCenter */
STLvectorReal *STL_ReadSignal(const char * FName,const int InitWindow, const int ImpulseCenter,
			      const IFileType FType,int &PreSpikeStart, int &PostSpikeEnd)
{
  unsigned int I; 
  int MaxWindow, RWDim;

  FILE * IOF;
  STLvectorReal *RetSig = new STLvectorReal();

  switch (FType)
    {
    case PcmFloat64Bit:
      RWDim = sizeof(DRCFileDouble);
      break;
      
    case PcmFloat32Bit:
      RWDim = sizeof(DRCFileFloat);
      break;
      
    case PcmInt16Bit:
      RWDim = sizeof(DRCFileInt);
      break;
    }
  if ((IOF = fopen(FName,"rb")) == NULL)
    {
      perror("\nUnable to open input file");
      return RetSig;
    }  
  MaxWindow = FSize(IOF)/RWDim;
  fclose(IOF);
  MaxWindow = ((InitWindow > MaxWindow) ? (MaxWindow) : (InitWindow) );
  DLReal *Dst = new DLReal[MaxWindow];
  if(ReadSignal(FName, Dst, MaxWindow, ImpulseCenter, FType, &PreSpikeStart, &PostSpikeEnd) == true)
      for(I = 0; I < (unsigned int)MaxWindow; I++)
	RetSig->push_back(Dst[I]);
  delete Dst;
  return RetSig;
}


STLvectorReal *STL_SND_ReadSignal(const char * FName, const int InitWindow, const int ImpulseCenter, int &PreSpikeStart, int &PostSpikeEnd)
{
  SNDFILE* sf_file;
  SF_INFO sf_info;
  int MaxWindow;

  unsigned int I; 
  STLvectorReal *RetSig = new STLvectorReal();

  sf_info.format = 0;

  if((sf_file = sf_open(FName, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for input (%s)\n",FName, sf_strerror(sf_file));
    return RetSig;
  }
  if (sf_info.frames == 0) {
    puts("\nInput file is zero length.");
    return RetSig;
  }
  MaxWindow = ((InitWindow > sf_info.frames) ? (sf_info.frames) : (InitWindow) );

  DLReal *Dst = new DLReal[MaxWindow];

  if(SND_ReadSignal(FName, Dst, MaxWindow, ImpulseCenter, &PreSpikeStart, &PostSpikeEnd) == true)
      for(I = 0; I < (unsigned int)MaxWindow; I++)
	RetSig->push_back(Dst[I]);
  delete Dst;
  return RetSig;
  } 

bool STL_WriteSignal(const char * FName,STLvectorReal *Src,const unsigned int WStart,
		     const unsigned int WLen, const IFileType FType)
{
  bool retval;
  unsigned int I, J, SSize = Src->size();

  if (SSize < WStart)
    return false;
  DLReal *Dst = new DLReal[WLen];
  if (SSize < WStart + WLen)
    {
      for(J = 0,I = WStart; I < SSize; I++, J++)
	Dst[J] = Src->at(I);
      for(J = SSize - WStart; J < WLen; J++)
	Dst[J] = 0;
    }
  else
    for(J = 0,I = WStart; J < WLen; I++, J++)
      Dst[J] = Src->at(I);
  retval = (WriteSignal(FName,Dst,WLen,FType)== true);
  delete Dst;
  return retval;
}

bool SND_WriteSignal(const char * FName,STLvectorReal *Src,const unsigned int WStart, 
		     const unsigned int WLen, const int SampleRate, const IFileType FType)

{
  SNDFILE* sf_file;
  SF_INFO sf_info;
  int short_mask;
  unsigned int I, J, SSize = Src->size();
  
  if (SSize < WStart)
    return false;

  DLReal *Dst = new DLReal[WLen];
  if (SSize < WStart + WLen) {
    for(J = 0,I = WStart; I < SSize; I++, J++)
      Dst[J] = Src->at(I);
    for(J = SSize - WStart; J < WLen; J++)
      Dst[J] = 0;
  }
  else
    for(J = 0,I = WStart; J < WLen; I++, J++)
      Dst[J] = Src->at(I);
  
  sf_info.samplerate = SampleRate;
  sf_info.channels = 1;
  sf_info.frames = 0;
  sf_info.sections = 0;
  sf_info.seekable = 0;
  switch (FType) 
    {
    case PcmFloat64Bit:
      short_mask = SF_FORMAT_DOUBLE | SF_ENDIAN_FILE;
      break;
      
    case PcmFloat32Bit:
      short_mask = SF_FORMAT_FLOAT | SF_ENDIAN_FILE;
      break;
      
    case PcmInt16Bit:
      short_mask = SF_FORMAT_PCM_16 | SF_ENDIAN_FILE;
      break;
    }
  sf_info.format = SF_FORMAT_WAV | short_mask;
  if((sf_file = sf_open(FName, SFM_WRITE, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",FName, sf_strerror(sf_file));
    return false;
  }
  sf_writef_double(sf_file, Dst, WLen);
  sf_close(sf_file);
  return true;
}

bool STL_OverwriteSignal(const char * FName,STLvectorReal *Src,const int Skip, const IFileType FType)
{
  bool retval;
  unsigned int I, SSize = Src->size();
  DRCFloat *Dst = new DRCFloat[SSize];
  for(I = 0; I < SSize; I++)
    Dst[I] = Src->at(I);
  retval = (OverwriteSignal(FName,Dst,SSize,Skip,FType) == true);
  delete Dst;
  return retval;  
}

/* Calcola l'autocorrelazione del sgnale S */
STLvectorReal *STL_AutoCorrelation(STLvectorReal *Src)
{
  STLvectorReal *RetSig = new STLvectorReal();
  unsigned int I, SSize = Src->size();
  DLReal *Dst = new DLReal[SSize];
  for(I = 0; I < SSize; I++)
    Dst[I] = Src->at(I);
  if (AutoCorrelation(Dst,SSize) == true)
    for(I = 0; I < SSize; I++)
      RetSig->push_back(Dst[I]);
  delete Dst;
  return RetSig;
}
STLvectorReal *STL_CrossCorrelation(STLvectorReal *S1, STLvectorReal *S2)
{
  STLvectorReal *RetSig = new STLvectorReal();
  unsigned int I, SSize1 = S1->size(), SSize2 = S2->size(), SSizeR = S1->size()*2-1;
  DLReal *Dst1 = new DLReal[SSize1];
  DLReal *Dst2 = new DLReal[SSize2];
  DLReal *retvec = new DLReal[SSizeR];
  for(I = 0; I < SSize1; I++)
    Dst1[I] = S1->at(I);
  for(I = 0; I < SSize2; I++)
    Dst2[I] = S2->at(I);
  if (CrossCorrelation(Dst1,Dst2,SSize1,retvec) == true)
    for(I = 0; I < SSizeR; I++)
      RetSig->push_back(retvec[I]);
  delete Dst1;
  delete Dst2;
  delete retvec;
  return RetSig;
}
STLvectorReal *STL_GroupDelay(STLvectorReal *Src)
{
  STLvectorReal *RetSig = new STLvectorReal();
  unsigned int I, SSize = Src->size();
  DLReal *Dst = new DLReal[SSize];
  DLReal *retvec = new DLReal[SSize];
  for(I = 0; I < SSize; I++)
    Dst[I] = Src->at(I);
  if (GroupDelay(Dst,SSize,retvec) == true)
    for(I = 0; I < SSize; I++)
      RetSig->push_back(retvec[I]);
  delete Dst;
  delete retvec;
  return RetSig;
}

unsigned int SND_FSize(const char * FName)
{
  SNDFILE* sf_file;
  SF_INFO sf_info;
  unsigned int retval;
   if((sf_file = sf_open(FName, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",FName, sf_strerror(sf_file));
    sf_close(sf_file);
    return 0;
   } else {
     retval = (unsigned int)sf_info.frames;
     sf_close(sf_file);
     return retval;
   }
}

int SND_GetSampleRate(const char * FName)
{
  SNDFILE* sf_file;
  SF_INFO sf_info;
   if((sf_file = sf_open(FName, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",FName, sf_strerror(sf_file));
    sf_close(sf_file);
    return 0;
   } else {
     sf_close(sf_file);
     return sf_info.samplerate;
   }
}

int STL_FindMaxPcm(const char * FName,const IFileType FType)
{
  int result;
  result= FindMaxPcm(FName, FType);
  return result;
}

int SND_FindMaxPcm(const char * FName)
{
  SNDFILE* sf_file;
  SF_INFO sf_info;
  int i;
  DLReal MaxPcm;
  int result;

  MaxPcm = 0;
  result = 0;
  if((sf_file = sf_open(FName, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",FName, sf_strerror(sf_file));
    return result;
  }
  DLReal *Dst = new DLReal[sf_info.frames];
  sf_readf_double(sf_file, Dst, sf_info.frames);
  
  for(i = 0; i < sf_info.frames; i++) {
    if(fabs(Dst[i]) > MaxPcm) {
      MaxPcm = fabs(Dst[i]);
      result = i;
    }
  }
  delete Dst;
  return result;
}

bool STL_ReadPoints(char* CorrFile, const TFMagType MagType, STLvectorReal *FilterFreq,
		    STLvectorReal *FilterM, STLvectorReal *FilterP, const int NPoints,
		    int SampleRate)
{
  int I;
  DLReal *filterfreq = new DLReal[NPoints];
  DLReal *filterm = new DLReal[NPoints];
  DLReal *filterp = new DLReal[NPoints];
  
  if(ReadPoints(CorrFile, MagType, filterfreq, filterm, filterp, NPoints, SampleRate) == true)
    {
      for(I = 0; I < NPoints; I++)
	{
	  FilterFreq->push_back(filterfreq[I]);
	  FilterM->push_back(filterm[I]);
	  FilterP->push_back(filterp[I]);
	}
      return true;
    }
  else
    return false;
}

bool GlSweep(DLReal Rate, DLReal Amplitude, DLReal HzStart, DLReal HzEnd, DLReal Duration, DLReal Silence, 
	     DLReal LeadIn, DLReal LeadOut, char *SweepFile, char *InverseFile)
{

  /* Base sweep generation */
  STLvectorReal *SweepSig = new STLvectorReal();
  STLvectorReal *InvSig = new STLvectorReal();
  int SweepLen;
  int SilenceLen;
  DLReal W1;
  DLReal W2;
  DLReal Ratio;
  DLReal Sample;
  DLReal S1;
  DLReal S2;
  DLReal DecayTime;
  DLReal Decay;
  DLReal RMSV;
  DLReal RMS;
  int I;
  int J;
  float FS;
  
  /* Lead in and lead out Blackman windowing */
  int LeadInLen;
  DLReal WC1In;
  DLReal WC2In;
  int LeadOutLen;
  DLReal WC1Out;
  DLReal WC2Out;
  DLReal WC;
  
  /* Base sweep generation */
  SweepLen = (int) (Rate * Duration);
  SilenceLen = (int) (Rate * Silence);
  W1 = (DLReal) (HzStart * M_2PI);
  W2 = (DLReal) (HzEnd * M_2PI);
  Ratio = (DLReal) log(W2 / W1);
  S1 = (DLReal) ((W1 * Duration) / Ratio);
  S2 = (DLReal) (Ratio / SweepLen);
  DecayTime = (DLReal) (SweepLen * log(2.0) / Ratio);
  
  /* Lead in and lead out Blackman windowing */
  LeadInLen = (int) (LeadIn * SweepLen);
  WC1In = (DLReal) M_PI / (LeadInLen - 1);
  WC2In = (DLReal) M_2PI / (LeadInLen - 1);
  LeadOutLen = (int) (LeadOut * SweepLen);
  WC1Out = (DLReal) M_PI / (LeadOutLen - 1);
  WC2Out = (DLReal) M_2PI / (LeadOutLen - 1);

  /* Report generation parameters */
  printf("\nSweep length: %d samples\n",SweepLen);
  printf("Silence length: %d samples\n",SilenceLen);
  printf("Total sweep length: %d samples\n",2 * SilenceLen + SweepLen);
  printf("Total sweep file size: %u bytes\n",sizeof(float) * (2 * SilenceLen + SweepLen));
  printf("Total inverse length: %d samples\n",SweepLen);
  printf("Total inverse file size: %u bytes\n\n",sizeof(float) * SweepLen);
  fflush(stdout);
  
  /* Generates the sweep file */
  sputs("Generating the sweep file...");
  
  /* Initial silence */
  FS = (DLReal) 0.0;
  for (I = 0;I < SilenceLen;I++)
    SweepSig->push_back((DLReal)0);
    //fwrite(&FS,sizeof(float),1,OF);
  
  /* Initial lead in */
  for (I = 0;I < LeadInLen;I++) {
    Sample = (DLReal) DLSin(S1 * (exp(I * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1In * I) + 0.08 * DLCos(WC2In * I));
    FS = (float) (Sample * WC * Amplitude);
    SweepSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  /* Full sweep */
  for (I = LeadInLen;I < SweepLen - LeadOutLen;I++) {
    Sample = (DLReal) DLSin(S1 * (exp(I * S2) - 1.0));
    FS = (float) (Sample * Amplitude);
    SweepSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  /* Final lead out */
  for (I = SweepLen - LeadOutLen,J = LeadOutLen;I < SweepLen;I++,J--) {
    Sample = (DLReal) DLSin(S1 * (exp(I * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1Out * J) + 0.08 * DLCos(WC2Out * J));
    FS = (float) (Sample * WC * Amplitude);
    SweepSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  /* Final silence */
  FS = (DLReal) 0.0;
  for (I = 0;I < SilenceLen;I++)
    SweepSig->push_back((DLReal)0);
    //fwrite(&FS,sizeof(float),1,OF);
  
  /* Close the sweep file */
  sputs("Sweep file generated.");

  SND_WriteSignal(SweepFile, SweepSig, 0, SweepSig->size(), Rate, PcmFloat32Bit);
  
  /* Computes the inverse normalization factor */
  sputs("Inverse normalization computation...");
  RMS = 0;
  
  // Final lead out 
  for (I = 0,J = SweepLen;I < LeadOutLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1Out * I) + 0.08 * DLCos(WC2Out * I));
    RMSV = (DLReal) (Sample * WC * Decay);
    RMS += RMSV * RMSV;
  }
  
  // Full sweep
  for (I = LeadOutLen,J = SweepLen - LeadOutLen;I < SweepLen - LeadInLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    RMSV = (DLReal) (Sample * Decay);
    RMS += RMSV * RMSV;
  }
  
  // Initial lead in
  for (I = SweepLen - LeadInLen,J = LeadInLen;I < SweepLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1In * J) + 0.08 * DLCos(WC2In * J));
    RMSV = (DLReal) (Sample * WC * Decay);
    RMS += RMSV * RMSV;
  }
  
  // Computes the normalization factor
  printf("Normalizaton factor: %f\n",RMS);
  fflush(stdout);
  RMS = (DLReal) 0.5 / RMS;
  
 
  // Generates the sweep file
  sputs("Generating the inverse file...");
  
  // Final lead out 
  for (I = 0,J = SweepLen;I < LeadOutLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1Out * I) + 0.08 * DLCos(WC2Out * I));
    FS = (float) (RMS * Sample * WC * Decay);
    InvSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  // Full sweep
  for (I = LeadOutLen,J = SweepLen - LeadOutLen;I < SweepLen - LeadInLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    FS = (float) (RMS * Sample * Decay);
    InvSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  // Initial lead in
  for (I = SweepLen - LeadInLen,J = LeadInLen;I < SweepLen;I++,J--) {
    Decay = (DLReal) pow(0.5,I / DecayTime);
    Sample = (DLReal) DLSin(S1 * (exp(J * S2) - 1.0));
    WC = (DLReal) (0.42 - 0.5 * DLCos(WC1In * J) + 0.08 * DLCos(WC2In * J));
    FS = (float) (RMS * Sample * WC * Decay);
    InvSig->push_back(FS);
    //fwrite(&FS,sizeof(float),1,OF);
  }
  
  // Close the inverse file
  sputs("Inverse file generated.");

  SND_WriteSignal(InverseFile, InvSig, 0, InvSig->size(), Rate, PcmFloat32Bit);
  
  // Execution completed
  delete SweepSig;
  delete InvSig; 
  return true;
}

bool LsConv(char *SweepFile, char *InverseFile, char *OutFile)
{
  /* Convolution parameters */
  int SS;
  int IS;
  int I;
  DLReal *Sweep;
  DLReal *Inverse;
  DLReal *Convol;
    
  /* Peak position */
  int MP;
  DLReal AMax;
  
  /* Input/output file */
  SNDFILE* sweep_file;
  SF_INFO sweep_info;

  SNDFILE* inv_file;
  SF_INFO inv_info;
  
  /* Open the sweep file */
  sputsp("Opening sweep file: ",SweepFile);
  if((sweep_file = sf_open(SweepFile, SFM_READ, &sweep_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",SweepFile, sf_strerror(sweep_file));
    return false;
  }

  if (sweep_info.frames == 0) {
    puts("\nInput file is zero length.");
    return false;
  }

  SS = sweep_info.frames;

  /* Open the inverse file */
  sputsp("Reading inverse file: ",InverseFile);
  if((inv_file = sf_open(InverseFile, SFM_READ, &inv_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",InverseFile, sf_strerror(inv_file));
    return false;
  }

  if (inv_info.frames == 0) {
    puts("\nInverse file is zero length.");
    return false;
  }

  IS = inv_info.frames;

  //CL = max(SS,IS);
  //for(CS = 1;CS < CL;CS <<= 1);

  Sweep = new DLReal[SS];
  memset(Sweep, 0, sizeof(DLReal)*SS);
  
  Inverse = new DLReal[IS];
  memset(Inverse, 0, sizeof(DLReal)*IS);

  Convol = new DLReal [SS + IS - 1];
  memset(Convol,0,sizeof(DLReal)*(SS + IS -1));

  sf_readf_double(sweep_file, Sweep, sweep_info.frames);
  sf_close(sweep_file);
  
  sf_readf_double(inv_file, Inverse, inv_info.frames);
  sf_close(inv_file);			
       
  /* Convolving sweep and inverse */		
  sputs("Sweep and inverse convolution...");
  FftwConvolve(Sweep, SS, Inverse, IS, Convol);


  /* Peak search */
  sputs("Finding impulse response peak value...");
  MP = 0;
  AMax = (DLReal) 0.0;
  for (I = 0;I < SS + IS -1; I++)
    if ((DLReal) fabs(Convol[I]) > AMax) {
      MP = I;
      AMax = (DLReal) fabs(Convol[I]);
    }
  printf("Peak position: %d\n",MP);
  if (AMax > (DLReal) 0.0)
    printf("Peak value: %f (%f dB)\n",(double) AMax, (double) (20 * log10((double) AMax)));
  else
    printf("Peak value: %f (-inf dB)\n",(double) AMax);
  fflush(stdout);
  
  if(AMax > (DLReal) 1.0)
    for (I = 0;I < SS + IS -1; I++)
      Convol[I] /= 2 * AMax;
  
  /* Writes output file */
  sputsp("Writing output file: ",OutFile);
  if((sweep_file = sf_open(OutFile, SFM_WRITE, &sweep_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for output (%s)\n",OutFile, sf_strerror(sweep_file));
    return false;
  }
  sf_writef_double(sweep_file, Convol, SS + IS - 1);
  sf_close(sweep_file);
  return true;

  /* Memory deallocation */
  delete Sweep;
  delete Inverse;
  delete Convol ;
  /* Execution completed */
  sputs("Completed.");
  return true;
}

