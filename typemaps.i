%include "stl_type.h"
%{
#define SWIG_FILE_WITH_INIT
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
%}

%init %{

import_array();

%}

// INPUT: Python floating point conversion to DLReal (double) 
%typemap(in) DLReal {
    if(PyInt_Check($input))
    {
        $1 = (DLReal) PyInt_AsLong($input);
    }
    else if(!PyFloat_Check($input))
    {
        PyErr_SetString(PyExc_TypeError, "Expecting Python floating point object.");
        return NULL;
    }
    else
        $1 = PyFloat_AsDouble($input);
}

// OUTPUT: DLReal (double) type conversion to python floating point
%typemap(out) DLReal {
    $result = PyFloat_FromDouble((double)$1);
}

// OUTPUT: complex type conversion to python complex type
%typemap(out) complex<double> {
	$result = PyComplex_FromDouble((double)std::real($1),(double)std::imag($1));
}

// INPUT: Python floating point list or numpy array conversion to STL vector of DLReal (double)
%typemap(in) STLvectorReal {
    if(PyList_Check($input))
    {
        for (int cii=0; cii<PyList_Size($input); cii++)
        {
            if(PyInt_Check(PyList_GetItem($input,cii)))
            {
                $1.push_back((DLReal)PyInt_AsLong(PyList_GetItem($input,cii)));
    }
            else if(!PyFloat_Check(PyList_GetItem($input,cii))) 
            {
                PyErr_SetString(PyExc_TypeError, "Expecting Python floating point object as list elements.");
                return NULL;
            }
	    else
                $1.push_back(PyFloat_AsDouble(PyList_GetItem($input,cii)));
        }
    }
    else if(PyArray_Check($input))
    {
        PyArrayObject *temp_array;
        PyArg_Parse($input,(char*)"O",&temp_array);
        for (int cii=0; cii<PyArray_DIM(temp_array,0); cii++)
	  $1.push_back(*(double *)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0)));
	//Py_DECREF(temp_array);
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Expecting Numarray or Python list point object.");
        return NULL;
    }
}

%typemap(in) STLvectorReal *{
    if(PyList_Check($input))
    {
	$1 = new STLvectorReal(PyList_Size($input));
        for (int cii=0; cii<PyList_Size($input); cii++)
        {
            if(PyInt_Check(PyList_GetItem($input,cii)))
                $1->at(cii) = (DLReal)PyInt_AsLong(PyList_GetItem($input,cii));
            else if(!PyFloat_Check(PyList_GetItem($input,cii))) 
            {
                PyErr_SetString(PyExc_TypeError, "Expecting Python floating point object as list elements.");
                return NULL;
            }
	    else
                $1->at(cii) = PyFloat_AsDouble(PyList_GetItem($input,cii)) ;
        }
    }
    else if(PyArray_Check($input))
    {
	PyArrayObject *temp_array = NULL;	
        PyArg_Parse($input,(char*)"O",&temp_array);
	$1 = new STLvectorReal(PyArray_DIM(temp_array,0));
        for (int cii=0; cii<PyArray_DIM(temp_array,0); cii++) {
	  $1->at(cii) = *(double *)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0));
	}
	//Py_DECREF(temp_array);
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Expecting Numarray or Python list point object.");
        return NULL;
    }
}

%typemap(out) STLvectorReal *{
    PyArrayObject* temp_array;	
    //double *  pointer;
    npy_intp dims[1];
    dims[0] = $1->size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    //pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
      *(double *)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0)) = $1->at(cii);
    $result = PyArray_Return(temp_array);
}

%typemap(in) STLvectorReal &{
    if(PyList_Check($input))
    {
	$1 = new STLvectorReal(PyList_Size($input));
        for (int cii=0; cii<PyList_Size($input); cii++)
        {
            if(PyInt_Check(PyList_GetItem($input,cii)))
                $1->at(cii) = (DLReal)PyInt_AsLong(PyList_GetItem($input,cii));
            else if(!PyFloat_Check(PyList_GetItem($input,cii))) 
            {
                PyErr_SetString(PyExc_TypeError, "Expecting Python floating point object as list elements.");
                return NULL;
            }
	    else
                $1->at(cii) = PyFloat_AsDouble(PyList_GetItem($input,cii)) ;
        }
    }
    else if(PyArray_Check($input))
    {
	PyArrayObject *temp_array;
        PyArg_Parse($input,(char*)"O",&temp_array);
	$1 = new STLvectorReal(PyArray_DIM(temp_array,0));
        for (int cii=0; cii<PyArray_DIM(temp_array,0); cii++)
	  $1->at(cii) = *(double *)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0));
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "Expecting Numarray or Python list point object.");
        return NULL;
    }
}

%typemap(argout) STLvectorReal &ARef {
    PyArrayObject* temp_array;	
    double *  pointer;
    npy_intp dims[1];
    dims[0] = $1->size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	    *pointer++ = $1->at(cii);
    $result = PyArray_Return(temp_array);
}

%typemap(argout) (STLvectorReal& MPOut, STLvectorReal& EPOut) {

    PyArrayObject* mp_array, *ep_array;
    double*  pointer;	
    npy_intp dims1[1], dims2[1];
    
    dims1[0] = $1->size();
    dims2[0] = $2->size();

    mp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims1,NPY_DOUBLE);
    ep_array = (PyArrayObject*)PyArray_SimpleNew(1,dims2,NPY_DOUBLE);

    pointer = (double*)PyArray_DATA(mp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	 *pointer++ = $1->at(cii);

    pointer = (double*)PyArray_DATA(ep_array);;
    for (unsigned int cii=0; cii<$2->size();cii++)
	 *pointer++ = $2->at(cii);

    $result = PyList_New(0);
    PyList_Append($result,(PyObject*)mp_array);
    PyList_Append($result,(PyObject*)ep_array);
}


// Passing out a STL vector of DLReal (double) as reference
%typemap(argout) STLvectorReal &Sig {
    PyArrayObject* temp_array;	
    double*  pointer;
    npy_intp dims[1];

    dims[0] = $1->size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	*pointer++ = $1->at(cii);
    $result = PyArray_Return(temp_array);
}

%typemap(argout) (STLvectorReal& FilterFreq, STLvectorReal& FilterM, STLvectorReal& FilterP) {

    PyArrayObject* freq_array, *mag_array, *ph_array;
    double*  pointer;	
    npy_intp dims1[1], dims2[1], dims3[1];
    
    dims1[0] = $1->size();
    dims2[0] = $2->size();
    dims3[0] = $3->size();

    freq_array = (PyArrayObject*)PyArray_SimpleNew(1,dims1,NPY_DOUBLE);
    mag_array = (PyArrayObject*)PyArray_SimpleNew(1,dims2,NPY_DOUBLE);
    ph_array = (PyArrayObject*)PyArray_SimpleNew(1,dims3,NPY_DOUBLE);;

    pointer = (double*)PyArray_DATA(freq_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	 *pointer++ = $1->at(cii);

    pointer = (double*)PyArray_DATA(mag_array);;
    for (unsigned int cii=0; cii<$2->size();cii++)
	 *pointer++ = $2->at(cii);

    pointer = (double*)PyArray_DATA(ph_array);;
    for (unsigned int cii=0; cii<$3->size();cii++)
	*pointer++ = $3->at(cii);

    $result = PyList_New(0);
    PyList_Append($result,(PyObject*)freq_array);
    PyList_Append($result,(PyObject*)mag_array);
    PyList_Append($result,(PyObject*)ph_array);
}


// Passing out a STL vector of DLReal (double) as reference
%typemap(argout) STLvectorReal &Sig {
    PyArrayObject* temp_array;	
    double*  pointer;
    npy_intp dims[1];

    dims[0] = $1->size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	*pointer++ = $1->at(cii);
    $result = PyArray_Return(temp_array);
}

%typemap(argout) (STLvectorReal& FilterFreq, STLvectorReal& FilterM, STLvectorReal& FilterP) {

    PyArrayObject* freq_array, *mag_array, *ph_array;
    double*  pointer;	
    npy_intp dims1[1], dims2[1], dims3[1];
    
    dims1[0] = $1->size();
    dims2[0] = $2->size();
    dims3[0] = $3->size();

    freq_array = (PyArrayObject*)PyArray_SimpleNew(1,dims1,NPY_DOUBLE);
    mag_array = (PyArrayObject*)PyArray_SimpleNew(1,dims2,NPY_DOUBLE);
    ph_array = (PyArrayObject*)PyArray_SimpleNew(1,dims3,NPY_DOUBLE);;

    pointer = (double*)PyArray_DATA(freq_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	 *pointer++ = $1->at(cii);

    pointer = (double*)PyArray_DATA(mag_array);;
    for (unsigned int cii=0; cii<$2->size();cii++)
	 *pointer++ = $2->at(cii);

    pointer = (double*)PyArray_DATA(ph_array);;
    for (unsigned int cii=0; cii<$3->size();cii++)
	*pointer++ = $3->at(cii);

    $result = PyList_New(0);
    PyList_Append($result,(PyObject*)freq_array);
    PyList_Append($result,(PyObject*)mag_array);
    PyList_Append($result,(PyObject*)ph_array);
}


// Passing out a STL vector of DLReal (double) as reference
%typemap(argout) STLvectorReal &Sig {
    PyArrayObject* temp_array;	
    double*  pointer;
    npy_intp dims[1];

    dims[0] = $1->size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1->size();cii++)
	*pointer++ = $1->at(cii);
    $result = PyArray_Return(temp_array);
}

%typemap(out) STLvectorReal {
    PyArrayObject* temp_array;	
    double*  pointer;
    npy_intp dims[1];

    dims[0] = $1.size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_DOUBLE);
    pointer = (double*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1.size();cii++)
	*pointer++ = $1.at(cii);
    $result = PyArray_Return(temp_array);
 }

// Python floating point elements list type conversion to DLReal (double) array
// Conversion includes numpy arrays treated directly.
%typemap(in) DLReal* {
  if(PyList_Check($input)) {
    $1 = new DLReal(PyList_Size($input));	
    for (int cii=0; cii<PyList_Size($input); cii++) {
      if(!PyFloat_Check(PyList_GetItem($input,cii))) {
	PyErr_SetString(PyExc_TypeError, "Expecting Python floating point object as list elements.");
	return NULL;
      }
      $1[cii] = (PyFloat_AsDouble(PyList_GetItem($input,cii)));
    }
  } else if(PyArray_Check($input)) {
    PyArrayObject *temp_array;
    PyArg_Parse($input,(char*)"O",&temp_array);
    $1 = new DLReal(PyArray_DIM(temp_array,0));
    for (int cii=0; cii<PyArray_DIM(temp_array,0); cii++)
      $1[cii] = *(double *)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0));
    //Py_DECREF(temp_array);
  } else {
    PyErr_SetString(PyExc_TypeError, "Expecting Numarray or Python list point object.");
    return NULL;
  }
 }

// Python floating point elements list type conversion to STL vector of DLComplex (STL complex)
// Conversion includes numpy arrays treated directly.
%typemap(in) STLvectorComplex {
    if(PyList_Check($input)) {
      for (int cii=0; cii<PyList_Size($input); cii++) {
	if(!PyComplex_Check(PyList_GetItem($input,cii))) {
	  PyErr_SetString(PyExc_TypeError, "Expecting Python complex object as list elements.");
	  return NULL;
	}
	$1.push_back(std::complex<double>(PyComplex_RealAsDouble(PyList_GetItem($input,cii))
					  ,PyComplex_ImagAsDouble(PyList_GetItem($input,cii))));	
      }
    } else if(PyArray_Check($input)) {
      PyArrayObject* temp_array;
      npy_cdouble* temp_value;
      PyArg_Parse($input,(char*)"O",&temp_array);
      for (int cii=0; cii<PyArray_DIM(temp_array,0); cii++) {
	temp_value = (npy_cdouble*)(PyArray_DATA(temp_array) + cii*PyArray_STRIDE(temp_array,0));
	$1.push_back(std::complex<double>(temp_value->real,temp_value->imag));
      }
      //Py_DECREF(temp_array);
    } else {
      PyErr_SetString(PyExc_TypeError, "Expecting Numpy array or Python list point object.");
      return NULL;
    }
 }


// STL vector of DLComplex (STL complex) conversion to Python list of complex objects.
%typemap(out) STLvectorComplex {
    PyArrayObject* temp_array;	
    npy_cdouble* pointer;
    npy_intp dims[1];

    dims[0] = $1.size();
    temp_array = (PyArrayObject*)PyArray_SimpleNew(1,dims,NPY_CDOUBLE);
    pointer = (npy_cdouble*)PyArray_DATA(temp_array);
    for (unsigned int cii=0; cii<$1.size();cii++) {
      pointer->real = (double)std::real($1[cii]);
      pointer->imag = (double)std::imag($1[cii]);
      pointer++;
    }	
    $result = PyArray_Return(temp_array);
    //Py_DECREF(temp_array);
 }

// DRC IFiletype mapping from Python string type
%typemap(in) IFileType {
  Py_ssize_t size = PyUnicode_GetLength($input);
  char* ifile_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
  fflush(stdout);
  switch(ifile_type[0]) {
  case 'I':
    $1 = PcmInt16Bit;
    break;
  case 'F':
    $1 = PcmFloat32Bit;
    break;
  case 'D':
    $1 = PcmFloat64Bit;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "Undefined file data type (I, F or D)");
    return NULL;
    break; 
  }
 }

%typemap(in)  SLPPrefilteringType {
  Py_ssize_t size = PyUnicode_GetLength($input);
  char* prefilt_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
  if(prefilt_type[0] == 'P')
    $1 = SLPProportional;	
  else
    $1 = SLPBilinear;
 }

%typemap(in)  WindowType {
  Py_ssize_t size = PyUnicode_GetLength($input);
  char* win_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
  if(strcmp(win_type,"WFull")==0)
    $1 = WFull;
  else if(strcmp(win_type,"WRight")==0)
    $1 = WRight;
  else if(strcmp(win_type,"WLeft")==0)
    $1 = WLeft;
  else {
    PyErr_SetString(PyExc_TypeError, "Undefined  Window type (WFull, WRight or WLeft)");
    return NULL;  
  }
 }

%typemap(in) InterpolationType {
  Py_ssize_t size = PyUnicode_GetLength($input);
  char* int_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
  if(strcmp(int_type,"Linear")==0)
    $1 = Linear;
  else if(strcmp(int_type,"Logarithmic")==0)
    $1 = Logarithmic;
  else if(strcmp(int_type,"SplineLinear")==0)
    $1 = SplineLinear;
  else if(strcmp(int_type,"SplineLogarithmic")==0)
    $1 = SplineLogarithmic;
  else {
    PyErr_SetString(PyExc_TypeError, "Undefined  Interpolation type (Linear, Logarithmic, SplineLinear, SplineLogarithmic)");
    return NULL;  
  }
}

%typemap(in) NormType {
    Py_ssize_t size = PyUnicode_GetLength($input);
    char* norm_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
    switch(norm_type[0]) {
    case 'M':
      $1 = NormMax;
      break;
    case 'S':
      $1 = NormSum;
      break;
    case 'E':
      $1 = NormEuclidean;
      break;
    default:
      PyErr_SetString(PyExc_TypeError, "Undefined  Norm data type (M, S or E)");
      return NULL;
      break;         
    }
 }

%typemap(in) TFMagType {
  Py_ssize_t size = PyUnicode_GetLength($input);
  char* norm_type = (char*)PyUnicode_AsUTF8AndSize($input, &size);
  switch(norm_type[0]) {
  case 'L':
    $1 = MAGLinear;
    break;
  case 'D':
    $1 = MAGdB;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "Undefined TFMagType type (L or D)");
    return NULL;
    break;         
  }
 }

%typemap(in) int* {
	$1 = new int(PyInt_AsLong($input));
 }

%typemap(in) int& {
	$1 = new int(PyInt_AsLong($input));
 }

%typemap(argout) int& {
    PyObject *o;
    o = PyInt_FromLong(*$1);
    if ((!$result) || ($result == Py_None))  {
      $result = o;
    } else {
        if (!PyTuple_Check($result)) {
	  PyObject *o2 = $result;
	  $result = PyTuple_New(1);
	  PyTuple_SetItem($result,PyTuple_Size($result)-1,o2);
        }
        if (_PyTuple_Resize(&$result,PyTuple_Size($result)+1)!= -1) {
	  PyTuple_SetItem($result,PyTuple_Size($result)-1,o);	
        }
    }
 }
