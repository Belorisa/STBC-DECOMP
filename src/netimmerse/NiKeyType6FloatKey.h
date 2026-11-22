/*
===============================================================================

	NiKeyType6FloatKey.h

	The float keys of key type NiFloatKey::KEYTYPE6: TCB keys whose value
	is the index of a morph target. Interpolated as a whole, the value
	picks the two targets of the keys around the time and splits a weight
	of one between them along the TCB curve; the float value itself does
	not interpolate (zero). The class name is invented; TODO identity.

	Evidence: 0x79DFD0 fills entry 6 of the float key tables (so the
	tables hold seven entries); the key is 0x1C bytes, an
	NiKeyType4FloatKey whose comparison, load and save it calls, with
	NiTCBFloatKey's tangents (0x79E030); the copy (0x79E0A0) converts the
	value to an int for the constructor (0x79E050), which stores it back
	as a float. NiMorphController's update (0x7A8880) calls the weight
	split (0x79E0F0) for these keys in place of NiMorphKey's
	InterpolateValues.

===============================================================================
*/

#ifndef __NIKEYTYPE6FLOATKEY_H__
#define __NIKEYTYPE6FLOATKEY_H__

#include "NiKeyType4FloatKey.h"
#include "NiStreamMacros.h"

class NiKeyType6FloatKey : public NiKeyType4FloatKey {
public:
							NiKeyType6FloatKey() {}
							NiKeyType6FloatKey( float time_, float tension_, float continuity_, float bias_, int target );

	// fills in the KEYTYPE6 entries of the tables, once; 1 on the first
	// call (name invented)
	static int				Register();

	// the KEYTYPE6 entries (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	// a new array of numValues target weights, zero but for the targets of
	// the two keys, which share a weight of one by the TCB curve at t
	// (name invented)
	static float *			InterpolateValues( float t, NiFloatKey *key0, NiFloatKey *key1, unsigned int numValues );

	void					LoadBinary( NiStream &stream, int unknown );
};

NiRegisterStream( NiKeyType6FloatKey );

#endif /* !__NIKEYTYPE6FLOATKEY_H__ */
