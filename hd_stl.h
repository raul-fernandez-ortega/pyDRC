#ifndef Hd_stl_h
#define Hd_stl_h

#include "stl_type.h"
#include "hd.h"
#include "baseclass.h"
using namespace std;

bool STL_CepstrumHD(const STLvectorReal In, STLvectorReal& MPOut, STLvectorReal& EPOut, const int N, const int MExp);
bool STL_HilbertHD(const STLvectorReal In, STLvectorReal& MPOut, STLvectorReal& EPOut, const int N, const int MExp);

#endif
