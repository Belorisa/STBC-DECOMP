/*
===============================================================================

	NiBezPosKey.h

	A position key on a cubic Bezier curve: the key type NiPosKey::BEZKEY.
	Each key keeps the incoming and outgoing tangents it was saved with and
	derives the two higher coefficients of the cubic from its position to
	the next key's, so that the curve is pos + out t + a t^2 + b t^3.

	Evidence: 0x79C940 fills entry 2 of the nine position key tables; the
	key is 0x40 bytes (the creation at 0x79CF50); the load (0x79CF70) and
	save (0x79CFA0) stream the position key then the NiPoint3s at 0x10 and
	0x1C; the derived-value function (0x79CCB0) fills 0x28 and 0x34 from
	the positions and the tangents at 0x1C of a key and 0x10 of the next;
	the interpolation (0x79CAF0) evaluates the cubic. Names follow the
	Gamebryo lineage.

===============================================================================
*/

#ifndef __NIBEZPOSKEY_H__
#define __NIBEZPOSKEY_H__

#include "NiPosKey.h"
#include "NiStreamMacros.h"

class NiBezPosKey : public NiPosKey {
public:
							NiBezPosKey() {}
							NiBezPosKey( float time_, const NiPoint3 &pos_, const NiPoint3 &inTangent_, const NiPoint3 &outTangent_ )
								: NiPosKey( time_, pos_ ) {
								inTangent = inTangent_;
								outTangent = outTangent_;
							}

	// fills in the BEZKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the BEZKEY entries (names invented)
	static NiPosKey *		Create( NiStream &stream );
	static NiPoint3			Interpolate( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 );
	static float			ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys );
	static void				FillDerivedValues( NiPosKey **keys, unsigned int numKeys );
	static bool				Equal( NiPosKey *key0, NiPosKey *key1 );
	static void				Save( NiStream &stream, NiPosKey *key );
	static NiPosKey *		Copy( NiPosKey *key );

	// the position key, then the two tangents
	void					LoadBinary( NiStream &stream );

	NiPoint3				inTangent;
	NiPoint3				outTangent;
	NiPoint3				a;			// the coefficient of t^2 to the next key, derived
	NiPoint3				b;			// the coefficient of t^3 to the next key, derived
};

NiRegisterStream( NiBezPosKey );

#endif /* !__NIBEZPOSKEY_H__ */
