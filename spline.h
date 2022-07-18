/****************************************************************************

    DRC: Digital Room Correction
    Copyright (C) 2002 Denis Sbragion

		This file is part of DRC, Digital Room Correction

		DRC  is  distributed  with  NO WARRANTY OF ANY KIND. No author or
		distributor accepts any responsibility for  the  consequences  of
		using  it,  or  for  whether  it serves any particular purpose or
		works at all, unless he or she says so in writing.  Refer to  the
		Aladdin Free Public License (the "License") for full details.

		Every copy of DRC must include a copy of the License, normally in
		a plain ASCII text file named PUBLIC. The License grants you  the
		right  to  copy, modify and redistribute DRC, but only under cer­
		tain conditions described in the License.   Among  other  things,
		the License requires that the copyright notice and this notice be
		preserved on all copies.

		You can contact the author on Internet at the following address:

				d.sbragion@infotecna.it

		This program uses the parsecfg library from Yuuki  NINOMIYA.  De­
		tails  on  this  library  can be found in the parsecfg.c and par­
		secfg.h files.  Many thanks to Yuuki NINOMIYA for this useful li­
		brary.

****************************************************************************/

/* Spline routines */

#ifndef Spline_h
#define Spline_h

#include "dsplib.h"

/* Tipo spline da generare */
typedef enum { SplineBDef, SplineBNat } CSplineType;

/* Valore della spline lineare nel punto X */
DLReal L1SplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal X);

/* Valore della spline lineare nei punti X */
/* X deve essere un array crescente strettamente monotonico */
void AL1SplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Prepara la base per la spline cubica interpolante, ponendola in SPD2YN */
bool SplinePrepare(const DLReal * XN, const DLReal * YN, const int N, const DLReal SPD1Y1, const DLReal SPD1YN,
		   const CSplineType SPType, DLReal * SPD2YN);

/* Valore della spline cubica interpolante nel punto X */
DLReal SplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * SPD2YN, const DLReal X);

/* Valore della spline cubica interpolante nei punti X */
/* X deve essere un array crescente strettamente monotonico */
void ASplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * SPD2YN, const DLReal * X, const int XS, DLReal * Y);

/* Valore della B spline cubica uniforme nel punto X */
DLReal B3SplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal X);

/* Valore della B spline cubica uniforme nei punti X */
/* X deve essere un array crescente strettamente monotonico */
void AB3SplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Valore della B spline cubica di tipo Catmull Rom nel punto X */
DLReal B3CRSplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal X);

/* Valore della B spline cubica di tipo Catmull Rom  nei punti X */
/* X deve essere un array crescente strettamente monotonico */
void AB3CRSplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Interpolazione su scala lineare tramite B spline cubica
   uniforme con riparametrizzazione arc length tramite B spline
   cubica Catmull-Rom interpolante */
bool AB3CRMSplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Interpolazione su scala lineare tramite
   Hermite spline cubica monotonica */
void APCHIPSplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Interpolazione su scala lineare tramite B spline cubica
   uniforme con riparametrizzazione arc length tramite spline
   cubica Hermite monotonica */
bool AB3HSMSplineValue(const DLReal * XN, const DLReal * YN, const int N, const DLReal * X, const int XS, DLReal * Y);

/* Definizione tipo spline da utilizzare */
typedef enum {
  CSLLinear, /* Lineare */
  CSLCSpline, /* Spline cubica interpolante classica, classe C2, non monotonica */
  CSLBSpline, /* B Spline cubica approssimante, classe C2, non interpolante */
  CSLCRSpline, /* CatmullRom, interpolante, non monotonica */
  CSLHSSpline, /* Hermite spline, interpolante, monotonica (PCHIP) */
} CSLSplineType;

/* Definizione tipo asse da utilizzare */
typedef enum {
  CSLLinXLinY,
  CSLLogXLinY,
  CSLLinXLogY,
  CSLLogXLogY,
} CSLAxisType;

/* Funzione generica interpolazione */
/* Nel caso si utilizzi un asse logaritmico i valori su tale
   asse devono	essere strettamente maggiori di 0 */
bool CSLASplineValue(const DLReal * XN, const DLReal * YN,
		     const int N, const DLReal * X, const int XS, DLReal * Y,
		     const CSLSplineType SplineType, const CSLAxisType AxisType);

/* Funzione generica interpolazione, versione complessa */
/* Nel caso si utilizzi un asse logaritmico i valori di ampiezza
   su tale	asse devono	essere strettamente maggiori di 0.
   I valori di fase vengono sempre considerati su ascissa lineare */
bool CSLCASplineValue(const DLReal * XN, const DLReal * MN,
		      const DLReal * PN, const int N, const DLReal * X, const int XS,
		      DLComplex * Y, const CSLSplineType SplineType, const CSLAxisType AxisType);

#endif
