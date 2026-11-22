/*
===============================================================================

	NiTCBFloatKey.h

	A float key interpolated by a Kochanek-Bartels (tension, continuity,
	bias) spline: the key type NiFloatKey::TCBKEY.

	Evidence: 0x79B920 fills entry 3 of the float key tables; the copy
	(0x79BA30) allocates 0x1C bytes; the derived-value function (0x79BA80)
	computes the tangents at 0x14 and 0x18 from the tension at 8, the
	continuity at 0xC and the bias at 0x10; the interpolation (0x79B9F0)
	passes a key's value and 0x18 with the next key's value and 0x14 to
	the Hermite basis (0x7B6F20). Names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NITCBFLOATKEY_H__
#define __NITCBFLOATKEY_H__

#include "NiFloatKey.h"
#include "NiStreamMacros.h"

class NiTCBFloatKey : public NiFloatKey {
public:
							NiTCBFloatKey() {}
							NiTCBFloatKey( float time_, float value_, float tension_, float continuity_, float bias_ ) {
								time = time_;
								value = value_;
								tension = tension_;
								continuity = continuity_;
								bias = bias_;
							}

	float					GetTension() const { return tension; }
	float					GetContinuity() const { return continuity; }
	float					GetBias() const { return bias; }

	// fills in the TCBKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the TCBKEY entries (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	// the tangents of key from the values of its neighbours and the lengths
	// of the intervals to them (Gamebryo lineage name)
	static void				CalculateDVals( NiTCBFloatKey *key, float prevValue, float nextValue, float prevLength, float nextLength );

	// the float key, then the tension, continuity and bias
	void					LoadBinary( NiStream &stream, int unknown );

	float					tension;
	float					continuity;
	float					bias;
	float					ds;			// the incoming tangent, derived
	float					dd;			// the outgoing tangent, derived
};

NiRegisterStream( NiTCBFloatKey );

#endif /* !__NITCBFLOATKEY_H__ */
