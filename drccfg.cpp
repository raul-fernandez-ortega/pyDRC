/****************************************************************************

    DRC: Digital Room Correction
    Copyright (C) 2002, 2003 Denis Sbragion

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

		You can contact the author on Internet at the following address:

				d.sbragion@infotecna.it

		This program uses the parsecfg library from Yuuki  NINOMIYA.  De­
		tails  on  this  library  can be found in the parsecfg.c and par­
		secfg.h files.  Many thanks to Yuuki NINOMIYA for this useful li­
		brary.

		This program uses  also the FFT  routines from  Takuya Ooura and
		the GNU Scientific  Library (GSL).  Many thanks  to Takuya Ooura
		and the GSL developers for these efficient routines.

****************************************************************************/

/* Opzioni di configurazione DRC */

/* Inclusioni */
#include "drccfg.h"
#include "baselib.h"
#include <stddef.h>
#include <string.h>

/* Opzioni di configurazione */
CfgParmsType Cfg;

/* Definizione struttura file di configurazione */
cfgStruct CfgParmsDef[] =
	{
		/* Base configuration */
		{ "BCBaseDir",CFG_STRING,&Cfg.BCBaseDir },
		{ "BCInFile",CFG_STRING,&Cfg.BCInFile },
		{ "BCInFileType",CFG_STRING,&Cfg.BCInFileType },
		{ "BCSampleRate",CFG_INT,&Cfg.BCSampleRate },
		{ "BCImpulseCenterMode",CFG_STRING,&Cfg.BCImpulseCenterMode },
		{ "BCImpulseCenter",CFG_INT,&Cfg.BCImpulseCenter },
		{ "BCInitWindow",CFG_INT,&Cfg.BCInitWindow },
		{ "BCPreWindowLen",CFG_INT,&Cfg.BCPreWindowLen },
		{ "BCPreWindowGap",CFG_INT,&Cfg.BCPreWindowGap },
		{ "BCNormFactor",DRCCfgFloat,&Cfg.BCNormFactor },
		{ "BCNormType",CFG_STRING,&Cfg.BCNormType },

		/* Base configuration dip limiting stage */
		{ "BCDLType",CFG_STRING,&Cfg.BCDLType },
		{ "BCDLMinGain",DRCCfgFloat,&Cfg.BCDLMinGain },
		{ "BCDLStartFreq",DRCCfgFloat,&Cfg.BCDLStartFreq },
		{ "BCDLEndFreq",DRCCfgFloat,&Cfg.BCDLEndFreq },
		{ "BCDLStart",DRCCfgFloat,&Cfg.BCDLStart },
		{ "BCDLMultExponent",CFG_INT,&Cfg.BCDLMultExponent },

		/* Homomorphic Deconvolution */
		{ "HDMultExponent",CFG_INT,&Cfg.HDMultExponent },
		{ "HDMPNormFactor",DRCCfgFloat,&Cfg.HDMPNormFactor },
		{ "HDMPNormType",CFG_STRING,&Cfg.HDMPNormType },
		{ "HDMPOutFile",CFG_STRING,&Cfg.HDMPOutFile },
		{ "HDMPOutFileType",CFG_STRING,&Cfg.HDMPOutFileType },
		{ "HDEPNormFactor",DRCCfgFloat,&Cfg.HDEPNormFactor },
		{ "HDEPNormType",CFG_STRING,&Cfg.HDEPNormType },
		{ "HDEPOutFile",CFG_STRING,&Cfg.HDEPOutFile },
		{ "HDEPOutFileType",CFG_STRING,&Cfg.HDEPOutFileType },

		/* Minimum phase prefiltering stage */
		{ "MPPrefilterType",CFG_STRING,&Cfg.MPPrefilterType },
		{ "MPPrefilterFctn",CFG_STRING,&Cfg.MPPrefilterFctn },
		{ "MPWindowGap",CFG_INT,&Cfg.MPWindowGap },
		{ "MPLowerWindow",CFG_INT,&Cfg.MPLowerWindow },
		{ "MPUpperWindow",CFG_INT,&Cfg.MPUpperWindow },
		{ "MPStartFreq",DRCCfgFloat,&Cfg.MPStartFreq },
		{ "MPEndFreq",DRCCfgFloat,&Cfg.MPEndFreq },
		{ "MPWindowExponent",DRCCfgFloat,&Cfg.MPWindowExponent },
		{ "MPFilterLen",CFG_INT,&Cfg.MPFilterLen },
		{ "MPFSharpness",DRCCfgFloat,&Cfg.MPFSharpness },
		{ "MPBandSplit",CFG_INT,&Cfg.MPBandSplit },
		{ "MPHDRecover",CFG_STRING,&Cfg.MPHDRecover },
		{ "MPEPPreserve",CFG_STRING,&Cfg.MPEPPreserve },
		{ "MPHDMultExponent",CFG_INT,&Cfg.MPHDMultExponent },
		{ "MPPFFinalWindow",CFG_INT,&Cfg.MPPFFinalWindow },
		{ "MPPFNormFactor",DRCCfgFloat,&Cfg.MPPFNormFactor },
		{ "MPPFNormType",CFG_STRING,&Cfg.MPPFNormType },
		{ "MPPFOutFile",CFG_STRING,&Cfg.MPPFOutFile },
		{ "MPPFOutFileType",CFG_STRING,&Cfg.MPPFOutFileType },

		/* Dip limiting stage */
		{ "DLType",CFG_STRING,&Cfg.DLType },
		{ "DLMinGain",DRCCfgFloat,&Cfg.DLMinGain },
		{ "DLStartFreq",DRCCfgFloat,&Cfg.DLStartFreq },
		{ "DLEndFreq",DRCCfgFloat,&Cfg.DLEndFreq },
		{ "DLStart",DRCCfgFloat,&Cfg.DLStart },
		{ "DLMultExponent",CFG_INT,&Cfg.DLMultExponent },

		/* Excess fase phase prefiltering stage */
		{ "EPPrefilterType",CFG_STRING,&Cfg.EPPrefilterType },
		{ "EPPrefilterFctn",CFG_STRING,&Cfg.EPPrefilterFctn },
		{ "EPWindowGap",CFG_INT,&Cfg.EPWindowGap },
		{ "EPLowerWindow",CFG_INT,&Cfg.EPLowerWindow },
		{ "EPUpperWindow",CFG_INT,&Cfg.EPUpperWindow },
		{ "EPStartFreq",DRCCfgFloat,&Cfg.EPStartFreq },
		{ "EPEndFreq",DRCCfgFloat,&Cfg.EPEndFreq },
		{ "EPWindowExponent",DRCCfgFloat,&Cfg.EPWindowExponent },
		{ "EPFilterLen",CFG_INT,&Cfg.EPFilterLen },
		{ "EPFSharpness",DRCCfgFloat,&Cfg.EPFSharpness },
		{ "EPBandSplit",CFG_INT,&Cfg.EPBandSplit },
		{ "EPPFFlatGain",DRCCfgFloat,&Cfg.EPPFFlatGain },
		{ "EPPFOGainFactor",DRCCfgFloat,&Cfg.EPPFOGainFactor },
		{ "EPPFFlatType",CFG_STRING,&Cfg.EPPFFlatType },
		{ "EPPFFGMultExponent",CFG_INT,&Cfg.EPPFFGMultExponent },
		{ "EPPFFinalWindow",CFG_INT,&Cfg.EPPFFinalWindow },
		{ "EPPFNormFactor",DRCCfgFloat,&Cfg.EPPFNormFactor },
		{ "EPPFNormType",CFG_STRING,&Cfg.EPPFNormType },
		{ "EPPFOutFile",CFG_STRING,&Cfg.EPPFOutFile },
		{ "EPPFOutFileType",CFG_STRING,&Cfg.EPPFOutFileType },

		/* Prefiltering completion stage */
		{ "PCOutWindow",CFG_INT,&Cfg.PCOutWindow },
		{ "PCNormFactor",DRCCfgFloat,&Cfg.PCNormFactor },
		{ "PCNormType",CFG_STRING,&Cfg.PCNormType },
		{ "PCOutFile",CFG_STRING,&Cfg.PCOutFile },
		{ "PCOutFileType",CFG_STRING,&Cfg.PCOutFileType },

		/* Inversion stage */
		{ "ISType",CFG_STRING,&Cfg.ISType },
		{ "ISPETType",CFG_STRING,&Cfg.ISPETType },
		{ "ISPrefilterFctn",CFG_STRING,&Cfg.ISPrefilterFctn },
		{ "ISPELowerWindow",CFG_INT,&Cfg.ISPELowerWindow },
		{ "ISPEUpperWindow",CFG_INT,&Cfg.ISPEUpperWindow },
		{ "ISPEStartFreq",CFG_INT,&Cfg.ISPEStartFreq },
		{ "ISPEEndFreq",CFG_INT,&Cfg.ISPEEndFreq },
		{ "ISPEFilterLen",CFG_INT,&Cfg.ISPEFilterLen },
		{ "ISPEFSharpness",DRCCfgFloat,&Cfg.ISPEFSharpness },
		{ "ISPEBandSplit",CFG_INT,&Cfg.ISPEBandSplit },
		{ "ISPEWindowExponent",DRCCfgFloat,&Cfg.ISPEWindowExponent },
		{ "ISPEOGainFactor",DRCCfgFloat,&Cfg.ISPEOGainFactor },
		{ "ISSMPMultExponent",CFG_INT,&Cfg.ISSMPMultExponent },
		{ "ISOutWindow",CFG_INT,&Cfg.ISOutWindow },
		{ "ISNormFactor",DRCCfgFloat,&Cfg.ISNormFactor },
		{ "ISNormType",CFG_STRING,&Cfg.ISNormType },
		{ "ISOutFile",CFG_STRING,&Cfg.ISOutFile },
		{ "ISOutFileType",CFG_STRING,&Cfg.ISOutFileType },

		/* Psychoacoustic target stage */
		{ "PTType",CFG_STRING,&Cfg.PTType },
		{ "PTReferenceWindow",CFG_INT,&Cfg.PTReferenceWindow },
		{ "PTDLType",CFG_STRING,&Cfg.PTDLType },
		{ "PTDLMinGain",DRCCfgFloat,&Cfg.PTDLMinGain },
		{ "PTDLStart",DRCCfgFloat,&Cfg.PTDLStart },
		{ "PTDLStartFreq",DRCCfgFloat,&Cfg.PTDLStartFreq },
		{ "PTDLEndFreq",DRCCfgFloat,&Cfg.PTDLEndFreq },
		{ "PTDLMultExponent",CFG_INT,&Cfg.PTDLMultExponent },
		{ "PTBandWidth",DRCCfgFloat,&Cfg.PTBandWidth },
		{ "PTPeakDetectionStrength",DRCCfgFloat,&Cfg.PTPeakDetectionStrength },
		{ "PTMultExponent",CFG_INT,&Cfg.PTMultExponent },
		{ "PTFilterLen",CFG_INT,&Cfg.PTFilterLen },
		{ "PTFilterFile",CFG_STRING,&Cfg.PTFilterFile },
		{ "PTFilterFileType",CFG_STRING,&Cfg.PTFilterFileType },
		{ "PTNormFactor",DRCCfgFloat,&Cfg.PTNormFactor },
		{ "PTNormType",CFG_STRING,&Cfg.PTNormType },
		{ "PTOutFile",CFG_STRING,&Cfg.PTOutFile },
		{ "PTOutFileType",CFG_STRING,&Cfg.PTOutFileType },
		{ "PTOutWindow",CFG_INT,&Cfg.PTOutWindow },

		/* Peak limiting stage */
		{ "PLType",CFG_STRING,&Cfg.PLType },
		{ "PLMaxGain",DRCCfgFloat,&Cfg.PLMaxGain },
		{ "PLStart",DRCCfgFloat,&Cfg.PLStart },
		{ "PLStartFreq",DRCCfgFloat,&Cfg.PLStartFreq },
		{ "PLEndFreq",DRCCfgFloat,&Cfg.PLEndFreq },
		{ "PLMultExponent",CFG_INT,&Cfg.PLMultExponent },
		{ "PLOutWindow",CFG_INT,&Cfg.PLOutWindow },
		{ "PLNormFactor",DRCCfgFloat,&Cfg.PLNormFactor },
		{ "PLNormType",CFG_STRING,&Cfg.PLNormType },
		{ "PLOutFile",CFG_STRING,&Cfg.PLOutFile },
		{ "PLOutFileType",CFG_STRING,&Cfg.PLOutFileType },

		/* Ringing truncation stage */
		{ "RTType",CFG_STRING,&Cfg.RTType },
		{ "RTPrefilterFctn",CFG_STRING,&Cfg.RTPrefilterFctn },
		{ "RTWindowGap",CFG_INT,&Cfg.RTWindowGap },
		{ "RTLowerWindow",CFG_INT,&Cfg.RTLowerWindow },
		{ "RTUpperWindow",CFG_INT,&Cfg.RTUpperWindow },
		{ "RTStartFreq",DRCCfgFloat,&Cfg.RTStartFreq },
		{ "RTEndFreq",DRCCfgFloat,&Cfg.RTEndFreq },
		{ "RTWindowExponent",DRCCfgFloat,&Cfg.RTWindowExponent },
		{ "RTFilterLen",CFG_INT,&Cfg.RTFilterLen },
		{ "RTFSharpness",DRCCfgFloat,&Cfg.RTFSharpness },
		{ "RTBandSplit",CFG_INT,&Cfg.RTBandSplit },
		{ "RTOutWindow",CFG_INT,&Cfg.RTOutWindow },
		{ "RTNormFactor",DRCCfgFloat,&Cfg.RTNormFactor },
		{ "RTNormType",CFG_STRING,&Cfg.RTNormType },
		{ "RTOutFile",CFG_STRING,&Cfg.RTOutFile },
		{ "RTOutFileType",CFG_STRING,&Cfg.RTOutFileType },

		/* Target response stage */
		{ "PSFilterType",CFG_STRING,&Cfg.PSFilterType },
		{ "PSInterpolationType",CFG_STRING,&Cfg.PSInterpolationType },
		{ "PSMultExponent",CFG_INT,&Cfg.PSMultExponent },
		{ "PSFilterLen",CFG_INT,&Cfg.PSFilterLen },
		{ "PSNumPoints",CFG_INT,&Cfg.PSNumPoints },
		{ "PSPointsFile",CFG_STRING,&Cfg.PSPointsFile },
		{ "PSMagType",CFG_STRING,&Cfg.PSMagType },
		{ "PSOutWindow",CFG_INT,&Cfg.PSOutWindow },
		{ "PSNormFactor",DRCCfgFloat,&Cfg.PSNormFactor },
		{ "PSNormType",CFG_STRING,&Cfg.PSNormType },
		{ "PSOutFile",CFG_STRING,&Cfg.PSOutFile },
		{ "PSOutFileType",CFG_STRING,&Cfg.PSOutFileType },

		/* Mic correction stage */
		{ "MCFilterType",CFG_STRING,&Cfg.MCFilterType },
		{ "MCInterpolationType",CFG_STRING,&Cfg.MCInterpolationType },
		{ "MCMultExponent",CFG_INT,&Cfg.MCMultExponent },
		{ "MCFilterLen",CFG_INT,&Cfg.MCFilterLen },
		{ "MCNumPoints",CFG_INT,&Cfg.MCNumPoints },
		{ "MCPointsFile",CFG_STRING,&Cfg.MCPointsFile },
		{ "MCMagType",CFG_STRING,&Cfg.MCMagType },
		{ "MCOutWindow",CFG_INT,&Cfg.MCOutWindow },
		{ "MCNormFactor",DRCCfgFloat,&Cfg.MCNormFactor },
		{ "MCNormType",CFG_STRING,&Cfg.MCNormType },
		{ "MCOutFile",CFG_STRING,&Cfg.MCOutFile },
		{ "MCOutFileType",CFG_STRING,&Cfg.MCOutFileType },

		/* Minimum phase filter extraction stage */
		{ "MSMultExponent",CFG_INT,&Cfg.MSMultExponent },
		{ "MSOutWindow",CFG_INT,&Cfg.MSOutWindow },
		{ "MSNormFactor",DRCCfgFloat,&Cfg.MSNormFactor },
		{ "MSNormType",CFG_STRING,&Cfg.MSNormType },
		{ "MSOutFile",CFG_STRING,&Cfg.MSOutFile },
		{ "MSOutFileType",CFG_STRING,&Cfg.MSOutFileType },

		/* Test convolution stage */
		{ "TCNormFactor",DRCCfgFloat,&Cfg.TCNormFactor },
		{ "TCNormType",CFG_STRING,&Cfg.TCNormType },
		{ "TCOutFile",CFG_STRING,&Cfg.TCOutFile },
		{ "TCOutFileType",CFG_STRING,&Cfg.TCOutFileType },
		{ "TCOWFile",CFG_STRING,&Cfg.TCOWFile },
		{ "TCOWFileType",CFG_STRING,&Cfg.TCOWFileType },
		{ "TCOWNormFactor",DRCCfgFloat,&Cfg.TCOWNormFactor },
		{ "TCOWNormType",CFG_STRING,&Cfg.TCOWNormType },
		{ "TCOWSkip",CFG_INT,&Cfg.TCOWSkip },
		{ "TCOWPrewindow",CFG_INT,&Cfg.TCOWPrewindow },
		{ "TCOWLength",CFG_INT,&Cfg.TCOWLength },

		/* Chiusura lista parametri */
		{NULL, CFG_END, NULL}
	};

/* Elenco dei parametri di configurazione che richiedo gestione della
directory base */
static const char * BaseDirParmsList[] =
	{
		"BCInFile",
		"HDMPOutFile",
		"HDEPOutFile",
		"MPPFOutFile",
		"EPPFOutFile",
		"PCOutFile",
		"ISOutFile",
		"PTFilterFile",
		"PTOutFile",
		"PLOutFile",
		"RTOutFile",
		"PSPointsFile",
		"PSOutFile",
		"MCPointsFile",
		"MCOutFile",
		"MSOutFile",
		"TCOutFile",
		"TCOWFile",
		NULL
	};

/* Concatena due stringhe allocando lo spazio necessario */
static char * StrJoin(const char * S1, const char * S2)
	{
		char * SJ;
		if ((SJ = (char *) malloc((1 + strlen(S1) + strlen(S2)) * sizeof(char))) == NULL)
			return NULL;
		strcpy(SJ,S1);
		strcat(SJ,S2);
		return SJ;
	}

/* Ritorna la posizione del parametro indicato */
static int GetCfgParmPos(const cfgStruct * CfgParmsDef, const char * ParmName)
	{
		int P = 0;
		while (CfgParmsDef[P].type != CFG_END)
			{
				if (strcmp(CfgParmsDef[P].parameterName,ParmName) == 0)
					return P;
				P++;
			}
		return -1;
	}

/* Impostazione directory base di lavoro */
int SetupDRCCfgBaseDir(CfgParmsType * DRCCfg, const cfgStruct * CfgParmsDef,
	const CmdLineType * OptData)
	{
		/* Parametro corrente */
		int P;

		/* Indice verifica parametri */
		int I;

		/* Concatenazione stringhe */
		char * S;

		/* Verifica che la directory base sia presente. */
		if (DRCCfg->BCBaseDir == NULL)
			return 0;
		if (strlen(DRCCfg->BCBaseDir) == 0)
			return 0;

		/* Ciclo sui parametri modificabili */
		I = 0;
		while (BaseDirParmsList[I] != NULL)
			{
				/* Recupera la posizione del parametro */
				P = GetCfgParmPos(CfgParmsDef,BaseDirParmsList[I]);

				/* Verifica se il parametro è stato impostato
				a linea di comando */
				if ((OptData->ParmSet[P] == False ||
					OptData->ParmSet[BCBaseDirParmPos] == True) &&
					(*((void **) CfgParmsDef[P].value) != NULL))
					{
						/* Aggiunge la directory base al parametro */
						S = StrJoin(DRCCfg->BCBaseDir,
							(char *) (* ((void **) CfgParmsDef[P].value)));

						/* Verifica che la concatenazione sia riuscita */
						if (S == NULL)
							return 1;

						/* Dealloca la stringa precedente */
						free(*((void **) CfgParmsDef[P].value));

						/* Sostituisce la stringa precedente */
						*((void **) CfgParmsDef[P].value) = (void *) S;
					}

				/* Passa al parametro successivo */
				I++;
			}

		/* Operazione completata */
		return 0;
	}

/* Controllo validità parametri di configurazione */
int CheckDRCCfg(const CfgParmsType * DRCCfg)
	{
		/* Calcolo finestra validazione parametri */
		int PWLen;

		/* Stringa temporanea errori */
		char TStr[256];

		/*********************************************************************************/
		/* Importazione iniziale risposta all'impulso */
		/*********************************************************************************/

		if (DRCCfg->BCInFile == NULL)
			{
				sputs("BC->BCInFile: No input file name supplied.");
				return 1;
			}
		if (DRCCfg->BCInFileType == NULL)
			{
				sputs("BC->BCInFileType: No input file type supplied.");
				return 1;
			}
		if (DRCCfg->BCImpulseCenterMode == NULL)
			{
				sputs("BC->BCImpulseCenterMode: No impulse center mode supplied.");
				return 1;
			}
		if (DRCCfg->BCNormFactor > 0 && DRCCfg->BCNormType == NULL)
			{
				sputs("BC->BCNormType: No input normalization type supplied.");
				return 1;
			}
		if (DRCCfg->BCInitWindow < 3)
			{
				sputs("BC->BCInitWindow: Initial window should be at least 3.");
				return 1;
			}
		if (DRCCfg->BCPreWindowGap < 0)
			{
				sputs("BC->BCPreWindowGap: BCPreWindowGap can't be less than 0.");
				return 1;
			}
		if (DRCCfg->BCPreWindowLen > 0)
			if (DRCCfg->BCPreWindowLen + DRCCfg->BCPreWindowGap > DRCCfg->BCInitWindow)
				{
					sputs("BC: BCPreWindowLen + BCPreWindowGap must be less than BCInitWindow.");
					return 1;
				}

		/*********************************************************************************/
		/* Base configuration dip limiting */
		/*********************************************************************************/

		if (DRCCfg->BCDLType == NULL)
			{
				sputs("BC->BCDLType: No BCDLType supplied.");
				return 1;
			}
		if (DRCCfg->BCDLType[0] != 'L' && DRCCfg->BCDLType[0] != 'M')
			{
				sputs("BC->BCDLType: Invalid dip limiting type supplied.");
				return 1;
			}
		if (DRCCfg->BCDLStart < (DLReal) 0.0)
			{
				sputs("BC->BCDLStart: BCDLStart must be greater than or equal to 0.");
				return 1;
			}

		/*********************************************************************************/
		/* Deconvoluzione omomorfa */
		/*********************************************************************************/

		if (DRCCfg->HDMPNormFactor > 0 && DRCCfg->HDMPNormType == NULL)
			{
				sputs("HD->HDMPNormType: No MP normalization type supplied.");
				return 1;
			}
		if (DRCCfg->HDEPNormFactor > 0 && DRCCfg->HDEPNormType == NULL)
			{
				sputs("HD->HDEPNormType: No EP normalization type supplied.");
				return 1;
			}
		if (DRCCfg->HDMPOutFile != NULL && DRCCfg->HDMPOutFileType == NULL)
			{
				sputs("HD->HDMPOutFileType: No MP output file type supplied.");
				return 1;
			}
		if (DRCCfg->HDEPOutFile != NULL && DRCCfg->HDEPOutFileType == NULL)
			{
				sputs("HD->HDEPOutFileType: No EP output file type supplied.");
				return 1;
			}

		/*********************************************************************************/
		/* Prefiltratura componente MP */
		/*********************************************************************************/

		if (DRCCfg->MPPrefilterType == NULL)
			{
				sputs("MP->MPPrefilterType: No MPPrefilterType supplied.");
				return 1;
			}
		if (DRCCfg->MPPrefilterType[0] != 'B' && DRCCfg->MPPrefilterType[0] != 'b'
			&& DRCCfg->MPPrefilterType[0] != 'S' && DRCCfg->MPPrefilterType[0] != 's')
			{
				sputs("MP->MPPrefilterType: Invalid MPPrefilterType supplied.");
				return 1;
			}
		if (DRCCfg->MPPrefilterFctn == NULL)
			{
				sputs("MP->MPPrefilterFctn: No MPPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->MPPrefilterFctn[0] != 'P' && DRCCfg->MPPrefilterFctn[0] != 'B')
			{
				sputs("MP->MPPrefilterFctn: Invalid MPPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->MPWindowGap < 0)
			{
				sputs("MP->MPWindowGap: MPWindowGap must be greater or equal to 0.");
				return 1;
			}
		if (DRCCfg->MPLowerWindow <= 0)
			{
				sputs("MP->MPLowerWindow: No MPLowerWindow supplied.");
				return 1;
			}
		if (DRCCfg->MPLowerWindow > 2 * DRCCfg->BCInitWindow)
			{
				sputs("MP->MPLowerWindow: MPLowerWindow can't be greater than 2 * BCInitWindow.");
				return 1;
			}
		if (DRCCfg->MPUpperWindow <= 0)
			{
				sputs("MP->MPUpperWindow: No MPUpperWindow supplied.");
				return 1;
			}
		if (DRCCfg->MPFSharpness <= (DLReal) 0.0)
			{
				sputs("MP->MPFSharpness: MPFSharpness must be greater than 0.");
				return 1;
			}
		if (DRCCfg->MPUpperWindow > 2 * DRCCfg->BCInitWindow)
			{
				sputs("MP->MPUpperWindow: MPUpperWindow can't be greater than 2 * BCInitWindow.");
				return 1;
			}
		if (DRCCfg->MPWindowExponent <= (DLReal) 0.0)
			{
				sputs("MP->MPWindowExponent: MPWindowExponent must be greater than 0.");
				return 1;
			}
		if (DRCCfg->MPHDRecover == NULL)
			{
				sputs("MP->MPHDRecover: No MPHDRecover supplied.");
				return 1;
			}
		if (DRCCfg->MPEPPreserve == NULL)
			{
				sputs("MP->MPEPPreserve: No MPEPPreserve supplied.");
				return 1;
			}
		if (DRCCfg->MPPFNormFactor > 0 && DRCCfg->MPPFNormType == NULL)
			{
				sputs("MP->MPPFNormType: No PF normalization type supplied.");
				return 1;
			}
		if (DRCCfg->MPPFOutFile != NULL && DRCCfg->MPPFOutFileType == NULL)
			{
				sputs("MP->MPPFOutFileType: No MPPF output file type supplied.");
				return 1;
			}
		if (DRCCfg->MPPFFinalWindow > DRCCfg->MPLowerWindow + DRCCfg->MPFilterLen - 1)
			{
				sputs("MP->MPPFFinalWindow: MPPFFinalWindow can't be greater than MPLowerWindow + MPFilterLen - 1.");
				return 1;
			}
		if (DRCCfg->MPPFFinalWindow <= 0)
			PWLen = DRCCfg->MPLowerWindow + DRCCfg->MPFilterLen - 1;
		else
			PWLen = DRCCfg->MPPFFinalWindow;

		/*********************************************************************************/
		/* Dip limiting */
		/*********************************************************************************/

		if (DRCCfg->DLType == NULL)
			{
				sputs("DL->DLType: No DLType supplied.");
				return 1;
			}
		if (DRCCfg->DLType[0] != 'L' && DRCCfg->DLType[0] != 'M')
			{
				sputs("DL->DLType: Invalid dip limiting type supplied.");
				return 1;
			}
		if (DRCCfg->DLStart < (DLReal) 0.0)
			{
				sputs("DL->DLStart: DLStart must be greater than or equal to 0.");
				return 1;
			}

		/*********************************************************************************/
		/* Prefiltratura componente EP */
		/*********************************************************************************/

		if (DRCCfg->EPPrefilterType == NULL)
			{
				sputs("EP->EPPrefilterType: No EPPrefilterType supplied.");
				return 1;
			}
		if (DRCCfg->EPPrefilterType[0] != 'B' && DRCCfg->EPPrefilterType[0] != 'b'
			&& DRCCfg->EPPrefilterType[0] != 'S' && DRCCfg->EPPrefilterType[0] != 's')
			{
				sputs("EP->EPPrefilterType: Invalid EPPrefilterType supplied.");
				return 1;
			}
		if (DRCCfg->EPPrefilterFctn == NULL)
			{
				sputs("EP->EPPrefilterFctn: No EPPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->EPPrefilterFctn[0] != 'P' && DRCCfg->EPPrefilterFctn[0] != 'B')
			{
				sputs("EP->EPPrefilterFctn: Invalid EPPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->EPWindowGap < 0)
			{
				sputs("EP->EPWindowGap: EPWindowGap must be greater or equal to 0.");
				return 1;
			}
		if (DRCCfg->EPLowerWindow <= 0)
			{
				sputs("EP->EPLowerWindow: EPLowerWindow must be greater than 0.");
				return 1;
			}
		if (DRCCfg->EPLowerWindow > 2 * DRCCfg->BCInitWindow)
			{
				sputs("EP->EPLowerWindow: EPLowerWindow can't be greater than 2 * BCInitWindow.");
				return 1;
			}
		if (DRCCfg->EPUpperWindow <= 0)
			{
				sputs("EP->EPUpperWindow: EPUpperWindow must be greater than 0.");
				return 1;
			}
		if (DRCCfg->EPUpperWindow > 2 * DRCCfg->BCInitWindow)
			{
				sputs("EP->EPUpperWindow: EPUpperWindow can't be greater than 2 * BCInitWindow.");
				return 1;
			}
		if (DRCCfg->EPFSharpness <= (DLReal) 0.0)
			{
				sputs("EP->EPFSharpness: EPFSharpness must be greater than 0.");
				return 1;
			}
		if (DRCCfg->EPWindowExponent <= (DLReal) 0.0)
			{
				sputs("EP->EPWindowExponent: EPWindowExponent must be greater than 0.");
				return 1;
			}
		if (DRCCfg->EPPFFlatGain > 0 && DRCCfg->EPPFFlatType == NULL)
			{
				sputs("EP->EPPFFlatType: No EPPFFlatType supplied.");
				return 1;
			}
		if (DRCCfg->EPPFFlatType[0] != 'L' && DRCCfg->EPPFFlatType[0] != 'M' &&
			DRCCfg->EPPFFlatType[0] != 'D')
			{
				sputs("EP->EPPFFlatType: Invalid EPPFFlatType supplied.");
				return 1;
			}
		if (DRCCfg->EPPFNormFactor > 0 && DRCCfg->EPPFNormType == NULL)
			{
				sputs("EP->EPPFNormType: No PF normalization type supplied.");
				return 1;
			}
		if (DRCCfg->EPPFOutFile != NULL && DRCCfg->EPPFOutFileType == NULL)
			{
				sputs("EP->EPPFOutFileType: No EPPF output file type supplied.");
				return 1;
			}
		if (DRCCfg->EPPFFinalWindow > DRCCfg->EPLowerWindow + DRCCfg->EPFilterLen - 1)
			{
				sputs("EP->EPPFFinalWindow: EPPFFinalWindow can't be greater than EPLowerWindow + EPFilterLen - 1.");
				return 1;
			}
		if (DRCCfg->EPPFFinalWindow <= 0)
			PWLen += (DRCCfg->EPLowerWindow + DRCCfg->EPFilterLen - 1) - 1;
		else
			PWLen += DRCCfg->EPPFFinalWindow - 1;

		/*********************************************************************************/
		/* Combinazione componente MP e EP */
		/*********************************************************************************/

		if (DRCCfg->PCOutWindow < 0)
			{
				sputs("PC->PCOutWindow: No PCOutWindow supplied.");
				return 1;
			}
		if (DRCCfg->PCNormFactor > 0 && DRCCfg->PCNormType == NULL)
			{
				sputs("PC->PCNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->PCOutFile != NULL && DRCCfg->PCOutFileType == NULL)
			{
				sputs("PC->PCOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->PCOutWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("PC->PCOutWindow: PCOutWindow too big. Max value allowed: ",TStr);
				return 1;
			}

		/*********************************************************************************/
		/* Inversione risposta all'impulso */
		/*********************************************************************************/

		if (DRCCfg->ISType == NULL)
			{
				sputs("IS->ISType: No inversion stage type supplied.");
				return 1;
			}
		if (DRCCfg->ISType[0] != 'L' && DRCCfg->ISType[0] != 'T')
			{
				sputs("IS->ISType: Invalid inversion stage type supplied.");
				return 1;
			}
		if (DRCCfg->ISPETType == NULL)
			{
				sputs("IS->ISPETType: No pre echo truncation type supplied.");
				return 1;
			}
		if (DRCCfg->ISPETType[0] != 'f' && DRCCfg->ISPETType[0] != 's')
			{
				sputs("IS->ISPETType: Invalid pre echo truncation type supplied.");
				return 1;
			}
		if (DRCCfg->ISPrefilterFctn == NULL)
			{
				sputs("IS->ISPrefilterFctn: No ISPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->ISPrefilterFctn[0] != 'P' && DRCCfg->ISPrefilterFctn[0] != 'B')
			{
				sputs("IS->ISPrefilterFctn: Invalid ISPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->ISPEFSharpness <= (DLReal) 0.0)
			{
				sputs("IS->ISPEFSharpness: ISPEFSharpness must be greater than 0.");
				return 1;
			}
		if (DRCCfg->ISNormFactor > 0 && DRCCfg->ISNormType == NULL)
			{
				sputs("IS->ISNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->ISOutFile != NULL && DRCCfg->ISOutFileType == NULL)
			{
				sputs("IS->ISOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->ISOutWindow < 0)
			{
				sputs("IS->ISOutWindow: ISOutWindow must be greater than 0.");
				return 1;
			}
		if (DRCCfg->ISType[0] == 'L' && DRCCfg->ISOutWindow > DRCCfg->PCOutWindow)
			{
				sputs("IS->ISOutWindow: ISOutWindow can't be greater than PCOutWindow.");
				return 1;
			}
		if (DRCCfg->ISType[0] == 'T' && DRCCfg->ISOutWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("IS->ISOutWindow: ISOutWindow too big. Max allowed value: ", TStr);
				return 1;
			}
		if (DRCCfg->ISOutWindow > 0)
			PWLen = DRCCfg->ISOutWindow;

		/*********************************************************************************/
		/* Calcolo target psicoacustico */
		/*********************************************************************************/
		if (DRCCfg->PTType == NULL)
			{
				sputs("PT->PTType: No psychoacoustic target type supplied.");
				return 1;
			}
		if (DRCCfg->PTType[0] != 'N' && DRCCfg->PTType[0] != 'L' && DRCCfg->PTType[0] != 'M')
			{
				sputs("PT->PTType: Invalid psychoacoustic target type supplied.");
				return 1;
			}
		if (DRCCfg->PTType[0] != 'N')
			{
				if (DRCCfg->PTReferenceWindow <= 0)
					{
						sputs("PT->PTReferenceWindow: PTReferenceWindow must be greater than 0.");
						return 1;
					}
				if (DRCCfg->PTReferenceWindow > (DRCCfg->BCInitWindow + PWLen - 1))
					{
						sprintf(TStr,"%d.",DRCCfg->BCInitWindow + PWLen - 1);
						sputsp("PT->PTReferenceWindow: PTReferenceWindow too big. Max allowed value: ", TStr);
						return 1;
					}
				if (DRCCfg->PTDLType == NULL)
					{
						sputs("PT->PTDLType: No DLType supplied.");
						return 1;
					}
				if (DRCCfg->PTDLType[0] != 'L' && DRCCfg->PTDLType[0] != 'M')
					{
						sputs("PT->PTDLType: Invalid dip limiting type supplied.");
						return 1;
					}
				if (DRCCfg->PTDLStart < (DLReal) 0.0)
					{
						sputs("PT->PTDLStart: PTDLStart must be greater than or equal to 0.");
						return 1;
					}
				if (DRCCfg->PTFilterLen <= 0)
					{
						sputs("PT->PTFilterLen: PTFilterLen must be greater than 0.");
						return 1;
					}
				if (DRCCfg->PTFilterLen > 2 * (DRCCfg->BCInitWindow + PWLen - 1))
					{
						sprintf(TStr,"%d.",2 * (DRCCfg->BCInitWindow + PWLen - 1));
						sputsp("PT->PTFilterLen: PTFilterLen too big. Max allowed value: ", TStr);
						return 1;
					}
				if (DRCCfg->PTFilterFile != NULL && DRCCfg->PTFilterFileType == NULL)
					{
						sputs("PT->PTFilterFileType: No PT filter file type supplied.");
						return 1;
					}
				if (DRCCfg->PTNormFactor > 0 && DRCCfg->PTNormType == NULL)
					{
						sputs("PT->PTNormType: No PT normalization type supplied.");
						return 1;
					}
				if (DRCCfg->PTOutFile != NULL && DRCCfg->PTOutFileType == NULL)
					{
						sputs("PT->PTOutFileType: No PT output file type supplied.");
						return 1;
					}
				if (DRCCfg->PTOutWindow < 0)
					{
						sputs("PT->PTOutWindow: PTOutWindow can't be less than 0.");
						return 1;
					}
				switch (DRCCfg->PTDLType[0])
					{
						case 'L':
							if (DRCCfg->PTOutWindow > PWLen + DRCCfg->PTFilterLen - 1)
								{
									sprintf(TStr,"%d.",PWLen + DRCCfg->PTFilterLen - 1);
									sputsp("PT->PTOutWindow: PTOutWindow too big. Max value allowed: ",TStr);
									return 1;
								}
							if (DRCCfg->PTOutWindow > 0)
								PWLen = DRCCfg->PTOutWindow;
							else
								PWLen = PWLen + DRCCfg->PTFilterLen - 1;
						break;

						case 'M':
							if (DRCCfg->PTOutWindow > PWLen)
								{
									sprintf(TStr,"%d.",PWLen);
									sputsp("PT->PTOutWindow: PTOutWindow too big. Max value allowed: ",TStr);
									return 1;
								}
							if (DRCCfg->PTOutWindow > 0)
								PWLen = DRCCfg->PTOutWindow;
						break;
					}
			}

		/*********************************************************************************/
		/* Peak limiting */
		/*********************************************************************************/

		if (DRCCfg->PLType == NULL)
			{
				sputs("PL->PLType: No peak limiting type supplied.");
				return 1;
			}
		if (DRCCfg->PLType[0] != 'L' && DRCCfg->PLType[0] != 'M')
			{
				sputs("PL->PLType: Invalid peak limiting type supplied.");
				return 1;
			}
		if (DRCCfg->PLStart < (DLReal) 0.0)
			{
				sputs("PL->PLStart: PLStart must be greater than or equal to 0.");
				return 1;
			}
		if (DRCCfg->PLNormFactor > 0 && DRCCfg->PLNormType == NULL)
			{
				sputs("PL->PLNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->PLOutFile != NULL && DRCCfg->PLOutFileType == NULL)
			{
				sputs("PL->PLOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->PLOutWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("PL->PLOutWindow: PLOutWindow too big. Max allowed value: ", TStr);
				return 1;
			}
		if (DRCCfg->PLOutWindow > 0)
			PWLen = DRCCfg->PLOutWindow;

		/*********************************************************************************/
		/* Troncatura ringing */
		/*********************************************************************************/

		if (DRCCfg->RTType == NULL)
			{
				sputs("RT->RTType: No RTType supplied.");
				return 1;
			}
		if (DRCCfg->RTType[0] != 'N' && DRCCfg->RTType[0] != 'B' && DRCCfg->RTType[0] != 'b'
			&& DRCCfg->RTType[0] != 'S' && DRCCfg->RTType[0] != 's')
			{
				sputs("RT->RTType: Invalid RTType supplied.");
				return 1;
			}
		if (DRCCfg->RTPrefilterFctn == NULL)
			{
				sputs("RT->RTPrefilterFctn: No RTPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->RTPrefilterFctn[0] != 'P' && DRCCfg->RTPrefilterFctn[0] != 'B')
			{
				sputs("RT->RTPrefilterFctn: Invalid RTPrefilterFctn supplied.");
				return 1;
			}
		if (DRCCfg->RTWindowGap < 0)
			{
				sputs("RT->RTWindowGap: RTWindowGap must be greater or equal to 0.");
				return 1;
			}
		if (DRCCfg->RTLowerWindow <= 0)
			{
				sputs("RT->RTLowerWindow: RTLowerWindow must be greater than 0.");
				return 1;
			}
		if (DRCCfg->RTLowerWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("RT->RTLowerWindow: RTLowerWindow too big. Max allowed value: ", TStr);
				return 1;
			}
		if (DRCCfg->RTUpperWindow <= 0)
			{
				sputs("RT->RTUpperWindow: RTUpperWindow must be greater than 0.");
				return 1;
			}
		if (DRCCfg->RTFSharpness <= (DLReal) 0.0)
			{
				sputs("RT->RTFSharpness: RTFSharpness must be greater than 0.");
				return 1;
			}
		if (DRCCfg->RTUpperWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("RT->RTUpperWindow: RTUpperWindow too big. Max allowed value: ", TStr);
				return 1;
			}
		if (DRCCfg->RTWindowExponent <= (DLReal) 0.0)
			{
				sputs("RT->RTWindowExponent: RTWindowExponent must be greater than 0.");
				return 1;
			}
		if (DRCCfg->RTNormFactor > 0 && DRCCfg->RTNormType == NULL)
			{
				sputs("RT->RTNormType: No RT normalization type supplied.");
				return 1;
			}
		if (DRCCfg->RTOutFile != NULL && DRCCfg->RTOutFileType == NULL)
			{
				sputs("RT->RTOutFileType: No RT output file type supplied.");
				return 1;
			}
		if (DRCCfg->RTOutWindow > DRCCfg->RTLowerWindow + DRCCfg->RTFilterLen - 1)
			{
				sputs("RT->RTOutWindow: RTOutWindow can't be greater than RTLowerWindow + RTFilterLen - 1.");
				return 1;
			}
		if (DRCCfg->RTOutWindow <= 0)
			PWLen = DRCCfg->RTLowerWindow + DRCCfg->RTFilterLen - 1;
		else
			PWLen = DRCCfg->RTOutWindow;

		/*********************************************************************************/
		/* Applicazione risposta target */
		/*********************************************************************************/

		if (DRCCfg->PSFilterType == NULL)
			{
				sputs("PS->PSFilterType: No filter type supplied.");
				return 1;
			}
		if (DRCCfg->PSFilterType[0] != 'L' && DRCCfg->PSFilterType[0] != 'M'
			&& DRCCfg->PSFilterType[0] != 'T')
			{
				sputs("PS->PSFilterType: Invalid filter type supplied.");
				return 1;
			}
		if (DRCCfg->PSInterpolationType == NULL)
			{
				sputs("PS->PSInterpolationType: No interpolation type supplied.");
				return 1;
			}
		if (DRCCfg->PSInterpolationType[0] != 'L' && DRCCfg->PSInterpolationType[0] != 'G'
			&& DRCCfg->PSInterpolationType[0] != 'R' && DRCCfg->PSInterpolationType[0] != 'S'
			&& DRCCfg->PSInterpolationType[0] != 'P' && DRCCfg->PSInterpolationType[0] != 'H')
			{
				sputs("PS->PSInterpolationType: Invalid interpolation type supplied.");
				return 1;
			}
		if (DRCCfg->PSMagType== NULL)
			{
				sputs("PS->PSMagType: No filter definition magnitude type supplied.");
				return 1;
			}
		if (DRCCfg->PSMagType[0] != 'L' && DRCCfg->PSMagType[0] != 'D')
			{
				sputs("PS->PSMagType: Invalid filter definition magnitude type supplied.");
				return 1;
			}
		if (DRCCfg->PSPointsFile == NULL)
			{
				sputs("PS->PSPointsFile: No correction point file supplied.");
				return 1;
			}
		if (DRCCfg->PSNumPoints < 0 || DRCCfg->PSNumPoints == 1)
			{
				sputs("PS->PSNumPoints: Invalid PSNumPoints supplied, it must be 0 or at least 2.");
				return 1;
			}
		if (DRCCfg->PSNormFactor > 0 && DRCCfg->PSNormType == NULL)
			{
				sputs("PS->PSNormFactor: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->PSOutFile != NULL && DRCCfg->PSOutFileType == NULL)
			{
				sputs("PS->PSOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->PSFilterLen <= 0)
			{
				sputs("PS->PSFilterLen: PSFilterLen must be greater than 0.");
				return 1;
			}
		switch (DRCCfg->PSFilterType[0])
			{
				case 'L':
					if (DRCCfg->PSOutWindow > PWLen + DRCCfg->PSFilterLen - 1)
						{
							sprintf(TStr,"%d.",PWLen + DRCCfg->PSFilterLen - 1);
							sputsp("PS->PSOutWindow: PSOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}
					PWLen += DRCCfg->PSFilterLen - 1;
				break;
				case 'M':
					if (DRCCfg->PSOutWindow > PWLen)
						{
							sprintf(TStr,"%d.",PWLen);
							sputsp("PS->PSOutWindow: PSOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}
				break;
				case 'T':
					if (DRCCfg->PSOutWindow > PWLen / 2 + DRCCfg->ISPELowerWindow + DRCCfg->PSFilterLen - 1)
						{
							sprintf(TStr,"%d.",PWLen / 2 + DRCCfg->ISPELowerWindow + DRCCfg->PSFilterLen - 1);
							sputsp("PS->PSOutWindow: PSOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}
				break;
			}

		/*********************************************************************************/
		/* Compensazione microfono */
		/*********************************************************************************/

		if (DRCCfg->MCFilterType == NULL)
			{
				sputs("MC->MCFilterType: No filter type supplied.");
				return 1;
			}
		if (DRCCfg->MCFilterType[0] != 'L' && DRCCfg->MCFilterType[0] != 'M'
			&& DRCCfg->MCFilterType[0] != 'T')
			{
				sputs("MC->MCFilterType: Invalid filter type supplied.");
				return 1;
			}
		if (DRCCfg->MCInterpolationType == NULL)
			{
				sputs("MC->MCInterpolationType: No interpolation type supplied.");
				return 1;
			}
		if (DRCCfg->MCInterpolationType[0] != 'L' && DRCCfg->MCInterpolationType[0] != 'G'
			&& DRCCfg->MCInterpolationType[0] != 'R' && DRCCfg->MCInterpolationType[0] != 'S'
			&& DRCCfg->MCInterpolationType[0] != 'P' && DRCCfg->MCInterpolationType[0] != 'H')
			{
				sputs("MC->MCInterpolationType: Invalid interpolation type supplied.");
				return 1;
			}
		if (DRCCfg->MCMagType == NULL)
			{
				sputs("MC->MCMagType: No filter definition magnitude type supplied.");
				return 1;
			}
		if (DRCCfg->MCMagType[0] != 'L' && DRCCfg->MCMagType[0] != 'D')
			{
				sputs("MC->MCMagType: Invalid filter definition magnitude type supplied.");
				return 1;
			}
		if (DRCCfg->MCPointsFile == NULL)
			{
				sputs("MC->MCPointsFile: No correction point file supplied.");
				return 1;
			}
		if (DRCCfg->MCNumPoints < 0 || DRCCfg->MCNumPoints == 1)
			{
				sputs("MC->MCNumPoints: Invalid MCNumPoints supplied, it must be 0 or at least 2.");
				return 1;
			}
		if (DRCCfg->MCNormFactor > 0 && DRCCfg->MCNormType == NULL)
			{
				sputs("MC->MCNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->MCOutFile != NULL && DRCCfg->MCOutFileType == NULL)
			{
				sputs("MC->MCOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->MCFilterLen <= 0)
			{
				sputs("MC->MCFilterLen: MCFilterLen must be greater than 0.");
				return 1;
			}
		switch (DRCCfg->MCFilterType[0])
			{
				case 'L':
					if (DRCCfg->MCOutWindow > PWLen + DRCCfg->MCFilterLen - 1)
						{
							sprintf(TStr,"%d.",PWLen + DRCCfg->MCFilterLen - 1);
							sputsp("MC->MCOutWindow: MCOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}

					if (DRCCfg->MCOutWindow > 0)
						PWLen = DRCCfg->MCOutWindow;
					else
						PWLen += DRCCfg->MCFilterLen - 1;
				break;
				case 'M':
					if (DRCCfg->MCOutWindow > PWLen)
						{
							sprintf(TStr,"%d.",PWLen);
							sputsp("MC->MCOutWindow: MCOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}
				case 'T':
					if (DRCCfg->MCOutWindow > PWLen / 2 + DRCCfg->ISPELowerWindow + DRCCfg->MCFilterLen - 1)
						{
							sprintf(TStr,"%d.",PWLen / 2 + DRCCfg->ISPELowerWindow + DRCCfg->MCFilterLen - 1);
							sputsp("MC->MCOutWindow: MCOutWindow too big. Max allowed value: ", TStr);
							return 1;
						}

					if (DRCCfg->MCOutWindow > 0)
						PWLen = DRCCfg->MCOutWindow;
					else
						PWLen += DRCCfg->MCFilterLen - 1;
				break;
			}

		/*********************************************************************************/
		/* Estrazione filtro a fase minima */
		/*********************************************************************************/

		if (DRCCfg->MSOutWindow > PWLen)
			{
				sprintf(TStr,"%d.",PWLen);
				sputsp("MS->MSOutWindow: MSOutWindow too big. Max allowed value: ", TStr);
				return 1;
			}
		if (DRCCfg->MSNormFactor > 0 && DRCCfg->MSNormType == NULL)
			{
				sputs("MS->MSNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->MSOutFile != NULL && DRCCfg->MSOutFileType == NULL)
			{
				sputs("MS->MSOutFileType: No output file type supplied.");
				return 1;
			}

		/*********************************************************************************/
		/* Convoluzione di test */
		/*********************************************************************************/

		if (DRCCfg->TCNormFactor > 0 && DRCCfg->TCNormType == NULL)
			{
				sputs("TC->TCNormType: No normalization type supplied.");
				return 1;
			}
		if (DRCCfg->TCOutFile != NULL && DRCCfg->TCOutFileType == NULL)
			{
				sputs("TC->TCOutFileType: No output file type supplied.");
				return 1;
			}
		if (DRCCfg->TCOWFile != NULL)
			{
				if (DRCCfg->TCOWFileType == NULL)
					{
						sputs("TC->TCOWFileType: No overwrite file type supplied.");
						return 1;
					}
				if (DRCCfg->TCOWNormFactor > 0 && DRCCfg->TCOWNormType == NULL)
					{
						sputs("TC->TCOWNormType: No normalization type supplied.");
						return 1;
					}
			}

		/* Controllo completato */
		return 0;
	}
