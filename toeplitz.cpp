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

/* Risoluzione matrici Toeplitz */

#include "toeplitz.h"

bool ToeplitzSolve(const DLReal * A, const DLReal * B, DLReal * X, int N) 
{
  int I, J, K;
  DLReal PError;
  DLReal RC;
  DLReal Tmp;
  DLReal Sum;
  DLReal * TA;
  
  PError = A[0];
  if (PError <= 0)
    return false;
  
  /* Alloca un arrya temporaneo */
  if ((TA = new DLReal[N]) == NULL)
    return false;

  X[0] = B[0] / PError;
  for (K = 0; K < N - 1; K++)  {
    Sum = A[K + 1];
    for (I = 0; I < K; I++)
      Sum = Sum - A[K - I] * TA[I];
    RC = -Sum / PError;
    
    /* Calcola l'errore del predittore (equivalente a 
       PError = PError * (1 - RC**2). Un cambio di segno di 
       PError indica che RC è maggiore dell'unità e quindi siamo
       di fronte ad un sistema di equazioni che non è definito
       positivo */
    
    PError = PError + RC * Sum;
    if (PError <= 0.0) {
      delete TA;
      return 1;
    }
    
    TA[K] = -RC;
    for (I = 0, J = K - 1; I < J; I++, J--) {
      Tmp = TA[I] + RC * TA[J];
      TA[J] = TA[J] + RC * TA[I];
      TA[I] = Tmp;
    }
    
    if (I == J)
      TA[I] = TA[I] + RC * TA[I];
    
    Sum = B[K+1];
    
    for (I = 0, J = K + 1; I <= K; I++, J--)
      Sum = Sum - X[I] * A[J];
    
    X[K + 1] = Sum / PError;
    
    for (I = 0, J = K; I <= K; I++, J--)
      X[I] = X[I] - X[K + 1] * TA[J];
    
  }
  
  /* Rimuove l'array temporaneo */
  delete TA;
  
  return true;
}
