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

/****************************************************************************
  Progetto    : DSP Library.
  File        : Convol.cpp
  Autore      : Sbragion Denis
  Descrizione : Funzioni per la convoluzione.
  Revisioni   :
  16/10/93    : Prima stesura.
****************************************************************************/

#include "convol.h"
#include "fft.h"

void Convolve(const DLReal * A,unsigned int NA, const DLReal * B, unsigned int NB, DLReal * R)
{
  unsigned int I,J,S,E,NR = NA+NB-1;
  
  for(I = 0;I < NR;I++) {
    if (I < NB)
      S = 0;
    else
      S = I-NB+1;
    
    if (I < NA)
      E = I+1;
    else
      E = NA;
    
    R[I] = 0;
    for(J = S;J < E;J++)
      R[I] += A[J]*B[I-J];
  }
}

void Convolve(const DLComplex * A, unsigned int NA, const DLComplex * B, unsigned int NB, DLComplex * R)
{
  unsigned int I,J,S,E,NR = NA+NB-1;
  
  for(I = 0;I < NR;I++) {
    if (I < NB)
      S = 0;
    else
      S = I-NB+1;
    
    if (I < NA)
      E = I+1;
    else
      E = NA;
    
    R[I] = 0;
    for(J = S;J < E;J++)
      R[I] += A[J]*B[I-J];
  }
}

bool FftwConvolve(const DLReal * A, unsigned int NA, const DLReal * B, unsigned int NB, DLReal *C)
{
  unsigned int n, size;
  DLReal *InArrayA, *InArrayB;
  DLComplex *fftA, *fftB;

  size = NA + NB - 1;
  
  InArrayA = new DLReal [size];
  InArrayB = new DLReal [size];
  fftA = new DLComplex [size];
  fftB = new DLComplex [size];

  memset(InArrayA, 0, sizeof(DLReal)*size);
  memset(InArrayB, 0, sizeof(DLReal)*size);

  for(n = 0; n < NA; n++)
    InArrayA[n] = A[n];
  for(n = 0; n < NB; n++)
    InArrayB[n] = B[n];

  Fftw(InArrayA, fftA, size);
  Fftw(InArrayB, fftB, size);
  
  for(n = 1; n < size; n++) {
    fftA[n] *= fftB[n] * (double)(size); 
  }	 

  IFftw(fftA, InArrayA, size);
  for(n = 0; n < size; n++)
    C[n] = InArrayA[n];
  delete fftA;
  delete fftB;
  delete InArrayA;
  delete InArrayB;
  return true;

}
