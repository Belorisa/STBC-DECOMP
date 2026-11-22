// NiKeyframeController.cpp -- transform animation

#include "NiRTLib.h"
#include <assert.h>
#include <float.h>

#include "NiKeyframeController.h"
#include "NiAVObject.h"
#include "NiNode.h"
#include "NiEulerRotKey.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00793390 _$E69
// FUNCTION: STBC 0x007933a0 _$E68
// FUNCTION: STBC 0x00793640 _$E80
// FUNCTION: STBC 0x00793650 _$E79
// FUNCTION: STBC 0x00793660 _$E230
// FUNCTION: STBC 0x00793670 _$E229
// FUNCTION: STBC 0x00793680 _$E30
// FUNCTION: STBC 0x00793690 _$E29
// FUNCTION: STBC 0x007936a0 _$E92
// FUNCTION: STBC 0x007936b0 _$E91
// FUNCTION: STBC 0x007936c0 _$E41
// FUNCTION: STBC 0x007936d0 _$E40
// FUNCTION: STBC 0x007936e0 _$E96
// FUNCTION: STBC 0x007936f0 _$E95
// FUNCTION: STBC 0x00793700 _$E100
// FUNCTION: STBC 0x00793710 _$E99
// FUNCTION: STBC 0x00793720 _$E217
// FUNCTION: STBC 0x00793730 _$E216
// FUNCTION: STBC 0x00793740 _$E222
// FUNCTION: STBC 0x00793750 _$E221
// FUNCTION: STBC 0x00793760 _$E226
// FUNCTION: STBC 0x00793770 _$E225
// FUNCTION: STBC 0x00793780 _$E60
// FUNCTION: STBC 0x00793790 _$E59
// FUNCTION: STBC 0x007937a0 _$E84
// FUNCTION: STBC 0x007937b0 _$E83
// FUNCTION: STBC 0x00793820 _$E88
// FUNCTION: STBC 0x00793830 _$E87

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00793610 _$E243
// FUNCTION: STBC 0x00793630 _$E242
// FUNCTION: STBC 0x00793590 _$E17
// FUNCTION: STBC 0x007935a0 _$E14
// FUNCTION: STBC 0x007935b0 _$E16
// FUNCTION: STBC 0x007935c0 _$E15
// FUNCTION: STBC 0x007935d0 _$E23
// FUNCTION: STBC 0x007935e0 _$E20
// FUNCTION: STBC 0x007935f0 _$E22
// FUNCTION: STBC 0x00793600 _$E21

// FUNCTION: STBC 0x00792ae0 _$E236
// FUNCTION: STBC 0x00792af0 _$E232
// FUNCTION: STBC 0x00792b10 _$E235
// FUNCTION: STBC 0x00792b20 _$E233
// DATA: STBC 0x9A0058 ?rtti@NiKeyframeController@@2VNiRTTI@@B
const NiRTTI NiKeyframeController::rtti( "NiKeyframeController", &NiTimeController::rtti );

/*
================
NiKeyframeController::Register
================
*/
// FUNCTION: STBC 0x792B40 ?Register@NiKeyframeController@@SAHXZ
NiImplementRegister( NiKeyframeController )

/*
================
NiKeyframeController::FindKeyframeController

The first keyframe controller of the object's list, or NULL.
================
*/
// FUNCTION: STBC 0x792C90
NiKeyframeController *NiKeyframeController::FindKeyframeController( NiObjectNET *object ) {
	for ( NiTimeController *controller = object->GetControllers(); controller; controller = controller->GetNext() ) {
		NiKeyframeController *keyframeController = NiDynamicCast( NiKeyframeController, controller );
		if ( keyframeController ) {
			return keyframeController;
		}
	}
	return 0;
}

/*
================
NiKeyframeController::NiKeyframeController
================
*/
// FUNCTION: STBC 0x792CE0
NiKeyframeController::NiKeyframeController( NiKeyframeData *data_ ) : data( data_ ) {
	lastRotIndex = 0;
	lastPosIndex = 0;
	lastScaleIndex = 0;
}

/*
================
NiKeyframeController::~NiKeyframeController
================
*/
// FUNCTION: STBC 0x792D10
NiKeyframeController::~NiKeyframeController() {
	data = 0;
}

/*
================
NiKeyframeController::ReplaceRotAnim
================
*/
// FUNCTION: STBC 0x792DB0
void NiKeyframeController::ReplaceRotAnim( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType, bool resetTimes ) {
	if ( !data ) {
		data = new NiKeyframeData;
	}
	data->ReplaceRotAnim( keys, numKeys, keyType );
	lastRotIndex = 0;
	if ( resetTimes ) {
		ResetTimeExtrema();
	}
}

/*
================
NiKeyframeController::ReplacePosAnim
================
*/
// FUNCTION: STBC 0x792E70
void NiKeyframeController::ReplacePosAnim( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType, bool resetTimes ) {
	if ( !data ) {
		data = new NiKeyframeData;
	}
	data->ReplacePosAnim( keys, numKeys, keyType );
	lastPosIndex = 0;
	if ( resetTimes ) {
		ResetTimeExtrema();
	}
}

/*
================
NiKeyframeController::ReplaceScaleAnim
================
*/
// FUNCTION: STBC 0x792F30
void NiKeyframeController::ReplaceScaleAnim( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType, bool resetTimes ) {
	if ( !data ) {
		data = new NiKeyframeData;
	}
	data->ReplaceScaleAnim( keys, numKeys, keyType );
	lastScaleIndex = 0;
	if ( resetTimes ) {
		ResetTimeExtrema();
	}
}

/*
================
NiKeyframeController::ResetTimeExtrema

Each group's first and last keys widen the times, from an empty range; the
Euler rotation keys are three groups of float keys in the first key.
================
*/
// FUNCTION: STBC 0x792FF0
void NiKeyframeController::ResetTimeExtrema() {
	loKeyTime = FLT_MAX;
	hiKeyTime = -FLT_MAX;

	unsigned int numKeys;

	NiRotKey::KeyType rotKeyType;
	NiRotKey **rotKeys = GetRotAnim( numKeys, rotKeyType );
	if ( rotKeys ) {
		if ( rotKeyType == NiRotKey::EULERKEY ) {
			NiEulerRotKey *euler = (NiEulerRotKey *)rotKeys[0];
			for ( int i = 0; i < 3; i++ ) {
				unsigned int numAxisKeys = euler->GetNumKeys( i );
				if ( numAxisKeys > 0 ) {
					NiFloatKey **axisKeys = euler->GetKeys( i );
					if ( axisKeys[0]->GetTime() < loKeyTime ) {
						loKeyTime = axisKeys[0]->GetTime();
					}
					if ( axisKeys[numAxisKeys - 1]->GetTime() > hiKeyTime ) {
						hiKeyTime = axisKeys[numAxisKeys - 1]->GetTime();
					}
				}
			}
		} else {
			if ( rotKeys[0]->GetTime() < loKeyTime ) {
				loKeyTime = rotKeys[0]->GetTime();
			}
			if ( rotKeys[numKeys - 1]->GetTime() > hiKeyTime ) {
				hiKeyTime = rotKeys[numKeys - 1]->GetTime();
			}
		}
	}

	NiPosKey::KeyType posKeyType;
	NiPosKey **posKeys = GetPosAnim( numKeys, posKeyType );
	if ( posKeys ) {
		if ( posKeys[0]->GetTime() < loKeyTime ) {
			loKeyTime = posKeys[0]->GetTime();
		}
		if ( posKeys[numKeys - 1]->GetTime() > hiKeyTime ) {
			hiKeyTime = posKeys[numKeys - 1]->GetTime();
		}
	}

	NiFloatKey::KeyType scaleKeyType;
	NiFloatKey **scaleKeys = GetScaleAnim( numKeys, scaleKeyType );
	if ( scaleKeys ) {
		if ( scaleKeys[0]->GetTime() < loKeyTime ) {
			loKeyTime = scaleKeys[0]->GetTime();
		}
		if ( scaleKeys[numKeys - 1]->GetTime() > hiKeyTime ) {
			hiKeyTime = scaleKeys[numKeys - 1]->GetTime();
		}
	}
}

/*
================
NiKeyframeController::Update

Sets the target's local rotation, translation and scale to the keys' values
at the scaled time; a scale that is not positive is flipped.
================
*/
// FUNCTION: STBC 0x793110
void NiKeyframeController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	float scaledTime = ComputeScaledTime( time );
	unsigned int numKeys;

	NiRotKey::KeyType rotKeyType;
	NiRotKey **rotKeys = GetRotAnim( numKeys, rotKeyType );
	if ( rotKeys ) {
		NiQuaternion quat = NiRotKey::GenInterp( scaledTime, rotKeys, rotKeyType, numKeys, lastRotIndex );
		( (NiAVObject *)target )->SetRotate( quat );
	}

	NiPosKey::KeyType posKeyType;
	NiPosKey **posKeys = GetPosAnim( numKeys, posKeyType );
	if ( posKeys ) {
		NiPoint3 pos = NiPosKey::GenInterp( scaledTime, posKeys, posKeyType, numKeys, lastPosIndex );
		( (NiAVObject *)target )->SetTranslate( pos );
	}

	NiFloatKey::KeyType scaleKeyType;
	NiFloatKey **scaleKeys = GetScaleAnim( numKeys, scaleKeyType );
	if ( scaleKeys ) {
		float scale = NiFloatKey::GenInterp( scaledTime, scaleKeys, scaleKeyType, numKeys, lastScaleIndex );
		NiAVObject *avObject = (NiAVObject *)target;
		avObject->SetScale( scale > 0.0f ? scale : -scale );
	}
}

/*
================
NiKeyframeController::CreateClone
================
*/
// FUNCTION: STBC 0x793210
NiObject *NiKeyframeController::CreateClone() {
	NiKeyframeController *clone = new NiKeyframeController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiKeyframeController::CopyMembers

The clone shares the data.
================
*/
// FUNCTION: STBC 0x793270
void NiKeyframeController::CopyMembers( NiKeyframeController *dest ) {
	NiTimeController::CopyMembers( dest );
	if ( data ) {
		dest->SetData( data );
	}
}

/*
================
NiKeyframeController::CreateObject
================
*/
// FUNCTION: STBC 0x7932E0
void NiKeyframeController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiKeyframeController *controller = new NiKeyframeController;
	stream.SetLoadRecord( new NiKeyframeControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiKeyframeController::LoadBinary

The data's link ID.
================
*/
// FUNCTION: STBC 0x793370
void NiKeyframeController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiKeyframeControllerLoadRecord *record = (NiKeyframeControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->dataLinkID );
}

/*
================
NiKeyframeController::LinkObject
================
*/
// FUNCTION: STBC 0x7933B0
void NiKeyframeController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiKeyframeControllerLoadRecord *record = (NiKeyframeControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	data = (NiKeyframeData *)stream.GetObjectFromLinkID( record->dataLinkID );
}

/*
================
NiKeyframeController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x793400
bool NiKeyframeController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( data ) {
		data->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiKeyframeController::SaveBinary
================
*/
// FUNCTION: STBC 0x793430
void NiKeyframeController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiKeyframeController::IsEqual
================
*/
// FUNCTION: STBC 0x793460
bool NiKeyframeController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiKeyframeController *controller = (NiKeyframeController *)object;
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}

/*
================
NiKeyframeController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x793490
void NiKeyframeController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiKeyframeController::TargetIsRequiredType

The target must be an NiAVObject. The test is NetImmerse 3.1's
NiIsKindOf( NiAVObject, target ), a null test around the member IsKindOf
(AnimOnly.exe 0x4784A0).
================
*/
// FUNCTION: STBC 0x793840
bool NiKeyframeController::TargetIsRequiredType() const {
	return NiIsKindOf( NiAVObject, target );
}
