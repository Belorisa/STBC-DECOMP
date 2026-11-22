// NiStencilProperty.cpp -- the stencil test

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiStencilProperty.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiAccumulator.h"
#include "NiDynamicEffect.h"
#include "renderer/NiImage.h"
#include "NiLight.h"
#include "NiPointLight.h"
#include "NiRawImageData.h"
#include "renderer/NiRender.h"
#include "NiSpotLight.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00808080 _$E227
// FUNCTION: STBC 0x008080a0 _$E17
// FUNCTION: STBC 0x008080c0 _$E15
// FUNCTION: STBC 0x008080d0 _$E23
// FUNCTION: STBC 0x008080f0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00808100 _$E30
// FUNCTION: STBC 0x00808110 _$E41
// FUNCTION: STBC 0x00808120 _$E45
// FUNCTION: STBC 0x00808130 _$E49
// FUNCTION: STBC 0x00808140 _$E53
// FUNCTION: STBC 0x00808180 _$E58

// FUNCTION: STBC 0x00807530 _$E209
// FUNCTION: STBC 0x00807550 _$E206
// DATA: STBC 0x9A2438 ?rtti@NiStencilProperty@@2VNiRTTI@@B
const NiRTTI NiStencilProperty::rtti( "NiStencilProperty", &NiProperty::rtti );

/*
================
NiStencilProperty::Register
================
*/
// FUNCTION: STBC 0x807570 ?Register@NiStencilProperty@@SAHXZ
NiImplementRegister( NiStencilProperty )

// FUNCTION: STBC 0x008076c0 _$E215
// FUNCTION: STBC 0x008076e0 _$E212
// DATA: STBC 0x9A241C
NiPointer<NiStencilProperty> NiStencilProperty::defaultProperty;

/*
===============================================================================

	NiStencilPropertyInit (name invented)

	A file-scope object whose constructor creates the default property and
	whose destructor releases it.

===============================================================================
*/
class NiStencilPropertyInit {
public:
	// FUNCTION: STBC 0x807740 ??0NiStencilPropertyInit@@QAE@XZ
	NiStencilPropertyInit() { NiStencilProperty::defaultProperty = new NiStencilProperty; }
	~NiStencilPropertyInit() { NiStencilProperty::defaultProperty = 0; }
};

// FUNCTION: STBC 0x00807720 _$E220
// FUNCTION: STBC 0x00807800 _$E218
static NiStencilPropertyInit init;

/*
================
NiStencilProperty::CreateClone
================
*/
// FUNCTION: STBC 0x807830
NiObject *NiStencilProperty::CreateClone() {
	NiStencilProperty *clone = new NiStencilProperty;
	CopyMembers( clone );
	return clone;
}

/*
================
NiStencilProperty::CopyMembers
================
*/
// FUNCTION: STBC 0x8078F0
void NiStencilProperty::CopyMembers( NiStencilProperty *dest ) {
	NiProperty::CopyMembers( dest );
	dest->enable = enable;
	dest->function = function;
	dest->reference = reference;
	dest->mask = mask;
	dest->failAction = failAction;
	dest->zFailAction = zFailAction;
	dest->passAction = passAction;
}

/*
================
NiStencilProperty::CreateObject

Creates the property and loads it, with a record for NiObjectNET's link
IDs.
================
*/
// FUNCTION: STBC 0x807930
void NiStencilProperty::CreateObject( NiStream &stream, NiObject *&object ) {
	NiStencilProperty *stencil = new NiStencilProperty;
	stream.SetLoadRecord( new NiStencilPropertyLoadRecord );
	stencil->LoadBinary( stream );
	object = stencil;
}

/*
================
NiStencilProperty::LoadBinary

The function and the actions are saved as ints.
================
*/
// FUNCTION: STBC 0x8079F0
void NiStencilProperty::LoadBinary( NiStream &stream ) {
	NiProperty::LoadBinary( stream );
	NiStreamLoadBinary( stream, enable );
	NiStreamLoadEnum( stream, function );
	NiStreamLoadBinary( stream, reference );
	NiStreamLoadBinary( stream, mask );
	NiStreamLoadEnum( stream, failAction );
	NiStreamLoadEnum( stream, zFailAction );
	NiStreamLoadEnum( stream, passAction );
}

/*
================
NiStencilProperty::LinkObject
================
*/
// FUNCTION: STBC 0x807A90
void NiStencilProperty::LinkObject( NiStream &stream ) {
	NiProperty::LinkObject( stream );
}

/*
================
NiStencilProperty::RegisterStreamables
================
*/
// FUNCTION: STBC 0x807AA0
bool NiStencilProperty::RegisterStreamables( NiStream &stream ) {
	return NiProperty::RegisterStreamables( stream );
}

/*
================
NiStencilProperty::SaveBinary
================
*/
// FUNCTION: STBC 0x807AB0
void NiStencilProperty::SaveBinary( NiStream &stream ) {
	NiProperty::SaveBinary( stream );
	NiStreamSaveBinary( stream, enable );
	NiStreamSaveEnum( stream, function );
	NiStreamSaveBinary( stream, reference );
	NiStreamSaveBinary( stream, mask );
	NiStreamSaveEnum( stream, failAction );
	NiStreamSaveEnum( stream, zFailAction );
	NiStreamSaveEnum( stream, passAction );
}

/*
================
NiStencilProperty::IsEqual
================
*/
// FUNCTION: STBC 0x807B30
bool NiStencilProperty::IsEqual( NiObject *object ) {
	if ( !NiProperty::IsEqual( object ) ) {
		return false;
	}

	NiStencilProperty *stencil = (NiStencilProperty *)object;
	// The original's bug, kept: the last test assigns this pass action to
	// the other property instead of comparing them, so the two count as
	// different whenever the pass action is not ACTION_KEEP.
	if ( stencil->enable != enable
		|| stencil->function != function
		|| stencil->reference != reference
		|| stencil->mask != mask
		|| stencil->failAction != failAction
		|| stencil->zFailAction != zFailAction
		|| ( stencil->passAction = passAction ) ) {
		return false;
	}
	return true;
}

/*
================
NiStencilProperty::GetViewerString

The block is left unwritten for a function out of range.
================
*/
// FUNCTION: STBC 0x807BA0 ?GetViewerString@NiStencilProperty@@KAPADPBDW4TestFunction@1@@Z
char *NiStencilProperty::GetViewerString( const char *prefix, TestFunction function ) {
	char *string = new char[strlen( prefix ) + 22];
	switch ( function ) {
	case TEST_NEVER:
		sprintf( string, "%s = TEST_NEVER", prefix );
		break;
	case TEST_LESS:
		sprintf( string, "%s = TEST_LESS", prefix );
		break;
	case TEST_EQUAL:
		sprintf( string, "%s = TEST_EQUAL", prefix );
		break;
	case TEST_LESSEQUAL:
		sprintf( string, "%s = TEST_LESSEQUAL", prefix );
		break;
	case TEST_GREATER:
		sprintf( string, "%s = TEST_GREATER", prefix );
		break;
	case TEST_NOTEQUAL:
		sprintf( string, "%s = TEST_NOTEQUAL", prefix );
		break;
	case TEST_GREATEREQUAL:
		sprintf( string, "%s = TEST_GREATEREQUAL", prefix );
		break;
	case TEST_ALWAYS:
		sprintf( string, "%s = TEST_ALWAYS", prefix );
		break;
	}
	return string;
}

/*
================
NiStencilProperty::GetViewerString

The block is left unwritten for an action out of range.
================
*/
// FUNCTION: STBC 0x807CA0 ?GetViewerString@NiStencilProperty@@KAPADPBDW4Action@1@@Z
char *NiStencilProperty::GetViewerString( const char *prefix, Action action ) {
	char *string = new char[strlen( prefix ) + 22];
	switch ( action ) {
	case ACTION_KEEP:
		sprintf( string, "%s = ACTION_KEEP", prefix );
		break;
	case ACTION_ZERO:
		sprintf( string, "%s = ACTION_ZERO", prefix );
		break;
	case ACTION_REPLACE:
		sprintf( string, "%s = ACTION_REPLACE", prefix );
		break;
	case ACTION_INCREMENT:
		sprintf( string, "%s = ACTION_INCREMENT", prefix );
		break;
	case ACTION_DECREMENT:
		sprintf( string, "%s = ACTION_DECREMENT", prefix );
		break;
	case ACTION_INVERT:
		sprintf( string, "%s = ACTION_INVERT", prefix );
		break;
	}
	return string;
}

/*
================
NiStencilProperty::GetViewerStrings
================
*/
// FUNCTION: STBC 0x807D70
void NiStencilProperty::GetViewerStrings( NiTArray<char *> *strings ) {
	NiProperty::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bEnable", GetStencilOn() ) );
	strings->Add( NiGetViewerString( "m_uiRef", GetStencilReference() ) );
	strings->Add( NiGetViewerString( "m_uiMask", GetStencilMask() ) );
	strings->Add( GetViewerString( "m_eTest", GetStencilFunction() ) );
	strings->Add( GetViewerString( "m_eFailAct", GetStencilFailAction() ) );
	strings->Add( GetViewerString( "m_eZFailAct", GetStencilPassZFailAction() ) );
	strings->Add( GetViewerString( "m_ePassAct", GetStencilPassAction() ) );
}
