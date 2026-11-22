// NiAVObject.cpp -- the base of the scene graph

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiAVObject.h"
#include "NiCamera.h"
#include "NiCollisionRecord.h"
#include "NiNode.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007ddee0 _$E219
// FUNCTION: STBC 0x007ddd30 _$E17
// FUNCTION: STBC 0x007ddd50 _$E15
// FUNCTION: STBC 0x007ddd60 _$E23
// FUNCTION: STBC 0x007ddd80 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007dddb0 _$E30
// FUNCTION: STBC 0x007dddc0 _$E41
// FUNCTION: STBC 0x007dddd0 _$E45
// FUNCTION: STBC 0x007ddde0 _$E49
// FUNCTION: STBC 0x007ddf00 _$E53
// FUNCTION: STBC 0x007ddfc0 _$E177
// FUNCTION: STBC 0x007ddfd0 _$E181
// FUNCTION: STBC 0x007ddfe0 _$E119
// FUNCTION: STBC 0x007ddff0 _$E132
// FUNCTION: STBC 0x007de010 _$E191
// FUNCTION: STBC 0x007de020 _$E195
// FUNCTION: STBC 0x007de030 _$E199
// FUNCTION: STBC 0x007de040 _$E206
// FUNCTION: STBC 0x007de000 _$E186

// FUNCTION: STBC 0x007dbf30 _$E212
// FUNCTION: STBC 0x007dbf50 _$E209
// DATA: STBC 0x9A1578 ?rtti@NiAVObject@@2VNiRTTI@@B
const NiRTTI NiAVObject::rtti( "NiAVObject", &NiObjectNET::rtti );

/*
================
NiAVObject::Register
================
*/
// FUNCTION: STBC 0x7DBF70 ?Register@NiAVObject@@SAHXZ
NiImplementRegister( NiAVObject )

/*
================
NiAVObject::NiAVObject
================
*/
// FUNCTION: STBC 0x7DC0C0
NiAVObject::NiAVObject() {
	flags = 0;
	parent = 0;
	SetAppCulled( false );
	localTranslate = NiPoint3::ZERO;
	localRotate = NiMatrix3::IDENTITY;
	localScale = 1.0f;
	localVelocity = NiPoint3::ZERO;
	worldTranslate = NiPoint3::ZERO;
	worldRotate = NiMatrix3::IDENTITY;
	worldScale = 1.0f;
	worldVelocity = NiPoint3::ZERO;
	SetPropagationMode( PROPAGATE_USE_OBBS );
	collideCallback = 0;
	collideCallbackData = 0;
	modelABV = 0;
	worldABV = 0;
}

/*
================
NiAVObject::~NiAVObject
================
*/
// FUNCTION: STBC 0x7DC210
NiAVObject::~NiAVObject() {
	properties.RemoveAll();
	delete modelABV;
	delete worldABV;
}

/*
================
NiAVObject::AttachParent

Leaves the old parent's children first.
================
*/
// FUNCTION: STBC 0x7DC330
void NiAVObject::AttachParent( NiNode *parent_ ) {
	if ( parent ) {
		parent->DetachChild( this );
	}
	parent = parent_;
}

/*
================
NiAVObject::GetProperty

The attached property of the given type, or NULL.
================
*/
// FUNCTION: STBC 0x7DC390
NiPointer<NiProperty> NiAVObject::GetProperty( int type ) {
	if ( type >= 0 && type < NiProperty::MAX_TYPES ) {
		const NiPropertyList *node = &properties;
		while ( node ) {
			NiPointer<NiProperty> property = node->GetItem();
			if ( property && property->Type() == type ) {
				return property;
			}
			node = node->GetNext();
		}
	}
	return 0;
}

/*
================
NiAVObject::RemoveProperty

Detaches the property of the given type and returns it, or NULL.
================
*/
// FUNCTION: STBC 0x7DC490
NiPointer<NiProperty> NiAVObject::RemoveProperty( int type ) {
	if ( type >= 0 && type < NiProperty::MAX_TYPES ) {
		NiPropertyList *node = &properties;
		while ( node ) {
			NiPointer<NiProperty> property = node->GetItem();
			if ( property && property->Type() == type ) {
				properties.Remove( property );
				return property;
			}
			node = node->GetNext();
		}
	}
	return 0;
}

/*
================
NiAVObject::Update

Updates the subtree below the object, then the bounds of its ancestors.
================
*/
// FUNCTION: STBC 0x7DC5C0
void NiAVObject::Update( float time ) {
	UpdateDownwardPass( time );
	if ( parent ) {
		parent->UpdateUpwardPass();
	}
}

/*
================
NiAVObject::UpdateDownwardPass

Runs the controllers of the properties and of the object, then derives the
world transform and bound.
================
*/
// FUNCTION: STBC 0x7DC5F0
void NiAVObject::UpdateDownwardPass( float time ) {
	for ( NiPropertyList *node = &properties; node; node = node->GetNext() ) {
		NiProperty *property = node->GetItem();
		if ( property && property->GetControllers() ) {
			property->Update( time );
		}
	}

	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		controller->Update( time );
	}

	UpdateWorldData();
	UpdateWorldBound();
}

/*
================
NiAVObject::UpdateProperties

Rebuilds the property states of the subtree from the parent's, or from
the defaults at a root.
================
*/
// FUNCTION: STBC 0x7DC690
void NiAVObject::UpdateProperties() {
	NiPointer<NiPropertyState> parentState;
	if ( parent ) {
		parentState = parent->UpdatePropertiesUpward();
	} else {
		parentState = new NiPropertyState;
	}
	UpdatePropertiesDownward( parentState );
}

/*
================
NiAVObject::GetPropertyState
================
*/
// FUNCTION: STBC 0x7DC780
NiPointer<NiPropertyState> NiAVObject::GetPropertyState() {
	return 0;
}

/*
================
NiAVObject::UpdatePropertiesDownward
================
*/
// FUNCTION: STBC 0x7DC7A0
void NiAVObject::UpdatePropertiesDownward( NiPropertyState *parentState ) {
}

/*
================
NiAVObject::PushLocalProperties

The parent state with this object's properties put in their slots. An
object without properties passes the parent state on; otherwise the state
is a copy when the caller asked for one.
================
*/
// FUNCTION: STBC 0x7DC7B0
NiPointer<NiPropertyState> NiAVObject::PushLocalProperties( NiPropertyState *parentState, bool copyOnChange ) {
	if ( GetPropertyList().IsEmpty() ) {
		return parentState;
	}

	NiPointer<NiPropertyState> state;
	if ( copyOnChange ) {
		state = new NiPropertyState( *parentState );
	} else {
		state = parentState;
	}

	NiPropertyList *node = &properties;
	while ( node && node->GetItem() ) {
		state->SetProperty( node->GetItem() );
		node = node->GetNext();
	}
	return state;
}

/*
================
NiAVObject::UpdateEffects

Rebuilds the effect states of the subtree from the parent's; the shared
last state lives only for the pass.

Retail releases the last state out of line (0x4E1580) at the first clear
and inline at the second; this build inlines both: not exact yet.
================
*/
// FUNCTION: STBC 0x7DC950
void NiAVObject::UpdateEffects() {
	NiDynamicEffectState::SetCachedState( 0 );
	NiPointer<NiDynamicEffectState> parentState;
	if ( parent ) {
		parentState = parent->UpdateEffectsUpward();
	}
	UpdateEffectsDownward( parentState );
	NiDynamicEffectState::SetCachedState( 0 );
}

/*
================
NiAVObject::GetEffectState
================
*/
// FUNCTION: STBC 0x7DCA40
NiPointer<NiDynamicEffectState> NiAVObject::GetEffectState() {
	return 0;
}

/*
================
NiAVObject::UpdateEffectsDownward
================
*/
// FUNCTION: STBC 0x7DCA60
void NiAVObject::UpdateEffectsDownward( NiDynamicEffectState *parentState ) {
}

/*
================
NiAVObject::Unknown7DCA70

The culling half of CullShow: true when the world bound lies wholly behind
a camera plane in use; the planes it lies wholly in front of are disabled
and set in the mask.
================
*/
// FUNCTION: STBC 0x7DCA70
bool NiAVObject::Unknown7DCA70( NiCamera *camera, NiBitVector &disabled ) {
	int count = camera->GetPlaneCount();
	NiTArray<NiPlane *> &planes = camera->GetPlaneArray();
	NiBitVector &disabledPlanes = camera->GetDisabledPlanes();
	for ( int i = count - 1; i >= 0; i-- ) {
		if ( !disabledPlanes.IsSet( i ) ) {
			NiPlane::Side side = worldBound.WhichSide( *planes.GetAt( i ) );
			// RULE-EXCEPTION: R35, the unbraced return keeps the function under /Ob2's
			// 175-tuple automatic inline limit (175 braced), as CullShow's expansion needs
			if ( side == NiPlane::NEGATIVE_SIDE )
				return true;
			if ( side == NiPlane::POSITIVE_SIDE ) {
				disabledPlanes.Set( i );
				disabled.Set( i );
			}
		}
	}
	return false;
}

/*
================
NiAVObject::Unknown7DCB00

The restoring half of CullShow: enables again the planes of the mask.
================
*/
// FUNCTION: STBC 0x7DCB00
void NiAVObject::Unknown7DCB00( NiCamera *camera, NiBitVector &disabled ) {
	int count = camera->GetPlaneCount();
	NiBitVector &disabledPlanes = camera->GetDisabledPlanes();
	for ( int i = 0; i < count; i++ ) {
		if ( disabled.IsSet( i ) ) {
			disabledPlanes.Clear( i );
		}
	}
}

/*
================
NiAVObject::VirtualSlot35

Passes the call on to every controller.
================
*/
// FUNCTION: STBC 0x7DCB50
void NiAVObject::VirtualSlot35( NiCamera *camera ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		controller->VirtualSlot15();
	}
}

/*
================
NiAVObject::VirtualSlot36
================
*/
// FUNCTION: STBC 0x7DCB70
void NiAVObject::VirtualSlot36( NiCamera *camera ) {
}

/*
================
NiAVObject::CullShow

Gamebryo 1.2.2's name; its body there tests plane flags instead. Each
culling plane still in use that the whole world bound lies behind
culls the object; each plane the bound lies wholly in front of is taken
out of use for the subtree and restored afterwards. Both halves are the
functions above, expanded here by /Ob2.
================
*/
// FUNCTION: STBC 0x7DCB80
void NiAVObject::CullShow( NiCamera *camera ) {
	if ( GetAppCulled() ) {
		return;
	}

	NiBitVector disabled;
	bool culled = Unknown7DCA70( camera, disabled );
	if ( !culled ) {
		VirtualSlot35( camera );
		Display( camera );
		VirtualSlot36( camera );
	}
	Unknown7DCB00( camera, disabled );
}

/*
================
NiAVObject::Unknown7DCC80
================
*/
// FUNCTION: STBC 0x7DCC80
unsigned int NiAVObject::Unknown7DCC80( NiRender *renderer ) {
	if ( !renderer ) {
		return 0x800000;
	}
	return renderer->VirtualSlot15( GetPropertyState(), GetEffectState() );
}

/*
================
NiAVObject::SetModelABV

Takes ownership of the volume and replaces the world volume with a new
volume of the same type.
================
*/
// FUNCTION: STBC 0x7DCD60
void NiAVObject::SetModelABV( NiBoundingVolume *volume ) {
	delete modelABV;
	modelABV = volume;
	delete worldABV;
	worldABV = modelABV->Create();
}

/*
================
NiAVObject::TestBoundIntersect

An object without a model volume stands in with a volume around its world
bound.
================
*/
// FUNCTION: STBC 0x7DCDB0
bool NiAVObject::TestBoundIntersect( float deltaTime, NiAVObject *other ) {
	if ( !modelABV ) {
		if ( !other->modelABV ) {
			return NiBound::TestIntersect( deltaTime, worldBound, worldVelocity, other->worldBound, other->worldVelocity );
		}
		other->worldABV->UpdateWorldData( *other->modelABV, other->worldRotate, other->worldTranslate, other->worldScale );
		// the volume around the bound is never deleted
		return NiBoundingVolume::TestIntersect( deltaTime, *NiBoundingVolume::CreateFromBound( worldBound ), worldVelocity,
												*other->worldABV, other->worldVelocity );
	}

	if ( other->modelABV ) {
		worldABV->UpdateWorldData( *modelABV, worldRotate, worldTranslate, worldScale );
		other->worldABV->UpdateWorldData( *other->modelABV, other->worldRotate, other->worldTranslate, other->worldScale );
		return NiBoundingVolume::TestIntersect( deltaTime, *worldABV, worldVelocity, *other->worldABV, other->worldVelocity );
	}
	worldABV->UpdateWorldData( *modelABV, worldRotate, worldTranslate, worldScale );
	// the volume around the bound is never deleted
	return NiBoundingVolume::TestIntersect( deltaTime, *worldABV, worldVelocity,
											*NiBoundingVolume::CreateFromBound( other->worldBound ), other->worldVelocity );
}

/*
================
NiAVObject::FindBoundIntersect
================
*/
// FUNCTION: STBC 0x7DCF00
bool NiAVObject::FindBoundIntersect( float deltaTime, NiAVObject *other, float &time, NiPoint3 &point,
									 bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 ) {
	if ( !modelABV ) {
		if ( !other->modelABV ) {
			return NiBound::FindIntersect( deltaTime, worldBound, worldVelocity, other->worldBound, other->worldVelocity,
										   time, point, calcNormals, normal0, normal1 );
		}
		other->worldABV->UpdateWorldData( *other->modelABV, other->worldRotate, other->worldTranslate, other->worldScale );
		// the volume around the bound is never deleted
		return NiBoundingVolume::FindIntersect( deltaTime, *NiBoundingVolume::CreateFromBound( worldBound ), worldVelocity,
												*other->worldABV, other->worldVelocity, time, point, calcNormals, normal0, normal1 );
	}

	if ( other->modelABV ) {
		worldABV->UpdateWorldData( *modelABV, worldRotate, worldTranslate, worldScale );
		other->worldABV->UpdateWorldData( *other->modelABV, other->worldRotate, other->worldTranslate, other->worldScale );
		return NiBoundingVolume::FindIntersect( deltaTime, *worldABV, worldVelocity, *other->worldABV, other->worldVelocity,
												time, point, calcNormals, normal0, normal1 );
	}
	worldABV->UpdateWorldData( *modelABV, worldRotate, worldTranslate, worldScale );
	// the volume around the bound is never deleted
	return NiBoundingVolume::FindIntersect( deltaTime, *worldABV, worldVelocity,
											*NiBoundingVolume::CreateFromBound( other->worldBound ), other->worldVelocity,
											time, point, calcNormals, normal0, normal1 );
}

/*
================
NiAVObject::ReportCollision

Each root's callback sees the collision from its own side. The swapped
normals are not swapped back.
================
*/
// FUNCTION: STBC 0x7DD0C0
int NiAVObject::ReportCollision( NiCollisionIntersect &intersect ) {
	int result0 = 0;
	int result1 = 0;

	if ( intersect.root0 && intersect.root0->GetCollideCallback() ) {
		result0 = intersect.root0->GetCollideCallback()( intersect );
	}

	if ( intersect.root1 ) {
		CollideCallback callback = intersect.root1->GetCollideCallback();
		if ( callback ) {
			NiAVObject *root = intersect.root0;
			intersect.root0 = intersect.root1;
			intersect.root1 = root;
			NiPoint3 normal = intersect.normal0;
			intersect.normal0 = intersect.normal1;
			intersect.normal1 = normal;
			result1 = callback( intersect );
			root = intersect.root0;
			intersect.root0 = intersect.root1;
			intersect.root1 = root;
		}
	}

	if ( result0 || result1 ) {
		return 1;
	}
	return 0;
}

/*
================
NiAVObject::TestRayBound

True when the origin lies inside the world bound, or when the ray heads
toward the bound's centre and passes within its radius.
================
*/
// FUNCTION: STBC 0x7DD170
bool NiAVObject::TestRayBound( const NiPoint3 &origin, const NiPoint3 &dir ) {
	NiPoint3 diff = origin - worldBound.center;
	float c = diff.SqrLength() - worldBound.radius * worldBound.radius;
	if ( c <= 0.0f ) {
		return true;
	}
	float b = dir.Dot( diff );
	if ( b >= 0.0f ) {
		return false;
	}
	return b * b >= c * dir.SqrLength();
}

/*
================
NiAVObject::GetObjectByName

The object itself when its name matches; NiNode searches its subtree.
================
*/
// FUNCTION: STBC 0x7DD230
NiAVObject *NiAVObject::GetObjectByName( const char *name_ ) {
	if ( !name_ || !GetName() ) {
		return 0;
	}
	return strcmp( name_, GetName() ) == 0 ? this : 0;
}

/*
================
NiAVObject::CreateClone

NiAVObject itself is never cloned; the derived classes create the clone.
================
*/
// FUNCTION: STBC 0x7DD2B0
NiObject *NiAVObject::CreateClone() {
	return 0;
}

/*
================
NiAVObject::CopyProperties

Walks to the end of the list first, so that the clones are attached in the
original's order.
================
*/
// FUNCTION: STBC 0x7DD2C0
void NiAVObject::CopyProperties( NiAVObject *dest, NiPropertyList *list ) {
	if ( list ) {
		CopyProperties( dest, list->GetNext() );
		NiProperty *property = list->GetItem();
		dest->properties.AddHead( (NiProperty *)property->CreateClone() );
	}
}

/*
================
NiAVObject::CopyMembers

The clone gets the flags, the local transform and velocity and clones of
the properties.
================
*/
// FUNCTION: STBC 0x7DD340
void NiAVObject::CopyMembers( NiAVObject *dest ) {
	NiObjectNET::CopyMembers( dest );

	dest->flags = flags;
	dest->localTranslate = localTranslate;
	dest->localRotate = localRotate;
	dest->localScale = localScale;
	dest->localVelocity = localVelocity;

	if ( properties.ItemsInList() > 0 ) {
		NiPropertyList *list = &properties;
		CopyProperties( dest, list );
	}
}

/*
================
NiAVObject::ProcessClone
================
*/
// FUNCTION: STBC 0x7DD3E0
void NiAVObject::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiAVObject, clone ) );
	NiObjectNET::ProcessClone( clone );
}

/*
================
NiAVObject::VirtualSlot03

Passes the call on to the properties.
================
*/
// FUNCTION: STBC 0x7DD3F0
void NiAVObject::VirtualSlot03() {
	NiObjectNET::VirtualSlot03();
	if ( properties.ItemsInList() > 0 ) {
		NiPropertyList *node = &properties;
		while ( node ) {
			NiProperty *property = node->GetItem();
			property->VirtualSlot03();
			node = node->GetNext();
		}
	}
}

/*
================
NiAVObject::CreateObject

NiAVObject is abstract: nothing to create.
================
*/
// FUNCTION: STBC 0x7DD470
void NiAVObject::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiAVObject::LoadBinary

NiObjectNET's members, the flags and the local transform, the link IDs of
the properties, then the model bounding volume when one was saved.

Gamebryo 1.2.2: NiAVObject::LoadBinary reads the old files' volume
pointer the same way (pkBV), as a flag that a volume follows.
================
*/
// FUNCTION: STBC 0x7DD480
void NiAVObject::LoadBinary( NiStream &stream ) {
	NiObjectNET::LoadBinary( stream );
	NiStreamLoadBinary( stream, flags );
	localTranslate.LoadBinary( stream );
	localRotate.LoadBinary( stream );
	NiStreamLoadBinary( stream, localScale );
	localVelocity.LoadBinary( stream );

	NiAVObjectLoadRecord *linkRecord = (NiAVObjectLoadRecord *)stream.GetLoadRecord();
	assert( linkRecord );
	unsigned int uiListSize;
	NiStreamLoadBinary( stream, uiListSize );
	for ( unsigned int i = 0; i < uiListSize; i++ ) {
		unsigned int linkID;
		NiStreamLoadBinary( stream, linkID );
		linkRecord->propertyLinkIDs.Add( linkID );
	}

	NiBoundingVolume *pBV;	// the saved pointer only tells whether a volume follows; (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, pBV );
	if ( pBV ) {
		pBV = NiBoundingVolume::CreateFromStream( stream );
		SetModelABV( pBV );
	}
}

/*
================
NiAVObject::LinkObject

Attaches the properties, last first, so that the list keeps the saved
order.
================
*/
// FUNCTION: STBC 0x7DD5F0
void NiAVObject::LinkObject( NiStream &stream ) {
	NiObjectNET::LinkObject( stream );

	NiAVObjectLoadRecord *linkRecord = (NiAVObjectLoadRecord *)stream.GetLoadRecord();
	assert( linkRecord );
	unsigned int uiListSize = linkRecord->propertyLinkIDs.GetSize();
	for ( int i = uiListSize - 1; i >= 0; i-- ) {
		NiProperty *property = (NiProperty *)stream.GetObjectFromLinkID( linkRecord->propertyLinkIDs.GetAt( i ) );
		AttachProperty( property );
	}
}

/*
================
NiAVObject::RegisterStreamables

Registers the properties after the object.
================
*/
// FUNCTION: STBC 0x7DD630
bool NiAVObject::RegisterStreamables( NiStream &stream ) {
	if ( !NiObjectNET::RegisterStreamables( stream ) ) {
		return false;
	}
	NiPropertyList *node = &properties;
	while ( node ) {
		NiProperty *property = node->GetItem();
		if ( property ) {
			property->RegisterStreamables( stream );
		}
		node = node->GetNext();
	}
	return true;
}

/*
================
NiAVObject::SaveBinary

The order LoadBinary reads: the flags, the local transform, the link IDs
(addresses) of the properties, then the model bounding volume.
================
*/
// FUNCTION: STBC 0x7DD6A0
void NiAVObject::SaveBinary( NiStream &stream ) {
	NiObjectNET::SaveBinary( stream );
	NiStreamSaveBinary( stream, flags );
	localTranslate.SaveBinary( stream );
	localRotate.SaveBinary( stream );
	NiStreamSaveBinary( stream, localScale );
	localVelocity.SaveBinary( stream );

	unsigned int count = properties.ItemsInList();
	NiStreamSaveBinary( stream, count );
	if ( count > 0 ) {
		NiPropertyList *node = &properties;
		while ( node ) {
			NiProperty *property = node->GetItem();
			NiStreamSaveBinary( stream, property );
			node = node->GetNext();
		}
	}

	// the pointer saves as the flag that LoadBinary tests
	NiStreamSaveBinary( stream, modelABV );
	if ( modelABV ) {
		modelABV->SaveBinary( stream );
	}
}

/*
================
NiAVObject::IsEqual

Compares the local transform, the properties, the propagation mode and
the model bounding volumes. The locals are Gamebryo 1.2.2's
(NiAVObject::IsEqual's uiCount0, pkAVObject, kPos0, pkProperty0), which
AnimOnly.exe 0x416233's frame order needs.
================
*/
// FUNCTION: STBC 0x7DD7B0
bool NiAVObject::IsEqual( NiObject *object ) {
	if ( !NiObjectNET::IsEqual( object ) ) {
		return false;
	}

	NiAVObject *pAVObject = (NiAVObject *)object;
	if ( GetAppCulled() != pAVObject->GetAppCulled() ) {
		return false;
	}
	if ( localTranslate != pAVObject->localTranslate ) {
		return false;
	}
	if ( localRotate != pAVObject->localRotate ) {
		return false;
	}
	if ( localScale != pAVObject->localScale ) {
		return false;
	}
	if ( localVelocity != pAVObject->localVelocity ) {
		return false;
	}

	unsigned int uiCount0 = properties.ItemsInList();
	unsigned int uiCount1 = pAVObject->properties.ItemsInList();
	if ( uiCount0 != uiCount1 ) {
		return false;
	}
	if ( uiCount0 > 0 ) {
		NiPropertyList *pPos0 = &properties;
		NiPropertyList *pPos1 = &pAVObject->properties;
		while ( pPos0 ) {
			NiProperty *pProperty0 = pPos0->GetItem();
			NiProperty *pProperty1 = pPos1->GetItem();
			if ( ( pProperty0 && !pProperty1 ) || ( !pProperty0 && pProperty1 ) ) {
				return false;
			}
			if ( pProperty0 && !pProperty0->IsEqual( pProperty1 ) ) {
				return false;
			}
			pPos0 = pPos0->GetNext();
			pPos1 = pPos1->GetNext();
		}
	}

	if ( GetPropagationMode() != pAVObject->GetPropagationMode() ) {
		return false;
	}

	if ( ( modelABV && !pAVObject->modelABV ) || ( !modelABV && pAVObject->modelABV ) ) {
		return false;
	}
	if ( modelABV && *modelABV != *pAVObject->modelABV ) {
		return false;
	}
	return true;
}

/*
================
NiAVObject::GetViewerStrings

NiObjectNET's strings, the transforms, velocities and bound, the
propagation mode, then the strings of every property. The list is walked
as the SDK's debug NetImmerse 3.1 walks it (AnimOnly.exe 0x4164EC).
================
*/
// FUNCTION: STBC 0x7DDA10
void NiAVObject::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObjectNET::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_bAppCulled", GetAppCulled() ) );
	strings->Add( localTranslate.GetViewerString( "m_localTranslate" ) );
	strings->Add( localRotate.GetViewerString( "m_localRotate" ) );
	strings->Add( NiGetViewerString( "m_fLocalScale", localScale ) );
	strings->Add( localVelocity.GetViewerString( "m_localVelocity" ) );
	strings->Add( worldTranslate.GetViewerString( "m_worldTranslate" ) );
	strings->Add( worldRotate.GetViewerString( "m_worldRotate" ) );
	strings->Add( NiGetViewerString( "m_worldScale", worldScale ) );
	strings->Add( worldVelocity.GetViewerString( "m_worldVelocity" ) );
	strings->Add( worldBound.GetViewerString( "m_worldBound" ) );
	strings->Add( GetViewerString( "m_ePropagate", GetPropagationMode() ) );

	if ( properties.ItemsInList() > 0 ) {
		NiPropertyList *list = &properties;
		while ( list ) {
			NiProperty *property = list->GetItem();
			strings->Add( NiGetViewerString( property->GetRTTI()->GetName(), property ) );
			list = list->GetNext();
		}
	}
}

/*
================
NiAVObject::GetViewerString

The propagation mode by name.
================
*/
// FUNCTION: STBC 0x7DDCC0
char *NiAVObject::GetViewerString( const char *prefix, PropagationMode mode ) {
	char *string = new char[strlen( prefix ) + 17];
	switch ( mode ) {
	case PROPAGATE_NONE:
		sprintf( string, "%s = NONE", prefix );
		break;
	case PROPAGATE_USE_TRIANGLES:
		sprintf( string, "%s = USE_TRIANGLES", prefix );
		break;
	case PROPAGATE_USE_OBBS:
		sprintf( string, "%s = USE_OBBS", prefix );
		break;
	}
	return string;
}

