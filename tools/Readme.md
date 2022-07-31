 # pyDRC tools
 
 ## 1. sweepgen.py
 
 sweepgen.py is a python script for sweep and inverse signals generation. Sweep signal should be used for measurement, while inverse signal is used for convolving with sweep acoustic measurements obtaining the impulse response we need for digital filtering/equalizacion.
 
 Script usage is simple:
 
 `python sweepgen.py sweep.xml [-s sweep_filename] [-i inverse_filename]`

sweep.xml is an xml file with sweep generation parameters. A sweep.xml sample is provided. Parameters:

- sample_rate: sweep wav sample rate in Hz
- Amplitude: sweep peak amplitude (max. value 1)
- Hzstart: sweep starting frequency in Hz
- HZend: sweep ending frequency in Hz
- length: sweep duration in seconds
- silence: intial and final silence length
- leadin: initial amplitude increase period
- leadout: final amplitude decrease period 

Output file names are defined by xml parameters or execution options.

sweep.xml sample:

```xml
<?xml version="1.0"?>
<generate_sweep>
  <params>
    <sample_rate>44100</sample_rate>
    <amplitude>0.5</amplitude>
    <Hzstart>20</Hzstart>
    <Hzend>20000</Hzend>
    <length>6</length>
    <silence>3</silence>
    <leadin>0.05</leadin>
    <leadout>0.005</leadout>
  </params>
  <sweep_filename>sweep_02.wav</sweep_filename>
  <inverse_filename>inverse_02.wav</inverse_filename>
</generate_sweep>
```

## 2.lsconv.py

This script convolves a measured sweep signal with teh corresponding inverse signal geneating the acoustic impulse response. Script usage is simple:

`python lsconv.py measurement.wav inverse.wav impulse.wav`

impulse.wav is the convolution result.


