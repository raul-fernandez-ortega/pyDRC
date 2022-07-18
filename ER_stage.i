%define ER_HELP
"
Excess Phase Recovery Stage
===========================

  After band windowing the excess phase component usually 
needs reequalization to get the flat frequency response it should have.
In this stage the equalization needed is applied.   
  ERParmsType contains the configuration parameters. 
  ERstage is the class that performs this stage.
"
%enddef

%define ER_CONFIGURATION
"  
1  EREPFlatGain
---------------
   
  After band windowing the excess phase component usually need
reequalization to get the flat frequency response it should have. This
is the gain applied with respect to the RMS level of the signal to get
this flat frequency response. Usually 1, a value of 0 disables this
step. Skipping this step, i.e. setting this parameter to 0, usually
gives bad results.
  

2 EREPOGainFactor
------------------
   
  This parameter controls how the excess phase flattening set by the
previous parameter is performed. Setting this to 0 tries to get a
perfectly flat excess phase component after prefiltering, as in version
1.3.0 of DRC. Sometimes trying to achieve a perfectly flat excess phase
component may lead to strong artifacts due to the limited length of the
window available for the impulse response of the excess phase component.
So keeping some of the magnitude response of the excess phase component
after prefiltering may help reducing or eliminating these artifacts.
This parameter control how much of the magnitude response is kept.
Usually this parameters takes values from 0 to about 0.5, with more
typical values not higher than 0.1.
  Furthermore this parameters applies only to the linear phase and
minimum phase excess phase flattening, it isn-t available for the D type
of excess phase flattening.
  

3  EREPFlatType
----------------
   
  This is the type of procedure adopted for the excess phase component
renormalization. L means applying linear phase renormalization, M means
applying minimum phase renormalization, D means applying another
homomorphic deconvolution stage to extract just the excess phase
component of the prefiltered excess phase component. L applies a linear
phase filter that equalizes the excess phase amplitude response to flat,
M mens minimum phase, i.e. it uses a minimum phase filter to achieve the
same result. The D procedure provides the same effect of the M procedure
when HREPOGainFactor is equal to 0. Any difference is just caused by
numerical errors.
  

4  EREPFGMultExponent
----------------------
   
  Exponent of the multiplier of the FFT size used to perform the
frequency response flattening. The FFT size used is equal to the first
power of two greater than or equal to HREPWFinalWindow * (2
^HREPFGMultExponent) . Higher exponents give more accurate results, but
require a longer computation. This parameter should be set using the
same criteria described in HRMPHDMultExponent. If this parameter is less
than 0 no multiplier will be used. Be careful because if the FFT size
isn-t a power of two the procedure can take a long time to complete.
  

5  EREPNormFactor
------------------
   
  Normalization factor for the excess phase component after reequalization.
Usually 0, i.e. disabled, if HREPFlatGain is used, else a value of 1 is 
used.
  

6  EREPNormType
----------------
   
  Normalization type for the excess phase component after reequalization.
Usually E.
  

7  EREPOutFile
---------------
   
  Output file for the excess phase component after reequalization. Usually
not used (commented out).
  

8 EREPOutFileType
------------------
   
  Output file type for the excess phase component after reequalization.
D = Double, F = Float, I = Integer.

"
%enddef

%module(docstring=ER_HELP) HRstage
%{
#include "ER_stage.h"
%}

%feature("autodoc", ER_CONFIGURATION) ERParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include ER_stage.h