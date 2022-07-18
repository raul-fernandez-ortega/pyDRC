%module DRCslprefilt
%{
#include <cctype>
#include "slprefilt_stl.h"
%}
%include typemaps.i
%rename(SLPreFilt) STL_SLPreFilt;
%include slprefilt_stl.h
