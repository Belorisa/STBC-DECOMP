/*
===============================================================================

	TGColorA.h

	The game's colour with alpha. The SWIG interface derives TGColorA from
	NiColorA (its type table lists _p_TGColorA as convertible to
	_p_NiColorA) and names the methods GetR to SetA, SetRGBA, Copy and
	ScaleRGB.

	Unlike NiColorA, TGColorA has a destructor: delete_TGColorA tests the
	pointer before freeing it, a function returning a TGColorA by value
	(ShieldProperty::GetShieldGlowColor, 0x5AF3D0) clears a
	constructed-result flag, and BlinkingLightProperty's constructor gives
	its colour member an EH state. Assigning an NiColorA to one goes
	through a converting constructor that takes the NiColorA by value
	(0x5AF390): BlinkingLightProperty's constructor copies NiColorA::WHITE
	into the argument and assigns the temporary.

===============================================================================
*/

#ifndef __TGCOLORA_H__
#define __TGCOLORA_H__

#include "NiColor.h"

class TGColorA : public NiColorA {
public:
				TGColorA( void ) {}
	// FUNCTION: STBC 0x005af390 ??0TGColorA@@QAE@VNiColorA@@@Z
				TGColorA( NiColorA color ) { r = color.r; g = color.g; b = color.b; a = color.a; }
				~TGColorA( void ) {}

	float		GetR( void ) const { return r; }
	float		GetG( void ) const { return g; }
	float		GetB( void ) const { return b; }
	float		GetA( void ) const { return a; }
	void		SetRGBA( float r_, float g_, float b_, float a_ ) { r = r_; g = g_; b = b_; a = a_; }
	void	SetR( float r_ ) { r = r_; }
	void	SetA( float a_ ) { a = a_; }
	void	SetB( float b_ ) { b = b_; }
	void	SetG( float g_ ) { g = g_; }
	void		ScaleRGB( float scale ) { r *= scale; g *= scale; b *= scale; }
	// FUNCTION: STBC 0x005eed10 ?Copy@TGColorA@@QAEXVNiColorA@@@Z
	void		Copy( NiColorA color ) { r = color.r; g = color.g; b = color.b; a = color.a; }
};

#endif /* !__TGCOLORA_H__ */
