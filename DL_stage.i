%define DL_HELP
"
Dip Limiting Stage
==================

  This stage performs a frequency response dip limiting of the input signal. 
  DLParmsType contains the configuration parameters. 
  DLstage is the class that performs this stage.
  
"
%enddef

%define DL_CONFIGURATION
"
Dip Limiting Stage parameters
=============================
  
  

1  DLType
---------
   
  To prevent numerical instabilities during the inversion stage, deep
dips in the frequency response must be limited (truncated). This
parameter sets the type of dip limiting performed. L means linear phase,
i.e. it applies a linear phase filter that removes dips below a given
threshold, M mens minimum phase, i.e. it uses a minimum phase filter to
achieve the same result. 
  Starting with version 2.0.0 DRC performs this step only on the
prefiltered minimum phase part, just before performing the second
homomorphic deconvolution if enabled. So if the MPHDRecover parameter is
set to Y and the MPEPPreserve parameter is set to N there is almost no
difference between the two procedures, because the subsequent
homomorphic deconvolution stage wipes out any phase difference giving
just a minimum phase signal. Any difference would be caused just by
numerical errors.
  

2  DLMinGain
------------
   
  This is the minimum gain allowed in the frequency response of the
prefiltered signal. Values lower than this will be truncated. Typical
values are between 0.1 and 0.5. These are absolute values with respect
to the RMS value, i.e. 0.1 is about -20 dB, 0.5 is about -6 dB.
  

3  DLStartFreq
--------------
   
  Start frequency where the reference RMS level used for dip limiting is
computed.
  

4  DLEndFreq
------------
   
  End frequency where the reference RMS level used for dip limiting is
computed.
  

5  DLStart
----------
   
  Setting this parameter to a value between 0.0 and 1.0 enables the
--soft clipping-- dip limiting procedure. Everything below DLStart *
DLMinGain , with respect to the RMS value, get rescaled so that it ends
up between DLStart * DLMinGain  and about DLMinGain. Values for this
parameter usually are between 0.5 and 0.95, with a typical value of
0.70. Setting this parameter to a value equal to or greater then 1.0
cause DRC to switch to hard clipping of the frequency response.
  

6  DLMultExponent
-----------------
   
  Exponent of the multiplier of the FFT size used to perform the dip
limiting stage. The FFT size used is equal to the first power of two
greater than or equal to (MPBWFinalWindow + EPBWFinalWindow - 1) * (2
^DLMultExponent) . Higher exponents give more accurate dip limiting, but
requires a longer computation. Usually a value of 2 or 3 is used. If
this parameter is less than 0 no multiplier will be used. Be careful
because if the FFT size isn-t a power of two the procedure can take a
long time to complete.

7  DLNormFactor
---------------
   
  Normalization factor after dip limiting. Usually 0.
  

8  DLNormType
-------------
   
  Normalization type after processing. Usually E.
  

9  DLOutFile
------------
   
  Output file after dip limiting. Usually not used (commented out).
  

10  DLOutFileType
-----------------
   
  Output file type after dip limiting. D = Double, F = Float, I = Integer.
  
"
%enddef

%module(docstring=DL_HELP) DLstage
%{
#include <cctype>
#include "DL_stage.h"
%}
%feature("autodoc", DL_CONFIGURATION) DLParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include DL_stage.h