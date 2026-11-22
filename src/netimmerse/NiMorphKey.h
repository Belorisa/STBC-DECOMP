/*
===============================================================================

	NiMorphKey.h

	A morph key: a TCB float key that holds one value per morph target,
	with the derived tangents of each: the key type NiFloatKey::MORPHKEY.
	The name is invented; the class is known only from its functions.

	Evidence: 0x79BFC0 fills entry 5 of the float key tables; the key is
	0x28 bytes (the creation at 0x79C470), an NiKeyType4FloatKey (whose
	comparison, load and save it calls first) followed by three arrays: the values (0x1C), the incoming tangents
	(0x20) and the outgoing tangents (0x24), which the derived-value
	function (0x79C220) fills target by target from the TCB tangents. The
	extra argument of the table functions is the number of values. The
	destructor (0x79C060) frees the three arrays; only NiMorphController's
	destructor calls it, and only NiMorphController (0x7A8880) calls the
	interpolation of every value (0x79C400).

===============================================================================
*/

#ifndef __NIMORPHKEY_H__
#define __NIMORPHKEY_H__

#include "NiKeyType4FloatKey.h"
#include "NiStreamMacros.h"

class NiMorphKey : public NiKeyType4FloatKey {
public:
							NiMorphKey() { values = 0; dsValues = 0; ddValues = 0; }
							NiMorphKey( float time_, float tension_, float continuity_, float bias_, float *values_ );
							~NiMorphKey();

	// fills in the MORPHKEY entries of the tables, once; 1 on the first
	// call (name invented)
	static int				Register();

	// the MORPHKEY entries; the last argument is the number of values
	// (names invented)
	static NiFloatKey *		Create( NiStream &stream, int numValues );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int numValues );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int numValues );
	static void				Save( NiStream &stream, NiFloatKey *key, int numValues );
	static NiFloatKey *		Copy( NiFloatKey *key, int numValues );

	// every value at t between two keys, in a new array the caller owns
	// (name invented)
	static float *			InterpolateValues( float t, NiMorphKey *key0, NiMorphKey *key1, unsigned int numValues );

	// takes ownership of values in place of the current ones (name
	// invented)
	void					SetValues( float *values_ );
	// (name invented; AnimOnly.exe 0x4980A0)
	float *					GetValues() const { return values; }

	void					LoadBinary( NiStream &stream, int numValues );

	float *					values;		// owned: one value per morph target
	float *					dsValues;	// owned: the incoming tangent of each value, derived
	float *					ddValues;	// owned: the outgoing tangent of each value, derived
};

NiRegisterStream( NiMorphKey );

#endif /* !__NIMORPHKEY_H__ */
