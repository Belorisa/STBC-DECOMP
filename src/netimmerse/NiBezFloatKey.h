/*
===============================================================================

	NiBezFloatKey.h

	A float key interpolated by a Bezier curve: the key type
	NiFloatKey::BEZKEY.

	Evidence: NiMorpherController::LoadBinary (0x7AB420) scales the value
	at 4 of every key of an old file to a fraction and, for keys of type
	BEZKEY, the two tangents at 8 and 0xC with it. 0x79C660 fills entry 2
	of the float key tables; the interpolation (0x79C720) passes a key's
	value and 0xC with the next key's value and 8 to the Bezier basis
	(0x7B6ED0). Names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIBEZFLOATKEY_H__
#define __NIBEZFLOATKEY_H__

#include "NiFloatKey.h"
#include "NiStreamMacros.h"

class NiBezFloatKey : public NiFloatKey {
public:
							NiBezFloatKey() {}
							NiBezFloatKey( float time_, float value_, float inTangent_, float outTangent_ ) {
								time = time_;
								value = value_;
								inTangent = inTangent_;
								outTangent = outTangent_;
							}

	float					GetInTangent() const { return inTangent; }
	float					GetOutTangent() const { return outTangent; }

	// fills in the BEZKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the BEZKEY entries (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	// the float key, then the two tangents
	void					LoadBinary( NiStream &stream, int unknown );

	float					inTangent;
	float					outTangent;
};

NiRegisterStream( NiBezFloatKey );

#endif /* !__NIBEZFLOATKEY_H__ */
