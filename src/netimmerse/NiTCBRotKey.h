/*
===============================================================================

	NiTCBRotKey.h

	A rotation key on a Kochanek-Bartels (tension, continuity, bias)
	spline: the key type NiRotKey::TCBKEY. Each key derives from its
	neighbours the two inner control points of the squad curves that meet
	at its rotation: a for the curve leaving it, b for the curve arriving.

	Evidence: 0x7B54A0 fills entry 3 of the six rotation key tables; the
	key is 0x40 bytes (the creation at 0x7B5940); the load (0x7B59C0) and
	save (0x7B5A00) stream the rotation key then the floats at 0x14, 0x18
	and 0x1C; the tangent function (0x7B5690) weighs the logarithms of the
	steps from the previous rotation and to the next one by those three and
	stores the control points at 0x20 and 0x30; the interpolation (0x7B5660)
	passes a key's rotation and 0x20 with the next key's 0x30 and rotation
	to NiQuaternion::Squad. Names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NITCBROTKEY_H__
#define __NITCBROTKEY_H__

#include "NiRotKey.h"
#include "NiStreamMacros.h"

class NiTCBRotKey : public NiRotKey {
public:
							NiTCBRotKey() {}
							NiTCBRotKey( float time_, const NiQuaternion &quat_ ) : NiRotKey( time_, quat_ ) {}

	// fills in the TCBKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the TCBKEY entries (names invented)
	static NiRotKey *		Create( NiStream &stream );
	static NiQuaternion		Interpolate( float t, NiRotKey *key0, NiRotKey *key1 );
	static void				FillDerivedValues( NiRotKey **keys, unsigned int numKeys );
	static bool				Equal( NiRotKey *key0, NiRotKey *key1 );
	static void				Save( NiStream &stream, NiRotKey *key );
	static NiRotKey *		Copy( NiRotKey *key );

	// the control points from the neighbouring keys (Gamebryo lineage name)
	void					CalculateDVals( const NiTCBRotKey *prev, const NiTCBRotKey *next );

	// the rotation key, then the tension, continuity and bias
	void					LoadBinary( NiStream &stream );

	float					tension;
	float					continuity;
	float					bias;
	NiQuaternion			a;			// the inner control point of the curve leaving this key, derived
	NiQuaternion			b;			// the inner control point of the curve arriving at this key, derived
};

NiRegisterStream( NiTCBRotKey );

#endif /* !__NITCBROTKEY_H__ */
