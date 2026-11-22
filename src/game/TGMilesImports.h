/*
===============================================================================

	TGMilesImports.h

	The Miles Sound System entry point the sound manager calls beyond those
	NiMilesImports.h declares: the digital driver's configuration. Declared
	as MSS32.DLL exports it; the import table's decorated name gives the
	stdcall argument size.

	Evidence: the sound manager reads the driver's configuration through
	_AIL_digital_configuration@16 (IAT 0x88849C); the sound file cache
	(TGSoundFile) reads whole files through _AIL_file_size@4 and
	_AIL_file_read@8 (IAT 0x8884A4, 0x8884A8) and frees them through
	_AIL_mem_free_lock@4 (IAT 0x8884A0).

===============================================================================
*/

#ifndef __TGMILESIMPORTS_H__
#define __TGMILESIMPORTS_H__

#include "NiMilesImports.h"

extern "C" {

__declspec( dllimport ) int AILCALL			AIL_digital_configuration( HDIGDRIVER dig, int *rate, int *format, char *string );

__declspec( dllimport ) void AILCALL		AIL_mem_free_lock( void *ptr );
__declspec( dllimport ) unsigned int AILCALL	AIL_file_size( const char *filename );
__declspec( dllimport ) void * AILCALL		AIL_file_read( const char *filename, void *dest );

}

#endif /* !__TGMILESIMPORTS_H__ */
