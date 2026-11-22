/*
===============================================================================

	Planet.cpp

===============================================================================
*/

#include "Planet.h"
#include "TGEventHandlerTable.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiAccumulator.h"
#include "TGEventManager.h"
#include "TGEvent.h"
#include "ShipClass.h"
#include "DamageableObject.h"
#include "ProximityEvent.h"
#include "ProximityCheck.h"
#include "ProximityManager.h"
#include "NiBound.h"
#include "TGModelManager.h"
#include "TGAnimNode.h"
#include "TGStream.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005a3b80 _$E55
// FUNCTION: STBC 0x005a3b90 _$E54
// FUNCTION: STBC 0x005a3ba0 _$E221
// FUNCTION: STBC 0x005a3bb0 _$E220

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x005a3bc0 _$E296
// FUNCTION: STBC 0x005a3bd0 _$E292
// FUNCTION: STBC 0x005a3be0 _$E295
// FUNCTION: STBC 0x005a3bf0 _$E293
// DATA: STBC 0x0098e398
TGEventHandlerTable Planet::classHandlers;

// The speeds at which an object in the atmosphere takes all of its damage,
// and below which it takes none (invented names).
// DATA: STBC 0x008942b0
static const float FULL_DAMAGE_SPEED = 7.5f;
// DATA: STBC 0x008942b4
static const float NO_DAMAGE_SPEED = 0.5f;

/*
================
Planet::AddToSet

Files the planet in the set's object table under its type.
================
*/
// FUNCTION: STBC 0x005a3c10
void Planet::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, PLANET_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
Planet::RemoveFromSet
================
*/
// FUNCTION: STBC 0x005a3c40
void Planet::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, PLANET_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
Planet::GetClassObjects

The set's planets and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x005a3c70
Planet **Planet::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (Planet **)set->objectTable.GetObjects( PLANET_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
Planet::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x005a3ca0
int Planet::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( PLANET_TYPE );
	}
	return 0;
}

/*
================
Planet::GetObjectByID

The object of the class with the ID in the set, or in the object table when
the set is NULL; NULL when it is none or not a Planet.
================
*/
// FUNCTION: STBC 0x005a3cc0
Planet *Planet::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( PLANET_TYPE ) ) {
			return (Planet *)object;
		}
		return NULL;
	}
	return (Planet *)set->GetClassObject( PLANET_TYPE, id );
}

/*
================
Planet::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x005a3d30
Planet *Planet::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( PLANET_TYPE, name );
		}
		return (Planet *)object;
	}
	return (Planet *)set->GetClassObject( PLANET_TYPE, name );
}

/*
================
Planet::Cast

The object as a Planet; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005a3d90
Planet *Planet::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( PLANET_TYPE ) ) {
		return (Planet *)object;
	}
	return NULL;
}

/*
================
Planet::Create

A planet of the radius and model, with an atmosphere of 125 units.
================
*/
// FUNCTION: STBC 0x005a3dc0
Planet *Planet::Create( float radius, char *modelName ) {
	return new Planet( radius, 125.0f, modelName );
}

/*
================
Planet::Planet
================
*/
// FUNCTION: STBC 0x005a3e00
Planet::Planet( float radius_, float atmosphereRadius_, char *modelName_ ) : modelName( NULL ), atmosphereRadius( atmosphereRadius_ ), radius( radius_ ), environmentalHullDamage( 0.0f ), environmentalShieldDamage( 0.0f ) {
	if ( modelName_ != NULL ) {
		modelName = new char[strlen( modelName_ ) + 1];
		strcpy( modelName, modelName_ );
		CreateModel();
	}
	SetHailable( false );
}

/*
================
Planet::SetContainingSet

In a set, the planet watches the damageable objects with a proximity check
that reaches the edge of its atmosphere.
================
*/
// FUNCTION: STBC 0x005a3eb0
void Planet::SetContainingSet( SetClass *set ) {
	ObjectClass::SetContainingSet( set );
	if ( set != NULL ) {
		ProximityCheck *check = new ProximityCheck( ET_PROXIMITY_PLANET, this );
		check->AddObjectTypeToCheckList( DAMAGEABLEOBJECT_TYPE );
		check->SetRadius( radius + atmosphereRadius );
		AttachObject( check );
		UpdateNodeOnly();
		set->GetProximityManager()->AddObject( check );
	}
}

/*
================
Planet::GetEnvironmentalDamage

Inside the atmosphere the depth runs from 0 at its edge to 1 at the planet's
surface, scaled down below a speed of 7.5 and to 0 below 0.5. The push is
away from the planet, against the object's velocity, mixed by that speed
factor.
================
*/
// FUNCTION: STBC 0x005a3f30
// PARKED: inline budget and x87 locals: retail inlines the velocity product's NiPoint3 constructor (site 17.1 needs a share of 47, ours 46) and so calls the last operator= out of line; ours keeps distance on the FPU stack where retail keeps it in a frame slot (frame 8 bytes larger)
float Planet::GetEnvironmentalDamage( float damage, ObjectClass *object, float *depth, NiPoint3 *direction ) {
	*depth = 0.0f;
	const NiPoint3 &objectLocation = object->GetNiObject()->GetWorldTranslate();
	const NiPoint3 &location = GetNiObject()->GetWorldTranslate();
	NiPoint3 offset = objectLocation - location;
	float distanceSquared = offset.SqrLength();
	float distance = radius;
	float speedFactor = 0.0f;
	float outerRadius = atmosphereRadius + radius;
	if ( distanceSquared < outerRadius * outerRadius && distance * distance < distanceSquared ) {
		distance = sqrtf( distanceSquared );
		*depth = 1.0f - ( distance - radius ) / atmosphereRadius;
		float speed = object->GetNiObject()->GetWorldVelocity().Length();
		if ( speed > NO_DAMAGE_SPEED ) {
			if ( speed < FULL_DAMAGE_SPEED ) {
				speedFactor = speed / FULL_DAMAGE_SPEED;
			} else {
				speedFactor = 1.0f;
			}
		} else {
			*depth = 0.0f;
		}
		*depth *= speedFactor;
	} else {
		*depth = 0.0f;
	}
	if ( *depth > 0.0f ) {
		*direction = -object->GetNiObject()->GetWorldVelocity() * speedFactor + ( objectLocation - location ) / distance * ( 1.0f - speedFactor );
		direction->Unitize();
		return damage * *depth;
	}
	*direction = -object->GetNiObject()->GetWorldVelocity();
	*depth = 0.0f;
	return damage * *depth;
}

/*
================
Planet::VirtualSlot67
================
*/
// FUNCTION: STBC 0x005a4240
float Planet::VirtualSlot67( ObjectClass *object, float *depth, NiPoint3 *direction ) {
	return GetEnvironmentalDamage( environmentalHullDamage, object, depth, direction );
}

/*
================
Planet::VirtualSlot68
================
*/
// FUNCTION: STBC 0x005a4260
float Planet::VirtualSlot68( ObjectClass *object, float *depth, NiPoint3 *direction ) {
	return GetEnvironmentalDamage( environmentalShieldDamage, object, depth, direction );
}

/*
================
Planet::RegisterHandlerNames

The atmosphere handler under its class-qualified name.
================
*/
// FUNCTION: STBC 0x005a4280
void Planet::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Planet::AtmosphereProximityHandler, "Planet::AtmosphereProximityHandler" );
}

/*
================
Planet::RegisterHandlers

The atmosphere handler in the class's table, under the proximity event type.
================
*/
// FUNCTION: STBC 0x005a42a0
void Planet::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_PROXIMITY_PLANET, "Planet::AtmosphereProximityHandler" );
}

/*
================
Planet::AtmosphereProximityHandler

An object that comes inside the planet radius sends a collision event; one that comes inside the atmosphere beyond it takes environmental damage.
================
*/
// FUNCTION: STBC 0x005a42c0
// PARKED: block order: retail lays the damage block after the collision block (both share the AddEvent tail) and duplicates the CallNextHandler tail for the miss inside the atmosphere; ours keeps the damage block in place; the miss's explicit CallNextHandler; return; compiles the same as falling to the final call
void Planet::AtmosphereProximityHandler( ProximityEvent *event ) {
	DamageableObject *object = DamageableObject::Cast( event->GetObject() );
	if ( object != NULL ) {
		float reach = object->GetWorldBound().GetRadius() + radius;
		if ( ( object->GetWorldLocation() - GetWorldLocation() ).SqrLength() >= reach * reach ) {
			if ( atmosphereRadius > 0.0f ) {
				reach = object->GetWorldBound().GetRadius() + radius;
				reach += atmosphereRadius;
				if ( ( object->GetWorldLocation() - GetWorldLocation() ).SqrLength() < reach * reach ) {
					TGEvent *damage = new TGEvent;
					damage->SetEventType( ET_ENVIRONMENT_DAMAGE );
					damage->SetSource( this );
					damage->SetDestination( object );
					g_kEventManager.AddEvent( damage );
				}
			}
		} else {
			TGEvent *collision = new TGEvent;
			collision->SetEventType( ET_PLANET_COLLISION );
			collision->SetSource( this );
			collision->SetDestination( event->GetObject() );
			g_kEventManager.AddEvent( collision );
		}
	}
	CallNextHandler( event );
}

/*
================
Planet::CreateModel
================
*/
// FUNCTION: STBC 0x005a4480
void Planet::CreateModel( void ) {
	if ( modelName == NULL ) {
		return;
	}
	if ( !g_kModelManager.IsModelLoaded( modelName ) ) {
		g_kModelManager.LoadModel( modelName );
	}
	TGAnimNode *model = g_kModelManager.CloneModel( modelName );
	if ( model == NULL ) {
		return;
	}
	SetNiObject( model );
	model->Update( 0.0f );
	float scale = radius / model->GetWorldBound().GetRadius();
	if ( scale <= 0.0f ) {
		scale = -scale;
	}
	model->SetScale( scale );
	model->Update( 0.0f );
}

/*
================
Planet::WriteToStream
================
*/
// FUNCTION: STBC 0x005a4500
bool Planet::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	char text[256];
	sprintf( text, "Saving planet %s\n", name );
	Log( text );
	stream->VirtualSlot11( modelName );
	stream->WriteFloat( radius );
	stream->WriteFloat( atmosphereRadius );
	return true;
}

/*
================
Planet::ReadFromStream
================
*/
// FUNCTION: STBC 0x005a4580
bool Planet::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	char text[256];
	sprintf( text, "Loading planet %s\n", name );
	Log( text );
	modelName = stream->VirtualSlot10( -1 );
	radius = stream->ReadFloat();
	atmosphereRadius = stream->ReadFloat();
	CreateModel();
	return true;
}
