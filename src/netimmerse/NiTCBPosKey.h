/*
===============================================================================

	NiTCBPosKey.h

	A position key on a Kochanek-Bartels (tension, continuity, bias)
	spline: the key type NiPosKey::TCBKEY. Each key derives its incoming
	and outgoing tangents from its neighbours, then the two higher
	coefficients of the cubic to the next key, so that the curve is
	pos + dd t + a t^2 + b t^3.

	Evidence: 0x7B4900 fills entry 3 of the nine position key tables; the
	key is 0x4C bytes (the creation at 0x7B5300); the load (0x7B5320) and
	save (0x7B5360) stream the position key then the floats at 0x10, 0x14
	and 0x18; the tangent function (0x7B4D10) derives 0x1C and 0x28 from
	those three as the float keys do (0x79BA80); the derived-value
	function (0x7B4EF0) fills 0x34 and 0x40 from the tangents as the Bezier
	position keys do; the interpolation (0x7B4B50) evaluates the cubic.
	Names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NITCBPOSKEY_H__
#define __NITCBPOSKEY_H__

#include "NiPosKey.h"
#include "NiStreamMacros.h"

class NiTCBPosKey : public NiPosKey {
public:
							NiTCBPosKey() {}
							NiTCBPosKey( float time_, const NiPoint3 &pos_, float tension_, float continuity_, float bias_ )
								: NiPosKey( time_, pos_ ) {
								tension = tension_;
								continuity = continuity_;
								bias = bias_;
							}

	// fills in the TCBKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the TCBKEY entries (names invented)
	static NiPosKey *		Create( NiStream &stream );
	static NiPoint3			Interpolate( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 );
	static float			ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys );
	static void				FillDerivedValues( NiPosKey **keys, unsigned int numKeys );
	static bool				Equal( NiPosKey *key0, NiPosKey *key1 );
	static void				Save( NiStream &stream, NiPosKey *key );
	static NiPosKey *		Copy( NiPosKey *key );

	// the tangents from the positions of the neighbours and the lengths of
	// the intervals to them (Gamebryo lineage name)
	void					CalculateDVals( const NiPoint3 &prevPos, const NiPoint3 &nextPos, float prevLength, float nextLength );

	// the position key, then the tension, continuity and bias
	void					LoadBinary( NiStream &stream );

	float					tension;
	float					continuity;
	float					bias;
	NiPoint3				ds;			// the incoming tangent, derived
	NiPoint3				dd;			// the outgoing tangent, derived
	NiPoint3				a;			// the coefficient of t^2 to the next key, derived
	NiPoint3				b;			// the coefficient of t^3 to the next key, derived
};

NiRegisterStream( NiTCBPosKey );

#endif /* !__NITCBPOSKEY_H__ */
