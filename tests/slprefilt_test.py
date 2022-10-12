#! /usr/bin/env python

import pyDRC
import numpy as np
import matplotlib.pyplot as plt

DATAFILE = "./imp.wav"
PLOTS_DIR = "./plots"

def main():
    
    #Sliding windowing sample
    print("Testing sliding window filtering function")
    PreWindowGap = 192
    PreWindowLen = 256
    dataLength = 2**16
    sample_rate = pyDRC.GetSampleRate(DATAFILE)
    peakpos = pyDRC.FindMaxPcm(DATAFILE)
    for j in range(0,3):
        lowerW = int(dataLength/(2**j))
        data, pstart, psend = pyDRC.ReadSignal(DATAFILE, lowerW, peakpos)
        data_peak = np.argmax(data)
        print("Data Peak position:%i"%data_peak)
        print("Data size:%i"%np.size(data))
        for i in range(0,10):
            windowExp = 0.5+i/10.
            print("Sliding window filter with low freq. window %i and window exponent %.2f"%(lowerW,windowExp))
            filtered = pyDRC.SLPreFilt(data, lowerW, 48, lowerW, 3, windowExp, sample_rate, 40, 20000, 48, 0.25, "WRight", "SLPProportional")
            filtered_peak = np.argmax(filtered)
            #print("Filter Peak position:%i"%filtered_peak)
            #print("Filter size:%i"%np.size(filtered))
            pyDRC.WriteSignal('%s/slprefiltered_%i_%3.2f.wav'%(PLOTS_DIR,lowerW, windowExp), filtered, 0, len(filtered),sample_rate, 'F')
            fig, ax = plt.subplots(1, 1)
            sp1 = 20*np.log10(np.abs(pyDRC.Fft(data)))
            sp2 = 20*np.log10(np.abs(pyDRC.Fft(filtered[filtered_peak-int(lowerW/2):filtered_peak+int(lowerW/2)])))
            f = np.linspace(0, sample_rate, np.size(sp2))
            ax.semilogx(f, sp1, color='black',linewidth = 0.4, label='Initial')
            ax.semilogx(f, sp2, color='red',linewidth = 0.7, label='Filtered')
            ax.set(xlabel='Frequency (Hz)', ylabel='Magnitude (dB)',title='Sliding Window Filter\nLow freq. window size: %i. Window exponent:%.2f'%(lowerW,windowExp))
            ax.set_xlim(xmin=20,xmax=20000)
            ax.set_ylim(ymin=-70,ymax=-20)
            ax.grid()
            ax.legend(loc='upper right')
            fig.tight_layout()
            fig.savefig("%s/slprefilt_%i_%3.2f.png"%(PLOTS_DIR,lowerW,windowExp))
    
if __name__ == '__main__':
    main()
