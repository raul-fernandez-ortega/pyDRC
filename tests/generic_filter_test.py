# /usr/bin/env python

import wave
import pyDRC
import numpy as np
import matplotlib
import matplotlib.pyplot as plt


PLOTS_DIR = "./plots"

matplotlib.use('Cairo')

# Filters at 44.1 kHz
sample_rate = 44100
half_srate = sample_rate/2

# Filter lenght: 65534 taps
filter_len = 2**16

# Array of frequency points. First point must be zero, and last one half sample rate
freqarray = np.array([0,18,19,20,350,400,450,19900,20000,20500,22050])
freqarray = freqarray/float(half_srate)
amplarray = np.array([-90.0,0.0,0.0,0.0,0.0,0.0,0.0,-5.95,-6.0,-6.0,-90.0])
amplarray = 10**(amplarray/20.)
phasearray = np.zeros(len(freqarray),np.int32)
t = np.linspace(0, filter_len, filter_len)/sample_rate

# Generic Filter
print("Generic filter")
filter_gen = pyDRC.SigNormalize(
    pyDRC.HanningWindow(pyDRC.GenericFir(filter_len, freqarray, amplarray,phasearray, 0, 'SplineLogarithmic'),0,filter_len),1,'M')
pyDRC.WriteSignal("genericfilterfir.wav",filter_gen,0,len(filter_gen),sample_rate,'F')
plt.rcParams["figure.figsize"] = (8,8)
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_gen, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='Generic Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(pyDRC.Fft(filter_gen)))
f = np.linspace(0, sample_rate, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Generic Filter')
ax[1].set_xlim(xmin=1,xmax=22050)
ax[1].set_ylim(ymin=-90)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/genericfir.png"%PLOTS_DIR)


# Hilbert filter of filter_len taps
print("Hilbert filter")
filter_hilbert = pyDRC.SigNormalize(pyDRC.HilbertFir(filter_len),1,'M')
pyDRC.WriteSignal("hilbertfilterfir.wav",filter_hilbert,0,len(filter_hilbert),sample_rate,'F')
plt.rcParams["figure.figsize"] = (8,8)
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_hilbert, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='Hilbert Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(pyDRC.Fft(filter_hilbert)))
f = np.linspace(0, sample_rate, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Hilbert Filter')
ax[1].set_xlim(xmin=1,xmax=22050)
ax[1].set_ylim(ymin=-60,ymax=-30)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/hilbertfir.png"%PLOTS_DIR)

