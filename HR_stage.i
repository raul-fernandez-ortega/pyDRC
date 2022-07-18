%define HR_HELP
"
Homomorphic Recovery Stage
==========================

  After frequency window filtering a minimum phase signal is no longer
minimun phase. This stage performs another homomorphic to make it 
minimun phase again.
"
%enddef

%define HR_CONFIGURATION
"
1  HRMPHDRecover
----------------
   
  After prefiltering the minimum phase component may be no longer
minimum phase, with a bit of excess phase component added. Setting this
parameter to Y enable a second homomorphic deconvolution on the
prefiltered minimum phase component to make it minimum phase again. This
is important especially if the pre-echo truncation inversion procedure
is used. This procedure assumes that the minimum phase part really is
minimum phase, so skipping this step may cause it to fail in avoiding
pre-echo artifacts.
  

2  HRMPEPPreserve
-----------------
   
  Setting this to Y causes the excess phase part of the filtered impulse
response to be preserved after the HRMPHDRecover step. This excess phase
part is then convolved with the excess phase part of the filtered
impulse response to preserve it and invert it. This provides a slight
improvement in the direct sound phase response. The default value is Y.
  

3 HRMPHDMultExponent
--------------------
   
  Exponent of the multiplier of the FFT size used to perform the
homomorphic deconvolution described above. The FFT size used is equal to
the first power of two greater than or equal to HRMPFinalWindow * (2
^HRMPHDMultExponent). Higher exponents give more accurate results, but
require a longer computation. Usually a value of 2 or 3 is used. If this
parameter is less than 0 no multiplier will be used. Be careful because
if the FFT size isn-t a power of two the procedure can take a long time
to complete.
  

4 HRMPFinalWindow
-----------------
   
  Final window of the prefiltering stage. If set to 0 no windowing is
applied.
  

5  HRMPNormFactor
----------------------
   
  Normalization factor for the minimum phase recovered signal. 
Usually 0.
  

6  HRMPNormType
--------------------
   
  Normalization type for the filtered minimum phase component after 
recovering. Usually E.
  

7  HRMPOutFile
-------------------
   
  Output file for the minimum phase component after homomorphic 
recovery. Usually not used (commented out).
  

8  HRMPOutFileType
------------------
   
  Output file type for the minimum phase component after homomorphic 
recovery. D = Double, F = Float, I = Integer.
 
"
%enddef

%module(docstring=HR_HELP) HRstage
%{
#include "HR_stage.h"
%}

%feature("autodoc", HR_CONFIGURATION) HRParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include HR_stage.h