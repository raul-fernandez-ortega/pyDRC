%define PL_HELP
"
Base Stage
==========

  In this stage, a peak limiting treatment is perform on the input
signal. 
  PLParmsType contains the configuration parameters. 
  PLstage is the class that performs this stage.
  
"
%enddef

%define PL_CONFIGURATION
" 
Peak Limiting Stage parameters
==============================

  The peak limiting stage limits the maximum allowed gain of the filter
to prevent amplification and speaker overload.
  

1  PLType
---------
   
  Type of peak limiting applied. L means linear phase, M means minimum
phase. If PSFilterType is set to T this should be set to M to ensure
that the initial zero valued part is preserved.
  

2  PLMaxGain
------------
   
  Maximum gain allowed in the correction filter. Peaks in the correction
filter amplitude response greater than this value will be compressed to
PLMaxGain. Typical values are between 1.2 and 4. These are absolute
value with respect to the RMS value, i.e. 1.2 is about 1.6 dB and 4 is
about 12 dB. This peak limiting stage is used to prevent speaker or
amplifier overloading, resulting in dynamic range limitations which are
subjectively worse than some narrow dips in the frequency response. A
typical value is 2.0, i.e. 6 dB.
  

3  PLStart
----------
   
  Setting this parameter to a value between 0.0 and 1.0 enables the
--soft clipping-- peak limiting procedure. Everything above PLStart *
PLMaxGain , with respect to the RMS value, get rescaled so that it ends
up between PLStart * PLMaxGain  and about PLMaxGain. Values for this
parameter usually are between 0.5 and 0.95, with a typical value of
0.80. Setting this parameter to a value equal to or greater then 1.0
switch to hard clipping of the frequency response.
  

4  PLStartFreq
--------------
   
  Start frequency where the reference RMS level used for peak limiting
is computed.
  

5  PLEndFreq
------------
   
  End frequency where the reference RMS level used for peak limiting is
computed.
  

6  PLMultExponent
-----------------
   
  Exponent of the multiplier of the FFT size used to perform the peak
limiting stage. The FFT size used is equal to the first power of two
greater than or equal to PSOutWindow * (2 ^PLMultExponent) . Higher
exponents give more accurate peak limiting, but requires a longer
computation. Usually a value of 2 or 3 is used. If this parameter is
less than 0 no multiplier will be used. Be careful because if the FFT
size isn-t a power of two the procedure can take a long time to
complete.
  

7  PLOutWindow
--------------
   
  Final window after peak limiting. Usually 0, i.e. disabled.
  

8  PLNormFactor
---------------
   
  Normalization factor for the final filter. Usually 0, i.e. disabled. 
  

9  PLNormType
-------------
   
  Normalization type for the peak limited filter, usually E.
  

10  PLOutFile
-------------
   
  Output file for the peak limited filter. Usually disabled (commented
out).
  

11  PLOutFileType
-----------------
   
  Output file type for the final filter. D = Double, F = Float, I =
Integer.
"
%enddef

%module(docstring=PL_HELP) PLstage
%{
#include <cctype>
#include "PL_stage.h"
%}
%feature("autodoc", PL_CONFIGURATION) BCParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include PL_stage.h