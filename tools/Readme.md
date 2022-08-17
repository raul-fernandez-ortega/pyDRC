 # pyDRC tools
 
 ## 1. sweepgen.py
 
sweepgen.py is a python script for sweep and inverse signals generation. Sweep signal should be used for measurement, while inverse signal is used for convolving with sweep acoustic measurements obtaining the impulse response needed for digital filtering/equalizacion.
 
 Script usage is simple:
 
 `python sweepgen.py sweep.xml [-s sweep_filename] [-i inverse_filename]`

sweep.xml is an xml file with sweep generation parameters. A sweep.xml sample is provided. Parameters:

- sample_rate: sweep wav sample rate in Hz
- amplitude: sweep peak amplitude (max. value 1)
- Hzstart: sweep starting frequency in Hz
- HZend: sweep ending frequency in Hz
- length: sweep duration in seconds
- silence: intial and final silence length
- leadin: initial amplitude increase period (fraction of total)
- leadout: final amplitude decrease period (fraction of total)

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

Using [ecasound](https://ecasound.seul.org/) for measurements:

ecasound -t:**<sweep_time>** -a:1 -i sweep.wav -a:1 -o:jack_auto,system:**<playback_ch>** -a:2 -i:jack_auto,system:**<capture_ch>** -a:2 -f:f32_le,1,**<sample_rate>** -o:**<measurement.wav>** -a:2 -ev

Where:
 - sweep_time: total sweep duration (s)
 - playback_ch: jackd device output channel selected
 - capture_ch: jackd microphone channel
 - sample_rate: sweep and jackd sample rate (Hz)
 - measurement.wav: wav format file name for saving recording
 
 This process is done in two chains, one for playback of sweep and another for recording from microphone.

## 2. lsconv.py

This script convolves a measured sweep signal with the corresponding inverse signal geneating the acoustic impulse response. Script usage is simple:

`python lsconv.py measurement.wav inverse.wav impulse.wav`

impulse.wav is the convolution result.

## 3. pca_analysis.py

One of the main problems of Digital Room Correction is that speaker equalization algorithms are based on one single point sweep measurement with an omnidirectional microphone. This restriction entails that Digital Room Correction can only be applied to one point in space. There are several proposal for calculating a new impulse response o new spectrum from more than one point in a room or space, trying to equalize a listening area. 

By using [Principal Component Analysis](https://en.wikipedia.org/wiki/Principal_component_analysis) (PCA) technique applied over several impulse responses measured inside a limited space in a room, this script computes a new single impulse response than is representative of a listening area. PCA is a statistical technique used for dimension reduction of a problem, trying to keep the maximun information in the reduced solution. In our case we reduce several impulse responses to a single one, the principal component. This asssumption will work if all the impulse responses are strongly correlated. The main source of correlation among our original impulse responses is that the same speaker is used in all the process, hence the direct speaker contribution will appear on the principal component resulted from PCA. The different acoustic contributions from room reflections have low correlation at high frequencies but this correlation factor increase as frequency decrease. This correlation is a function of the distance between the different recorded points. As equalization area increases, correlation between measurements decreases, therefore the equalization is less effective. 

This script is able to load several impulse responses, measured at different points in the room, and calculates a new impulse response which is the main component of a PCA performed over all the impulses responses. The loaded impulse response set should be measured around a small area of interest (p.e. about 0.5 x 0.5 meters around one or two listening points). 

Let's see a sample about how to use pca_analysis.py:

We have 16 different measurements over a typical listening area:

Impulse responses:

![Impulse 1](aux_plots/no_eq_left/impulse_spectrum_0.png) ![Impulse 2](aux_plots/no_eq_left/impulse_spectrum_1.png) 
![Impulse 3](aux_plots/no_eq_left/impulse_spectrum_2.png) ![Impulse 4](aux_plots/no_eq_left/impulse_spectrum_3.png) 
![Impulse 5](aux_plots/no_eq_left/impulse_spectrum_4.png) ![Impulse 6](aux_plots/no_eq_left/impulse_spectrum_5.png) 
![Impulse 7](aux_plots/no_eq_left/impulse_spectrum_6.png) ![Impulse 8](aux_plots/no_eq_left/impulse_spectrum_7.png) 

After running pca_analysis over this set of measurements, the PCA principal component is:

![PCA principal component](aux_plots/no_eq_left/Spectrum_PCA_principal.png)

As can be seen, PCA removes high frequency component of acoustic reflexions, maintaining the speaker response contribution that is common to all measurements. The high frequency spectrum dispersion is strongly reduced, while keeping the low frequency room efects. Dip effects were reduced.

The frequency spectra of all the 16 components resulting of PCA analysis are (Hz in the x-axis, dB in the y-axis, each component spectrum is normalized to max. value):

![Impulse 1](aux_plots/no_eq_left/PCA_spectrum_0.png)
![Impulse 2](aux_plots/no_eq_left/PCA_spectrum_1.png)
![Impulse 3](aux_plots/no_eq_left/PCA_spectrum_2.png)
![Impulse 4](aux_plots/no_eq_left/PCA_spectrum_3.png)
![Impulse 5](aux_plots/no_eq_left/PCA_spectrum_4.png)
![Impulse 6](aux_plots/no_eq_left/PCA_spectrum_5.png)
![Impulse 7](aux_plots/no_eq_left/PCA_spectrum_6.png)
![Impulse 8](aux_plots/no_eq_left/PCA_spectrum_7.png)

After calculating equalizating using pyDRC, and applying to the audio system, we measure again another 16 points in the listening area, to see the equalization results. These points are not exactly the same as in the first step, but in the same area.

Impulse responses:

![Impulse 1](aux_plots/eq_left/impulse_spectrum_0.png) ![Impulse 2](aux_plots/eq_left/impulse_spectrum_1.png) 
![Impulse 3](aux_plots/eq_left/impulse_spectrum_2.png) ![Impulse 4](aux_plots/eq_left/impulse_spectrum_3.png) 
![Impulse 5](aux_plots/eq_left/impulse_spectrum_4.png) ![Impulse 6](aux_plots/eq_left/impulse_spectrum_5.png) 
![Impulse 7](aux_plots/eq_left/impulse_spectrum_6.png) ![Impulse 8](aux_plots/eq_left/impulse_spectrum_7.png) 

The spectra are flatten than before equalization. After running pca_analysis over the new set, the PCA principal component is:

![PCA principal component](aux_plots/eq_left/Spectrum_PCA_principal.png)

which is much more flatter than the initial principal component result.

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


