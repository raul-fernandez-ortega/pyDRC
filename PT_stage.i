%define PT_HELP
"
Psychoacoustic Target Stage
===========================

This stage computes a psychoacoustic target response based on the spectral envelope of the magnitude response.
"
%enddef

%define PY_CONFIGURATION
" 
Psychoacoustic Target Stage parameters
=======================================

1  PTType
---------

Defines the type of psychoacoustic target filter to use. N means no filter, this skipping the psychoacoustic target stage completely, M means that a minimum phase filter is used and L M means that a linear phase filter is used. The default is M.

2  PTReferenceWindow (*)
------------------------

This parameter define the size used to window the corrected impulse response. The windowed response is then used to compute the spectral envelope that the target response is based upon. Usually a portion of the impulse response going from 150 ms to 500 ms is used. The default value is 26460, corresponding to a symmetric window 300 ms long on each side at 44100 Hz sample rate.

3  PTDLType, PTDLMinGain, PTDLStartFreq, PTDLEndFreq, PTDLStart, PTDLMultExponent
---------------------------------------------------------------------------------

This parameters are used to set a small dip limiting on the corrected impulse response in order to avoid numerical problems in the inversion of the spectral envelope. For a detailed description of these parameters see the similar procedure described in section 6.4. This stage is used just to prevent overflow or underflow problems so under standard conditions there is no need at all to change these parameters.

4  PTBandWidth (*)
------------------

This parameter define the resolution used for the computation of the spectral envelope. It is defined as fraction of octaves, so a value of 0.25 means a resolution of 1/4 of octave. Values below 0 down to -1 causes the adoption of the Bark scale, values below -1 causes the adoption of the ERB scale. The default value is -2, which means that the computation is performed on the ERB scale.

5  PTPeakDetectionStrength (*)
------------------------------

This parameter define how close the spectral envelope will be to to the peaks in the unsmoothed spectrum. Higher values provide a closer match. Typical values are between 5 and 30, with the default value, based on documented psychoacoustic assumptions, set to 15. Values above 50 are probably going to cause numerical problems and should be avoided.

6  PTMultExponent
-----------------

Multiplier exponent for the computation of the spectral envelope. Default

7  PTFilterLen
--------------

Length of the psychoacoustic target filter. Default set to 65536.

8  PTFilterFile
---------------

Output file for the psychoacoustic target filter. Usually not used (commented out).

9  PTFilterFileType
-------------------

Output file type for the psychoacoustic target filter. D = Double, F = Float, I = Integer.

10  PTNormFactor
----------------

Normalization factor for the inverted signal after convolution with the psychoacoustic target filter. Usually 0, i.e. disabled.

11  PTNormType
--------------

Normalization type for the inverted signal after convolution with the psychoacoustic target filter. Usually E.

12  PTOutFile
-------------

Output file for the inverted signal after convolution with the psychoacoustic target filter. Usually not used (commented out).

13  PTOutFileType
-----------------

Output file type for the inverted signal after convolution with the psychoacoustic target filter. D = Double, F = Float, I = Integer.

14  PTOutWindow
---------------

Normalization factor for the inverted signal after convolution with the psychoacoustic target filter. Usually 0, i.e. disabled.

"
%enddef

%module(docstring=PT_HELP) PTstage
%{
#include <cctype>
#include "PT_stage.h"
%}

//%feature("autodoc", PT_CONFIGURATION) PTParmsType;

%feature("autodoc",1);
%include typemaps.i
%include DRCbaseclass.i
%include PT_stage.h
