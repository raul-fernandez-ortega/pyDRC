%define WF_HELP
"
(Frequency-dependant Time) Window Filtering Stage
=======================================

  In this stage, the input signal is filtered using frequency-dependant
time windows according to the configuration parameters. 
  WFParmsType contains the configuration parameters. 
  WFstage is the class that performs this stage.
  
"
%enddef

%define WF_CONFIGURATION
"
Window Filtering Stage parameters
=================================
  
1  WFfilterType
---------------
   
  This parameter can be either B for the usual band windowing
prefiltering stage or S for the sliding lowpass method. The first method
splits the input response into log spaced bands and window them
depending on some parameters but basically with a window length which
decrease exponentially with the frequency of the band. The sliding
lowpass method instead filters the impulse response with a time varying
lowpass filter with a cutoff frequency which decreases exponentially
with the sample position with respect to the time axis zero. This is a
stepless procedure. 
  Using either a lowercase b or s for the WFfilterType parameters enable 
the single side version of the prefiltering procedures. The procedure is 
applied starting from the impulse center, leaving the first half of the 
impulse response unchanged. This gives less pre-echo artifacts, and 
should be used when the pre-echo truncation inversion procedure is used. 
  

2  WFfilterFctn
---------------
   
  This parameter set the type of prefiltering function used, i.e. P for
the usual inverse proportional function, or B for the bilinear transform
based prefiltering function. For a comparison between the two functions
see figure 6 for a comparison between the two functions. The default is
B.
  

3  WFWindowGap
--------------
   
  This parameters changes a little the window function (Blackman) used
for the band windowing prefiltering stage. It sets a small flat unitary
gap, whose length is expressed in samples, at the center of the window
function, so that even if the impulse center is slightly off with
respect to the time axis zero there is no high frequency overcorrection.
For band windowing prefiltering procedure usually this overcorrection is
in the order of 0.1 - 0.2 dB at 20 KHz for errors of 2 to 3 samples, so
it is not important at all in real world situations, but if you want to
fix even this small problem this parameter lets you do it.
  WFWindowGap should never be more than 2 sample less than WFUpperWindow
and it is usually no more than few samples (5 to 10). If in any
situation it is bigger than the calculated window DRC automatically
reduces the gap to 2 less than the applied window. When WFWindowGap is 0
DRC behaves exactly as in the previous version. For the sliding lowpass
procedure this sets just the window gap used for the initial windowing
before the procedure starts.
  

4  WFLowerWindow (*)
--------------------
   
  Length of the window for the minimum phase component prefiltering at
the bottom end of the frequency range. Longer windows cause DRC to try
to correct a longer part of the impulse response but cause greater
sensibility on the listening position. Typical values are between 16384
and 65536. WFLowerWindow must be not greater than BCInitWindow.
  

5  WFUpperWindow (*)
--------------------
   
  Length of the window for the minimum phase component prefiltering at
the upper end of the frequency range. Longer windows cause DRC to try to
correct a longer part of the impulse response but cause greater
sensibility on the listening position. Typical values are between 32 and
128. WFUpperWindow must be not greater than WFLowerWindow, and usually
is much shorter than that.
  

6  WFStartFreq
--------------
   
  Start frequency for the prefiltering stage. Usually 20 Hz or just
something less.
  

7  WFEndFreq
------------
   
  End frequency for the prefiltering stage. Usually something more than
20 KHz, for example 21000. Of course you must be using a sample rate
which is greater than 40 KHz to set this above 20 KHz.
  

8  WFWindowExponent (*)
-----------------------
   
  This is the exponent used in the frequency dependent window length
computation for the band windowing procedure, or in the computation of
the time dependent cutoff frequency for the sliding lowpass procedure.
  The window length for band windowing is computed with the following
expression:
                                     1        
                               -------------- 
                           W =             WE 
                               A * (F + Q)    
   Where W is the window length, F is the normalized frequency, WE is
the window exponent, A and Q are computed so that W is equal to
WFLowerWindow at WFStartFreq and is equal to WFUpperWindow at WFEndFreq.
If you set WFLowerWindow equal to the value used for WFInitWindow in DRC
1.2, set WFWindowExponent to the same value of version 1.2 and set
WFUpperWindow to the value you got at the upper limit of the frequency
range in version 1.2 you should get results much similar to the 1.2 DRC
release.
  In a similar way the cutoff frequency for the sliding lowpass
prefiltering stage is computed with:
                                     1        
                               -------------- 
                           F =             WE 
                               A * (W + Q)    
   with identical parameters but reversed perspective, i.e. the cutoff
frequency is computed from the window length and not the other way
around. In both cases W and F are considered normalized between 0 and 1.
  These parametric functions are used when the proportional function is
selected using the WFPrefilterFctn parameter. The parametric functions 
derived from the bilinear transformation are quite different and more 
complicated, so they aren-t explained here.
  Changing the window exponent gives different filtering curves. Increasing 
the window exponent gives higher correction in the midrange. Typical values 
are between 0.7 and 1.2.
  

9  WFFilterLen
--------------
   
  Filter length, in taps, used to perform band splitting or sliding
lowpass prefiltering of the input signal. Higher values gives better
filter resolution but require a longer computation. Typical values for
band windowing are between 4096 and 65536. Sometimes may be useful to
use short filters (64 - 512 taps) to get a more --fuzzy-- correction at
lower frequencies.
  With the sliding lowpass procedure much shorter filters should be
used. Usually the filter length is in the 512 - 16384 range. Short
filters (16 - 64 taps) gives a similar fuzzy correction at the bottom
end, but with a different behaviour than band windowing.
  

10  WFFSharpness (*)
--------------------
   
  This parameter applies only to the sliding lowpass prefiltering
procedure and control the sharpness of the filtering performed in the
filtered region of the time-frequency plane. A value of 1.0 provides the 
maximum allowable filtering sharpness without affecting the direct sound, 
but also creates a substantial amount of spectral spreading in the filter
transition region of the time-frequency plane. Values above 1.0 increase
the spectral spreading up to a point where it starts affecting also the
direct sound, with the introduction of some ripple in the direct sound
itself. Values below 1.0 reduce the spectral spreading in the filtered
region at the expense of a little reduction in the filter sharpness.
Typical values for this parameter are between 0.1 and 0.75, with a
default value of 0.25.
  

11  WFBandSplit
---------------
   
  Fractional octave splitting of band windowing. Band windowing is
performed in 1 / WFBandSplit  of octave bands. Usual values are between
2 and 6. The higher this value the higher should be WFFilterLen. Values
greater than 6 usually give no improvements.
  For the sliding lowpass prefiltering this just gives the rate at which
log messages are reported during the prefiltering procedure and has no
effect on the prefiltering procedure itself, which is always stepless.
  
15  WFFinalWindow
-----------------
   
  Final window of the window filtering stage. Usually the same as
WFInitWindow or just something more. If set to 0 no windowing is
applied.
  
(*)  Parameters which are important for the quality of the generated
filters. 
"
%enddef

%module(docstring=WF_HELP) WFstage
%{
#include <cctype>
#include "WF_stage.h"
%}

//%feature("autodoc", WF_CONFIGURATION) WFParmsType;

%feature("autodoc",1);
%include typemaps.i
%include DRCbaseclass.i
%include WF_stage.h
