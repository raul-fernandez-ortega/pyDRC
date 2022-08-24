%define BC_HELP
"
Base Stage
==========

  In this stage, the main part of impulse response is extracted for 
following analysis. 
  BCParmsType stores the configuration parameters needed. 
  BCstage is the class that performs this stage.
  
"
%enddef

%define BC_CONFIGURATION
" 
Base Stage parameters
=====================
  
1  BCBaseDir
----------------
   
  This parameter define the base directory that is prepended to all file
parameters, like for example BCInFile, HDMPOutFile or PSPointsFile. This
parameter allow the implicit definition of a library directory where all
DRC support file might be placed.
  File parameters supplied on the command line are not affected by this
parameter unless the BCBaseDir parameter is also supplied on the command
line. File parameters supplied in the configuration file are instead
always affected by the BCBaseDir parameter, no matter if it has been
supplied in the configuration file or on the command line.
  

2  BCInFile
---------------
   
  Just the name of the input file with the input room impulse response.
  

3  BCInFileType
-------------------
   
  The type of the input file. D = Double, F = Float, I = Integer.
  

4  BCSampleRate
-------------------
   
  The sample rate of the input file. Usually 44100 or 48000.
  

5  BCImpulseCenterMode
--------------------------
   
  The impulse response impulse center may be set manually using the
BCImpulseCenter parameter or you may ask DRC to try to find it
automatically. If BCImpulseCenterMode is set to A DRC will look for the
impulse center within the input file. If BCImpulseCenterMode is set to M
DRC uses the impulse center supplied with the BCImpulseCenter parameter.
  Be careful when using automatic impulse center recognition. Strong
reflections or weird situations may easily fool the simple procedure
used by DRC, which simply looks for the sample with the maximum absolute
amplitude.
  

6  BCImpulseCenter (*)
--------------------------
   
  This is the position in samples of the time axis zero of the impulse
response read from BCInFile. Usually this is where the peak of the
impulses is, but for complex situations it might not be easy to identify
where the zero is. Even few samples displacement in this parameter may
cause high frequency overcorrection, causing too bright sound. If
BCImpulseCenterMode is set to A this parameter is ignored.
  

7  BCInitWindow (*)
-----------------------
   
  Initial portion of the impulse response which is used to perform the
correction. The longer the portion used the better the correction, but
you get also greater sensibility to the listening position. The window
is symmetrical with respect of the impulse center. If needed, the signal
is padded with zeroes. Usual values are between 16384 and 131072,
depending on the values of the parameters for the subsequent steps. This
initial window may be further limited in subsequent steps, which sets
the real window used.
  

8  BCPreWindowLen
---------------------
   
  This the length of the window used to remove any noise coming before
the impulse center. This is usually just few samples, with a typical
value of 256 sample, corresponding to 5.8 ms at 44.1 KHz sample rate. If
this value is 0 this step is skipped
  

9  BCPreWindowGap
---------------------
   
  This is the central flat gap left in the previous windowing operation.
Considering that the previous window is rather small it is important to
leave a small flat gap in the window to avoid touching the main spike in
the case where the impulse response center is slightly misaligned.
Typical values are between 32 and 128 samples.
  

10  BCNormFactor
--------------------
   
  Initial normalization of the input impulse response. This is usually
set to 1.0 to reduce small errors in subsequent computations. 0 means no
normalization.
  

11  BCNormType
------------------
   
  Type of normalization applied. M means max normalization, i.e. the
input signal is rescaled so that the maximum value of the samples is
equal to the normalization factor. E means Euclidean normalization (L2
Norm), i.e. the input signal is rescaled so that the RMS value of the
signal is equal to the normalization factor. S means sum normalization
(L1 Norm), i.e. the input signal is rescaled so that the sum of the
absolute values of the samples is equal to the normalization factor.

(*)  Parameters which are important for the quality of the generated
filters.
"
%enddef

%module(docstring=BC_HELP) BCstage
%{
#include "BC_stage.h"
%}

%feature("autodoc", BC_CONFIGURATION) BCParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h
%include BC_stage.h

