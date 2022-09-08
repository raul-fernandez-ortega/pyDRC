#include "baseclass.h"

DRCSignal::DRCSignal()
{
  PreWindowLen = 0;
  PSStart = 0;
  PSEnd = 0;
  WStart = 0;
  WLen = 0;
  SampleRate = 0;
  Data = new STLvectorReal();
  Path = NULL;
}
DRCSignal::DRCSignal(DRCSignal *SCopy)
{
  PreWindowLen = SCopy->getPreWindowLen();
  PSStart = SCopy->getPSStart();
  PSEnd = SCopy->getPSEnd();
  WStart = SCopy->getWStart();
  WLen = SCopy->getWLen();
  SampleRate = SCopy->getSampleRate();
  Path = SCopy->getPath();
  Data = new STLvectorReal();
  setData(SCopy->getData());
}
void DRCSignal::setParams(DRCSignal *SCopy)
{
  PreWindowLen = SCopy->getPreWindowLen();
  PSStart = SCopy->getPSStart();
  PSEnd = SCopy->getPSEnd();
  WStart = SCopy->getWStart();
  WLen = SCopy->getWLen();
  SampleRate = SCopy->getSampleRate();
  Path = SCopy->getPath();
}
void DRCSignal::setData(STLvectorReal *InData)
{
  if(Data->size() > 0)
    Data->clear();
  for(unsigned int i = 0; i < InData->size(); i++)
    Data->push_back(InData->at(i));
  PSStart = 0;
  PSEnd = InData->size();
}
void DRCSignal::setData(STLvectorReal *InData, int psstart, int psend)
{
  Data->clear();
  for(int i = psstart; i < psend; i++)
    Data->push_back(InData->at(i));
  PSStart = psstart;
  PSEnd = psend;
}
void DRCSignal::clearData(void)
{
  Data->clear();
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

STLvectorReal *DRCSignal::getData(void)
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
    printf("Input signal normalization: %s type.\n",normtype);
    STL_SigNormalize(Data,normfactor,(NormType) normtype[0]);
  }
}

bool DRCSignal::WriteSignal(char* outfile, char* outfiletype) 
{
  char *path_outfile;
  IFileType ftype;
  
  if (outfile !=NULL) {
    if(Path!=NULL) {
      path_outfile = (char*) malloc(sizeof(char)*(1+strlen(Path)+strlen(outfile)));
      strcpy(path_outfile,Path);
      strcat(path_outfile,outfile);
    } else {
      path_outfile = (char*) malloc(sizeof(char)*(1+strlen(outfile)));
      strcpy(path_outfile,outfile);
    }
    switch(outfiletype[0]) {
    case 'I':
      ftype = PcmInt16Bit;
      break;
    case 'F':
      ftype = PcmFloat32Bit;
      break;
    case 'D':
      ftype = PcmFloat64Bit;
      break;
    default:
      sputs("WriteSignal: undefined file data type (I, F or D)");
    return false;
    break; 
    }
    printf("Saving output signal: %s\n",path_outfile);
    if (SND_WriteSignal(path_outfile, Data, WStart, WLen, SampleRate, ftype) == false) {
      sputs("Input signal component save failed");
      return false;
    }
    return true;
  }
  return false;
}

STLvectorReal *STL_ToeplitzSolve(STLvectorReal *Av, STLvectorReal *Bv, unsigned int N)
{
  unsigned int I;
  DLReal *A;
  DLReal *B;
  DLReal *X;
  STLvectorReal *Xv = new STLvectorReal();

  if(Av->size() < N || Bv->size() < N) {
    sputs("Toeplitz vectors size less than N factor");
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
    A[I] = Av->at(I);
    B[I] = Bv->at(I);
  }
  if(ToeplitzSolve(A, B, X, N) == false)
    return Xv;
  for(I = 0; I < N; I++)
    Xv->push_back(X[I]);
  delete A;
  delete B;
  delete X;
  return Xv;
}

STLvectorReal *STL_KirkebyFDInvert(STLvectorReal *InSig, unsigned int InSigLen, 
				   unsigned int InvFilterSigLen, STLvectorReal *EffortSig, 
				   DLReal EffortFactor, int MExp)
{
  unsigned int I;
  DLReal *InS;
  DLReal *EffortS;
  DLReal *InvFilterS;
  STLvectorReal *InvFilterSig = new STLvectorReal();

  InS = new DLReal[InSig->size()];
  if (InS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EffortS = new DLReal[EffortSig->size()];
  if (EffortS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < InSig->size(); I++) 
    InS[I] = InSig->at(I);
  for(I = 0; I < EffortSig->size(); I++) 
    EffortS[I] = EffortSig->at(I);
  if(KirkebyFDInvert(InS, InSig->size(), InvFilterS, InvFilterSigLen, EffortS, EffortSig->size(), EffortFactor,MExp)== true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig->push_back(InvFilterS[I]);
  }
  delete InS;
  delete EffortS;
  delete InvFilterS;
  return InvFilterSig;
}
STLvectorReal *STL_PEISMPKirkebyFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig, 
					unsigned int InvFilterSigLen, DLReal EffortFactor, int MExp)
{
  unsigned int I;
  DLReal *MPS;
  DLReal *EPS;
  DLReal *InvFilterS;
  STLvectorReal *InvFilterSig = new STLvectorReal();
  
  MPS = new DLReal[MPSig->size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig->size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig->size(); I++) 
    MPS[I] = MPSig->at(I);
  for(I = 0; I < EPSig->size(); I++) 
    EPS[I] = EPSig->at(I);
  if(PEISMPKirkebyFDInvert(MPS, MPSig->size(), EPS, EPSig->size(), InvFilterS, InvFilterSigLen, EffortFactor, MExp) == true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig->push_back(InvFilterS[I]);
  }
  delete MPS;
  delete EPS;
  delete InvFilterS;
  return InvFilterSig;
}

STLvectorReal *STL_PEMSMPKirkebyFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig,
					 unsigned int InvFilterSigLen, DLReal EffortFactor, 
					 int PEStart, int PETransition, int MExp)
{
  unsigned int I;
  DLReal *MPS;
  DLReal *EPS;
  DLReal *InvFilterS;
  STLvectorReal *InvFilterSig = new STLvectorReal();
  
  MPS = new DLReal[MPSig->size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig->size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig->size(); I++) 
    MPS[I] = MPSig->at(I);
  for(I = 0; I < EPSig->size(); I++) 
    EPS[I] = EPSig->at(I);
  if(PEMSMPKirkebyFDInvert(MPS, MPSig->size(), EPS, EPSig->size(), InvFilterS, InvFilterSigLen, EffortFactor, PEStart, PETransition, MExp) == true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig->push_back(InvFilterS[I]);
  }
  delete MPS;
  delete EPS;
  delete InvFilterS;
  return InvFilterSig;
}

STLvectorReal *STL_PETFDInvert(STLvectorReal *MPSig, STLvectorReal *EPSig, 
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
  STLvectorReal *InvFilterSig = new STLvectorReal();
  
  MPS = new DLReal[MPSig->size()];
  if (MPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  EPS = new DLReal[EPSig->size()];
  if (EPS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  InvFilterS = new DLReal[InvFilterSigLen];
  if (InvFilterS == NULL) {
    sputs("Memory allocation failed.");
    return InvFilterSig;
  }
  for(I = 0; I < MPSig->size(); I++) 
    MPS[I] = MPSig->at(I);
  for(I = 0; I < EPSig->size(); I++) 
    EPS[I] = EPSig->at(I);
  if(PETFDInvert(MPS, MPSig->size(), EPS, EPSig->size(), InvFilterS,InvFilterSigLen, PEType, 
		 PELowerWindow, PEUpperWindow, PEStartFreq, PEEndFreq, PEFilterLen, FSharpness,
		 PEBandSplit, PEWindowExponent, SLPType, OGainFactor, SampleRate, MExp)== true) {
    for(I = 0; I < InvFilterSigLen; I++)
      InvFilterSig->push_back(InvFilterS[I]);
  }
  delete MPS;
  delete EPS;
  delete InvFilterS;
  return InvFilterSig;
}
