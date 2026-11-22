// NiBSPNode.cpp -- binary space partition nodes

#include "NiRTLib.h"
#include <assert.h>
#include "NiBSPNode.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiBone.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f8b90 _$E30
// FUNCTION: STBC 0x007f8ba0 _$E41
// FUNCTION: STBC 0x007f8bb0 _$E45
// FUNCTION: STBC 0x007f8bc0 _$E49
// FUNCTION: STBC 0x007f8bd0 _$E53
// FUNCTION: STBC 0x007f8be0 _$E177
// FUNCTION: STBC 0x007f8bf0 _$E182
// FUNCTION: STBC 0x007f8c00 _$E186
// FUNCTION: STBC 0x007f8c50 _$E190
// FUNCTION: STBC 0x007f8c60 _$E194
// FUNCTION: STBC 0x007f8c70 _$E119
// FUNCTION: STBC 0x007f8c80 _$E132
// FUNCTION: STBC 0x007f8c90 _$E199
// FUNCTION: STBC 0x007f8ca0 _$E203
// FUNCTION: STBC 0x007f8cb0 _$E210

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f8b70 _$E228
// FUNCTION: STBC 0x007f8b10 _$E17
// FUNCTION: STBC 0x007f8b30 _$E15
// FUNCTION: STBC 0x007f8b40 _$E23
// FUNCTION: STBC 0x007f8b60 _$E21

// FUNCTION: STBC 0x007f7d10 _$E220
// FUNCTION: STBC 0x007f7d30 _$E217
// DATA: STBC 0x9A1E68 ?rtti@NiBSPNode@@2VNiRTTI@@B
const NiRTTI NiBSPNode::rtti( "NiBSPNode", &NiNode::rtti );

/*
================
NiBSPNode::Register
================
*/
// FUNCTION: STBC 0x7F7D50 ?Register@NiBSPNode@@SAHXZ
NiImplementRegister( NiBSPNode )

/*
================
NiBSPNode::UpdateWorldPlane
================
*/
// FUNCTION: STBC 0x7F7EA0
void NiBSPNode::UpdateWorldPlane() {
	NiPoint3 normal = worldRotate * modelPlane.GetNormal();
	float constant = modelPlane.GetConstant() * worldScale + normal.Dot( worldTranslate );
	worldPlane.SetNormal( normal );
	worldPlane.SetConstant( constant );
}

/*
================
NiBSPNode::UpdateWorldData

NiNode's, then the world plane follows.
================
*/
// FUNCTION: STBC 0x7F7F20
void NiBSPNode::UpdateWorldData() {
	NiNode::UpdateWorldData();
	UpdateWorldPlane();
}

/*
================
NiBSPNode::ApplyTransform

NiNode's, with the model plane carried through the transform: the normal
by the inverse rotation, the constant by the moved origin.
================
*/
// FUNCTION: STBC 0x7F7FA0
void NiBSPNode::ApplyTransform( const NiMatrix3 &rotate, const NiPoint3 &translate, bool onLeft ) {
	unsigned int i;
	NiAVObject *avObject;	// (name invented; fits the /Od frame order)

	if ( onLeft ) {
		NiMatrix3 m1 = localRotate.TransposeTimes( rotate * localRotate );	// (name invented; fits the /Od frame order)
		NiPoint3 t1 = ( ( rotate * localTranslate + translate - localTranslate ) * localRotate ) / localScale;	// (name invented; fits the /Od frame order)
		NiMatrix3 m2;	// (name invented; fits the /Od frame order)
		m1.Inverse( m2 );
		modelPlane.SetNormal( modelPlane.GetNormal() * m2 );
		modelPlane.SetConstant( modelPlane.GetConstant() + modelPlane.GetNormal() * t1 );
		for ( i = 0; i < children.GetSize(); i++ ) {
			avObject = children.GetAt( i );
			if ( avObject ) {
				avObject->ApplyTransform( m1, t1, true );
			}
		}
	} else {
		NiMatrix3 inverse;
		rotate.Inverse( inverse );
		modelPlane.SetNormal( modelPlane.GetNormal() * inverse );
		modelPlane.SetConstant( modelPlane.GetConstant() + modelPlane.GetNormal() * translate );
		for ( i = 0; i < children.GetSize(); i++ ) {
			avObject = children.GetAt( i );
			if ( avObject ) {
				avObject->ApplyTransform( rotate, translate, true );
			}
		}
	}
}

/*
================
NiBSPNode::Display

Draws the child on the camera's far side of the plane first; a camera in
the plane decides by its view direction.
================
*/
// FUNCTION: STBC 0x7F8210
void NiBSPNode::Display( NiCamera *camera ) {
	int side = worldPlane.WhichSide( camera->GetWorldLocation() );
	if ( side == NiPlane::NO_SIDE ) {
		side = worldPlane.WhichSide( camera->GetWorldDirection() );
	}

	NiAVObject *child0 = children.GetAt( 0 );	// (name invented; fits the /Od frame order)
	NiAVObject *child1 = children.GetAt( 1 );	// (name invented; fits the /Od frame order)
	if ( side == NiPlane::NEGATIVE_SIDE ) {
		if ( child1 ) {
			child1->CullShow( camera );
		}
		if ( child0 ) {
			child0->CullShow( camera );
		}
	} else {
		if ( child0 ) {
			child0->CullShow( camera );
		}
		if ( child1 ) {
			child1->CullShow( camera );
		}
	}
}

/*
================
NiBSPNode::CreateClone
================
*/
// FUNCTION: STBC 0x7F8380
NiObject *NiBSPNode::CreateClone() {
	NiBSPNode *clone = new NiBSPNode;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiBSPNode::CopyMembers

The world plane follows at the clone's next update.
================
*/
// FUNCTION: STBC 0x7F8560
void NiBSPNode::CopyMembers( NiBSPNode *dest ) {
	NiNode::CopyMembers( dest );
	dest->modelPlane = modelPlane;
}

/*
================
NiBSPNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7F8590
void NiBSPNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBSPNode *node = new NiBSPNode;
	stream.SetLoadRecord( new NiBSPNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiBSPNode::LoadBinary
================
*/
// FUNCTION: STBC 0x7F8730
void NiBSPNode::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
	modelPlane.LoadBinary( stream );
}

/*
================
NiBSPNode::LinkObject
================
*/
// FUNCTION: STBC 0x7F8750
void NiBSPNode::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
}

/*
================
NiBSPNode::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F8760
bool NiBSPNode::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiBSPNode::SaveBinary
================
*/
// FUNCTION: STBC 0x7F8770
void NiBSPNode::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
	modelPlane.SaveBinary( stream );
}

/*
================
NiBSPNode::IsEqual
================
*/
// FUNCTION: STBC 0x7F8790
bool NiBSPNode::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiBSPNode *node = (NiBSPNode *)object;
	if ( modelPlane != node->modelPlane ) {
		return false;
	}
	return true;
}

/*
================
NiBSPNode::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F87D0
void NiBSPNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( modelPlane.GetViewerString( "m_modelPlane" ) );
	strings->Add( worldPlane.GetViewerString( "m_worldPlane" ) );
}
