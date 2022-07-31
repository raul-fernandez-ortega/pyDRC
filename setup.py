# Distutils installer for PyDRC

from distutils.core import setup, Extension
from distutils import sysconfig
import site

save_init_posix = sysconfig._init_posix

def my_init_posix():
    print('my_init_posix: changing gcc to g++')
    save_init_posix()
    g = sysconfig._config_vars
    g['CC'] = 'g++'
    #g['LDSHARED'] = 'g++ -shared -Xlinker'
    g['LDSHARED'] = 'g++ -pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions'
    g['CFLAGS']='-fno-strict-aliasing -DNDEBUG -O2 -Wall -Wno-maybe-uninitialized -Wno-pointer-arith'
    g['OPT']='-DNDEBUG -O2 -Wall -Wno-maybe-uninitialized -Wno-pointer-arith'

sysconfig._init_posix = my_init_posix

# Include site-packages directory
include_dirs = ["."]
for directory in site.getsitepackages():
    include_dirs.append(directory + '/numpy/core/include/')

setup(
    name = "pyDRC",
    version = "0.1",
    description = "Python bindings for Digital Room Correction module",
    author = "Raul Fernandez",
    author_email = "raul00fernandez@gmail.com",
    options={'build_ext':{'swig_opts':'-c++'}},
    py_modules = ['pyDRC'],
    #package_dir={'pyDRC':'.'},
    #packages =['pyDRC'],
    ext_modules = [Extension("_pyDRC", ["pyDRC.i",
                                        "fft_stl.cpp","fft.cpp",
                                        "fir_stl.cpp","fir.cpp",
                                        "spline.cpp","gammatone.cpp",
                                        "dspwind_stl.cpp","dspwind.cpp",
                                        "level_stl.cpp", "level.cpp",
                                        "baselib_stl.cpp", "baselib.cpp",
                                        "convol_stl.cpp",
                                        "convol.cpp", "baseclass.cpp",
                                        "hd_stl.cpp", "hd.cpp",
                                        "toeplitz.cpp", "kirkebyfd.cpp",
                                        "slprefilt_stl.cpp", "slprefilt.cpp",
                                        "bwprefilt_stl.cpp", "bwprefilt.cpp",
					"psychoacoustic.cpp",
                                        "BC_stage.cpp", "HD_stage.cpp", "WF_stage.cpp",
                                        "DL_stage.cpp", "HR_stage.cpp", "IS_stage.cpp",
					"PT_stage.cpp", "ER_stage.cpp", "PL_stage.cpp", 
                                        "TC_stage.cpp", "PS_stage.cpp", "MS_stage.cpp"
                                        ],
                             swig_opts=['-c++'], language='C++',
                             include_dirs=include_dirs,
                             libraries=["fftw3", "fftw3f", "m", "dl", "sndfile"]
                             )]
    )
