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

#ifndef CmdLine_h
	#define CmdLine_h

	#include "drc.h"
	#include "parsecfg.h"
	#include "boolean.h"
	#include <getopt.h>

	/* Struttura gestione parametri */
	typedef
		union
			{
				char * StrParm;
				int IntParm;
				DRCFloat FltParm;
			}
		ParameterType;

	/* Struttura gestione opzioni */
	typedef
		struct
			{
				int OptCount;
				struct option * Options;
				ParameterType * Parms;
				Boolean * ParmSet;
			}
		CmdLineType;

	#ifdef __cplusplus
		extern "C"
			{
	#endif

	/* Registra le impostazioni per la command line a partire
	da una definizione ParseCfg. Torna una struttura opaque
	utilizzata per il recupero dei parametri dalla linea di comando. */
	CmdLineType * RegisterCmdLine(const cfgStruct * CfgParmsDef);

	/* Effettua il processing della command line */
	int GetCmdLine(const int argc, char * const * argv,
		const cfgStruct * CfgParmsDef, CmdLineType * OptData,
		char ** CfgFileName);

	/* Sovrascrive i parametri della definizione ParseCfg CfgParmsDef con
	i parametri recuperati dalla linea di comando e registrati nella
	in OptData. */
	void CopyCmdLineParms(const CmdLineType * OptData,cfgStruct * CfgParmsDef);

	/* Visualizza le opzioni disponibili a linea di comando */
	void ShowCmdLine(const cfgStruct * CfgParmsDef);

	/* Libera la memoria occupata per la gestione della command line */
	void FreeCmdLine(CmdLineType * OptData, const cfgStruct * CfgParmsDef);

	/* Libera la memoria di una strttura di configurazione */
	void FreeCfgStruct(cfgStruct * CfgParmsDef);

	#ifdef __cplusplus
		}
	#endif
#endif
