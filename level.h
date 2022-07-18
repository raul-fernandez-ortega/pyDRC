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

/* Funzioni correzione e gestione livello del segnale */

#ifndef Level_h
	#define Level_h

	/* Inclusioni */
	#include "dsplib.h"

	/* Calola il valore RMS del segnale Sig */ 
	DLReal GetRMSLevel(const DLReal * Sig,const int SigLen);

	/* Calola il valore RMS del segnale Sig sulla banda di frequenze indicate */ 
	DLReal GetBLRMSLevel(const DLReal * Sig,const int SigLen,const int SampleFreq, 
		const DLReal StartFreq, const DLReal EndFreq,const int MExp);

	/* Calola il valore RMS del segnale trasformato FFTArray sulla banda di frequenze indicate */ 
	DLReal GetBLFFTRMSLevel(const DLComplex * FFTArray,const int FFTSize,const int SampleFreq, 
		const DLReal StartFreq, const DLReal EndFreq);

	/* Tipi normalizzazione */
	typedef enum { NormMax = 'M', NormSum = 'S', 
		NormEuclidean = 'E' } NormType;

	/* Effettua la normalizzazione del segnale al valore indicato e
	secondo il metodo indicato */
	void SigNormalize(DLReal * Sig,const int SigLen,const DLReal NormFactor,
		const NormType TNorm);

	/* Limitazione valli a fase lineare con calcolo del valore RMS sull banda indicata */
	bool LPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione valli a fase lineare con calcolo del valore RMS sull banda indicata */
	/* Versione con mantenimento continuità della derivata prima nei punti di limitazione */
	bool C1LPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione valli a fase minima con calcolo del valore RMS sull banda indicata */
	/* Versione basata sulla trasformata di Hilbert */
	bool HMPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione valli a fase minima con calcolo del valore RMS sull banda indicata */
	/* Versione basata sulla trasformata di Hilbert */
	/* Versione con mantenimento continuità della derivata prima nei punti di limitazione */
	bool C1HMPDipLimit(DLReal * Sig,const int SigLen,const DLReal MinGain,const DLReal DLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione picchi a fase lineare con calcolo del valore RMS sull banda indicata */
	bool LPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione picchi a fase lineare con calcolo del valore RMS sull banda indicata */
	/* Versione con mantenimento continuità della derivata prima nei punti di limitazione */
	bool C1LPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);	

	/* Limitazione picchi a fase minima con calcolo del valore RMS sull banda indicata */
	/* Versione basata sulla trasformata di Hilbert */
	bool HMPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Limitazione picchi a fase minima con calcolo del valore RMS sull banda indicata */
	/* Versione basata sulla trasformata di Hilbert */
	/* Versione con mantenimento continuità della derivata prima nei punti di limitazione */
	bool C1HMPPeakLimit(DLReal * Sig,const int SigLen,const DLReal MaxGain,const DLReal PLStart,
		const int SampleFreq, const DLReal StartFreq, const DLReal EndFreq, const int MExp);

	/* Livellazione norma a fase lineare */
	bool LPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp);

	/* Livellazione norma a fase minima */
	/* Versione basato sul calcolo del cepstrum */
	bool CMPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp);

	/* Livellazione norma a fase minima */
	/* Versione basata sulla trasformata di Hilbert */
	bool HMPNormFlat(DLReal * Sig,const int SigLen,const DLReal Gain,const DLReal OGainFactor,const int MExp);
#endif
