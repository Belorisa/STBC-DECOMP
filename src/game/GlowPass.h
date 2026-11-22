/*
===============================================================================

	GlowPass.h

	GlowPass is the extra data that marks a geometry's glow: its RTTI
	(0x9861B0) is named "GlowPass" with NiExtraData's as its base, and
	LODModel::AddGlowPassTexture makes them. The accumulator, during the
	glow's camera click, draws each registered geometry with its GlowPass
	properties (or its own, without one) swapped into the renderer's
	property state.

	Its own unit: the RTTI (0x9861B0) and the class registrations of
	NiObject, NiExtraData, NiTimeController, NiObjectNET and NiProperty
	(0x4E1A70-0x4E1B00, no NiAVObject) share .bss 0x9861A0-0x9861C4 with
	CreateClone (0x4E18E0) and CopyMembers (0x4E1970).

===============================================================================
*/

#ifndef __GLOWPASS_H__
#define __GLOWPASS_H__

#include "NiExtraData.h"
#include "NiProperty.h"

/*
================
GlowPass

The properties a geometry glows with. 0x20 bytes: NiExtraData's, a flag at
0x14 (set by the constructor; a geometry whose GlowPass has it set is not
drawn into the glow) and a property list at 0x18. The vtable 0x88B730
overrides CreateClone (0x4E18E0) and the destructor; the copies of the
inline GetRTTI (0x450410) and of the destructor (0x450440) that the linker
kept come from LODModel's unit.
================
*/
// FUNCTION: STBC 0x00450420 ??_GGlowPass@@UAEPAXI@Z @game/LODModel
class GlowPass : public NiExtraData {
public:
	// FUNCTION: STBC 0x00450410 ?GetRTTI@GlowPass@@UBEPBVNiRTTI@@XZ @game/LODModel
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// FUNCTION: STBC 0x00450440 ??1GlowPass@@UAE@XZ @game/LODModel
	virtual					~GlowPass() {
								while ( !properties.IsEmpty() ) {
									properties.RemoveHead();
								}
							}

							GlowPass() : unknown14( true ) {}

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( GlowPass *dest );

public:
	bool					unknown14;		// 0x14 true: the geometry is left out of the glow
	NiPropertyList			properties;		// 0x18 swapped into the renderer's state while it is drawn
};

#endif /* !__GLOWPASS_H__ */
