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
  File        : Convol.h
  Autore      : Sbragion Denis
  Descrizione : Funzioni per la convoluzione.
  Revisioni   :
  16/10/93    : Prima stesura.
****************************************************************************/

#ifndef Convol_h
  #define Convol_h

  #include "dsplib.h"
  #include <stdio.h>
  #include <string.h>
		
  // Effettua la convoluzione con il metodo diretto degli array A e B.
  // Il risultato viene posto in R che deve essere di lunghezza NA+NB-1.
  void Convolve(const DLReal * A, unsigned int NA, const DLReal * B, unsigned int NB, DLReal * R);
  void Convolve(const DLComplex * A, unsigned int NA, const DLComplex * B, unsigned int NB, DLComplex * R);

  bool FftwConvolve(const DLReal * A, unsigned int NA, const DLReal * B, unsigned int NB, DLReal *R);

#endif

/***************************************************************************/
