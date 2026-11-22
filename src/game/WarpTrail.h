/*
===============================================================================

	WarpTrail.h

	One streak of light in a WarpSet's tunnel: a textured quad on a
	billboard node that turns about its up axis to face the camera. The
	class name is invented, after the Effects script values it reads
	(g_pcWarpTrailTextureName, g_fWarpTrailWidth, g_fWarpTrailHeight).

	0xEC bytes, NiBillboardNode's size. Evidence: WarpSet::SetNumTrails
	(0x4D7D50) allocates 0xEC bytes per trail and calls the constructor
	(0x4D88E0), which builds NiBillboardNode's inline (vtable 0x88CD48),
	stores the vtable 0x88CE8C and attaches the quad. The vtable keeps
	NiBillboardNode's 44 slots, GetRTTI included (the class has no NiRTTI of
	its own), replaces the destructor (0x4D8D50, the deleting destructor of
	0x4D8D70, which only stores the vtable) and adds slot 44 (0x4D8D80);
	AnimatedTriShapeParticles' vtable (0x88CF40) follows it. The class has
	a unit of its own after WarpSet's: a second NiMain registration run
	(0x4D8030-0x4D88D0) comes before its code, and its <iostream> statics
	(0x4D8D90-0x4D8E00) after it.

===============================================================================
*/

#ifndef __WARPTRAIL_H__
#define __WARPTRAIL_H__

#include "NiBillboardNode.h"

// FUNCTION: STBC 0x004d8d50 ??_GWarpTrail@@UAEPAXI@Z @game/WarpTrail
class WarpTrail : public NiBillboardNode {
public:
							WarpTrail();
	virtual					~WarpTrail();

	// Updates the trail's subtree and its parents' bounds: NiAVObject's
	// Update, made virtual here (slot 44; the name is a placeholder).
	virtual void			VirtualSlot44( float time );
};

#endif /* !__WARPTRAIL_H__ */
