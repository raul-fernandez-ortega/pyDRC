#! /usr/bin/env python

from pyDRC import *
from drccfg import *
from time import time
import sys
import pca
import math
import numpy as np
import array
import matplotlib.pyplot as plt


class drc:

    def __init__(self, DRCfile):

        self.ConfigDict = DRCCfg(DRCfile)
        if not self.ConfigDict.CheckDRCCfg():
            print("DRC: Error in DRC config file %s exiting..."%DRCFile) 
            sys.exit()
        self.activeStage = None

    def plot_analysis(self, signal, sampleRate, signalName, plotDir, plotName, windowExp=0.8, length = 0, yrange = 50):
        if length == 0:
            length = len(signal)
        else:
            peak = np.argmax(signal)
            signal = signal[peak-math.floor(length/2):peak+math.ceil(length/2)]
        fig, (ax1, ax2) = plt.subplots(2)
        if windowExp > 0:
            print("Sliding window filter for %s with low freq. window %i and window exponent %.2f"%(signalName,length,windowExp))
            filtered = SLPreFilt(signal, length, 48, length, 3, windowExp, sampleRate, 40, 20000, 48, 0.25, "WRight", "SLPProportional")
            filtered_peak = np.argmax(filtered)
        print("Plotting spectrum curve for %s"%signalName)
        sp1 = 20*np.log10(np.abs(Fft(signal)))
        if windowExp > 0:
            sp2 = 20*np.log10(np.abs(Fft(filtered[filtered_peak-math.floor(length/2):filtered_peak+math.ceil(length/2)])))
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


    def plot_analysis_list(self, signalList, sampleRate, signalNameList, plotDir,plotName, windowExp=0.8, yrange = 50):
        length = len(signalList[0])
        for signal in signalList:
            if len(signal) < length:
                length = len(signal)
        fig, (ax) = plt.subplots(len(signalList),2)
        for i in range(len(signalList)):
            if windowExp > 0:
                print("Sliding window filter of %s with low freq. window %i and window exponent %.2f"%(signalNameList[i],length,windowExp))
                filtered = SLPreFilt(signalList[i], length, 48, length, 3, windowExp, sampleRate, 40, 20000, 48, 0.25, "WRight", "SLPProportional")
                filtered_peak = np.argmax(filtered)
            print("Plotting spectrum curve for %s"%signalNameList[i])
            sp1 = 20*np.log10(np.abs(Fft(signalList[i][:length])))
            if windowExp > 0:
                sp2 = 20*np.log10(np.abs(Fft(filtered[filtered_peak-int(length/2):filtered_peak+int(length/2)])))
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

    def runStage(self, name, inSig, stage, stageconf):
        if inSig is None:
            self.activeStage = stage(stageconf)
        else:
            self.activeStage = stage(inSig, stageconf)
        if not self.activeStage.process():
            print("DRC: %s stage process fails"%name)
            sys.exit()
        if stageconf.NormFactor > 0:
            self.activeStage.Normalize()
        if stageconf.OutFile is not None:
            self.activeStage.WriteOutput()
        return self.activeStage.getOutSig()
        

    def run(self):

        # 1 - PCA stage. Impulses PCA.
        
        if self.ConfigDict.PCAImpulsesDir is not None:
            print("# 1 - PCA stage. Impulses PCA.")
            pca_drc = pca.PCA(self.ConfigDict.PCAImpulsesDir,self.ConfigDict.BCInitWindow)
            pca_drc.proccess()
            pca_drc.save_main_component(self.ConfigDict.BCBaseDir,self.ConfigDict.BCInFile)

        # 2 - BC stage. Base Configuration.
    
        BCstageconf = BCParmsType()
        BCstageconf.BCBaseDir = self.ConfigDict.BCBaseDir
        BCstageconf.BCInFile = self.ConfigDict.BCBaseDir + self.ConfigDict.BCInFile
        BCstageconf.BCSampleRate = self.ConfigDict.BCSampleRate
        BCstageconf.BCInFileType = self.ConfigDict.BCInFileType
        BCstageconf.BCImpulseCenterMode = self.ConfigDict.BCImpulseCenterMode
        BCstageconf.BCImpulseCenter = self.ConfigDict.BCImpulseCenter
        BCstageconf.BCInitWindow = self.ConfigDict.BCInitWindow
        BCstageconf.BCPreWindowLen = self.ConfigDict.BCPreWindowLen
        BCstageconf.BCPreWindowGap = self.ConfigDict.BCPreWindowGap
        BCstageconf.BCDLType = self.ConfigDict.BCDLType
        BCstageconf.BCDLMinGain = self.ConfigDict.BCDLMinGain
        BCstageconf.BCDLStartFreq = self.ConfigDict.BCDLStartFreq 
        BCstageconf.BCDLEndFreq = self.ConfigDict.BCDLEndFreq
        BCstageconf.BCDLStart = self.ConfigDict.BCDLStart
        BCstageconf.BCDLMultExponent = self.ConfigDict.BCDLMultExponent
        BCstageconf.NormFactor = self.ConfigDict.BCNormFactor 
        BCstageconf.NormType = self.ConfigDict.BCNormType
        BCstageconf.OutFile = self.ConfigDict.BCOutFile
        BCstageconf.OutFileType = self.ConfigDict.BCOutFileType
        print("# 2 - BC stage. Base Configuration.")
        OutBC = self.runStage("BC", None, BCstage, BCstageconf)
        self.plot_analysis(OutBC.getData(), OutBC.getSampleRate(), "BC stage Output", "p_left", "BCplot",
                           windowExp=self.ConfigDict.MPWindowExponent)
    
        # 3 - MC stage. Microphone Compensation.

        MCstageconf = MCParmsType()
        MCstageconf.MCFilterType = self.ConfigDict.MCFilterType 
        MCstageconf.MCInterpolationType = self.ConfigDict.MCInterpolationType
        MCstageconf.MCMultExponent = self.ConfigDict.MCMultExponent
        MCstageconf.MCFilterLen = self.ConfigDict.MCFilterLen 
        MCstageconf.MCNumPoints = self.ConfigDict.MCNumPoints
        MCstageconf.MCPointsFile = self.ConfigDict.BCBaseDir + self.ConfigDict.MCPointsFile 
        MCstageconf.MCMagType = self.ConfigDict.MCMagType
        MCstageconf.MCFilterFile = self.ConfigDict.MCFilterFile
        MCstageconf.MCFilterFileType = self.ConfigDict.MCFilterFileType
        MCstageconf.MCOutWindow = self.ConfigDict.MCOutWindow 
        MCstageconf.MCNormFactor = self.ConfigDict.MCNormFactor
        MCstageconf.MCNormType = self.ConfigDict.MCNormType
        MCstageconf.MCOutFile = self.ConfigDict.MCOutFile
        MCstageconf.MCOutFileType = self.ConfigDict.MCOutFileType
        print("# 3 - MC stage. Microphone Compensation.")
        stage = MCstage(OutBC, MCstageconf)
        stage.process()
        OutMC = stage.getOutSig()

        # 4 - BCDL stage. Base Configuration Dip Limiting.

        BCDLstageconf = BCDLParmsType()
        BCDLstageconf.BCDLType = self.ConfigDict.BCDLType
        BCDLstageconf.BCDLMinGain = self.ConfigDict.BCDLMinGain
        BCDLstageconf.BCDLStartFreq = self.ConfigDict.BCDLStartFreq
        BCDLstageconf.BCDLEndFreq = self.ConfigDict.BCDLEndFreq
        BCDLstageconf.BCDLStart = self.ConfigDict.BCDLStart
        BCDLstageconf.BCDLMultExponent = self.ConfigDict.BCDLMultExponent
        BCDLstageconf.NormFactor = self.ConfigDict.BCDLNormFactor
        BCDLstageconf.NormType = self.ConfigDict.BCDLNormType
        BCDLstageconf.OutFile = self.ConfigDict.BCDLOutFile
        BCDLstageconf.OutFileType = self.ConfigDict.BCDLOutFileType
        print("# 4 - BCDL stage. Base Configuration Dip Limiting.")
        OutBCDL = self.runStage("BCDL",OutMC, BCDLstage, BCDLstageconf)
        self.plot_analysis(OutBCDL.getData(), OutBCDL.getSampleRate(), "BCDL stage Output", "p_left", "BCDLplot",
                           windowExp=self.ConfigDict.MPWindowExponent)
        self.plot_analysis_list([OutBC.getData(), OutBCDL.getData()], OutBC.getSampleRate(), ["BC stage Output", "BCDL stage Output"],
                           "p_left", "BCvsBCDLplot", windowExp=0.0)
    
        # 5 - HD stage. Homomorphic Deconvolution.

        HDstageconf = HDParmsType()
        HDstageconf.HDMultExponent = self.ConfigDict.HDMultExponent
        HDstageconf.HDMPNormFactor = self.ConfigDict.HDMPNormFactor
        HDstageconf.HDMPNormType = self.ConfigDict.HDMPNormType
        HDstageconf.HDMPOutFile = self.ConfigDict.HDMPOutFile
        HDstageconf.HDMPOutFileType = self.ConfigDict.HDMPOutFileType
        HDstageconf.HDEPNormType = self.ConfigDict.HDEPNormType
        HDstageconf.HDEPNormFactor = self.ConfigDict.HDEPNormFactor
        HDstageconf.HDEPOutFile = self.ConfigDict.HDEPOutFile
        HDstageconf.HDEPOutFileType = self.ConfigDict.HDEPOutFileType
        print("# 5 - HD stage. Homomorphic Deconvolution.")
        stage = HDstage(OutBCDL,HDstageconf)
        if not stage.process():
            print("DRC: Homomorphic Deconvolution process fails")
            sys.exit()
        if HDstageconf.HDMPNormFactor > 0 or HDstageconf.HDEPNormFactor > 0:
            stage.Normalize()
        if HDstageconf.HDMPOutFile is not None or HDstageconf.HDEPOutFile is not None:
            stage.WriteOutput()
        MPS = stage.getMPSig()
        EPS = stage.getEPSig()
        self.plot_analysis(MPS.getData(), MPS.getSampleRate(), "HD Minimum phase stage Output", "p_left", "HDMPplot",
                           windowExp=self.ConfigDict.MPWindowExponent)
        self.plot_analysis(EPS.getData(), EPS.getSampleRate(), "HD Excess phase stage Output", "p_left", "HDEPplot",
                           windowExp=self.ConfigDict.EPWindowExponent)
        self.plot_analysis_list([OutBC.getData(), MPS.getData()], OutBC.getSampleRate(), ["BC stage Output", "HD Minimum phase stage Output"],
                           "p_left", "BCvsHDMPplot", windowExp=0.0)
          
        # 6 - MP stage. Minimum Phase Prefiltering.
        
        MPstageconf = WFParmsType()
        MPstageconf.WFfilterType = self.ConfigDict.MPPrefilterType
        MPstageconf.WFfilterFctn = self.ConfigDict.MPPrefilterFctn
        MPstageconf.WFWindowGap = self.ConfigDict.MPWindowGap
        MPstageconf.WFLowerWindow = self.ConfigDict.MPLowerWindow
        MPstageconf.WFUpperWindow = self.ConfigDict.MPUpperWindow
        MPstageconf.WFStartFreq = self.ConfigDict.MPStartFreq
        MPstageconf.WFEndFreq = self.ConfigDict.MPEndFreq
        MPstageconf.WFFilterLen = self.ConfigDict.MPFilterLen
        MPstageconf.WFFSharpness = self.ConfigDict.MPFSharpness
        MPstageconf.WFBandSplit = self.ConfigDict.MPBandSplit
        MPstageconf.WFWindowExponent = self.ConfigDict.MPWindowExponent
        MPstageconf.WFFinalWindow = self.ConfigDict.MPPFFinalWindow
        MPstageconf.WFRTFlag = 'N'
        MPstageconf.NormFactor = self.ConfigDict.MPPFNormFactor
        MPstageconf.NormType = self.ConfigDict.MPPFNormType
        MPstageconf.OutFile = self.ConfigDict.MPPFOutFile 
        MPstageconf.OutFileType = self.ConfigDict.MPPFOutFileType
        print("# 6 - MP stage. Minimum Phase Prefiltering.")
        MPSWF = self.runStage("MP", MPS, WFstage, MPstageconf)
        self.plot_analysis(MPSWF.getData(), MPSWF.getSampleRate(), "MP Prefiltering stage Output", "p_left", "MPSplot", windowExp=0.0)
        self.plot_analysis_list([OutBC.getData(), MPSWF.getData()], OutBC.getSampleRate(), ["BC stage Output", "MP stage Output"],
                           "p_left", "BCvsMPplot", windowExp=0.0)
        
    
        # 6 - DL stage. Dip Limiting.

        DLstageconf = DLParmsType()
        DLstageconf.DLType = self.ConfigDict.DLType
        DLstageconf.DLMinGain = self.ConfigDict.DLMinGain
        DLstageconf.DLStartFreq = self.ConfigDict.DLStartFreq
        DLstageconf.DLEndFreq = self.ConfigDict.DLEndFreq
        DLstageconf.DLStart = self.ConfigDict.DLStart
        DLstageconf.DLMultExponent = self.ConfigDict.DLMultExponent
        #DLstageconf.DLNormFactor = self.ConfigDict.MPPFNormFactor
        #DLstageconf.DLNormType = self.ConfigDict.MPPFNormType
        #DLstageconf.DLOutFile = self.ConfigDict.MPPFOutFile
        #DLstageconf.DLOutFileType = self.ConfigDict.MPPFOutFileType
        print("# 6 - DL stage. Dip Limiting.")
        stage = DLstage(MPS,DLstageconf)
        if not stage.process():
            print("DRC: Dip limiting process fails")
            sys.exit()
        if DLstageconf.DLNormFactor > 0:
            stage.Normalize()
        if DLstageconf.DLOutFile is not None:
            stage.WriteOutput()
        MPS = stage.getOutSig()
        
        # 7 - HR stage. Homomorphic Recovery.

        HRstageconf = HRParmsType()
        HRstageconf.HRMPHDRecover = self.ConfigDict.MPHDRecover
        HRstageconf.HRMPEPPreserve = self.ConfigDict.MPEPPreserve
        HRstageconf.HRMPHDMultExponent = self.ConfigDict.MPHDMultExponent
        HRstageconf.HRMPFinalWindow = self.ConfigDict.MPPFFinalWindow
        HRstageconf.HRMPOutFileType = self.ConfigDict.MPPFOutFileType
        HRstageconf.HRMPNormFactor = self.ConfigDict.MPPFNormFactor
        HRstageconf.HRMPNormType = self.ConfigDict.MPPFNormType
        HRstageconf.HRMPOutFile = self.ConfigDict.MPPFOutFile
        HRstageconf.HRMPOutFileType = self.ConfigDict.MPPFOutFileType
        HRstageconf.HREPNormFactor = self.ConfigDict.EPPFNormFactor
        HRstageconf.HREPNormType = self.ConfigDict.EPPFNormType
        HRstageconf.HREPOutFile = self.ConfigDict.EPPFOutFile
        HRstageconf.HREPOutFileType = self.ConfigDict.EPPFOutFileType
        print("# 7 - HR stage. Homomorphic Recovery.")
        stage = HRstage(MPS,EPS,HRstageconf)
        if not stage.process():
            print("DRC: Homomorphic Recovery process fails")
            sys.exit()
        if HRstageconf.HRMPNormFactor > 0 or HRstageconf.HREPNormFactor > 0:
            stage.Normalize()
        if HRstageconf.HRMPOutFile is not None or HRstageconf.HREPOutFile is not None:
            stage.WriteOutput()    
        MPS = stage.getMPSig()
        EPS = stage.getEPSig()

        # 8 - EP stage. Excess Phase Prefiltering.
 
        EPstageconf = WFParmsType()
        EPstageconf.WFfilterType = self.ConfigDict.EPPrefilterType
        EPstageconf.WFfilterFctn = self.ConfigDict.EPPrefilterFctn
        EPstageconf.WFWindowGap = self.ConfigDict.EPWindowGap 
        EPstageconf.WFLowerWindow = self.ConfigDict.EPLowerWindow
        EPstageconf.WFUpperWindow = self.ConfigDict.EPUpperWindow
        EPstageconf.WFStartFreq = self.ConfigDict.EPStartFreq
        EPstageconf.WFEndFreq = self.ConfigDict.EPEndFreq 
        EPstageconf.WFFilterLen = self.ConfigDict.EPFilterLen
        EPstageconf.WFFSharpness = self.ConfigDict.EPFSharpness
        EPstageconf.WFBandSplit = self.ConfigDict.EPBandSplit
        EPstageconf.WFWindowExponent = self.ConfigDict.EPWindowExponent
        EPstageconf.WFFinalWindow = self.ConfigDict.EPPFFinalWindow
        EPstageconf.WFRTFlag = 'N'
        EPstageconf.WFNormFactor = self.ConfigDict.EPPFNormFactor
        EPstageconf.WFNormType = self.ConfigDict.EPPFNormType
        EPstageconf.WFOutFile = self.ConfigDict.EPPFOutFile
        EPstageconf.WFOutFileType = self.ConfigDict.EPPFOutFileType
        print("# 8 - EP stage. Excess Phase Prefiltering.")
        stage = WFstage(EPS,EPstageconf)
        if not stage.process():
            print("DRC: Excess Phase Prefiltering process fails")
            sys.exit()
        if EPstageconf.WFNormFactor > 0:
            stage.Normalize()
        if EPstageconf.WFOutFile is not None:
            stage.WriteOutput()
        EPS = stage.getOutSig()
    
        # 9 - ER stage. Excess Phase Recovery.

        ERstageconf = ERParmsType()
        ERstageconf.EREPFlatGain = self.ConfigDict.EPPFFlatGain
        ERstageconf.EREPOGainFactor = self.ConfigDict.EPPFOGainFactor
        ERstageconf.EREPFlatType = self.ConfigDict.EPPFFlatType
        ERstageconf.EREPFGMultExponent = self.ConfigDict.EPPFFGMultExponent
        ERstageconf.EREPFinalWindow = self.ConfigDict.EPPFFinalWindow
        ERstageconf.EREPNormFactor = self.ConfigDict.EPPFNormFactor
        ERstageconf.EREPNormType = self.ConfigDict.EPPFNormType
        ERstageconf.EREPOutFile = self.ConfigDict.EPPFOutFile
        ERstageconf.EREPOutFileType = self.ConfigDict.EPPFOutFileType
        print("# 9 - ER stage. Excess Phase Recovery.")
        stage = ERstage(EPS,ERstageconf)
        if not stage.process():
            print("DRC: Excess Phase Recovery process fails")
            sys.exit()
        if ERstageconf.EREPNormFactor > 0:
            stage.Normalize()
        if ERstageconf.EREPOutFile is not None:
            stage.WriteOutput()    
        EPS = stage.getOutSig()

        # 10 - IS stage. Inversion Stage.
        
        ISstageconf = ISParmsType()
        ISstageconf.PCOutWindow = self.ConfigDict.PCOutWindow 
        ISstageconf.PCNormFactor = self.ConfigDict.PCNormFactor
        ISstageconf.PCNormType = self.ConfigDict.PCNormType
        ISstageconf.PCOutFile = self.ConfigDict.PCOutFile
        ISstageconf.PCOutFileType = self.ConfigDict.PCOutFileType
        ISstageconf.ISType = self.ConfigDict.ISType
        ISstageconf.ISPETType = self.ConfigDict.ISPETType
        ISstageconf.ISPrefilterFctn = self.ConfigDict.ISPrefilterFctn
        ISstageconf.ISPELowerWindow = self.ConfigDict.ISPELowerWindow
        ISstageconf.ISPEUpperWindow = self.ConfigDict.ISPEUpperWindow 
        ISstageconf.ISPEStartFreq = self.ConfigDict.ISPEStartFreq 
        ISstageconf.ISPEEndFreq = self.ConfigDict.ISPEEndFreq 
        ISstageconf.ISPEFilterLen = self.ConfigDict.ISPEFilterLen
        ISstageconf.ISPEFSharpness = self.ConfigDict.ISPEFSharpness
        ISstageconf.ISPEBandSplit = self.ConfigDict.ISPEBandSplit
        ISstageconf.ISPEWindowExponent = self.ConfigDict.ISPEWindowExponent
        ISstageconf.ISPEOGainFactor = self.ConfigDict.ISPEOGainFactor
        ISstageconf.ISSMPMultExponent = self.ConfigDict.ISSMPMultExponent
        ISstageconf.ISOutWindow = self.ConfigDict.ISOutWindow 
        ISstageconf.ISNormFactor = self.ConfigDict.ISNormFactor
        ISstageconf.ISNormType = self.ConfigDict.ISNormType
        ISstageconf.ISOutFile = self.ConfigDict.ISOutFile
        ISstageconf.ISOutFileType = self.ConfigDict.ISOutFileType
        print("# 10 - IS stage. Inversion Stage.")
        stage = ISstage(MPS,EPS,ISstageconf)
        if not stage.process():
            print("DRC: Prefilter Completion/Inversion process fails")
            sys.exit()
        if ISstageconf.ISNormFactor > 0 or ISstageconf.PCNormFactor > 0:
            stage.Normalize()
        if ISstageconf.ISOutFile is not None or  ISstageconf.PCOutFile is not None:
            stage.WriteOutput()
        OutS = stage.getISOutSig()
    
        # 11 - PT stage. Psychoacoustic Target stage.

        PTstageconf = PTParmsType()
        PTstageconf.PTType = self.ConfigDict.PTType
        PTstageconf.PTReferenceWindow = self.ConfigDict.PTReferenceWindow
        PTstageconf.PTDLType = self.ConfigDict.PTDLType
        PTstageconf.PTDLMinGain = self.ConfigDict.PTDLMinGain
        PTstageconf.PTDLStartFreq = self.ConfigDict.PTDLStartFreq
        PTstageconf.PTDLEndFreq = self.ConfigDict.PTDLEndFreq
        PTstageconf.PTDLStart = self.ConfigDict.PTDLStart
        PTstageconf.PTDLMultExponent = self.ConfigDict.PTDLMultExponent 
        PTstageconf.PTBandWidth = self.ConfigDict.PTBandWidth
        PTstageconf.PTPeakDetectionStrength = self.ConfigDict.PTPeakDetectionStrength
        PTstageconf.PTMultExponent = self.ConfigDict.PTMultExponent
        PTstageconf.PTFilterLen = self.ConfigDict.PTFilterLen
        PTstageconf.PTFilterFile = self.ConfigDict.PTFilterFile
        PTstageconf.PTFilterFileType = self.ConfigDict.PTFilterFileType
        PTstageconf.PTNormFactor = self.ConfigDict.PTNormFactor
        PTstageconf.PTNormType = self.ConfigDict.PTNormType
        PTstageconf.PTOutFile = self.ConfigDict.PTOutFile
        PTstageconf.PTOutFileType = self.ConfigDict.PTOutFileType
        PTstageconf.PTOutWindow = self.ConfigDict.PTOutWindow
        PTstageconf.BCInitWindow = self.ConfigDict.BCInitWindow
        print("# 11 - PT stage. Psychoacoustic Target stage.")
        stage = PTstage(OutS, OutInS, PTstageconf)
        if not stage.process():
            print("DRC: Psychoacoustic Target process fails")
            sys.exit()
        if PTstageconf.PTNormFactor > 0:
            stage.Normalize()
        if PTstageconf.PTOutFile is not None:
            stage.WriteOutput()
        OutS = stage.getOutSig()

        # 12 - PL stage. Peak Limiting stage.
    
        PLstageconf = PLParmsType()
        PLstageconf.PLType = self.ConfigDict.PLType
        PLstageconf.PLMaxGain = self.ConfigDict.PLMaxGain
        PLstageconf.PLStart = self.ConfigDict.PLStart
        PLstageconf.PLStartFreq = self.ConfigDict.PLStartFreq
        PLstageconf.PLEndFreq = self.ConfigDict.PLEndFreq
        PLstageconf.PLMultExponent = self.ConfigDict.PLMultExponent
        PLstageconf.PLOutWindow = self.ConfigDict.PLOutWindow
        PLstageconf.PLNormFactor = self.ConfigDict.PLNormFactor
        PLstageconf.PLNormType = self.ConfigDict.PLNormType 
        PLstageconf.PLOutFile = self.ConfigDict.PLOutFile
        PLstageconf.PLOutFileType = self.ConfigDict.PLOutFileType
        print("# 12 - PL stage. Peak Limiting stage.")
        stage = PLstage(OutS,PLstageconf)
        if not stage.process():
            print("DRC: Peak Limiting process fails")
            sys.exit()
        if PLstageconf.PLNormFactor > 0:
            stage.Normalize()
        if PLstageconf.PLOutFile is not None:
            stage.WriteOutput()
        OutS = stage.getOutSig()
    
        # 13 - RT stage. Ringing Truncation stage.

        RTstageconf = WFParmsType()
        RTstageconf.WFfilterType = self.ConfigDict.RTType
        RTstageconf.WFfilterFctn = self.ConfigDict.RTPrefilterFctn
        RTstageconf.WFWindowGap = self.ConfigDict.RTWindowGap
        RTstageconf.WFLowerWindow = self.ConfigDict.RTLowerWindow
        RTstageconf.WFUpperWindow = self.ConfigDict.RTUpperWindow
        RTstageconf.WFStartFreq = self.ConfigDict.RTStartFreq
        RTstageconf.WFEndFreq = self.ConfigDict.RTEndFreq
        RTstageconf.WFFilterLen = self.ConfigDict.RTFilterLen 
        RTstageconf.WFFSharpness = self.ConfigDict.RTFSharpness
        RTstageconf.WFBandSplit = self.ConfigDict.RTBandSplit
        RTstageconf.WFWindowExponent = self.ConfigDict.RTWindowExponent
        RTstageconf.WFFinalWindow = self.ConfigDict.RTOutWindow
        RTstageconf.WFRTFlag = 'Y'
        RTstageconf.WFNormFactor = self.ConfigDict.RTNormFactor
        RTstageconf.WFNormType = self.ConfigDict.RTNormType 
        RTstageconf.WFOutFile = self.ConfigDict.RTOutFile
        RTstageconf.WFOutFileType = self.ConfigDict.RTOutFileType
        if RTstageconf.WFfilterType[0] != 'N':
            print("# 13 - RT stage. Ringing Truncation stage.")
            stage = WFstage(OutS, RTstageconf)
            if not stage.process():
                print("DRC: Ringing Truncation process fails")
                sys.exit()
            if RTstageconf.WFNormFactor > 0:
                stage.Normalize()
            if RTstageconf.WFOutFile is not None:
                stage.WriteOutput()
            OutS = stage.getOutSig()
    
        # 14 - PS stage. Postfiltering Stage.
        
        PSstageconf = PSParmsType()
        PSstageconf.PSFilterType = self.ConfigDict.PSFilterType 
        PSstageconf.PSInterpolationType = self.ConfigDict.PSInterpolationType
        PSstageconf.PSMultExponent = self.ConfigDict.PSMultExponent
        PSstageconf.PSFilterLen = self.ConfigDict.PSFilterLen 
        PSstageconf.PSNumPoints = self.ConfigDict.PSNumPoints
        PSstageconf.PSPointsFile = self.ConfigDict.BCBaseDir + self.ConfigDict.PSPointsFile
        PSstageconf.PSMagType = self.ConfigDict.PSMagType
        PSstageconf.PSOutWindow = self.ConfigDict.PSOutWindow
        PSstageconf.PSNormFactor = self.ConfigDict.PSNormFactor
        PSstageconf.PSNormType = self.ConfigDict.PSNormType
        PSstageconf.PSOutFile = self.ConfigDict.PSOutFile
        PSstageconf.PSOutFileType = self.ConfigDict.PSOutFileType
        PSstageconf.ISPELowerWindow = self.ConfigDict.ISPELowerWindow
        print("# 14 - PS stage. Postfiltering Stage.")
        stage= PSstage(OutS,PSstageconf)
        if not stage.process():
            print("DRC: Postfiltering Stage process fails")
            sys.exit()
        if PSstageconf.PSNormFactor > 0:
            stage.Normalize()
        if PSstageconf.PSOutFile is not None:
            stage.WriteOutput()
        OutS = stage.getOutSig()
    
        # 15 - MS stage. Minimum Phase Filter Extraction stage.
        
        MSstageconf = MSParmsType()
        MSstageconf.MSMultExponent = self.ConfigDict.MSMultExponent
        MSstageconf.MSOutWindow = self.ConfigDict.MSOutWindow
        MSstageconf.MSNormFactor = self.ConfigDict.MSNormFactor
        MSstageconf.MSNormType = self.ConfigDict.MSNormType
        MSstageconf.MSOutFile = self.ConfigDict.MSOutFile
        MSstageconf.MSOutFileType = self.ConfigDict.MSOutFileType
        print(" # 15 - MS stage. Minimum Phase Filter Extraction stage.")
        stage = MSstage(OutS,MSstageconf)
        if not stage.process():
            print("DRC: Minimum Phase Filter Extraction process fails")
            sys.exit()
        if MSstageconf.MSNormFactor > 0:
            stage.Normalize()
        if MSstageconf.MSOutFile is not None:
            stage.WriteOutput()
    
        # 16 - TC stage. Test Convolution stage.
            
        TCstageconf = TCParmsType()
        TCstageconf.TCNormFactor = self.ConfigDict.TCNormFactor
        TCstageconf.TCNormType = self.ConfigDict.TCNormType
        TCstageconf.TCOutFile = self.ConfigDict.TCOutFile
        TCstageconf.TCOutFileType = self.ConfigDict.TCOutFileType
        TCstageconf.TCWNormFactor = self.ConfigDict.TCWNormFactor
        TCstageconf.TCWNormType = self.ConfigDict.TCWNormType
        TCstageconf.TCWOutFile = self.ConfigDict.TCWOutFile
        TCstageconf.TCWOutFileType = self.ConfigDict.TCWOutFileType
        TCstageconf.BCInitWindow = BCstageconf.BCInitWindow
        TCstageconf.ISPELowerWindow = ISstageconf.ISPELowerWindow
        TCstageconf.PSFilterType = PSstageconf.PSFilterType
        print("# 16 - TC stage. Test Convolution stage.")
        stage = TCstage(OutInS,OutS,TCstageconf)
        if not stage.process():
            print("DRC: Test Convolution process fails")
            sys.exit()
        if TCstageconf.TCNormFactor > 0:
            stage.Normalize()
        if TCstageconf.TCOutFile is not None:
            stage.WriteOutput()
        
                  
if __name__ == '__main__':
    inittime = time()
    print("Executing drc with %s parameters file"%sys.argv[-1])
    DRC = drc(sys.argv[-1])
    DRC.run()
    print('Total computing time: %lu s'%(time()-inittime))
