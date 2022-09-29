/****************************************************************************

    DRC: Digital Room Correction
    Copyright (C) 2002, 2003 Denis Sbragion

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

		You can contact the author on Internet at the following address:

				d.sbragion@infotecna.it

		This program uses the parsecfg library from Yuuki  NINOMIYA.  De­
		tails  on  this  library  can be found in the parsecfg.c and par­
		secfg.h files.  Many thanks to Yuuki NINOMIYA for this useful li­
		brary.

****************************************************************************/

/* Funzioni di libreria base */

/* Inclusioni */
#include "baselib.h"
#include "fft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Output stringhe con sync output e parametro */
int sputsp(const char * s, const char * p)
{
  int Res;
  if (p == NULL)
    Res = puts(s);
  else
    Res = printf("%s%s\n",s,p);
  fflush(stdout);
  return(Res);
}

/* Output stringhe con sync output */
int sputs(const char * s)
{
  return(sputsp(s, NULL));
}

/* Determina la lunghezza di un file */
size_t FSize(FILE * F)
{
  long CPos;
  long FS;
  
  CPos = ftell(F);
  fseek(F,0,SEEK_END);
  FS = ftell(F);
  fseek(F,CPos,SEEK_SET);
  return (size_t) FS;
}

/* Legge parte di un file di ingresso e lo pone nell'array indicato
   di dimensione InitWindow.
   Ritorna true se l'operazione ha successo.
*/
bool ReadSignal(const char * FName,DRCFloat * Dst,const int InitWindow,
		const int ImpulseCenter,const IFileType FType,
		int * PreSpikeStart, int * PostSpikeEnd)
{
  int I;
  int IStart;
  int IEnd;
  int WHalf;
  DRCFileDouble RWD;
  DRCFileFloat RWF;
  DRCFileInt RWI;
  int RWDim;
  
  /* File gestione IO */
  FILE * IOF;
  long FS;
  
  /* Azzera l'array destinazione */
  for (I = 0;I < InitWindow;I++)
    Dst[I] = 0;
  
  /* Salva la dimensione del blocco lettura */
  RWDim = sizeof(DRCFileFloat);
  switch (FType) {
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
  
  /* Apre il file di input */
  if ((IOF = fopen(FName,"rb")) == NULL) {
    perror("\nUnable to open input file");
    return false;
  }
  
  /* Recupera la dimensione del file */
  FS = FSize(IOF) / RWDim;
  
  /* Controllo validità parametri */
  if (FS == 0) {
    puts("\nInput file is zero length.");
    return false;
  }
  if (ImpulseCenter > FS) {
    puts("\nImpulseCenter is out of input file range.");
    return false;
  }
  
  /* Calcolo la posizione centrale finestra */
  if (InitWindow % 2 == 0)
    WHalf = (InitWindow - 1) / 2;
  else
    WHalf = InitWindow / 2;
  
  /* Calcola gli estremi lettura finestra */
  if (ImpulseCenter > WHalf) {
    if (fseek(IOF,(ImpulseCenter - WHalf) * RWDim,SEEK_SET) != 0) {
      perror("\nError reading input file");
      return false;
    }
    IStart = 0;
    if (InitWindow > FS - (ImpulseCenter - WHalf))
      IEnd = FS - (ImpulseCenter - WHalf);
    else
      IEnd = InitWindow;
  } else {
    IStart = WHalf - ImpulseCenter;
    if (InitWindow > IStart + FS)
      IEnd = IStart + FS;
    else
      IEnd = InitWindow;
  }
  
  /* Imposta inizio pre spike */
  if (PreSpikeStart != NULL)
    *PreSpikeStart = IStart;
  
  /* Imposta fine post spike */
  if (PostSpikeEnd != NULL)
    *PostSpikeEnd = IEnd;
  
  /* Legge la risposta all'impulso */
  switch (FType) {
  case PcmFloat64Bit:
    for (I = IStart; I < IEnd; I++) {
      if (fread((void *) &RWD,sizeof(DRCFileDouble),1,IOF) != 1) {
	perror("\nError reading input file");
	return false;
      }
      Dst[I] = (DLReal) RWD;
    }
    break;
    
  case PcmFloat32Bit:
    for (I = IStart; I < IEnd; I++) {
      if (fread((void *) &RWF,sizeof(DRCFileFloat),1,IOF) != 1) {
	perror("\nError reading input file");
	return false;
      }
      Dst[I] = (DLReal) RWF;
    }
    break;
    
  case PcmInt16Bit:
    for (I = IStart; I < IEnd; I++) {
      if (fread((void *) &RWI,sizeof(DRCFileInt),1,IOF) != 1) {
	perror("\nError reading input file");
	return false;
      }
      Dst[I] = (DLReal) RWI;
    }
    break;
  }
  
  /* Chiude il file */
  fclose(IOF);
  
  /* Operazione completata */
  return true;
}

bool SND_ReadSignal(const char * FName, DRCFloat * Dst, const int InitWindow, const int ImpulseCenter, 
		    int *PreSpikeStart, int *PostSpikeEnd)
{
  SNDFILE* sf_file;
  SF_INFO sf_info;

  int I; 
  int IStart;
  int IEnd;
  int WHalf;

  sf_info.format = 0;

  if((sf_file = sf_open(FName, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "cannot open sndfile \"%s\" for input (%s)\n",FName, sf_strerror(sf_file));
    sf_close(sf_file);
    return false;
  }

  if (sf_info.frames == 0) {
    puts("\nInput file is zero length.");
    sf_close(sf_file);
    return false;
  }
  if (ImpulseCenter > sf_info.frames) {
    puts("\nImpulseCenter is out of input file range.");
    sf_close(sf_file);
    return false;
  }

  for (I = 0; I < InitWindow; I++)
    Dst[I] = 0;
  
  if (InitWindow % 2 == 0)
    WHalf = (InitWindow - 1) / 2;
  else
    WHalf = InitWindow / 2;
  
  if (ImpulseCenter > WHalf) {
    if (sf_seek(sf_file, ImpulseCenter - WHalf, SEEK_SET) != (ImpulseCenter - WHalf)) {
      perror("\nError reading input file");
      sf_close(sf_file);
      return false;
    }
    IStart = 0;
    if (InitWindow > sf_info.frames - (ImpulseCenter - WHalf))
      IEnd = sf_info.frames - (ImpulseCenter - WHalf);
    else
      IEnd = InitWindow;
  }
  else {
    IStart = WHalf - ImpulseCenter;
    if (InitWindow > IStart + sf_info.frames)
      IEnd = IStart + sf_info.frames;
    else
      IEnd = InitWindow;
  }
  
  /* Imposta inizio pre spike */
  if (PreSpikeStart != NULL)
    *PreSpikeStart = IStart;
  
  /* Imposta fine post spike */
  if (PostSpikeEnd != NULL)
    *PostSpikeEnd = IEnd;

  sf_readf_double(sf_file, (DLReal*)&Dst[IStart], IEnd-IStart);
  sf_close(sf_file);
  return true;
}

/* Scrive il segnale indicato su disco */
bool WriteSignal(const char * FName, const DRCFloat * Src, const int SSize, const IFileType FType)
{
  /* File gestione IO */
  FILE * IOF;
  int I;
  DRCFileDouble RWD;
  DRCFileFloat RWF;
  DRCFileInt RWI;
  
  /* Apre il file di output */
  if ((IOF = fopen(FName,"wb")) == NULL) {
    perror("\nUnable to open ouput file");
    return false;
  }
  
  /* Salva la risposta risultante */
  switch (FType) {
  case PcmFloat64Bit:
    for (I = 0; I < SSize; I++) {
      RWD = (DRCFileDouble) Src[I];
      if (fwrite((void *) &RWD,sizeof(DRCFileDouble),1,IOF) != 1) {
	perror("\nError writing ouput file");
	return false;
      }
    }
    break;
    
  case PcmFloat32Bit:
    for (I = 0; I < SSize; I++) {
      RWF = (DRCFileFloat) Src[I];
      if (fwrite((void *) &RWF,sizeof(DRCFileFloat),1,IOF) != 1) {
	perror("\nError writing ouput file");
	return false;
      }
    }
    break;
    
  case PcmInt16Bit:
    for (I = 0; I < SSize; I++) {
      RWI = (DRCFileInt) floor(0.5 + Src[I]);
      if (fwrite((void *) &RWI,sizeof(DRCFileInt),1,IOF) != 1) {
	perror("\nError writing ouput file");
	return false;
      }
    }
    break;
  }
  
  /* Chiude il file */
  fclose(IOF);
  
  /* Operazione completata */
  return true;
}

/* Sovrascrive il segnale indicato su disco */
bool OverwriteSignal(const char * FName, const DRCFloat * Src, const int SSize, const int Skip, const IFileType FType)
{
  /* File gestione IO */
  FILE * IOF;
  int I;
  DRCFileDouble RWD;
  DRCFileFloat RWF;
  DRCFileInt RWI;
  
  /* Apre il file di output */
  IOF = fopen(FName,"r+b");
  
  /* Salva la risposta risultante */
  switch (FType) {
  case PcmFloat64Bit:
    fseek(IOF,Skip * sizeof(DRCFileDouble), SEEK_SET);
    for (I = 0; I < SSize; I++) {
      RWD = (DRCFileDouble) Src[I];
      if (fwrite((void *) &RWD,sizeof(DRCFileDouble),1,IOF) != 1) {
	perror("\nError writing ouput file.");
	return false;
      }
    }
    break;
    
  case PcmFloat32Bit:
    fseek(IOF,Skip * sizeof(DRCFileFloat), SEEK_SET);
    for (I = 0; I < SSize; I++) {
	RWF = (DRCFileFloat) Src[I];
	if (fwrite((void *) &RWF,sizeof(DRCFileFloat),1,IOF) != 1) {
	  perror("\nError writing ouput file.");
	  return false;
	}
    }
    break;
    
  case PcmInt16Bit:
    fseek(IOF,Skip * sizeof(DRCFileInt), SEEK_SET);
    for (I = 0; I < SSize; I++) {
      RWI = (DRCFileInt) floor(0.5 + Src[I]);
      if (fwrite((void *) &RWI,sizeof(DRCFileInt),1,IOF) != 1) {
	perror("\nError writing ouput file.");
	return false;
      }
    }
    break;
  }
  
  /* Chiude il file */
  fclose(IOF);
  
  /* Operazione completata */
  return true;
}

/* Calcola l'autocorrelazione del segnale S */
bool AutoCorrelation(DLReal * S, int N)
{
  DLComplex *C;
  int I;
  
  if ((C = new DLComplex[N]) == NULL)
    return false;
  
  /*for (I = 0; I < N; I++)
    C[I] = S[I];*/
  
  Fftw(S, C, N);
  
  for (I = 0; I < N; I++)
    /* C[I] = C[I] * std::conj(C[I]); */
    C[I] = std::real(C[I]) * std::real(C[I]) + std::imag(C[I]) * std::imag(C[I]);

  IFftw(C, S, N);

  for (I = 0; I < N; I++)
    S[I] /= N;

  delete C;
  
  return true;
}

/* Calcola la cross correlazione tra S1 e S2 */
/* XC deve avere lunghezza 2N - 1 */
bool CrossCorrelation(DLReal * S1, DLReal * S2, int N, DLReal * XC)
{
  DLReal *In1;
  DLReal *In2;
  DLComplex *C1;
  DLComplex *C2;
  int I;
  
  if ((In1 = new DLReal[2 * N]) == NULL)
    return false;
  if ((In2 = new DLReal[2 * N]) == NULL) {
    delete In1;
    return false;
  }
  if ((C1 = new DLComplex[2 * N]) == NULL) {
    delete In1;
    delete In2;
    return false;
  }
  if ((C2 = new DLComplex[2 * N]) == NULL) {
    delete In1;
    delete In2;
    delete C1;
    return false;
  }
  
  for (I = 0; I < N; I++) {
    In1[I] = S1[I];
    In2[I] = S2[I];
  }
  for (I = N; I < 2 * N; I++) {
    In1[I] = 0;
    In2[I] = 0;
  }

  Fftw(In1, C1, 2 * N);
  Fftw(In2, C2, 2 * N);
  
  for (I = 0; I < 2 * N; I++)
    C1[I] *= std::conj<DLReal>(C2[I]);

  delete C2;

  IFftw(C1, In1, 2 * N);

  for (I = 0; I < 2 * N - 1; I++)
    XC[I] = In1[I] / N;
  
  delete C1;
  
  return true;
}

/* Calcola il ritardo di gruppo del segnale S */
bool GroupDelay(const DLReal * S, const int N, DLReal * GD)
{
  DLReal *InH;
  DLReal *InDH;
  DLComplex *HA;
  DLComplex *DHA;
  int I;

  if ((InH = new DLReal[N]) == NULL)
    return false;
  if ((InDH = new DLReal[N]) == NULL) {
    delete InH;
    return false;
  }
  if ((HA = new DLComplex[N]) == NULL) {
    delete InH;
    delete InDH;
    return false;
  }
  if ((DHA = new DLComplex[N]) == NULL) {
    delete InH;
    delete InDH;
    delete HA;
    return false;
  }

  for(I = 0;I < N;I++) {
    InH[I] = S[I];
    InDH[I] = S[I] * I;
  }
  
  Fftw(InH, HA, N);
  Fftw(InDH, DHA, N);
  
  for(I = 0;I < N;I++)
    GD[I] = std::real(DHA[I] / HA[I]);
  
  delete InH;
  delete InDH;
  delete HA;
  delete DHA;
  
  return true;
}

/* I/O Delay computation, reliable only for simple impulse responses */
DLReal LinearDelay(DLReal * Hn, unsigned int N, unsigned int Np, DLReal MZE)
{
  unsigned int I;
  DLReal *InH;
  DLReal *InH1;
  DLComplex * H;
  DLComplex * H1;
  DLComplex Sum,i(0,1);

  if (Np == 0)
    for(Np = 2;Np < N;Np <<= 1);
  
  Np = 2*Np;

  if ((InH = new DLReal[Np]) == NULL)
    return(-1);
  if ((InH1 = new DLReal[Np]) == NULL) {
    delete InH;
    return(-1);
  }
  if ((H = new DLComplex[Np]) == NULL) {
    delete InH;
    delete InH1;
    return(-1);
  }
  if ((H1 = new DLComplex[Np]) == NULL) {
    delete InH;
    delete InH1;
    delete H;
    return(-1);
  }

  for(I = 0;I < N;I++)
    InH[I] = Hn[I];
  
  for(I = N;I < Np;I++)
    InH[I] = 0;
  
  if (Fftw(InH, H, Np) == false) {
    delete InH;
    delete H;
    delete H1 ;
    return(-1);
  }

  for(I = 0;I < N;I++)
    InH1[I] = I*Hn[I];

  for(I = N;I < Np;I++)
    InH1[I] = 0;

  if (Fftw(InH1, H1, Np) == false) {
    delete InH;
    delete InH1;
    delete H;
    delete H1;
    return(-1);
  }

  for(I = 0;I < Np;I++)
    if (std::abs<DLReal>(H[I]) <= MZE) {
      H[I] = 1;
      H1[I] = 0;
    }

  Sum = 0;
  for(I = 0;I < Np;I++)
    Sum += (-i*H1[I])/H[I];

  delete InH;
  delete InH1;
  delete H;
  delete H1;
  return(std::real<DLReal>(-Sum/(i*(DLReal) Np)));
}


/* Conta il numero di righe in un file */
int FLineCount(const char * FName)
{
  /* File gestione IO */
  FILE * IOF;
  
  /* Variabili ausiliarie */
  int LC;
  char InStr[256];
  
  /* Apre il file di input */
  if ((IOF = fopen(FName,"rt")) == NULL) {
    perror("\nUnable to open input file.");
    return false;
  }
  
  /* Conta le righe */
  LC = 0;
  while (fgets(InStr,256,IOF) != NULL)
    LC++;
  
  /* Chiude il file di input */
  fclose(IOF);
  
  /* Ritorna il numero di righe */
  return LC;
}

/* Legge i punti di generazione filtro FIR dal file indicato */
bool ReadPoints(char * CorrFile,const TFMagType MagType, DLReal * FilterFreqs,DLReal * FilterM,DLReal * FilterP,const int NPoints, int SampleRate)
{
  /* File gestione IO */
  FILE * IOF;
  
  /* Variabili ausiliarie */
  int I;
  double Freq;
  double Mag;
  double Phase;
  char *pos1, *pos2;
  char InStr[256];
  
  /* Apre il file di correzione */
  if ((IOF = fopen(CorrFile,"rt")) == NULL) {
    perror("\nUnable to open correction file.");
    return false;
  }

  /* Legge i valori */
  for (I = 0; I < NPoints; I++) {
    if (fgets(InStr,256,IOF) == NULL) {
      perror("\nError reading correction file");
      return false;
    }
    
    Phase = 0;
    Freq = strtod(InStr, &pos1);
    Mag = strtod(pos1, &pos2);
    if (pos2 != NULL)
      Phase = strtod(pos2, NULL);
    /*if (sscanf(InStr,"%f %f %f",&Freq,&Mag,&Phase) < 2) {
      printf("Not enough parameters on line %d.",I);
      sputs("Error reading correction file.");
      return false;
      }*/
    
    /* Verifica il tipo di ampiezza */
    if (MagType == MAGdB)
      /* Ricava l'ampiezza assoluta dai dB */
      Mag = (DLReal) pow(10.0,Mag / 20.0);
    
    /* Imposta i punti per il filtro */
    FilterFreqs[I] = (DLReal) (2 * Freq) / SampleRate;
    FilterM[I] = (DLReal) Mag;
    FilterP[I] = (DLReal) ((Phase * M_PI) / 180);
  }
  FilterFreqs[NPoints - 1] = (DLReal) 1.0;
  
  /* Chiude il file di input */
  fclose(IOF);
  
  /* Operazione completata */
  return true;
}

/* Integra due funzioni di trsferimento definite per punti, usando
una interpolazione lineare, ritorna il numero di punti generati,
che non sono mai più di NPoints1 + NPoints2 */
int LITFMerge(DLReal * FilterFreqs1,DLComplex * FilterPoints1,const int NPoints1, 
	      DLReal * FilterFreqs2,DLComplex * FilterPoints2,const int NPoints2,
	      DLReal * FilterFreqsOut,DLComplex * FilterPointsOut)
{
  /* Indice sull'array di output */
  int OI;
  
  /* Indice sugli array di ingresso */
  int I1;
  int I2;
  
  /* Calcolo interpolazione */
  DLReal DMag;
  DLReal DArg;
  DLReal DFreq;
  
  /* Inizializza gli indici ciclo interpolazione */
  OI = 0;
  I1 = 0;
  I2 = 0;
  
  /* Ciclo interpolazione */
  while (I1 < NPoints1 || I2 < NPoints2) {
    if (FilterFreqs1[I1] == FilterFreqs2[I2]) {
      /* Calcola il punto di uscita */
      FilterFreqsOut[OI] = FilterFreqs1[I1];
      FilterPointsOut[OI] = FilterPoints1[I1] * FilterPoints2[I2];
      
      /* Avanza gli indici elaborati */
      I1++;
      I2++;
    } else
      if (FilterFreqs1[I1] > FilterFreqs2[I2]) {
	/* Calcola la frequeunza del punto di uscita */
	FilterFreqsOut[OI] = FilterFreqs2[I2];
	
	/* Calcola le variazioni per l'interpolazione */
	DMag = std::abs(FilterPoints1[I1]) - std::abs(FilterPoints1[I1 - 1]);
	DArg = std::arg(FilterPoints1[I1]) - std::arg(FilterPoints1[I1 - 1]);
	DFreq = FilterFreqs1[I1] - FilterFreqs1[I1 - 1];
	
	/* Calcola il punto di uscita */
	FilterPointsOut[OI] = FilterPoints2[I2] * std::polar(std::abs(FilterPoints1[I1 - 1]) + (FilterFreqsOut[OI] - FilterFreqs1[I1 - 1]) * DMag / DFreq,
							     std::arg(FilterPoints1[I1 - 1]) + (FilterFreqsOut[OI] - FilterFreqs1[I1 - 1]) * DArg / DFreq);

	/* Avanza l'indice elaborato */
	I2++;
      } else {
	/* Calcola la frequeunza del punto di uscita */
	FilterFreqsOut[OI] = FilterFreqs1[I1];
	
	/* Calcola le variazioni per l'interpolazione */
	DMag = std::abs(FilterPoints2[I2]) -
	  std::abs(FilterPoints2[I2 - 1]);
	DArg = std::arg(FilterPoints2[I2]) -
	  std::arg(FilterPoints2[I2 - 1]);
	DFreq = FilterFreqs2[I2] -
	  FilterFreqs2[I2 - 1];
	
	/* Calcola il punto di uscita */
	FilterPointsOut[OI] = FilterPoints1[I1] * std::polar(std::abs(FilterPoints2[I2 - 1]) + (FilterFreqsOut[OI] - FilterFreqs2[I2 - 1]) * DMag / DFreq,
							     std::arg(FilterPoints2[I2 - 1]) + (FilterFreqsOut[OI] - FilterFreqs2[I2 - 1]) * DArg / DFreq);
	
	/* Avanza l'indice elaborato */
	I1++;
      }
    
    /* Avanza l'indice di uscita */
    OI++;
  }
  
  /* Ritorna il numero punti generati */
  return OI;
}

/* Trova il valore massimo all'interno di un file. Versione float 64 bit. */
static int FindMaxPcmFloat64Bit(const char * FName)
{
  DRCFileDouble RWD;
  int MaxPos;
  int Pos;
  DRCFileDouble PcmMax;
  
  /* File gestione IO */
  FILE * IOF;
  
  /* Apre il file di input */
  if ((IOF = fopen(FName,"rb")) == NULL) {
    perror("\nUnable to open input file.");
    return -1;
  }
  
  /* Ciclo ricerca massimo */
  Pos = 0;
  MaxPos = 0;
  PcmMax = 0;
  while(feof(IOF) == 0) {
    if (fread((void *) &RWD,sizeof(DRCFileDouble),1,IOF) != 1) {
      if (feof(IOF) == 0) {
	perror("\nError reading input file.");
	return -1;
      }
    }
    
    if (((DRCFileFloat) fabs(RWD)) > PcmMax) {
      PcmMax = (DRCFileFloat) fabs(RWD);
      MaxPos = Pos;
    }
    
    Pos++;
  }
  
  /* Chiude il file di input */
  fclose(IOF);
  
  /* Operazione completata */
  return MaxPos;
}

/* Trova il valore massimo all'interno di un file. Versione float 32 bit. */
static int FindMaxPcmFloat32Bit(const char * FName)
{
  DRCFileFloat RWF;
  int MaxPos;
  int Pos;
  DRCFileFloat PcmMax;
  
  /* File gestione IO */
  FILE * IOF;
  
  /* Apre il file di input */
  if ((IOF = fopen(FName,"rb")) == NULL) {
    perror("\nUnable to open input file.");
    return -1;
  }
  
  /* Ciclo ricerca massimo */
  Pos = 0;
  MaxPos = 0;
  PcmMax = 0;
  while(feof(IOF) == 0) {
    if (fread((void *) &RWF,sizeof(DRCFileFloat),1,IOF) != 1) {
      if (feof(IOF) == 0) {
	perror("\nError reading input file.");
	return -1;
      }
    }
    
    if (((DRCFileFloat) fabs(RWF)) > PcmMax) {
      PcmMax = (DRCFileFloat) fabs(RWF);
      MaxPos = Pos;
    }
    
    Pos++;
  }
  
  /* Chiude il file di input */
  fclose(IOF);
  
  /* Operazione completata */
  return MaxPos;
}

/* Trova il valore massimo all'interno di un file. */
int FindMaxPcm(const char * FName,const IFileType FType)
{
  /* Controlla il tipo file */
  switch (FType) {
  case PcmFloat64Bit:
    return FindMaxPcmFloat64Bit(FName);
    break;

  case PcmFloat32Bit:
    return FindMaxPcmFloat32Bit(FName);
    break;
    
  default:
    break;
  }
  
  /* Tipo file errato */
  return -1;
}

