#ifndef Convol_stl_h
#define Convol_stl_h

#include "dsplib.h"
#include "stl_type.h"
#include "convol.h"
		
// Direct Convolution 
STLvectorReal STL_Dir_Convolve(const STLvectorReal A, const STLvectorReal B);

// FFT Convolution
STLvectorReal STL_Convolve(const STLvectorReal A, const STLvectorReal B);

STLvectorReal STL_Convolve_part(const STLvectorReal A, unsigned int startA, unsigned int lenA,
				const STLvectorReal B, unsigned int startB, unsigned int lenB);

#endif
