%define IS_HELP
"
Inversion Stage
===============

  This stage performs the signal inversion for a combination of minimum 
phase and excess phase signals as input data.
  ISParmsType contains the configuration parameters. 
  ISstage is the class that performs this stage.
  This stage is equivalent to PC (Prefiltering Completion) and IS (Inverse
Stage) stages of DRC.
"
%enddef

%define IS_CONFIGURATION
"
Prefilter Completion parameters
===============================
  
  The prefilter completion stage combines the prefiltered minimum phase
and excess phase parts together again. The impulse response recovered
after prefilter completion defines the impulse response of the system as
seen by DRC.
  

1  PCOutWindow
--------------
   
  Final window after prefiltering completion stage and before impulse
inversion. This is usually between 8192 and 65536. Values greater than
65536 make no sense, giving a filter resolution lower than 1 Hz at a
44.1 KHz sample rate. Furthermore inversion of signals longer than 65536
samples may require a lot of time. Starting this version 2.0.0 this step
is no longer needed with pre-echo truncation fast deconvolution, which
works directly on the minimum and excess phase components from the
prefiltering stages. So if PCOutFile is not defined and the ISType is S
this step is completely skipped.
  

2  PCNormFactor
---------------
   
  Normalization factor for the prefiltered signal. Usually 0, i.e.
disabled.
  

3  PCNormType
-------------
   
  Normalization type for the prefiltered signal. Usually E.
  

4  PCOutFile
------------
   
  Output file for the prefiltered signal. Usually not used (commented
out).
  

5  PCOutFileType
----------------
   
  Output file type for the prefiltered signal. D = Double, F = Float, I
= Integer.
  

IS - Inversion Stage parameters
===============================
  
  

1  ISType (*)
-------------
   
  Type of inversion stage. L uses the usual Toeplitz least square
inversion, T activates the pre-echo truncation fast deconvolution.
  

2  ISPETType (*)
----------------
   
  This sets the type of pre echo truncation applied when ISType is T. f
means a fixed pre-echo truncation, s means a time dependent pre-echo
truncation applied using the usual single side sliding low-pass
procedure, but with reversed behaviour, i.e. only what comes before the
impulse center is processed.
  

3  ISPrefilterFctn
------------------
   
  Same as MPPrefilterFctn but for the pre-echo truncation windowing.
  

4  ISPELowerWindow (*)
----------------------
   
  When ISPETType is f this is the number of samples before the impulse
center where the inverted impulse response is considered pre-echo. This
can have a value between 16 and 512, with a typical value of 64. When
ISPETType is s this is the number of sample considered pre-echo at the
ISPEStartFreq frequency, with a typical value of EPLowerWindow / 40 .
  

5  ISPEUpperWindow (*)
----------------------
   
  When ISPETType is f this is the number of sample before the impulse
center where the pre-echo region, defined by the previous parameter,
ends, and the full impulse response of the inverted filter should start.
This is usually few sample (8-64), with a typical value of 32. When
ISPETType is s this is the number of sample considered pre-echo at the
ISPEEndFreq frequency, with a typical value of ISPELowerWindow / 100 .
  

6 ISPEStartFreq
---------------
   
  Start frequency for the sliding low pass pre-echo truncation
procedure. Usually 20 Hz. Used only when ISPETType is s.
  

7  ISPEEndFreq
--------------
   
  End frequency for the sliding low pass pre-echo truncation procedure.
Usually 21000 Hz. Used only when ISPETType is s.
  

8  ISPEFilterLen
----------------
   
  Length of the filter used for the pre-echo truncation sliding lowpass
procedure. Usually 8192. Used only when ISPETType is s.
  

9  ISPEFSharpness (*)
---------------------
   
  Same as MPFSharpness, but applied to the inversion stage pre-echo
truncation. Here slightly bigger values usually provide better results
because of the shorter windowing. The default value is 0.5.
  

10  ISPEBandSplit
-----------------
   
  For the sliding lowpass prefiltering this just gives the rate at which
log messages are reported and has no effect on the prefiltering
procedure, which is always stepless. Used only when ISPETType is s.
  

11  ISPEWindowExponent (*)
--------------------------
   
  Window exponent applied to the pre-echo truncation sliding lowpass
procedure. Usual values goes from 0.5 to 1.5, with a typical value of
1.0. Used only when ISPETType is s.
  

12  ISPEOGainFactor
-------------------
   
  This parameter has the same effect of the EPPFOGainFactor (see section
6.5.14) but applied to the renormalization of the excess phase part of
the inverse filter after pre-echo truncation. Used in conjunction with
the EPPFOGainFactor parameter, this parameter can be used to balance the
amount of correction applied to the direct sound compared to the amount
of correction applied to the reverberant field. A negative value
disables the renormalization.
  

13  ISSMPMultExponent
---------------------
   
  This is the exponent of the multiplier for the S inversion stage,
using the longest of the input and output signals as a basis. This
parameter should be set using the same criterion used for the
MPHDMultExponent parameters and a values of at least 3 is suggested.
  

14 ISOutWindow
--------------
   
  Final window after inversion stage. Usually 0, i.e. disabled, with the
L type inversion stage. With the S type this is the output filter size
and can be any length but usually is between 8192 and 65536. If it is 0
than a length equal to MPPFFinalWindow + EPPFFinalWindow - 1 , i.e. the
length of the convolution of the two components together, is assumed and
no windowing is applied to the output filter.
  

15  ISNormFactor
----------------
   
  Normalization factor for the inverted signal. Usually 0, i.e.
disabled.
  

16  ISNormType
--------------
   
  Normalization type for the inverted signal. Usually E.
  

17  ISOutFile
-------------
   
  Output file for the inverted signal. Usually not used (commented out).
  

18  ISOutFileType
-----------------
   
  Output file type for inverted signal. D = Double, F = Float, I =
Integer.
  

"
%enddef

%module(docstring=IS_HELP) ISstage
%{
#include <cctype>
#include "IS_stage.h"
%}

%feature("autodoc", IS_CONFIGURATION) ISParmsType;

%feature("autodoc",1);
%include typemaps.i
%include DRCbaseclass.i
%include IS_stage.h