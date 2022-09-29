from sys import argv
import os
import re
import math
import numpy
import array
import pyDRC
import matplotlib.pyplot as plt

def PlotSpectrum(FName,Src,WStart,WLen,SampleRate,FType):
  SpectrumArray = 20*numpy.log10(numpy.abs(numpy.fft.fft(Src)))
  plot.grid(True, which="both")
  plt.semilogx(numpy.linspace(0, SampleRate, len(SpectrumArray)),SpectrumArray - SpectrumArray.max())
  plt.set_xlim(xmin=20.,xmax=20000.)
  plt.set_ylim(ymin=-50.,ymax=10.)
  plt.xlabel("Freq. (Hz)")
  plt.ylabel("dB")
  plt.title("%s Spectrum"%FName)
  plt.savefig("%s_spectrum.png"%FName)
  return _pyDCR.SND_WriteSignal(FName,Src,WStart,WLen,SampleRate,FType)

def main():
  PreWindowGap = 192
  PreWindowLen = 256
  dataLength = 2**8

  wkdir = argv[-1]
  for fileName in os.listdir(wkdir):
      
      if fileName.find(".wav") > 0:
          length = 0
          Data = numpy.zeros(dataLength, numpy.float)
          SampleRate = pyDRC.GetSampleRate("%s/%s"%(wkdir,fileName))
          print("---------------------------------------------------------------------------------")
          print("\tProcessing DRC output file:%s"%fileName)
          length = max(length, pyDRC.FSize("%s/%s"%(wkdir,fileName)))
          peakPos =  pyDRC.FindMaxPcm("%s/%s"%(wkdir,fileName))
          print("\t\tLength:%i samples. Peak at sample:%i"%(length,peakPos))
          Data  = numpy.asarray(pyDRC.ReadSignal("%s/%s"%(wkdir,fileName), 2*dataLength, peakPos)[0])
          #for i in range(dataLength - PreWindowLen):
          #    Data[i] = 0.
          Data *= numpy.blackman(Data.size)
          Spectrum = 20*numpy.log10(numpy.abs(numpy.fft.fft(Data)))
          plt.semilogx(numpy.linspace(0, SampleRate, len(Spectrum)),Spectrum - Spectrum.max())
          plt.xlim(xmin=20.,xmax=20000.)
          plt.ylim(ymin=-50.,ymax=10.)
          plt.title("%s Spectrum"%fileName.replace(".wav",""))
          plt.xlabel("Frequency (Hz)")
          plt.ylabel("dB")
          plt.grid(True, which="both")
          plt.savefig("%s/%s_spectrum.png"%(wkdir,fileName.replace(".wav","")))
          plt.clf()

if __name__ == '__main__':
    main()
