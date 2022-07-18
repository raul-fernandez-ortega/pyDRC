%module DRCbwprefilt
%{
#include <cctype>
#include "bwprefilt_stl.h"
%}
%rename(BWPreFilt)  STL_BWPreFilt;
%include typemaps.i
%include bwprefilt_stl.h