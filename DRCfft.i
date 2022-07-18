%{
#include "fft_stl.h"
%}

%include typemaps.i

%rename(Fft)                  STL_Fftw;
%rename(IFft)                 STL_IFftw;

%include stl_type.h
%include fft_stl.h

%feature("autodoc", "
Fftw(STLvectorReal Sig) --> STLvectorComplex

FFT of a real numarray given by input signal Sig. 
Returned object is a complex numarray.
") STL_Fftw;

%feature("autodoc", "
IFftw(STLvectorComplex SIg) --> STLvectorReal

FFT of a complex numarray given by input signal Sig. 
Returned object is a real numarray with the real part of the IFFT.
") STL_IFftw;
