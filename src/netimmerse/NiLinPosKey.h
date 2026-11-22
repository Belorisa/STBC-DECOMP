/*
===============================================================================

	NiLinPosKey.h

	A position key interpolated linearly between the positions of the two
	keys around the time.

	Evidence: its register function (0x795EF0) stores entry 1 (LINKEY) of
	the nine NiPosKey tables; its creation function (0x796070) allocates
	0x10 bytes, the position key's own size, its interpolation (0x795FB0)
	blends the two keys' positions by t, and its first derivative
	(0x796020) is their difference. The class name follows the Gamebryo
	lineage.

===============================================================================
*/

#ifndef __NILINPOSKEY_H__
#define __NILINPOSKEY_H__

#include "NiPosKey.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x004d3560 ??1NiLinPosKey@@QAE@XZ @game/TexturedSparksController
// FUNCTION: STBC 0x004d38c0 ??_ENiLinPosKey@@QAEPAXI@Z @game/TexturedSparksController
class NiLinPosKey : public NiPosKey {
public:
							NiLinPosKey() {}
							NiLinPosKey( float time_, const NiPoint3 &pos_ ) : NiPosKey( time_, pos_ ) {}

	// fills in the LINKEY entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the LINKEY entries (names invented)
	static NiPosKey *		Create( NiStream &stream );
	static NiPoint3			Interpolate( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 );
	static float			ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys );
	static void				FillDerivedValues( NiPosKey **keys, unsigned int numKeys );
	static bool				Equal( NiPosKey *key0, NiPosKey *key1 );
	static void				Save( NiStream &stream, NiPosKey *key );
	static NiPosKey *		Copy( NiPosKey *key );

	void					LoadBinary( NiStream &stream );
};

NiRegisterStream( NiLinPosKey );

#endif /* !__NILINPOSKEY_H__ */
