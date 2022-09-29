#! /usr/bin/env python

import pyDRC
import numpy as np
import scipy.signal as signal
from scipy.io.wavfile import read as scipy_io_read
import matplotlib.pyplot as plt

DATAFILE = "./measurement.wav"
PLOTS_DIR = "./plots"

def main():
    # FastLowPassFir Plot
    print("Testing FastLowPassFir function and comparison with scipy.signal.firwin function")
    Length = 200
    Freq = 0.3
    Rate = 1.0
    t = np.arange(0.0, 1.0, 1.0/Length)
    s = pyDRC.Ones(Length)
    Window = pyDRC.BlackmanWindow(s, 0, Length)
    s1 = pyDRC.FastLowPassFir(Length, 2*Freq, Window, len(Window))
    s2 = signal.firwin(Length, Freq, fs=Rate, window='blackman')
    plt.rcParams["figure.figsize"] = (8,8)
    fig, ax = plt.subplots(2, 1)
    ax[0].plot(t, s1, t, s2, linewidth = 0.8)
    ax[0].set(xlabel='X value', ylabel='Filter value',title='Fast Low Pass FIR')
    ax[0].grid()
    sp1 = 20*np.log10(np.abs(np.fft.rfft(s1)))
    sp2 = 20*np.log10(np.abs(np.fft.rfft(s2)))
    f = np.linspace(0, Rate/2, len(sp1))
    ax[1].plot(f, sp1, f, sp2, linewidth = 0.8)
    ax[1].set(xlabel='Frequency', ylabel='Magnitude',title='Fast Low Pass FIR')
    ax[1].grid()
    fig.tight_layout()
    fig.savefig("%s/fast_low_pass_fir.png"%PLOTS_DIR)
    
    #Sliding windowing sample
    print("Testing sliding window filtering function")
    PreWindowGap = 192
    PreWindowLen = 256
    dataLength = 2**16
    samplerate = pyDRC.GetSampleRate(DATAFILE)
    peakpos = pyDRC.FindMaxPcm(DATAFILE )
    data, pstart, psend = pyDRC.ReadSignal(DATAFILE, 2*dataLength, peakpos)
    for j in range(PreWindowLen - PreWindowGap):
        data[j] = 0.
        data[PreWindowLen - PreWindowGap: PreWindowLen] *= np.blackman(2*PreWindowGap)[:PreWindowGap]
        data[-PreWindowGap:] *= np.blackman(2*PreWindowGap)[PreWindowGap:]
    print("Sample Rate:%d Hz"%samplerate)
    t = np.linspace(0, 2*dataLength,2*dataLength)/samplerate
    filtered = pyDRC.SLPreFilt(data, 48000, 48, dataLength, 3, 0.8, samplerate, 50, 20000, 48, 0.25, "WFull", "SLPProportional")
    filtered = np.concatenate((filtered,np.zeros(2*dataLength - len(filtered))))
    fig, ax = plt.subplots(2, 1)
    #ax[0].plot(t, data, t, filtered, linewidth = 0.8)
    ax[0].plot(t, filtered, linewidth = 0.6)
    ax[0].set(xlabel='Time (s)', ylabel='Amplitude',title='Sliding Window PreFilter')
    #ax[0].set_xlim(xmin=1.45,xmax=1.55)
    ax[0].grid()
    sp1 = 20*np.log10(np.abs(np.fft.rfft(data)))
    #sp2 = 20*np.log10(np.abs(np.fft.rfft(filtered)))
    f = np.linspace(0, samplerate/2, len(sp1))
    #ax[1].plot(f, sp1, f, sp2, linewidth = 0.8)
    ax[1].semilogx(f, sp1, linewidth = 0.6)
    ax[1].set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Sliding Window Filteringxs')
    ax[1].set_xlim(xmin=20,xmax=20000)
    ax[1].set_ylim(ymin=-30,ymax=20)
    ax[1].grid()
    fig.tight_layout()
    fig.savefig("%s/slprefilt.png"%PLOTS_DIR)
    
if __name__ == '__main__':
    main()
