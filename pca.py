#! /usr/bin/env python

import sys
import os
import re
import math
import numpy as np
import array
import pyDRC
import matplotlib.pyplot as plt


def plot_analysis(signal, sampleRate, signalName, plotDir, plotName, windowExp=0.8, length = 0, yrange = 50):
    if length == 0:
        length = len(signal)
    else:
        peak = np.argmax(signal)
        signal = signal[peak-math.floor(length/2):peak+math.ceil(length/2)]
    fig, (ax1, ax2) = plt.subplots(2)
    if windowExp > 0:
        print("Sliding window filter for %s with low freq. window %i and window exponent %.2f"%(signalName,length,windowExp))
        filtered = pyDRC.SLPreFilt(signal, length, 48, length, 3, windowExp, sampleRate, 40, 20000, 48, 0.25, "WRight", "SLPProportional")
        filtered_peak = np.argmax(filtered)
    print("Plotting spectrum curve for %s"%signalName)
    sp1 = 20*np.log10(np.abs(pyDRC.Fft(signal)))
    if windowExp > 0:
        sp2 = 20*np.log10(np.abs(pyDRC.Fft(filtered[filtered_peak-math.floor(length/2):filtered_peak+math.ceil(length/2)])))
    f = np.linspace(0, sampleRate, int(length))
    ax2.semilogx(f, sp1, color='black',linewidth = 0.4)
    if windowExp > 0:
        ax2.semilogx(f, sp2, color='red',linewidth = 0.7)
    smax = int(10*np.ceil(sp1.max()/10))
    ax2.set_xlim(xmin = 20.,xmax = 20000.)
    ax2.set_ylim(ymin = smax-yrange, ymax = smax)
    ax2.tick_params(axis='x', labelsize=9)
    ax2.tick_params(axis='y', labelsize=9)
    ax2.set_xlabel("Frequency Hz", fontsize = 9)
    ax2.set_ylabel("dB", fontsize = 9)
    ax2.set_title("%s spectrum"%signalName, fontsize = 9)
    ax2.grid(True)
    plotArray = signal[np.argmax(signal)-300:np.argmax(signal)+700]
    if signal.max() == np.abs(signal).max():
        ax1.plot(np.linspace(0, len(plotArray), len(plotArray)),plotArray/np.abs(plotArray).max(),color='black',linewidth=0.4)
    else:
        ax1.plot(np.linspace(0, len(plotArray), len(plotArray)),-1.*plotArray/np.abs(plotArray).max(),color='black',linewidth=0.4)
    ax1.set_xlim(xmin=0,xmax=1000)
    ax1.set_ylim(ymin=-1.2,ymax=1.2)
    ax1.tick_params(axis='x', labelsize=9)
    ax1.tick_params(axis='y', labelsize=9)
    ax1.set_xlabel("Time (samples)",fontsize = 9)
    ax1.set_ylabel("Amplitude (normalized)", fontsize = 9)
    ax1.set_title("%s"%signalName, fontsize = 9)
    ax1.grid(True)
    plt.tight_layout()
    plt.savefig("%s/%s.png"%(plotDir,plotName))

def plot_analysis_list(signalList, sampleRate, signalNameList, plotDir,plotName, windowExp=0.8, yrange = 50):
    length = len(signalList[0])
    fig, (ax) = plt.subplots(len(signalList),2)
    for i in range(len(signalList)):
        if windowExp > 0:
            print("Sliding window filter of %s with low freq. window %i and window exponent %.2f"%(signalNameList[i],length,windowExp))
            filtered = pyDRC.SLPreFilt(signalList[i], length, 48, length, 3, windowExp, sampleRate, 40, 20000, 48, 0.25, "WRight", "SLPProportional")
            filtered_peak = np.argmax(filtered)
        print("Plotting spectrum curve for %s"%signalNameList[i])
        sp1 = 20*np.log10(np.abs(pyDRC.Fft(signalList[i])))
        if windowExp > 0:
            sp2 = 20*np.log10(np.abs(pyDRC.Fft(filtered[filtered_peak-int(length/2):filtered_peak+int(length/2)])))
        f = np.linspace(0, sampleRate, int(length))
        smax = int(10*np.ceil((sp1).max()/10))
        ax[i,1].semilogx(f, sp1, color='black',linewidth = 0.4, label='Initial')
        if windowExp > 0:
            ax[i,1].semilogx(f, sp2, color='red',linewidth = 0.7, label='Filtered')
        ax[i,1].set_xlim(xmin = 20., xmax = 20000.)
        ax[i,1].set_ylim(ymin = smax-yrange, ymax = smax)
        ax[i,1].tick_params(axis='x', labelsize=9)
        ax[i,1].tick_params(axis='y', labelsize=9)
        ax[i,1].set_xlabel("Frequency Hz", fontsize = 9)
        ax[i,1].set_ylabel("dB", fontsize = 9)
        ax[i,1].set_title("%s spectrum"%signalNameList[i],fontsize = 9)
        ax[i,1].grid(True)
        plotArray = signalList[i][np.argmax(signalList[i])-300:np.argmax(signalList[i])+700]
        if signalList[i].max() == np.abs(signalList[i]).max():
            ax[i,0].plot(np.linspace(0, len(plotArray), len(plotArray)),plotArray/np.abs(plotArray).max(),color='black',linewidth=0.4)
        else:
            ax[i,0].plot(np.linspace(0, len(plotArray), len(plotArray)),-1.0*plotArray/np.abs(plotArray).max(),color='black',linewidth=0.4)
        ax[i,0].set_xlim(xmin=0,xmax=1000)
        ax[i,0].set_ylim(ymin=-1.2,ymax=1.2)
        ax[i,0].tick_params(axis='x', labelsize=9)
        ax[i,0].tick_params(axis='y', labelsize=9)
        ax[i,0].set_xlabel("Time (samples)",fontsize = 9)
        ax[i,0].set_ylabel("Amplitude (normalized)", fontsize = 9)
        ax[i,0].set_title("%s"%signalNameList[i],fontsize = 9)
        ax[i,0].grid(True)
        plt.tight_layout()
    plt.savefig("%s/%s.png"%(plotDir,plotName))

class PCA:

    def __init__(self, impulse_dir, impulseLength):
        self.DataFileList = []
        for fileName in os.listdir(impulse_dir):
            if fileName.find("wav") > 0:
                print("Found impulse file %s"%fileName)
                self.DataFileList.append("%s/%s"%(impulse_dir,fileName))
        self.DataFileList.sort()
        self.impulseLength = impulseLength
        self.SampleRate = pyDRC.GetSampleRate(self.DataFileList[0])
        self.Size = len(self.DataFileList)
        self.DataArray = np.zeros((self.Size,self.impulseLength), np.double)
        self.PCAArray = None
        
        length = 0
        for i in range(self.Size):
            print("---------------------------------------------------------------------------------")
            print("\tProcessing impulse file:%s"%self.DataFileList[i])
            l = pyDRC.FSize(self.DataFileList[i])
            length = max(length, l)
            peakPos =  pyDRC.FindMaxPcm(self.DataFileList[i])
            print("\t\tLenght:%i samples. Sample peak at:%i"%(l,peakPos))
            data, pstart, psend = pyDRC.ReadSignal(self.DataFileList[i], self.impulseLength, peakPos)
            self.DataArray[i]  = pyDRC.BlackmanWindow(pyDRC.ReadSignal(self.DataFileList[i], self.impulseLength, peakPos)[0],0,self.impulseLength)
        print("Data File max length:%d"%length)

    def proccess(self):
        Mean = np.zeros((self.Size), np.double)
        TrArray = np.zeros((self.Size,self.impulseLength), np.double)
        for i in range(self.Size):
            Mean[i] = np.mean(self.DataArray[i])
            TrArray[i] = self.DataArray[i] - Mean[i]    
    
        ER = np.cov(TrArray)
        eigvalues, eigvectors = np.linalg.eig(ER)
        print("EigenValues:")
        for i in range(len(eigvalues)):
            print("%3d %.3e"%(i+1,eigvalues[i]))
        self.PCAArray = np.dot(np.transpose(eigvectors), TrArray)
        

    def impulse_analysis(self, analysis_dir, windowExp = 0.8):
        j = 1
        for k in range(0, int(2*np.ceil(self.Size/2)), 2):
            signalList = []
            signalNameList = []
            for l in range(2):
                i = k + l
                signalList.append(self.DataArray[i])
                signalNameList.append(self.DataFileList[i].split('/')[-1].replace('.wav',''))
            plot_analysis_list(signalList, self.SampleRate, signalNameList, analysis_dir,'impulse_spectrum_%d'%j, windowExp)
            j += 1

    def PCA_analysis(self, analysis_dir, windowExp = 0.8):
        j = 1
        for k in range(0, int(2*np.ceil(self.Size)/2), 2):
            signalList = []
            signalNameList = []
            for l in range(2):
                i = k + l
                signalList.append(self.PCAArray[i])
                signalNameList.append("PCA_%d"%i)
            plot_analysis_list(signalList, self.SampleRate, signalNameList, analysis_dir,'PCA_spectrum_%d'%j, windowExp)
            j += 1
        plot_analysis(self.PCAArray[0], self.SampleRate, 'Main PCA signal', analysis_dir,'Spectrum_PCA_principal', windowExp)

    def save_PCA_components(self, PCA_dir):
        maxvalue =  np.abs(self.PCAArray[0]).max()
        for i in range(self.Size):
            print("Saving PCA vector %d to %s/PCA_%d.wav"%(i,PCA_dir,i+1))
            if self.PCAArray[0].max() == np.abs(self.PCAArray[0]).max():
                pyDRC.WriteSignal('%s/PCA_%d.wav'%(PCA_dir, i+1), self.PCAArray[i]/maxvalue, 0, len(self.PCAArray[i]), self.SampleRate,'F')
            else:
                pyDRC.WriteSignal('%s/PCA_%d.wav'%(PCA_dir, i+1), -1.0*self.PCAArray[i]/maxvalue, 0, len(self.PCAArray[i]), self.SampleRate,'F')

    def save_main_component(self, PCA_dir, FileName):
        if self.PCAArray[0].max() == np.abs(self.PCAArray[0]).max():
            pyDRC.WriteSignal('%s/%s'%(PCA_dir, FileName), self.PCAArray[0], 0, len(self.PCAArray[0]), self.SampleRate,'F')
        else:
            pyDRC.WriteSignal('%s/%s'%(PCA_dir, FileName), -1.0*self.PCAArray[0], 0, len(self.PCAArray[0]), self.SampleRate,'F')
        
def main():
    impulse_dir ="."
    PCA_dir = "."
    analysis_dir = "."
    if len(sys.argv) < 7:
        print("Usage:")
        print("python pca_analysis.py -i impulses_dir -p PCA_dir -a analysis_dir")
        sys.exit()
    for i in range(1,len(sys.argv)):
        if sys.argv[i] == "-i":
            impulse_dir = sys.argv[i+1]
        elif sys.argv[i] == "-p":
            PCA_dir = sys.argv[i+1]
            if not os.path.isdir(PCA_dir):
                os.mkdir(PCA_dir)
        elif sys.argv[i] == "-a":
            analysis_dir = sys.argv[i+1]
            if not os.path.isdir(analysis_dir):
                os.mkdir(analysis_dir)
                
    pca = PCA(impulse_dir, 2**16)
    pca.impulse_analysis(analysis_dir)
    pca.proccess()
    pca.save_PCA_components(PCA_dir)
    pca.PCA_analysis(analysis_dir)

if __name__ == '__main__':
    main()
