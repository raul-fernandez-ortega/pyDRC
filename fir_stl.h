#ifndef Fir_stl_h
#define Fir_stl_h

#include "stl_type.h"
#include "fir.h"

STLvectorReal STL_LowPassFir(unsigned int Order, DLReal Freq);

STLvectorReal STL_HighPassFir(unsigned int Order, DLReal Freq);

STLvectorReal STL_BandPassFir(unsigned int Order, DLReal Low, DLReal High);

STLvectorReal STL_BandStopFir(unsigned int Order,DLReal Low, DLReal High);

STLvectorReal STL_HilbertFir(unsigned int Order);

STLvectorReal STL_GenericFir(unsigned int Order, STLvectorReal inF, STLvectorReal inM, STLvectorReal inP, unsigned int Is, InterpolationType);

#endif
