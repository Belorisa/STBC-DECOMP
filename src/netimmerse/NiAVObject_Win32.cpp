// NiAVObject_Win32.cpp -- the platform part of NiAVObject: the world transform's update
//
// Gamebryo 1.2.2: Win32/NiAVObject_Win32.cpp holds NiAVObject::UpdateWorldData
// apart from NiAVObject.cpp; retail keeps it in a unit of its own after
// NiPoint2's (0x8201A0), with its <iostream> quads and registrations.

#include "NiRTLib.h"
#include "NiAVObject.h"
#include "NiNode.h"
#include "NiCamera.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00820380 _$E30
// FUNCTION: STBC 0x00820390 _$E41
// FUNCTION: STBC 0x008203a0 _$E45
// FUNCTION: STBC 0x008203b0 _$E49
// FUNCTION: STBC 0x008203c0 _$E53
// FUNCTION: STBC 0x008203d0 _$E177
// FUNCTION: STBC 0x008203e0 _$E190
// FUNCTION: STBC 0x008203f0 _$E119
// FUNCTION: STBC 0x00820400 _$E132
// FUNCTION: STBC 0x00820410 _$E182
// FUNCTION: STBC 0x00820420 _$E195
// FUNCTION: STBC 0x00820430 _$E186
// FUNCTION: STBC 0x00820440 _$E199
// FUNCTION: STBC 0x00820450 _$E206

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00820300 _$E17
// FUNCTION: STBC 0x00820320 _$E15
// FUNCTION: STBC 0x00820330 _$E23
// FUNCTION: STBC 0x00820350 _$E21
// FUNCTION: STBC 0x00820360 _$E213

/*
================
NiAVObject::UpdateWorldData

The world transform and velocity composed from the parent's, or the local
ones of an object with no parent.
================
*/
// FUNCTION: STBC 0x8201A0
void NiAVObject::UpdateWorldData() {
	if ( parent ) {
		worldScale = parent->worldScale * localScale;
		worldRotate = parent->worldRotate * localRotate;
		worldTranslate = parent->worldTranslate + parent->worldScale * ( parent->worldRotate * localTranslate );
		worldVelocity = parent->worldRotate * localVelocity + parent->worldVelocity;
	} else {
		worldScale = localScale;
		worldRotate = localRotate;
		worldTranslate = localTranslate;
		worldVelocity = localVelocity;
	}
}
