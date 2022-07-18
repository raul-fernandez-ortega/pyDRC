%module DRCgammatone
%{
#include "gammatone.h"
%}
%include typemaps.i

%include gammatone.h
%pythoncode %{
def ERBGlasberg():
    erb = ERBdata()
    erb.EarQ = 9.26449
    erb.minBW = 24.7
    erb.Order = 1
    return erb

def ERBLyon():
    erb = ERBdata()
    erb.EarQ = 125.0
    erb.minBW = 8.0
    erb.Order = 2
    return erb

def ERBGreenwood():
    erb = ERBdata()
    erb.EarQ = 7.23824
    erb.minBW = 22.8509
    erb.Order = 1
    return erb

%}