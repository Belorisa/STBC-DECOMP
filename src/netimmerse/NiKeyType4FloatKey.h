/*
===============================================================================

	NiKeyType4FloatKey.h

	The float keys of key type NiFloatKey::KEYTYPE4: laid out as TCB keys,
	whose copying, comparison and streaming they share, but they do not
	interpolate (the value is zero), derive no tangents and are not read
	from a stream (the creation answers no key). NiMorphKey derives from
	it. The class name is invented; TODO identity.

	Evidence: 0x79BDA0 fills entry 4 of the float key tables; the copy
	(0x79BE10), the comparison (0x79BE30) and the save (0x79BE90) pass
	their arguments to the NiTCBFloatKey functions, and the member at
	0x79BE70 to NiTCBFloatKey::LoadBinary with the same this. NiMorphKey's
	comparison (0x79C090), load (0x79C4B0) and save (0x79C4F0) call these
	three first, and its unit registers this type as well as its own.

===============================================================================
*/

#ifndef __NIKEYTYPE4FLOATKEY_H__
#define __NIKEYTYPE4FLOATKEY_H__

#include "NiTCBFloatKey.h"
#include "NiStreamMacros.h"

class NiKeyType4FloatKey : public NiTCBFloatKey {
public:
							NiKeyType4FloatKey() {}
							NiKeyType4FloatKey( float time_, float value_, float tension_, float continuity_, float bias_ )
								: NiTCBFloatKey( time_, value_, tension_, continuity_, bias_ ) {}

	// fills in the KEYTYPE4 entries of the tables, once; 1 on the first
	// call (name invented)
	static int				Register();

	// the KEYTYPE4 entries (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	void					LoadBinary( NiStream &stream, int unknown );
};

NiRegisterStream( NiKeyType4FloatKey );

#endif /* !__NIKEYTYPE4FLOATKEY_H__ */
