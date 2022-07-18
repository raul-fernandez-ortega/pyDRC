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

#ifndef HD_h
#define HD_h

/* Inclusioni */
#include "dsplib.h"

/* Effettua la deconvoluzione omomorfa del segnale In */
/* Versione basata sul calcolo del Cepstrum */
bool CepstrumHD(const DLReal * In, DLReal * MPOut, DLReal * EPOut, const int N, const int MExp);

/* Effettua la deconvoluzione omomorfa del segnale In */
/* Versione basata sulla trasformata di Hilbert */
bool HilbertHD(const DLReal * In, DLReal * MPOut, DLReal * EPOut, const int N, const int MExp);

#endif
