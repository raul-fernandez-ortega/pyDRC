from sys import argv
from os.path import exists, isdir
import re

class DRCCfg:
    # Base file directory
    BCBaseDir = './'

    # BC = Base Configuration
    BCInFile = None
    BCSampleRate = None
    BCInFileType = None
    BCImpulseCenterMode = None
    BCImpulseCenter = 0
    BCInitWindow = 0
    BCPreWindowLen = 0
    BCPreWindowGap = 0
    BCNormFactor = 0.0
    BCNormType = None
    BCOutFile = None
    BCOutFileType = None

    # HD = Homomorphic Deconvolution
    HDMultExponent = 0
    HDMPNormFactor = 0.0
    HDMPNormType = None
    HDMPOutFile = None
    HDMPOutFileType = None
    HDEPNormFactor = 0.0
    HDEPNormType = None
    HDEPOutFile = None
    HDEPOutFileType = None

    # MP = Minimum phase band windowing stage
    MPPrefilterType = None
    MPPrefilterFctn = None
    MPWindowGap = 0
    MPLowerWindow = 0
    MPUpperWindow = 0
    MPStartFreq = 0
    MPEndFreq = 0
    MPFilterLen = 0
    MPFSharpness = 0.0
    MPBandSplit = 0
    MPWindowExponent = 0.0
    MPHDRecover = None
    MPEPPreserve = None
    MPHDMultExponent = 0
    MPPFFinalWindow = 0
    MPPFNormFactor = 0
    MPPFNormType = None
    MPPFOutFile = None
    MPPFOutFileType = None

    # DL = Dip limiting stage
    DLType = None
    DLMinGain = 0.0
    DLStartFreq = 0
    DLEndFreq = 0
    DLStart = 0.0
    DLMultExponent = 0
    
    # EP = Excess phase phase band windowing stage
    EPPrefilterType = None
    EPPrefilterFctn = None
    EPWindowGap = None
    EPLowerWindow = 0
    EPUpperWindow = 0
    EPStartFreq = 0
    EPEndFreq = 0
    EPFilterLen = 0
    EPFSharpness = 0.0
    EPBandSplit = 0
    EPWindowExponent = 0.0
    EPPFFlatGain = 0.0
    EPPFOGainFactor = 0.0
    EPPFFlatType = None
    EPPFFGMultExponent = None
    EPPFFinalWindow = 0
    EPPFNormFactor = 0.0
    EPPFNormType = None
    EPPFOutFile = None
    EPPFOutFileType = None
    
    # PC = Prefiltering completion stage
    PCOutWindow = 0
    PCNormFactor = 0.0
    PCNormType = None
    PCOutFile = None
    PCOutFileType = None
    
    # IS = Inversion stage
    ISType = None
    ISPETType = None
    ISPrefilterFctn = None
    ISPELowerWindow = 0 
    ISPEUpperWindow = 0
    ISPEStartFreq = 0
    ISPEEndFreq = 0
    ISPEFilterLen = 0
    ISPEFSharpness = 0.0
    ISPEBandSplit = 0
    ISPEWindowExponent = 0.0 
    ISPEOGainFactor = 0.0
    ISSMPMultExponent = 0
    ISOutWindow = 0
    ISNormFactor = 0.0
    ISNormType = None
    ISOutFile = None
    ISOutFileType = None
    
    # PL = Peak limiting stage
    PLType = None
    PLMaxGain = 0.0
    PLStart = 0.0
    PLStartFreq = 00
    PLEndFreq = 0
    PLMultExponent = 0
    PLOutWindow = 0
    PLNormFactor = 0.0
    PLNormType = None
    PLOutFile = None
    PLOutFileType = None
    
    # RT = Ringing truncation stage
    RTType = None
    RTPrefilterFctn = None
    RTWindowGap = 0
    RTLowerWindow = 0
    RTUpperWindow = 0
    RTStartFreq = 0
    RTEndFreq = 0
    RTFilterLen = 0
    RTFSharpness = 0.0
    RTBandSplit = 0
    RTWindowExponent = 0.0
    RTOutWindow = 0
    RTNormFactor = 0.0
    RTNormType = 0
    RTOutFile = None
    RTOutFileType = None
    
    PTType = None
    PTReferenceWindow = 0
    PTDLType = None
    PTDLMinGain = 0.0
    PTDLStartFreq = 0.0
    PTDLEndFreq = 0.0
    PTDLStart = 0
    PTDLMultExponent = 0.0 
    PTBandWidth = 0.0
    PTPeakDetectionStrength = 0.0
    PTMultExponent = 0
    PTFilterLen = 0
    PTFilterFile = None
    PTFilterFileType = None
    PTNormFactor = 0.0
    PTNormType = None
    PTOutFile = None
    PTOutFileType = None
    PTOutWindow = 0
    

    # PS = Target respose stage
    PSFilterType = None
    PSInterpolationType = None
    PSMultExponent = 0
    PSFilterLen = 0
    PSNumPoints = 0
    PSPointsFile = None
    PSMagType = None
    PSOutWindow = 0
    PSNormFactor = 0.0
    PSNormType = None
    PSOutFile = None
    PSOutFileType = None
    
    # MC = Mic compensation stage
    MCFilterType = 0
    MCInterpolationType = None
    MCMultExponent = 0
    MCFilterLen = 0
    MCNumPoints = 0
    MCPointsFile = None
    MCMagType = None
    MCOutWindow = 0
    MCNormFactor = 0.0
    MCNormType = None
    MCOutFile = None
    MCOutFileType = None
    
    # Minimum phase filter extraction stage
    MSMultExponent = 0
    MSOutWindow = 0
    MSNormFactor = 0.0
    MSNormType = None
    MSOutFile = None
    MSOutFileType = None
    
    # Test convolution stage
    TCNormFactor = 0.0
    TCNormType = None
    TCOutFile = None
    TCOutFileType = None
    TCWNormFactor = 0
    TCWNormType = None
    TCWOutFile = None
    TCWOutFileType = None

    def __init__(self, DRCfile):
        f = open(DRCfile,'r')
        expr = re.compile("^#{0}(?P<NAME>\w+)\s*=\s*(?P<VALUE>(\S+)).*")
        for line in f.readlines():
            m1 = expr.match(line)
            if m1:
                try:
                    print("Parameter %s ---> %s"%(m1.group('NAME'),m1.group('VALUE')))
                    self.__dict__[m1.group('NAME')] = int(m1.group('VALUE'))
                except:
                    try:
                        self.__dict__[m1.group('NAME')] = float(m1.group('VALUE'))
                    except:
                        self.__dict__[m1.group('NAME')]= m1.group('VALUE')
        f.close()
        keep_param = None
        for arg in argv:
            if arg[:2]=='--':
                if arg.find('==')>0:
                    param = arg[:arg.find('==')]
                    value = arg[arg.find('==')+2:]
                    if param in dir(self):
                        print("Setting",param,"=",value)
                        try:
                            self.__dict__[param] = int(value)
                        except:
                            try:
                                self.__dict__[param] = float(value)
                            except:
                                self.__dict__[param]= value
                    else:
                        print(param,"not a correct DRC configuration parameter.")
                else:
                    keep_param = arg[2:]
            elif keep_param:
                if keep_param in dir(self):
                    print("Setting",keep_param,"=",arg)
                    try:
                        self.__dict__[keep_param] = int(arg)
                    except:
                        try:
                            self.__dict__[keep_param] = float(arg)
                        except:
                            self.__dict__[keep_param]= arg
                else:
                    print(keep_param,"not a correct DRC configuration parameter.")
                keep_param = None


    def CheckDRCCfg(self):

        PWLen = 0
        ################################################################################
        #                                                                              #
        # BC - Base Configuration  configuration check                                 #
        #                                                                              #
        ################################################################################
        if self.BCBaseDir[-1]!='/':
            self.BCBaseDir +='/'
        if not isdir(self.BCBaseDir):
            print("BC->BCBaseDir: No base directory found.")
            return 1
        if not self.BCInFile:
            print("BC->BCInFile: No input file name supplied.")
            return 1
        if not self.BCInFileType:
            print("BC->BCInFileType: No input file type supplied.")
            return 1
        if not exists(self.BCBaseDir + self.BCInFile):
            print("BC->BCInFile: Input file does not exists.")
            return 1
        if not self.BCImpulseCenterMode:
            print("BC->BCImpulseCenterMode: No impulse center mode supplied.")
            return 1
        if self.BCNormFactor > 0 and not self.BCNormType:
            print("BC->BCNormType: No input normalization type supplied.")
            return 1
        if self.BCInitWindow < 3:
            print("BC->BCInitWindow: Initial window should be at least 3.")
            return 1
        if self.BCPreWindowGap < 0:
            print("BC->BCPreWindowGap: BCPreWindowGap can't be less than 0.")
            return 1
        if not self.BCPreWindowLen:
            print("BC->BCPreWindowLen: No value supplied.")
            return 1
        if not self.BCPreWindowGap:
            print("BC->BCPreWindowGap: No value supplied.")
            return 1
        if self.BCPreWindowLen > 0:
            if self.BCPreWindowLen + self.BCPreWindowGap > self.BCInitWindow:
                print("BC: BCPreWindowLen + BCPreWindowGap must be less than BCInitWindow.")
                return 1
            
        ################################################################################
        #                                                                              #
        # HD - Homomorphic Deconvolution configuration check                           #
        #                                                                              #
        ################################################################################
    
        if self.HDMPNormFactor > 0 and not self.HDMPNormType:
            print("HD->HDMPNormType: No MP normalization type supplied.")
            return 1

        if self.HDEPNormFactor > 0 and not self.HDEPNormType:
            print("HD->HDEPNormType: No EP normalization type supplied.")
            return 1
        if self.HDMPOutFile and not self.HDMPOutFileType:
            print("HD->HDMPOutFileType: No MP output file type supplied.")
            return 1
        if self.HDEPOutFile and not self.HDEPOutFileType:
            print("HD->HDEPOutFileType: No EP output file type supplied.")
            return 1
        
        ################################################################################
        #                                                                              #
        # MP - Minimum phase Prefiltering configuration check                          #
        #                                                                              #
        ################################################################################
    
        if not self.MPPrefilterType:
            print("MP->MPPrefilterType: No MPPrefilterType supplied.")
            return 1
        if self.MPPrefilterType[0] != 'B' and self.MPPrefilterType[0] != 'b' \
               and self.MPPrefilterType[0] != 'S' and self.MPPrefilterType[0] != 's':
            print("MP->MPPrefilterType: Invalid MPPrefilterType supplied.")
            return 1
        if not self.MPPrefilterFctn:
            print("MP->MPPrefilterFctn: No MPPrefilterFctn supplied.")
            return 1
        if self.MPPrefilterFctn[0] != 'P' and self.MPPrefilterFctn[0] != 'B':
            print("MP->MPPrefilterFctn: Invalid MPPrefilterFctn supplied.")
            return 1
        if self.MPWindowGap < 0:
            print("MP->MPWindowGap: MPWindowGap must be greater or equal to 0.")
            return 1
        if self.MPLowerWindow <= 0:
            print("MP->MPLowerWindow: No MPLowerWindow supplied.")
            return 1
        if self.MPLowerWindow > 2 * self.BCInitWindow:
            print("MP->MPLowerWindow: MPLowerWindow can't be greater than 2 * BCInitWindow.")
            return 1
        if self.MPUpperWindow <= 0:
            print("MP->MPUpperWindow: No MPUpperWindow supplied.")
            return 1
        if self.MPFSharpness <= 0.0:
            print("MP->MPFSharpness: MPFSharpness must be greater than 0.")
            return 1
        if self.MPUpperWindow > 2 * self.BCInitWindow:
            print("MP->MPUpperWindow: MPUpperWindow can't be greater than 2 * BCInitWindow.")
            return 1
        if self.MPWindowExponent <= 0.0:
            print("MP->MPWindowExponent: MPWindowExponent must be greater than 0.")
            return 1
        if not self.MPHDRecover:
            print("MP->MPHDRecover: No MPHDRecover supplied.")
            return 1
        if not self.MPEPPreserve:
            print("MP->MPEPPreserve: No MPEPPreserve supplied.")
            return 1
        if self.MPPFNormFactor > 0 and not self.MPPFNormType:
            print("MP->MPPFNormType: No PF normalization type supplied.")
            return 1
        if self.MPPFOutFile and not self.MPPFOutFileType:
            print("MP->MPPFOutFileType: No MPPF output file type supplied.")
            return 1
        if self.MPPFFinalWindow > self.MPLowerWindow + self.MPFilterLen - 1:
            print("MP->MPPFFinalWindow: MPPFFinalWindow can't be greater than MPLowerWindow + MPFilterLen - 1.")
            return 1
        if self.MPPFFinalWindow <= 0:
            PWLen = self.MPLowerWindow + self.MPFilterLen - 1
        else:
            PWLen = self.MPPFFinalWindow
            
        ################################################################################
        #                                                                              #
        # DL - Dip Limiting configuration check                                        #
        #                                                                              #
        ################################################################################

        if not self.DLType:
            print("DL->DLType: No DLType supplied.")
            return 1
        if self.DLType[0] != 'L' and self.DLType[0] != 'M':
            print("DL['DLType: Invalid dip limiting type supplied.")
            return 1
        if self.DLStart < 0.0:
            print("DL->DLStart: DLStart must be greater than or equal to 0.")
            return 1
        
        ################################################################################
        #                                                                              #
        # EP - Excess phase Prefiltering configuration check                           #
        #                                                                              #
        ################################################################################

        if not self.EPPrefilterType:
            print("EP->EPPrefilterType: No EPPrefilterType supplied.")
            return 1
        if self.EPPrefilterType[0] != 'B' and self.EPPrefilterType[0] != 'b' \
               and self.EPPrefilterType[0] != 'S' and self.EPPrefilterType[0] != 's':
            print("EP->EPPrefilterType: Invalid EPPrefilterType supplied.")
            return 1
        if not self.EPPrefilterFctn:
            print("EP->EPPrefilterFctn: No EPPrefilterFctn supplied.")
            return 1
        if self.EPPrefilterFctn[0] != 'P' and self.EPPrefilterFctn[0] != 'B':
            print("EP->EPPrefilterFctn: Invalid EPPrefilterFctn supplied.")
            return 1
        if self.EPWindowGap < 0:
            print("EP->EPWindowGap: EPWindowGap must be greater or equal to 0.")
            return 1
        if self.EPLowerWindow <= 0:
            print("EP->EPLowerWindow: EPLowerWindow must be greater than 0.")
            return 1			
        if self.EPLowerWindow > 2 * self.BCInitWindow:
            print("EP->EPLowerWindow: EPLowerWindow can't be greater than 2 * BCInitWindow.")
            return 1
        if self.EPUpperWindow <= 0:
            print("EP->EPUpperWindow: EPUpperWindow must be greater than 0.")
            return 1
        if self.EPUpperWindow > 2 * self.BCInitWindow:
            print("EP->EPUpperWindow: EPUpperWindow can't be greater than 2 * BCInitWindow.")
            return 1
        if self.EPFSharpness <= 0.0:
            print("EP->EPFSharpness: EPFSharpness must be greater than 0.")
            return 1
        if self.EPWindowExponent <= 0.0:
            print("EP->EPWindowExponent: EPWindowExponent must be greater than 0.")
            return 1
        if self.EPPFFlatGain > 0 and not self.EPPFFlatType:
            print("EP->EPPFFlatType: No EPPFFlatType supplied.")
            return 1
        if self.EPPFFlatType[0] != 'L' and self.EPPFFlatType[0] != 'M' and self.EPPFFlatType[0] != 'D':
            print("EP->EPPFFlatType: Invalid EPPFFlatType supplied.")
            return 1
        if self.EPPFNormFactor > 0 and not self.EPPFNormType:
            print("EP->EPPFNormType: No PF normalization type supplied.")
            return 1
        if self.EPPFOutFile and not self.EPPFOutFileType:
            print("EP->EPPFOutFileType: No EPPF output file type supplied.")
            return 1
        #if not self.EPPFFinalWindow:
        #    print "EP->EPPFFinalWindow: No EPPF final window length supplied."
        #    return 1
        if self.EPPFFinalWindow > self.EPLowerWindow + self.EPFilterLen - 1:
            print("EP->EPPFFinalWindow: EPPFFinalWindow can't be greater than EPLowerWindow + EPFilterLen - 1.")
            return 1
        if self.EPPFFinalWindow <= 0:
            PWLen += self.EPLowerWindow + self.EPFilterLen - 2
        else:
            PWLen += self.EPPFFinalWindow - 1

        ################################################################################
        #                                                                              #
        # PC - Prefilter Completion configuration check                                #
        #                                                                              #
        ################################################################################

        if self.PCOutWindow < 0:
            print("PC->PCOutWindow: No PCOutWindow supplied.")
            return 1
        if self.PCNormFactor > 0  and not self.PCNormType:
            print("PC->PCNormType: No normalization type supplied.")
            return 1
        if self.PCOutFile and not self.PCOutFileType:
            print("PC->PCOutFileType: No output file type supplied.")
            return 1
        if self.PCOutWindow > PWLen:
            print("PC->PCOutWindow: PCOutWindow too big.")
            return 1
	
        ################################################################################
        #                                                                              #
        # IS - Inversion Stage configuration check                                     #
        #                                                                              #
        ################################################################################

        if not self.ISType:
            print("IS->ISType: No inversion stage type supplied.")
            return 1
        if self.ISType[0] != 'L' and self.ISType[0] != 'T':
            print("IS->ISType: Invalid inversion stage type supplied.")
            return 1
        if not self.ISPETType:
            print("IS->ISPETType: No pre echo truncation type supplied.")
            return 1
        if self.ISPETType[0] != 'f' and self.ISPETType[0] != 's':
            print("IS->ISPETType: Invalid pre echo truncation type supplied.")
            return 1
        if not self.ISPrefilterFctn:
            print("IS->ISPrefilterFctn: No ISPrefilterFctn supplied.")
            return 1
        if self.ISPrefilterFctn[0] != 'P' and self.ISPrefilterFctn[0] != 'B':
            print("IS->ISPrefilterFctn: Invalid ISPrefilterFctn supplied.")
            return 1
        if self.ISPEFSharpness <= 0.0:
            print("IS->ISPEFSharpness: ISPEFSharpness must be greater than 0.")
            return 1
        if self.ISNormFactor and self.ISNormFactor > 0 and not self.ISNormType:
            print("IS->ISNormType: No normalization type supplied.")
            return 1
        if self.ISOutFile and not self.ISOutFileType:
            print("IS->ISOutFileType: No output file type supplied.")
            return 1
        if self.ISOutWindow < 0:
            print("IS->ISOutWindow: ISOutWindow must be greater than 0.")
            return 1
        if self.ISType[0] == 'L' and self.ISOutWindow > self.PCOutWindow:
            print("IS->ISOutWindow: ISOutWindow too big.")
            return 1
        if self.ISType[0] == 'T' and self.ISOutWindow > PWLen:
            print("IS->ISOutWindow: ISOutWindow too big.")
            return 1
        if self.ISOutWindow > 0:
            PWLen = self.ISOutWindow

        ################################################################################
        #                                                                              #
        # PL - Peak Limiting configuration check                                       #
        #                                                                              #
        ################################################################################

        if not self.PLType:
            print("PL->PLType: No peak limiting type supplied.")
            return 1
        if self.PLType[0] != 'L' and self.PLType[0] != 'M':
            print("PL->PLType: Invalid peak limiting type supplied.")
            return 1
        if self.PLStart < 0.0:
            print("PL->PLStart: PLStart must be greater than or equal to 0.")
            return 1
        if self.PLNormFactor and not self.PLNormType:
            print("PL->PLNormType: No normalization type supplied.")
            return 1
        if self.PLOutFile and not self.PLOutFileType:
            print("PL->PLOutFileType: No output file type supplied.")
            return 1
        if self.PLOutWindow > PWLen:
            print("PL->PLOutWindow: PLOutWindow too big.")
            return 1
        if self.PLOutWindow > 0:
            PWLen = self.PLOutWindow
            
        ################################################################################
        #                                                                              #
        # RT - Ringing Truncation configuration check                                  #
        #                                                                              #
        ################################################################################

        if not self.RTType:
            print("RT->RTType: No RTType supplied.")
            return 1
        if self.RTType[0] != 'N' and self.RTType[0] != 'B' and self.RTType[0] != 'b'\
               and self.RTType[0] != 'S' and self.RTType[0] != 's':
            print("RT->RTType: Invalid RTType supplied.")
            return 1
        if not self.RTPrefilterFctn:
            print("RT->RTPrefilterFctn: No RTPrefilterFctn supplied.")
            return 1
        if self.RTPrefilterFctn[0] != 'P' and self.RTPrefilterFctn[0] != 'B':
            print("RT->RTPrefilterFctn: Invalid RTPrefilterFctn supplied.")
            return 1
        if self.RTWindowGap < 0:
            print("RT->RTWindowGap: RTWindowGap must be greater or equal to 0.")
            return 1
        if self.RTLowerWindow <= 0:
            print("RT->RTLowerWindow: RTLowerWindow must be greater than 0.")
            return 1
        if self.RTLowerWindow > PWLen:
            print("RT->RTLowerWindow: RTLowerWindow too big.")
            return 1
        if self.RTUpperWindow < 0:
            print("RT->RTUpperWindow: RTUpperWindow must be greater than 0.")
            return 1
        if self.RTFSharpness < 0.0:
            print("RT->RTFSharpness: RTFSharpness must be greater than 0.")
            return 1
        if not self.RTUpperWindow:
            print("RT->RTUpperWindow: No upper window length supplied.")
            return 1
        if self.RTUpperWindow > PWLen:
            print("RT->RTUpperWindow: RTUpperWindow too big.")
            return 1
        if not self.RTWindowExponent:
            print("RT->RTWindowExponent: No window exponent supplied.")
            return 1
        if self.RTWindowExponent <= 0.0:
            print("RT->RTWindowExponent: RTWindowExponent must be greater than 0.")
            return 1
        if self.RTNormFactor and not self.RTNormType:
            print("RT->RTNormType: No RT normalization type supplied.")
            return 1
        if self.RTOutFile and not self.RTOutFileType:
            print("RT->RTOutFileType: No RT output file type supplied.")
            return 1
        if self.RTOutWindow < 0:
            print("RT->RTOutWindow: RTOutWindow must be greater than 0.")
            return 1
        if self.RTOutWindow > self.RTLowerWindow+ self.RTFilterLen - 1:
            print("RT->RTOutWindow: RTOutWindow can't be greater than RTLowerWindow + RTFilterLen - 1.")
            return 1
        if self.RTOutWindow <= 0:
            PWLen = self.RTLowerWindow + self.RTFilterLen - 1
        else:
            WLen = self.RTOutWindow
            
        ################################################################################
        #                                                                              #
        # PS - Postfiltering Stage configuration check                                 #
        #                                                                              #
        ################################################################################

        if not self.PSFilterType:
            print("PS->PSFilterType: No filter type supplied.")
            return 1
        if self.PSFilterType[0] != 'L' and self.PSFilterType[0] != 'M' and self.PSFilterType[0] != 'T':
            print("PS->PSFilterType: Invalid filter type supplied.")
            return 1
        if not self.PSInterpolationType:
            print("PS->PSInterpolationType: No interpolation type supplied.")
            return 1
        if self.PSInterpolationType[0] != 'L' and self.PSInterpolationType[0] != 'G'\
               and self.PSInterpolationType[0] != 'R' and self.PSInterpolationType[0] != 'S' \
	       and self.PSInterpolationType[0] != 'P' and self.PSInterpolationType[0] != 'H':
            print("PS->PSInterpolationType: Invalid interpolation type supplied.")
            return 1
        if not self.PSMagType:
            print("PS->PSMagType: No filter definition magnitude type supplied.")
            return 1
        if self.PSMagType[0] != 'L' and self.PSMagType[0] != 'D':
            print("PS->PSMagType: Invalid filter definition magnitude type supplied.")
            return 1
        if not self.PSPointsFile:
            print("PS->PSPointsFile: No correction point file supplied.")
            return 1
        if not exists(self.BCBaseDir + self.PSPointsFile):
            print("PS->PSPointsFile: ",self.PSPointsFile,"PSPointsFile does not exists.")
            return 1
        if self.PSNumPoints < 0 or self.PSNumPoints == 1:
            print("PS->PSNumPoints: Invalid PSNumPoints supplied, it must be 0 or at least 2.")
            return 1
        if self.PSNormFactor and not self.PSNormType:
            print("PS->PSNormFactor: No normalization type supplied.")
            return 1
        if self.PSOutFile and not self.PSOutFileType:
            print("PS->PSOutFileType: No output file type supplied.")
            return 1
        if self.PSFilterLen <= 0:
            print("PS->PSFilterLen: PSFilterLen must be greater than 0.")
            return 1
        if self.PSOutWindow <= 0:
            print("PS->PSOutWindow: PSOutWindow must be greater than 0.")
            return 1
        if self.PSFilterType[0] == 'L':
            if self.PSOutWindow > PWLen + self.PSFilterLen - 1:
                print("PS->PSOutWindow: PSOutWindow too big.")
                return 1
            PWLen += self.PSFilterLen - 1
        elif self.PSFilterType[0] == 'M':
            if self.PSOutWindow > PWLen:
                print("PS->PSOutWindow: PSOutWindow too big.")
                return 1
            elif self.PSFilterType[0] == 'T':
                if self.PSOutWindow > PWLen / 2 + self.ISPELowerWindow + self.PSFilterLen - 1:
                    print("PS->PSOutWindow: PSOutWindow too big.")
                    return 1

        ################################################################################
        #                                                                              #
        # MC - Microphone Compensation configuration check                             #
        #                                                                              #
        ################################################################################
    
        if not self.MCFilterType:
            print("MC->MCFilterType: No filter type supplied.")
            return 1
        if self.MCFilterType[0] != 'L' and self.MCFilterType[0] != 'M' and self.MCFilterType[0] != 'T':
            print("MC->MCFilterType: Invalid filter type supplied.")
            return 1
        if not self.MCInterpolationType:
            print("MC->MCInterpolationType: No interpolation type supplied.")
            return 1
        if self.MCInterpolationType[0] != 'L' and self.MCInterpolationType[0] != 'G'\
               and self.MCInterpolationType[0] != 'R' and self.MCInterpolationType[0] != 'S' \
               and self.MCInterpolationType[0] != 'P' and self.MCInterpolationType[0] != 'H':
            print("MC->MCInterpolationType: Invalid interpolation type supplied.")
            return 1
        if not self.MCMagType:
            print("MC->MCMagType: No filter definition magnitude type supplied.")
            return 1
        if self.MCMagType[0] != 'L' and self.MCMagType[0] != 'D':
            print("MC->MCMagType: Invalid filter definition magnitude type supplied.")
            return 1
        if not self.MCPointsFile:
            print("MC->MCPointsFile: No correction point file supplied.")
            return 1
        if not exists(self.BCBaseDir + self.MCPointsFile):
            print("MC->MCPointsFile: MCPointsFile",self.MCPointsFile,"does not exists.")
            return 1
        if self.MCNumPoints < 0 or self.MCNumPoints == 1:
            print("MC->MCNumPoints: Invalid MCNumPoints supplied, it must be 0 or at least 2.")
            return 1
        if self.MCNormFactor and not self.MCNormType:
            print("MC->MCNormFactor: No normalization type supplied.")
            return 1
        if self.MCOutFile and not self.MCOutFileType:
            print("MC->MCOutFileType: No output file type supplied.")
            return 1
        if self.MCFilterLen <= 0:
            print("PS->MCFilterLen: MCFilterLen must be greater than 0.")
            return 1
        if self.MCOutWindow <= 0:
            print("MC->MCOutWindow: MCOutWindow must be greater than 0.")
            return 1
        if self.MCFilterType[0] == 'L':
            if self.MCOutWindow > PWLen + self.MCFilterLen - 1:
                print("MC->MCOutWindow: MCOutWindow too big.")
                return 1
            if self.MCOutWindow > 0:
                PWLen = self.MCOutWindow
            else:
                PWLen += self.MCFilterLen - 1
        elif self.MCFilterType[0] == 'M':
            if self.MCOutWindow > PWLen:
                print("MC->MCOutWindow: MCOutWindow too big.")
                return 1
            elif self.PSFilterType[0] == 'T':
                if self.PSOutWindow > PWLen / 2 + self.ISPELowerWindow + self.PSFilterLen - 1:
                    print("PS->PSOutWindow: PSOutWindow too big.")
                    return 1
        if self.MCOutWindow> 0:
            PWLen = self.MCOutWindow
        else:
            PWLen += self.MCFilterLen - 1
                        
        ################################################################################
        #                                                                              #
        # MS - Minimum phase filter extraction Stage configuration check               #
        #                                                                              #
        ################################################################################

        if not self.MSOutWindow:
            print("MS->MSOutWindow: No output window length supplied.")
            return 1
        if self.MSOutWindow > PWLen:
            print("MS->MSOutWindow: MSOutWindow too big.")
            return 1
        if self.MSNormFactor and not self.MSNormType:
            print("MS->MSNormType: No normalization type supplied.")
            return 1
        if self.MSOutFile and not self.MSOutFileType:
            print("MS->MSOutFileType: No output file type supplied.")
            return 1
	
        ################################################################################
        #                                                                              #
        #  TC - Test Convolution configuration check                                   #
        #                                                                              #
        ################################################################################

        if self.TCNormFactor and not self.TCNormType:
            print("TC->TCNormType: No normalization type supplied.")
            return 1
        if self.TCOutFile and not self.TCOutFileType:
            print("TC->TCOutFileType: No output file type supplied.")
            return 1
        if self.TCWOutFile:
            if not self.TCWOutFileType:
                print("TC->TCOWFileType: No overwrite file type supplied.")
                return 1
            if self.TCWNormFactor and not self.TCWNormType:
                print("TC->TCOWNormType: No normalization type supplied.")
                return 1
            return 0
