/*
===============================================================================

	ObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "ObjectClass.h"
#include "ImageManagerClass.h"
#include "ModelHelpers.h"
#include "Episode.h"
#include "Game.h"
#include "Mission.h"
#include "ObjectGroup.h"
#include "PyEmbed.h"
#include "STColors.h"
#include "PlacementObject.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "NiDynamicEffect.h"
#include "NiPick.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiLODNode.h"
#include "NiNode.h"
#include "TGBoolEvent.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "TGSystemWrapperClass.h"
#include "TGMatrix3.h"
#include "NiMatrix3.h"
#include "NiMath.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00436fc0 _$E498
// FUNCTION: STBC 0x00436fe0 _$E497

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004361d0 _$E32
// FUNCTION: STBC 0x004361e0 _$E29
// FUNCTION: STBC 0x004361f0 _$E31
// FUNCTION: STBC 0x00436200 _$E30
// FUNCTION: STBC 0x00436210 _$E38
// FUNCTION: STBC 0x00436220 _$E35
// FUNCTION: STBC 0x00436230 _$E37
// FUNCTION: STBC 0x00436240 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004368a0 _$E66
// FUNCTION: STBC 0x004368b0 _$E65
// FUNCTION: STBC 0x004368c0 _$E278
// FUNCTION: STBC 0x004368d0 _$E277
// FUNCTION: STBC 0x004368e0 _$E77
// FUNCTION: STBC 0x004368f0 _$E76
// FUNCTION: STBC 0x00436900 _$E81
// FUNCTION: STBC 0x00436910 _$E80
// FUNCTION: STBC 0x00436920 _$E85
// FUNCTION: STBC 0x00436930 _$E84
// FUNCTION: STBC 0x00436940 _$E89
// FUNCTION: STBC 0x00436950 _$E88
// FUNCTION: STBC 0x00436960 _$E206
// FUNCTION: STBC 0x00436970 _$E205
// FUNCTION: STBC 0x00436980 _$E148
// FUNCTION: STBC 0x00436990 _$E147
// FUNCTION: STBC 0x004369a0 _$E161
// FUNCTION: STBC 0x004369b0 _$E160
// FUNCTION: STBC 0x004369c0 _$E211
// FUNCTION: STBC 0x004369d0 _$E210
// FUNCTION: STBC 0x004369e0 _$E283
// FUNCTION: STBC 0x004369f0 _$E282
// FUNCTION: STBC 0x00436a00 _$E215
// FUNCTION: STBC 0x00436a10 _$E214
// FUNCTION: STBC 0x00436a20 _$E287
// FUNCTION: STBC 0x00436a30 _$E286
// FUNCTION: STBC 0x00436a40 _$E294
// FUNCTION: STBC 0x00436a50 _$E293
// FUNCTION: STBC 0x00436a60 _$E325
// FUNCTION: STBC 0x00436a70 _$E324
// FUNCTION: STBC 0x00436a80 _$E329
// FUNCTION: STBC 0x00436a90 _$E328
// FUNCTION: STBC 0x00436aa0 _$E106
// FUNCTION: STBC 0x00436ab0 _$E105
// FUNCTION: STBC 0x00436ac0 _$E267
// FUNCTION: STBC 0x00436ad0 _$E266
// FUNCTION: STBC 0x00436ae0 _$E274
// FUNCTION: STBC 0x00436af0 _$E273
// FUNCTION: STBC 0x00436b00 _$E341
// FUNCTION: STBC 0x00436b10 _$E340
// FUNCTION: STBC 0x00436b20 _$E345
// FUNCTION: STBC 0x00436b30 _$E344
// FUNCTION: STBC 0x00436b40 _$E385
// FUNCTION: STBC 0x00436b50 _$E384
// FUNCTION: STBC 0x00436b60 _$E391
// FUNCTION: STBC 0x00436b70 _$E390
// FUNCTION: STBC 0x00436b80 _$E333
// FUNCTION: STBC 0x00436b90 _$E332
// FUNCTION: STBC 0x00436ba0 _$E337
// FUNCTION: STBC 0x00436bb0 _$E336
// FUNCTION: STBC 0x00436bc0 _$E349
// FUNCTION: STBC 0x00436bd0 _$E348
// FUNCTION: STBC 0x00436be0 _$E353
// FUNCTION: STBC 0x00436bf0 _$E352
// FUNCTION: STBC 0x00436c00 _$E357
// FUNCTION: STBC 0x00436c10 _$E356
// FUNCTION: STBC 0x00436c20 _$E361
// FUNCTION: STBC 0x00436c30 _$E360
// FUNCTION: STBC 0x00436c40 _$E365
// FUNCTION: STBC 0x00436c50 _$E364
// FUNCTION: STBC 0x00436c60 _$E369
// FUNCTION: STBC 0x00436c70 _$E368
// FUNCTION: STBC 0x00436c80 _$E373
// FUNCTION: STBC 0x00436c90 _$E372
// FUNCTION: STBC 0x00436ca0 _$E377
// FUNCTION: STBC 0x00436cb0 _$E376
// FUNCTION: STBC 0x00436cc0 _$E111
// FUNCTION: STBC 0x00436cd0 _$E110
// FUNCTION: STBC 0x00436ce0 _$E381
// FUNCTION: STBC 0x00436cf0 _$E380
// FUNCTION: STBC 0x00436d00 _$E116
// FUNCTION: STBC 0x00436d10 _$E115
// FUNCTION: STBC 0x00436d20 _$E395
// FUNCTION: STBC 0x00436d30 _$E394
// FUNCTION: STBC 0x00436d60 _$E399
// FUNCTION: STBC 0x00436d70 _$E398
// FUNCTION: STBC 0x00436d80 _$E298
// FUNCTION: STBC 0x00436d90 _$E297
// FUNCTION: STBC 0x00436da0 _$E403
// FUNCTION: STBC 0x00436db0 _$E402
// FUNCTION: STBC 0x00436dc0 _$E125
// FUNCTION: STBC 0x00436dd0 _$E124
// FUNCTION: STBC 0x00436de0 _$E407
// FUNCTION: STBC 0x00436df0 _$E406
// FUNCTION: STBC 0x00436e00 _$E411
// FUNCTION: STBC 0x00436e10 _$E410
// FUNCTION: STBC 0x00436e20 _$E302
// FUNCTION: STBC 0x00436e30 _$E301
// FUNCTION: STBC 0x00436e40 _$E131
// FUNCTION: STBC 0x00436e50 _$E130
// FUNCTION: STBC 0x00436e60 _$E136
// FUNCTION: STBC 0x00436e70 _$E135
// FUNCTION: STBC 0x00436e80 _$E166
// FUNCTION: STBC 0x00436e90 _$E165
// FUNCTION: STBC 0x00436ea0 _$E415
// FUNCTION: STBC 0x00436eb0 _$E414
// FUNCTION: STBC 0x00436ec0 _$E171
// FUNCTION: STBC 0x00436ed0 _$E170
// FUNCTION: STBC 0x00436ee0 _$E419
// FUNCTION: STBC 0x00436ef0 _$E418
// FUNCTION: STBC 0x00436f00 _$E423
// FUNCTION: STBC 0x00436f10 _$E422
// FUNCTION: STBC 0x00436f20 _$E176
// FUNCTION: STBC 0x00436f30 _$E175
// FUNCTION: STBC 0x00436f40 _$E427
// FUNCTION: STBC 0x00436f50 _$E426
// FUNCTION: STBC 0x00436f60 _$E141
// FUNCTION: STBC 0x00436f70 _$E140
// FUNCTION: STBC 0x00436f80 _$E431
// FUNCTION: STBC 0x00436f90 _$E430
// FUNCTION: STBC 0x00436fa0 _$E435
// FUNCTION: STBC 0x00436fb0 _$E434
// FUNCTION: STBC 0x00436ff0 _$E181
// FUNCTION: STBC 0x00437000 _$E180
// FUNCTION: STBC 0x00437010 _$E186
// FUNCTION: STBC 0x00437020 _$E185
// FUNCTION: STBC 0x00437030 _$E439
// FUNCTION: STBC 0x00437040 _$E438
// FUNCTION: STBC 0x00437050 _$E443
// FUNCTION: STBC 0x00437060 _$E442
// FUNCTION: STBC 0x00437070 _$E447
// FUNCTION: STBC 0x00437080 _$E446
// FUNCTION: STBC 0x00437090 _$E451
// FUNCTION: STBC 0x004370a0 _$E450
// FUNCTION: STBC 0x004370b0 _$E455
// FUNCTION: STBC 0x004370c0 _$E454
// FUNCTION: STBC 0x004370d0 _$E459
// FUNCTION: STBC 0x004370e0 _$E458
// FUNCTION: STBC 0x004370f0 _$E463
// FUNCTION: STBC 0x00437100 _$E462
// FUNCTION: STBC 0x00437110 _$E467
// FUNCTION: STBC 0x00437120 _$E466
// FUNCTION: STBC 0x00437130 _$E191
// FUNCTION: STBC 0x00437140 _$E190
// FUNCTION: STBC 0x00437150 _$E471
// FUNCTION: STBC 0x00437160 _$E470
// FUNCTION: STBC 0x00437170 _$E196
// FUNCTION: STBC 0x00437180 _$E195
// FUNCTION: STBC 0x00437190 _$E201
// FUNCTION: STBC 0x004371a0 _$E200
// FUNCTION: STBC 0x00437280 _$E475
// FUNCTION: STBC 0x00437290 _$E474
// FUNCTION: STBC 0x004372a0 _$E480
// FUNCTION: STBC 0x004372b0 _$E479
// FUNCTION: STBC 0x004372c0 _$E484
// FUNCTION: STBC 0x004372d0 _$E483

// DATA: STBC 0x008d97c4
int ObjectClass::maximumNumberOfLights = 8;

// FUNCTION: STBC 0x00434d00 _$E490
// FUNCTION: STBC 0x00434d10 _$E486
// FUNCTION: STBC 0x00434d20 _$E489
// FUNCTION: STBC 0x00434d30 _$E487
// DATA: STBC 0x0097f5b0
TGEventHandlerTable ObjectClass::classHandlers;

/*
================
ObjectClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00434d50
void ObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, OBJECTCLASS_TYPE );
		BaseObjectClass::AddToSet( set );
	}
}

/*
================
ObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00434d80
void ObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, OBJECTCLASS_TYPE );
		BaseObjectClass::RemoveFromSet( set );
	}
}

/*
================
ObjectClass::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x00434db0
ObjectClass **ObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (ObjectClass **)set->objectTable.GetObjects( OBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
ObjectClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x00434de0
int ObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( OBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
ObjectClass::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not an ObjectClass.
================
*/
// FUNCTION: STBC 0x00434e00
ObjectClass *ObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( OBJECTCLASS_TYPE ) ) {
			return (ObjectClass *)object;
		}
		return NULL;
	}
	return (ObjectClass *)set->GetClassObject( OBJECTCLASS_TYPE, id );
}

/*
================
ObjectClass::GetObject

The object with the given name in the set, or in the first set that has
one, searching from the last set, when the set is NULL.
================
*/
// FUNCTION: STBC 0x00434e70
ObjectClass *ObjectClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( OBJECTCLASS_TYPE, name_ );
		}
		return (ObjectClass *)object;
	}
	return (ObjectClass *)set->GetClassObject( OBJECTCLASS_TYPE, name_ );
}

/*
================
ObjectClass::Cast

The object as an ObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00434ed0
ObjectClass *ObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( OBJECTCLASS_TYPE ) ) {
		return (ObjectClass *)object;
	}
	return NULL;
}

/*
================
ObjectClass::ObjectClass

An object showing the given NetImmerse object.
================
*/
// FUNCTION: STBC 0x00434f00 ??0ObjectClass@@QAE@PAVNiAVObject@@@Z
ObjectClass::ObjectClass( NiAVObject *object ) : BaseObjectClass( object ) {
	InitObject();
}

/*
================
ObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x00434f60
int ObjectClass::GetObjType( void ) {
	return OBJECTCLASS_TYPE;
}

/*
================
ObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x00434fc0
char *ObjectClass::GetClassName( void ) {
	return "ObjectClass";
}

/*
================
ObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00434fd0
char *ObjectClass::GetSwigTypeName( void ) {
	return "_p_ObjectClass";
}

/*
================
ObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00434fe0
char *ObjectClass::GetPythonClassName( void ) {
	return "ObjectClassPtr";
}

/*
================
ObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00434ff0
TGEventHandlerTable *ObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ObjectClass::ObjectClass
================
*/
// FUNCTION: STBC 0x00435030 ??0ObjectClass@@QAE@J@Z
ObjectClass::ObjectClass( long id ) : BaseObjectClass( id ) {
	InitObject();
}

/*
================
ObjectClass::InitObject

What both constructors share: hailable and scannable, every collision
flag set.
================
*/
// FUNCTION: STBC 0x00435090
void ObjectClass::InitObject( void ) {
	collisionFlags = 0x7f;
	unknown3D = false;
	unknown40 = NiPoint3::ZERO;
	unknown4C = NiPoint3::ZERO;
	hailable = true;
	scannable = true;
	numReplacedTextures = 0;
	replacedTextures = NULL;
}

/*
================
ObjectClass::~ObjectClass
================
*/
// FUNCTION: STBC 0x004350e0
ObjectClass::~ObjectClass( void ) {
	if ( replacedTextures != NULL ) {
		for ( int i = 0; i < numReplacedTextures; i++ ) {
			g_kPoolManager.Unknown717960( replacedTextures[i] );
		}
		g_kPoolManager.Unknown717960( replacedTextures );
		replacedTextures = NULL;
	}
}

/*
================
ObjectClass::Create
================
*/
// FUNCTION: STBC 0x00435190
ObjectClass *ObjectClass::Create( TGAnimNode *animNode ) {
	return new ObjectClass( animNode );
}

/*
================
ObjectClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004351c0
void ObjectClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ObjectClass::EnteredSetHandler, "ObjectClass::EnteredSetHandler" );
}

/*
================
ObjectClass::RegisterHandlers

The class adds no handlers of its own.
================
*/
// FUNCTION: STBC 0x004351e0
void ObjectClass::RegisterHandlers( void ) {
}

/*
================
ObjectClass::Update
================
*/
// FUNCTION: STBC 0x004351f0
void ObjectClass::Update( float time ) {
	VirtualSlot58( &unknown40, &unknown4C );
	unknown3D = true;
	BaseObjectClass::Update( time );
}

/*
================
ObjectClass::PlaceObjectByName

Places the object where the placement object of its set with the name is.
================
*/
// FUNCTION: STBC 0x00435220
void ObjectClass::PlaceObjectByName( char *placementName ) {
	if ( containingSet != NULL ) {
		PlaceObject( PlacementObject::GetObject( containingSet, placementName ) );
	}
}

/*
================
ObjectClass::PlaceObject
================
*/
// FUNCTION: STBC 0x00435280
void ObjectClass::PlaceObject( PlacementObject *placement ) {
	if ( placement != NULL ) {
		SetTranslate( placement->GetWorldLocation() );
		SetMatrixRotation( placement->GetWorldRotation() );
	}
}

/*
================
ObjectClass::AlignToObject

Turns the object to face the way the other one faces.
================
*/
// FUNCTION: STBC 0x004352c0
void ObjectClass::AlignToObject( ObjectClass *object ) {
	NiPoint3 forward = object->GetWorldForward();
	NiPoint3 up = object->GetWorldUp();
	AlignToVectors( forward, up );
}

/*
================
ObjectClass::AddLight

Lets the light light the object, unless the object is already lit by the
most lights allowed. The count starts at the node's first effect: when
that slot is empty, the effects after it are not counted.
================
*/
// FUNCTION: STBC 0x00435310
void ObjectClass::AddLight( NiDynamicEffect *light ) {
	if ( light != NULL ) {
		NiNode *objectNode = node;
		if ( objectNode != NULL ) {
			const NiDynamicEffectList &effects = objectNode->GetEffectList();
			unsigned int count = 0;
			if ( effects.item != NULL ) {
				count = 1;
				for ( NiDynamicEffectList *list = effects.next; list != NULL; list = list->next ) {
					if ( list->item != NULL ) {
						count++;
					}
				}
			}
			if ( count < maximumNumberOfLights ) {
				light->AttachAffectedNode( objectNode );
				objectNode->UpdateEffects();
			}
		}
	}
}

/*
================
ObjectClass::RemoveLight
================
*/
// FUNCTION: STBC 0x00435370
void ObjectClass::RemoveLight( NiDynamicEffect *light ) {
	if ( light != NULL ) {
		NiNode *objectNode = node;
		if ( objectNode != NULL && IsLitBy( light ) ) {
			light->DetachAffectedNode( objectNode );
			objectNode->UpdateEffects();
		}
	}
}

/*
================
ObjectClass::IsLitBy

True when the light is in the node's effect list: the list's embedded head
first, then its links.
================
*/
// FUNCTION: STBC 0x004353b0
bool ObjectClass::IsLitBy( NiDynamicEffect *light ) {
	if ( light == NULL ) {
		return false;
	}
	if ( node == NULL ) {
		return false;
	}
	const NiDynamicEffectList *list = &node->GetEffectList();
	if ( list->item != light ) {
		for ( list = list->next; list != NULL; list = list->next ) {
			if ( list->item == light ) {
				return true;
			}
		}
		return false;
	}
	return true;
}

/*
================
ObjectClass::GetRelativePositionInfo

A tuple of the point relative to the object: the offset from the object
to it, its distance, the direction to it and the angle between that
direction and the object's forward axis. The two vectors are new
TGPoint3 shadows that own their points.
================
*/
// FUNCTION: STBC 0x004353f0
// PARKED: schedule: the angle argument's fstp [esp] waits on the add esp, 0x14 at c13 (+2: ready c15), so item 2's store issues first at c14; retail issues the fstp at c14, one cycle earlier than any input order of these statements gives
PyObject *ObjectClass::GetRelativePositionInfo( TGPoint3 *point ) {
	NiPoint3 *offset = new NiPoint3( *point - GetWorldLocation() );
	NiPoint3 *direction = new NiPoint3( *offset );
	double distance = direction->Unitize();
	float angle = NiACos( GetWorldForward().Dot( *direction ) );

	PyObject *tuple = PyTuple_New( 4 );
	PyObject *offsetObject = PyEmbed::Unknown6F9920( offset, "TGPoint3" );
	PyObject *directionObject = PyEmbed::Unknown6F9920( direction, "TGPoint3" );
	PyTuple_SET_ITEM( tuple, 0, offsetObject );
	PyTuple_SET_ITEM( tuple, 1, PyFloat_FromDouble( distance ) );
	PyTuple_SET_ITEM( tuple, 2, directionObject );
	PyTuple_SET_ITEM( tuple, 3, PyFloat_FromDouble( angle ) );
	PyEmbed::SetAttribute( offsetObject, "thisown", "i", 1 );
	PyEmbed::SetAttribute( directionObject, "thisown", "i", 1 );
	return tuple;
}

/*
================
ObjectClass_GetRelativePositionInfo

The Python method ObjectClass.GetRelativePositionInfo( object, point ).
================
*/
// FUNCTION: STBC 0x004355e0
PyObject *ObjectClass_GetRelativePositionInfo( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *pointInstance = NULL;
	if ( PyArg_ParseTuple( args, "OO", &instance, &pointInstance ) != -1 ) {
		TGPoint3 *point = NULL;
		PyEmbed::Unknown6F98E0( pointInstance, (void **)&point, "TGPoint3 *" );
		ObjectClass *object;
		PyEmbed::Unknown6F98E0( instance, (void **)&object, "ObjectClass *" );
		return object->GetRelativePositionInfo( point );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

// Declared ahead of their definitions: retail's order (VirtualSlot59 at 0x435670 calls GetRandomVertex at
// 0x435B10, which calls CountVertices and FindVertex after it) shows the original declared them before
// their callers.
bool	GetRandomVertex( NiAVObject *object, NiGeometry **geometry, NiPoint3 **vertex, NiPoint3 **normal );
int		CountVertices( NiAVObject *object );

/*
================
ObjectClass::VirtualSlot59
================
*/
// FUNCTION: STBC 0x00435670
bool ObjectClass::VirtualSlot59( NiPoint3 **vertex, NiPoint3 **normal, NiGeometry **geometry ) {
	if ( niObject == NULL ) {
		return false;
	}
	return GetRandomVertex( niObject, geometry, vertex, normal ) ? true : false;
}

/*
================
ObjectClass::GetRandomPointOnModel

A new node attached under the object's node at a random vertex of the
model, scaled as the vertex's geometry and turned so that its first row is
the vertex normal, when there is one. The caller owns the node.
================
*/
// FUNCTION: STBC 0x004356a0
// PARKED: complete; frame: calls and blocks agree (Perpendicular with UnitCross out of line); retail's frame is 0xC smaller (0x54): it shares one NiPoint3 temporary's slot that this build gives its own, and copies the normal's product from its temporary where this build copies through the returned pointer
NiAVObject *ObjectClass::GetRandomPointOnModel( void ) {
	NiNode *point = new NiNode;
	node->AttachChild( point, true );

	NiPoint3 *vertex = NULL;
	NiPoint3 *normal = NULL;
	NiGeometry *geometry = NULL;
	if ( VirtualSlot59( &vertex, &normal, &geometry ) ) {
		NiPoint3 translate = TGModelUtils::LocalToLocalPoint( geometry, node, *vertex );
		point->SetTranslate( translate );
		float scale = geometry->GetWorldScale() / node->GetWorldScale();
		if ( scale <= 0.0f ) {
			scale = -scale;
		}
		point->SetScale( scale );

		if ( normal != NULL ) {
			*normal = ( geometry->GetWorldRotate() * *normal ) * node->GetWorldRotate();

			NiMatrix3 rotate;
			rotate.SetRow( 0, *normal );

			NiPoint3 side = normal->Perpendicular();
			rotate.SetRow( 1, side );
			rotate.SetRow( 2, side.Cross( *normal ) );
			point->SetRotate( rotate );
		}
		point->Update( 0.0f );
	}
	return point;
}

/*
================
ObjectClass::GetLocalRandomPointAndNormalOnModel

A random vertex of the model and its normal, in the node's frame; the
origin and the X axis when the model has none.
================
*/
// FUNCTION: STBC 0x004359d0 ?GetLocalRandomPointAndNormalOnModel@ObjectClass@@UAEXPAVNiPoint3@@0@Z
void ObjectClass::GetLocalRandomPointAndNormalOnModel( NiPoint3 *point, NiPoint3 *normal ) {
	NiPoint3 *vertex = NULL;
	NiPoint3 *vertexNormal = NULL;
	NiGeometry *geometry = NULL;
	if ( VirtualSlot59( &vertex, &vertexNormal, &geometry ) ) {
		*point = TGModelUtils::LocalToLocalPoint( geometry, node, *vertex );
		*normal = TGModelUtils::LocalToLocalUnitVector( geometry, node, *vertexNormal );
	} else {
		*point = NiPoint3::ZERO;
		*normal = NiPoint3::UNIT_X;
	}
}

/*
================
GetRandomVertex

A random vertex of the tree, as FindVertex gives it; false when the tree
has no vertices (invented name).
================
*/
// FUNCTION: STBC 0x00435b10
bool GetRandomVertex( NiAVObject *object, NiGeometry **geometry, NiPoint3 **vertex, NiPoint3 **normal ) {
	int count = CountVertices( object );
	if ( count == 0 ) {
		return false;
	}
	int index = g_kSystemWrapper.GetRandomNumber( count );
	FindVertex( &index, object, geometry, vertex, normal );
	return true;
}

/*
================
CountVertices

The vertices of the geometry in the tree; a LOD node counts its first
level only (invented name).
================
*/
// FUNCTION: STBC 0x00435b60
int CountVertices( NiAVObject *object ) {
	int count = 0;

	NiGeometry *geometry = NiDynamicCast( NiGeometry, object );
	if ( geometry != NULL ) {
		count = geometry->GetVertexCount();
	} else {
		// a LOD node: its first level only
		NiLODNode *lod = NiDynamicCast( NiLODNode, object );
		if ( lod != NULL ) {
			if ( lod->GetArrayCount() != 0 ) {
				count = CountVertices( lod->GetAt( 0 ) );
			}
		} else {
			NiNode *node = NiDynamicCast( NiNode, object );
			if ( node != NULL ) {
				for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
					count += CountVertices( node->GetAt( i ) );
				}
			}
		}
	}
	return count;
}

/*
================
FindVertex

Finds the vertex *index counts to, in CountVertices' order: its position,
its normal when normal is not NULL (NULL when the data has none) and its
geometry. Not found, *index is left counting the vertices still to skip
(invented name).
================
*/
// FUNCTION: STBC 0x00435c50
bool FindVertex( int *index, NiAVObject *object, NiGeometry **geometry, NiPoint3 **vertex, NiPoint3 **normal ) {
	NiGeometry *shape = NiDynamicCast( NiGeometry, object );
	if ( shape != NULL ) {
		NiGeometryData *data = shape->modelData;
		if ( *index < data->GetVertexCount() ) {
			*vertex = &data->GetVertices()[*index];
			if ( normal != NULL ) {
				NiPoint3 *normals = shape->modelData->GetNormals();
				if ( normals != NULL ) {
					*normal = &normals[*index];
				} else {
					*normal = NULL;
				}
			}
			*geometry = shape;
			return true;
		}
		*index -= data->GetVertexCount();
		return false;
	}

	// a LOD node: its first level only
	NiLODNode *lod = NiDynamicCast( NiLODNode, object );
	if ( lod != NULL ) {
		if ( lod->GetArrayCount() != 0 ) {
			return FindVertex( index, lod->GetAt( 0 ), geometry, vertex, normal );
		}
		return false;
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			if ( FindVertex( index, node->GetAt( i ), geometry, vertex, normal ) == true ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
ObjectClass::VirtualSlot62

Whether the ray hits the object: the collision model's test when it has
one, else a pick through the model.
================
*/
// FUNCTION: STBC 0x00435dc0
bool ObjectClass::VirtualSlot62( NiPoint3 *origin, const NiPoint3 &direction, NiPick *pick ) {
	if ( unknown58 != NULL ) {
		return unknown58->FindSegmentIntersections( *origin, direction, *pick );
	}
	pick->SetTarget( GetNiObject() );
	pick->PickObjects( *origin, direction, false );
	return pick->GetResults().GetEffectiveSize() > 0;
}

/*
================
ObjectClass::VirtualSlot63

Picks along the ray through the collision model when there is one, else
through the model; true when something was hit.
================
*/
// FUNCTION: STBC 0x00435e40
bool ObjectClass::VirtualSlot63( const NiPoint3 *origin, const NiPoint3 &direction, NiPick *pick ) {
	if ( unknown58 != NULL ) {
		return unknown58->FindRayIntersections( *origin, direction, *pick );
	}
	pick->SetTarget( GetNiObject() );
	pick->PickObjects( *origin, direction, false );
	return pick->GetResults().GetEffectiveSize() > 0;
}

/*
================
ObjectClass::LineCollides

Whether the segment between the points hits the object.
================
*/
// FUNCTION: STBC 0x00435ec0
bool ObjectClass::LineCollides( TGPoint3 *start, TGPoint3 *end ) {
	NiPick pick;
	return VirtualSlot62( start, *end - *start, &pick );
}

/*
================
ObjectClass::Unknown435F50

Sets the collision model the ray tests use. Nothing in retail calls it: the
linker kept it out of line with no references.
================
*/
// FUNCTION: STBC 0x00435f50
void ObjectClass::Unknown435F50( OBBModel *model ) {
	unknown58 = model;
}

/*
================
ObjectClass::ReplaceTexture

Gives the model the image of the new texture file wherever a texture's
image has the old name in its file name (the scripts pass a ship's
texture and "ID"). When record is set, the pair is kept, the new file
first, so that RefreshReplacedTextures can do it again.
================
*/
// FUNCTION: STBC 0x00435f90
void ObjectClass::ReplaceTexture( char *newTexture, char *oldTexture, bool record ) {
	NiImage *image = g_kImageManager.GetImage( newTexture, true, true );
	if ( image == NULL ) {
		return;
	}
	ReplaceTextureImages( niObject, image, oldTexture );
	if ( record == true ) {
		int newLength = strlen( newTexture ) + 1;
		int oldLength = strlen( oldTexture ) + 1;
		char *textures = (char *)malloc( newLength + oldLength );
		strcpy( textures, newTexture );
		strcpy( textures + newLength, oldTexture );
		numReplacedTextures++;
		replacedTextures = (char **)realloc( replacedTextures, numReplacedTextures * sizeof( char * ) );
		replacedTextures[numReplacedTextures - 1] = textures;
	}
}

/*
================
ObjectClass::RefreshReplacedTextures

Replaces the textures again, each by the new one of its block.
================
*/
// FUNCTION: STBC 0x00436080
void ObjectClass::RefreshReplacedTextures( void ) {
	for ( int i = 0; i < numReplacedTextures; i++ ) {
		char *newTexture = replacedTextures[i];
		ReplaceTexture( newTexture, newTexture + strlen( newTexture ) + 1, false );
	}
}

/*
================
ObjectClass::VirtualSlot58

The corners of the box around the world bound: the center less and plus
the radius on every axis.
================
*/
// FUNCTION: STBC 0x004360c0
void ObjectClass::VirtualSlot58( NiPoint3 *point1, NiPoint3 *point2 ) {
	NiBound &bound = GetWorldBound();
	NiPoint3 extent( bound.radius, bound.radius, bound.radius );
	*point1 = bound.center - extent;
	*point2 = bound.center + extent;
}

/*
================
ObjectClass::GetExtents

The corners of the object's box in the world: slot 58's points, widened
by the box at 0x40 and 0x4C when unknown3D is set.
================
*/
// FUNCTION: STBC 0x00436130
void ObjectClass::GetExtents( NiPoint3 *minimum, NiPoint3 *maximum ) {
	VirtualSlot58( minimum, maximum );
	if ( unknown3D ) {
		if ( minimum->x > unknown40.x ) {
			minimum->x = unknown40.x;
		}
		if ( minimum->y > unknown40.y ) {
			minimum->y = unknown40.y;
		}
		if ( minimum->z > unknown40.z ) {
			minimum->z = unknown40.z;
		}
		if ( maximum->x < unknown4C.x ) {
			maximum->x = unknown4C.x;
		}
		if ( maximum->y < unknown4C.y ) {
			maximum->y = unknown4C.y;
		}
		if ( maximum->z < unknown4C.z ) {
			maximum->z = unknown4C.z;
		}
	}
}

/*
================
ObjectClass::GetAffiliationColor

The radar colour of the group of the current mission that names the
object: friendly, enemy or neutral; the unknown colour without a name, a
mission or a group that names it.
================
*/
// FUNCTION: STBC 0x00436250
NiColorA *ObjectClass::GetAffiliationColor( void ) {
	if ( GetName() != NULL ) {
		Game *game = Game::GetCurrentGame();
		if ( game != NULL ) {
			Episode *episode = game->GetCurrentEpisode();
			if ( episode != NULL ) {
				Mission *mission = episode->GetCurrentMission();
				if ( mission != NULL ) {
					ObjectGroup *friendlyGroup = NULL;
					PyEmbed::Unknown6F98E0( mission->GetFriendlyGroup(), (void **)&friendlyGroup, "ObjectGroup" );
					ObjectGroup *enemyGroup = NULL;
					PyEmbed::Unknown6F98E0( mission->GetEnemyGroup(), (void **)&enemyGroup, "ObjectGroup" );
					ObjectGroup *neutralGroup = NULL;
					PyEmbed::Unknown6F98E0( mission->GetNeutralGroup(), (void **)&neutralGroup, "ObjectGroup" );
					if ( friendlyGroup->IsNameInGroup( GetName() ) ) {
						return &g_kRadarFriendlyColor;
					}
					if ( enemyGroup->IsNameInGroup( GetName() ) ) {
						return &g_kRadarEnemyColor;
					}
					if ( neutralGroup->IsNameInGroup( GetName() ) ) {
						return &g_kRadarNeutralColor;
					}
				}
			}
		}
	}
	return &g_kRadarUnknownColor;
}

/*
================
ObjectClass::EnteredSetHandler
================
*/
// FUNCTION: STBC 0x00436340
void ObjectClass::EnteredSetHandler( TGEvent *event ) {
	CallNextHandler( event );
}

/*
================
ObjectClass::RandomOrientation

Turns the object by a random angle about a random axis.
================
*/
// FUNCTION: STBC 0x00436350
void ObjectClass::RandomOrientation( void ) {
	float angle = rand() * NI_TWO_PI * ( 1.0f / RAND_MAX );
	// BUG: the offset to -1000..999 is converted as an unsigned number, so the components below 0 come out as about 4.29e9 rather than negative
	NiPoint3 axis;
	axis.x = (float)(unsigned int)( g_kSystemWrapper.GetRandomNumber( 2000 ) - 1000 );
	axis.y = (float)(unsigned int)( g_kSystemWrapper.GetRandomNumber( 2000 ) - 1000 );
	axis.z = (float)(unsigned int)( g_kSystemWrapper.GetRandomNumber( 2000 ) - 1000 );
	axis.Unitize();
	NiMatrix3 turn;
	turn.MakeRotation( angle, axis );
	NiMatrix3 current( GetRotation() );
	SetMatrixRotation( turn * current );
}

/*
================
ObjectClass::SetHailable

Unless a game is loading, tells the object's handlers with a ET_HAILABLE_CHANGE
event. The event of a plain ObjectClass is not saved.
================
*/
// FUNCTION: STBC 0x004364a0
void ObjectClass::SetHailable( bool hailable_ ) {
	hailable = hailable_;
	if ( !g_kUtopiaModule.IsLoading() ) {
		TGBoolEvent *event = new TGBoolEvent;
		event->SetDestination( this );
		event->SetSource( this );
		event->SetEventType( ET_HAILABLE_CHANGE );
		event->SetBool( hailable_ );
		char *className = GetClassName();
		if ( strcmp( className, "ObjectClass" ) == 0 ) {
			event->SetNotSaved( true );
		}
		g_kEventManager.AddEvent( event );
	}
}

/*
================
ObjectClass::SetScannable

Unless a game is loading, tells the object's handlers with a ET_SCANNABLE_CHANGE
event. The event of a plain ObjectClass is not saved.
================
*/
// FUNCTION: STBC 0x00436570
void ObjectClass::SetScannable( bool scannable_ ) {
	scannable = scannable_;
	if ( !g_kUtopiaModule.IsLoading() ) {
		TGBoolEvent *event = new TGBoolEvent;
		event->SetDestination( this );
		event->SetSource( this );
		event->SetEventType( ET_SCANNABLE_CHANGE );
		event->SetBool( scannable_ );
		char *className = GetClassName();
		if ( strcmp( className, "ObjectClass" ) == 0 ) {
			event->SetNotSaved( true );
		}
		g_kEventManager.AddEvent( event );
	}
}
/*
================
ObjectClass::WriteToStream

The collision flags, the two points of slot 58, the flags that say whether
the object can be hailed and scanned, and the replaced textures.
================
*/
// FUNCTION: STBC 0x00436640
bool ObjectClass::WriteToStream( TGStream *stream ) {
	if ( !BaseObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteChar( collisionFlags );
	stream->WriteBool( unknown3D );
	stream->WriteFloat( unknown40.x );
	stream->WriteFloat( unknown40.y );
	stream->WriteFloat( unknown40.z );
	stream->WriteFloat( unknown4C.x );
	stream->WriteFloat( unknown4C.y );
	stream->WriteFloat( unknown4C.z );
	stream->WriteBool( hailable );
	stream->WriteBool( scannable );
	VirtualSlot64( stream );
	return true;
}

/*
================
ObjectClass::VirtualSlot64

Writes the replaced textures: the count, then for each the length of its
block and the block.
================
*/
// FUNCTION: STBC 0x004366e0 ?VirtualSlot64@ObjectClass@@UAE_NPAVTGStream@@@Z
bool ObjectClass::VirtualSlot64( TGStream *stream ) {
	stream->WriteInt( numReplacedTextures );
	for ( int i = 0; i < numReplacedTextures; i++ ) {
		char *textures = replacedTextures[i];
		int length = strlen( textures ) + 1;
		length += strlen( textures + length ) + 1;
		stream->WriteLong( length );
		stream->Write( textures, length );
	}
	return true;
}

/*
================
ObjectClass::ReadFromStream
================
*/
// FUNCTION: STBC 0x00436760
bool ObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !BaseObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	collisionFlags = stream->ReadChar();
	unknown3D = stream->ReadBool();
	unknown40.x = stream->ReadFloat();
	unknown40.y = stream->ReadFloat();
	unknown40.z = stream->ReadFloat();
	unknown4C.x = stream->ReadFloat();
	unknown4C.y = stream->ReadFloat();
	unknown4C.z = stream->ReadFloat();
	hailable = stream->ReadBool();
	scannable = stream->ReadBool();
	VirtualSlot65( stream );
	return true;
}

/*
================
ObjectClass::VirtualSlot65

Reads the replaced textures VirtualSlot64 writes.
================
*/
// FUNCTION: STBC 0x004367f0 ?VirtualSlot65@ObjectClass@@UAE_NPAVTGStream@@@Z
bool ObjectClass::VirtualSlot65( TGStream *stream ) {
	numReplacedTextures = stream->ReadInt();
	if ( numReplacedTextures > 0 ) {
		replacedTextures = (char **)malloc( numReplacedTextures * sizeof( char * ) );
	}
	for ( int i = 0; i < numReplacedTextures; i++ ) {
		int length = stream->ReadLong();
		replacedTextures[i] = (char *)malloc( length );
		stream->Read( replacedTextures[i], length );
	}
	return true;
}

/*
================
ObjectClass::PostFixUp
================
*/
// FUNCTION: STBC 0x00436880
void ObjectClass::PostFixUp( void ) {
	BaseObjectClass::PostFixUp();
	RefreshReplacedTextures();
}

