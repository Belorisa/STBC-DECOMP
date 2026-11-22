/*
===============================================================================

	TractorBeamSystem.cpp

===============================================================================
*/

#include <iostream>
#include "TractorBeamSystem.h"
#include "TGEventManager.h"
#include "TGObjPtrEvent.h"
#include "TractorBeamProjector.h"
#include "ImpulseEngineSubsystem.h"
#include "ShipClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "StartFiringEvent.h"
#include "UtopiaModule.h"
#include "ObjectEmitterProperty.h"
#include "TGModelPropertyInstance.h"
#include "TGModelPropertyList.h"
#include "TGModelPropertySet.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00583100 _$E487
// FUNCTION: STBC 0x00583120 _$E486

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00581f40 _$E32
// FUNCTION: STBC 0x00581f50 _$E29
// FUNCTION: STBC 0x00581f60 _$E31
// FUNCTION: STBC 0x00581f70 _$E30
// FUNCTION: STBC 0x00581f80 _$E38
// FUNCTION: STBC 0x00581f90 _$E35
// FUNCTION: STBC 0x00581fa0 _$E37
// FUNCTION: STBC 0x00581fb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00581fc0 _$E79
// FUNCTION: STBC 0x00581fd0 _$E78
// FUNCTION: STBC 0x00581fe0 _$E293
// FUNCTION: STBC 0x00581ff0 _$E292
// FUNCTION: STBC 0x005828a0 _$E90
// FUNCTION: STBC 0x005828b0 _$E89
// FUNCTION: STBC 0x005828c0 _$E94
// FUNCTION: STBC 0x005828d0 _$E93
// FUNCTION: STBC 0x005828e0 _$E98
// FUNCTION: STBC 0x005828f0 _$E97
// FUNCTION: STBC 0x00582900 _$E102
// FUNCTION: STBC 0x00582910 _$E101
// FUNCTION: STBC 0x00582920 _$E221
// FUNCTION: STBC 0x00582930 _$E220
// FUNCTION: STBC 0x00582940 _$E163
// FUNCTION: STBC 0x00582950 _$E162
// FUNCTION: STBC 0x00582960 _$E176
// FUNCTION: STBC 0x00582970 _$E175
// FUNCTION: STBC 0x00582980 _$E226
// FUNCTION: STBC 0x00582990 _$E225
// FUNCTION: STBC 0x005829a0 _$E298
// FUNCTION: STBC 0x005829b0 _$E297
// FUNCTION: STBC 0x005829c0 _$E230
// FUNCTION: STBC 0x005829d0 _$E229
// FUNCTION: STBC 0x005829e0 _$E302
// FUNCTION: STBC 0x005829f0 _$E301
// FUNCTION: STBC 0x00582a00 _$E309
// FUNCTION: STBC 0x00582a10 _$E308
// FUNCTION: STBC 0x00582a20 _$E327
// FUNCTION: STBC 0x00582a30 _$E326
// FUNCTION: STBC 0x00582a40 _$E331
// FUNCTION: STBC 0x00582a50 _$E330
// FUNCTION: STBC 0x00582a60 _$E121
// FUNCTION: STBC 0x00582a70 _$E120
// FUNCTION: STBC 0x00582a80 _$E238
// FUNCTION: STBC 0x00582a90 _$E237
// FUNCTION: STBC 0x00582aa0 _$E245
// FUNCTION: STBC 0x00582ab0 _$E244
// FUNCTION: STBC 0x00582ac0 _$E249
// FUNCTION: STBC 0x00582ad0 _$E248
// FUNCTION: STBC 0x00582ae0 _$E253
// FUNCTION: STBC 0x00582af0 _$E252
// FUNCTION: STBC 0x00582b00 _$E257
// FUNCTION: STBC 0x00582b10 _$E256
// FUNCTION: STBC 0x00582b20 _$E263
// FUNCTION: STBC 0x00582b30 _$E262
// FUNCTION: STBC 0x00582b40 _$E335
// FUNCTION: STBC 0x00582b50 _$E334
// FUNCTION: STBC 0x00582b60 _$E339
// FUNCTION: STBC 0x00582b70 _$E338
// FUNCTION: STBC 0x00582b80 _$E343
// FUNCTION: STBC 0x00582b90 _$E342
// FUNCTION: STBC 0x00582ba0 _$E347
// FUNCTION: STBC 0x00582bb0 _$E346
// FUNCTION: STBC 0x00582bc0 _$E351
// FUNCTION: STBC 0x00582bd0 _$E350
// FUNCTION: STBC 0x00582be0 _$E355
// FUNCTION: STBC 0x00582bf0 _$E354
// FUNCTION: STBC 0x00582c00 _$E267
// FUNCTION: STBC 0x00582c10 _$E266
// FUNCTION: STBC 0x00582c20 _$E359
// FUNCTION: STBC 0x00582c30 _$E358
// FUNCTION: STBC 0x00582c40 _$E363
// FUNCTION: STBC 0x00582c50 _$E362
// FUNCTION: STBC 0x00582c60 _$E367
// FUNCTION: STBC 0x00582c70 _$E366
// FUNCTION: STBC 0x00582c80 _$E126
// FUNCTION: STBC 0x00582c90 _$E125
// FUNCTION: STBC 0x00582ca0 _$E371
// FUNCTION: STBC 0x00582cb0 _$E370
// FUNCTION: STBC 0x00582cc0 _$E131
// FUNCTION: STBC 0x00582cd0 _$E130
// FUNCTION: STBC 0x00582ce0 _$E375
// FUNCTION: STBC 0x00582cf0 _$E374
// FUNCTION: STBC 0x00582d00 _$E379
// FUNCTION: STBC 0x00582d10 _$E378
// FUNCTION: STBC 0x00582d20 _$E383
// FUNCTION: STBC 0x00582d30 _$E382
// FUNCTION: STBC 0x00582d40 _$E387
// FUNCTION: STBC 0x00582d50 _$E386
// FUNCTION: STBC 0x00582d60 _$E140
// FUNCTION: STBC 0x00582d70 _$E139
// FUNCTION: STBC 0x00582d80 _$E391
// FUNCTION: STBC 0x00582d90 _$E390
// FUNCTION: STBC 0x00582da0 _$E395
// FUNCTION: STBC 0x00582db0 _$E394
// FUNCTION: STBC 0x00582dc0 _$E399
// FUNCTION: STBC 0x00582dd0 _$E398
// FUNCTION: STBC 0x00582de0 _$E146
// FUNCTION: STBC 0x00582df0 _$E145
// FUNCTION: STBC 0x00582e00 _$E151
// FUNCTION: STBC 0x00582e10 _$E150
// FUNCTION: STBC 0x00582e20 _$E181
// FUNCTION: STBC 0x00582e30 _$E180
// FUNCTION: STBC 0x00582e40 _$E403
// FUNCTION: STBC 0x00582e50 _$E402
// FUNCTION: STBC 0x00582e60 _$E186
// FUNCTION: STBC 0x00582e70 _$E185
// FUNCTION: STBC 0x00582e80 _$E407
// FUNCTION: STBC 0x00582e90 _$E406
// FUNCTION: STBC 0x00582ea0 _$E411
// FUNCTION: STBC 0x00582eb0 _$E410
// FUNCTION: STBC 0x00582ec0 _$E191
// FUNCTION: STBC 0x00582ed0 _$E190
// FUNCTION: STBC 0x00582ee0 _$E415
// FUNCTION: STBC 0x00582ef0 _$E414
// FUNCTION: STBC 0x00582f00 _$E156
// FUNCTION: STBC 0x00582f10 _$E155
// FUNCTION: STBC 0x00582f20 _$E419
// FUNCTION: STBC 0x00582f30 _$E418
// FUNCTION: STBC 0x00582f40 _$E423
// FUNCTION: STBC 0x00582f50 _$E422
// FUNCTION: STBC 0x00582f60 _$E196
// FUNCTION: STBC 0x00582f70 _$E195
// FUNCTION: STBC 0x00582f80 _$E201
// FUNCTION: STBC 0x00582f90 _$E200
// FUNCTION: STBC 0x00582fa0 _$E427
// FUNCTION: STBC 0x00582fb0 _$E426
// FUNCTION: STBC 0x00582fc0 _$E431
// FUNCTION: STBC 0x00582fd0 _$E430
// FUNCTION: STBC 0x00582fe0 _$E435
// FUNCTION: STBC 0x00582ff0 _$E434
// FUNCTION: STBC 0x00583000 _$E439
// FUNCTION: STBC 0x00583010 _$E438
// FUNCTION: STBC 0x00583020 _$E443
// FUNCTION: STBC 0x00583030 _$E442
// FUNCTION: STBC 0x00583040 _$E447
// FUNCTION: STBC 0x00583050 _$E446
// FUNCTION: STBC 0x00583060 _$E451
// FUNCTION: STBC 0x00583070 _$E450
// FUNCTION: STBC 0x00583080 _$E455
// FUNCTION: STBC 0x00583090 _$E454
// FUNCTION: STBC 0x005830a0 _$E206
// FUNCTION: STBC 0x005830b0 _$E205
// FUNCTION: STBC 0x005830c0 _$E459
// FUNCTION: STBC 0x005830d0 _$E458
// FUNCTION: STBC 0x005830e0 _$E211
// FUNCTION: STBC 0x005830f0 _$E210
// FUNCTION: STBC 0x00583130 _$E216
// FUNCTION: STBC 0x00583140 _$E215
// FUNCTION: STBC 0x00583150 _$E463
// FUNCTION: STBC 0x00583160 _$E462
// FUNCTION: STBC 0x00583170 _$E468
// FUNCTION: STBC 0x00583180 _$E467
// FUNCTION: STBC 0x00583190 _$E472
// FUNCTION: STBC 0x005831a0 _$E471

/*
================
TractorBeamSystem::Cast

The object as a TractorBeamSystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00582050
TractorBeamSystem *TractorBeamSystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TRACTORBEAMSYSTEM_TYPE ) ) {
		return (TractorBeamSystem *)object;
	}
	return NULL;
}

/*
================
TractorBeamSystem::TractorBeamSystem
================
*/
// FUNCTION: STBC 0x00582080
TractorBeamSystem::TractorBeamSystem( long id ) : WeaponSystem( id ) {
	impulseEngineSubsystem = NULL;
	mode = 1;
	unknownF8 = 0.0f;
	unknownFC = 0.0f;
	powerMode = POWER_MODE_STEAL;
}

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00582000 _$E478
// FUNCTION: STBC 0x00582010 _$E474
// FUNCTION: STBC 0x00582020 _$E477
// FUNCTION: STBC 0x00582030 _$E475
// DATA: STBC 0x0098d098
TGEventHandlerTable TractorBeamSystem::classHandlers;

/*
================
TractorBeamSystem::GetObjType
================
*/
// FUNCTION: STBC 0x005820c0
int TractorBeamSystem::GetObjType( void ) {
	return TRACTORBEAMSYSTEM_TYPE;
}

/*
================
TractorBeamSystem::GetClassName
================
*/
// FUNCTION: STBC 0x00582130
char *TractorBeamSystem::GetClassName( void ) {
	return "TractorBeamSystem";
}

/*
================
TractorBeamSystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00582140
char *TractorBeamSystem::GetSwigTypeName( void ) {
	return "_p_TractorBeamSystem";
}

/*
================
TractorBeamSystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00582150
char *TractorBeamSystem::GetPythonClassName( void ) {
	return "TractorBeamSystemPtr";
}

/*
================
TractorBeamSystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00582160
TGEventHandlerTable *TractorBeamSystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TractorBeamSystem::~TractorBeamSystem
================
*/
// FUNCTION: STBC 0x005821a0
// FUNCTION: STBC 0x00582170 ??_GTractorBeamSystem@@UAEPAXI@Z
TractorBeamSystem::~TractorBeamSystem( void ) {
}

/*
================
TractorBeamSystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005821b0
void TractorBeamSystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TractorBeamSystem::StartFiringHandler, "TractorBeamSystem::StartFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TractorBeamSystem::StopFiringHandler, "TractorBeamSystem::StopFiringHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TractorBeamSystem::StopFiringAtTargetHandler, "TractorBeamSystem::StopFiringAtTargetHandler" );
}

/*
================
TractorBeamSystem::RegisterHandlers
================
*/
// FUNCTION: STBC 0x005821f0
void TractorBeamSystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_START_FIRING, "TractorBeamSystem::StartFiringHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_STOP_FIRING, "TractorBeamSystem::StopFiringHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_STOP_FIRING_AT_TARGET, "TractorBeamSystem::StopFiringAtTargetHandler" );
}

/*
================
TractorBeamSystem::SetParentShip

The tractor beam drags on the ship's impulse engines.
================
*/
// FUNCTION: STBC 0x00582230
void TractorBeamSystem::SetParentShip( ShipClass *ship ) {
	PoweredSubsystem::SetParentShip( ship );

	ImpulseEngineSubsystem *engines = ship->GetImpulseEngineSubsystem();
	if ( engines != NULL ) {
		impulseEngineSubsystem = engines;
		engines->SetTractorBeamSystem( this );
	}
}

/*
================
TractorBeamSystem::GetNormalPowerWanted

The beam draws power only while it fires.
================
*/
// FUNCTION: STBC 0x00582260
// PARKED: retail calls PoweredSubsystem::GetNormalPowerWanted and returns; ours tail-jumps to it (as PulseWeapon::GetPowerScaled)
float TractorBeamSystem::GetNormalPowerWanted( void ) {
	if ( firing == true ) {
		return PoweredSubsystem::GetNormalPowerWanted();
	}
	return 0.0f;
}

/*
================
TractorBeamSystem::GetProjectorsMaxDamage

The sum of the maximum damage of the system's projectors (invented name).
================
*/
// FUNCTION: STBC 0x00582280
float TractorBeamSystem::GetProjectorsMaxDamage( void ) {
	float total = 0.0f;
	for ( int i = 0; i < numChildSubsystems; i++ ) {
		TractorBeamProjector *projector = TractorBeamProjector::Cast( GetChildSubsystem( i ) );
		if ( projector != NULL ) {
			total += projector->GetMaxDamage();
		}
	}
	return total;
}

/*
================
TractorBeamSystem::GetLoad

The share of the projectors' maximum damage in use (unknownFC over
unknownF8); none without projectors (invented name). The impulse
engines lose that share of their performance.
================
*/
// FUNCTION: STBC 0x005822d0
float TractorBeamSystem::GetLoad( void ) {
	if ( unknownF8 <= 0.0f ) {
		return 0.0f;
	}
	return unknownFC / unknownF8;
}

/*
================
TractorBeamSystem::GetDockingPoint

Where in the world the ship launches its shuttles: its first shuttle
emitter's position through the ship's world transform; the origin when it has
none (invented name; DockTarget pulls the target there).
================
*/
// FUNCTION: STBC 0x00582300
NiPoint3 TractorBeamSystem::GetDockingPoint( void ) {
	ShipClass *ship = parentShip;
	TGModelPropertyList *list = ship->GetPropertySet()->GetPropertiesByType( OBJECTEMITTERPROPERTY_TYPE );
	NiPoint3 point = NiPoint3::ZERO;
	NiTListIterator pos = list->GetHeadPos();
	while ( pos ) {
		ObjectEmitterProperty *emitter = ObjectEmitterProperty::Cast( list->GetNext( pos )->GetProperty() );
		if ( emitter->GetEmittedObjectType() == ObjectEmitterProperty::OEP_SHUTTLE ) {
			point = emitter->GetPosition();
			point = TGModelUtils::LocalToWorldPoint( ship->GetNode(), point );
			break;
		}
	}
	delete list;
	return point;
}

/*
================
TractorBeamSystem::Update

Takes the projectors' total maximum damage afresh each update.
================
*/
// FUNCTION: STBC 0x00582460
void TractorBeamSystem::Update( float deltaTime ) {
	WeaponSystem::Update( deltaTime );
	unknownF8 = GetProjectorsMaxDamage();
	unknownFC = 0.0f;
}

/*
================
TractorBeamSystem::VirtualSlot34

Starts firing at the target and offset; in a multiplayer game, a system that is
trying to fire tells the others with a StartFiringEvent (ET_START_FIRING_NOTIFY).
================
*/
// FUNCTION: STBC 0x00582490
void TractorBeamSystem::VirtualSlot34( ObjectClass *target, NiPoint3 *offset ) {
	WeaponSystem::VirtualSlot34( target, offset );
	if ( g_kUtopiaModule.IsMultiplayer() && tryingToFire == true && unknownA4 ) {
		StartFiringEvent *event = new StartFiringEvent;
		event->SetEventType( ET_START_FIRING_NOTIFY );
		event->SetDestination( this );
		event->SetTarget( target );
		event->SetOffset( offset );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
TractorBeamSystem::StopFiring

In a multiplayer game, a firing system that was trying to fire tells the others
with ET_STOP_FIRING_NOTIFY; then WeaponSystem stops the weapons and the beam
goes back to its first mode.
================
*/
// FUNCTION: STBC 0x00582520
void TractorBeamSystem::StopFiring( void ) {
	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 && tryingToFire ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_STOP_FIRING_NOTIFY );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
	}
	WeaponSystem::StopFiring();
	mode = 1;
}

/*
================
TractorBeamSystem::VirtualSlot38

In a multiplayer game, a firing system that was trying to fire tells the
others which target it stopped firing at, with
ET_STOP_FIRING_AT_TARGET_NOTIFY; then WeaponSystem stops the weapons aiming
at it.
================
*/
// FUNCTION: STBC 0x005825a0
void TractorBeamSystem::VirtualSlot38( ObjectClass *target ) {
	WeaponSystem::StopFiringAtTarget( target );
	if ( g_kUtopiaModule.IsMultiplayer() && unknownA4 && tryingToFire ) {
		TGObjPtrEvent *event = new TGObjPtrEvent;
		event->SetEventType( ET_STOP_FIRING_AT_TARGET_NOTIFY );
		event->SetDestination( this );
		event->SetObjPtr( target );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
TractorBeamSystem::StartFiringHandler

Starts firing at the event's target and point; the change is not sent on
while the event is handled.
================
*/
// FUNCTION: STBC 0x00582640
void TractorBeamSystem::StartFiringHandler( TGEvent *event ) {
	StartFiringEvent *startEvent = (StartFiringEvent *)event;
	unknownA4 = false;
	NiPoint3 offset = *startEvent->GetOffset();
	VirtualSlot34( startEvent->GetTarget(), &offset );
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
TractorBeamSystem::StopFiringHandler

Stops firing; the change is not sent on while the event is handled.
================
*/
// FUNCTION: STBC 0x005826a0
void TractorBeamSystem::StopFiringHandler( TGEvent *event ) {
	unknownA4 = false;
	StopFiring();
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
TractorBeamSystem::StopFiringAtTargetHandler

Stops firing at the event's object; the change is not sent on while the
event is handled.
================
*/
// FUNCTION: STBC 0x005826d0
void TractorBeamSystem::StopFiringAtTargetHandler( TGEvent *event ) {
	unknownA4 = false;
	VirtualSlot38( (ObjectClass *)( (TGObjPtrEvent *)event )->GetObjPtr() );
	unknownA4 = true;
	CallNextHandler( event );
}

/*
================
TractorBeamSystem::WriteToStream

The impulse engines by ID, 0 without them, then the mode.
================
*/
// FUNCTION: STBC 0x00582710
bool TractorBeamSystem::WriteToStream( TGStream *stream ) {
	if ( !WeaponSystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteID( impulseEngineSubsystem ? impulseEngineSubsystem->GetObjID() : 0 );
	stream->WriteLong( mode );
	stream->WriteFloat( unknownF8 );
	stream->WriteFloat( unknownFC );
	return true;
}

/*
================
TractorBeamSystem::ReadFromStream

Reads what WriteToStream writes; the impulse engines stay an ID until
PostFixUp.
================
*/
// FUNCTION: STBC 0x00582780
bool TractorBeamSystem::ReadFromStream( TGStream *stream ) {
	if ( !WeaponSystem::ReadFromStream( stream ) ) {
		return false;
	}

	impulseEngineSubsystem = (ImpulseEngineSubsystem *)stream->ReadID();
	mode = stream->ReadLong();
	unknownF8 = stream->ReadFloat();
	unknownFC = stream->ReadFloat();
	return true;
}

/*
================
TractorBeamSystem::FixUpPointers

Fails when the impulse engines it was saved with have not been loaded.
================
*/
// FUNCTION: STBC 0x005827e0
bool TractorBeamSystem::FixUpPointers( void ) {
	if ( !WeaponSystem::FixUpPointers() ) {
		return false;
	}
	if ( impulseEngineSubsystem != NULL && GetTGObjectPtr( (long)impulseEngineSubsystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TractorBeamSystem::PostFixUp

Turns the impulse engines' ID back into the object.
================
*/
// FUNCTION: STBC 0x00582810
void TractorBeamSystem::PostFixUp( void ) {
	WeaponSystem::PostFixUp();
	impulseEngineSubsystem = (ImpulseEngineSubsystem *)GetSavedObject( (long)impulseEngineSubsystem );
}

/*
================
TractorBeamSystem::VirtualSlot26

After WeaponSystem's state, the mode.
================
*/
// FUNCTION: STBC 0x00582840
void TractorBeamSystem::VirtualSlot26( TGStream *stream ) {
	WeaponSystem::VirtualSlot26( stream );

	stream->WriteChar( (char)mode );
	stream->GetPos();
}

/*
================
TractorBeamSystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x00582870
void TractorBeamSystem::VirtualSlot27( TGStream *stream ) {
	WeaponSystem::VirtualSlot27( stream );

	mode = stream->ReadChar();
	stream->GetPos();
}
