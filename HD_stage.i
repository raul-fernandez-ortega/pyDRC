%define HD_HELP
"
Homomorphic Deconvoltion Stage
=============================

  In this stage, the input signal is separated into minimun phase and
excess phase parts. 
  HDParmsType contains the configuration parameters. 
  HDstage is the class that performs this stage.
  
"
%enddef
%define HD_CONFIGURATION
" 
Homomorphic Deconvolution Stage parameters
==========================================
  
1  HDMultExponent
---------------------
   
  Exponent of the multiplier of the FFT size used to perform the
homomorphic deconvolution. The FFT size used is equal to the first power
of two greater than or equal to BCInitWindow * (2 ^HDMultExponent).
Higher exponents give more accurate deconvolution, providing less
circular convolution artifacts. 
  For previous DRC versions achieving low circular artifacts was not so
important because they were masked by the higher pre-echo artifacts in
other steps. Starting with version 2.0.0 it is possible to achieve
really low pre echo artifacts so circular artifacts now are an issue,
because when truncated by the pre-echo truncation inversion procedure
they may cause errors on the phase correction. In this situation a value
of at least 3 is suggested.
  

2  HDMPNormFactor
---------------------
   
  Normalization factor for the minimum phase component. Usually 1.
  

3  HDMPNormType
-------------------
   
  Normalization type for the minimum phase component. Usually E.
  

4  HDMPOutFile
------------------
   
  Output file for the minimum phase component. Usually not used
(commented out).
  

5  HDMPOutFileType
----------------------
   
  Output file type for the minimum phase component. D = Double, F =
Float, I = Integer.
  

6  HDEPNormFactor
---------------------
   
  Normalization factor for the excess phase component. Usually 1.
  

7  HDEPNormType
-------------------
   
  Normalization type for the excess phase component. Usually E.
  

8  HDEPOutFile
------------------
   
  Output file for the excess phase component. Usually not used
(commented out).
  

9  HDEPOutFileType
----------------------
   
  Output file type for the excess phase component. D = Double, F =
Float, I = Integer.

(*)  Parameters which are important for the quality of the generated
filters.
"
%enddef

%module(docstring=HD_HELP) HDstage
%{
#include "HD_stage.h"
%}

%feature("autodoc", HD_CONFIGURATION) HDParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include HD_stage.h