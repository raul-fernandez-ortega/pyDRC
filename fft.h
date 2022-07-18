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

		This program uses  also the FFT  routines from  Takuya Ooura and
		the GNU Scientific  Library (GSL).  Many thanks  to Takuya Ooura
		and the GSL developers for these efficient routines.

****************************************************************************/

/****************************************************************************
  Progetto    : DSP Library.
  File        : Fft.h
  Autore      : Sbragion Denis
  Descrizione : Funzioni per fft, ifft etc.
  Revisioni   :
  16/10/93    : Prima stesura.
****************************************************************************/

#ifndef Fft_h
#define Fft_h

#include "dsplib.h"
#include <fftw3.h>

// Ritorna la potenza N-esima di X
// Complessita` : O(log2(N)).
DLComplex Power(DLComplex X,unsigned int N);

// Ritorna la J-esima radice N-esima dell' unita`.
// La prima ha indice 0, l' ultima N-1, poi si ripetono ciclicamente.
DLComplex UnitRoot(unsigned int I,unsigned int N);

// Ritorna il primo divisore di N.
unsigned int FirstFactor(unsigned int N);

bool Fftw(DLReal *in, DLComplex *out, unsigned int n);
bool IFftw(DLComplex *in, DLReal *out, unsigned int n);

// Valutazione del polinomio P nel punto X col metodo di Horner
// Il polinomio e` considerato nella forma :
// P[0] + P[1]*X + P[2]*X^2 + ... + P[N-1]*X^(N-1)
// Complessita` : O(N).
DLComplex PolyEval(DLComplex P[],unsigned int N,DLComplex X);

// Dft e IDft del polinomio P (Valutazione nelle radici dell'unita `).
// Richiede spazio in memoria per almemo N complessi. Se l' allocazione
// non ha sucesso ritorna False altrimenti ritorna True;
// Complessita` : O(N^2).
bool Dft(DLComplex P[],unsigned int N);
bool IDft(DLComplex P[],unsigned int N);

#endif

/***************************************************************************/
