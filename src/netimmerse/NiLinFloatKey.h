/*
===============================================================================

	NiLinFloatKey.h

	A float key interpolated linearly between the values of the two keys
	around the time.

	Evidence: its register function (0x7A73D0) stores entry 1 (LINKEY) of
	the six NiFloatKey tables; its creation function (0x7A74B0) allocates 8
	bytes, the float key's own size, and its interpolation (0x7A7440)
	passes the two keys' values to NiLinearInterp. The class name follows
	the Gamebryo lineage.

===============================================================================
*/

#ifndef __NILINFLOATKEY_H__
#define __NILINFLOATKEY_H__

#include "NiFloatKey.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x004ca730 ??1NiLinFloatKey@@QAE@XZ @game/EffectController
// FUNCTION: STBC 0x004caa80 ??_ENiLinFloatKey@@QAEPAXI@Z @game/EffectController
class NiLinFloatKey : public NiFloatKey {
public:
							// FUNCTION: STBC 0x004ca720 ??0NiLinFloatKey@@QAE@XZ @game/EffectController
							NiLinFloatKey() {}
							NiLinFloatKey( float time_, float value_ ) { time = time_; value = value_; }

	// fills in the LINKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the LINKEY entries (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	void					LoadBinary( NiStream &stream, int unknown );
};

NiRegisterStream( NiLinFloatKey );

#endif /* !__NILINFLOATKEY_H__ */
