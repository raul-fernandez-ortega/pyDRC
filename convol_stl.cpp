#include "convol_stl.h"

STLvectorReal *STL_Dir_Convolve(STLvectorReal *A, STLvectorReal *B)
{
  STLvectorReal *Convol =new STLvectorReal();
  int I,NA = A->size(), NB = B->size();
  DLReal *InA = new DLReal[NA];
  DLReal *InB = new DLReal[NB];
  DLReal *OutR = new DLReal[NA+NB+1];
  for(I = 0; I < NA; I++)
    InA[I] = A->at(I);
  for(I = 0; I < NB; I++)
    InB[I] = B->at(I);
  Convolve(InA, NA, InB, NB, OutR);
  for(I = 0; I < NA+NB+1; I++)
    Convol->push_back(OutR[I]);
  delete InA;
  delete InB;
  return Convol;
}

STLvectorReal *STL_Convolve(STLvectorReal *A, STLvectorReal *B)
{
  STLvectorReal *Convol = new STLvectorReal();
  int I,NA = A->size(), NB = B->size();
  DLReal *InA = new DLReal[NA];
  DLReal *InB = new DLReal[NB];
  DLReal *OutR = new DLReal[NA+NB+1];
  for(I = 0; I < NA; I++)
    InA[I] = A->at(I);
  for(I = 0; I < NB; I++)
    InB[I] = B->at(I);
  FftwConvolve(InA, NA, InB, NB, OutR);
  for(I = 0; I < NA+NB+1; I++)
    Convol->push_back(OutR[I]);
  delete InA;
  delete InB;
  return Convol;
}
STLvectorReal *STL_Convolve_part(STLvectorReal *A, unsigned int startA, unsigned int lenA,
				 STLvectorReal *B, unsigned int startB, unsigned int lenB)
{
  STLvectorReal *Convol= new STLvectorReal();
  unsigned int I, J;
  DLReal *InA = new DLReal[lenA];
  DLReal *InB = new DLReal[lenB];
  DLReal *OutR = new DLReal[lenA+lenB+1];
  for(J = 0, I = startA; J < lenA; J++, I++)
    InA[J] = A->at(I);
  for(J = 0, I = startB; J < lenB; J++, I++)
    InB[J] = B->at(I);
  FftwConvolve(InA, lenA, InB, lenB, OutR);
  for(I = 0; I < lenA +lenB + 1; I++)
    Convol->push_back(OutR[I]);
  delete OutR;
  return Convol;
}
