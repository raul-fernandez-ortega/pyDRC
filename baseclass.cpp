#include "baseclass.h"

DRCSignal::DRCSignal()
{
  PreWindowLen = 0;
  PSStart = 0;
  PSEnd = 0;
  WStart = 0;
  WLen = 0;
  SampleRate = 0;
  Path = NULL;
}
DRCSignal::DRCSignal(DRCSignal &SCopy)
{
  PreWindowLen = SCopy.getPreWindowLen();
  Data = SCopy.getData();
  PSStart = SCopy.getPSStart();
  PSEnd = SCopy.getPSEnd();
  WStart = SCopy.getWStart();
  WLen = SCopy.getWLen();
  SampleRate = SCopy.getSampleRate();
  Path = SCopy.getPath();
}
void DRCSignal::setParams(DRCSignal &SCopy)
{
  PreWindowLen = SCopy.getPreWindowLen();
  PSStart = SCopy.getPSStart();
  PSEnd = SCopy.getPSEnd();
  WStart = SCopy.getWStart();
  WLen = SCopy.getWLen();
  SampleRate = SCopy.getSampleRate();
  Path = SCopy.getPath();
}
void DRCSignal::setData(STLvectorReal InData, int psstart, int psend)
{
  Data = InData;
  PSStart = psstart;
  PSEnd = psend;
}
void DRCSignal::clearData(void)
{
  Data.clear();
}
void DRCSignal::setPreWindowLen(int prewindowlen)
{
  PreWindowLen = prewindowlen;
  return;
}
void DRCSignal::setPSStart(int psstart)
{
  PSStart = psstart;
  return;
}
void DRCSignal::setPSEnd(int psend)
{
  PSEnd = psend;
  return;
}
void DRCSignal::setWStart(int wstart)
{
  WStart = wstart;
  return;
}
void DRCSignal::setWLen(int wlen)
{
  WLen = wlen;
  return;
}
void DRCSignal::setSampleRate(int srate)
{
  SampleRate = srate;
  return;
}
void DRCSignal::setPath(char *path)
{
  Path = (char*) malloc(sizeof(char)*(strlen(path)+1));
  strcpy(Path,path);
  return;
}

STLvectorReal DRCSignal::getData(void)
{
  return Data;
}
int DRCSignal::getPreWindowLen(void)
{
  return PreWindowLen;
}

int DRCSignal::getPSStart(void)
{
  return PSStart;
}
int DRCSignal::getPSEnd(void)
{
  return PSEnd;
}
int DRCSignal::getWStart(void)
{
  return WStart;
}
int DRCSignal::getWLen(void)
{
  return WLen;
}
int DRCSignal::getSampleRate(void)
{
  return SampleRate;
}
char *DRCSignal::getPath(void)
{
  return Path;
}
void DRCSignal::Normalize(DLReal normfactor, char* normtype)
{
  if (normfactor > 0) {
    printf("DRCSignal: input signal normalization: %s type.\n",normtype);
    STL_SigNormalize(Data,normfactor,(NormType) normtype[0]);
  }
}

bool DRCSignal::WriteSignal(char* outfile, char* outfiletype) 
{
  char *path_outfile;
  if (outfile !=NULL) {
    if(Path!=NULL) {
      path_outfile = (char*) malloc(sizeof(char)*(1+strlen(Path)+strlen(outfile)));
      strcpy(path_outfile,Path);
      strcat(path_outfile,outfile);
    } else {
      path_outfile = (char*) malloc(sizeof(char)*(1+strlen(outfile)));
      strcpy(path_outfile,outfile);
    }
    printf("DRCSignal: saving signal: %s\n",path_outfile);
    if (SND_WriteSignal(path_outfile,Data,WStart,WLen, SampleRate, (IFileType) outfiletype[0]) == false) {
      printf("DRCSignal: signal saving failed.\n");
      return false;
    }
    return true;
  }
  return false;
}

STLvectorReal STL_ToeplitzSolve(STLvectorReal Av, STLvectorReal Bv, unsigned int N)
{
  unsigned int I;
  DLReal *A;
  DLReal *B;
  DLReal *X;
  STLvectorReal Xv;

  if(Av.size() < N || Bv.size() < N) {
    printf("Toeplitz vectors size less than N factor\n");
    fflush(stdout);
    return Xv;
  }
  A = new DLReal[N];
  if (A == NULL) {
    sputs("Memory allocation failed.");
    return Xv;
  }
  B = new DLReal[N];
  if (B == NULL) {
    sputs("Memory allocation failed.");
    return Xv;
  }
  X = new DLReal[N];
  if (X == NULL) {
    sputs("Memory allocation failed.");
    return Xv;
  }
  for(I = 0; I < N; I++) { 
    A[I] = Av[I];
    B[I] = Bv[I];
  }
  if(ToeplitzSolve(A, B, X, N) == false)
    return Xv;
  for(I = 0; I < N; I++)
    Xv.push_back(X[I]);
  return Xv;
}

STLvectorReal STL_KirkebyFDInvert(const STLvectorReal InSig, unsigned int InSigLen, 
				  unsigned int InvFilterSigLen, const STLvectorReal EffortSig, 
				  DLReal EffortFactor, int MExp)
{
  unsigned int I;
  DLReal *InS;
  DLReal *EffortS;
  DLReal *InvFilterS;
  STLvectorReal InvFilterSig;

  InS = new DLReal[InSig.size()];
  if (InS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EffortS = new DLReal[EffortSig.size()];
  if (EffortS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < InSig.size(); I++) 
    InS[I] = InSig[I];
  for(I = 0; I < EffortSig.size(); I++) 
    EffortS[I] = EffortSig[I];
  if(KirkebyFDInvert(InS, InSig.size(), InvFilterS, InvFilterSigLen, EffortS, EffortSig.size(), EffortFactor,MExp)== true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig.push_back(InvFilterS[I]);
  }
  return InvFilterSig;
}
STLvectorReal STL_PEISMPKirkebyFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig, 
					unsigned int InvFilterSigLen, DLReal EffortFactor, 
					int MExp)
{
  unsigned int I;
  DLReal *MPS;
  DLReal *EPS;
  DLReal *InvFilterS;
  STLvectorReal InvFilterSig;
  
  MPS = new DLReal[MPSig.size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig.size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig.size(); I++) 
    MPS[I] = MPSig[I];
  for(I = 0; I < EPSig.size(); I++) 
    EPS[I] = EPSig[I];
  if(PEISMPKirkebyFDInvert(MPS, MPSig.size(), EPS, EPSig.size(), InvFilterS, InvFilterSigLen, EffortFactor, MExp) == true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig.push_back(InvFilterS[I]);
  }
  return InvFilterSig;
}

STLvectorReal STL_PEMSMPKirkebyFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig,
					unsigned int InvFilterSigLen, DLReal EffortFactor, 
					int PEStart, int PETransition, int MExp)
{
  unsigned int I;
  DLReal *MPS;
  DLReal *EPS;
  DLReal *InvFilterS;
  STLvectorReal InvFilterSig;
  
  MPS = new DLReal[MPSig.size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig.size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig.size(); I++) 
    MPS[I] = MPSig[I];
  for(I = 0; I < EPSig.size(); I++) 
    EPS[I] = EPSig[I];
  if(PEMSMPKirkebyFDInvert(MPS, MPSig.size(), EPS, EPSig.size(), InvFilterS, InvFilterSigLen, EffortFactor, PEStart, PETransition, MExp) == true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig.push_back(InvFilterS[I]);
  }
  return InvFilterSig;
}

STLvectorReal STL_PETFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig, 
			      unsigned int InvFilterSigLen, char PEType, int PELowerWindow, 
			      int PEUpperWindow, int PEStartFreq, int PEEndFreq, 
			      int PEFilterLen, DLReal FSharpness, int PEBandSplit, 
			      DLReal PEWindowExponent,const DLReal OGainFactor, 
			      int SampleRate, int MExp, const SLPPrefilteringType SLPType)
{
  unsigned int I;
  DLReal *MPS;
  DLReal *EPS;
  DLReal *InvFilterS;
  STLvectorReal InvFilterSig;
  
  MPS = new DLReal[MPSig.size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig.size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig.size(); I++) 
    MPS[I] = MPSig[I];
  for(I = 0; I < EPSig.size(); I++) 
    EPS[I] = EPSig[I];
  if(PETFDInvert(MPS, MPSig.size(), EPS, EPSig.size(), InvFilterS,InvFilterSigLen, PEType, 
		 PELowerWindow, PEUpperWindow, PEStartFreq, PEEndFreq, PEFilterLen, FSharpness,
		 PEBandSplit, PEWindowExponent, SLPType, OGainFactor, SampleRate, MExp)== true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig.push_back(InvFilterS[I]);
  }
  return InvFilterSig;
}
