#ifndef Fft_stl_h
#define Fft_stl_h

#include "stl_type.h"
#include "fft.h"

STLvectorComplex STL_Fftw(STLvectorReal Sig);

STLvectorReal STL_IFftw(STLvectorComplex SIg);
#endif
