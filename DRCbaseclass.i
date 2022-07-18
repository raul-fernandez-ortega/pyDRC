%module DRCbaseclass
%{
#include "baseclass.h"
#include "slprefilt_stl.h"
%}
%include typemaps.i

%rename(ToeplitzSolve)            STL_ToeplitzSolve;
%rename(KirkebyFDInvert)          STL_KirkebyFDInvert;
%rename(PEISMPKirkebyFDInvert)    STL_PEISMPKirkebyFDInvert;
%rename(PETFDInvert)              STL_PETFDInvert;
%rename(SLPreFilt)                STL_SLPreFilt;

%include baseclass.h
%include slprefilt_stl.h

%feature("autodoc", "
Class DRCSignal: DRC structure and methods associated to an impulse response signal.
Attributes:
   - Data:          impulse response data vector. Impulse peak is centered.
   - PreWindowLen:  impulse pre-peak window length 
   - PSStart:       Pre-spike start. Usually PSStart = 0.
   - PSEnd:         Post-spike end. Usually PSEnd = len(Data).
   - WStart:        Required window starting point.
   - WLen:          Required window ending point.
   - SampleRate:    Signal sample rate.
   - Path:          File path for DRCsignal Data storage.

	(data length)/2                  Peak         (data length)/2      
|<----------------------------------------|---------------------------------------->|
|                                                                                   |
|                                                                                   |
|    leading zeros                PreWindowLen                                      |
|<--------------------------------->|<--->|                                         |
|                                                                                   |
|        WStart                                   WLen                              |
|<------------------------------->|<---------------------------------------->|      |

DRCSignal is the pyDRC class containing impulse response as readed and processed by BCStage.
The Data numarray is loaded from a headerless raw format file and, usually, after BCStage, it is 
centered at peak value.
If data file lenght from begining to peak value position is less than half the required Data 
lenght, the necessary leading zeros are inserted at the begining and PSStart stores this new data segment
lenght. PSEnd is the real amount of data readed from input file. Usually it is equal to Data numarray lenght.
WStart and WLen are pointers to begining and end of data subsegment of interest (by any reason) inside Data numarray. 

Methods:
   - DRCSignal(): Basic constructor.

   - DRCSignal(SCopy): Copy constructor from DRCSignal SCopy.

   - setParams(SCopy): Copy everything but Data.

   - setData(InData, psstart, psend): Sets InData as Data, PSStart as psstart and PSEnd 
                    as psend.
 
   - setPreWindowLen(prewindowlen):
   - setPSStart(int psstart):
   - setPSEnd(int psend):
   - setWStart(int wstart):
   - setWLen(int wlen):
   - setSampleRate(int srate):
   - setPath(char *path):

        Sets each DRCSignal member to input value.
 
   - getData():
   - getPreWindowLen():
   - getPSStart():
   - getPSEnd():
   - getWStart():
   - getWLen():
   - getSampleRate():
   - getPath():

	Returns each DRCSignal member value.

   - Normalize(normfactor, normtype): Normalized Data to normfactor. normtype can be 'M' (max value),'S' (sum value) or 'E' (euclidean).
  
   - WriteSignal(outfile, outfiletype): Stores data to outfile as outfiletype: 'F' (float), 'D' (double precision), 'I' (integer). Outfile will be created on indicated DRCSignal attribute Path.
") DRCSignal;

%feature("autodoc", "
ToeplitzSolve(const STLvectorReal A, const STLvectorReal B, unsigned int N) --> STLvectorReal
Solves equation system:

	AX = B

where A is a Toeplitz symmetric matrix of N dimensions. Given Toeplitz matrix symetry, A input
vector is the first matrix column. The solving process is done by using Levinson method.""") ToeplitzSolve;

%feature("autodoc", """
KirkebyFDInvert(const STLvectorReal InSig, unsigned int InSigLen, 
		unsigned int InvFilterSigLen, const STLvectorReal EffortSig, 
		DLReal EffortFactor, int MExp) --> STLvectorReal
Inversion by Kirkeby Fast Deconvolution.
") KirkebyFDInvert;

%feature("autodoc", "
PEISMPKirkebyFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig, 
		      unsigned int InvFilterSigLen, DLReal EffortFactor, 
		      int MExp) --> STLvectorReal 
") PEISMPKirkebyFDInvert;

%feature("autodoc", "
PEMSMPKirkebyFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig,
		      unsigned int InvFilterLen, DLReal EffortFactor, 
		      int PEStart, int PETransition, int MExp) --> STLvectorReal 
") PEMSMPKirkebyFDInvert;

%feature("autodoc", "
PETFDInvert(const STLvectorReal MPSig, const STLvectorReal EPSig, 
			      unsigned int InvFilterLen, char PEType, int PELowerWindow, 
			      int PEUpperWindow, int PEStartFreq, int PEEndFreq, 
			      int PEFilterLen, DLReal FSharpness, int PEBandSplit, 
			      DLReal PEWindowExponent,const DLReal OGainFactor, int SampleRate, 
			      int MExp, const SLPPrefilteringType SLPType) --> STLvectorReal
") PETFDInvert;

