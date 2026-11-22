/*
===============================================================================

	BaseObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include "BaseObjectClass.h"
#include "TGEventHandlerTable.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "UtopiaModule.h"
#include "Game.h"
#include "Episode.h"
#include "Mission.h"
#include "TGLocalizationDatabase.h"
#include "NiMath.h"
#include "NiQuaternion.h"
#include "NiMatrix3.h"
#include "NiMain.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004326b0 _$E472
// FUNCTION: STBC 0x004326d0 _$E471

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00431fa0 _$E32
// FUNCTION: STBC 0x00431fb0 _$E29
// FUNCTION: STBC 0x00431fc0 _$E31
// FUNCTION: STBC 0x00431fd0 _$E30
// FUNCTION: STBC 0x00431fe0 _$E38
// FUNCTION: STBC 0x00431ff0 _$E35
// FUNCTION: STBC 0x00432000 _$E37
// FUNCTION: STBC 0x00432010 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00432020 _$E66
// FUNCTION: STBC 0x00432030 _$E65
// FUNCTION: STBC 0x00432040 _$E236
// FUNCTION: STBC 0x00432050 _$E235
// FUNCTION: STBC 0x00432060 _$E77
// FUNCTION: STBC 0x00432070 _$E76
// FUNCTION: STBC 0x00432080 _$E81
// FUNCTION: STBC 0x00432090 _$E80
// FUNCTION: STBC 0x004320a0 _$E85
// FUNCTION: STBC 0x004320b0 _$E84
// FUNCTION: STBC 0x004320c0 _$E89
// FUNCTION: STBC 0x004320d0 _$E88
// FUNCTION: STBC 0x004320e0 _$E206
// FUNCTION: STBC 0x004320f0 _$E205
// FUNCTION: STBC 0x00432100 _$E148
// FUNCTION: STBC 0x00432110 _$E147
// FUNCTION: STBC 0x00432120 _$E161
// FUNCTION: STBC 0x00432130 _$E160
// FUNCTION: STBC 0x00432140 _$E211
// FUNCTION: STBC 0x00432150 _$E210
// FUNCTION: STBC 0x00432160 _$E241
// FUNCTION: STBC 0x00432170 _$E240
// FUNCTION: STBC 0x00432180 _$E215
// FUNCTION: STBC 0x00432190 _$E214
// FUNCTION: STBC 0x004321f0 _$E245
// FUNCTION: STBC 0x00432200 _$E244
// FUNCTION: STBC 0x00432210 _$E252
// FUNCTION: STBC 0x00432220 _$E251
// FUNCTION: STBC 0x00432230 _$E291
// FUNCTION: STBC 0x00432240 _$E290
// FUNCTION: STBC 0x00432250 _$E295
// FUNCTION: STBC 0x00432260 _$E294
// FUNCTION: STBC 0x00432270 _$E106
// FUNCTION: STBC 0x00432280 _$E105
// FUNCTION: STBC 0x00432290 _$E277
// FUNCTION: STBC 0x004322a0 _$E276
// FUNCTION: STBC 0x004322b0 _$E284
// FUNCTION: STBC 0x004322c0 _$E283
// FUNCTION: STBC 0x004322d0 _$E307
// FUNCTION: STBC 0x004322e0 _$E306
// FUNCTION: STBC 0x004322f0 _$E311
// FUNCTION: STBC 0x00432300 _$E310
// FUNCTION: STBC 0x00432310 _$E351
// FUNCTION: STBC 0x00432320 _$E350
// FUNCTION: STBC 0x00432330 _$E357
// FUNCTION: STBC 0x00432340 _$E356
// FUNCTION: STBC 0x00432350 _$E299
// FUNCTION: STBC 0x00432360 _$E298
// FUNCTION: STBC 0x00432370 _$E303
// FUNCTION: STBC 0x00432380 _$E302
// FUNCTION: STBC 0x00432390 _$E315
// FUNCTION: STBC 0x004323a0 _$E314
// FUNCTION: STBC 0x004323b0 _$E319
// FUNCTION: STBC 0x004323c0 _$E318
// FUNCTION: STBC 0x004323d0 _$E323
// FUNCTION: STBC 0x004323e0 _$E322
// FUNCTION: STBC 0x004323f0 _$E327
// FUNCTION: STBC 0x00432400 _$E326
// FUNCTION: STBC 0x00432410 _$E331
// FUNCTION: STBC 0x00432420 _$E330
// FUNCTION: STBC 0x00432430 _$E335
// FUNCTION: STBC 0x00432440 _$E334
// FUNCTION: STBC 0x00432450 _$E339
// FUNCTION: STBC 0x00432460 _$E338
// FUNCTION: STBC 0x00432470 _$E343
// FUNCTION: STBC 0x00432480 _$E342
// FUNCTION: STBC 0x00432490 _$E111
// FUNCTION: STBC 0x004324a0 _$E110
// FUNCTION: STBC 0x004324b0 _$E347
// FUNCTION: STBC 0x004324c0 _$E346
// FUNCTION: STBC 0x004324d0 _$E116
// FUNCTION: STBC 0x004324e0 _$E115
// FUNCTION: STBC 0x004324f0 _$E361
// FUNCTION: STBC 0x00432500 _$E360
// FUNCTION: STBC 0x00432510 _$E365
// FUNCTION: STBC 0x00432520 _$E364
// FUNCTION: STBC 0x00432530 _$E369
// FUNCTION: STBC 0x00432540 _$E368
// FUNCTION: STBC 0x00432550 _$E373
// FUNCTION: STBC 0x00432560 _$E372
// FUNCTION: STBC 0x00432570 _$E125
// FUNCTION: STBC 0x00432580 _$E124
// FUNCTION: STBC 0x00432590 _$E377
// FUNCTION: STBC 0x004325a0 _$E376
// FUNCTION: STBC 0x004325b0 _$E381
// FUNCTION: STBC 0x004325c0 _$E380
// FUNCTION: STBC 0x004325d0 _$E385
// FUNCTION: STBC 0x004325e0 _$E384
// FUNCTION: STBC 0x004325f0 _$E131
// FUNCTION: STBC 0x00432600 _$E130
// FUNCTION: STBC 0x00432610 _$E136
// FUNCTION: STBC 0x00432620 _$E135
// FUNCTION: STBC 0x00432630 _$E166
// FUNCTION: STBC 0x00432640 _$E165
// FUNCTION: STBC 0x00432650 _$E389
// FUNCTION: STBC 0x00432660 _$E388
// FUNCTION: STBC 0x00432670 _$E171
// FUNCTION: STBC 0x00432680 _$E170
// FUNCTION: STBC 0x00432690 _$E393
// FUNCTION: STBC 0x004326a0 _$E392
// FUNCTION: STBC 0x004326e0 _$E397
// FUNCTION: STBC 0x004326f0 _$E396
// FUNCTION: STBC 0x00432700 _$E176
// FUNCTION: STBC 0x00432710 _$E175
// FUNCTION: STBC 0x00432720 _$E401
// FUNCTION: STBC 0x00432730 _$E400
// FUNCTION: STBC 0x00432740 _$E141
// FUNCTION: STBC 0x00432750 _$E140
// FUNCTION: STBC 0x00432760 _$E405
// FUNCTION: STBC 0x00432770 _$E404
// FUNCTION: STBC 0x00432780 _$E409
// FUNCTION: STBC 0x00432790 _$E408
// FUNCTION: STBC 0x004327a0 _$E181
// FUNCTION: STBC 0x004327b0 _$E180
// FUNCTION: STBC 0x004327c0 _$E186
// FUNCTION: STBC 0x004327d0 _$E185
// FUNCTION: STBC 0x004327e0 _$E413
// FUNCTION: STBC 0x004327f0 _$E412
// FUNCTION: STBC 0x00432800 _$E417
// FUNCTION: STBC 0x00432810 _$E416
// FUNCTION: STBC 0x00432820 _$E421
// FUNCTION: STBC 0x00432830 _$E420
// FUNCTION: STBC 0x00432840 _$E425
// FUNCTION: STBC 0x00432850 _$E424
// FUNCTION: STBC 0x00432860 _$E429
// FUNCTION: STBC 0x00432870 _$E428
// FUNCTION: STBC 0x00432880 _$E433
// FUNCTION: STBC 0x00432890 _$E432
// FUNCTION: STBC 0x004328a0 _$E437
// FUNCTION: STBC 0x004328b0 _$E436
// FUNCTION: STBC 0x004328c0 _$E441
// FUNCTION: STBC 0x004328d0 _$E440
// FUNCTION: STBC 0x004328e0 _$E191
// FUNCTION: STBC 0x004328f0 _$E190
// FUNCTION: STBC 0x00432900 _$E445
// FUNCTION: STBC 0x00432910 _$E444
// FUNCTION: STBC 0x00432920 _$E196
// FUNCTION: STBC 0x00432930 _$E195
// FUNCTION: STBC 0x00432940 _$E201
// FUNCTION: STBC 0x00432950 _$E200
// FUNCTION: STBC 0x00432960 _$E449
// FUNCTION: STBC 0x00432970 _$E448
// FUNCTION: STBC 0x00432980 _$E454
// FUNCTION: STBC 0x00432990 _$E453
// FUNCTION: STBC 0x004329a0 _$E458
// FUNCTION: STBC 0x004329b0 _$E457

// FUNCTION: STBC 0x00430650 _$E464
// FUNCTION: STBC 0x00430660 _$E460
// FUNCTION: STBC 0x00430670 _$E463
// FUNCTION: STBC 0x00430680 _$E461
// DATA: STBC 0x0097f358
TGEventHandlerTable BaseObjectClass::classHandlers;

/*
================
BaseObjectClass::AddToSet

Adds the object to the set's list of its class type. Derived classes add
it under their own type too and call this.
================
*/
// FUNCTION: STBC 0x004306a0
void BaseObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, BASEOBJECTCLASS_TYPE );
	}
}

/*
================
BaseObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x004306c0
void BaseObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, BASEOBJECTCLASS_TYPE );
	}
}

/*
================
BaseObjectClass::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x004306e0
BaseObjectClass **BaseObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return set->objectTable.GetObjects( BASEOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
BaseObjectClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x00430710
int BaseObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( BASEOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
BaseObjectClass::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a BaseObjectClass.
================
*/
// FUNCTION: STBC 0x00430730
BaseObjectClass *BaseObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( BASEOBJECTCLASS_TYPE ) ) {
			return (BaseObjectClass *)object;
		}
		return NULL;
	}
	return set->GetClassObject( BASEOBJECTCLASS_TYPE, id );
}

/*
================
BaseObjectClass::GetObject

The object of the name in the set, or in any set when the set is NULL,
the last set first.
================
*/
// FUNCTION: STBC 0x004307a0
BaseObjectClass *BaseObjectClass::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( BASEOBJECTCLASS_TYPE, name );
		}
		return object;
	}
	return set->GetClassObject( BASEOBJECTCLASS_TYPE, name );
}

/*
================
BaseObjectClass::Cast

The object as a BaseObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00430800
BaseObjectClass *BaseObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( BASEOBJECTCLASS_TYPE ) ) {
		return (BaseObjectClass *)object;
	}
	return NULL;
}

/*
================
BaseObjectClass::InitObject

What both constructors share: the object starts outside any set, with a
node of its own and no name.
================
*/
// FUNCTION: STBC 0x00430830
void BaseObjectClass::InitObject( void ) {
	lastUpdateTime = -1.0f;
	containingSet = NULL;
	deleteMe = false;
	unknown25 = false;
	name = NULL;
	node = new NiNode;
	displayName.AssignC( "{unspecified display name}" );
}

/*
================
BaseObjectClass::BaseObjectClass
================
*/
// FUNCTION: STBC 0x004308e0 ??0BaseObjectClass@@QAE@J@Z
BaseObjectClass::BaseObjectClass( long id ) : TGEventHandlerObject( id ), displayName( "" ) {
	InitObject();
}

/*
================
BaseObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x00430950
int BaseObjectClass::GetObjType( void ) {
	return BASEOBJECTCLASS_TYPE;
}

/*
================
BaseObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x004309a0
char *BaseObjectClass::GetClassName( void ) {
	return "BaseObjectClass";
}

/*
================
BaseObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004309b0
char *BaseObjectClass::GetSwigTypeName( void ) {
	return "_p_BaseObjectClass";
}

/*
================
BaseObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004309c0
char *BaseObjectClass::GetPythonClassName( void ) {
	return "BaseObjectClassPtr";
}

/*
================
BaseObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004309d0
TGEventHandlerTable *BaseObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
BaseObjectClass::BaseObjectClass

An object showing the given NetImmerse object.
================
*/
// FUNCTION: STBC 0x00430a10 ??0BaseObjectClass@@QAE@PAVNiAVObject@@@Z
BaseObjectClass::BaseObjectClass( NiAVObject *object ) : TGEventHandlerObject( 0 ), displayName( "" ) {
	InitObject();
	SetNiObject( object );
}

/*
================
BaseObjectClass::~BaseObjectClass
================
*/
// FUNCTION: STBC 0x00430a90
BaseObjectClass::~BaseObjectClass( void ) {
	delete name;
}

/*
================
BaseObjectClass::Create
================
*/
// FUNCTION: STBC 0x00430b40
BaseObjectClass *BaseObjectClass::Create( NiAVObject *object ) {
	return new BaseObjectClass( object );
}

/*
================
BaseObjectClass::SetNiObject

Replaces what the object shows: the old object leaves the node, the new
one is attached under it.
================
*/
// FUNCTION: STBC 0x00430b70
void BaseObjectClass::SetNiObject( NiAVObject *object ) {
	if ( niObject != NULL && node != NULL ) {
		node->DetachChild( niObject );
	}
	niObject = object;
	if ( object != NULL && node != NULL ) {
		node->AttachChild( object, false );
		node->UpdateProperties();
	}
}

/*
================
BaseObjectClass::AttachObject

Attaches another object's node under this object's, so that it follows
this object.
================
*/
// FUNCTION: STBC 0x00430c10
void BaseObjectClass::AttachObject( BaseObjectClass *object ) {
	if ( object != NULL && node != NULL ) {
		node->AttachChild( object->node, true );
	}
}

/*
================
BaseObjectClass::IsObjectAttached

True when the other object's node is a child of this object's.
================
*/
// FUNCTION: STBC 0x00430c30
bool BaseObjectClass::IsObjectAttached( BaseObjectClass *object ) {
	NiNode *parent = node;
	NiNode *child = object->node;
	if ( parent != NULL && child != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			if ( parent->GetAt( i ) == child ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
BaseObjectClass::DetachObject

Takes another object's node from under this object's and puts it back
under the scene root of the other object's set.
================
*/
// FUNCTION: STBC 0x00430c80
void BaseObjectClass::DetachObject( BaseObjectClass *object ) {
	if ( object != NULL && node != NULL ) {
		NiNode *objectNode = object->node;
		if ( objectNode != NULL ) {
			node->DetachChild( objectNode );
			SetClass *set = object->containingSet;
			if ( set != NULL && set->GetSceneRoot() != NULL ) {
				set->GetSceneRoot()->AttachChild( objectNode, false );
			}
		}
	}
}

/*
================
BaseObjectClass::Update
================
*/
// FUNCTION: STBC 0x00430cf0
void BaseObjectClass::Update( float time ) {
	lastUpdateTime = time;
	UpdateNode( time );
}

/*
================
BaseObjectClass::UpdateNode

Updates the node, once the object is in a set.
================
*/
// FUNCTION: STBC 0x00430d10
void BaseObjectClass::UpdateNode( float time ) {
	if ( containingSet != NULL ) {
		node->Update( time );
	}
}

/*
================
BaseObjectClass::UpdateNodeOnly
================
*/
// FUNCTION: STBC 0x00430d30
void BaseObjectClass::UpdateNodeOnly( void ) {
	if ( node != NULL ) {
		node->Update( 0.0f );
	}
}

/*
================
BaseObjectClass::ContainsNode

Whether the node is the object's own or lies under it.
================
*/
// FUNCTION: STBC 0x00430d40 ?ContainsNode@BaseObjectClass@@QAE_NPAVNiAVObject@@@Z
bool BaseObjectClass::ContainsNode( NiAVObject *target ) {
	return ContainsNode( target, node );
}

/*
================
BaseObjectClass::ContainsNode

Whether the node is the root or lies under it.
================
*/
// FUNCTION: STBC 0x00430d60 ?ContainsNode@BaseObjectClass@@QAE_NPAVNiAVObject@@0@Z
bool BaseObjectClass::ContainsNode( NiAVObject *target, NiAVObject *root ) {
	if ( root == target ) {
		return true;
	}
	NiNode *parent = NiDynamicCast( NiNode, root );
	bool found = false;
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			NiAVObject *child = parent->GetAt( i );
			if ( child != NULL && child->IsKindOf( &NiNode::rtti ) && ContainsNode( target, child ) ) {
				found = true;
				break;
			}
		}
	}
	return found;
}

/*
================
BaseObjectClass::SetContainingSet

Attaches the object's node under the set's scene root.
================
*/
// FUNCTION: STBC 0x00430e20
void BaseObjectClass::SetContainingSet( SetClass *set ) {
	containingSet = set;
	if ( set != NULL ) {
		set->GetSceneRoot()->AttachChild( node, true );
	}
}

/*
================
BaseObjectClass::SetTranslateXYZ
================
*/
// FUNCTION: STBC 0x00430e50
void BaseObjectClass::SetTranslateXYZ( float x, float y, float z ) {
	if ( node != NULL ) {
		node->SetTranslate( NiPoint3( x, y, z ) );
	}
}

/*
================
BaseObjectClass::GetTranslate

The node's translation; the origin when there is no node.
================
*/
// FUNCTION: STBC 0x00430e70
const TGPoint3 &BaseObjectClass::GetTranslate( void ) {
	if ( node != NULL ) {
		return static_cast<const TGPoint3 &>( node->GetTranslate() );
	}
	return TGPoint3::ZERO;
}

/*
================
BaseObjectClass::GetWorldLocation

The node's world translation; the origin when there is no node.
================
*/
// FUNCTION: STBC 0x00430e90
const TGPoint3 &BaseObjectClass::GetWorldLocation( void ) {
	if ( node != NULL ) {
		return static_cast<const TGPoint3 &>( node->GetWorldTranslate() );
	}
	return TGPoint3::ZERO;
}

/*
================
BaseObjectClass::SetScale

A negative scale is stored as its magnitude.
================
*/
// FUNCTION: STBC 0x00430eb0
void BaseObjectClass::SetScale( float scale ) {
	if ( node != NULL ) {
		if ( scale > 0.0f ) {
			node->SetScale( scale );
		} else {
			node->SetScale( -scale );
		}
	}
}

/*
================
BaseObjectClass::GetScale
================
*/
// FUNCTION: STBC 0x00430ee0
float BaseObjectClass::GetScale( void ) {
	if ( node != NULL ) {
		return node->GetScale();
	}
	return 1.0f;
}

/*
================
BaseObjectClass::SetAngleAxisRotation

Sets the node's rotation to the given angle, in radians, about the axis.
================
*/
// FUNCTION: STBC 0x00430f00
void BaseObjectClass::SetAngleAxisRotation( float angle, float x, float y, float z ) {
	if ( node != NULL ) {
		NiMatrix3 rotation;
		rotation.MakeRotation( angle, NiPoint3( x, y, z ) );
		node->SetRotate( rotation );
	}
}

/*
================
BaseObjectClass::SetMatrixRotation
================
*/
// FUNCTION: STBC 0x00430f50 ?SetMatrixRotation@BaseObjectClass@@UAEXABVNiMatrix3@@@Z
void BaseObjectClass::SetMatrixRotation( const NiMatrix3 &matrix ) {
	if ( node != NULL ) {
		node->SetRotate( matrix );
	}
}

/*
================
BaseObjectClass::GetRotation

The node's rotation; the identity when there is no node.
================
*/
// FUNCTION: STBC 0x00430f70
const TGMatrix3 &BaseObjectClass::GetRotation( void ) {
	if ( node != NULL ) {
		return static_cast<const TGMatrix3 &>( node->GetRotate() );
	}
	return static_cast<const TGMatrix3 &>( NiMatrix3::IDENTITY );
}

/*
================
BaseObjectClass::GetWorldRotation

The node's world rotation; the identity when there is no node.
================
*/
// FUNCTION: STBC 0x00430f90
const TGMatrix3 &BaseObjectClass::GetWorldRotation( void ) {
	if ( node != NULL ) {
		return static_cast<const TGMatrix3 &>( node->GetWorldRotate() );
	}
	return static_cast<const TGMatrix3 &>( NiMatrix3::IDENTITY );
}

/*
================
BaseObjectClass::Rotate

Turns the object by the rotation, given in world space.
================
*/
// FUNCTION: STBC 0x00430fb0 ?Rotate@BaseObjectClass@@UAEXABVNiMatrix3@@@Z
void BaseObjectClass::Rotate( const NiMatrix3 &matrix ) {
	if ( node != NULL ) {
		NiMatrix3 rotation = node->GetWorldRotate().Inverse() * matrix * node->GetWorldRotate();
		rotation = node->GetRotate() * rotation;
		rotation.Reorthogonalize();
		node->SetRotate( rotation );
	}
}

/*
================
BaseObjectClass::CompareKey

Zero when the object has the ID; positive when the ID sorts after the
object's, negative when before.
================
*/
// FUNCTION: STBC 0x00431030
int BaseObjectClass::CompareKey( long id_ ) {
	if ( id_ == id ) {
		return 0;
	}
	return id_ < id ? -1 : 1;
}

/*
================
BaseObjectClass::VirtualSlot41

The translation of the named node of the object's animation; the origin
when there is no such node.
================
*/
// FUNCTION: STBC 0x00431060
TGPoint3 BaseObjectClass::VirtualSlot41( char *nodeName ) {
	TGAnimNode *animNode = BaseObjectClass::GetAnimNode();
	if ( animNode != NULL ) {
		NiNode *namedNode = NiDynamicCast( NiNode, animNode->GetObjectByName( nodeName ) );
		if ( namedNode != NULL ) {
			return namedNode->GetTranslate();
		}
	}
	return TGPoint3::ZERO;
}

/*
================
BaseObjectClass::VirtualSlot42

The rotation of the named node of the object's animation; the identity
when there is no such node.
================
*/
// FUNCTION: STBC 0x00431130
TGMatrix3 BaseObjectClass::VirtualSlot42( char *nodeName ) {
	TGAnimNode *animNode = BaseObjectClass::GetAnimNode();
	if ( animNode != NULL ) {
		NiNode *namedNode = NiDynamicCast( NiNode, animNode->GetObjectByName( nodeName ) );
		if ( namedNode != NULL ) {
			return namedNode->GetRotate();
		}
	}
	return NiMatrix3::IDENTITY;
}

/*
================
BaseObjectClass::AlignToVectors

Turns the object by the turn that takes its own forward and up onto the given ones.
================
*/
// FUNCTION: STBC 0x004311f0 ?AlignToVectors@BaseObjectClass@@QAEXABVNiPoint3@@0@Z
void BaseObjectClass::AlignToVectors( const NiPoint3 &forward, const NiPoint3 &up ) {
	NiMatrix3 turn = Unknown431220( forward, up );
	Rotate( turn );
}

/*
================
BaseObjectClass::Unknown431220

The matrix of the turn from the object's world frame to the given one.
================
*/
// FUNCTION: STBC 0x00431220
NiMatrix3 BaseObjectClass::Unknown431220( const NiPoint3 &forward, const NiPoint3 &up ) {
	NiMatrix3 matrix( NiMatrix3::IDENTITY );
	NiQuaternion turn = Unknown431280( forward, up );
	turn.ToRotation( matrix );
	return matrix;
}

/*
================
BaseObjectClass::Unknown431280

The turn from the object's world frame to the given one.
================
*/
// FUNCTION: STBC 0x00431280
NiQuaternion BaseObjectClass::Unknown431280( const NiPoint3 &forward, const NiPoint3 &up ) {
	return GetTurnBetweenFrames( GetWorldForward(), GetWorldUp(), forward, up );
}

/*
================
BaseObjectClass::Unknown4312D0

The turn as a vector: its axis scaled by minus its angle.
================
*/
// FUNCTION: STBC 0x004312d0
NiPoint3 BaseObjectClass::Unknown4312D0( const NiPoint3 &forward, const NiPoint3 &up ) {
	NiQuaternion turn = Unknown431280( forward, up );
	float angle;
	NiPoint3 axis;
	turn.ToAngleAxis( angle, axis );
	axis *= -angle;
	return axis;
}

/*
================
GetTurnBetweenFrames

The turn that takes fromForward onto toForward and then the turned fromUp
onto toUp. A turn between parallel directions has no axis: it turns about
the direction's up instead.
================
*/
// FUNCTION: STBC 0x00431340
NiQuaternion GetTurnBetweenFrames( const NiPoint3 &fromForward, const NiPoint3 &fromUp, const NiPoint3 &toForward, const NiPoint3 &toUp ) {
	NiPoint3 axis;
	float angle;
	GetTurnBetween( angle, axis, fromForward, toForward );
	if ( axis == NiPoint3::ZERO ) {
		axis = fromUp;
	}
	NiQuaternion first;
	first.FromAngleAxis( -angle, axis );
	NiMatrix3 rotation;
	first.ToRotation( rotation );
	NiPoint3 turnedUp = rotation * fromUp;

	GetTurnBetween( angle, axis, turnedUp, toUp );
	if ( axis == NiPoint3::ZERO ) {
		axis = toForward;
	}
	NiQuaternion second;
	second.FromAngleAxis( -angle, axis );
	first = second * first;
	return first;
}

/*
================
GetTurnBetween

The angle, and the unit axis, of the turn that takes from onto to; a zero
axis when they are parallel.
================
*/
// FUNCTION: STBC 0x004314b0
void GetTurnBetween( float &angle, NiPoint3 &axis, const NiPoint3 &from, const NiPoint3 &to ) {
	axis = to.Cross( from );
	axis.Unitize();
	angle = NiACos( from.Dot( to ) );
}

/*
================
BaseObjectClass::SetName

Renames the object in its set and takes the display name from the first
localisation database that knows the name: the mission's, the episode's,
the game's. Without one the name is its own display name.
================
*/
// FUNCTION: STBC 0x004315c0 ?SetName@BaseObjectClass@@UAEXPAD@Z
// PARKED: register: retail keeps the game in edi and the episode in ebp, ours the reverse (register allocation of the three database lookups; tried local order, one shared database local, block-scoped locals)
void BaseObjectClass::SetName( char *name_ ) {
	if ( name_ == name ) {
		return;
	}
	if ( name != NULL ) {
		if ( containingSet != NULL ) {
			containingSet->Unknown4145E0( name, name_ );
		}
		delete name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
		Game *game = Game::GetCurrentGame();
		if ( game != NULL ) {
			Episode *episode = game->GetCurrentEpisode();
			if ( episode != NULL ) {
				Mission *mission = episode->GetCurrentMission();
				if ( mission != NULL ) {
					TGLocalizationDatabase *database = mission->GetDatabase();
					if ( database != NULL && database->HasString( name ) ) {
						TGString *display = database->GetString( name );
						if ( display != NULL ) {
							SetDisplayName( display );
							return;
						}
					}
				}
				TGLocalizationDatabase *database = episode->GetDatabase();
				if ( database != NULL && database->HasString( name ) ) {
					TGString *display = database->GetString( name );
					if ( display != NULL ) {
						SetDisplayName( display );
						return;
					}
				}
			}
			TGLocalizationDatabase *database = game->GetDatabase();
			if ( database != NULL && database->HasString( name ) ) {
				TGString *display = database->GetString( name );
				if ( display != NULL ) {
					SetDisplayName( display );
					return;
				}
			}
		}
		TGString displayName( name );
		SetDisplayName( &displayName );
	} else {
		TGString displayName( "" );
		SetDisplayName( &displayName );
	}
}

/*
================
BaseObjectClass::SetDisplayName

Unless a game is loading, tells the object's handlers at once with an
ET_NAME_CHANGE event. The event of a plain ObjectClass is not saved.
================
*/
// FUNCTION: STBC 0x00431780
void BaseObjectClass::SetDisplayName( TGString *displayName_ ) {
	displayName = *displayName_;
	if ( !g_kUtopiaModule.IsLoading() ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_NAME_CHANGE );
		event->SetDestination( this );
		event->SetSource( this );
		char *className = GetClassName();
		if ( strcmp( className, "ObjectClass" ) == 0 ) {
			event->SetNotSaved( true );
		}
		g_kEventManager.ProcessEvent( event );
		delete event;
	}
}

/*
================
BaseObjectClass::RemovePropertyFromTree
================
*/
// FUNCTION: STBC 0x00431850
void BaseObjectClass::RemovePropertyFromTree( NiAVObject *object, int type ) {
	// BUG: the object is used before the NULL test below
	object->RemoveProperty( type );
	if ( object != NULL && object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL ) {
				RemovePropertyFromTree( child, type );
			}
		}
	}
}

/*
================
BaseObjectClass::MarkGeometryChanged

Counts the model data of every geometry under the object as changed, so the
renderer reads it again. A NULL object is the object's own NetImmerse object.
================
*/
// FUNCTION: STBC 0x00431910
// PARKED: register (peephole): retail tests the geometry cast result with its own test eax,eax; ours branches on the flags of the cast's and
void BaseObjectClass::MarkGeometryChanged( NiAVObject *object ) {
	if ( object == NULL ) {
		object = niObject;
		if ( object == NULL ) {
			return;
		}
	}
	NiGeometry *geometry = NiDynamicCast( NiGeometry, object );
	if ( geometry != NULL ) {
		geometry->modelData->IncRevisionID();
	}
	NiNode *parent = NiDynamicCast( NiNode, object );
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			NiAVObject *child = parent->GetAt( i );
			if ( child != NULL ) {
				MarkGeometryChanged( child );
			}
		}
	}
}

/*
================
BaseObjectClass::GetWorldForward

The object's forward direction in world space.
================
*/
// FUNCTION: STBC 0x004319d0
NiPoint3 BaseObjectClass::GetWorldForward( void ) {
	if ( node == NULL ) {
		return NiPoint3::ZERO;
	}
	// retail builds the product in a temporary and copies it into the result
	return NiPoint3( node->GetWorldRotate() * MODEL_FORWARD );
}

/*
================
BaseObjectClass::GetWorldUp

The object's up direction in world space.
================
*/
// FUNCTION: STBC 0x00431a30
NiPoint3 BaseObjectClass::GetWorldUp( void ) {
	if ( node == NULL ) {
		return NiPoint3::ZERO;
	}
	// retail builds the product in a temporary and copies it into the result
	return NiPoint3( node->GetWorldRotate() * MODEL_UP );
}

/*
================
BaseObjectClass::GetWorldRight

The object's right direction in world space.
================
*/
// FUNCTION: STBC 0x00431a90
NiPoint3 BaseObjectClass::GetWorldRight( void ) {
	if ( node == NULL ) {
		return NiPoint3::ZERO;
	}
	// retail builds the product in a temporary and copies it into the result
	return NiPoint3( node->GetWorldRotate() * MODEL_RIGHT );
}

/*
================
BaseObjectClass::WriteToStream

The node is saved by its address, which ReadFromStream files the loaded
node under; the node's parent likewise, for PostFixUp to attach it again.
================
*/
// FUNCTION: STBC 0x00431af0
bool BaseObjectClass::WriteToStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( lastUpdateTime );
	stream->VirtualSlot17( (long)(NiNode *)node );
	stream->WriteBool( deleteMe );
	stream->WriteBool( unknown25 );
	stream->VirtualSlot11( name );
	displayName.WriteToStream( stream );

	if ( node != NULL && node->GetParent() != NULL ) {
		stream->WriteBool( true );
		stream->VirtualSlot17( (long)node->GetParent() );
	} else {
		stream->WriteBool( false );
	}
	stream->WriteBool( IsHidden() );

	TGMatrix3 rotation = GetRotation();
	TGPoint3 translation = GetTranslate();
	stream->WriteFloat( rotation.GetEntry( 0, 0 ) );
	stream->WriteFloat( rotation.GetEntry( 0, 1 ) );
	stream->WriteFloat( rotation.GetEntry( 0, 2 ) );
	stream->WriteFloat( rotation.GetEntry( 1, 0 ) );
	stream->WriteFloat( rotation.GetEntry( 1, 1 ) );
	stream->WriteFloat( rotation.GetEntry( 1, 2 ) );
	stream->WriteFloat( rotation.GetEntry( 2, 0 ) );
	stream->WriteFloat( rotation.GetEntry( 2, 1 ) );
	stream->WriteFloat( rotation.GetEntry( 2, 2 ) );
	stream->WriteFloat( translation.x );
	stream->WriteFloat( translation.y );
	stream->WriteFloat( translation.z );
	stream->WriteFloat( GetScale() );
	return true;
}

/*
================
BaseObjectClass::ReadFromStream

Files the node under its saved address. Until PostFixUp the containing
set holds the saved address of the node's parent.
================
*/
// FUNCTION: STBC 0x00431cd0
bool BaseObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}

	GetAnimNode();		// the result is not used
	lastUpdateTime = stream->ReadFloat();
	pointerMap.Add( stream->VirtualSlot16(), node );
	deleteMe = stream->ReadBool();
	unknown25 = stream->ReadBool();
	name = stream->VirtualSlot10( -1 );
	displayName.ReadFromStream( stream );

	if ( stream->ReadBool() ) {
		containingSet = (SetClass *)stream->VirtualSlot16();
	}
	unknown26 = stream->ReadBool();

	NiMatrix3 rotation;
	NiPoint3 translation;
	rotation.SetEntry( 0, 0, stream->ReadFloat() );
	rotation.SetEntry( 0, 1, stream->ReadFloat() );
	rotation.SetEntry( 0, 2, stream->ReadFloat() );
	rotation.SetEntry( 1, 0, stream->ReadFloat() );
	rotation.SetEntry( 1, 1, stream->ReadFloat() );
	rotation.SetEntry( 1, 2, stream->ReadFloat() );
	rotation.SetEntry( 2, 0, stream->ReadFloat() );
	rotation.SetEntry( 2, 1, stream->ReadFloat() );
	rotation.SetEntry( 2, 2, stream->ReadFloat() );
	translation.x = stream->ReadFloat();
	translation.y = stream->ReadFloat();
	translation.z = stream->ReadFloat();
	SetMatrixRotation( rotation );
	SetTranslate( &translation );
	SetScale( stream->ReadFloat() );
	return true;
}

/*
================
BaseObjectClass::FixUpPointers

Until PostFixUp the containing set holds the set's saved address; fails
while the set has not been loaded.
================
*/
// FUNCTION: STBC 0x00431e20
bool BaseObjectClass::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	if ( containingSet != NULL && pointerMap.Find( (long)containingSet ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
BaseObjectClass::PostFixUp

Until here the containing set holds the saved address of the node's
parent: the node is attached to the loaded parent again, and its hidden
state, rotation and translation are restored.
================
*/
// FUNCTION: STBC 0x00431e90
void BaseObjectClass::PostFixUp( void ) {
	TGMatrix3 rotation = GetRotation();
	TGPoint3 translation = GetTranslate();

	TGEventHandlerObject::PostFixUp();
	if ( containingSet != NULL ) {
		( (NiNode *)pointerMap.Find( (long)containingSet ) )->AttachChild( node, false );
	}
	containingSet = NULL;

	if ( node != NULL ) {
		node->SetAppCulled( unknown26 );
	}
	SetMatrixRotation( &rotation );
	SetTranslate( translation );
	UpdateNodeOnly();
}
