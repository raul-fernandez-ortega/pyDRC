%module DRCconvol
%{
#include "convol_stl.h"
%}

%rename(Dir_Convolve)    STL_Dir_Convolve;
%rename(Convolve)        STL_Convolve;
%rename(Convolve_part)   STL_Convolve_part;
%rename(FftConvolve)     STL_FftConvolve;

%include typemaps.i
%include stl_type.h
%include dsplib.h
%include convol_stl.h

%feature("autodoc", "
Dir_Convolve(const STLvectorReal A, const STLvectorReal B) --> STLvectorReal 
Direct signal convolution. Input data: two real number numarrays of N and M length.
Output: the resulting convolution as real number numarray of N+M-1 length.
") STL_Dir_Convolve;

%feature("autodoc", "
Convolve(const STLvectorReal A, const STLvectorReal B) --> STLvectorReal
FFT convolution. Input data: two real number numarrays of N and M length.
Output: the resulting convolution as real number numarray of N+M-1 length.
") STL_Convolve;

%feature("autodoc", "
Convolve_part(const STLvectorReal A, unsigned int startA, unsigned int lenA,
              const STLvectorReal B, unsigned int startB, unsigned int lenB) --> STLvecotrReal
Partial FFT convolution: Input data: two real number numarrays and the desired start and length 
of convolution vectors.
Output: the resulting signal convolution as real number numarray of lenA+lenB-1 length.
") STL_Convolve_part;

