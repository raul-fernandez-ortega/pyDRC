%define PS_HELP
"
Postfiltering Stage
===================

  In this stage, a target transfer function is applied to the input
signal. 
  PSParmsType contains the configuration parameters. 
  PSstage is the class that performs this stage.
  
"
%enddef

%define PS_CONFIGURATION
" 
Postfiltering Stage parameters
==============================
   
  During the postfiltering stage the final target transfer function is
applied to the filter and the filter is normalized to suitable values
for the convolver used.
  

1  PSFilterType
---------------
   
  This is the type of filter used for the postfiltering stage. L means
the usual linear phase filtering, M means minimum phase filtering, T
means minimum phase filtering with initial zero truncation. If the
pre-echo truncation inversion is used and the final post filtering stage
is minimum phase all the filter taps before ISPELowerWindow are zero
(there could be some roundoff errors that make them different from zero,
but considering them zero makes no difference for our needs). So this
initial all zero part can be windowed out without changing the filter
behaviour. This way the filter becomes almost zero delay, providing a
delay of just ISPELowerWindow samples. This sometimes may be low enough
to make it usable even with home theater systems where audio delay is a
major issue. Of course to ensure that the initial all zero part is
preserved the minimum phase peak limiting should also be used.
  

2  PSInterpolationType
----------------------
   
  This parameter defines the type of interpolation used between the
points of the target transfer function. L means the usual linear
interpolation, G means logarithmic interpolation, i.e. interpolation
performed on a bilogarithmic scale, R means interpolation using Uniform
Cubic B Splines, S means interpolation using Uniform Cubic B Splines on
a bilogarithmic scale. The logarithmic interpolation makes the
definition of the target transfer function easier, without the need to
define intermediate points to get the desired behaviour on a
bilogarithmic scale. The default is G.
  The B Splines interpolation options allow for the definition of smooth
target transfer functions which provides less ringing. Be careful when
using these options because defining the right control points to get the
desired target transfer function might be tricky. If you want sharp
corners in the transfer function just place few close control points
near to the desired corner. Remember that B Splines of the type used are
unaffected by control points which are more than two control points away
from any given point on the curve. Take a look at the supplied examples
for some simple transfer function definition.
  The use of the B Spline interpolation procedure it is often useful
also for the definition of the mic compensation transfer function,
especially when only few points are available.
  

3  PSMultExponent
-----------------
   
  The multiplier exponent used for the homomorphic deconvolution used to
compute the minimum phase post filter. Usually a value of 2 or 3 is
used.
  

4  PSFilterLen
--------------
   
  Length of the FIR filter used during the postfiltering stage. Usually
between 16384 and 65536.
  

5  PSNumPoints
--------------
   
  Number of points used for the definition of the post filter frequency
response. If this parameter is 0 DRC automatically counts the number of
lines in the post filter definition file. See the following parameters
for further details about the post filter frequency response.
  

6  PSMagType
------------
   
  This parameter selects how the amplitude of the target frequency
response is defined. L means linear amplitude (0.5 means half the level,
i.e about -6 dB), D means that the amplitude is expressed in dB.
  

7  PSPointsFile (*)
-------------------
   
  File containing the post filter frequency response definition. This
file should contain PSNumPoints lines, each line in the form --Frequency
Gain--, with the gain expressed as a linear gain or in dB depending on
the PSMagType parameter value. The following examples are in dB. The
first line must have a frequency equal to 0, the last line must have a
frequency equal to BCSampleRate / 2 . A post filter definition file must
have the following format:
               
              <<
                0 -40
                18 -20
                20 0
                20000 0
                21000 -40
                22050 -100
              >>
   This is for a 44.1 KHz sample rate. 
  The post filter stage is usually used to prevent overcompensation in
the subsonic or ultrasonic range, but may be used also to change the
target frequency response from linear to a more euphonic one.
  In my HiFi system I-m currently using a modified version of the
bk-3.txt target response (see section 5.2 for the details of my current
configuration). This is usually the one which provides the best results,
with an almost flat direct sound and a smooth transition to a slightly
tilted frequency response on the stationary field.
  Starting from version 2.0.0 DRC lets you specify the phase for the
target post filter stage. Phase specification should be placed after the
amplitude specification and should be expressed in degrees. Following
the example above:
               
              <<
                0 -40 0
                18 -20 45
                20 0 90
                20000 0 180
                21000 -40 90
                22050 -100 0
              >>
   If not specified a value of 0 is assumed. Setting a phase different
than 0, i.e. flat, is useless within normal HiFi systems in almost all
circumstances. Furthermore the phase specification is used only if the
PSFilterType is L, else any phase specification is wiped out by the
minimum phase filter extraction.
  

8  PSOutWindow
--------------
   
  Final window after post filtering. This is also the length of the
generated correction filter. Usual values are between 8192 and 65536.
Filter with 65536 taps gives about 0.5 Hz resolution at 44.1 KHz sample
rate, 16384 is usually enough for most situation and 8192 gives somewhat
good results with much less computing needs during real time
convolution.
  

9  PSNormFactor
---------------
   
  Normalization factor for the correction filter. Usually 1.0. See
section 4.5.2 for some instructions on how to set this parameter.
  

10  PSNormType
--------------
   
  Normalization type for the correction filter. Usually E. See section
4.5.2 for some instructions on how to set this parameter.
  

11  PSOutFile
-------------
   
  Output file for the correction filter. This file contains the filter
to be used with the convolution engine, unless microphone compensation
has to be applied. See section 6.11 for further details.
  

12  PSOutFileType
-----------------
   
  Output file type for the correction filter. D = Double, F = Float, I =
Integer.
 
"
%enddef
%module(docstring=PS_HELP) PSstage
%{
#include <cctype>
#include "PS_stage.h"
%}

%feature("autodoc", PS_CONFIGURATION) BCParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include PS_stage.h