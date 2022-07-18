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
  File        : Fft.cpp
  Autore      : Sbragion Denis
  Descrizione : Funzioni per fft, ifft etc.
  Revisioni   :
  16/10/93    : Prima stesura.
****************************************************************************/

#include "fft.h"

DLComplex Power(DLComplex X,unsigned int N)
{
  unsigned int I;
  DLComplex Pow(1);
  
  for(I = 1;I <= N;I <<= 1) {
    if ((I & N) != 0)
      Pow *= X;
    X *= X;
  }  
  return(Pow);
}

DLComplex UnitRoot(unsigned int I,unsigned int N)
{
  DLReal Arg = (DLReal) (((2*M_PI)*(I%N))/N);
  return(DLComplex((DLReal) DLCos(Arg),(DLReal) DLSin(Arg)));
}

unsigned int FirstFactor(unsigned int N)
{
  unsigned int I;
  
  if (N < 2)
    return(1);
  else
    if (N%2 == 0)
      return(2);
    else {
      for(I = 3;I <= N/I && N%I != 0; I += 2);
    
      if (I > N/I)
	I = N;
    }

  return(I);
}

bool Fftw(DLReal *in, DLComplex *out, unsigned int n)
{
  unsigned int i;
  fftw_plan fftplan;
  double norm;
  
  fftplan = fftw_plan_dft_r2c_1d(n, in, reinterpret_cast<fftw_complex*>(out), FFTW_ESTIMATE);
  fftw_execute(fftplan);
  fftw_destroy_plan(fftplan);

  norm = sqrt(n);
  out[0] /=norm;
  for(i = 1; i < n/2; i++) {
    out[i] /= norm;
    out[n-i] = std::conj(out[i]);
  }
  out[n/2] /=norm;
  return true;
}

bool IFftw(DLComplex *in, DLReal *out, unsigned int n)
{
  unsigned int i;
  fftw_plan fftplan;
  double norm;
  
  fftplan = fftw_plan_dft_c2r_1d(n, reinterpret_cast<fftw_complex*>(in), out, FFTW_ESTIMATE);
  fftw_execute(fftplan);
  fftw_destroy_plan(fftplan);

  norm = sqrt(n);
  for(i = 0; i < n; i++) {
    out[i] /= norm;
  }
  return true;
}

DLComplex PolyEval(DLComplex P[],unsigned int N,DLComplex X)
{
  DLComplex Y(0);
  unsigned int I;
  
  if (N > 0) {
    Y = P[N-1];
    for(I = N-1;I > 0;I--)
      Y = X*Y+P[I-1];
  }
  
  return(Y);
}

bool Dft(DLComplex P[],unsigned int N)
{
  unsigned int I;
  DLComplex * R;
  
  if ((R = new DLComplex[N]) != NULL) {
    for(I = 0;I < N;I++)
      R[I] = PolyEval(P,N,std::conj(UnitRoot(I,N)));
    
    for(I = 0;I < N;I++)
      P[I] = R[I];
    
    delete(R);
    
    return(true);
  } else
    return(false);
}

bool IDft(DLComplex P[],unsigned int N)
{
  unsigned int I;
  DLComplex * R;
  
  if ((R = new DLComplex[N]) != NULL) {
    for(I = 0;I < N;I++)
      R[I] = PolyEval(P,N,UnitRoot(I,N));
    
    for(I = 0;I < N;I++)
      P[I] = R[I]/((DLReal) N);
    
    delete(R);
    
    return(true);
  } else
    return(false);
}

/***************************************************************************/
