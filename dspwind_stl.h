#ifndef DSPWind_stl_h
#define DSPWind_stl_h

#include "dspwind.h"
#include "stl_type.h"

void STL_HammingWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len);

void STL_HanningWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len);

void STL_BlackmanWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len);

void STL_BartlettWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len);

void STL_BlackmanHarrisWindow(STLvectorReal &ARef, unsigned int Start, unsigned int Len);	

void STL_ExponentialWindow(STLvectorReal &ARef, DLReal P, unsigned int Start, unsigned int Len);

void STL_SpacedBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType);
void STL_HalfBlackmanWindow(STLvectorReal &ARef, unsigned int Len, unsigned int Space, WindowType WType);

STLvectorReal STL_Ones(unsigned int Size);

STLvectorReal STL_Delta(unsigned int Size, unsigned int delay);

#endif
