// NiBillboardNode.cpp -- nodes that face the camera

#include "NiRTLib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "NiBillboardNode.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiAutoNormalParticles.h"
#include "NiAutoNormalParticlesData.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiParticles.h"
#include "NiParticlesData.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f7190 _$E30
// FUNCTION: STBC 0x007f71a0 _$E41
// FUNCTION: STBC 0x007f71b0 _$E45
// FUNCTION: STBC 0x007f71c0 _$E49
// FUNCTION: STBC 0x007f71d0 _$E53
// FUNCTION: STBC 0x007f71e0 _$E177
// FUNCTION: STBC 0x007f71f0 _$E182
// FUNCTION: STBC 0x007f7200 _$E186
// FUNCTION: STBC 0x007f7210 _$E190
// FUNCTION: STBC 0x007f7220 _$E194
// FUNCTION: STBC 0x007f7230 _$E119
// FUNCTION: STBC 0x007f7240 _$E132
// FUNCTION: STBC 0x007f7250 _$E199
// FUNCTION: STBC 0x007f7260 _$E203
// FUNCTION: STBC 0x007f7270 _$E210

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f7170 _$E262
// FUNCTION: STBC 0x007f7110 _$E17
// FUNCTION: STBC 0x007f7130 _$E15
// FUNCTION: STBC 0x007f7140 _$E23
// FUNCTION: STBC 0x007f7160 _$E21

// FUNCTION: STBC 0x007f6570 _$E255
// FUNCTION: STBC 0x007f6590 _$E252
// DATA: STBC 0x9A1DC8 ?rtti@NiBillboardNode@@2VNiRTTI@@B
const NiRTTI NiBillboardNode::rtti( "NiBillboardNode", &NiNode::rtti );

/*
================
NiBillboardNode::Register
================
*/
// FUNCTION: STBC 0x7F65B0 ?Register@NiBillboardNode@@SAHXZ
NiImplementRegister( NiBillboardNode )

/*
================
NiBillboardNode::RotateToCamera

Builds the world transform from the parent's with a local rotation that
turns the subtree toward the camera, and updates the children with it.
The subtree's own frame keeps its up axis in y and faces along z.

Gamebryo 1.2.2: NiBillboardNode::RotateToCamera replaces the length with
its reciprocal in ROTATE_ABOUT_UP ("unitize NDz").
================
*/
// FUNCTION: STBC 0x7F6700
void NiBillboardNode::RotateToCamera( NiCamera *camera ) {
	// the parent's world transform
	NiMatrix3 parentRotate;
	NiPoint3 parentTranslate;
	float parentScale;
	NiNode *parentNode = GetParent();
	if ( parentNode ) {
		parentRotate = parentNode->GetWorldRotate();
		parentTranslate = parentNode->GetWorldTranslate();
		parentScale = parentNode->GetWorldScale();
	} else {
		parentRotate = NiMatrix3::IDENTITY;
		parentTranslate = NiPoint3::ZERO;
		parentScale = 1.0f;
	}

	// the billboard's world transform before it turns
	NiMatrix3 rotate = parentRotate * localRotate;
	NiPoint3 translate = parentTranslate + parentScale * ( parentRotate * localTranslate );
	float scale = parentScale * localScale;

	// the turn, in the billboard's frame
	NiMatrix3 faceRotate = NiMatrix3::IDENTITY;
	switch ( GetMode() ) {
	case ALWAYS_FACE_CAMERA: {
		// face the camera, with the up axis as close to the camera's as the
		// facing allows
		NiPoint3 cameraDir = camera->GetWorldDirection() * rotate;
		NiPoint3 cameraUp = camera->GetWorldUpVector() * rotate;
		NiPoint3 cameraRight = camera->GetWorldRightVector() * rotate;
		float length = sqrtf( cameraRight.y * cameraRight.y + cameraUp.y * cameraUp.y );
		if ( length > 1e-06f ) {
			float invLength = 1.0f / length;
			float cosine = invLength * cameraUp.y;
			float sine = -( invLength * cameraRight.y );
			faceRotate.SetCol( 0, cosine * cameraRight.x + sine * cameraUp.x, sine * cameraUp.y + cosine * cameraRight.y, cosine * cameraRight.z + sine * cameraUp.z );
			faceRotate.SetCol( 1, cosine * cameraUp.x - sine * cameraRight.x, cosine * cameraUp.y - sine * cameraRight.y, cosine * cameraUp.z - sine * cameraRight.z );
			faceRotate.SetCol( 2, -cameraDir.x, -cameraDir.y, -cameraDir.z );
		}
		break;
	}
	case ROTATE_ABOUT_UP: {
		// turn about the up axis toward the eye
		NiPoint3 toCamera = ( 1.0f / scale ) * ( ( camera->GetWorldLocation() - translate ) * rotate );
		float length = sqrtf( toCamera.x * toCamera.x + toCamera.z * toCamera.z );
		if ( length >= 1e-12f ) {
			length = 1.0f / length;	// RULE-EXCEPTION: R30, the reciprocal reuses the length (x87 keeps it on the stack)
			toCamera.x *= length;
			toCamera.z *= length;
			faceRotate.SetCol( 0, toCamera.z, 0.0f, -toCamera.x );
			faceRotate.SetCol( 1, 0.0f, 1.0f, 0.0f );
			faceRotate.SetCol( 2, toCamera.x, 0.0f, toCamera.z );
		}
		break;
	}
	case RIGID_FACE_CAMERA: {
		// take the camera's frame
		NiPoint3 cameraDir = camera->GetWorldDirection() * rotate;
		NiPoint3 cameraUp = camera->GetWorldUpVector() * rotate;
		NiPoint3 cameraRight = camera->GetWorldRightVector() * rotate;
		faceRotate.SetCol( 0, cameraRight.x, cameraRight.y, cameraRight.z );
		faceRotate.SetCol( 1, cameraUp.x, cameraUp.y, cameraUp.z );
		faceRotate.SetCol( 2, cameraDir.x, cameraDir.y, cameraDir.z );
		break;
	}
	}

	worldRotate = rotate * faceRotate;
	worldTranslate = translate;
	worldScale = scale;

	for ( unsigned int i = 0; i < children.GetSize(); i++ ) {
		NiAVObject *child = children.GetAt( i );
		if ( child ) {
			child->UpdateDownwardPass( savedTime );
		}
	}
}

/*
================
NiBillboardNode::VirtualSlot35

NiNode's, then the subtree turns to face the camera.
================
*/
// FUNCTION: STBC 0x7F6B90
void NiBillboardNode::VirtualSlot35( NiCamera *camera ) {
	NiNode::VirtualSlot35( camera );
	RotateToCamera( camera );
}

/*
================
NiBillboardNode::UpdateDownwardPass

Keeps the time for the update when drawn.
================
*/
// FUNCTION: STBC 0x7F6BB0
void NiBillboardNode::UpdateDownwardPass( float time ) {
	savedTime = time;
	NiNode::UpdateDownwardPass( time );
	UpdateWorldBound();
}

/*
================
NiBillboardNode::UpdateWorldBound

The children's bound grown into the sphere about the node's own position
that holds it, so that it holds the subtree however it turns.
================
*/
// FUNCTION: STBC 0x7F6BE0
void NiBillboardNode::UpdateWorldBound() {
	if ( children.GetEffectiveSize() ) {
		NiPoint3 offset = worldBound.center - worldTranslate;
		float radius = offset.Length() + worldBound.radius;
		worldBound.center = worldTranslate;
		worldBound.radius = radius;
		worldBound.unknown10 = false;
	}
}

/*
================
NiBillboardNode::CreateClone
================
*/
// FUNCTION: STBC 0x7F6C50
NiObject *NiBillboardNode::CreateClone() {
	NiBillboardNode *clone = new NiBillboardNode;
	CopyMembers( clone );
	return clone;
}

/*
================
NiBillboardNode::CopyMembers

The mode comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7F6CE0
void NiBillboardNode::CopyMembers( NiBillboardNode *dest ) {
	NiNode::CopyMembers( dest );
}

/*
================
NiBillboardNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7F6CF0
void NiBillboardNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBillboardNode *node = new NiBillboardNode;
	stream.SetLoadRecord( new NiBillboardNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiBillboardNode::LoadBinary

The mode comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7F6E80
void NiBillboardNode::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
}

/*
================
NiBillboardNode::LinkObject
================
*/
// FUNCTION: STBC 0x7F6E90
void NiBillboardNode::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
}

/*
================
NiBillboardNode::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F6EA0
bool NiBillboardNode::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiBillboardNode::SaveBinary
================
*/
// FUNCTION: STBC 0x7F6EB0
void NiBillboardNode::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
}

/*
================
NiBillboardNode::IsEqual
================
*/
// FUNCTION: STBC 0x7F6EC0
bool NiBillboardNode::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiBillboardNode *node = (NiBillboardNode *)object;
	if ( GetMode() != node->GetMode() ) {
		return false;
	}
	return true;
}

/*
================
NiBillboardNode::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F6EF0
void NiBillboardNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( GetViewerString( "m_eMode", GetMode() ) );
}

/*
================
NiBillboardNode::GetViewerString

Returns "<prefix> = <mode>" in a new string that the caller deletes; an
unknown mode leaves the string unset.
================
*/
// FUNCTION: STBC 0x7F70A0
char *NiBillboardNode::GetViewerString( const char *prefix, FaceMode mode ) {
	char *string = new char[strlen( prefix ) + 22];
	switch ( mode ) {
	case ALWAYS_FACE_CAMERA:
		sprintf( string, "%s = ALWAYS_FACE_CAMERA", prefix );
		break;
	case ROTATE_ABOUT_UP:
		sprintf( string, "%s = ROTATE_ABOUT_UP", prefix );
		break;
	case RIGID_FACE_CAMERA:
		sprintf( string, "%s = RIGID_FACE_CAMERA", prefix );
		break;
	}
	return string;
}
