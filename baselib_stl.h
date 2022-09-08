#ifndef BaseLib_stl_h
#define BaseLib_stl_h
	
/* Inclusioni */
#include "baselib.h"
#include "convol.h"
#include "stl_type.h"
#include <stdio.h>
#include <string.h>
#include <sndfile.h>

/* File length calculation */
//size_t FSize(FILE * F);

/* Signal reading from a raw file. Data length InitWindow, centered at ImpulseCenter */
STLvectorReal *STL_ReadSignal(const char * FName,const int InitWindow, const int ImpulseCenter,
			 const IFileType FType,int &PreSpikeStart, int &PostSpikeEnd);

STLvectorReal *STL_SND_ReadSignal(const char * FName, const int InitWindow, const int ImpulseCenter, int &PreSpikeStart, int &PostSpikeEnd);

/* Scrive il segnale indicato su disco */
bool STL_WriteSignal(const char * FName,STLvectorReal *Src, const unsigned int WStart, const unsigned int WLen, const IFileType FType);

bool SND_WriteSignal(const char * FName, STLvectorReal *Src,const unsigned int WStart, const unsigned int WLen, const int SampleRate, const IFileType FType);

/* Sovrascrive il segnale indicato su disco */
bool STL_OverwriteSignal(const char * FName, STLvectorReal *Src,const int Skip, const IFileType FType);

/* Calcola l'autocorrelazione del sgnale S */
STLvectorReal *STL_AutoCorrelation(STLvectorReal *S);

/* Calcola la cross correlazione tra S1 e S2 */
/* XC deve avere lunghezza 2N - 1 */
STLvectorReal *STL_CrossCorrelation(STLvectorReal *S1, STLvectorReal *S2);

/* Calcola il ritardo di gruppo del segnale S */
STLvectorReal *STL_GroupDelay(STLvectorReal *S);

/* I/O Delay computation, reliable only for simple impulse responses */
/*DLReal LinearDelay(DLReal * Hn,unsigned int N,unsigned int Np,DLReal MZE);*/

/* Conta il numero di righe in un file */
int FLineCount(const char * FName);

bool STL_ReadPoints(char* CorrFile, const TFMagType MagType, STLvectorReal *FilterFreq,
		    STLvectorReal *FilterM, STLvectorReal *FilterP, const int NPoints, int SampleRate);

unsigned int SND_FSize(const char * FName);

int SND_GetSampleRate(const char * FName);

int STL_FindMaxPcm(const char * FName,const IFileType FType);

int SND_FindMaxPcm(const char * FName);

bool GlSweep(DLReal Rate, DLReal Amplitude, DLReal HzStart, DLReal HzEnd, DLReal Duration, DLReal Silence, 
	     DLReal LeadIn, DLReal LeadOut, char *SweepFile, char *InverseFile);

bool LsConv(char *SweepFile, char *InverseFile, char *OutFile);
#endif
