/****************************************************************************

    DRC: Digital Room Correction
    Copyright (C) 2002-2004 Denis Sbragion

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

		This program uses the parsecfg library from Yuuki  NINOMIYA.  De�
		tails  on  this  library  can be found in the parsecfg.c and par�
		secfg.h files.  Many thanks to Yuuki NINOMIYA for this useful li�
		brary.

		This program uses  also the FFT  routines from  Takuya Ooura and
		the GNU Scientific  Library (GSL).  Many thanks  to Takuya Ooura
		and the GSL developers for these efficient routines.

****************************************************************************/

/* Command line parsing utilities */

/* Inclusioni */
#include "cmdline.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

/* Opzione help */
static char HelpSwitch[] = "help";

/* Effettua il conteggio degli elementi di una struttura
ParseCfg */
static long GetCfgStructCount(const cfgStruct * CfgParmsDef)
	{
		int I;

		I = 0;
		while (CfgParmsDef[I].type != CFG_END)
			I++;
		return I;
	}

/* Registra le impostazioni per la command line a partire
da una definizione ParseCfg. Torna una struttura opaque
utilizzata per il recupero dei parametri dalla linea di comando. */
CmdLineType * RegisterCmdLine(const cfgStruct * CfgParmsDef)
	{
		int OptCount;
		CmdLineType * OptData;
		int I;

		/* Recupera il conteggio parametri */
		OptCount = GetCfgStructCount(CfgParmsDef);

		/* Alloca la struttura per i parametri della command line */
		if ((OptData = (CmdLineType *) malloc(sizeof(CmdLineType))) == NULL)
			return NULL;
		if ((OptData->Options = (struct option *) malloc((OptCount + 2) *
			sizeof(struct option))) == NULL)
			{
				free(OptData);
				return NULL;
			}
		if ((OptData->Parms = (ParameterType *) malloc((OptCount + 1) *
			sizeof(ParameterType))) == NULL)
			{
				free(OptData->Options);
				free(OptData);
				return NULL;
			}
		if ((OptData->ParmSet = (Boolean *) malloc((OptCount + 1) *
			sizeof(Boolean))) == NULL)
			{
				free(OptData->Options);
				free(OptData->Parms);
				free(OptData);
				return NULL;
			}

		/* Registra il conteggio parametri */
		OptData->OptCount = OptCount;

		/* Ciclo di impostazione dei parametri di configurazione */
		for (I = 0;I < OptCount;I++)
			{
				/* Azzera il flag impostazione */
				OptData->ParmSet[I] = False;

				/* Imposta il nome dell'opzione */
				OptData->Options[I].name = CfgParmsDef[I].parameterName;

				/* Imposta il tipo argomento */
				OptData->Options[I].has_arg = required_argument;

				/* Imposta i parametri facoltativi */
				OptData->Options[I].flag = NULL;
				OptData->Options[I].val = 0;
			}

		/* Aggiunge l'opzione di help */
		OptData->Options[OptCount].name = HelpSwitch;
		OptData->Options[OptCount].has_arg = no_argument;
		OptData->Options[OptCount].flag = NULL;
		OptData->Options[OptCount].val = 0;

		/* Chiude l'array opzioni */
		OptData->Options[OptCount + 1].name = NULL;
		OptData->Options[OptCount + 1].has_arg = no_argument;
		OptData->Options[OptCount + 1].flag = NULL;
		OptData->Options[OptCount + 1].val = 0;

		/* Ritorna la struttura gestione command line */
		return OptData;
	}

/* Effettua il processing della command line */
int GetCmdLine(const int argc, char * const * argv,
	const cfgStruct * CfgParmsDef, CmdLineType * OptData,
	char ** CfgFileName)
	{
		int OptCount = OptData->OptCount;
		int OptInd;
		double FVal;
		long int LVal;
		char * CTail;
		int PRes = 0;
		int GORetCode;

		while((GORetCode = getopt_long(argc,argv,"",OptData->Options,&OptInd)) == 0)
			{
				/* Segnala il parametro come impostato */
				OptData->ParmSet[OptInd] = True;

				/* Controlla se si tratta dell'opzione di help */
				if (OptInd < OptCount)
					/* Verifica il tipo dell'opzione */
					switch (CfgParmsDef[OptInd].type)
						{
							/* Stringa */
							case CFG_STRING:
								/* Assegna il parametro */
								if (optarg == NULL)
									OptData->Parms[OptInd].StrParm = NULL;
								else
									if (strlen(optarg) == 0)
										OptData->Parms[OptInd].StrParm = NULL;
									else
										OptData->Parms[OptInd].StrParm = strdup(optarg);
							break;

							/* Intero */
							case CFG_INT:
								errno = 0;
								LVal = strtol(optarg,&CTail,0);
								if (errno != 0)
									{
										printf("Error parsing command line parameter '%s' with value '%s': %s\n",
											CfgParmsDef[OptInd].parameterName,optarg,strerror(errno));
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										LVal = 0;
										PRes = errno;
									}
								if (*CTail != '\0' || CTail == optarg)
									{
										printf("Command line parameter '%s' with value '%s' contains invalid characters.\n",
											CfgParmsDef[OptInd].parameterName,optarg);
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										LVal = 0;
										PRes = EILSEQ;
									}
								if (LVal < INT_MIN || LVal > INT_MAX)
									{
										printf("Command line parameter '%s' with value '%s' is out of range.\n",
											CfgParmsDef[OptInd].parameterName,optarg);
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										LVal = 0;
										PRes = EILSEQ;
									}
								OptData->Parms[OptInd].IntParm = LVal;
							break;

							/* Floating point */
							case DRCCfgFloat:
								errno = 0;
								FVal = strtod(optarg,&CTail);
								if (errno != 0)
									{
										printf("Error parsing command line parameter '%s' with value '%s': %s\n",
											CfgParmsDef[OptInd].parameterName,optarg,strerror(errno));
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										FVal = 0.0;
										PRes = errno;
									}
								if (*CTail != '\0' || CTail == optarg)
									{
										printf("Command line parameter '%s' with value '%s' contains invalid characters.\n",
											CfgParmsDef[OptInd].parameterName,optarg);
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										FVal = 0.0;
										PRes = EILSEQ;
									}
								if (FVal < -DRCMaxFloat || FVal > DRCMaxFloat)
									{
										printf("Command line parameter '%s' with value '%s' is out of range.\n",
											CfgParmsDef[OptInd].parameterName,optarg);
										fflush(stdout);
										OptData->ParmSet[OptInd] = False;
										FVal = 0.0;
										PRes = EILSEQ;
									}
								OptData->Parms[OptInd].FltParm = FVal;
							break;

							/* Parametri non supportati */
							default:
								perror("\nGetCmdLine: invalid parameter type.");
							break;
						}
			}

		/* Verifica la condizione di uscita */
		if (GORetCode != -1)
			return EINVAL;
		else
			{
				/* Recupera il nome del file */
				if (optind < argc)
					*CfgFileName = strdup(argv[optind]);
				else
					*CfgFileName = NULL;
			}

		return PRes;
	}

/* Sovrascrive i parametri della definizione ParseCfg CfgParmsDef con
i parametri recuperati dalla linea di comando e registrati nella
in OptData. Recupera il nome del file di configurazione ponendolo
in CfgFileName */
void CopyCmdLineParms(const CmdLineType * OptData,cfgStruct * CfgParmsDef)
	{
		int OptCount = OptData->OptCount;
		int I;

		/* Ciclo di impostazione dei parametri di configurazione */
		for (I = 0;I < OptCount;I++)
			{
				/* Verifica se invocato a linea di comando */
				if (OptData->ParmSet[I] == True)
					{
						/* Verifica il tipo del parametro */
						switch (CfgParmsDef[I].type)
							{
								/* Stringa */
								case CFG_STRING:
									/* Dealloca se presente */
									if (*((void **) CfgParmsDef[I].value) != NULL)
										free(*((void **) CfgParmsDef[I].value));

									/* Assegna il parametro */
									if (OptData->Parms[I].StrParm == NULL)
										*((void **) CfgParmsDef[I].value) =	NULL;
									else
										*((void **) CfgParmsDef[I].value) =
											(void *) strdup(OptData->Parms[I].StrParm);
								break;

								/* Intero */
								case CFG_INT:
									*((int *) CfgParmsDef[I].value) = OptData->Parms[I].IntParm;
								break;

								/* Floating point */
								case DRCCfgFloat:
									*((DRCFloat *) CfgParmsDef[I].value) =
										OptData->Parms[I].FltParm;
								break;

								/* Parametri non supportati */
								default:
									perror("\nCopyCmdLineParms: invalid parameter type.");
								break;
							}
					}
			}
	}

/* Visualizza le opzioni disponibili a linea di comando */
void ShowCmdLine(const cfgStruct * CfgParmsDef)
	{
		int I;

		/* Ciclo di visualizzazione dei parametri di configurazione */
		I = 0;
		while (CfgParmsDef[I].type != CFG_END)
			{
				/* Verifica il tipo del parametro */
				switch (CfgParmsDef[I].type)
					{
						/* Stringa */
						case CFG_STRING:
							printf("  --%-28s <STRING>\n",CfgParmsDef[I].parameterName);
						break;

						/* Intero */
						case CFG_INT:
							printf("  --%-28s <INTEGER>\n",CfgParmsDef[I].parameterName);
						break;

						/* Floating point */
						case DRCCfgFloat:
							printf("  --%-28s <FLOAT>\n",CfgParmsDef[I].parameterName);
						break;

						/* Parametri non supportati */
						default:
							perror("\nShowCmdLine: invalid parameter type.");
						break;
					}

				/* Passa al parametro successivo */
				I++;
			}
		/* Svuota i buffer */
		fflush(stdout);
	}

/* Libera la memoria occupata per la gestione della command line */
void FreeCmdLine(CmdLineType * OptData, const cfgStruct * CfgParmsDef)
	{
		int OptCount = OptData->OptCount;
		int I;

		/* Ciclo di dellocazione dei parametri di configurazione */
		for (I = 0;I < OptCount;I++)
			{
				/* Verifica se invocato a linea di comando */
				if (OptData->ParmSet[I] == True)
					{
						/* Verifica il tipo del parametro */
						switch (CfgParmsDef[I].type)
							{
								/* Stringa */
								case CFG_STRING:
									/* Dealloca se presente */
									if (OptData->Parms[I].StrParm != NULL)
										free(OptData->Parms[I].StrParm);
								break;

								/* Intero */
								case CFG_INT:
								break;

								/* Floating point */
								case DRCCfgFloat:
								break;

								/* Parametri non supportati */
								default:
									perror("\nFreeCmdLine: invalid parameter type.");
								break;
							}
					}
			}

		/* Dealloca la struttura dati */
		free(OptData->Options);
		free(OptData->Parms);
		free(OptData->ParmSet);
		free(OptData);
	}

/* Libera la memoria di una strttura di configurazione */
void FreeCfgStruct(cfgStruct * CfgParmsDef)
	{
		int OptCount;
		int I;

		/* Recupera il conteggio parametri */
		OptCount = GetCfgStructCount(CfgParmsDef);

		/* Ciclo di dellocazione dei parametri di configurazione */
		for (I = 0;I < OptCount;I++)
			{
				/* Verifica il tipo del parametro */
				switch (CfgParmsDef[I].type)
					{
						/* Stringa */
						case CFG_STRING:
							/* Dealloca se presente */
							if (*((void **) CfgParmsDef[I].value) != NULL)
								free(*((void **) CfgParmsDef[I].value));
						break;

						/* Intero */
						case CFG_INT:
						break;

						/* Floating point */
						case DRCCfgFloat:
						break;

						/* Parametri non supportati */
						default:
							perror("\nFreeCfgStruct: invalid parameter type.");
						break;
					}
			}
	}
