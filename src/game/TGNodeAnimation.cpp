/*
===============================================================================

	TGNodeAnimation.cpp

===============================================================================
*/

#include <iostream>
#include <new>
#include <string.h>

#include "TGNodeAnimation.h"
#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiExtraData.h"
#include "renderer/NiImage.h"
#include "NiKeyframeController.h"
#include "NiNode.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiRawImageData.h"
#include "NiTimeController.h"
#include "NiVisController.h"

// The locale-id pair and the standard streams' initialisers and exit-time
// destructors (<iostream>): retail lays them out after the unit's code.
// FUNCTION: STBC 0x006ccce0 _$E256
// FUNCTION: STBC 0x006ccd00 _$E255
// FUNCTION: STBC 0x006ccd10 _$E32
// FUNCTION: STBC 0x006ccd20 _$E29
// FUNCTION: STBC 0x006ccd30 _$E31
// FUNCTION: STBC 0x006ccd40 _$E30
// FUNCTION: STBC 0x006ccd50 _$E38
// FUNCTION: STBC 0x006ccd60 _$E35
// FUNCTION: STBC 0x006ccd70 _$E37
// FUNCTION: STBC 0x006ccd80 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006cc940 _$E237
// FUNCTION: STBC 0x006cc950 _$E236
// FUNCTION: STBC 0x006ccd90 _$E52
// FUNCTION: STBC 0x006ccda0 _$E51
// FUNCTION: STBC 0x006ccdb0 _$E93
// FUNCTION: STBC 0x006ccdc0 _$E92
// FUNCTION: STBC 0x006ccdd0 _$E73
// FUNCTION: STBC 0x006ccde0 _$E72
// FUNCTION: STBC 0x006ccdf0 _$E82
// FUNCTION: STBC 0x006cce00 _$E81
// FUNCTION: STBC 0x006cce10 _$E97
// FUNCTION: STBC 0x006cce20 _$E96
// FUNCTION: STBC 0x006cce30 _$E101
// FUNCTION: STBC 0x006cce40 _$E100

/*
================
TGNodeAnimation::TGNodeAnimation

No name and no keys; the identity transform.
================
*/
// FUNCTION: STBC 0x006cca00 ??0TGNodeAnimation@@QAE@XZ
TGNodeAnimation::TGNodeAnimation( void ) {
	name = NULL;
	keyframeData = NULL;	// BUG: redundant: the members start NULL
	visData = NULL;			// BUG: redundant: the members start NULL
	scale = 1.0f;
	rotate = NiMatrix3::IDENTITY;
	translate = NiPoint3::ZERO;
	appCulled = true;
}

/*
================
TGNodeAnimation::TGNodeAnimation

Copies the name and shares the keyframe keys; the visibility keys and the
transform are not copied. Nothing calls it.
================
*/
// FUNCTION: STBC 0x006ccac0 ??0TGNodeAnimation@@QAE@ABV0@@Z
TGNodeAnimation::TGNodeAnimation( const TGNodeAnimation &other ) {
	if ( name != NULL ) {	// BUG: name is not set yet
		delete name;
		name = NULL;
	}
	keyframeData = other.keyframeData;
	if ( other.name != NULL ) {
		name = new char[strlen( other.name ) + 1];
		strcpy( name, other.name );
	}
}

/*
================
TGNodeAnimation::~TGNodeAnimation
================
*/
// FUNCTION: STBC 0x006ccba0 ??1TGNodeAnimation@@QAE@XZ
TGNodeAnimation::~TGNodeAnimation( void ) {
	if ( name != NULL ) {
		delete name;
	}
	keyframeData = NULL;
}

/*
================
TGNodeAnimation::SetName
================
*/
// FUNCTION: STBC 0x006ccc60 ?SetName@TGNodeAnimation@@QAEXPBD@Z
void TGNodeAnimation::SetName( const char *name_ ) {
	if ( name != NULL ) {
		delete name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}
