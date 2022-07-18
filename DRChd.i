%module DRChd
%{
#include "hd_stl.h"
%}

%feature("autodoc",1);

%include typemaps.i
%include DRCbaseclass.i
%include stl_type.h
%include hd_stl.h

%feature("autodoc","
Homomorphic deconvolution (HD) process DRC class.
Attributes:
    - InSig:    Input signal as DRCSignal object.
    - MPSig:    Minimum phase after HD result. DRCSignal object.
    - EPSig:    Excess phase after HD result. DRCSignal object.
    - hd_start: InSig starting point for HD.
    - hd_len:   InSig length for HD.
    - MExp:     Exponent of the multiplier of the FFT size used to perform the HD.
                FFT length equals hd_len*2^MExp.
Methods:
    - HD(): Constructor.
      No parameters.
    - HD(DRCSignal InputSig, int start, int len, InMExp): Constructor.
      InputSig: input signal as DRCSignal class.
      start: starting point on InputSig.getData() for homomorphic deconvolution.
      len: len of InputSig.getData() for homomorphic deconvolution.
      InMExp: Multiplication exponent of homomorphic deconvolution process. 
    - SetInputSignal(InputSig, start, len):
      Changes InSig, hd_start, hd_len to InputSig, start and len.
    - SetMExp(InMExp):
      Sets MExp to InMExp. Multiplication exponent of homomorfic deconvolution.
      Minimun phase ans excess phase components will be len(InSig.getData())*2**MExp long.
    - process():
      performs homomorphic deconvolution. Before process MPSig and EPSig are empty.
    - getInSig():
      returns InSig. Input signal as DRCSignal class.
    - getMPSig():
      returns MPSig. Minimum phase component as DRCSignal class.
    - getEPSig():
      returns EPSig. Excess phase component as DRCSignal class.
") HD;
