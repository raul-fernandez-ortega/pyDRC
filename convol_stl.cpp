#include "convol_stl.h"

STLvectorReal STL_Dir_Convolve(const STLvectorReal A, const STLvectorReal B)
{
  DLReal *OutR = new DLReal[A.size() + B.size() + 1];
  Convolve(A.data(),A.size(), B.data(), B.size(), OutR);
  STLvectorReal Convol(OutR, OutR + A.size() + B.size() + 1);
  delete OutR;
  return Convol;
  /*STLvectorReal Convol;
  int I,NA = A.size(), NB = B.size();
  DLReal *InA = new DLReal[NA];
  DLReal *InB = new DLReal[NB];
  DLReal *OutR = new DLReal[NA+NB+1];
  for(I = 0; I < NA; I++)
    InA[I] = A[I];
  for(I = 0; I < NB; I++)
    InB[I] = B[I];
  Convolve(InA, NA, InB, NB, OutR);
  for(I = 0; I < NA+NB+1; I++)
    Convol.push_back(OutR[I]);
  delete InA;
  delete InB;
  return Convol;*/
}

STLvectorReal STL_Convolve(const STLvectorReal A, const STLvectorReal B)
{
  DLReal *OutR = new DLReal[A.size() + B.size() + 1];
  FftwConvolve(A.data(), A.size(), B.data(), B.size(), OutR);
  STLvectorReal Convol(OutR, OutR + A.size() + B.size() + 1);
  delete OutR;
  return Convol;
  /*STLvectorReal Convol;
  int I,NA = A.size(), NB = B.size();
  DLReal *InA = new DLReal[NA];
  DLReal *InB = new DLReal[NB];
  DLReal *OutR = new DLReal[NA+NB+1];
  for(I = 0; I < NA; I++)
    InA[I] = A[I];
  for(I = 0; I < NB; I++)
    InB[I] = B[I];
  FftwConvolve(InA, NA, InB, NB, OutR);
  for(I = 0; I < NA+NB+1; I++)
    Convol.push_back(OutR[I]);
  delete InA;
  delete InB;
  return Convol;*/
}
STLvectorReal STL_Convolve_part(const STLvectorReal A, unsigned int startA, unsigned int lenA,
				const STLvectorReal B, unsigned int startB, unsigned int lenB)
{
  DLReal *OutR = new DLReal[lenA + lenB + 1];
  Convolve(&A.data()[startA], lenA, &B.data()[startB], lenB, OutR);
  STLvectorReal Convol(OutR, OutR + lenA + lenB + 1);
  delete OutR;
  return Convol;
  /*
  STLvectorReal Convol;
  unsigned int I, J;
  DLReal *InA = new DLReal[lenA];
  DLReal *InB = new DLReal[lenB];
  DLReal *OutR = new DLReal[lenA+lenB+1];
  for(J = 0, I = startA; J < lenA; J++, I++)
    InA[J] = A[I];
  for(J = 0, I = startB; J < lenB; J++, I++)
    InB[J] = B[I];
  FftwConvolve(InA, lenA, InB, lenB, OutR);
  for(I = 0; I < lenA +lenB + 1; I++)
    Convol.push_back(OutR[I]);
  delete OutR;
  return Convol;*/
}
