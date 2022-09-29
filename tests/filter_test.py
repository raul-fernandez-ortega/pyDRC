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

lowfreq = 100.
highfreq = 300
t = np.linspace(0, filter_len, filter_len)/sample_rate

print("Plotting FIR filters script test")

# Low pass filter at 100 Hz
print("Low Pass FIR filter")
filter_1 = pyDRC.SigNormalize(pyDRC.HanningWindow(pyDRC.LowPassFir(filter_len,lowfreq/half_srate),0,filter_len),1.0,'M')
pyDRC.WriteSignal("lowpassfir.wav",filter_1,0,len(filter_1),sample_rate,'F')
plt.rcParams["figure.figsize"] = (8,8)
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_1, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='Low Pass Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(np.fft.rfft(filter_1)))
f = np.linspace(0, sample_rate/2, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Low Pass Filter')
ax[1].set_xlim(xmin=20,xmax=20000)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/lowpassfir.png"%PLOTS_DIR)

# High pass filter at 3000 Hz
print("High Pass FIR filter")
filter_2 = pyDRC.SigNormalize(pyDRC.HanningWindow(pyDRC.HighPassFir(filter_len,highfreq/half_srate),0,filter_len),1.0,'M')
pyDRC.WriteSignal("highpassfir.wav",filter_2,0,len(filter_2),sample_rate,'F')
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_2, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='High Pass Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(np.fft.rfft(filter_2)))
f = np.linspace(0, sample_rate/2, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='High Pass Filter')
ax[1].set_xlim(xmin=20,xmax=20000)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/highpassfir.png"%PLOTS_DIR)


# Band pass filter from 100 Hz to 3000 Hz
print("Band Pass FIR filter")
filter_3 = pyDRC.SigNormalize(pyDRC.HanningWindow(pyDRC.BandPassFir(filter_len,lowfreq/half_srate,highfreq/half_srate),0,filter_len),1.0,'M')
pyDRC.WriteSignal("bandpassfir.wav",filter_3,0,len(filter_3),sample_rate,'F')
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_3, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='Band Pass Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(np.fft.rfft(filter_3)))
f = np.linspace(0, sample_rate/2, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Band Pass Filter')
ax[1].set_xlim(xmin=20,xmax=20000)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/bandpassfir.png"%PLOTS_DIR)


# Band stop filter from 100 Hz to 3000 Hz
print("Band Stop FIR filter")
filter_4 = pyDRC.SigNormalize(pyDRC.HanningWindow(pyDRC.BandStopFir(filter_len,lowfreq/half_srate,highfreq/half_srate),0,filter_len),1.0,'M')
pyDRC.WriteSignal("bandstopfir.wav",filter_4,0,len(filter_4),sample_rate,'F')
fig, ax = plt.subplots(2, 1)
ax[0].plot(t, filter_4, linewidth = 0.6)
ax[0].set(xlabel='Time', ylabel='Amplitude',title='Band Stop Filter')
ax[0].grid()
ax[0].set_xlim(xmin=0,xmax=filter_len/sample_rate)
ax[0].set_ylim(ymin=-1.0,ymax=1.0)
sp = 20*np.log10(np.abs(np.fft.rfft(filter_4)))
f = np.linspace(0, sample_rate/2, len(sp))
ax[1].semilogx(f,sp, linewidth = 0.6)
ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Band Stop Filter')
ax[1].set_xlim(xmin=20,xmax=20000)
ax[1].grid()
fig.tight_layout()
fig.savefig("%s/bandstopfir.png"%PLOTS_DIR)

