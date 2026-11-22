// NiBone.cpp -- skeleton nodes

#include "NiRTLib.h"
#include "NiBone.h"
#include "NiCloneExtraData.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiScreenPolygon.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f7c20 _$E30
// FUNCTION: STBC 0x007f7c30 _$E41
// FUNCTION: STBC 0x007f7c40 _$E45
// FUNCTION: STBC 0x007f7c70 _$E49
// FUNCTION: STBC 0x007f7c80 _$E53
// FUNCTION: STBC 0x007f7c90 _$E177
// FUNCTION: STBC 0x007f7ca0 _$E182
// FUNCTION: STBC 0x007f7cb0 _$E186
// FUNCTION: STBC 0x007f7d00 _$E190

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f7c00 _$E224
// FUNCTION: STBC 0x007f7ba0 _$E17
// FUNCTION: STBC 0x007f7bc0 _$E15
// FUNCTION: STBC 0x007f7bd0 _$E23
// FUNCTION: STBC 0x007f7bf0 _$E21

// FUNCTION: STBC 0x007f7280 _$E217
// FUNCTION: STBC 0x007f72a0 _$E214
// DATA: STBC 0x9A1E08 ?rtti@NiBone@@2VNiRTTI@@B
const NiRTTI NiBone::rtti( "NiBone", &NiNode::rtti );

/*
================
NiBone::Register
================
*/
// FUNCTION: STBC 0x7F72C0 ?Register@NiBone@@SAHXZ
NiImplementRegister( NiBone )

/*
================
NiBone::NiBone

A bone starts at the skeleton root's identity transform.
================
*/
// FUNCTION: STBC 0x7F7410
NiBone::NiBone() {
	boneRotate = NiMatrix3::IDENTITY;
	boneTranslate = NiPoint3::ZERO;
	boneScale = 1.0f;
}

/*
================
NiBone::IsSkeletonRoot
================
*/
// FUNCTION: STBC 0x7F7490
int NiBone::IsSkeletonRoot() const {
	if ( parent && parent->GetRTTI() == &rtti ) {
		return 0;
	}
	return 1;
}

/*
================
NiBone::UpdateDownwardPass

NiNode's pass, with the bone transform derived between the object's own
world data and the children: from the parent bone's, or from the identity
at the skeleton root.
The properties and the parent bone are read through their accessors, as
the SDK's debug NetImmerse 3.1 does (AnimOnly.exe 0x43A372).
================
*/
// FUNCTION: STBC 0x7F74B0
void NiBone::UpdateDownwardPass( float time ) {
	NiPropertyList *list = &properties;
	while ( list ) {
		NiProperty *property = list->GetItem();
		if ( property && property->GetControllers() ) {
			property->Update( time );
		}
		list = list->GetNext();
	}

	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Update( time );
	}

	UpdateWorldData();

	if ( IsSkeletonRoot() ) {
		UpdateBoneTransform( NiMatrix3::IDENTITY, NiPoint3::ZERO, 1.0f );
	} else {
		NiBone *parentBone = (NiBone *)GetParent();
		UpdateBoneTransform( parentBone->GetBoneRotate(), parentBone->GetBoneTranslate(), parentBone->GetBoneScale() );
	}

	bool hasBound = false;
	NiBound *bound = &worldBound;
	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->UpdateDownwardPass( time );
			if ( child->VirtualSlot26() ) {
				if ( !hasBound ) {
					hasBound = true;
					*bound = child->GetWorldBound();
				} else {
					bound->Merge( &child->GetWorldBound() );
				}
			}
		}
	}

	SetVisual( hasBound );
}

/*
================
NiBone::UpdateBoneTransform

The local transform composed with the parent bone's.
================
*/
// FUNCTION: STBC 0x7F7700
void NiBone::UpdateBoneTransform( const NiMatrix3 &parentRotate, const NiPoint3 &parentTranslate, float parentScale ) {
	boneRotate = parentRotate * GetRotate();
	boneTranslate = ( parentRotate * GetTranslate() ) * parentScale + parentTranslate;
	boneScale = parentScale * GetScale();
}

/*
================
NiBone::CreateClone

Marks the original with its clone unless it already carries the mark.
================
*/
// FUNCTION: STBC 0x7F77A0
NiObject *NiBone::CreateClone() {
	NiBone *result = new NiBone;	// (name invented; fits the /Od frame order)
	assert( result );
	CopyMembers( result );
	NiExtraData *extraData = GetExtraData();
	if ( !extraData || !NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
		extraData = new NiCloneExtraData( result );
		assert( extraData );
		AddExtraData( extraData );
	}
	return result;
}

/*
================
NiBone::CopyMembers
================
*/
// FUNCTION: STBC 0x7F7900
void NiBone::CopyMembers( NiBone *dest ) {
	NiNode::CopyMembers( dest );
	dest->boneRotate = boneRotate;
	dest->boneTranslate = boneTranslate;
	dest->boneScale = boneScale;
}

/*
================
NiBone::VirtualSlot03

Also drops the NiCloneExtraData that cloning left at the head of the
extra data list.
================
*/
// FUNCTION: STBC 0x7F7960
void NiBone::VirtualSlot03() {
	NiNode::VirtualSlot03();
	NiExtraData *extraData = GetExtraData();
	if ( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
		assert( extraData->GetRefCount() == 1 );
		RemoveExtraData( extraData );
	}
}

/*
================
NiBone::CreateObject

Creates the bone and loads it.
================
*/
// FUNCTION: STBC 0x7F7990
void NiBone::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBone *bone = new NiBone;
	stream.SetLoadRecord( new NiBoneLoadRecord );
	bone->LoadBinary( stream );
	object = bone;
}

/*
================
NiBone::LoadBinary

The bone transform is not streamed: the first update derives it.
================
*/
// FUNCTION: STBC 0x7F7B50
void NiBone::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
}

/*
================
NiBone::LinkObject
================
*/
// FUNCTION: STBC 0x7F7B60
void NiBone::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
}

/*
================
NiBone::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F7B70
bool NiBone::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiBone::SaveBinary
================
*/
// FUNCTION: STBC 0x7F7B80
void NiBone::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
}

/*
================
NiBone::IsEqual

The bone transform is derived, so it takes no part.
================
*/
// FUNCTION: STBC 0x7F7B90
bool NiBone::IsEqual( NiObject *object ) {
	return NiNode::IsEqual( object );
}
