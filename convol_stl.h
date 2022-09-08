#ifndef Convol_stl_h
#define Convol_stl_h

#include "dsplib.h"
#include "stl_type.h"
#include "convol.h"
		
// Direct Convolution 
STLvectorReal *STL_Dir_Convolve(STLvectorReal *A, STLvectorReal *B);

// FFT Convolution
STLvectorReal *STL_Convolve(STLvectorReal *A, STLvectorReal *B);

STLvectorReal *STL_Convolve_part(STLvectorReal *A, unsigned int startA, unsigned int lenA,
				 STLvectorReal *B, unsigned int startB, unsigned int lenB);

#endif
