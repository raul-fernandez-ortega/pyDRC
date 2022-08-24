pyDRC: python bindings for Digital Room Correction

Copyright (C) 2002-2005 Denis Sbragion
Copyright (C) 2008 Raul Fernandez

This  program  is  free  software; you can redistribute it and/or
modify it under the terms of the GNU General  Public  License  as
published  by  the  Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will  be  useful,
but  WITHOUT  ANY  WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foun­
dation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

You can contact the author on Internet at the following address:

          matrixhifi@yahoo.es


General information
-------------------

pyDRC is a set of python bindings for DRC (Digital Room Correction) application 
that provides scripting access to several DRC functions and routines. DRC can 
be found at http://drc-fir.sourceforge.net.

pyDRC can be used to develop new DSP scripts that need algorithms such as:

- FIR filtering. 
- Cepstrum calculation.
- Frequency dependant time window filtering methods.
- Impulse inversion.

Combined with graphical libraries it can be used to develop custom GUI for DRC.  
During pyDRC development original DRC code has been splitted into several pieces, 
providing custom access to any DRC calculation stage. New data structures has been 
developed in order to provide python data communication between stages.

These different stages are:

Original DRC stage         pyDRC class     pyDRC configuration structure
-------------------------------------------------------------------------
BC stage                   BCstage         BCParmsType
HD stage                   HDstage         HDParmsType
MP stage                   WFstage         WFParmsType
DL stage                   DLstage         DLParmsType
EP stage                   WFstage         WFParmsType
HR stage                   HRstage         HRParmsType
IR stage                   IRstage         IRParmsType
PL stage                   PLstage         PLParmsType
RT stage                   WFstage         WFParmsType
PT stage                   PTstage         PTParmsType
PS stage                   PSstage         PSParmsType
MC stage                   PSstage         PSParmsType
MS stage                   MSstage         MSParmsType
TC stage                   TCstage         TCParmsType

DRC Data class (Impulse responses and filters container) is DRCSignal class.

pyDRC methods and classes are documented inside python script (with help pyDRC
command).

There are some simple tests scripts to show how to use pyDRC methods.

Included with pyDRC is drc.py, the python version of DRC that mantains the same 
features, input and configuration parameters and output messages format that
the original C++ program.

Linux installation
------------------

Required tools:

Python (>=2.4)         : http://www.python.org/download/
Swig                   : http://www.swig.org/

Installation command:

python setup.py install

Windows installation
--------------------

Required tools:

Python (>=2.4)         : http://www.python.org/download/
Swig                   : http://www.swig.org/
Mingw32(*)             : http://www.mingw.org/

(*) msys console and gcc compiler at least.

Before installation, swig executable must be included in PATH: 

 export PATH=/c/swigwin-1.3.31/:$PATH

Then proceed to installation with:

 /c/Python25/python.exe setup_windows.py build -c mingw32 install

Module import
-------------

import pyDRC

Module help
-----------

dir(pyDRC) 

provides modules content (functions and classes).

help(pyDRC)

provides help on all available pyDRC functions and classes.


      