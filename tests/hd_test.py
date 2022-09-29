# /usr/bin/env python
import pyDRC

PLOTS_DIR = "./plots"


# Filters at 44.1 kHz
sample_rate = 44100
half_srate = sample_rate/2

# Filter lenght: 65534 taps
signal_len = 2**16

# Read impulse and load it into DRCSignal class
peak = pyDRC.FindMaxPcm('imp.wav')
signal = pyDRC.DRCSignal()
data, a, b = pyDRC.ReadSignal('imp.wav',signal_len,peak)
signal.setData(data, a, b)
signal.setWLen(len(signal.getData()))
signal.setSampleRate(sample_rate)

hd = pyDRC.HD(signal, 0, len(signal.getData()), 3)
hd.process()

# Saving filtered_imp as raw data file
pyDRC.WriteSignal('minphase_imp.wav',pyDRC.SigNormalize(hd.getMPSig().getData(),1.0,'M')
                  ,0,len(hd.getMPSig().getData()),sample_rate, 'F')

