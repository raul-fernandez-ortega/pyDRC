#! /usr/bin/env python

import sys
import os
import re
import math
import numpy
import array
import pyDRC
import matplotlib.pyplot as plt
     
def PCA(DataFileList, PCA_dir, analysis_dir):
    PreWindowGap = 192
    PreWindowLen = 256
    dataLength = 2**16
    SampleRate = pyDRC.GetSampleRate(DataFileList[0])
    Size = len(DataFileList)
    
    Mean = numpy.zeros((Size), numpy.double)
    DataArray = numpy.zeros((Size,dataLength), numpy.double)
    TrArray = numpy.zeros((Size,dataLength), numpy.double)
    ResultArray = numpy.zeros((Size, dataLength), numpy.double)
    #SpectrumArray = numpy.zeros((Size, dataLength), float)

    length = 0
    for i in range(Size):
        print("---------------------------------------------------------------------------------")
        print("\tProcessing impulse file:%s"%DataFileList[i])
        l = pyDRC.FSize(DataFileList[i])
        length = max(length, l)
        peakPos =  pyDRC.FindMaxPcm(DataFileList[i])
        DataArray[i]  = numpy.asarray(pyDRC.ReadSignal(DataFileList[i], 2*dataLength, peakPos)[0])[dataLength-PreWindowLen:-PreWindowLen]
        for j in range(PreWindowLen - PreWindowGap):
            DataArray[i][j] = 0.
        DataArray[i][PreWindowLen - PreWindowGap: PreWindowLen] *= numpy.blackman(2*PreWindowGap)[:PreWindowGap]
        DataArray[i][-PreWindowGap:] *= numpy.blackman(2*PreWindowGap)[PreWindowGap:]
        print("\t\tLenght:%i samples. Sample peak at:%i"%(l, peakPos))
        
    for k in range(0, int(2*numpy.ceil(Size/2)), 2):
        fig, (ax) = plt.subplots(2,2)
        for l in range(2):
            i = k + l
            spectrum = 20*numpy.log10(numpy.abs(numpy.fft.fft(DataArray[i])))
            mn = spectrum.mean()
            smax = int(10*numpy.ceil((spectrum-mn).max()/10))
            ax[l,1].semilogx(numpy.linspace(0, SampleRate, len(spectrum)),spectrum - mn, linewidth=0.4)
            ax[l,1].set_xlim(xmin=20.,xmax=20000.)
            ax[l,1].set_ylim(ymin=smax-50.,ymax=smax)
            ax[l,1].tick_params(axis='x', labelsize=9)
            ax[l,1].tick_params(axis='y', labelsize=9)
            ax[l,1].set_xlabel("Frequency Hz", fontsize = 9)
            ax[l,1].set_ylabel("dB", fontsize = 9)
            ax[l,1].set_title("%s spectrum"%DataFileList[i].split("/")[-1].replace('.wav',''),fontsize = 9)
            ax[l,1].grid(True)
            if DataArray[i].max() == numpy.abs(DataArray[i]).max():
                ax[l,0].plot(numpy.linspace(0, len(DataArray[i]), len(DataArray[i])),DataArray[i]/numpy.abs(DataArray[i]).max(),linewidth=0.4)
            else:
                ax[l,0].plot(numpy.linspace(0, len(DataArray[i]), len(DataArray[i])),-1.0*DataArray[i]/numpy.abs(DataArray[i]).max(),linewidth=0.4)
            ax[l,0].set_xlim(xmin=0,xmax=1000)
            ax[l,0].set_ylim(ymin=-1.2,ymax=1.2)
            ax[l,0].tick_params(axis='x', labelsize=9)
            ax[l,0].tick_params(axis='y', labelsize=9)
            ax[l,0].set_xlabel("Time (samples)",fontsize = 9)
            ax[l,0].set_ylabel("Amplitude (normalized)", fontsize = 9)
            ax[l,0].set_title("%s"%DataFileList[i].split("/")[-1].replace('.wav',''),fontsize = 9)
            ax[l,0].grid(True)
            plt.tight_layout()
            pyDRC.WriteSignal('%s/filtered_data__%s.wav'%(analysis_dir,DataFileList[i].split("/")[-1].replace('.wav','')),DataArray[i]/(2*numpy.abs(DataArray[i]).max()),0,dataLength,SampleRate,'F')
        plt.savefig("%s/impulse_spectrum_%i.png"%(analysis_dir,k/2))
    print("Data File max length:%d"%length)
    
    for i in range(Size):
        Mean[i] = numpy.mean(DataArray[i])
        TrArray[i] = DataArray[i] - Mean[i]    
    
    ER = numpy.cov(TrArray)
    print("Covariance matrix:\n",ER)
    eigvalues, eigvectors = numpy.linalg.eig(ER)
    print("Eigenvalues:\n",eigvalues)
    print("Eigenvectors:\n",eigvectors)
    ResultArray = numpy.dot(numpy.transpose(eigvectors),TrArray)
    ER2 = numpy.cov(ResultArray)
    print("Final Covariance matrix:\n",ER2)

    maxvalue = 0
    for i in range(Size):
        maxvalue = max(maxvalue,max(ResultArray[i]))
        if ResultArray[0].max() == numpy.abs(ResultArray[0]).max():
            pyDRC.WriteSignal('%s/PCA_%d.wav'%(PCA_dir, i+1),numpy.concatenate((numpy.zeros(len(ResultArray[i])),ResultArray[i]/(2*numpy.abs(ResultArray[i]).max()))),0,2*len(ResultArray[i]), SampleRate,'F')
        else:
            pyDRC.WriteSignal('%s/PCA_%d.wav'%(PCA_dir, i+1),numpy.concatenate((numpy.zeros(len(ResultArray[i])),-1.0*ResultArray[i]/(2*numpy.abs(ResultArray[i]).max()))),0,2*len(ResultArray[i]), SampleRate,'F')
    for k in range(0, int(2*numpy.ceil(Size)/2), 2):
        fig, (ax) = plt.subplots(2,2)
        for l in range(2):
            i = k + l
            spectrum = 20*numpy.log10(numpy.abs(numpy.fft.fft(ResultArray[i])))
            smax = int(10*numpy.ceil(spectrum.max()/10))
            ax[l,1].semilogx(numpy.linspace(0, SampleRate, len(spectrum)),spectrum, linewidth=0.4)
            ax[l,1].set_xlim(xmin=20.,xmax=20000.)
            ax[l,1].set_ylim(ymin=smax-50.,ymax=smax)
            ax[l,1].tick_params(axis='x', labelsize=9)
            ax[l,1].tick_params(axis='y', labelsize=9)
            ax[l,1].set_xlabel("Frequency Hz", fontsize = 9)
            ax[l,1].set_ylabel("dB", fontsize = 9)
            ax[l,1].set_title("PCA-%d sprectrum"%i, fontsize = 9)
            ax[l,1].grid(True)
            if ResultArray[0].max() == numpy.abs(ResultArray[0]).max():
                ax[l,0].plot(numpy.linspace(0, len(ResultArray[i]), len(ResultArray[i])),ResultArray[i]/numpy.abs(ResultArray[i]).max(),linewidth=0.4)
            else:
                ax[l,0].plot(numpy.linspace(0, len(ResultArray[i]), len(ResultArray[i])),-1.*ResultArray[i]/numpy.abs(ResultArray[i]).max(),linewidth=0.4)
            ax[l,0].set_xlim(xmin=0,xmax=1000)
            ax[l,0].set_ylim(ymin=-1.2,ymax=1.2)
            ax[l,0].tick_params(axis='x', labelsize=9)
            ax[l,0].tick_params(axis='y', labelsize=9)
            ax[l,0].set_xlabel("Time (samples)",fontsize = 9)
            ax[l,0].set_ylabel("Amplitude (normalized)", fontsize = 9)
            ax[l,0].set_title("PCA-%d "%i, fontsize = 9)
            ax[l,0].grid(True)
            plt.tight_layout()
        plt.savefig("%s/PCA_spectrum_%i.png"%(analysis_dir,k/2))
        
    fig, (ax1, ax2) = plt.subplots(2)
    spectrum = 20*numpy.log10(numpy.abs(numpy.fft.fft(ResultArray[0])))
    ax2.semilogx(numpy.linspace(0, SampleRate, len(spectrum)),spectrum-spectrum.max(),linewidth=0.4)
    ax2.set_xlim(xmin=20.,xmax=20000.)
    ax2.set_ylim(ymin=-50.,ymax=10.)
    ax2.tick_params(axis='x', labelsize=9)
    ax2.tick_params(axis='y', labelsize=9)
    ax2.set_xlabel("Frequency Hz", fontsize = 9)
    ax2.set_ylabel("dB", fontsize = 9)
    ax2.grid(True)
    if ResultArray[0].max() == numpy.abs(ResultArray[0]).max():
        ax1.plot(numpy.linspace(0, len(ResultArray[0]), len(ResultArray[0])),ResultArray[0]/numpy.abs(ResultArray[0]).max(),linewidth=0.4)
    else:
        ax1.plot(numpy.linspace(0, len(ResultArray[0]), len(ResultArray[0])),-1.*ResultArray[0]/numpy.abs(ResultArray[0]).max(),linewidth=0.4)
    ax1.set_xlim(xmin=0,xmax=1000)
    ax1.set_ylim(ymin=-1.2,ymax=1.2)
    ax1.tick_params(axis='x', labelsize=9)
    ax1.tick_params(axis='y', labelsize=9)
    ax1.set_xlabel("Time (samples)",fontsize = 9)
    ax1.set_ylabel("Amplitude (normalized)", fontsize = 9)
    ax1.grid(True)
    plt.savefig("%s/Spectrum_PCA_principal.png"%(analysis_dir))
       
    
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
            
    DataFileList = []
    for fileName in os.listdir(impulse_dir):
        if fileName.find("wav") > 0:
            print("Found impulse file %s"%fileName)
            DataFileList.append("%s/%s"%(impulse_dir,fileName))
    DataFileList.sort()
    PCA(DataFileList, PCA_dir, analysis_dir)

if __name__ == '__main__':
    main()
