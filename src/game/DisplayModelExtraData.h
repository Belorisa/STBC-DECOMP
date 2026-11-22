/*
===============================================================================

	DisplayModelExtraData.h

	The extra data block that marks a node as a property's display model:
	the debug geometry a subsystem or position property shows on a model
	(TGModelProperty slot 15) carries one. It adds nothing to NiExtraData.

	0x14 bytes. Evidence: its NiRTTI at 0x992D78 is built with the name
	"DisplayModelExtraData" (0x958604) and NiExtraData's NiRTTI as its
	base (0x680FB0). The slot 15 overrides of SubsystemProperty (0x6929A0),
	PositionOrientationProperty (0x6891E0), PhaserProperty,
	PulseWeaponProperty, TorpedoTubeProperty and TractorBeamProperty
	allocate 0x14 bytes, call the constructor (0x681000), which runs
	NiExtraData's and stores the vtable 0x894F84, and add the block to the
	new node. The vtable overrides only GetRTTI (0x681140) and the
	destructor (0x681040, which stores the vtable before NiExtraData's).

===============================================================================
*/

#ifndef __DISPLAYMODELEXTRADATA_H__
#define __DISPLAYMODELEXTRADATA_H__

#include "NiExtraData.h"

// FUNCTION: STBC 0x00681020 ??_GDisplayModelExtraData@@UAEPAXI@Z
class DisplayModelExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x00681140 ?GetRTTI@DisplayModelExtraData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~DisplayModelExtraData();

							DisplayModelExtraData();

	static const NiRTTI		rtti;
};

#endif /* !__DISPLAYMODELEXTRADATA_H__ */
