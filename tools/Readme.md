 # pyDRC tools
 
 ## 1. sweepgen.py
 
sweepgen.py is a python script for sweep and inverse signals generation. Sweep signal should be used for measurement, while inverse signal is used for convolving with sweep acoustic measurements obtaining the impulse response needed for digital filtering/equalizacion.
 
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

Output file names are defined by xml parameters or execution options. Execution options have preference.

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

The sweep file can be used for measurement, p.e. in [Audacity](https://www.audacityteam.org/)

## 2. lsconv.py

This script convolves a measured sweep signal with the corresponding inverse signal geneating the acoustic impulse response. Script usage is simple:

`python lsconv.py measurement.wav inverse.wav impulse.wav`

impulse.wav is the convolution result.

## 3. pca_analysis.py

One of the main issues of Digital Room Correction is that speaker equalization algorithms are based of one single point sweep measurement with an omnidirectional microphone. This restriction entails that Digital Room Correction can only be applied to one point in space. There are several proposal for calculating a new impulse response o new spectrum from more than one point in a room or space, trying to equalize a listening area. 

By using [Principal Component Analysis](https://en.wikipedia.org/wiki/Principal_component_analysis) (PCA) technique applied over several impulse responses measured inside a limited space in a room, this script computes a new impulse response than could be representative of a listening area.

This script is able to load several impulse responses, measured at different points in the room, and calculates a new impulse response which is the main component of a PCA performed over all the impulses responses. The loaded impulse response set should be measured around a small area of interest (p.e. about 0.5 x 0.5 meters around one or two listening points). 

Let's see a sample about how to use pca_analysis.py:

We have 8 different measurements over a typical listening area:

- Impulse response 1:

![Impulse 1](sample_plots/impulse_spectrum_impulse_sweep_left_01.wav.png)

- Impulse response 2:

![Impulse 2](sample_plots/impulse_spectrum_impulse_sweep_left_02.wav.png)

- Impulse response 3:

![Impulse 3](sample_plots/impulse_spectrum_impulse_sweep_left_03.wav.png)

- Impulse response 4:

![Impulse 4](sample_plots/impulse_spectrum_impulse_sweep_left_04.wav.png)

- Impulse response 5:

![Impulse 5](sample_plots/impulse_spectrum_impulse_sweep_left_05.wav.png)

- Impulse response 6:

![Impulse 6](sample_plots/impulse_spectrum_impulse_sweep_left_06.wav.png)

- Impulse response 7:

![Impulse 7](sample_plots/impulse_spectrum_impulse_sweep_left_07.wav.png)

- Impulse response 8:

![Impulse 8](sample_plots/impulse_spectrum_impulse_sweep_left_08.wav.png)

After running pca_analysis over this set of measurements, the PCA principal component is:

![PCA principal component](sample_plots/Spectrum_PCA_principal.png)

As can be seen, PCA removes high frequency component of acoustic reflexions, maintaining the speaker response contribution that is common to all measurements. The high frequency spectrum dispersion is strongly reduced, while keeping the low frequency room efects. Dip effects were reduced.

The frequency spectra of all the 8 components resulting of PCA analysis are (Hz in the x-axis, dB in the y-axis, each component spectrum is normalized to max. value):

![PCA component_spectra](sample_plots/spectrum_PCA.png)

pca_analysis.py Usage:

`python pca_analysis.py -i impulses_dir -p PCA_dir -a analysis_dir`

where:
- impulses_dir is a directory where all impulse response measurements are saved, each one is a wav format file. Every impulse should have the same sample rate.
- PCA_dir is a directory where all the components resulting of the PCA analysis will be saved after calculation. Principal component is PCA_1.wav. 
- analysis_dir is a directory where all the final impulse and spectrum plots are saved, for analysis purpose.

PCA_1.wav is the impulse response we can input to pyDRC for final DRC filter calculation.

## 4. Software needed

These scripts need the following python modules:

- pyDRC 
- numpy
- matplotlib (for analysis plots)

Currently (I don't know why) pca_anaylis.py only runs on python2.7, not in python 3.x. Still investigating the reasons behind...


