/*
===============================================================================

	NiEulerRotKey.h

	A rotation animated as three Euler angles: one key of this type holds
	three groups of float keys, one per axis, and the order in which the
	axis rotations compose.

	Evidence: its register function (0x7964B0) stores entry 4 (EULERKEY)
	of the six NiRotKey tables; its creation (0x796860) and copy
	(0x7966F0) functions allocate 0x48 bytes and clear the three counts at
	0x14, the key types at 0x20, the axis order at 0x2C, the key arrays at
	0x30 and the last indices at 0x3C. The interpolation (0x796560) feeds
	each group to NiFloatKey::GenInterp, turns the angles into rotations
	about x, y and z and multiplies them in the order the table at 0x975FD0
	gives for the axis order. NiKeyframeData (0x7921F0) frees the groups
	through Cleanup (0x796510) on the first key before freeing the keys.
	The class, enum and Cleanup names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIEULERROTKEY_H__
#define __NIEULERROTKEY_H__

#include "NiFloatKey.h"
#include "NiRotKey.h"
#include "NiStreamMacros.h"

class NiEulerRotKey : public NiRotKey {
public:
							NiEulerRotKey();

	// the orders in which the three axis rotations compose, the first
	// applied first (Gamebryo lineage names)
	enum AxisOrder {
		XYZ,
		XZY,
		YZX,
		YXZ,
		ZXY,
		ZYX,
		XYX,
		YZY,
		ZXZ,
		NUMAXISORDERS
	};

	// fills in the EULERKEY entries of the tables, once; 1 on the first
	// call (name invented)
	static int				Register();

	// the EULERKEY entries (names invented)
	static NiRotKey *		Create( NiStream &stream );
	static NiQuaternion		Interpolate( float t, NiRotKey *key0, NiRotKey *key1 );
	static void				FillDerivedValues( NiRotKey **keys, unsigned int numKeys );
	static bool				Equal( NiRotKey *key0, NiRotKey *key1 );
	static void				Save( NiStream &stream, NiRotKey *key );
	static NiRotKey *		Copy( NiRotKey *key );

	// frees the three groups of float keys
	void					Cleanup();

	// one axis's float keys and their count (Gamebryo lineage names)
	unsigned int			GetNumKeys( unsigned char axis ) const { return numKeys[axis]; }
	NiFloatKey **			GetKeys( unsigned char axis ) const { return keys[axis]; }
	// the axis order (Gamebryo lineage name; AnimOnly.exe 0x47E0F0, which
	// Interpolate calls)
	int						GetOrder() const { return axisOrder; }

	// the axis order, then each group: its count and, when it has keys,
	// their type and the keys
	void					LoadBinary( NiStream &stream );

	// the axes (0 x, 1 y, 2 z) of each order, the first applied first
	// (name invented)
	static int				axisOrderMap[NUMAXISORDERS][3];

	unsigned int			numKeys[3];
	NiFloatKey::KeyType		keyTypes[3];
	int						axisOrder;		// an AxisOrder
	NiFloatKey **			keys[3];
	unsigned int			lastIndex[3];	// GenInterp's search start per axis
};

NiRegisterStream( NiEulerRotKey );

/*
================
NiEulerRotKey::NiEulerRotKey
================
*/
inline NiEulerRotKey::NiEulerRotKey() {
	numKeys[0] = 0;
	numKeys[1] = 0;
	numKeys[2] = 0;
	axisOrder = XYZ;
	keys[0] = 0;
	keys[1] = 0;
	keys[2] = 0;
	keyTypes[0] = NiFloatKey::NOINTERP;
	keyTypes[1] = NiFloatKey::NOINTERP;
	keyTypes[2] = NiFloatKey::NOINTERP;
	lastIndex[0] = 0;
	lastIndex[1] = 0;
	lastIndex[2] = 0;
}

#endif /* !__NIEULERROTKEY_H__ */
