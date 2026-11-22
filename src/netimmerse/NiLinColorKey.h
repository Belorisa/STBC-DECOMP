/*
===============================================================================

	NiLinColorKey.h

	A colour key interpolated linearly between the colours of the two keys
	around the time.

	Evidence: its register function (0x7A70F0) stores entry 1 (LINKEY) of
	NiColorKey's interpolation table (0x9A02A4); the game's effect
	controllers keep arrays of these keys by value (0x14 bytes: the time and
	an NiColorA), whose element constructor (0x4CB330) clears the colour.
	The class name follows the Gamebryo lineage.

===============================================================================
*/

#ifndef __NILINCOLORKEY_H__
#define __NILINCOLORKEY_H__

#include "NiColorKey.h"
#include "NiStreamMacros.h"

// FUNCTION: STBC 0x004ca5a0 ??1NiLinColorKey@@QAE@XZ @game/EffectController
// FUNCTION: STBC 0x004ca9d0 ??_ENiLinColorKey@@QAEPAXI@Z @game/EffectController
class NiLinColorKey : public NiColorKey {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7A70F0)
	static int				Register();

	// the LINKEY entries (names invented)
	static NiColorKey *		Create( NiStream &stream );
	static NiColorA			Interpolate( float t, NiColorKey *key0, NiColorKey *key1 );
	static NiColorKey *		Copy( NiColorKey *key );
	static void				Save( NiStream &stream, NiColorKey *key );
	// FUNCTION: STBC 0x7A73B0 ?Equal@NiLinColorKey@@SA_NPAVNiColorKey@@0@Z @netimmerse/NiLinColorKey
	static bool				Equal( NiColorKey *key0, NiColorKey *key1 ) { return NiColorKey::Equal( key0, key1 ); }

	void					LoadBinary( NiStream &stream );

							// FUNCTION: STBC 0x004cb330 ??0NiLinColorKey@@QAE@XZ @game/EffectController
							NiLinColorKey() {}
							NiLinColorKey( float time_, const NiColorA &color_ ) { time = time_; color = color_; }
};

NiRegisterStream( NiLinColorKey );

#endif /* !__NILINCOLORKEY_H__ */
