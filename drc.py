#! /usr/bin/env python

from pyDRC import *
from drccfg import *
from time import time
from sys import argv

def drc(DRCfile):

    ConfigDict = DRCCfg(DRCfile)
    if ConfigDict.CheckDRCCfg() == 1:
        return

    stages = []
        
    # 0 - BC stage. Base Configuration.
    
    BCstageconf = BCParmsType()
    BCstageconf.BCBaseDir = ConfigDict.BCBaseDir
    BCstageconf.BCInFile = ConfigDict.BCBaseDir + ConfigDict.BCInFile
    BCstageconf.BCSampleRate = ConfigDict.BCSampleRate
    BCstageconf.BCInFileType = ConfigDict.BCInFileType
    BCstageconf.BCImpulseCenterMode = ConfigDict.BCImpulseCenterMode
    BCstageconf.BCImpulseCenter = ConfigDict.BCImpulseCenter
    BCstageconf.BCInitWindow = ConfigDict.BCInitWindow
    BCstageconf.BCPreWindowLen = ConfigDict.BCPreWindowLen
    BCstageconf.BCPreWindowGap = ConfigDict.BCPreWindowGap
    BCstageconf.BCNormFactor = ConfigDict.BCNormFactor 
    BCstageconf.BCNormType = ConfigDict.BCNormType
    BCstageconf.BCOutFile = ConfigDict.BCOutFile
    BCstageconf.BCOutFileType = ConfigDict.BCOutFileType
    BCstageconf.BCDLType = ConfigDict.BCDLType
    BCstageconf.BCDLMinGain = ConfigDict.BCDLMinGain
    BCstageconf.BCDLStartFreq = ConfigDict.BCDLStartFreq 
    BCstageconf.BCDLEndFreq = ConfigDict.BCDLEndFreq
    BCstageconf.BCDLStart = ConfigDict.BCDLStart
    BCstageconf.BCDLMultExponent = ConfigDict.BCDLMultExponent
    stage = BCstage(BCstageconf)
    stage.process()
    OutInS = stage.getOutSig()

    # 1 - MC stage. Microphone Compensation.

    MCstageconf = MCParmsType()
    MCstageconf.MCFilterType = ConfigDict.MCFilterType 
    MCstageconf.MCInterpolationType = ConfigDict.MCInterpolationType
    MCstageconf.MCMultExponent = ConfigDict.MCMultExponent
    MCstageconf.MCFilterLen = ConfigDict.MCFilterLen 
    MCstageconf.MCNumPoints = ConfigDict.MCNumPoints
    MCstageconf.MCPointsFile = ConfigDict.BCBaseDir + ConfigDict.MCPointsFile 
    MCstageconf.MCMagType = ConfigDict.MCMagType
    MCstageconf.MCFilterFile = ConfigDict.MCFilterFile
    MCstageconf.MCFilterFileType = ConfigDict.MCFilterFileType
    MCstageconf.MCOutWindow = ConfigDict.MCOutWindow 
    MCstageconf.MCNormFactor = ConfigDict.MCNormFactor
    MCstageconf.MCNormType = ConfigDict.MCNormType
    MCstageconf.MCOutFile = ConfigDict.MCOutFile
    MCstageconf.MCOutFileType = ConfigDict.MCOutFileType
    stage = MCstage(OutInS, MCstageconf)
    stage.process()
    OutS = stage.getOutSig()

    # 2 - BCDL stage. Base Configuration Dip Limiting.

    BCDLstageconf = BCDLParmsType()
    BCDLstageconf.BCDLType = ConfigDict.BCDLType
    BCDLstageconf.BCDLMinGain = ConfigDict.BCDLMinGain
    BCDLstageconf.BCDLStartFreq = ConfigDict.BCDLStartFreq
    BCDLstageconf.BCDLEndFreq = ConfigDict.BCDLEndFreq
    BCDLstageconf.BCDLStart = ConfigDict.BCDLStart
    BCDLstageconf.BCDLMultExponent = ConfigDict.BCDLMultExponent
    BCDLstageconf.BCDLNormFactor = ConfigDict.BCDLNormFactor
    BCDLstageconf.BCDLNormType = ConfigDict.BCDLNormType
    BCDLstageconf.BCDLOutFile = ConfigDict.BCDLOutFile
    BCDLstageconf.BCDLOutFileType = ConfigDict.BCDLOutFileType
    stage = BCDLstage(OutS, BCDLstageconf)
    stage.process()
    OutS = stage.getOutSig()
    
    
    # 3 - HD stage. Homomorphic Deconvolution.

    HDstageconf = HDParmsType()
    HDstageconf.HDMultExponent = ConfigDict.HDMultExponent
    HDstageconf.HDMPNormFactor = ConfigDict.HDMPNormFactor
    HDstageconf.HDMPNormType = ConfigDict.HDMPNormType
    HDstageconf.HDMPOutFile = ConfigDict.HDMPOutFile
    HDstageconf.HDMPOutFileType = ConfigDict.HDMPOutFileType
    HDstageconf.HDEPNormType = ConfigDict.HDEPNormType
    HDstageconf.HDEPNormFactor = ConfigDict.HDEPNormFactor
    HDstageconf.HDEPOutFile = ConfigDict.HDEPOutFile
    HDstageconf.HDEPOutFileType = ConfigDict.HDEPOutFileType
    stage = HDstage(OutS,HDstageconf)
    stage.process()
    MPS = stage.getMPSig()
    EPS = stage.getEPSig()
    
    # 4 - MP stage. Minimum phase Prefiltering.

    MPstageconf = WFParmsType()
    MPstageconf.WFfilterType = ConfigDict.MPPrefilterType
    MPstageconf.WFfilterFctn = ConfigDict.MPPrefilterFctn
    MPstageconf.WFWindowGap = ConfigDict.MPWindowGap
    MPstageconf.WFLowerWindow = ConfigDict.MPLowerWindow
    MPstageconf.WFUpperWindow = ConfigDict.MPUpperWindow
    MPstageconf.WFStartFreq = ConfigDict.MPStartFreq
    MPstageconf.WFEndFreq = ConfigDict.MPEndFreq
    MPstageconf.WFFilterLen = ConfigDict.MPFilterLen
    MPstageconf.WFFSharpness = ConfigDict.MPFSharpness
    MPstageconf.WFBandSplit = ConfigDict.MPBandSplit
    MPstageconf.WFWindowExponent = ConfigDict.MPWindowExponent
    MPstageconf.WFFinalWindow = ConfigDict.MPPFFinalWindow
    MPstageconf.WFRTFlag = 'N'
    MPstageconf.WFNormFactor = ConfigDict.MPPFNormFactor
    MPstageconf.WFNormType = ConfigDict.MPPFNormType
    MPstageconf.WFOutFile = ConfigDict.MPPFOutFile 
    MPstageconf.WFOutFileType = ConfigDict.MPPFOutFileType
    stage = WFstage(MPS,MPstageconf)
    stage.process()
    MPS = stage.getOutSig()
    
    # 5 - DL stage. Dip Limiting.

    DLstageconf = DLParmsType()
    DLstageconf.DLType = ConfigDict.DLType
    DLstageconf.DLMinGain = ConfigDict.DLMinGain
    DLstageconf.DLStartFreq = ConfigDict.DLStartFreq
    DLstageconf.DLEndFreq = ConfigDict.DLEndFreq
    DLstageconf.DLStart = ConfigDict.DLStart
    DLstageconf.DLMultExponent = ConfigDict.DLMultExponent
    DLstageconf.DLNormFactor = ConfigDict.MPPFNormFactor
    DLstageconf.DLNormType = ConfigDict.MPPFNormType
    DLstageconf.DLOutFile = ConfigDict.MPPFOutFile
    DLstageconf.DLOutFileType = ConfigDict.MPPFOutFileType
    stage = DLstage(MPS,DLstageconf)
    stage.process()
    MPS = stage.getOutSig()
    
    # 6 - HR stage. Homomorphic recovery.

    HRstageconf = HRParmsType()
    HRstageconf.HRMPHDRecover = ConfigDict.MPHDRecover
    HRstageconf.HRMPEPPreserve = ConfigDict.MPEPPreserve
    HRstageconf.HRMPHDMultExponent = ConfigDict.MPHDMultExponent
    HRstageconf.HRMPFinalWindow = ConfigDict.MPPFFinalWindow
    HRstageconf.HRMPOutFileType = ConfigDict.MPPFOutFileType
    HRstageconf.HRMPNormFactor = ConfigDict.MPPFNormFactor
    HRstageconf.HRMPNormType = ConfigDict.MPPFNormType
    HRstageconf.HRMPOutFile = ConfigDict.MPPFOutFile
    HRstageconf.HRMPOutFileType = ConfigDict.MPPFOutFileType
    HRstageconf.HREPNormFactor = ConfigDict.EPPFNormFactor
    HRstageconf.HREPNormType = ConfigDict.EPPFNormType
    HRstageconf.HREPOutFile = ConfigDict.EPPFOutFile
    HRstageconf.HREPOutFileType = ConfigDict.EPPFOutFileType
    stage = HRstage(MPS,EPS,HRstageconf)
    stage.process()    
    MPS = stage.getMPSig()
    EPS = stage.getEPSig()

    # 7 - EP stage. Excess phase Prefiltering.
 
    EPstageconf = WFParmsType()
    EPstageconf.WFfilterType = ConfigDict.EPPrefilterType
    EPstageconf.WFfilterFctn = ConfigDict.EPPrefilterFctn
    EPstageconf.WFWindowGap = ConfigDict.EPWindowGap 
    EPstageconf.WFLowerWindow = ConfigDict.EPLowerWindow
    EPstageconf.WFUpperWindow = ConfigDict.EPUpperWindow
    EPstageconf.WFStartFreq = ConfigDict.EPStartFreq
    EPstageconf.WFEndFreq = ConfigDict.EPEndFreq 
    EPstageconf.WFFilterLen = ConfigDict.EPFilterLen
    EPstageconf.WFFSharpness = ConfigDict.EPFSharpness
    EPstageconf.WFBandSplit = ConfigDict.EPBandSplit
    EPstageconf.WFWindowExponent = ConfigDict.EPWindowExponent
    EPstageconf.WFFinalWindow = ConfigDict.EPPFFinalWindow
    EPstageconf.WFRTFlag = 'N'
    EPstageconf.WFNormFactor = ConfigDict.EPPFNormFactor
    EPstageconf.WFNormType = ConfigDict.EPPFNormType
    EPstageconf.WFOutFile = ConfigDict.EPPFOutFile
    EPstageconf.WFOutFileType = ConfigDict.EPPFOutFileType
    stage = WFstage(EPS,EPstageconf)
    stage.process()
    EPS = stage.getOutSig()
    
    # 8 - ER stage. Excess phase recovery.

    ERstageconf = ERParmsType()
    ERstageconf.EREPFlatGain = ConfigDict.EPPFFlatGain
    ERstageconf.EREPOGainFactor = ConfigDict.EPPFOGainFactor
    ERstageconf.EREPFlatType = ConfigDict.EPPFFlatType
    ERstageconf.EREPFGMultExponent = ConfigDict.EPPFFGMultExponent
    ERstageconf.EREPFinalWindow = ConfigDict.EPPFFinalWindow
    ERstageconf.EREPNormFactor = ConfigDict.EPPFNormFactor
    ERstageconf.EREPNormType = ConfigDict.EPPFNormType
    ERstageconf.EREPOutFile = ConfigDict.EPPFOutFile
    ERstageconf.EREPOutFileType = ConfigDict.EPPFOutFileType
    stage = ERstage(EPS,ERstageconf)
    stage.process()    
    EPS = stage.getOutSig()

    # 9 - IS stage.Inversion Stage.

    ISstageconf = ISParmsType()
    ISstageconf.PCOutWindow = ConfigDict.PCOutWindow 
    ISstageconf.PCNormFactor = ConfigDict.PCNormFactor
    ISstageconf.PCNormType = ConfigDict.PCNormType
    ISstageconf.PCOutFile = ConfigDict.PCOutFile
    ISstageconf.PCOutFileType = ConfigDict.PCOutFileType
    ISstageconf.ISType = ConfigDict.ISType
    ISstageconf.ISPETType = ConfigDict.ISPETType
    ISstageconf.ISPrefilterFctn = ConfigDict.ISPrefilterFctn
    ISstageconf.ISPELowerWindow = ConfigDict.ISPELowerWindow
    ISstageconf.ISPEUpperWindow = ConfigDict.ISPEUpperWindow 
    ISstageconf.ISPEStartFreq = ConfigDict.ISPEStartFreq 
    ISstageconf.ISPEEndFreq = ConfigDict.ISPEEndFreq 
    ISstageconf.ISPEFilterLen = ConfigDict.ISPEFilterLen
    ISstageconf.ISPEFSharpness = ConfigDict.ISPEFSharpness
    ISstageconf.ISPEBandSplit = ConfigDict.ISPEBandSplit
    ISstageconf.ISPEWindowExponent = ConfigDict.ISPEWindowExponent
    ISstageconf.ISPEOGainFactor = ConfigDict.ISPEOGainFactor
    ISstageconf.ISSMPMultExponent = ConfigDict.ISSMPMultExponent
    ISstageconf.ISOutWindow = ConfigDict.ISOutWindow 
    ISstageconf.ISNormFactor = ConfigDict.ISNormFactor
    ISstageconf.ISNormType = ConfigDict.ISNormType
    ISstageconf.ISOutFile = ConfigDict.ISOutFile
    ISstageconf.ISOutFileType = ConfigDict.ISOutFileType
    stage = ISstage(MPS,EPS,ISstageconf)
    stage.process()
    OutS = stage.getISOutSig()
    
    # 10 - PT stage.Psychoacoustic Stage.

    PTstageconf = PTParmsType()
    PTstageconf.PTType = ConfigDict.PTType
    PTstageconf.PTReferenceWindow = ConfigDict.PTReferenceWindow
    PTstageconf.PTDLType = ConfigDict.PTDLType
    PTstageconf.PTDLMinGain = ConfigDict.PTDLMinGain
    PTstageconf.PTDLStartFreq = ConfigDict.PTDLStartFreq
    PTstageconf.PTDLEndFreq = ConfigDict.PTDLEndFreq
    PTstageconf.PTDLStart = ConfigDict.PTDLStart
    PTstageconf.PTDLMultExponent = ConfigDict.PTDLMultExponent 
    PTstageconf.PTBandWidth = ConfigDict.PTBandWidth
    PTstageconf.PTPeakDetectionStrength = ConfigDict.PTPeakDetectionStrength
    PTstageconf.PTMultExponent = ConfigDict.PTMultExponent
    PTstageconf.PTFilterLen = ConfigDict.PTFilterLen
    PTstageconf.PTFilterFile = ConfigDict.PTFilterFile
    PTstageconf.PTFilterFileType = ConfigDict.PTFilterFileType
    PTstageconf.PTNormFactor = ConfigDict.PTNormFactor
    PTstageconf.PTNormType = ConfigDict.PTNormType
    PTstageconf.PTOutFile = ConfigDict.PTOutFile
    PTstageconf.PTOutFileType = ConfigDict.PTOutFileType
    PTstageconf.PTOutWindow = ConfigDict.PTOutWindow
    PTstageconf.BCInitWindow = ConfigDict.BCInitWindow
    stage = PTstage(OutS, OutInS, PTstageconf)
    stage.process()
    OutS = stage.getOutSig()

    # 11 - PL stage. Peak Limiting.
    
    PLstageconf = PLParmsType()
    PLstageconf.PLType = ConfigDict.PLType
    PLstageconf.PLMaxGain = ConfigDict.PLMaxGain
    PLstageconf.PLStart = ConfigDict.PLStart
    PLstageconf.PLStartFreq = ConfigDict.PLStartFreq
    PLstageconf.PLEndFreq = ConfigDict.PLEndFreq
    PLstageconf.PLMultExponent = ConfigDict.PLMultExponent
    PLstageconf.PLOutWindow = ConfigDict.PLOutWindow
    PLstageconf.PLNormFactor = ConfigDict.PLNormFactor
    PLstageconf.PLNormType = ConfigDict.PLNormType 
    PLstageconf.PLOutFile = ConfigDict.PLOutFile
    PLstageconf.PLOutFileType = ConfigDict.PLOutFileType
    stage = PLstage(OutS,PLstageconf)
    stage.process()
    OutS = stage.getOutSig()
    
    # 12 - RT stage. Ringing Truncation.

    RTstageconf = WFParmsType()
    RTstageconf.WFfilterType = ConfigDict.RTType
    RTstageconf.WFfilterFctn = ConfigDict.RTPrefilterFctn
    RTstageconf.WFWindowGap = ConfigDict.RTWindowGap
    RTstageconf.WFLowerWindow = ConfigDict.RTLowerWindow
    RTstageconf.WFUpperWindow = ConfigDict.RTUpperWindow
    RTstageconf.WFStartFreq = ConfigDict.RTStartFreq
    RTstageconf.WFEndFreq = ConfigDict.RTEndFreq
    RTstageconf.WFFilterLen = ConfigDict.RTFilterLen 
    RTstageconf.WFFSharpness = ConfigDict.RTFSharpness
    RTstageconf.WFBandSplit = ConfigDict.RTBandSplit
    RTstageconf.WFWindowExponent = ConfigDict.RTWindowExponent
    RTstageconf.WFFinalWindow = ConfigDict.RTOutWindow
    RTstageconf.WFRTFlag = 'Y'
    RTstageconf.WFNormFactor = ConfigDict.RTNormFactor
    RTstageconf.WFNormType = ConfigDict.RTNormType 
    RTstageconf.WFOutFile = ConfigDict.RTOutFile
    RTstageconf.WFOutFileType = ConfigDict.RTOutFileType
    if RTstageconf.WFfilterType[0] != 'N':
        stage = WFstage(OutS, RTstageconf)
        stage.process()
        OutS = stage.getOutSig()
    
    # 13 - PS stage. Postfiltering Stage.

    PSstageconf = PSParmsType()
    PSstageconf.PSFilterType = ConfigDict.PSFilterType 
    PSstageconf.PSInterpolationType = ConfigDict.PSInterpolationType
    PSstageconf.PSMultExponent = ConfigDict.PSMultExponent
    PSstageconf.PSFilterLen = ConfigDict.PSFilterLen 
    PSstageconf.PSNumPoints = ConfigDict.PSNumPoints
    PSstageconf.PSPointsFile = ConfigDict.BCBaseDir + ConfigDict.PSPointsFile
    PSstageconf.PSMagType = ConfigDict.PSMagType
    PSstageconf.PSOutWindow = ConfigDict.PSOutWindow
    PSstageconf.PSNormFactor = ConfigDict.PSNormFactor
    PSstageconf.PSNormType = ConfigDict.PSNormType
    PSstageconf.PSOutFile = ConfigDict.PSOutFile
    PSstageconf.PSOutFileType = ConfigDict.PSOutFileType
    PSstageconf.ISPELowerWindow = ConfigDict.ISPELowerWindow
    stage= PSstage(OutS,PSstageconf)
    stage.process()
    OutS = stage.getOutSig()
    
    # 14 - MS stage. Minimum phase filter extraction.

    MSstageconf = MSParmsType()
    MSstageconf.MSMultExponent = ConfigDict.MSMultExponent
    MSstageconf.MSOutWindow = ConfigDict.MSOutWindow
    MSstageconf.MSNormFactor = ConfigDict.MSNormFactor
    MSstageconf.MSNormType = ConfigDict.MSNormType
    MSstageconf.MSOutFile = ConfigDict.MSOutFile
    MSstageconf.MSOutFileType = ConfigDict.MSOutFileType
    stage = MSstage(OutS,MSstageconf)
    stage.process()
    
    # 15 - TC stage. Test Convolution.

    TCstageconf = TCParmsType()
    TCstageconf.TCNormFactor = ConfigDict.TCNormFactor
    TCstageconf.TCNormType = ConfigDict.TCNormType
    TCstageconf.TCOutFile = ConfigDict.TCOutFile
    TCstageconf.TCOutFileType = ConfigDict.TCOutFileType
    TCstageconf.TCWNormFactor = ConfigDict.TCWNormFactor
    TCstageconf.TCWNormType = ConfigDict.TCWNormType
    TCstageconf.TCWOutFile = ConfigDict.TCWOutFile
    TCstageconf.TCWOutFileType = ConfigDict.TCWOutFileType
    TCstageconf.BCInitWindow = BCstageconf.BCInitWindow
    TCstageconf.ISPELowerWindow = ISstageconf.ISPELowerWindow
    TCstageconf.PSFilterType = PSstageconf.PSFilterType
    
    stage = TCstage(OutInS,OutS,TCstageconf)
    stage.process()
        
                  
if __name__ == '__main__':
    inittime = time()
    print("Executing drc with %s parameters file"%argv[-1])
    drc(argv[-1])
    print('Total computing time: %lu s'%(time()-inittime))
