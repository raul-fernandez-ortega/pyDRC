%module(package="DRCbaselib") DRCbaselib
%{
#include "baselib_stl.h"
%}
%pythoncode %{

import numpy as np
  
def ReadSignal(FName,InitWindow,ImpulseCenter):
  return _pyDRC.STL_SND_ReadSignal(FName,InitWindow,ImpulseCenter,0,0)

def ReadPoints(CorrFile, MagType, NPoints, SampleRate):
  Freq = np.array([],np.float32)
  Mag = np.array([],np.float32)
  Phase = np.array([],np.float32)
  Len = _pyDRC.FLineCount(CorrFile)
  return _pyDRC.STL_ReadPoints(CorrFile, MagType ,Freq, Mag, Phase, Len, SampleRate)
    
%}
%rename(WriteSignal)          SND_WriteSignal(const char * FName,const STLvectorReal Src,const unsigned int WStart,
					      const unsigned int WLen, const int SampleRate, const IFileType FType);
%rename(OverwriteSignal)      STL_OverwriteSignal;
%rename(AutoCorrelation)      STL_AutoCorrelation;
%rename(CrossCorrelation)     STL_CrossCorrelation;
%rename(GroupDelay)           STL_GroupDelay;
%rename(FSize)		      SND_FSize;
%rename(GetSampleRate)	      SND_GetSampleRate;
%rename(FindMaxPcm)           SND_FindMaxPcm;
%include typemaps.i
%include baselib_stl.h

%feature("autodoc", "
ReadSignal(const char* FName,const int InitWindow, const int ImpulseCenter,
	   const IFileType FType) --> STLvectorReal
Reads signal data in raw format from FName file. Returning numarray of InitWindow lenght, 
centered on ImpulseCenter.
Input parameters:
    FName:         Data file name.
    InitWindow:    Signal data length. If it is larger than file length, the necessary leading 
                   and ending zeros will be added.
    ImpulseCenter: Center of signal numarray, usually in DRC the peak point of FName data.
    FType:         Data type: PcmInt16Bit = 'I', PcmFloat32Bit = 'F', PcmFloat64Bit = 'D' 
Returned object: python tuple with three elements: python numarray of data, leading zeros added,
                 real data length readed.
") STL_ReadSignal;

%feature("autodoc", "
WriteSignal(const char * FName,const STLvectorReal Src, const unsigned int WStart, 
            const unsigned int WLen, const IFileType FType) --> bool
Writes signal data to a raw format FName file.
Input parameters:
    FName:         Output data file name.
    Src:           Signal data to be written. Python numarray format. 
    WStart:        Begining of data inside Src.
    WLen:          Lenght of data inside Src.
    FType:         Data type: PcmInt16Bit = 'I', PcmFloat32Bit = 'F', PcmFloat64Bit = 'D' 
Returns success (True) of failure (False) of writing proccess.
") STL_WriteSignal;

%feature("autodoc", "
ReadPoints(char* CorrFile, const TFMagType MagType, int SampleRate) --> 
                   STLvectorReal, STLvectorReal, STLvectorReal 
Read a CorrFile data file and loads data into three numarrays: frequency, magnitude, phase.
Input parameters:
    CorrFile:      Data file name. It should cotain at least 0 and half sample rate frequency data.
    MagType:       Magnitude dimensions. 'L' = Linear, 'D' = dB
    SampleRate:    Sample rate.
Return three numarrays with data loaded: frequency (normalized), magnitude and phase.
") STL_ReadPoints;

%feature("autodoc", "
FindMaxPcm(const char * FName, const IFileType FType) --> int
Returns peak value position in FName.
Input parameters:
    FName:         Input data file name.
    FType:         Data type: PcmInt16Bit = 'I', PcmFloat32Bit = 'F', PcmFloat64Bit = 'D' 
Return peak position in file data stream (integer).
") STL_FindMaxPcm;



