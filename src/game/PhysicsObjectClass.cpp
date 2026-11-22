/*
===============================================================================

	PhysicsObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include "PhysicsObjectClass.h"
#include "Force.h"
#include "AIManager.h"
#include "ArtificialIntelligence.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGEvent.h"
#include "TGIntEvent.h"
#include "TGMatrix3.h"
#include "TGBufferStream.h"
#include "TGModelManager.h"
#include "TGAnimNode.h"
#include "NiNode.h"
#include "NiQuaternion.h"
#include "NiMatrix3.h"
#include <string.h>
#include "TGStream.h"
#include "UtopiaModule.h"
#include "UnknownClass6A8C50.h"
#include "Game.h"
#include "MainWindow.h"
#include "MultiplayerGame.h"
#include "MultiplayerPlayer.h"
#include "MultiplayerWindow.h"
#include "ShipClass.h"
#include "TopWindow.h"
#include "TGMessage.h"
#include "NetworkAI.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005a3a70 _$E497
// FUNCTION: STBC 0x005a3a90 _$E496

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0059fa80 _$E32
// FUNCTION: STBC 0x0059fa90 _$E29
// FUNCTION: STBC 0x0059faa0 _$E31
// FUNCTION: STBC 0x0059fab0 _$E30
// FUNCTION: STBC 0x0059fac0 _$E38
// FUNCTION: STBC 0x0059fad0 _$E35
// FUNCTION: STBC 0x0059fae0 _$E37
// FUNCTION: STBC 0x0059faf0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0059fb00 _$E66
// FUNCTION: STBC 0x0059fb10 _$E65
// FUNCTION: STBC 0x0059fb20 _$E240
// FUNCTION: STBC 0x0059fb30 _$E239
// FUNCTION: STBC 0x0059fb40 _$E77
// FUNCTION: STBC 0x0059fb50 _$E76
// FUNCTION: STBC 0x0059ff00 _$E81
// FUNCTION: STBC 0x0059ff10 _$E80
// FUNCTION: STBC 0x005a0430 _$E85
// FUNCTION: STBC 0x005a0440 _$E84
// FUNCTION: STBC 0x005a0770 _$E89
// FUNCTION: STBC 0x005a0780 _$E88
// FUNCTION: STBC 0x005a1130 _$E206
// FUNCTION: STBC 0x005a1140 _$E205
// FUNCTION: STBC 0x005a2bd0 _$E148
// FUNCTION: STBC 0x005a2be0 _$E147
// FUNCTION: STBC 0x005a3010 _$E161
// FUNCTION: STBC 0x005a3020 _$E160
// FUNCTION: STBC 0x005a3030 _$E211
// FUNCTION: STBC 0x005a3040 _$E210
// FUNCTION: STBC 0x005a3050 _$E245
// FUNCTION: STBC 0x005a3060 _$E244
// FUNCTION: STBC 0x005a3070 _$E215
// FUNCTION: STBC 0x005a3080 _$E214
// FUNCTION: STBC 0x005a3090 _$E249
// FUNCTION: STBC 0x005a30a0 _$E248
// FUNCTION: STBC 0x005a30b0 _$E256
// FUNCTION: STBC 0x005a30c0 _$E255
// FUNCTION: STBC 0x005a30d0 _$E333
// FUNCTION: STBC 0x005a30e0 _$E332
// FUNCTION: STBC 0x005a30f0 _$E337
// FUNCTION: STBC 0x005a3100 _$E336
// FUNCTION: STBC 0x005a3110 _$E106
// FUNCTION: STBC 0x005a3120 _$E105
// FUNCTION: STBC 0x005a3130 _$E290
// FUNCTION: STBC 0x005a3140 _$E289
// FUNCTION: STBC 0x005a3150 _$E297
// FUNCTION: STBC 0x005a3160 _$E296
// FUNCTION: STBC 0x005a3170 _$E301
// FUNCTION: STBC 0x005a3180 _$E300
// FUNCTION: STBC 0x005a3190 _$E305
// FUNCTION: STBC 0x005a31a0 _$E304
// FUNCTION: STBC 0x005a31b0 _$E309
// FUNCTION: STBC 0x005a31c0 _$E308
// FUNCTION: STBC 0x005a31d0 _$E315
// FUNCTION: STBC 0x005a31e0 _$E314
// FUNCTION: STBC 0x005a31f0 _$E341
// FUNCTION: STBC 0x005a3200 _$E340
// FUNCTION: STBC 0x005a3210 _$E345
// FUNCTION: STBC 0x005a3220 _$E344
// FUNCTION: STBC 0x005a3230 _$E349
// FUNCTION: STBC 0x005a3240 _$E348
// FUNCTION: STBC 0x005a3250 _$E353
// FUNCTION: STBC 0x005a3260 _$E352
// FUNCTION: STBC 0x005a3270 _$E357
// FUNCTION: STBC 0x005a3280 _$E356
// FUNCTION: STBC 0x005a3290 _$E361
// FUNCTION: STBC 0x005a32a0 _$E360
// FUNCTION: STBC 0x005a32b0 _$E365
// FUNCTION: STBC 0x005a32c0 _$E364
// FUNCTION: STBC 0x005a32d0 _$E369
// FUNCTION: STBC 0x005a32e0 _$E368
// FUNCTION: STBC 0x005a32f0 _$E373
// FUNCTION: STBC 0x005a3300 _$E372
// FUNCTION: STBC 0x005a3310 _$E377
// FUNCTION: STBC 0x005a3320 _$E376
// FUNCTION: STBC 0x005a3330 _$E111
// FUNCTION: STBC 0x005a3340 _$E110
// FUNCTION: STBC 0x005a3350 _$E381
// FUNCTION: STBC 0x005a3360 _$E380
// FUNCTION: STBC 0x005a3370 _$E116
// FUNCTION: STBC 0x005a3380 _$E115
// FUNCTION: STBC 0x005a3390 _$E385
// FUNCTION: STBC 0x005a33a0 _$E384
// FUNCTION: STBC 0x005a33b0 _$E389
// FUNCTION: STBC 0x005a33c0 _$E388
// FUNCTION: STBC 0x005a33d0 _$E393
// FUNCTION: STBC 0x005a33e0 _$E392
// FUNCTION: STBC 0x005a33f0 _$E397
// FUNCTION: STBC 0x005a3400 _$E396
// FUNCTION: STBC 0x005a3410 _$E125
// FUNCTION: STBC 0x005a3420 _$E124
// FUNCTION: STBC 0x005a3430 _$E401
// FUNCTION: STBC 0x005a3440 _$E400
// FUNCTION: STBC 0x005a3450 _$E405
// FUNCTION: STBC 0x005a3460 _$E404
// FUNCTION: STBC 0x005a3470 _$E409
// FUNCTION: STBC 0x005a3480 _$E408
// FUNCTION: STBC 0x005a3490 _$E131
// FUNCTION: STBC 0x005a34a0 _$E130
// FUNCTION: STBC 0x005a34b0 _$E136
// FUNCTION: STBC 0x005a34c0 _$E135
// FUNCTION: STBC 0x005a34d0 _$E166
// FUNCTION: STBC 0x005a34e0 _$E165
// FUNCTION: STBC 0x005a34f0 _$E413
// FUNCTION: STBC 0x005a3500 _$E412
// FUNCTION: STBC 0x005a3510 _$E171
// FUNCTION: STBC 0x005a3520 _$E170
// FUNCTION: STBC 0x005a3530 _$E417
// FUNCTION: STBC 0x005a3540 _$E416
// FUNCTION: STBC 0x005a3550 _$E421
// FUNCTION: STBC 0x005a3560 _$E420
// FUNCTION: STBC 0x005a3570 _$E176
// FUNCTION: STBC 0x005a3580 _$E175
// FUNCTION: STBC 0x005a3590 _$E425
// FUNCTION: STBC 0x005a35a0 _$E424
// FUNCTION: STBC 0x005a35b0 _$E141
// FUNCTION: STBC 0x005a35c0 _$E140
// FUNCTION: STBC 0x005a35d0 _$E429
// FUNCTION: STBC 0x005a35e0 _$E428
// FUNCTION: STBC 0x005a35f0 _$E433
// FUNCTION: STBC 0x005a3600 _$E432
// FUNCTION: STBC 0x005a3610 _$E181
// FUNCTION: STBC 0x005a3620 _$E180
// FUNCTION: STBC 0x005a3630 _$E186
// FUNCTION: STBC 0x005a3640 _$E185
// FUNCTION: STBC 0x005a3650 _$E437
// FUNCTION: STBC 0x005a3660 _$E436
// FUNCTION: STBC 0x005a3670 _$E441
// FUNCTION: STBC 0x005a3680 _$E440
// FUNCTION: STBC 0x005a3690 _$E445
// FUNCTION: STBC 0x005a36a0 _$E444
// FUNCTION: STBC 0x005a36b0 _$E449
// FUNCTION: STBC 0x005a36c0 _$E448
// FUNCTION: STBC 0x005a36d0 _$E453
// FUNCTION: STBC 0x005a36e0 _$E452
// FUNCTION: STBC 0x005a3760 _$E457
// FUNCTION: STBC 0x005a3770 _$E456
// FUNCTION: STBC 0x005a37e0 _$E461
// FUNCTION: STBC 0x005a37f0 _$E460
// FUNCTION: STBC 0x005a3870 _$E465
// FUNCTION: STBC 0x005a3880 _$E464
// FUNCTION: STBC 0x005a3890 _$E191
// FUNCTION: STBC 0x005a38a0 _$E190
// FUNCTION: STBC 0x005a3910 _$E469
// FUNCTION: STBC 0x005a3920 _$E468
// FUNCTION: STBC 0x005a3930 _$E196
// FUNCTION: STBC 0x005a3940 _$E195
// FUNCTION: STBC 0x005a3950 _$E201
// FUNCTION: STBC 0x005a3960 _$E200
// FUNCTION: STBC 0x005a3aa0 _$E473
// FUNCTION: STBC 0x005a3ab0 _$E472
// FUNCTION: STBC 0x005a3ac0 _$E478
// FUNCTION: STBC 0x005a3ad0 _$E477
// FUNCTION: STBC 0x005a3ae0 _$E482
// FUNCTION: STBC 0x005a3af0 _$E481

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0059fb60 _$E488
// FUNCTION: STBC 0x0059fb70 _$E484
// FUNCTION: STBC 0x0059fb80 _$E487
// FUNCTION: STBC 0x0059fb90 _$E485
// DATA: STBC 0x0098e250
TGEventHandlerTable PhysicsObjectClass::classHandlers;

/*
================
PhysicsObjectClass::GetPredictedPosition
================
*/
// FUNCTION: STBC 0x0047f990
TGPoint3 PhysicsObjectClass::GetPredictedPosition( const NiPoint3 *position, const NiPoint3 *velocity, const NiPoint3 *acceleration, float time ) {
	return PredictPosition( NiPoint3( *position ), NiPoint3( *velocity ), NiPoint3( *acceleration ), time );
}

/*
================
PhysicsObjectClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x0059fbb0
void PhysicsObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, PHYSICSOBJECTCLASS_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
PhysicsObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x0059fbe0
void PhysicsObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, PHYSICSOBJECTCLASS_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
PhysicsObjectClass::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x0059fc10
PhysicsObjectClass **PhysicsObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (PhysicsObjectClass **)set->objectTable.GetObjects( PHYSICSOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
PhysicsObjectClass::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x0059fc40
int PhysicsObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( PHYSICSOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
PhysicsObjectClass::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a PhysicsObjectClass.
================
*/
// FUNCTION: STBC 0x0059fc60
PhysicsObjectClass *PhysicsObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
			return (PhysicsObjectClass *)object;
		}
		return NULL;
	}
	return (PhysicsObjectClass *)set->GetClassObject( PHYSICSOBJECTCLASS_TYPE, id );
}

/*
================
PhysicsObjectClass::GetObject

The object with the given name in the set, or in the first set that has
one, searching from the last set, when the set is NULL.
================
*/
// FUNCTION: STBC 0x0059fcd0
PhysicsObjectClass *PhysicsObjectClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( PHYSICSOBJECTCLASS_TYPE, name_ );
		}
		return (PhysicsObjectClass *)object;
	}
	return (PhysicsObjectClass *)set->GetClassObject( PHYSICSOBJECTCLASS_TYPE, name_ );
}

/*
================
PhysicsObjectClass::Cast

The object as a PhysicsObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0059fd30
PhysicsObjectClass *PhysicsObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
		return (PhysicsObjectClass *)object;
	}
	return NULL;
}

/*
================
PhysicsObjectClass::PhysicsObjectClass
================
*/
// FUNCTION: STBC 0x0059fd60 ??0PhysicsObjectClass@@QAE@J@Z
PhysicsObjectClass::PhysicsObjectClass( long id ) : ObjectClass( id ) {
	InitObject( id );
}

/*
================
PhysicsObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x0059fe50
int PhysicsObjectClass::GetObjType( void ) {
	return PHYSICSOBJECTCLASS_TYPE;
}

/*
================
PhysicsObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x0059fec0
char *PhysicsObjectClass::GetClassName( void ) {
	return "PhysicsObjectClass";
}

/*
================
PhysicsObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0059fed0
char *PhysicsObjectClass::GetSwigTypeName( void ) {
	return "_p_PhysicsObjectClass";
}

/*
================
PhysicsObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0059fee0
char *PhysicsObjectClass::GetPythonClassName( void ) {
	return "PhysicsObjectClassPtr";
}

/*
================
PhysicsObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0059fef0
TGEventHandlerTable *PhysicsObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
PhysicsObjectClass::PhysicsObjectClass

An object showing a NetImmerse object; it takes a new ID.
================
*/
// FUNCTION: STBC 0x0059ff50 ??0PhysicsObjectClass@@QAE@PAVNiAVObject@@@Z
PhysicsObjectClass::PhysicsObjectClass( NiAVObject *object ) : ObjectClass( object ) {
	InitObject( 0 );
}

/*
================
PhysicsObjectClass::InitObject

What both constructors share: at rest, with unit mass and inertia. In
multiplayer, an object created without an ID takes the next client ID,
unless the module says to ignore it; any other object's ID becomes the
module's next unused one. A new object announces itself, and in
multiplayer it also posts the notification.
================
*/
// FUNCTION: STBC 0x005a0040
void PhysicsObjectClass::InitObject( long id ) {
	isStatic = false;
	acceleration = NiPoint3::ZERO;
	mass = 1.0f;
	rotationalInertia = 1.0f;
	angularAxis.x = 1.0f;
	angularAxis.y = 0.0f;
	angularAxis.z = 0.0f;
	angularSpeed = 0.0f;
	angularDirectionType = 0;
	angularAcceleration = NiPoint3::ZERO;
	unknownC0 = NiPoint3::ZERO;
	unknownE4 = 0;
	netType = 0;
	usePhysics = true;
	doNetUpdate = true;

	if ( g_kUtopiaModule.IsMultiplayer() && id == 0 ) {
		if ( !g_kUtopiaModule.IsIgnoreClientIDForObjectCreation() ) {
			SetObjID( g_kUtopiaModule.GetUnusedClientID() );
			g_kUtopiaModule.IncrementClientID();
		}
	} else {
		g_kUtopiaModule.SetUnusedClientID( id );
		g_kUtopiaModule.IncrementClientID();
	}

	if ( id == 0 ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_OBJECT_CREATED );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
	}
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_OBJECT_CREATED_NOTIFY );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
	}

	unknown78 = 0;
	unknown7C = 0;
	unknown80 = 0;
	unknown84 = 0;
	unknown88 = 0;
	unknown8C = 0;
}

/*
================
PhysicsObjectClass::~PhysicsObjectClass

Deletes the values of the map at 0x68.
================
*/
// FUNCTION: STBC 0x005a0200
// FUNCTION: STBC 0x0059ff20 ??_GPhysicsObjectClass@@UAEPAXI@Z
PhysicsObjectClass::~PhysicsObjectClass( void ) {
	NiTMapIterator pos = unknown68.GetFirstPos();
	while ( pos != NULL ) {
		unsigned int key;
		UnknownClass6A8C50 *value;
		unknown68.GetNext( pos, key, value );
		delete value;
	}
}

/*
================
PhysicsObjectClass::SetupModel

Shows a clone of the named model under the object's node.
================
*/
// FUNCTION: STBC 0x005a03a0
void PhysicsObjectClass::SetupModel( char *modelName ) {
	if ( modelName != NULL ) {
		NiPointer<TGAnimNode> model = g_kModelManager.CloneModel( modelName );
		if ( model != NULL ) {
			node->AttachChild( model, false );
			SetNiObject( model );
		}
	}
}

/*
================
PhysicsObjectClass::ConstCast

The object as a PhysicsObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005a0450
PhysicsObjectClass *PhysicsObjectClass::ConstCast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
		return (PhysicsObjectClass *)object;
	}
	return NULL;
}

/*
================
PhysicsObjectClass::Create

A physics object showing the NetImmerse object.
================
*/
// FUNCTION: STBC 0x005a0480
PhysicsObjectClass *PhysicsObjectClass::Create( NiAVObject *object ) {
	return new PhysicsObjectClass( object );
}

/*
================
PhysicsObjectClass::SetVelocity

Sets the node's velocity; nothing without a node.
================
*/
// FUNCTION: STBC 0x005a04c0
void PhysicsObjectClass::SetVelocity( const NiPoint3 *velocity ) {
	if ( node != NULL ) {
		node->SetVelocity( *velocity );
	}
}

/*
================
PhysicsObjectClass::SetAngularVelocity

Splits the velocity into its axis and speed; the direction type says
whether the axis is in the world's frame or the object's.
================
*/
// FUNCTION: STBC 0x005a04f0
void PhysicsObjectClass::SetAngularVelocity( const NiPoint3 *velocity, int directionType ) {
	SetAngularDirectionType( directionType );
	angularSpeed = velocity->Length();
	if ( angularSpeed != 0.0f ) {
		angularAxis = *velocity / angularSpeed;
	} else {
		angularAxis = NiPoint3::ZERO;
	}
}

/*
================
PhysicsObjectClass::GetVelocity

The node's velocity; zero without a node.
================
*/
// FUNCTION: STBC 0x005a05a0
const NiPoint3 &PhysicsObjectClass::GetVelocity( void ) {
	if ( node != NULL ) {
		return node->GetVelocity();
	}
	return NiPoint3::ZERO;
}

/*
================
PhysicsObjectClass::Update

Moves the object by the time since its last update (a thousandth of a
second at least, a second at most) and writes the result to its node; a
static object is held still instead.
================
*/
// FUNCTION: STBC 0x005a05c0
void PhysicsObjectClass::Update( float time ) {
	if ( !isStatic ) {
		float elapsed;
		if ( lastUpdateTime < 0.0f ) {
			elapsed = 0.001f;
		} else {
			float delta = time - lastUpdateTime;
			if ( delta < 0.001f ) {
				delta = 0.001f;
			}
			elapsed = delta;
			if ( elapsed > 1.0f ) {
				elapsed = 1.0f;
			}
		}

		NiMatrix3 rotation = GetRotation();
		NiPoint3 position = GetTranslate();
		NiPoint3 velocity = GetVelocity();
		Unknown5A09D0( elapsed, &position, &velocity, &acceleration, &angularAxis, &angularSpeed, &angularAcceleration, &unknownC0, &unknownE4, &rotation );
		SetVelocity( &velocity );
		node->SetRotate( rotation );
		node->SetTranslate( position );
	} else {
		SetVelocity( &NiPoint3::ZERO );
		SetAcceleration( NiPoint3::ZERO );
		SetAngularVelocity( &NiPoint3::ZERO, 1 );
		SetAngularAcceleration( &NiPoint3::ZERO );
	}
	ObjectClass::Update( time );
}

/*
================
PhysicsObjectClass::UpdateNode
================
*/
// FUNCTION: STBC 0x005a0750
void PhysicsObjectClass::UpdateNode( float time ) {
	ObjectClass::UpdateNode( time );
}

/*
================
PhysicsObjectClass::SetContainingSet
================
*/
// FUNCTION: STBC 0x005a0760
void PhysicsObjectClass::SetContainingSet( SetClass *set ) {
	ObjectClass::SetContainingSet( set );
}

/*
================
PhysicsObjectClass::ApplyForce

The force, moved to act at the object's location, changes the velocity by
its push over the mass and the angular velocity by its torque over the
rotational inertia.
================
*/
// FUNCTION: STBC 0x005a0790
void PhysicsObjectClass::ApplyForce( Force *force ) {
	Force applied( *force );
	applied.MoveTo( GetWorldLocation() );
	// one vector carries the new velocity, then the new angular velocity:
	// retail keeps both in one frame slot, which two named locals never share
	NiPoint3 change = applied.GetForce() / mass + GetVelocity();
	SetVelocity( &change );
	change = applied.GetTorque();
	change /= -rotationalInertia;
	SetAngularDirectionType( 1 );
	change = change + angularSpeed * angularAxis;
	SetAngularVelocity( &change, 1 );
}

/*
================
PhysicsObjectClass::Unknown5A08F0
================
*/
// FUNCTION: STBC 0x005a08f0
void PhysicsObjectClass::Unknown5A08F0( NiPoint3 *position, NiPoint3 *velocity, NiPoint3 *acceleration_, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_, NiMatrix3 *rotation ) {
	*position = node->GetTranslate();
	*velocity = GetVelocity();
	*acceleration_ = acceleration;
	*angularAxis_ = angularAxis;
	*angularSpeed_ = angularSpeed;
	*angularAcceleration_ = angularAcceleration;
	*unknownC0_ = unknownC0;
	*unknownE4_ = unknownE4;
	*rotation = GetRotation();
}

/*
================
PhysicsObjectClass::Unknown5A09D0

The angular step is split at the end of the angular acceleration's
duration when it falls inside the time.
================
*/
// FUNCTION: STBC 0x005a09d0
void PhysicsObjectClass::Unknown5A09D0( float time, NiPoint3 *position, NiPoint3 *velocity, NiPoint3 *acceleration_, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_, NiMatrix3 *rotation ) {
	*position = PredictPosition( *position, *velocity, *acceleration_, time );
	NiPoint3 change = time * *acceleration_;
	velocity->x = change.x + velocity->x;
	velocity->y = change.y + velocity->y;
	velocity->z = change.z + velocity->z;
	if ( *unknownE4_ > 0.0f && *unknownE4_ < time ) {
		float first = *unknownE4_;
		Unknown5A0D80( first, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_, rotation );
		Unknown5A1240( first, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_ );
		Unknown5A0D80( time - first, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_, rotation );
		Unknown5A1240( time - first, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_ );
	} else {
		Unknown5A0D80( time, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_, rotation );
		Unknown5A1240( time, angularAxis_, angularSpeed_, angularAcceleration_, unknownC0_, unknownE4_ );
	}
}

/*
================
PhysicsObjectClass::PredictPosition

The position after time seconds of uniform acceleration:
position + velocity * time + acceleration * time^2 / 2. Uses none of the
object's own state.
================
*/
// FUNCTION: STBC 0x005a0b50
NiPoint3 PhysicsObjectClass::PredictPosition( const NiPoint3 &position, const NiPoint3 &velocity, const NiPoint3 &acceleration_, float time ) {
	return acceleration_ * 0.5f * time * time + time * velocity + position;
}

/*
================
PhysicsObjectClass::GetPredictedRotation

The rotation the angular motion turns the object to in the time, the
object left as it is.
================
*/
// FUNCTION: STBC 0x005a0c20
NiMatrix3 PhysicsObjectClass::GetPredictedRotation( float time ) {
	NiPoint3 axis = angularAxis;
	float speed = angularSpeed;
	NiPoint3 accel = angularAcceleration;
	NiPoint3 accelEnd = unknownC0;
	float duration = unknownE4;
	NiMatrix3 rotation = GetRotation();
	if ( duration > 0.0f && duration < time ) {
		float first = duration;
		Unknown5A0D80( duration, &axis, &speed, &accel, &accelEnd, &duration, &rotation );
		duration = 0.0f;
		Unknown5A0D80( time - first, &axis, &speed, &accel, &accelEnd, &duration, &rotation );
	} else {
		Unknown5A0D80( time, &axis, &speed, &accel, &accelEnd, &duration, &rotation );
	}
	return rotation;
}

// The third-order term's factor of the angular step.
static const float ONE_SIXTH = 1.0f / 6.0f;

/*
================
PhysicsObjectClass::Unknown5A0D80
================
*/
// FUNCTION: STBC 0x005a0d80
// PARKED: operand order: retail loads the time and divides it into 1.0 (fdivr); this build loads 1.0 first; / time, * ( 1.0f / time ) and ( 1.0f / time ) * ( ... ) compile alike
void PhysicsObjectClass::Unknown5A0D80( float time, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_, NiMatrix3 *rotation ) {
	NiPoint3 velocity = *angularSpeed_ * *angularAxis_;
	NiPoint3 start, end;
	Unknown5A1150( &start, &end, time, unknownE4_, angularAcceleration_, unknownC0_ );
	NiPoint3 half = start * 0.5f;
	NiPoint3 change = ( end - start ) / time;
	NiPoint3 turn = ( ( change * ONE_SIXTH * time + half ) * time + velocity ) * time;
	float angle = turn.Length();
	if ( angle != 0.0f ) {
		turn *= 1.0f / angle;
		NiMatrix3 step;
		step.MakeRotation( angle, turn );
		switch ( angularDirectionType ) {
			case 0:
				*rotation = *rotation * step;
				break;
			case 1: {
				TGMatrix3 local( step );
				*rotation = *rotation * ( GetWorldRotation().Inverse() * local * GetWorldRotation() );
				break;
			}
		}
	}
}

/*
================
PhysicsObjectClass::Unknown5A1150

The first point is start; the second is the blend of start and end that the
time reaches through the duration, or start when the duration is spent.
================
*/
// FUNCTION: STBC 0x005a1150
void PhysicsObjectClass::Unknown5A1150( NiPoint3 *startOut, NiPoint3 *blended, float time, float *duration, NiPoint3 *start, NiPoint3 *end ) {
	*startOut = *start;
	if ( *duration > 0.0f ) {
		float remaining = *duration - time;
		*blended = ( time * *end + remaining * *start ) / *duration;
	} else {
		*duration = 0.0f;
		*blended = *startOut;
	}
}

// Below this the angular acceleration's duration counts as spent.
static const double DURATION_EPSILON = 1e-16;

/*
================
PhysicsObjectClass::Unknown5A1240
================
*/
// FUNCTION: STBC 0x005a1240
// PARKED: schedule: in the out-of-line NiPoint3 constructor call of ( ... ) * time, retail multiplies the x argument after the lea of the temporary, this build before the push; the operand orders of both products change the inline set (94.0) or the tail
void PhysicsObjectClass::Unknown5A1240( float time, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_ ) {
	NiPoint3 velocity = *angularSpeed_ * *angularAxis_;
	NiPoint3 start, end;
	Unknown5A1150( &start, &end, time, unknownE4_, angularAcceleration_, unknownC0_ );
	NiPoint3 slope = ( end - start ) / ( time + time );
	NiPoint3 turned = ( slope * time + start ) * time + velocity;
	float speed = turned.Length();
	*angularSpeed_ = speed;
	if ( speed != 0.0f ) {
		turned *= 1.0f / speed;
	} else {
		turned = NiPoint3::ZERO;
	}
	*angularAxis_ = turned;
	*angularAcceleration_ = end;
	*unknownE4_ -= time;
	if ( *unknownE4_ < DURATION_EPSILON ) {
		*angularAcceleration_ = *unknownC0_ = NiPoint3::ZERO;
		*unknownE4_ = 0.0f;
	}
}

/*
================
PhysicsObjectClass::SetAngularAcceleration
================
*/
// FUNCTION: STBC 0x005a1430
void PhysicsObjectClass::SetAngularAcceleration( const NiPoint3 *acceleration_ ) {
	angularAcceleration = unknownC0 = *acceleration_;
	unknownE4 = 0.0f;
}

/*
================
PhysicsObjectClass::SetAngularAccelerationLinear
================
*/
// FUNCTION: STBC 0x005a1480
void PhysicsObjectClass::SetAngularAccelerationLinear( NiPoint3 *acceleration_, NiPoint3 *unknownC0_, float unknownE4_ ) {
	angularAcceleration = *acceleration_;
	unknownC0 = *unknownC0_;
	unknownE4 = unknownE4_;
}

/*
================
PhysicsObjectClass::SetAngularDirectionType

Carries the angular velocity's axis into the new frame.
================
*/
// FUNCTION: STBC 0x005a14d0
void PhysicsObjectClass::SetAngularDirectionType( int directionType ) {
	if ( angularDirectionType != directionType ) {
		if ( angularDirectionType == 0 ) {
			angularAxis = angularAxis * NiMatrix3( GetWorldRotation().Inverse() );
		} else {
			angularAxis = angularAxis * NiMatrix3( GetWorldRotation() );
		}
	}
	angularDirectionType = directionType;
}

/*
================
PhysicsObjectClass::SetAI

Gives the object the AI and returns the AI's ID; no AI clears the
object's AIs. AIs still building are dropped first. With unknown1 an AI
the object already has is kept, recorded under the new AI's ID;
otherwise it is cleared.
================
*/
// FUNCTION: STBC 0x005a15a0
int PhysicsObjectClass::SetAI( ArtificialIntelligence *ai, bool unknown1, bool unknown2 ) {
	if ( ai == NULL ) {
		ClearAI( true, NULL );
		return 0;
	}

	aiManager.RemoveBuildingAIs( this );
	ArtificialIntelligence *oldAI = GetAI();
	bool keepOldAI = false;
	if ( oldAI != NULL ) {
		if ( !unknown1 ) {
			ClearAI( true, NULL );
		} else {
			keepOldAI = true;
		}
	}

	long aiID = aiManager.AddAI( ai, false );
	if ( keepOldAI ) {
		unknownF4.SetAt( aiID, oldAI->GetID() );
	}
	if ( !unknown2 ) {
		aiManager.Unknown472A00( ai );
	}
	return aiID;
}

/*
================
PhysicsObjectClass::ClearAI

Removes the AI when it drives this object; without one, removes all the
object's AIs.
================
*/
// FUNCTION: STBC 0x005a16b0
void PhysicsObjectClass::ClearAI( bool unknown, ArtificialIntelligence *ai ) {
	if ( ai != NULL ) {
		if ( ai->GetObject() == this ) {
			aiManager.RemoveAI( ai, unknown );
		}
		return;
	}

	aiManager.RemoveBuildingAIs( this );
	for ( ai = aiManager.GetAI( this ); ai != NULL; ai = aiManager.GetAI( this ) ) {
		aiManager.RemoveAI( ai, unknown );
	}
}

/*
================
PhysicsObjectClass::GetAI
================
*/
// FUNCTION: STBC 0x005a1720
ArtificialIntelligence *PhysicsObjectClass::GetAI( void ) {
	return aiManager.GetAI( this );
}

/*
================
PhysicsObjectClass::HasBuildingAIs
================
*/
// FUNCTION: STBC 0x005a1730
bool PhysicsObjectClass::HasBuildingAIs( void ) {
	return aiManager.HasBuildingAIs( this );
}

/*
================
PhysicsObjectClass::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005a1740
void PhysicsObjectClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhysicsObjectClass::DeleteMeStage1, "PhysicsObjectClass::DeleteMeStage1" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhysicsObjectClass::ObjectDestroyedHandler, "PhysicsObjectClass::ObjectDestroyedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&PhysicsObjectClass::AIFinishedBuilding, "PhysicsObjectClass::AIFinishedBuilding" );
}

/*
================
PhysicsObjectClass::RegisterHandlers
================
*/
// FUNCTION: STBC 0x005a1780
void PhysicsObjectClass::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_DELETE_ME_STAGE_1, "PhysicsObjectClass::DeleteMeStage1" );
	AddMethodHandlerForClass( &classHandlers, ET_OBJECT_DESTROYED, "PhysicsObjectClass::ObjectDestroyedHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_AI_FINISHED_BUILDING, "PhysicsObjectClass::AIFinishedBuilding" );
}

/*
================
PhysicsObjectClass::DeleteMeStage1
================
*/
// FUNCTION: STBC 0x005a17c0
void PhysicsObjectClass::DeleteMeStage1( TGEvent *event ) {
	ClearAI( true, NULL );
	CallNextHandler( event );
}

/*
================
PhysicsObjectClass::ObjectDestroyedHandler
================
*/
// FUNCTION: STBC 0x005a17e0
void PhysicsObjectClass::ObjectDestroyedHandler( TGEvent *event ) {
	ClearAI( true, NULL );
	CallNextHandler( event );
}

/*
================
PhysicsObjectClass::AIFinishedBuilding

The AI the event names has finished building; the AI it replaced is
removed and deleted, and the replacement is no longer remembered.
================
*/
// FUNCTION: STBC 0x005a1800
void PhysicsObjectClass::AIFinishedBuilding( TGEvent *event ) {
	long replacedID;
	if ( unknownF4.GetAt( ( (TGIntEvent *)event )->GetInt(), replacedID ) ) {
		ArtificialIntelligence *replacedAI = ArtificialIntelligence::GetAIByID( replacedID );
		if ( replacedAI != NULL ) {
			aiManager.RemoveAI( replacedAI, true );
		}
		unknownF4.RemoveAt( ( (TGIntEvent *)event )->GetInt() );
	}
}

/*
================
PhysicsObjectClass::WriteToStream

Writes ObjectClass's state, then whether the object is static, its
velocity, its motion state and the map at 0xF4 (the count, then each key
and value).
================
*/
// FUNCTION: STBC 0x005a18f0
bool PhysicsObjectClass::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( isStatic );
	NiPoint3 velocity = GetVelocity();
	stream->WriteFloat( velocity.x );
	stream->WriteFloat( velocity.y );
	stream->WriteFloat( velocity.z );
	stream->WriteFloat( acceleration.x );
	stream->WriteFloat( acceleration.y );
	stream->WriteFloat( acceleration.z );
	stream->WriteFloat( angularAcceleration.x );
	stream->WriteFloat( angularAcceleration.y );
	stream->WriteFloat( angularAcceleration.z );
	stream->WriteFloat( unknownC0.x );
	stream->WriteFloat( unknownC0.y );
	stream->WriteFloat( unknownC0.z );
	stream->WriteFloat( angularAxis.x );
	stream->WriteFloat( angularAxis.y );
	stream->WriteFloat( angularAxis.z );
	stream->WriteFloat( mass );
	stream->WriteFloat( rotationalInertia );
	stream->WriteFloat( angularSpeed );
	stream->WriteFloat( unknownE4 );
	stream->WriteLong( angularDirectionType );

	stream->WriteInt( unknownF4.GetCount() );
	NiTMapIterator pos = unknownF4.GetFirstPos();
	while ( pos ) {
		long key;
		long value;
		unknownF4.GetNext( pos, key, value );
		stream->WriteLong( key );
		stream->WriteLong( value );
	}
	return true;
}

/*
================
PhysicsObjectClass::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x005a1af0
bool PhysicsObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	isStatic = stream->ReadBool();
	NiPoint3 velocity;
	velocity.x = stream->ReadFloat();
	velocity.y = stream->ReadFloat();
	velocity.z = stream->ReadFloat();
	SetVelocity( &velocity );
	acceleration.x = stream->ReadFloat();
	acceleration.y = stream->ReadFloat();
	acceleration.z = stream->ReadFloat();
	angularAcceleration.x = stream->ReadFloat();
	angularAcceleration.y = stream->ReadFloat();
	angularAcceleration.z = stream->ReadFloat();
	unknownC0.x = stream->ReadFloat();
	unknownC0.y = stream->ReadFloat();
	unknownC0.z = stream->ReadFloat();
	angularAxis.x = stream->ReadFloat();
	angularAxis.y = stream->ReadFloat();
	angularAxis.z = stream->ReadFloat();
	mass = stream->ReadFloat();
	rotationalInertia = stream->ReadFloat();
	angularSpeed = stream->ReadFloat();
	unknownE4 = stream->ReadFloat();
	angularDirectionType = stream->ReadLong();

	int count = stream->ReadInt();
	for ( int i = count - 1; i >= 0; i-- ) {
		long key = stream->ReadLong();
		long value = stream->ReadLong();
		unknownF4.SetAt( key, value );
	}
	return true;
}

/*
================
PhysicsObjectClass::VirtualSlot67

Writes the object's network header and state into the buffer and returns
how many bytes they took.
================
*/
// FUNCTION: STBC 0x005a1cf0
int PhysicsObjectClass::VirtualSlot67( char *buffer, int size ) {
	TGBufferStream stream;
	stream.VirtualSlot60( buffer, size );
	VirtualSlot68( &stream );
	VirtualSlot69( &stream );
	return stream.GetPos();
}

/*
================
PhysicsObjectClass::VirtualSlot68

The object's type, ID and net type, ahead of its state in a network
message.
================
*/
// FUNCTION: STBC 0x005a1d80
int PhysicsObjectClass::VirtualSlot68( TGStream *stream ) {
	stream->WriteLong( GetObjType() );
	stream->WriteLong( id );
	stream->WriteChar( GetNetType() );
	return stream->GetPos();
}

/*
================
PhysicsObjectClass::VirtualSlot69

The object's state in a network message: the position, the orientation as
a quaternion, the velocity, then the lengths and the characters of the
object's name and of its set's name. Returns the stream position.
================
*/
// FUNCTION: STBC 0x005a1dc0
int PhysicsObjectClass::VirtualSlot69( TGStream *stream ) {
	NiPoint3 position = GetTranslate();
	NiMatrix3 rotation;
	rotation = GetRotation();
	NiQuaternion orientation;
	orientation.FromRotation( rotation );
	NiPoint3 velocity = GetVelocity();

	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	stream->WriteFloat( orientation.w );
	stream->WriteFloat( orientation.GetX() );
	stream->WriteFloat( orientation.GetY() );
	stream->WriteFloat( orientation.GetZ() );
	stream->VirtualSlot36( velocity.x, velocity.y, velocity.z, false );

	char *objectName = GetName();
	if ( objectName == NULL ) {
		stream->WriteChar( 0 );
	} else {
		int length = strlen( objectName );
		stream->WriteChar( length );
		stream->Write( objectName, length );
	}
	if ( containingSet == NULL ) {
		stream->WriteChar( 0 );
	} else {
		char *setName = containingSet->GetName();
		if ( setName == NULL ) {
			stream->WriteChar( 0 );
		} else {
			int length = strlen( setName );
			stream->WriteChar( length );
			stream->Write( setName, length );
		}
	}
	return stream->GetPos();
}

/*
================
PhysicsObjectClass::CreateFromNetBuffer

Makes the object a network message describes (VirtualSlot67 wrote it):
the type and ID, then slots 70 and 71 read the rest. NULL when an object
of that ID exists already (invented name).
================
*/
// FUNCTION: STBC 0x005a1f50
PhysicsObjectClass *PhysicsObjectClass::CreateFromNetBuffer( char *buffer, int size ) {
	TGBufferStream stream;
	stream.VirtualSlot60( buffer, size );
	int type = stream.ReadLong();
	long id = stream.ReadLong();
	if ( BaseObjectClass::GetObjectByID( NULL, id ) != NULL ) {
		return NULL;
	}
	PhysicsObjectClass *object = (PhysicsObjectClass *)TGObject::CreateObject( type, id );
	object->VirtualSlot70( &stream );
	object->VirtualSlot71( &stream );
	return object;
}

/*
================
PhysicsObjectClass::VirtualSlot70

Reads the net type from a network message; returns the stream position.
================
*/
// FUNCTION: STBC 0x005a2030
int PhysicsObjectClass::VirtualSlot70( TGStream *stream ) {
	netType = stream->ReadChar();
	return stream->GetPos();
}

/*
================
PhysicsObjectClass::VirtualSlot71

Reads the state VirtualSlot69 wrote, and files the object in its set under
its name. An object of that name already in the set is renamed first.
Returns the stream position.
================
*/
// FUNCTION: STBC 0x005a2060
// PARKED: one instruction apart; retail delays the store of the last quaternion component until after the call arguments are set up
int PhysicsObjectClass::VirtualSlot71( TGStream *stream ) {
	NiPoint3 position;
	NiQuaternion orientation;
	NiMatrix3 rotation;
	NiPoint3 velocity;
	char objectName[256];
	char setName[256];
	char uniqueName[256];

	position.x = stream->ReadFloat();
	position.y = stream->ReadFloat();
	position.z = stream->ReadFloat();
	orientation.w = stream->ReadFloat();
	orientation.x = stream->ReadFloat();
	orientation.y = stream->ReadFloat();
	orientation.z = stream->ReadFloat();
	orientation.ToRotation( rotation );
	stream->VirtualSlot37( &velocity.x, &velocity.y, &velocity.z, false );

	SetTranslate( &position );
	unknown90 = position;
	unknown9C = position;
	SetMatrixRotation( rotation );
	SetVelocity( &velocity );
	UpdateNodeOnly();

	int nameLength = stream->ReadChar();
	if ( nameLength != 0 ) {
		stream->Read( objectName, nameLength );
		objectName[nameLength] = '\0';
	}
	int setNameLength = stream->ReadChar();
	if ( setNameLength != 0 ) {
		stream->Read( setName, setNameLength );
		setName[setNameLength] = '\0';
		SetClass *set = g_kSetManager.GetSet( setName );
		if ( set != NULL ) {
			if ( nameLength != 0 ) {
				ObjectClass *existing = set->GetObject( objectName );
				if ( existing != NULL ) {
					int suffix = 1;
					while ( true ) {
						sprintf( uniqueName, "%s - D%d", objectName, suffix );
						if ( set->GetObject( uniqueName ) == NULL ) {
							break;
						}
						suffix++;
					}
					existing->SetName( uniqueName );
				}
				set->AddObjectToSet( this, objectName );
			} else {
				set->AddObjectToSet( this, NULL );
			}
		}
	}
	return stream->GetPos();
}

/*
================
PhysicsObjectClass::VirtualSlot79

Whether an update of the object's state is due for the player: the
player's record (made on first use) keeps the game time of the last one.
The interval grows with the number of players over modem connections, and
farther objects (unless the player's ship targets this one or is it) wait
longer; the record's time is snapped to the interval.
================
*/
// FUNCTION: STBC 0x005a22a0
// PARKED: x87 and registers: retail compares the elapsed time through fld interval/fxch/fcompp where this build uses fcomp, and picks eax/edx the other way round at two loads
bool PhysicsObjectClass::VirtualSlot79( MultiplayerPlayer *player ) {
	if ( GetNetType() == 0 ) {
		return false;
	}

	UnknownClass6A8C50 *record;
	if ( !unknown68.GetAt( player->playerID, record ) ) {
		record = new UnknownClass6A8C50;
		unknown68.SetAt( player->playerID, record );
	}

	float now = g_kUtopiaModule.GetGameTime();
	float interval = 0.1f;
	bool crowded = false;
	MultiplayerWindow *window = (MultiplayerWindow *)TopWindow::topWindow->FindMainWindow( MWT_MULTIPLAYER );
	MultiplayerGame *game = (MultiplayerGame *)Game::GetCurrentGame();
	if ( game != NULL ) {
		int count = game->GetNumberPlayersInGame();
		if ( g_kUtopiaModule.IsHost() ) {
			if ( g_kUtopiaModule.IsModemConnection() ) {
				if ( !g_kUtopiaModule.IsClient() ) {
					if ( count <= 1 ) {
						interval = 0.1f;
					} else {
						interval = count <= 2 ? 0.167f : 0.5f;
						crowded = true;
					}
				} else {
					if ( count <= 2 ) {
						interval = 0.1f;
					} else {
						interval = count <= 3 ? 0.167f : 0.5f;
						crowded = true;
					}
				}
			} else if ( player->usingModem ) {
				interval = count <= 4 ? 0.1f : 0.125f;
			}
		} else if ( window->IsHostUsingModem() ) {
			if ( count <= 2 ) {
				interval = 0.1f;
			} else if ( count <= 3 ) {
				interval = 0.167f;
			} else {
				interval = 0.25f;
			}
		} else if ( g_kUtopiaModule.IsModemConnection() ) {
			interval = count <= 4 ? 0.1f : 0.125f;
		}
	}

	float elapsed = now - record->unknown00;
	if ( elapsed < interval ) {
		return false;
	}

	float limit;
	ShipClass *ship = ShipClass::GetObjectByID( NULL, player->shipID );
	if ( ship == NULL ) {
		limit = 1.0f;
	} else if ( ship->GetTarget() == this ) {
		limit = interval;
	} else if ( ship == this ) {
		limit = interval;
	} else {
		if ( ship->GetContainingSet() != GetContainingSet() ) {
			return false;
		}
		NiPoint3 shipLocation = ship->GetWorldLocation();
		NiPoint3 location = GetWorldLocation();
		NiPoint3 offset = shipLocation - location;
		float distance = offset.Length();
		if ( !window->IsHostUsingModem() && !( g_kUtopiaModule.IsHost() && g_kUtopiaModule.IsModemConnection() ) ) {
			if ( distance < 170.0f ) {
				limit = interval;
			} else if ( distance < 228.0f ) {
				limit = interval * 2.0f;
			} else if ( distance < 343.0f ) {
				limit = interval * 5.0f;
			} else {
				limit = 1.0f;
			}
		} else {
			if ( distance < 114.0f ) {
				limit = interval;
			} else if ( distance < 170.0f ) {
				limit = interval * 2.0f;
			} else {
				limit = 1.0f;
			}
		}
		if ( crowded ) {
			limit *= 1.33333f;
			if ( limit > 1.0f ) {
				limit = 1.0f;
			}
		}
	}
	if ( elapsed < limit ) {
		return false;
	}

	float rate = 1.0f / interval;
	record->unknown00 = (int)( rate * now ) / rate;
	return true;
}
/*
================
PhysicsObjectClass::VirtualSlot72

The object's state update for the player, when one is due (VirtualSlot79):
a SHIP_UPDATE_MESSAGE with the ID, the game time and a byte of flags, then
the parts that changed since the player's record last saw them. Every
second the position goes whole (flag 1); otherwise its offset from the
last whole one goes packed (flag 2). Then the packed forward and up axes
(4, 8) and the packed speed (0x10); the host's updates set 0x20. Every
second all of them go. NULL when nothing changed.
================
*/
// FUNCTION: STBC 0x005a26c0
// DRAFT: retail copies the game time through the FPU, loads the record's position onto the FPU before the offset's subtractions (an inline taking floats is missing) and lays the frame out 0xC larger
TGMessage *PhysicsObjectClass::VirtualSlot72( MultiplayerPlayer *player ) {
	if ( !VirtualSlot79( player ) ) {
		return NULL;
	}
	UnknownClass6A8C50 *record;
	if ( !unknown68.GetAt( player->playerID, record ) ) {
		return NULL;
	}

	float now = g_kUtopiaModule.GetGameTime();
	TGBufferStream stream;
	unsigned char flags = 0;
	bool everything = false;
	bool positionDue = false;
	bool notHost = false;
	if ( now - record->unknown04 > 1.0f ) {
		everything = true;
	}
	if ( now - record->unknown24 > 1.0f ) {
		positionDue = true;
		record->unknown24 = now;
	}
	if ( !g_kUtopiaModule.IsHost() ) {
		notHost = true;
	}

	NiPoint3 position = GetWorldLocation();
	NiPoint3 forward;
	forward = GetWorldForward();
	NiPoint3 up;
	up = GetWorldUp();
	float speed = GetVelocity().Length();

	NiPoint3 offset( position.x - record->unknown10, position.y - record->unknown14, position.z - record->unknown18 );
	char offsetX, offsetY, offsetZ;
	unsigned short offsetLength;
	stream.VirtualSlot41( offset.x, offset.y, offset.z, &offsetX, &offsetY, &offsetZ, &offsetLength );
	char forwardX, forwardY, forwardZ;
	stream.VirtualSlot42( forward.x, forward.y, forward.z, &forwardX, &forwardY, &forwardZ );
	char upX, upY, upZ;
	stream.VirtualSlot42( up.x, up.y, up.z, &upX, &upY, &upZ );

	if ( positionDue ) {
		flags = 1;
		record->unknown10 = position.x;
		record->unknown14 = position.y;
		record->unknown18 = position.z;
		record->unknown20 = 0;
		record->unknown21 = 0;
		record->unknown22 = 0;
		record->unknown1C = 0.0f;
	} else if ( offsetX != record->unknown20 || offsetY != record->unknown21 || offsetZ != record->unknown22 ||
				offsetLength != record->unknown1C || everything ) {
		record->unknown20 = offsetX;
		record->unknown21 = offsetY;
		record->unknown22 = offsetZ;
		flags = 2;
		record->unknown1C = offsetLength;
	}
	if ( forwardX != record->unknown28 || forwardY != record->unknown29 || forwardZ != record->unknown2A || everything ) {
		flags |= 4;
	}
	if ( upX != record->unknown2B || upY != record->unknown2C || upZ != record->unknown2D || everything ) {
		flags |= 8;
	}
	if ( speed != record->unknown0C || everything ) {
		flags |= 0x10;
	}
	if ( !notHost ) {
		flags |= 0x20;
	}
	if ( flags == 0 ) {
		return NULL;
	}
	if ( everything || ( ( flags & 4 ) && ( flags & 8 ) && ( flags & 0x10 ) && ( flags & 3 ) ) ) {
		record->unknown04 = now;
	}

	char buffer[1024];
	stream.VirtualSlot60( buffer, sizeof( buffer ) );
	stream.WriteChar( SHIP_UPDATE_MESSAGE );
	stream.WriteLong( id );
	stream.WriteFloat( now );
	stream.WriteChar( flags );
	if ( flags & 1 ) {
		stream.WriteFloat( position.x );
		stream.WriteFloat( position.y );
		stream.WriteFloat( position.z );
	}
	if ( flags & 2 ) {
		stream.VirtualSlot36( offset.x, offset.y, offset.z, true );
	}
	if ( flags & 4 ) {
		stream.VirtualSlot34( forward.x, forward.y, forward.z );
	}
	if ( flags & 8 ) {
		stream.VirtualSlot34( up.x, up.y, up.z );
	}
	if ( flags & 0x10 ) {
		stream.WriteShort( Unknown6D3A90( speed / 100.0f ) );
	}

	TGMessage *message = new TGMessage;
	message->SetData( buffer, stream.GetPos() );
	message->SetGuaranteed( false );
	return message;
}

/*
================
PhysicsObjectClass::VirtualSlot73

Reads a state update VirtualSlot72 made into the object's NetworkAI; each
part only when it is newer than the last of its kind. A whole position
restarts the dead reckoning from it; a packed offset moves it on. Without
a position for a second, the position is extrapolated from the velocity
and the acceleration.
================
*/
// FUNCTION: STBC 0x005a2bf0
// PARKED: schedule: the PredictPosition time argument's fstp [esp] ranks 0x44000 in ours (height 26 + the float-store bonus) and goes before the accel copy; retail's order needs 0x34000, the same store without the bonus
void PhysicsObjectClass::VirtualSlot73( TGMessage *message ) {
	NetworkAI *ai = NetworkAI::Cast( GetAI() );
	if ( ai == NULL ) {
		return;
	}
	int length;
	char *data = message->GetData( &length );
	TGBufferStream stream;
	stream.VirtualSlot60( data, length );
	stream.ReadChar();
	stream.ReadLong();
	float time = stream.ReadFloat();
	unsigned char flags = stream.ReadChar();

	float x, y, z;
	if ( flags & 1 ) {
		x = stream.ReadFloat();
		y = stream.ReadFloat();
		z = stream.ReadFloat();
		if ( time > unknown88 ) {
			unknown90.x = x;
			unknown90.y = y;
			unknown90.z = z;
			ai->position = unknown90;
			ai->stateTime = time;
			unknown88 = time;
			unknown78 = time;
			unknown8C = 0.0f;
			unknown9C.x = x;
			unknown9C.y = y;
			unknown9C.z = z;
		}
	}
	if ( time - unknown88 > 1.0f && time - unknown8C > 1.0f && unknown78 > 0.0f ) {
		NiPoint3 velocity = GetVelocity();
		NiPoint3 accel = acceleration;
		unknown90 = PredictPosition( unknown9C, velocity, accel, time - unknown78 );
		unknown9C = unknown90;
		unknown8C = time;
	}
	if ( flags & 2 ) {
		stream.VirtualSlot37( &x, &y, &z, true );
		if ( time > unknown78 ) {
			unknown9C.x = x + unknown9C.x;
			unknown9C.y = y + unknown9C.y;
			unknown9C.z = z + unknown9C.z;
			NiPoint3 position = unknown90;
			position.x += x;
			position.y += y;
			position.z += z;
			ai->position = position;
			ai->stateTime = time;
			unknown78 = time;
		}
	}
	if ( flags & 4 ) {
		stream.VirtualSlot35( &x, &y, &z );
		if ( time > unknown7C ) {
			ai->direction = NiPoint3( x, y, z );
			unknown7C = time;
		}
	}
	if ( flags & 8 ) {
		stream.VirtualSlot35( &x, &y, &z );
		if ( time > unknown80 ) {
			ai->up = NiPoint3( x, y, z );
			unknown80 = time;
		}
	}
	if ( flags & 0x10 ) {
		float speed = Unknown6D3B30( stream.ReadShort() ) * 100.0f;
		if ( time > unknown84 ) {
			ai->speed = speed;
			unknown84 = time;
		}
	}
}

