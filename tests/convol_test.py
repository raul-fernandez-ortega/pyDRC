# /usr/bin/env python

import pyDRC

# Filters at 44.1 kHz
sample_rate = 44100
half_srate = sample_rate/2

# Filter lenght: 65534 taps
filter_len = 2**16


lowfreq = 100.
highfreq = 3000.

# Band pass filter from 100 Hz to 3000 Hz
filter_1 = pyDRC.HanningWindow(pyDRC.BandPassFir(filter_len,lowfreq/half_srate,highfreq/half_srate),0,filter_len)

# Reading impulse
peak = pyDRC.FindMaxPcm('imp.wav')
impulse, a, b = pyDRC.ReadSignal('imp.wav',65536,peak)
filtered_imp = pyDRC.SigNormalize(pyDRC.Convolve(impulse, filter_1),1.0,'M')

# Saving filtered_imp as raw data file
pyDRC.WriteSignal('filtered_imp.wav',filtered_imp,0,len(filtered_imp),sample_rate, 'F')
