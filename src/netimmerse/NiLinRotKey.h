/*
===============================================================================

	NiLinRotKey.h

	A rotation key interpolated linearly: spherically between the
	quaternions of the two keys around the time.

	Evidence: its register function (0x7961E0) stores entry 1 (LINKEY) of
	the six NiRotKey tables; its creation function (0x7962F0) allocates
	0x14 bytes, the rotation key's own size, and its interpolation
	(0x7962C0) is NiQuaternion::Slerp of the two keys' rotations. The class
	name follows the Gamebryo lineage.

===============================================================================
*/

#ifndef __NILINROTKEY_H__
#define __NILINROTKEY_H__

#include "NiRotKey.h"
#include "NiStreamMacros.h"

class NiLinRotKey : public NiRotKey {
public:
							NiLinRotKey() {}
							NiLinRotKey( float time_, const NiQuaternion &quat_ ) : NiRotKey( time_, quat_ ) {}

	// fills in the LINKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the LINKEY entries (names invented)
	static NiRotKey *		Create( NiStream &stream );
	static NiQuaternion		Interpolate( float t, NiRotKey *key0, NiRotKey *key1 );
	static void				FillDerivedValues( NiRotKey **keys, unsigned int numKeys );
	static bool				Equal( NiRotKey *key0, NiRotKey *key1 );
	static void				Save( NiStream &stream, NiRotKey *key );
	static NiRotKey *		Copy( NiRotKey *key );

	void					LoadBinary( NiStream &stream );
};

NiRegisterStream( NiLinRotKey );

#endif /* !__NILINROTKEY_H__ */
