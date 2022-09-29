#include "fir_stl.h"

STLvectorReal STL_LowPassFir(unsigned int Order, DLReal Freq)
{
  DLReal *Filter = new DLReal[Order];
  LowPassFir(Filter, Order, Freq);
  STLvectorReal RetFilter(Filter, Filter + Order);
  return RetFilter;
}
STLvectorReal STL_HighPassFir(unsigned int Order, DLReal Freq)
{
  DLReal *Filter = new DLReal[Order];
  HighPassFir(Filter, Order, Freq);
  STLvectorReal RetFilter(Filter, Filter + Order);
  return RetFilter;
}
STLvectorReal STL_BandPassFir(unsigned int Order, DLReal Low, DLReal High)
{
  DLReal *Filter = new DLReal[Order];
  BandPassFir(Filter, Order, Low, High);
  STLvectorReal RetFilter(Filter, Filter + Order);
  return RetFilter;
}
STLvectorReal STL_BandStopFir(unsigned int Order,DLReal Low, DLReal High)
{
  DLReal *Filter = new DLReal[Order];
  BandStopFir(Filter, Order, Low, High);
  STLvectorReal RetFilter(Filter, Filter + Order);
  return RetFilter;
}
STLvectorReal STL_HilbertFir(unsigned int Order)
{
  DLReal *Filter = new DLReal[Order];
  HilbertFir(Filter, Order);
  STLvectorReal RetFilter(Filter, Filter + Order);
  return RetFilter;
}
STLvectorReal STL_GenericFir(unsigned int Order,STLvectorReal inF,
			     STLvectorReal inM, STLvectorReal inP,
			     unsigned int Is = 0, InterpolationType It = Linear)
{
  DLReal *Filter = new DLReal[Order];
  if (GenericFir(Filter, Order, inF.data(), inM.data(), inP.data(), inF.size(), Is, It) == true) {
    STLvectorReal RetFilter(Filter, Filter + Order);
    delete Filter;
    return RetFilter;
  } else {
    STLvectorReal RetFilter;
    delete Filter;
    return RetFilter;
  }
}
