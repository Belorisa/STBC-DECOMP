// NiMaterialColorController.cpp -- material colour animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiMaterialColorController.h"
#include "NiObjectNET.h"
#include "NiBezFloatKey.h"
#include "NiBezPosKey.h"
#include "NiBezRotKey.h"
#include "NiBezierMesh.h"
#include "NiBezierPatch.h"
#include "NiColorKey.h"
#include "NiDynamicEffect.h"
#include "NiEulerRotKey.h"
#include "NiKeyType4FloatKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiLinColorKey.h"
#include "NiLinFloatKey.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiMorphKey.h"
#include "NiNode.h"
#include "NiRotKey.h"
#include "NiTCBFloatKey.h"
#include "NiTCBPosKey.h"
#include "NiTCBRotKey.h"
#include "NiTriShapeDynamicData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007abd20 _$E297
// FUNCTION: STBC 0x007abd30 _$E296
// FUNCTION: STBC 0x007abd40 _$E105
// FUNCTION: STBC 0x007abd50 _$E104
// FUNCTION: STBC 0x007abd60 _$E306
// FUNCTION: STBC 0x007abd70 _$E305
// FUNCTION: STBC 0x007abd80 _$E116
// FUNCTION: STBC 0x007abd90 _$E115
// FUNCTION: STBC 0x007abda0 _$E300
// FUNCTION: STBC 0x007abdb0 _$E299
// FUNCTION: STBC 0x007abdc0 _$E119
// FUNCTION: STBC 0x007abdd0 _$E118
// FUNCTION: STBC 0x007abde0 _$E309
// FUNCTION: STBC 0x007abdf0 _$E308
// FUNCTION: STBC 0x007abe00 _$E279
// FUNCTION: STBC 0x007abe10 _$E278
// FUNCTION: STBC 0x007abe20 _$E294
// FUNCTION: STBC 0x007abe30 _$E293
// FUNCTION: STBC 0x007abe60 _$E282
// FUNCTION: STBC 0x007abe70 _$E281
// FUNCTION: STBC 0x007abe80 _$E275
// FUNCTION: STBC 0x007abe90 _$E274
// FUNCTION: STBC 0x007abea0 _$E291
// FUNCTION: STBC 0x007abeb0 _$E290
// FUNCTION: STBC 0x007abec0 _$E285
// FUNCTION: STBC 0x007abed0 _$E284
// FUNCTION: STBC 0x007abee0 _$E303
// FUNCTION: STBC 0x007abef0 _$E302
// FUNCTION: STBC 0x007abf00 _$E288
// FUNCTION: STBC 0x007abf10 _$E287
// FUNCTION: STBC 0x007ac970 _$E30
// FUNCTION: STBC 0x007ac980 _$E29
// FUNCTION: STBC 0x007ac990 _$E72
// FUNCTION: STBC 0x007ac9a0 _$E71
// FUNCTION: STBC 0x007ac9b0 _$E41
// FUNCTION: STBC 0x007ac9c0 _$E40
// FUNCTION: STBC 0x007ac9d0 _$E76
// FUNCTION: STBC 0x007ac9e0 _$E75
// FUNCTION: STBC 0x007ac9f0 _$E80
// FUNCTION: STBC 0x007aca00 _$E79
// FUNCTION: STBC 0x007aca10 _$E86
// FUNCTION: STBC 0x007aca20 _$E85
// FUNCTION: STBC 0x007aca30 _$E60
// FUNCTION: STBC 0x007aca40 _$E59
// FUNCTION: STBC 0x007aca50 _$E64
// FUNCTION: STBC 0x007aca60 _$E63
// FUNCTION: STBC 0x007acad0 _$E90
// FUNCTION: STBC 0x007acae0 _$E89

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007ac8c0 _$E322
// FUNCTION: STBC 0x007ac8e0 _$E321
// FUNCTION: STBC 0x007ac8f0 _$E17
// FUNCTION: STBC 0x007ac900 _$E14
// FUNCTION: STBC 0x007ac910 _$E16
// FUNCTION: STBC 0x007ac920 _$E15
// FUNCTION: STBC 0x007ac930 _$E23
// FUNCTION: STBC 0x007ac940 _$E20
// FUNCTION: STBC 0x007ac950 _$E22
// FUNCTION: STBC 0x007ac960 _$E21

// FUNCTION: STBC 0x007abfc0 _$E315
// FUNCTION: STBC 0x007abfd0 _$E311
// FUNCTION: STBC 0x007abff0 _$E314
// FUNCTION: STBC 0x007ac000 _$E312
// DATA: STBC 0x9A06D0 ?rtti@NiMaterialColorController@@2VNiRTTI@@B
const NiRTTI NiMaterialColorController::rtti( "NiMaterialColorController", &NiTimeController::rtti );

/*
================
NiMaterialColorController::Register
================
*/
// FUNCTION: STBC 0x7AC020 ?Register@NiMaterialColorController@@SAHXZ
NiImplementRegister( NiMaterialColorController )

/*
================
NiMaterialColorController::FindController
================
*/
// FUNCTION: STBC 0x7AC170
NiMaterialColorController *NiMaterialColorController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiMaterialColorController *colorController = NiDynamicCast( NiMaterialColorController, controller );
		if ( colorController ) {
			return colorController;
		}
	}
	return 0;
}

/*
================
NiMaterialColorController::NiMaterialColorController

A new controller animates the ambient colour.
================
*/
// FUNCTION: STBC 0x7AC1C0
NiMaterialColorController::NiMaterialColorController( NiPosData *data_ ) : data( data_ ) {
	SetField( AMB );
	lastIndex = 0;
}

/*
================
NiMaterialColorController::~NiMaterialColorController
================
*/
// FUNCTION: STBC 0x7AC1F0
NiMaterialColorController::~NiMaterialColorController() {
	data = 0;
}

/*
================
NiMaterialColorController::ReplaceAnim
================
*/
// FUNCTION: STBC 0x7AC290
void NiMaterialColorController::ReplaceAnim( unsigned int numKeys, NiPosKey **keys, NiPosKey::KeyType keyType ) {
	if ( !data ) {
		data = new NiPosData;
	}
	data->ReplaceAnim( keys, numKeys, keyType );
	lastIndex = 0;
	loKeyTime = keys[0]->GetTime();
	hiKeyTime = keys[numKeys - 1]->GetTime();
}

/*
================
NiMaterialColorController::Update

Sets the field's colour of the material to the keys' value at the scaled
time, each channel clamped to [0, 1].
================
*/
// FUNCTION: STBC 0x7AC3B0
void NiMaterialColorController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	unsigned int num;	// (name invented; fits the /Od frame order)
	NiPosKey::KeyType type;	// (name invented; fits the /Od frame order)
	NiPosKey **anim = GetAnim( num, type );	// (name invented; fits the /Od frame order)
	float sTime = ComputeScaledTime( time );	// (name invented; fits the /Od frame order)
	NiPoint3 keyValue = NiPosKey::GenInterp( sTime, anim, type, num, lastIndex );	// (name invented; fits the /Od frame order)

	NiColor col( keyValue.x, keyValue.y, keyValue.z );	// (name invented; fits the /Od frame order)
	if ( col.r < 0.0f ) {
		col.r = 0.0f;
	} else if ( col.r > 1.0f ) {
		col.r = 1.0f;
	}
	if ( col.g < 0.0f ) {
		col.g = 0.0f;
	} else if ( col.g > 1.0f ) {
		col.g = 1.0f;
	}
	if ( col.b < 0.0f ) {
		col.b = 0.0f;
	} else if ( col.b > 1.0f ) {
		col.b = 1.0f;
	}

	if ( target ) {
		NiMaterialProperty *material = (NiMaterialProperty *)target;
		switch ( GetField() ) {
		case AMB:
			material->SetAmbientColor( col );
			break;
		case DIFF:
			material->SetDiffuseColor( col );
			break;
		case SPEC:
			material->SetSpecularColor( col );
			break;
		case SELF_ILLUM:
			material->SetEmittance( col );
			break;
		}
	}
}

/*
================
NiMaterialColorController::CreateClone
================
*/
// FUNCTION: STBC 0x7AC570
NiObject *NiMaterialColorController::CreateClone() {
	NiMaterialColorController *clone = new NiMaterialColorController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiMaterialColorController::CopyMembers

The clone gets a clone of the data; the field is not copied.
================
*/
// FUNCTION: STBC 0x7AC5D0
void NiMaterialColorController::CopyMembers( NiMaterialColorController *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->SetPosData( (NiPosData *)data->CreateClone() );
}

/*
================
NiMaterialColorController::CreateObject
================
*/
// FUNCTION: STBC 0x7AC620
void NiMaterialColorController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMaterialColorController *controller = new NiMaterialColorController;
	stream.SetLoadRecord( new NiMaterialColorControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiMaterialColorController::LoadBinary

The data's link ID.
================
*/
// FUNCTION: STBC 0x7AC6B0
void NiMaterialColorController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiMaterialColorControllerLoadRecord *record = (NiMaterialColorControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->dataLinkID );
	assert( record->dataLinkID );
}

/*
================
NiMaterialColorController::LinkObject
================
*/
// FUNCTION: STBC 0x7AC6D0
void NiMaterialColorController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiMaterialColorControllerLoadRecord *record = (NiMaterialColorControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	data = (NiPosData *)stream.GetObjectFromLinkID( record->dataLinkID );
}

/*
================
NiMaterialColorController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7AC720
bool NiMaterialColorController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	data->RegisterStreamables( stream );
	return true;
}

/*
================
NiMaterialColorController::SaveBinary
================
*/
// FUNCTION: STBC 0x7AC750
void NiMaterialColorController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiMaterialColorController::IsEqual
================
*/
// FUNCTION: STBC 0x7AC780
bool NiMaterialColorController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiMaterialColorController *controller = (NiMaterialColorController *)object;
	if ( GetField() != controller->GetField() ) {
		return false;
	}
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}

/*
================
NiMaterialColorController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7AC7C0
void NiMaterialColorController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
