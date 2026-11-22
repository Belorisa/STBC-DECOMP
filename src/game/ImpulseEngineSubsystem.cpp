/*
===============================================================================

	ImpulseEngineSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "ImpulseEngineSubsystem.h"
#include "ImpulseEngineProperty.h"
#include "ShipClass.h"
#include "TractorBeamSystem.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00561f40 _$E469
// FUNCTION: STBC 0x00561f60 _$E468

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00560f40 _$E32
// FUNCTION: STBC 0x00560f50 _$E29
// FUNCTION: STBC 0x00560f60 _$E31
// FUNCTION: STBC 0x00560f70 _$E30
// FUNCTION: STBC 0x00560f80 _$E38
// FUNCTION: STBC 0x00560f90 _$E35
// FUNCTION: STBC 0x00560fa0 _$E37
// FUNCTION: STBC 0x00560fb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00561780 _$E77
// FUNCTION: STBC 0x00561790 _$E76
// FUNCTION: STBC 0x00561800 _$E279
// FUNCTION: STBC 0x00561810 _$E278
// FUNCTION: STBC 0x00561820 _$E88
// FUNCTION: STBC 0x00561830 _$E87
// FUNCTION: STBC 0x00561840 _$E92
// FUNCTION: STBC 0x00561850 _$E91
// FUNCTION: STBC 0x00561860 _$E96
// FUNCTION: STBC 0x00561870 _$E95
// FUNCTION: STBC 0x00561880 _$E100
// FUNCTION: STBC 0x00561890 _$E99
// FUNCTION: STBC 0x005618a0 _$E217
// FUNCTION: STBC 0x005618b0 _$E216
// FUNCTION: STBC 0x005618c0 _$E159
// FUNCTION: STBC 0x005618d0 _$E158
// FUNCTION: STBC 0x005618e0 _$E172
// FUNCTION: STBC 0x005618f0 _$E171
// FUNCTION: STBC 0x00561900 _$E222
// FUNCTION: STBC 0x00561910 _$E221
// FUNCTION: STBC 0x00561920 _$E284
// FUNCTION: STBC 0x00561930 _$E283
// FUNCTION: STBC 0x00561940 _$E226
// FUNCTION: STBC 0x00561950 _$E225
// FUNCTION: STBC 0x00561960 _$E288
// FUNCTION: STBC 0x00561970 _$E287
// FUNCTION: STBC 0x00561980 _$E295
// FUNCTION: STBC 0x00561990 _$E294
// FUNCTION: STBC 0x005619a0 _$E305
// FUNCTION: STBC 0x005619b0 _$E304
// FUNCTION: STBC 0x005619c0 _$E309
// FUNCTION: STBC 0x005619d0 _$E308
// FUNCTION: STBC 0x005619e0 _$E117
// FUNCTION: STBC 0x005619f0 _$E116
// FUNCTION: STBC 0x00561a00 _$E242
// FUNCTION: STBC 0x00561a10 _$E241
// FUNCTION: STBC 0x00561a20 _$E249
// FUNCTION: STBC 0x00561a30 _$E248
// FUNCTION: STBC 0x00561a40 _$E253
// FUNCTION: STBC 0x00561a50 _$E252
// FUNCTION: STBC 0x00561a60 _$E257
// FUNCTION: STBC 0x00561a70 _$E256
// FUNCTION: STBC 0x00561a80 _$E261
// FUNCTION: STBC 0x00561a90 _$E260
// FUNCTION: STBC 0x00561aa0 _$E267
// FUNCTION: STBC 0x00561ab0 _$E266
// FUNCTION: STBC 0x00561ac0 _$E313
// FUNCTION: STBC 0x00561ad0 _$E312
// FUNCTION: STBC 0x00561ae0 _$E317
// FUNCTION: STBC 0x00561af0 _$E316
// FUNCTION: STBC 0x00561b00 _$E321
// FUNCTION: STBC 0x00561b10 _$E320
// FUNCTION: STBC 0x00561b20 _$E325
// FUNCTION: STBC 0x00561b30 _$E324
// FUNCTION: STBC 0x00561b40 _$E329
// FUNCTION: STBC 0x00561b50 _$E328
// FUNCTION: STBC 0x00561b60 _$E333
// FUNCTION: STBC 0x00561b70 _$E332
// FUNCTION: STBC 0x00561b80 _$E337
// FUNCTION: STBC 0x00561b90 _$E336
// FUNCTION: STBC 0x00561ba0 _$E341
// FUNCTION: STBC 0x00561bb0 _$E340
// FUNCTION: STBC 0x00561bc0 _$E345
// FUNCTION: STBC 0x00561bd0 _$E344
// FUNCTION: STBC 0x00561be0 _$E349
// FUNCTION: STBC 0x00561bf0 _$E348
// FUNCTION: STBC 0x00561c00 _$E122
// FUNCTION: STBC 0x00561c10 _$E121
// FUNCTION: STBC 0x00561c20 _$E353
// FUNCTION: STBC 0x00561c30 _$E352
// FUNCTION: STBC 0x00561c40 _$E127
// FUNCTION: STBC 0x00561c50 _$E126
// FUNCTION: STBC 0x00561c60 _$E357
// FUNCTION: STBC 0x00561c70 _$E356
// FUNCTION: STBC 0x00561c80 _$E361
// FUNCTION: STBC 0x00561c90 _$E360
// FUNCTION: STBC 0x00561ca0 _$E365
// FUNCTION: STBC 0x00561cb0 _$E364
// FUNCTION: STBC 0x00561cc0 _$E369
// FUNCTION: STBC 0x00561cd0 _$E368
// FUNCTION: STBC 0x00561ce0 _$E136
// FUNCTION: STBC 0x00561cf0 _$E135
// FUNCTION: STBC 0x00561d00 _$E373
// FUNCTION: STBC 0x00561d10 _$E372
// FUNCTION: STBC 0x00561d20 _$E377
// FUNCTION: STBC 0x00561d30 _$E376
// FUNCTION: STBC 0x00561d40 _$E381
// FUNCTION: STBC 0x00561d50 _$E380
// FUNCTION: STBC 0x00561d60 _$E142
// FUNCTION: STBC 0x00561d70 _$E141
// FUNCTION: STBC 0x00561d80 _$E147
// FUNCTION: STBC 0x00561d90 _$E146
// FUNCTION: STBC 0x00561da0 _$E177
// FUNCTION: STBC 0x00561db0 _$E176
// FUNCTION: STBC 0x00561dc0 _$E385
// FUNCTION: STBC 0x00561dd0 _$E384
// FUNCTION: STBC 0x00561de0 _$E182
// FUNCTION: STBC 0x00561df0 _$E181
// FUNCTION: STBC 0x00561e00 _$E389
// FUNCTION: STBC 0x00561e10 _$E388
// FUNCTION: STBC 0x00561e20 _$E393
// FUNCTION: STBC 0x00561e30 _$E392
// FUNCTION: STBC 0x00561e40 _$E187
// FUNCTION: STBC 0x00561e50 _$E186
// FUNCTION: STBC 0x00561e60 _$E397
// FUNCTION: STBC 0x00561e70 _$E396
// FUNCTION: STBC 0x00561e80 _$E152
// FUNCTION: STBC 0x00561e90 _$E151
// FUNCTION: STBC 0x00561ea0 _$E401
// FUNCTION: STBC 0x00561eb0 _$E400
// FUNCTION: STBC 0x00561ec0 _$E405
// FUNCTION: STBC 0x00561ed0 _$E404
// FUNCTION: STBC 0x00561ee0 _$E192
// FUNCTION: STBC 0x00561ef0 _$E191
// FUNCTION: STBC 0x00561f00 _$E197
// FUNCTION: STBC 0x00561f10 _$E196
// FUNCTION: STBC 0x00561f20 _$E409
// FUNCTION: STBC 0x00561f30 _$E408
// FUNCTION: STBC 0x00561f70 _$E413
// FUNCTION: STBC 0x00561f80 _$E412
// FUNCTION: STBC 0x00561f90 _$E417
// FUNCTION: STBC 0x00561fa0 _$E416
// FUNCTION: STBC 0x00561fb0 _$E421
// FUNCTION: STBC 0x00561fc0 _$E420
// FUNCTION: STBC 0x00561fd0 _$E425
// FUNCTION: STBC 0x00561fe0 _$E424
// FUNCTION: STBC 0x00561ff0 _$E429
// FUNCTION: STBC 0x00562000 _$E428
// FUNCTION: STBC 0x00562010 _$E433
// FUNCTION: STBC 0x00562020 _$E432
// FUNCTION: STBC 0x00562030 _$E437
// FUNCTION: STBC 0x00562040 _$E436
// FUNCTION: STBC 0x00562050 _$E202
// FUNCTION: STBC 0x00562060 _$E201
// FUNCTION: STBC 0x00562070 _$E441
// FUNCTION: STBC 0x00562080 _$E440
// FUNCTION: STBC 0x00562090 _$E207
// FUNCTION: STBC 0x005620a0 _$E206
// FUNCTION: STBC 0x005620b0 _$E212
// FUNCTION: STBC 0x005620c0 _$E211
// FUNCTION: STBC 0x005620d0 _$E445
// FUNCTION: STBC 0x005620e0 _$E444
// FUNCTION: STBC 0x005620f0 _$E450
// FUNCTION: STBC 0x00562100 _$E449
// FUNCTION: STBC 0x00562110 _$E454
// FUNCTION: STBC 0x00562120 _$E453

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00560fd0 _$E460
// FUNCTION: STBC 0x00560fe0 _$E456
// FUNCTION: STBC 0x00560ff0 _$E459
// FUNCTION: STBC 0x00561000 _$E457
// DATA: STBC 0x0098bb00
TGEventHandlerTable ImpulseEngineSubsystem::classHandlers;

/*
================
ImpulseEngineSubsystem::GetProperty
================
*/
// FUNCTION: STBC 0x00560fc0
ImpulseEngineProperty *ImpulseEngineSubsystem::GetProperty( void ) {
	return (ImpulseEngineProperty *)property;
}

/*
================
ImpulseEngineSubsystem::Cast

The object as an ImpulseEngineSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00561020
ImpulseEngineSubsystem *ImpulseEngineSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( IMPULSEENGINESUBSYSTEM_TYPE ) ) {
		return (ImpulseEngineSubsystem *)object;
	}
	return NULL;
}

/*
================
ImpulseEngineSubsystem::ImpulseEngineSubsystem
================
*/
// FUNCTION: STBC 0x00561050
ImpulseEngineSubsystem::ImpulseEngineSubsystem( long id ) : PoweredSubsystem( id ) {
	tractorBeamSystem = NULL;
	curMaxSpeed = 0.0f;
	curMaxAccel = 0.0f;
	curMaxAngularVelocity = 0.0f;
	curMaxAngularAccel = 0.0f;
}

/*
================
ImpulseEngineSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x00561090
int ImpulseEngineSubsystem::GetObjType( void ) {
	return IMPULSEENGINESUBSYSTEM_TYPE;
}

/*
================
ImpulseEngineSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x00561100
char *ImpulseEngineSubsystem::GetClassName( void ) {
	return "ImpulseEngineSubsystem";
}

/*
================
ImpulseEngineSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00561110
char *ImpulseEngineSubsystem::GetSwigTypeName( void ) {
	return "_p_ImpulseEngineSubsystem";
}

/*
================
ImpulseEngineSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00561120
char *ImpulseEngineSubsystem::GetPythonClassName( void ) {
	return "ImpulseEngineSubsystemPtr";
}

/*
================
ImpulseEngineSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00561130
TGEventHandlerTable *ImpulseEngineSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ImpulseEngineSubsystem::~ImpulseEngineSubsystem
================
*/
// FUNCTION: STBC 0x00561170
// FUNCTION: STBC 0x00561140 ??_GImpulseEngineSubsystem@@UAEPAXI@Z
ImpulseEngineSubsystem::~ImpulseEngineSubsystem( void ) {
}

/*
================
ImpulseEngineSubsystem::Update

The current limits are the property's, scaled by the power received;
the children update with the engines.
================
*/
// FUNCTION: STBC 0x00561180
void ImpulseEngineSubsystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );

	curMaxSpeed = GetPowerPercentage() * GetMaxSpeed();
	curMaxAccel = GetPowerPercentage() * GetMaxAccel();
	curMaxAngularVelocity = GetPowerPercentage() * GetMaxAngularVelocity();
	curMaxAngularAccel = GetPowerPercentage() * GetMaxAngularAccel();

	int numChildren = numChildSubsystems;
	for ( int i = 0; i < numChildren; i++ ) {
		ShipSubsystem *child = GetChildSubsystem( i );
		if ( child != NULL ) {
			child->Update( deltaTime );
		}
	}
}

/*
================
ImpulseEngineSubsystem::GetMaxSpeed

The property's maximum, less each engine's share for every disabled or
damaged engine and the tractor beams' load, at most the property's
maximum, scaled by the power wanted; none while the engines are disabled
or off.
================
*/
// FUNCTION: STBC 0x00561230
float ImpulseEngineSubsystem::GetMaxSpeed( void ) {
	if ( IsDisabled() || !IsOn() ) {
		return 0.0f;
	}

	float maximum = GetProperty()->GetMaxSpeed();
	float value = maximum;
	int count = GetNumChildSubsystems();
	float engineShare = value / count;
	for ( int i = 0; i < count; i++ ) {
		ShipSubsystem *engine = GetChildSubsystem( i );
		if ( engine != NULL ) {
			if ( engine->IsDisabled() == true ) {
				value -= engineShare;
			} else {
				value -= ( 1.0f - engine->GetConditionPercentage() ) * engineShare;
			}
		}
	}

	if ( tractorBeamSystem != NULL ) {
		value *= 1.0f - tractorBeamSystem->GetLoad();
	}
	if ( value > maximum ) {
		value = maximum;
	}
	if ( value < 0.0f ) {
		return 0.0f;
	}
	return GetPowerPercentageWanted() * value;
}

/*
================
ImpulseEngineSubsystem::GetMaxAccel

The property's maximum, less each engine's share for every disabled or
damaged engine, scaled by the power wanted and by what the tractor beams'
load leaves (between none and all); none while the engines are disabled
or off.
================
*/
// FUNCTION: STBC 0x00561330
// PARKED: schedule residue: retail loads the property's maximum after the child count; ours one instruction earlier
float ImpulseEngineSubsystem::GetMaxAccel( void ) {
	if ( IsDisabled() || !IsOn() ) {
		return 0.0f;
	}

	float maximum = GetProperty()->GetMaxAccel();
	int count = GetNumChildSubsystems();
	float value = maximum;
	float engineShare = maximum / count;
	for ( int i = 0; i < count; i++ ) {
		ShipSubsystem *engine = GetChildSubsystem( i );
		if ( engine != NULL ) {
			if ( engine->IsDisabled() == true ) {
				value -= engineShare;
			} else {
				value -= ( 1.0f - engine->GetConditionPercentage() ) * engineShare;
			}
		}
	}

	float tractorFactor = 1.0f;
	if ( tractorBeamSystem != NULL ) {
		tractorFactor = 1.0f - tractorBeamSystem->GetLoad();
		if ( tractorFactor < 0.0f ) {
			tractorFactor = 0.0f;
		} else if ( tractorFactor > 1.0f ) {
			tractorFactor = 1.0f;
		}
	}
	return tractorFactor * powerPercentageWanted * value;
}

/*
================
ImpulseEngineSubsystem::GetMaxAngularVelocity

The property's maximum, less each engine's share for every disabled or
damaged engine, scaled by the power wanted and by what the tractor beams'
load leaves (between none and all); none while the engines are disabled
or off.
================
*/
// FUNCTION: STBC 0x00561430
// PARKED: schedule residue: retail loads the property's maximum after the child count; ours one instruction earlier
float ImpulseEngineSubsystem::GetMaxAngularVelocity( void ) {
	if ( IsDisabled() || !IsOn() ) {
		return 0.0f;
	}

	float maximum = GetProperty()->GetMaxAngularVelocity();
	int count = GetNumChildSubsystems();
	float value = maximum;
	float engineShare = maximum / count;
	for ( int i = 0; i < count; i++ ) {
		ShipSubsystem *engine = GetChildSubsystem( i );
		if ( engine != NULL ) {
			if ( engine->IsDisabled() == true ) {
				value -= engineShare;
			} else {
				value -= ( 1.0f - engine->GetConditionPercentage() ) * engineShare;
			}
		}
	}

	float tractorFactor = 1.0f;
	if ( tractorBeamSystem != NULL ) {
		tractorFactor = 1.0f - tractorBeamSystem->GetLoad();
		if ( tractorFactor < 0.0f ) {
			tractorFactor = 0.0f;
		} else if ( tractorFactor > 1.0f ) {
			tractorFactor = 1.0f;
		}
	}
	return tractorFactor * powerPercentageWanted * value;
}

/*
================
ImpulseEngineSubsystem::GetMaxAngularAccel

The property's maximum, less each engine's share for every disabled or
damaged engine, scaled by the power wanted and by what the tractor beams'
load leaves (between none and all); none while the engines are disabled
or off.
================
*/
// FUNCTION: STBC 0x00561530
// PARKED: schedule residue: retail loads the property's maximum after the child count; ours one instruction earlier
float ImpulseEngineSubsystem::GetMaxAngularAccel( void ) {
	if ( IsDisabled() || !IsOn() ) {
		return 0.0f;
	}

	float maximum = GetProperty()->GetMaxAngularAccel();
	int count = GetNumChildSubsystems();
	float value = maximum;
	float engineShare = maximum / count;
	for ( int i = 0; i < count; i++ ) {
		ShipSubsystem *engine = GetChildSubsystem( i );
		if ( engine != NULL ) {
			if ( engine->IsDisabled() == true ) {
				value -= engineShare;
			} else {
				value -= ( 1.0f - engine->GetConditionPercentage() ) * engineShare;
			}
		}
	}

	float tractorFactor = 1.0f;
	if ( tractorBeamSystem != NULL ) {
		tractorFactor = 1.0f - tractorBeamSystem->GetLoad();
		if ( tractorFactor < 0.0f ) {
			tractorFactor = 0.0f;
		} else if ( tractorFactor > 1.0f ) {
			tractorFactor = 1.0f;
		}
	}
	return tractorFactor * powerPercentageWanted * value;
}

/*
================
ImpulseEngineSubsystem::SetParentShip

The ship's tractor beam system drags on these engines; the current limits
start at the property's.
================
*/
// FUNCTION: STBC 0x00561630
void ImpulseEngineSubsystem::SetParentShip( ShipClass *ship ) {
	PoweredSubsystem::SetParentShip( ship );

	TractorBeamSystem *tractorBeams = ship->GetTractorBeamSystem();
	if ( tractorBeams != NULL ) {
		tractorBeams->SetImpulseEngineSubsystem( this );
		tractorBeamSystem = tractorBeams;
	}

	curMaxSpeed = GetProperty()->GetMaxSpeed();
	curMaxAccel = GetProperty()->GetMaxAccel();
	curMaxAngularVelocity = GetProperty()->GetMaxAngularVelocity();
	curMaxAngularAccel = GetProperty()->GetMaxAngularAccel();
}

/*
================
ImpulseEngineSubsystem::WriteToStream

The tractor beam system is written by ID, 0 without one.
================
*/
// FUNCTION: STBC 0x005616a0
bool ImpulseEngineSubsystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( tractorBeamSystem ) );
	stream->WriteFloat( curMaxSpeed );
	stream->WriteFloat( curMaxAccel );
	stream->WriteFloat( curMaxAngularVelocity );
	stream->WriteFloat( curMaxAngularAccel );
	return true;
}

/*
================
ImpulseEngineSubsystem::ReadFromStream

The tractor beam system stays an ID until FixUpPointers.
================
*/
// FUNCTION: STBC 0x00561710
bool ImpulseEngineSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}
	tractorBeamSystem = (TractorBeamSystem *)stream->ReadID();
	curMaxSpeed = stream->ReadFloat();
	curMaxAccel = stream->ReadFloat();
	curMaxAngularVelocity = stream->ReadFloat();
	curMaxAngularAccel = stream->ReadFloat();
	return true;
}

/*
================
ImpulseEngineSubsystem::FixUpPointers

False while the tractor beam system's ID names no object yet.
================
*/
// FUNCTION: STBC 0x005617a0
bool ImpulseEngineSubsystem::FixUpPointers( void ) {
	if ( !PoweredSubsystem::FixUpPointers() ) {
		return false;
	}
	if ( tractorBeamSystem != NULL && GetTGObjectPtr( (long)tractorBeamSystem ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ImpulseEngineSubsystem::PostFixUp

Turns the tractor beam system's ID back into the object.
================
*/
// FUNCTION: STBC 0x005617d0
void ImpulseEngineSubsystem::PostFixUp( void ) {
	PoweredSubsystem::PostFixUp();
	tractorBeamSystem = (TractorBeamSystem *)GetSavedObject( (long)tractorBeamSystem );
}
