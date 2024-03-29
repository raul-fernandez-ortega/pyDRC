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

		This program uses the parsecfg library from Yuuki  NINOMIYA.  De�
		tails  on  this  library  can be found in the parsecfg.c and par�
		secfg.h files.  Many thanks to Yuuki NINOMIYA for this useful li�
		brary.

****************************************************************************/

/* Funzioni correzione e gestione livello del segnale */

/* Inclusioni */
#include "level.h"
#include "baselib.h"
#include "fft.h"
#include "dspwind.h"
#include <math.h>

/* Calola il valore RMS del segnale Sig */
DLReal GetRMSLevel(const DLReal * Sig,const int SigLen)
{
  DLReal RMS;
  int I;
  
  /* Variabili di supporto per il Kahan summation algorithm */
  DLReal KC;
  DLReal KY;
  DLReal KT;
  
  /* Calcolo RMS */
  RMS = (DLReal) 0.0;
  KC = (DLReal) 0.0;
  for (I = 0; I < SigLen; I++) {
    KY = (Sig[I] * Sig[I]) - KC;
    KT = RMS + KY;
    KC = (KT - RMS) - KY;
    RMS = KT;
  }
  /* Ritorna il valore normalizzato */
  return (DLReal) sqrt(RMS);
}

/* Calola il valore RMS del segnale Sig sulla banda di frequenze indicate */
DLReal GetBLRMSLevel(const DLReal * Sig,const int SigLen,const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W,const int MExp)
{
  DLReal RMS;
  int I;
  DLReal *InArray;
  DLComplex *FFTArray;
  int FFTSize;
  
  /* Determina la prima potenza di 2 >= SigLen */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for(FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;
  
  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL) {
    delete[] InArray;
    return false;
  }
    /* Copia il segnale nell'array */
    for (I = 0;I < SigLen;I++)
      InArray[I] = Sig[I];
    for (I = SigLen;I < FFTSize;I++)
      InArray[I] = (DLReal) 0.0;

    /* Effettua l'fft del segnale */
    Fftw(InArray, FFTArray, FFTSize);

    /* Calcola il valore RMS sulla banda interessata */
    RMS = GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq, W);
    
    /* Dealloca l'array FFT */
    delete[] FFTArray;
    delete[] InArray;

    /* Ritorna il valore RMS */
    return RMS;
}

/* Euler Mascheroni constant */
#define RMSEMC (0.57721566490153286060651209008240243104215933593992)

/* Limite calcolo diretto */
#define RMSHNL (256)

/* Calola il valore RMS del segnale trasformato FFTArray sulla banda di frequenze
   indicate con pesatura in frequenza pari a 1/(f^w). Per W = 0 calcola il normale
   valore RMS, per W = 1 assegna peso uguale in potenza per ottava/decade. */
DLReal GetBLFFTRMSLevel(const DLComplex * FFTArray,const int FFTSize,const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W)
{
  DLReal RMS;
  int I;
  int FS;
  int FE;
  int HS;
  
  /* Fattore di compensazione per pesatura */
  DLReal CF;
  
  /* Variabili di supporto per il Kahan summation algorithm
     nel calcolo del fattore di compensazione */
  DLReal KC;
  DLReal KY;
  DLReal KT;

  /* Determina gli indici per il calcolo del valore RMS */
  FS = (int) floor(0.5 + ((FFTSize * StartFreq) / SampleFreq));
  FE = (int) floor(0.5 + ((FFTSize * EndFreq) / SampleFreq));
  HS = FFTSize / 2;

  /* Calcola il livello RMS */
  if (W == ((DLReal) 0.0)) {
    /* Valore iniziale */
    if (FS > 0)
      RMS = (DLReal) 0.0;
    else {
      RMS = std::real(FFTArray[0]) * std::real(FFTArray[0]);
      FS = 1;
    }
    
    /* Calcolo RMS */
    KC = (DLReal) 0.0;
    for (I = FS;I < FE;I++) {
      KY = std::norm(FFTArray[I]) - KC;
      KT = RMS + KY;
      KC = (KT - RMS) - KY;
      RMS = KT;
    }
    
    return (DLReal) sqrt(2.0 * RMS / FFTSize);
  } else {
    /* Valore iniziale */
    if (FS > 0) {
      /* Nessuna componente DC */
      CF = (DLReal) 0.0;
      RMS = (DLReal) 0.0;
    } else {
      /* Assume che la componente DC abbia lo stesso peso della prima
	 componente utile */
      CF = (DLReal) 1.0;
      RMS = CF * std::real(FFTArray[0]) * std::real(FFTArray[0]);
      FS = 1;
    }
    
    /* Calcolo RMS */
    KC = ((DLReal) 0.0);
    for (I = FS;I < FE;I++) {
      KY = (std::norm(FFTArray[I]) * ((DLReal) pow(I,-W))) - KC;
      KT = RMS + KY;
      KC = (KT - RMS) - KY;
      RMS = KT;
    }

    /* Calcola il fattore di normalizzazione, pari al numero armonico */
    if (HS < RMSHNL) {
      /* Error > 1e-16, harmonic number direct computation */
      KC = ((DLReal) 0.0);
      for (I = HS; I > 0; I--) {
	KY = ((DLReal) pow(I,-W)) - KC;
	KT = CF + KY;
	KC = (KT - CF) - KY;
	CF = KT;
      }
    } else {
	/* Error < 1e-16, harmonic number approximation */
	CF += (DLReal) (RMSEMC + log(HS) + 1.0 / (2 * HS) - 1.0 / (12.0 * HS * HS) + 1.0 / (120.0 * pow(HS,4.0)));
      }
    return (DLReal) sqrt(RMS / CF);
  }
}

/* Effettua la normalizzazione del segnale al valore indicato e
secondo il metodo indicato */
bool SigNormalize(DLReal * Sig,const int SigLen,const DLReal NormFactor, const NormType TNorm)
{
  DLReal NormBase;
  int I;
  DLComplex * PFA;
  DLReal * IFA;
  DLReal CAV;
  
  switch (TNorm) {
  case NormEuclidean:
    {
      NormBase = GetRMSLevel(Sig,SigLen);
      
      if (NormBase <= (DLReal) 0.0)
	return false;
      
      for (I = 0; I < SigLen; I++)
	Sig[I] = (Sig[I] * NormFactor) / NormBase;
    }
    break;
    
  case NormMax:
    {
      NormBase = (DLReal) 0.0;
      for (I = 0; I < SigLen; I++)
	if (((DLReal) fabs(Sig[I])) > NormBase)
	  NormBase = (DLReal) fabs(Sig[I]);
      if (NormBase <= (DLReal) 0.0)
	return false;
      for (I = 0; I < SigLen; I++)
	Sig[I] = (Sig[I] * NormFactor) / NormBase;
    }
    break;
    
  case NormSum:
    {
      NormBase = (DLReal) 0.0;
      for (I = 0; I < SigLen; I++)
	NormBase += (DLReal) fabs(Sig[I]);
      
      if (NormBase <= (DLReal) 0.0)
	return false;
      
      for (I = 0; I < SigLen; I++)
	Sig[I] = (Sig[I] * NormFactor) / NormBase;
    }
    break;
    
  case NormFFTPeak:
    {
      if ((IFA = new DLReal[SigLen]) == NULL)
	return false;
      if ((PFA = new DLComplex[SigLen]) == NULL)
	return false;
      for (I = 0; I < SigLen; I++)
	IFA[I] = Sig[I];
      Fftw(IFA, PFA, SigLen);
      NormBase = (DLReal) 0.0;
      for (I = 0; I < SigLen / 2; I++) {
	CAV = std::abs<DLReal>(PFA[I]);
	if (CAV > NormBase)
	  NormBase = CAV;
      }
      if (NormBase <= (DLReal) 0.0) {
	delete[] PFA;
	return false;
      }
      for (I = 0; I < SigLen; I++)
	Sig[I] = (Sig[I] * NormFactor) / NormBase;
      delete[] PFA;
    }
    break;
  }
  
  /* Operazione completata */
  return true;
}

/* Limitazione valli a fase lineare con calcolo del valore RMS sull banda indicata */
bool LPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  int FFTSize;
  int I;
  int J;
  DLReal *InArray;
  DLComplex *FFTArray;
  DLReal RMSLevel;
  DLReal DLSLevel;
  DLReal DLLevel;
  DLReal DLGFactor;
  DLReal DLMFactor;
  DLReal DLAbs;
  DLReal DLMin;
  
  /* Determina la prima potenza di 2 >= SigLen */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for (FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;

  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL) {
    delete[] InArray;
    return false;
  }
  
  /* Copia il segnale nell'array */
  for (I = 0;I < SigLen;I++)
    InArray[I] = Sig[I];
  for (I = SigLen;I < FFTSize;I++)
    InArray[I] = (DLReal) 0.0;

  /* Effettua l'fft del segnale */
  Fftw(InArray, FFTArray, FFTSize);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MinGain * GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq,W);
  /* Calcola il livello minimo tenendo conto delle limitazioni di banda */
  
  RMSLevel = (DLReal) (RMSLevel * MinGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  /* Verifica il tipo di limitazione impostata */
  if (DLStart >= (DLReal) 1.0) {
    /* Scansione per troncatura guadagno */
    for (I = 0; I < FFTSize; I++) {
      DLAbs = std::abs(FFTArray[I]);
      if (DLAbs < RMSLevel)
	FFTArray[I] = std::polar<DLReal>(RMSLevel,std::arg<DLReal>(FFTArray[I]));
    }
  } else {
    /* Determina i fattori per la limitazione guadagno */
    DLSLevel = RMSLevel / DLStart;
    DLGFactor = DLSLevel - RMSLevel;
    DLMin = (DLReal) -1.0;
    
    /* Scansione per limitazione guadagno */
    for (I = 0; I < FFTSize; I++) {
      DLAbs = std::abs(FFTArray[I]);
      if (DLAbs < DLSLevel) {
	/* Verifica se � gi� disponibile il minimo locale */
	if (DLMin < (DLReal) 0.0) {
	  /* Cerca il minimo locale */
	  DLMin = DLAbs;
	  DLLevel = DLAbs;
	  for (J = I + 1; J < FFTSize && DLLevel < DLSLevel; J++) {
	    if (DLLevel < DLMin)
	      DLMin = DLLevel;
	    DLLevel = std::abs(FFTArray[J]);
	  }
	  
	  /* Verifica se il minimo locale � inferiore al livello minimo e ricalcola i fattori di compressione */
	  if (DLMin < RMSLevel)
	    DLMFactor = DLSLevel - DLMin;
	  else
	    DLMFactor = DLSLevel - RMSLevel;
	}
	
	/* Riassegna il guadagno del filtro */
	DLLevel = (DLSLevel - DLAbs) / DLMFactor;
	FFTArray[I] = std::polar<DLReal>(DLSLevel - DLGFactor * DLLevel, std::arg<DLReal>(FFTArray[I]));
      } else
	DLMin = (DLReal) -1.0;
    }
  }
  
  /* Effettua l'fft inversa del segnale */
  IFftw(FFTArray, InArray, FFTSize);
  
  /* Ritorna il segnale limitato */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray[I];
  
  /* Dealloca l'array */
  delete[] FFTArray;
  delete[] InArray;

  /* Operazione completata */
  return true;
}

/* Limitazione valli a fase lineare con calcolo del valore RMS sulla banda indicata */
/* Versione con mantenimento continuit� della derivata prima nei punti di limitazione */
bool C1LPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		  const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  int FFTSize;
  int I;
  DLReal *InArray;
  DLComplex *FFTArray;
  DLReal RMSLevel;
  DLReal DLSLevel;
  DLReal DLLevel;
  DLReal DLGFactor;
  DLReal DLAbs;
  bool tick;
  
  /* Determina la prima potenza di 2 >= SigLen */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for (FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;
  
  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL) {
    delete[] InArray;
    return false;
  }

  /* Copia il segnale nell'array */
  for (I = 0;I < SigLen;I++)
    InArray[I] = Sig[I];
  for (I = SigLen;I < FFTSize;I++)
    InArray[I] = (DLReal) 0.0;

  /* Effettua l'fft del segnale */
  Fftw(InArray, FFTArray, FFTSize);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MinGain * GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq,W);
  
  /* Calcola il livello minimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MinGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  /* Verifica il tipo di limitazione impostata */
  if (DLStart >= (DLReal) 1.0) {
    tick = true;
    /* Scansione per troncatura guadagno */
    for (I = 0; I < FFTSize; I++) {
      DLAbs = std::abs(FFTArray[I]);
      if (DLAbs < RMSLevel) {
	if(tick && I < FFTSize/2) {
	  printf("Dip limit found at %.2f Hz\n",(float)I*SampleFreq/FFTSize);
	  tick = false;
	}
	FFTArray[I] = std::polar<DLReal>(RMSLevel,std::arg<DLReal>(FFTArray[I]));
      } else if(!tick && I < FFTSize/2) {
	printf("Dip limit finished at %.2f Hz\n",(float)I*SampleFreq/FFTSize);
	tick = true;
      }
    }
  } else {
    /* Determina i fattori per la limitazione guadagno */
    DLSLevel = RMSLevel / DLStart;
    DLGFactor = DLSLevel - RMSLevel;
    
    /* Scansione per limitazione guadagno */
    tick = true;
    for (I = 0; I < FFTSize; I++) {
      DLAbs = std::abs(FFTArray[I]);
      if (DLAbs < DLSLevel) {
	if(tick && I < FFTSize/2) {
	  printf("Dip limit found at %.2f Hz\n",(float)I*SampleFreq/FFTSize);
	  tick = false;
	}
	/* Riassegna il guadagno del filtro */
	DLLevel = (DLSLevel - DLAbs) / DLGFactor;
	DLLevel = DLLevel / (((DLReal) 1.0) + DLLevel);
	FFTArray[I] = std::polar<DLReal>(DLSLevel - DLGFactor * DLLevel, std::arg<DLReal>(FFTArray[I]));
      }  else if(!tick && I < FFTSize/2) {
	printf("Dip limit finished at %.2f Hz\n",(float)I*SampleFreq/FFTSize);
	tick = true;
      }
    }
  }
  
  /* Effettua l'fft inversa del segnale */
  IFftw(FFTArray, InArray, FFTSize);

  /* Ritorna il segnale limitato */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray[I];

  /* Dealloca l'array */
  delete[] FFTArray;
  delete[] InArray;

  /* Operazione completata */
  return true;
}

/* Limitazione valli a fase minima con calcolo del valore RMS sulla banda indicata */
/* Versione basata sulla trasformata di Hilbert */
bool HMPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		 const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  DLReal *FFTArray3;
  int FS;
  int I;
  int J;
  DLReal RMSLevel;
  DLReal DLSLevel;
  DLReal DLLevel;
  DLReal DLGFactor;
  DLReal DLMFactor;
  DLReal DLAbs;
  DLReal DLMin;
  bool LogLimit;
  
  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  }else
    FS = SigLen;

  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;
  
  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MinGain * GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq);
  
  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq,W);
  
  /* Calcola il livello minimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MinGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  
  /* Verifica il tipo di limitazione impostata */
  if (DLStart >= (DLReal) 1.0) {
    /* Scansione per troncatura guadagno */
    LogLimit = false;
    for (I = 0;I < FS;I++) {
      DLAbs = std::abs<DLReal>(FFTArray1[I]);
      if (DLAbs <= 0) {
	LogLimit = true;
	FFTArray3[I] = DRCMaxFloat;
	FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
      } else {
	if (DLAbs < RMSLevel)
	  FFTArray3[I] = (DLReal) RMSLevel / DLAbs;
	else
	  FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  } else {
    /* Determina i fattori per la limitazione guadagno */
    DLSLevel = RMSLevel / DLStart;
    DLGFactor = DLSLevel - RMSLevel;
    DLMin = (DLReal) -1.0;
    
    /* Scansione per limitazione guadagno */
    LogLimit = false;
    for (I = 0; I < FS; I++) {
      DLAbs = std::abs<DLReal>(FFTArray1[I]);
      if (DLAbs < DLSLevel) {
	/* Verifica se � gi� disponibile il minimo locale */
	if (DLMin < (DLReal) 0.0) {
	  /* Cerca il minimo locale */
	  DLMin = DLAbs;
	  DLLevel = DLAbs;
	  for (J = I + 1; J < FS && DLLevel < DLSLevel; J++) {
	    if (DLLevel < DLMin)
	      DLMin = DLLevel;
	    DLLevel = std::abs<DLReal>(FFTArray1[J]);
	  }
	  
	  /* Verifica se il minimo locale � inferiore al livello minimo e ricalcola i fattori di compressione */
	  if (DLMin < RMSLevel)
	    DLMFactor = DLSLevel - DLMin;
	  else
	    DLMFactor = DLSLevel - RMSLevel;
	}
	
	/* Riassegna il guadagno del filtro */
	DLLevel = (DLSLevel - DLAbs) / DLMFactor;
	DLLevel = DLSLevel - DLGFactor * DLLevel;
	if (DLAbs <= 0) {
	  LogLimit = true;
	  FFTArray3[I] = DRCMaxFloat;
	  FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
	} else {
	  FFTArray3[I] = (DLReal) DLLevel / DLAbs;
	  FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
	}
      } else {
	DLMin = (DLReal) -1.0;
	FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = (DLReal) 0.0;
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  }
  
  /* Calcola la fase per il filtro a fase minima */
  IFftw(FFTArray2, InArray2, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray2[I] = -InArray2[I];
  InArray2[0] = (DLReal) 0.0;
  InArray2[FS / 2] = (DLReal) 0.0;
  Fftw(InArray2, FFTArray2, FS);
  
  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);
  
  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);

  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];

  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] FFTArray3;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}

/* Limitazione valli a fase minima con calcolo del valore RMS sulla banda indicata */
/* Versione basata sulla trasformata di Hilbert */
/* Versione con mantenimento continuit� della derivata prima nei punti di limitazione */
bool C1HMPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		   const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex * FFTArray1;
  DLComplex * FFTArray2;
  DLReal * FFTArray3;
  int FS;
  int I;
  DLReal RMSLevel;
  DLReal DLSLevel;
  DLReal DLLevel;
  DLReal DLGFactor;
  DLReal DLAbs;
  bool LogLimit;
  bool tick;
  
  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = SigLen;

  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;
  
  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MinGain * GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq,W);

  /* Calcola il livello minimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MinGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  /* Verifica il tipo di limitazione impostata */
  if (DLStart >= (DLReal) 1.0) {
    /* Scansione per troncatura guadagno */
    LogLimit = false;
    for (I = 0;I < FS;I++) {
      DLAbs = std::abs<DLReal>(FFTArray1[I]);
      if (DLAbs <= 0) {
	LogLimit = true;
	FFTArray3[I] = DRCMaxFloat;
	FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
      } else {
	if (DLAbs < RMSLevel)
	  FFTArray3[I] = (DLReal) RMSLevel / DLAbs;
	else
	  FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  } else {
    /* Determina i fattori per la limitazione guadagno */
    DLSLevel = RMSLevel / DLStart;
    DLGFactor = DLSLevel - RMSLevel;
    
    /* Scansione per limitazione guadagno */
    LogLimit = false;
    tick = true;
    for (I = 0; I < FS; I++) {
      DLAbs = std::abs(FFTArray1[I]);
      if (DLAbs < DLSLevel) {
	if(tick && I < FS/2) {
	    printf("Dip limit found at %.2f Hz\n",(float)I*SampleFreq/FS);
	    tick = false;
	  }
	/* Riassegna il guadagno del filtro */
	DLLevel = (DLSLevel - DLAbs) / DLGFactor;
	DLLevel = DLLevel / (((DLReal) 1.0) + DLLevel);
	DLLevel = DLSLevel - DLGFactor * DLLevel;
	if (DLAbs <= 0) {
	  LogLimit = true;
	  FFTArray3[I] = DRCMaxFloat;
	  FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
	} else {
	  FFTArray3[I] = (DLReal) DLLevel / DLAbs;
	  FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
	}
      } else {
	if(!tick && I < FS/2) {
	    printf("Dip limit finished at %.2f Hz\n",(float)I*SampleFreq/FS);
	    tick = true;
	  }
	FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = (DLReal) 0.0;
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  }
  
  /* Calcola la fase per il filtro a fase minima */
  IFftw(FFTArray2, InArray2, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray2[I] = -InArray2[I];
  InArray2[0] = (DLReal) 0.0;
  InArray2[FS / 2] = (DLReal) 0.0;
  Fftw(InArray2, FFTArray2, FS);

  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);

  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);

  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];

  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] FFTArray3;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}

/* Limitazione picchi a fase lineare con calcolo del valore RMS sull banda indicata */
bool LPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		 const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  int FFTSize;
  int I;
  int J;
  DLReal *InArray;
  DLComplex *FFTArray;
  DLReal RMSLevel;
  DLReal PLSLevel;
  DLReal PLLevel;
  DLReal PLGFactor;
  DLReal PLMFactor;
  DLReal PLAbs;
  DLReal PLMax;
  
  /* Determina la prima potenza di 2 >= SigLen */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for(FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;
  
  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL)
    return false;
  
  /* Copia il segnale nell'array */
  for (I = 0;I < SigLen;I++)
    InArray[I] = Sig[I];
  for (I = SigLen;I < FFTSize;I++)
    InArray[I] = (DLReal) 0.0;
  
  /* Effettua l'fft del segnale */
  Fftw(InArray, FFTArray, FFTSize);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MaxGain * GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq,W);

  /* Calcola il livello massimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MaxGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  
  /* Verifica il tipo di limitazione impostata */
  if (PLStart >= (DLReal) 1.0) {
    /* Scansione per troncatura guadagno */
    for (I = 0; I < FFTSize; I++) {
      PLAbs = std::abs(FFTArray[I]);
      if (PLAbs > RMSLevel)
	FFTArray[I] = std::polar<DLReal>(RMSLevel,std::arg<DLReal>(FFTArray[I]));
    }
  } else {
    /* Determina i fattori per la limitazione guadagno */
    PLSLevel = PLStart * RMSLevel;
    PLGFactor = RMSLevel - PLSLevel;
    PLMax = (DLReal) -1.0;
    
    /* Scansione per limitazione guadagno */
    for (I = 0; I < FFTSize; I++) {
      PLAbs = std::abs(FFTArray[I]);
      if (PLAbs > PLSLevel) {
	/* Verifica se � gi� disponibile il massimo locale */
	if (PLMax < (DLReal) 0.0) {
	  /* Cerca il massimo locale */
	  PLMax = PLAbs;
	  PLLevel = PLAbs;
	  for (J = I + 1; J < FFTSize && PLLevel > PLSLevel; J++) {
	    if (PLLevel > PLMax)
	      PLMax = PLLevel;
	    PLLevel = std::abs(FFTArray[J]);
	  }
	  
	  /* Verifica se il massimo locale � superiore al livello massimo e ricalcola i fattori di compressione */
	  if (PLMax > RMSLevel)
	    PLMFactor = PLMax - PLSLevel;
	  else
	    PLMFactor = RMSLevel - PLSLevel;
	}
	
	/* Riassegna il guadagno del filtro */
	PLLevel = (PLAbs - PLSLevel) / PLMFactor;
	FFTArray[I] = std::polar<DLReal>(PLSLevel + PLGFactor * PLLevel, std::arg<DLReal>(FFTArray[I]));
      } else
	PLMax = (DLReal) -1.0;
    }
  }
  
  /* Effettua l'fft inversa del segnale */
  IFftw(FFTArray,InArray, FFTSize);
  
  /* Ritorna il segnale limitato */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray[I];

  /* Dealloca l'array */
  delete[] FFTArray;
  delete[] InArray;
  
  /* Operazione completata */
  return true;
}

/* Limitazione picchi a fase lineare con calcolo del valore RMS sull banda indicata */
/* Versione con mantenimento continuit� della derivata prima nei punti di limitazione */
bool C1LPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		   const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  int FFTSize;
  int I;
  DLReal *InArray;
  DLComplex *FFTArray;
  DLReal RMSLevel;
  DLReal PLSLevel;
  DLReal PLLevel;
  DLReal PLGFactor;
  DLReal PLAbs;
  
  /* Determina la prima potenza di 2 >= SigLen */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for(FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;
  
  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL)
    return false;
  
  /* Copia il segnale nell'array */
  for (I = 0;I < SigLen;I++)
    InArray[I] = Sig[I];
  for (I = SigLen;I < FFTSize;I++)
    InArray[I] = (DLReal) 0.0;
  
  /* Effettua l'fft del segnale */
  Fftw(InArray, FFTArray, FFTSize);
  
  /* Determina il livello RMS del segnale */
  //RMSLevel = MaxGain * GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq);
  
  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray,FFTSize,SampleFreq,StartFreq,EndFreq,W);

  /* Calcola il livello massimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MaxGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  
  /* Verifica il tipo di limitazione impostata */
  if (PLStart >= (DLReal) 1.0) {
      /* Scansione per troncatura guadagno */
    for (I = 0; I < FFTSize; I++) {
      PLAbs = std::abs(FFTArray[I]);
      if (PLAbs > RMSLevel)
	FFTArray[I] = std::polar<DLReal>(RMSLevel,std::arg<DLReal>(FFTArray[I]));
    }
  } else {
    /* Determina i fattori per la limitazione guadagno */
    PLSLevel = PLStart * RMSLevel;
    PLGFactor = RMSLevel - PLSLevel;
    
    /* Scansione per limitazione guadagno */
    for (I = 0; I < FFTSize; I++) {
      PLAbs = std::abs(FFTArray[I]);
      if (PLAbs > PLSLevel) {
	/* Riassegna il guadagno del filtro */
	PLLevel = (PLAbs - PLSLevel) / PLGFactor;
	PLLevel = PLLevel / (((DLReal) 1.0) + PLLevel);
	FFTArray[I] = std::polar<DLReal>(PLSLevel + PLGFactor * PLLevel, std::arg<DLReal>(FFTArray[I]));
      }
    }
  }
  
  /* Effettua l'fft inversa del segnale */
  IFftw(FFTArray, InArray, FFTSize);
  
  /* Ritorna il segnale limitato */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray[I];
  
  /* Dealloca l'array */
  delete[] FFTArray;
  delete[] InArray;
  
  /* Operazione completata */
  return true;
}

/* Limitazione picchi a fase minima con calcolo del valore RMS sull banda indicata */
/* Versione basata sulla trasformata di Hilbert */
bool HMPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		  const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  DLReal *FFTArray3;
  int FS;
  int I;
  int J;
  DLReal RMSLevel;
  DLReal PLSLevel;
  DLReal PLLevel;
  DLReal PLGFactor;
  DLReal PLMFactor;
  DLReal PLAbs;
  DLReal PLMax;
  bool LogLimit;
  
  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = SigLen;

  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;

  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MaxGain  * GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq,W);

  /* Calcola il livello massimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MaxGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  /* Verifica il tipo di limitazione impostata */
  if (PLStart >= (DLReal) 1.0) {
    /* Scansione per troncamento guadagno */
    LogLimit = false;
    for (I = 0;I < FS;I++) {
      PLAbs = std::abs<DLReal>(FFTArray1[I]);
      if (PLAbs <= 0) {
	LogLimit = true;
	FFTArray3[I] = DRCMaxFloat;
	FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
      } else {
	if (PLAbs > RMSLevel)
	  FFTArray3[I] = (DLReal) RMSLevel / PLAbs;
	else
	  FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  } else {
    /* Determina i fattori per la limitazione guadagno */
    PLSLevel = PLStart * RMSLevel;
    PLGFactor = RMSLevel - PLSLevel;
    PLMax = (DLReal) -1.0;
    
    /* Scansione per limitazione guadagno */
    LogLimit = false;
    for (I = 0; I < FS; I++) {
      PLAbs = std::abs(FFTArray1[I]);
      if (PLAbs > PLSLevel) {
	/* Verifica se � gi� disponibile il massimo locale */
	if (PLMax < (DLReal) 0.0) {
	  /* Cerca il massimo locale */
	  PLMax = PLAbs;
	  PLLevel = PLAbs;
	  for (J = I + 1; J < FS && PLLevel > PLSLevel; J++) {
	    if (PLLevel > PLMax)
	      PLMax = PLLevel;
	    PLLevel = std::abs(FFTArray1[J]);
	  }
	  
	  /* Verifica se il massimo locale � superiore al livello massimo e ricalcola i fattori di compressione */
	  if (PLMax > RMSLevel)
	    PLMFactor = PLMax - PLSLevel;
	  else
	    PLMFactor = RMSLevel - PLSLevel;
	}
	
	/* Riassegna il guadagno del filtro */
	PLLevel = (PLAbs - PLSLevel) / PLMFactor;
	PLLevel = PLSLevel + PLGFactor * PLLevel;
	if (PLAbs <= 0) {
	  LogLimit = true;
	  FFTArray3[I] = DRCMaxFloat;
	  FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
	} else {
	  FFTArray3[I] = (DLReal) PLLevel / PLAbs;
	  FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
	}
      } else {
	PLMax = (DLReal) -1.0;
	FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = (DLReal) 0.0;
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  }
  
  /* Calcola la fase per il filtro a fase minima */
  IFftw(FFTArray2, InArray2, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray2[I] = -InArray2[I];
  InArray2[0] = (DLReal) 0.0;
  InArray2[FS / 2] = (DLReal) 0.0;
  Fftw(InArray2, FFTArray2, FS);

  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);
  
  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);
  
  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];

  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] FFTArray3;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}

/* Limitazione picchi a fase minima con calcolo del valore RMS sull banda indicata */
/* Versione basata sulla trasformata di Hilbert */
/* Versione con mantenimento continuit� della derivata prima nei punti di limitazione */
bool C1HMPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		    const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const DLReal W, const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  DLReal *FFTArray3;
  int FS;
  int I;
  DLReal RMSLevel;
  DLReal PLSLevel;
  DLReal PLLevel;
  DLReal PLGFactor;
  DLReal PLAbs;
  bool LogLimit;

  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = SigLen;
  
  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;

  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Determina il livello RMS del segnale */
  //RMSLevel = MaxGain  * GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq);

  /* Determina il livello RMS del segnale */
  RMSLevel = GetBLFFTRMSLevel(FFTArray1,FS,SampleFreq,StartFreq,EndFreq,W);
  
  /* Calcola il livello massimo tenendo conto delle limitazioni di banda */
  RMSLevel = (DLReal) (RMSLevel * MaxGain / sqrt(2.0 * (EndFreq - StartFreq) / SampleFreq));

  /* Verifica il tipo di limitazione impostata */
  if (PLStart >= (DLReal) 1.0) {
    /* Scansione per troncamento guadagno */
    LogLimit = false;
    for (I = 0;I < FS;I++) {
      PLAbs = std::abs<DLReal>(FFTArray1[I]);
      if (PLAbs <= 0) {
	LogLimit = true;
	FFTArray3[I] = DRCMaxFloat;
	FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
      } else {
	if (PLAbs > RMSLevel)
	  FFTArray3[I] = (DLReal) RMSLevel / PLAbs;
	else
	  FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  } else {
    /* Determina i fattori per la limitazione guadagno */
    PLSLevel = PLStart * RMSLevel;
    PLGFactor = RMSLevel - PLSLevel;
    
    /* Scansione per limitazione guadagno */
    LogLimit = false;
    for (I = 0; I < FS; I++) {
      PLAbs = std::abs(FFTArray1[I]);
      if (PLAbs > PLSLevel) {
	/* Riassegna il guadagno del filtro */
	PLLevel = (PLAbs - PLSLevel) / PLGFactor;
	PLLevel = PLLevel / (((DLReal) 1.0) + PLLevel);
	PLLevel = PLSLevel + PLGFactor * PLLevel;
	if (PLAbs <= 0) {
	  LogLimit = true;
	  FFTArray3[I] = DRCMaxFloat;
	  FFTArray2[I] = std::log<DLReal>(DRCMaxFloat);
	} else {
	  FFTArray3[I] = (DLReal) PLLevel / PLAbs;
	  FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
	}
      } else {
	FFTArray3[I] = (DLReal) 1.0;
	FFTArray2[I] = (DLReal) 0.0;
      }
    }
    
    /* Verifica se si � raggiunto il limite */
    if (LogLimit == true)
      sputs("Notice: limit reached in Hilbert filter computation.");
  }
  
  /* Calcola la fase per il filtro a fase minima */
  IFftw(FFTArray2, InArray2, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray2[I] = -InArray2[I];
  InArray2[0] = (DLReal) 0.0;
  InArray2[FS / 2] = (DLReal) 0.0;
  Fftw(InArray2, FFTArray2, FS);

  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);
  
  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);

  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];
  
  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] FFTArray3;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}

/* Livellazione norma a fase lineare */
bool LPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp)
{
  int FFTSize;
  int I;
  DLReal *InArray;
  DLComplex *FFTArray;
  DLReal G;

  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Determina la prima potenza di 2 >= SigLen */
    for(FFTSize = 1;FFTSize <= SigLen;FFTSize <<= 1);
    FFTSize *= 1 << MExp;
  } else
    FFTSize = SigLen;

  /* Alloca l'array per l'fft */
  InArray = new DLReal[FFTSize];
  if (InArray == NULL)
    return false;
  FFTArray = new DLComplex[FFTSize];
  if (FFTArray == NULL)
    return false;

  /* Copia il segnale nell'array */
  for (I = 0;I < SigLen;I++)
    InArray[I] = Sig[I];
  for (I = SigLen;I < FFTSize;I++)
    InArray[I] = (DLReal) 0.0;

  /* Effettua l'fft del segnale */
  Fftw(InArray, FFTArray, FFTSize);

  /* Scansione per normalizzazione guadagno */
  G = (1 - OGainFactor) * GetRMSLevel(Sig,SigLen);
  for (I = 0; I < FFTSize; I++)
    FFTArray[I] = std::polar<DLReal>(Gain * (G + OGainFactor * std::abs(FFTArray[I])), std::arg<DLReal>(FFTArray[I]));
  
  /* Effettua l'fft inversa del segnale */
  IFftw(FFTArray, InArray, FFTSize);
  
  /* Ritorna il segnale limitato */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray[I];

  /* Dealloca l'array */
  delete[] FFTArray;
  delete[] InArray;

  /* Operazione completata */
  return true;
}

/* Livellazione norma a fase minima */
/* Versione basato sul calcolo del cepstrum */
bool CMPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  int FS;
  int I;
  DLReal G;
  bool LogLimit;
  
  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for(FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = SigLen;
  
  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;

  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Calcola i valori per il cepstrum */
  G = OGainFactor * GetRMSLevel(Sig,SigLen);
  LogLimit = false;
  for (I = 0;I < FS;I++)
    if (std::abs<DLReal>(FFTArray1[I]) <= 0) {
      LogLimit = true;
      FFTArray2[I] = DRCMaxFloat;
    } else
      FFTArray2[I] = std::log<DLReal>(Gain / (G + (1 - OGainFactor) * std::abs<DLReal>(FFTArray1[I])));
  
  /* Verifica se si � raggiunto il limite */
  if (LogLimit == true)
    sputs("Notice: limit reached in cepstrum computation.");
  
  /* Calcola il cepstrum */
  IFftw(FFTArray2, InArray2, FS);
  
  /* Finestra il cepstrum */
  for (I = 1; I < FS/2;I++)
    InArray2[I] *= (DLReal) 2.0;
  for (I = FS/2 + 1; I < FS;I++)
    InArray2[I] = (DLReal) 0.0;

  /* Calcola la trsformata del cepstrum finestrato */
  Fftw(InArray2, FFTArray2, FS);

  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= std::exp<DLReal>(FFTArray2[I]);
  
  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);

  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];

  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}

/* Livellazione norma a fase minima */
/* Versione basata sulla trasformata di Hilbert */
bool HMPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp)
{
  DLReal *InArray1;
  DLReal *InArray2;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  DLReal *FFTArray3;
  int FS;
  int I;
  DLReal G;
  bool LogLimit;
  
  /* Controlla se si deve adottare un potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for(FS = 1;FS <= SigLen;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = SigLen;
  
  /* Alloca gli array per l'FFT */
  if ((InArray1 = new DLReal[FS]) == NULL)
    return false;
  if ((InArray2 = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;
  
  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < SigLen;I++)
    InArray1[I] = Sig[I];

  /* Azzera la parte rimanente */
  for (I = SigLen;I < FS;I++)
    InArray1[I] = (DLReal) 0.0;

  /* Trasforma l'array risultante */
  Fftw(InArray1, FFTArray1, FS);

  /* Calcola l'ampiezza per il filtro a fase minima */
  G = OGainFactor * GetRMSLevel(Sig,SigLen);
  LogLimit = false;
  for (I = 0;I < FS;I++)
    if (std::abs<DLReal>(FFTArray1[I]) <= 0) {
      LogLimit = true;
      FFTArray2[I] = DRCMaxFloat;
      FFTArray3[I] = (DLReal) 0.0;
    } else {
      FFTArray3[I] = (DLReal) Gain / (G + (1 - OGainFactor) * std::abs<DLReal>(FFTArray1[I]));
      FFTArray2[I] = std::log<DLReal>(FFTArray3[I]);
    }
  
  /* Verifica se si � raggiunto il limite */
  if (LogLimit == true)
    sputs("Notice: limit reached in Hilbert filter computation.");
  
  /* Calcola la fase per il filtro a fase minima */
  IFftw(FFTArray2, InArray2, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray2[I] = -InArray2[I];
  InArray2[0] = (DLReal) 0.0;
  InArray2[FS / 2] = (DLReal) 0.0;
  Fftw(InArray2, FFTArray2,FS);

  /* Effettua la convoluzione con il sistema inverso a fase minima */
  for (I = 0;I < FS;I++)
    FFTArray1[I] *= FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);
  
  /* Determina la risposta del sistema */
  IFftw(FFTArray1, InArray1, FS);

  /* Copia il risultato nell'array destinazione */
  for (I = 0;I < SigLen;I++)
    Sig[I] = InArray1[I];

  /* Dealloca gli array */
  delete[] FFTArray1;
  delete[] FFTArray2;
  delete[] FFTArray3;
  delete[] InArray1;
  delete[] InArray2;

  /* Operazione completata */
  return true;
}
