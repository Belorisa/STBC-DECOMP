/*
===============================================================================

	NiBezRotKey.h

	A rotation key on a squad curve (the spherical analogue of a cubic
	Bezier curve): the key type NiRotKey::BEZKEY. Each key derives the
	inner control point of the curve at its rotation from its neighbours.

	Evidence: 0x79D0C0 fills entry 2 of the six rotation key tables; the
	key is 0x24 bytes (the creation at 0x79D300); the derived-value
	function (0x79D230) stores NiQuaternion::Intermediate of each key and
	its neighbours at 0x14, the interpolation (0x79D200) passes a key's
	rotation and control point with the next key's control point and
	rotation to NiQuaternion::Squad, and the comparison (0x79D120) compares
	the control points too. Only the rotation key is streamed. Names follow
	the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIBEZROTKEY_H__
#define __NIBEZROTKEY_H__

#include "NiRotKey.h"
#include "NiStreamMacros.h"

class NiBezRotKey : public NiRotKey {
public:
							NiBezRotKey() {}
							NiBezRotKey( float time_, const NiQuaternion &quat_ ) : NiRotKey( time_, quat_ ) {}

	// fills in the BEZKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the BEZKEY entries (names invented)
	static NiRotKey *		Create( NiStream &stream );
	static NiQuaternion		Interpolate( float t, NiRotKey *key0, NiRotKey *key1 );
	static void				FillDerivedValues( NiRotKey **keys, unsigned int numKeys );
	static bool				Equal( NiRotKey *key0, NiRotKey *key1 );
	static void				Save( NiStream &stream, NiRotKey *key );
	static NiRotKey *		Copy( NiRotKey *key );

	// the rotation key alone: the control point is derived
	void					LoadBinary( NiStream &stream );

	NiQuaternion			intQuat;	// the inner control point of the curve at this key, derived
};

NiRegisterStream( NiBezRotKey );

#endif /* !__NIBEZROTKEY_H__ */
