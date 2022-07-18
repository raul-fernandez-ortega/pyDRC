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

/* Deconvoluzione omomorfa */

/* Inclusioni */
#include "hd.h"
#include "fft.h"
#include "baselib.h"
#include "level.h"
#include <math.h>

/* Effettua la deconvoluzione omomorfa del segnale In */
/* Versione basata sul calcolo del Cepstrum */
bool CepstrumHD(const DLReal * In, DLReal * MPOut, DLReal * EPOut, const int N, const int MExp)
{
  DLReal *InArray;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  int FS;
  int I;
  bool LogLimit;
  DLReal CV;
  
  /* Controlla se si deve adottare una potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= N;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = N;
  
  /* Alloca gli array per l'FFT */
  if ((InArray = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  
  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < N;I++)
    InArray[I] = In[I];
  
  /* Azzera la parte rimanente */
  for (I = N;I < FS;I++)
    InArray[I] = 0;
  
  /* Trasforma l'array risultante */
  Fftw(InArray, FFTArray1, FS);
  
  /* Calcola i valori per il cepstrum */
  LogLimit = false;
  for (I = 0;I < FS;I++) {
    CV = std::abs<DLReal>(FFTArray1[I]);
    if (CV <= 0) {
      LogLimit = true;
      FFTArray2[I] = (DLReal) log(DRCMinFloat);
    } else
      FFTArray2[I] = std::log<DLReal>(CV);
  }
  
  /* Verifica se si è raggiunto il limite */
  if (LogLimit == true)
    sputs("Notice: log limit reached in cepstrum computation.");
  
  /* Calcola il cepstrum */
  IFftw(FFTArray2, InArray, FS);
  
  /* Finestra il cepstrum */
  for (I = 1; I < FS/2;I++)
    InArray[I] *= 2;
  for (I = FS/2 + 1; I < FS;I++)
    InArray[I] = 0;
  
  /* Calcola la trsformata del cepstrum finestrato */
  Fftw(InArray, FFTArray2, FS);
  
  /* Effettua il calcolo dell'esponenziale */
  for (I = 0;I < FS;I++)
    FFTArray2[I] = std::exp<DLReal>(FFTArray2[I]);
  
  /* Verifica se deve estrarre la componente EP */
  if (EPOut != NULL) {
    /* Determina la trasformata della parte excess phase */
    for (I = 0;I < FS;I++)
      FFTArray1[I] = std::polar((DLReal) 1.0, std::arg(FFTArray1[I]) - std::arg(FFTArray2[I]));
    
    /* Determina la risposta del sistema excess phase */
    IFftw(FFTArray1, InArray, FS);
    
    /* Copia il risultato nell'array destinazione */
    for (I = 0;I < N;I++)
      EPOut[I] = InArray[I];
  }
  
  /* Verifica se deve estrarre la componente MP */
  if (MPOut != NULL) {
    /* Determina la risposta del sistema a fase minima */
    IFftw(FFTArray2, InArray, FS);
    
    /* Copia il risultato nell'array destinazione */
    for (I = 0;I < N;I++)
      MPOut[I] = InArray[I];
  }
  
  /* Dealloca gli array */
  delete FFTArray1;
  delete FFTArray2;
  delete InArray;
  
  /* Operazione completata */
  return true;
}

/* Effettua la deconvoluzione omomorfa del segnale In */
/* Versione basata sulla trasformata di Hilbert */
bool HilbertHD(const DLReal * In, DLReal * MPOut, DLReal * EPOut, const int N,const int MExp)
{
  DLReal *InArray;
  DLComplex *FFTArray1;
  DLComplex *FFTArray2;
  DLReal *FFTArray3;
  int FS;
  int I;
  bool LogLimit;
  DLReal CV;

  /* Controlla se si deve adottare una potenza di due */
  if (MExp >= 0) {
    /* Calcola la potenza di due superiore a N */
    for (FS = 1;FS <= N;FS <<= 1);
    FS *= 1 << MExp;
  } else
    FS = N;

  /* Alloca gli array per l'FFT */
  if ((InArray = new DLReal[FS]) == NULL)
    return false;
  if ((FFTArray1 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray2 = new DLComplex[FS]) == NULL)
    return false;
  if ((FFTArray3 = new DLReal[FS]) == NULL)
    return false;
  
  /* Copia l'array sorgente in quello temporaneo */
  for (I = 0;I < N;I++)
    InArray[I] = In[I];
  
  /* Azzera la parte rimanente */
  for (I = N;I < FS;I++)
    InArray[I] = 0;
  
  /* Trasforma l'array risultante */
  Fftw(InArray, FFTArray1, FS);

  /* Calcola i valori per la trasformata di Hilbert */
  LogLimit = false;
  for (I = 0;I < FS;I++) {
    CV = std::abs<DLReal>(FFTArray1[I]);
    if (CV <= DRCMinFloat) {
      LogLimit = true;
      FFTArray2[I] = (DLReal) log(DRCMinFloat);
      FFTArray3[I] = DRCMinFloat;
    } else {
      FFTArray2[I] = std::log<DLReal>(CV);
      FFTArray3[I] = CV;
    }
  }
  
  /* Verifica se si è raggiunto il limite */
  if (LogLimit == true)
    sputs("Notice: log limit reached in Hilbert computation.");
  
  /* Calcola la fase per la componente a fase minima */
  IFftw(FFTArray2, InArray, FS);
  for (I = 1 + FS / 2; I < FS;I++)
    InArray[I] = -InArray[I];
  InArray[0] = 0;
  InArray[FS / 2] = 0;
  Fftw(InArray, FFTArray2, FS);

  /* Effettua la convoluzione per l'estrazione del sistema a fase minima */
  for (I = 0; I < FS;I++)
    FFTArray2[I] = FFTArray3[I] * std::exp<DLReal>(FFTArray2[I]);

  /* Dealloca gli array */
  delete FFTArray3;

  /* Verifica se deve estrarre la componente EP */
  if (EPOut != NULL) {
    /* Determina la trasformata della parte excess phase */
    for (I = 0;I < FS;I++)
      FFTArray1[I] = std::polar((DLReal) 1.0, std::arg(FFTArray1[I]) - std::arg(FFTArray2[I]));

    /* Determina la risposta del sistema excess phase */
    IFftw(FFTArray1, InArray, FS);

    /* Copia il risultato nell'array destinazione */
    for (I = 0;I < N;I++)
      EPOut[I] = InArray[I];
  }
  
  /* Verifica se deve estrarre la componente MP */
  if (MPOut != NULL) {
    /* Determina la risposta del sistema a fase minima */
    IFftw(FFTArray2, InArray, FS);

    /* Copia il risultato nell'array destinazione */
    for (I = 0;I < N;I++)
      MPOut[I] = InArray[I];
  }

  /* Dealloca gli array */
  delete FFTArray1;
  delete FFTArray2;
  delete InArray;

  /* Operazione completata */
  return true;
}


