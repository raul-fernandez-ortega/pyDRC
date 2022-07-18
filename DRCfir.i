%module DRCfir
%{
#include "fir_stl.h"
%}

%rename(LowPassFir) STL_LowPassFir;
%rename(HighPassFir) STL_HighPassFir;
%rename(BandPassFir) STL_BandPassFir;
%rename(BandStopFir) STL_BandStopFir;
%rename(HilbertFir) STL_HilbertFir;
%rename(GenericFir) STL_GenericFir;

%include typemaps.i
%include stl_type.h
%include fir_stl.h

%feature("autodoc", "
LowPassFir(unsigned int Order, DLReal Freq) --> STLvectorReal

Linear phase low pass FIR filter computation.
Input parameters:
    Order: Filter length.
    Freq: Filter frequency given as a fraction of half sample rate.
Returned object: python numarray.
") STL_LowPassFir;

%feature("autodoc", "
HighPassFir(unsigned int Order, DLReal Freq) --> STLvectorReal

Linear phase high pass FIR filter computation.
Input parameters:
    Order: Filter length.
    Freq: Filter frequency given as a fraction of half sample rate.
Returned object: python numarray.
") STL_HighPassFir;

%feature("autodoc", "
BandPassFir(unsigned int Order, DLReal Low, DLReal High) --> STLvectorRea

Linear phase band pass FIR filter computation.
Input parameters:
    Order: Filter length.
    Low: Low Pass filter frequency given as a fraction of half sample rate.
    High: High Pass filter frequency given as a fraction of half sample rate.
Returned object: python numarray.
") STL_BandPassFir;

%feature("autodoc", "
BandStopFir(unsigned int Order,DLReal Low, DLReal High) --> STLvectorReal
Linear phase band rejection FIR filter computation.
Input parameters:
    Order: Filter length.
    Low: Low Pass filter frequency given as a fraction of half sample rate.
    High: High Pass filter frequency given as a fraction of half sample rate.
Returned object: python numarray.
") STL_BandStopFir;

%feature("autodoc", "1") STL_HilbertFir;

%feature("autodoc", "
GenericFir(unsigned int Order, STLvectorReal inF, STLvectorReal inM, STLvectorReal inP, unsigned int Is, InterpolationType) --> STLvectorReal
Generic FIR filter computation.
Input parameters:
    Order: Filter length.
    inF: numarray of frequency points normalized to half sample rate.
    inM: numarray of magnitude values for each frequency point.
    inP: numarray of phase values for each frequency point.
    Is: 0 FFT length during computation is the same as Order. > 0 FFT length during computation is given by 2^Is.
    InterpolationType:  Possible values are Linear, Logarithmic, SplineLinear or SplineLogarithmic.
Returned object: python numarray.
") STL_BandStopFir;
