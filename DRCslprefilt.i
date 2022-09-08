%module DRCslprefilt
%{
#include <cctype>
#include "slprefilt_stl.h"
%}
%include typemaps.i
%rename(FastLowPassFir) STL_FastLowPassFir;
%rename(SLPreFilt) STL_SLPreFilt;
%include slprefilt_stl.h
