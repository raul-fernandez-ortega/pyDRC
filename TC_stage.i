%define TC_HELP
"
Test convolution Stage
======================

  In this stage, two signals are convolved. In DRC that operation is
perform in order to evaluate the filter efect on the initial impulse 
response.  
  TCParmsType contains the configuration parameters. 
  TCstage is the class that performs this stage.
  
"
%enddef

%define TC_CONFIGURATION
" 
Test Convolution parameters
===========================
  
  

1  TCNormFactor
---------------
   
  Normalization factor for the output of the final convolution stage.
Usually 1.0.
  

2  TCNormType
-------------
   
  Normalization type for the output of the final convolution stage.
Usually M.
  

3  TCOutFile
------------
   
  Output file for the final test convolution. If this is not supplied
the test convolution stage is skipped.
  

4  TCOutFileType
----------------
   
  Output type for the file above. D = Double, F = Float, I = Integer.
"
%enddef
%module(docstring=TC_HELP) TCstage
%{
#include <cctype>
#include "TC_stage.h"
%}
%feature("autodoc", TC_CONFIGURATION) BCParmsType;

%feature("autodoc",1);
%include typemaps.i
%include baseclass.h	
%include TC_stage.h