#include "fir_stl.h"

STLvectorReal STL_LowPassFir(unsigned int Order, DLReal Freq)
{
  unsigned int I;
  DLReal *Filter = new DLReal[Order];
  STLvectorReal RetFilter(Order);
  LowPassFir(Filter, Order, Freq);
  for(I = 0; I < Order; I++)
    RetFilter[I] = Filter[I];
  delete Filter;
  return RetFilter;
}
STLvectorReal STL_HighPassFir(unsigned int Order, DLReal Freq)
{
  unsigned int I;
  DLReal *Filter = new DLReal[Order];
  STLvectorReal RetFilter(Order);
  HighPassFir(Filter, Order, Freq);
  for(I = 0; I < Order; I++)
    RetFilter[I] = Filter[I];
  delete Filter;
  return RetFilter;
}
STLvectorReal STL_BandPassFir(unsigned int Order, DLReal Low, DLReal High)
{
  unsigned int I;
  DLReal *Filter = new DLReal[Order];
  STLvectorReal RetFilter(Order);
  BandPassFir(Filter, Order, Low, High);
  for(I = 0; I < Order; I++)
    RetFilter[I] = Filter[I];
  delete Filter;
  return RetFilter;
}
STLvectorReal STL_BandStopFir(unsigned int Order,DLReal Low, DLReal High)
{
  unsigned int I;
  DLReal *Filter = new DLReal[Order];
  STLvectorReal RetFilter(Order);
  BandStopFir(Filter, Order, Low, High);
  for(I = 0; I < Order; I++)
    RetFilter[I] = Filter[I];
  delete Filter;
  return RetFilter;
}
STLvectorReal STL_HilbertFir(unsigned int Order)
{
  unsigned int I;
  DLReal *Filter = new DLReal[Order];
  STLvectorReal RetFilter(Order);
  HilbertFir(Filter, Order);
  for(I = 0; I < Order; I++)
    RetFilter[I] = Filter[I];
  delete Filter;
  return RetFilter;
}
STLvectorReal STL_GenericFir(unsigned int Order,STLvectorReal inF,
			     STLvectorReal inM, STLvectorReal inP,
			     unsigned int Is = 0, InterpolationType It = Linear)
{
  unsigned int I, Np;
  Np = inF.size();
  DLReal *Filter = new DLReal[Order];
  DLReal *Fp = new DLReal[Np];
  DLReal *Mp = new DLReal[Np];
  DLReal *Pp = new DLReal[Np];

  STLvectorReal RetFilter(Order);
  for(I = 0; I < Np; I++)
    {
      Fp[I] = inF[I];
      Mp[I] = inM[I];
      Pp[I] = inP[I];
    }
  if (GenericFir(Filter, Order, Fp, Mp, Pp, Np, Is, It) == true)
    for(I = 0; I < Order; I++)
      RetFilter[I] = Filter[I];
  delete Filter;
  delete Fp;
  delete Mp;
  delete Pp;
  return RetFilter;
}
