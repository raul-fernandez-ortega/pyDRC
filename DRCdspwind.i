%define DSPWIND_HELP
"dspwind module methods:
    * HammingWindow:
    * HanningWindow:
    * BlackmanWindow:
    * BartlettWindow:
    * BlackmanHarrisWindow:

            These five methods return the given python numarray (ARef) modified by the desired window.
            Input parameters:
               - ARef: numarray input signal.
               - Start: windowing starting point.
               - Len: windowing len.
            These methods don't check ARef length, Start and Len input consistency.
            It returns the ARef weighted signal. 

* ExponentialWindow:

	Exponential window weighting.
        Input parameters:
           - ARef: numarray input signal.
           - Start: windowing starting point.
           - Len: windowing len.
	   - P: coefficient of the exponential term of teh window. 
         This method doesn't check ARef length, Start and Len input consistency.
         It returns the ARef weighted signal. 

* SpacedBlackmanWindow:

	Spaced Blackman window weighting.
	    - ARef: numarray input signal.
            - Len: window len.
            - Space: leading space before windowing.
            - WindowType: valid values WRight, WFull or WLeft.
         This method doesn't check ARef length, Start and Len input consistency.
         It returns the ARef weighted signal. 

* HalfBlackmanWindow:

         Half Blackman window weigthing.
	    - ARef: numarray input signal.
            - Len: window len.
            - Space: leading space before windowing.
            - WindowType: valid values WRight or WLeft.
         This method doesn't check ARef length, Start and Len input consistency.
         It returns the ARef weighted signal. 

* Ones:
         Creates a python numarray of ones with a given length. 

* Delta:
	 Creates a python numarray with a delta function of given size and given delay.

"
%enddef

%module(docstring=DSPWIND_HELP) DRCdspwind
%{
#include "dspwind_stl.h"
%}

%include typemaps.i
%include stl_type.h

%feature("autodoc",1);

%rename(HammingWindow)        STL_HammingWindow;
%rename(HanningWindow)        STL_HanningWindow;
%rename(BlackmanWindow)       STL_BlackmanWindow;
%rename(BartlettWindow)       STL_BartlettWindow;
%rename(BlackmanHarrisWindow) STL_BlackmanHarrisWindow;
%rename(ExponentialWindow)    STL_ExponentialWindow;
%rename(SpacedBlackmanWindow) STL_SpacedBlackmanWindow;
%rename(HalfBlackmanWindow)   STL_HalfBlackmanWindow;
%rename(Ones)                 STL_Ones;
%rename(Delta)                STL_Delta;

%include dspwind_stl.h

%feature("autodoc", "
STL_HammingWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len) --> STLvectorReal
Weights a ARef array by a Hamming window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_HammingWindow;

%feature("autodoc", "
STL_HanningWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len) --> STLvectorReal
Weights a ARef array by a Hanning window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_HanningWindow;

%feature("autodoc", "
STL_BlackmanWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len) --> STLvectorReal
Weights a ARef array by a Blackman window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_BlackmanWindow;

%feature("autodoc", "
STL_BarlettWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len) --> STLvectorReal
Weights a ARef array by a Barlett window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_BarlettWindow;

%feature("autodoc", "
STL_BlackmanHarrisWindow(STLvectorReal *ARef, unsigned int Start, unsigned int Len) --> STLvectorReal
Weights a ARef array by a Blackman-Harris window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_BlackmanHarrisWindow;

%feature("autodoc", "
STL_ExponentialWindow(STLvectorReal *ARef, DLReal P, unsigned int Start, unsigned int Len) 
                      --> STLvectorReal
Weights a ARef array by an Gauss window of Len lenght, beginning at Start point.)
Input parameters:
    ARef:          Signal numpy array.
    P:             gauss window exponent.         
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_ExponentialWindow;

%feature("autodoc", "
STL_SpacedBlackmanWindow(STLvectorReal *ARef, unsigned int Len, unsigned int Space, WindowType WType)
                          --> STLvectorReal
Weights a ARef array by a Blackman window of Len lenght, beginning at Start point)
Input parameters:
    ARef:          Signal numpy array.
    Start:         Window starting point.
    Len:           Window length. 
Returned object: weighted data.
") STL_SpacedBlackmanWindow;

