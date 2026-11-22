/*
===============================================================================

	Weapon.cpp

===============================================================================
*/

#include <iostream>
#include "Weapon.h"
#include "WeaponProperty.h"
#include "TGStream.h"
#include "TGEventHandlerTable.h"

#include <math.h>
#include <stdlib.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00583a00 _$E449
// FUNCTION: STBC 0x00583a20 _$E448

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00583480 _$E32
// FUNCTION: STBC 0x00583490 _$E29
// FUNCTION: STBC 0x005834a0 _$E31
// FUNCTION: STBC 0x005834b0 _$E30
// FUNCTION: STBC 0x005834c0 _$E38
// FUNCTION: STBC 0x005834d0 _$E35
// FUNCTION: STBC 0x005834e0 _$E37
// FUNCTION: STBC 0x005834f0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00583500 _$E75
// FUNCTION: STBC 0x00583510 _$E74
// FUNCTION: STBC 0x00583520 _$E79
// FUNCTION: STBC 0x00583530 _$E78
// FUNCTION: STBC 0x00583540 _$E90
// FUNCTION: STBC 0x00583550 _$E89
// FUNCTION: STBC 0x00583560 _$E94
// FUNCTION: STBC 0x00583570 _$E93
// FUNCTION: STBC 0x00583580 _$E98
// FUNCTION: STBC 0x00583590 _$E97
// FUNCTION: STBC 0x005835a0 _$E102
// FUNCTION: STBC 0x005835b0 _$E101
// FUNCTION: STBC 0x005835c0 _$E221
// FUNCTION: STBC 0x005835d0 _$E220
// FUNCTION: STBC 0x005835e0 _$E163
// FUNCTION: STBC 0x005835f0 _$E162
// FUNCTION: STBC 0x00583600 _$E176
// FUNCTION: STBC 0x00583610 _$E175
// FUNCTION: STBC 0x00583620 _$E226
// FUNCTION: STBC 0x00583630 _$E225
// FUNCTION: STBC 0x00583640 _$E231
// FUNCTION: STBC 0x00583650 _$E230
// FUNCTION: STBC 0x00583660 _$E235
// FUNCTION: STBC 0x00583670 _$E234
// FUNCTION: STBC 0x00583680 _$E239
// FUNCTION: STBC 0x00583690 _$E238
// FUNCTION: STBC 0x005836a0 _$E246
// FUNCTION: STBC 0x005836b0 _$E245
// FUNCTION: STBC 0x005836c0 _$E268
// FUNCTION: STBC 0x005836d0 _$E267
// FUNCTION: STBC 0x005836e0 _$E272
// FUNCTION: STBC 0x005836f0 _$E271
// FUNCTION: STBC 0x00583700 _$E121
// FUNCTION: STBC 0x00583710 _$E120
// FUNCTION: STBC 0x00583720 _$E254
// FUNCTION: STBC 0x00583730 _$E253
// FUNCTION: STBC 0x00583740 _$E261
// FUNCTION: STBC 0x00583750 _$E260
// FUNCTION: STBC 0x00583760 _$E284
// FUNCTION: STBC 0x00583770 _$E283
// FUNCTION: STBC 0x00583780 _$E288
// FUNCTION: STBC 0x00583790 _$E287
// FUNCTION: STBC 0x005837a0 _$E328
// FUNCTION: STBC 0x005837b0 _$E327
// FUNCTION: STBC 0x005837c0 _$E334
// FUNCTION: STBC 0x005837d0 _$E333
// FUNCTION: STBC 0x005837e0 _$E276
// FUNCTION: STBC 0x005837f0 _$E275
// FUNCTION: STBC 0x00583800 _$E280
// FUNCTION: STBC 0x00583810 _$E279
// FUNCTION: STBC 0x00583820 _$E292
// FUNCTION: STBC 0x00583830 _$E291
// FUNCTION: STBC 0x00583840 _$E296
// FUNCTION: STBC 0x00583850 _$E295
// FUNCTION: STBC 0x00583860 _$E300
// FUNCTION: STBC 0x00583870 _$E299
// FUNCTION: STBC 0x00583880 _$E304
// FUNCTION: STBC 0x00583890 _$E303
// FUNCTION: STBC 0x005838a0 _$E308
// FUNCTION: STBC 0x005838b0 _$E307
// FUNCTION: STBC 0x005838c0 _$E312
// FUNCTION: STBC 0x005838d0 _$E311
// FUNCTION: STBC 0x005838e0 _$E316
// FUNCTION: STBC 0x005838f0 _$E315
// FUNCTION: STBC 0x00583900 _$E320
// FUNCTION: STBC 0x00583910 _$E319
// FUNCTION: STBC 0x00583920 _$E126
// FUNCTION: STBC 0x00583930 _$E125
// FUNCTION: STBC 0x00583940 _$E324
// FUNCTION: STBC 0x00583950 _$E323
// FUNCTION: STBC 0x00583960 _$E131
// FUNCTION: STBC 0x00583970 _$E130
// FUNCTION: STBC 0x00583980 _$E338
// FUNCTION: STBC 0x00583990 _$E337
// FUNCTION: STBC 0x005839a0 _$E342
// FUNCTION: STBC 0x005839b0 _$E341
// FUNCTION: STBC 0x005839c0 _$E346
// FUNCTION: STBC 0x005839d0 _$E345
// FUNCTION: STBC 0x005839e0 _$E350
// FUNCTION: STBC 0x005839f0 _$E349
// FUNCTION: STBC 0x00583a30 _$E140
// FUNCTION: STBC 0x00583a40 _$E139
// FUNCTION: STBC 0x00583a50 _$E354
// FUNCTION: STBC 0x00583a60 _$E353
// FUNCTION: STBC 0x00583a70 _$E358
// FUNCTION: STBC 0x00583a80 _$E357
// FUNCTION: STBC 0x00583a90 _$E362
// FUNCTION: STBC 0x00583aa0 _$E361
// FUNCTION: STBC 0x00583ab0 _$E146
// FUNCTION: STBC 0x00583ac0 _$E145
// FUNCTION: STBC 0x00583ad0 _$E151
// FUNCTION: STBC 0x00583ae0 _$E150
// FUNCTION: STBC 0x00583af0 _$E181
// FUNCTION: STBC 0x00583b00 _$E180
// FUNCTION: STBC 0x00583b10 _$E366
// FUNCTION: STBC 0x00583b20 _$E365
// FUNCTION: STBC 0x00583b30 _$E186
// FUNCTION: STBC 0x00583b40 _$E185
// FUNCTION: STBC 0x00583b50 _$E370
// FUNCTION: STBC 0x00583b60 _$E369
// FUNCTION: STBC 0x00583b70 _$E374
// FUNCTION: STBC 0x00583b80 _$E373
// FUNCTION: STBC 0x00583b90 _$E191
// FUNCTION: STBC 0x00583ba0 _$E190
// FUNCTION: STBC 0x00583bb0 _$E378
// FUNCTION: STBC 0x00583bc0 _$E377
// FUNCTION: STBC 0x00583bd0 _$E156
// FUNCTION: STBC 0x00583be0 _$E155
// FUNCTION: STBC 0x00583bf0 _$E382
// FUNCTION: STBC 0x00583c00 _$E381
// FUNCTION: STBC 0x00583c10 _$E386
// FUNCTION: STBC 0x00583c20 _$E385
// FUNCTION: STBC 0x00583c30 _$E196
// FUNCTION: STBC 0x00583c40 _$E195
// FUNCTION: STBC 0x00583c50 _$E201
// FUNCTION: STBC 0x00583c60 _$E200
// FUNCTION: STBC 0x00583c70 _$E390
// FUNCTION: STBC 0x00583c80 _$E389
// FUNCTION: STBC 0x00583c90 _$E394
// FUNCTION: STBC 0x00583ca0 _$E393
// FUNCTION: STBC 0x00583cb0 _$E398
// FUNCTION: STBC 0x00583cc0 _$E397
// FUNCTION: STBC 0x00583cd0 _$E402
// FUNCTION: STBC 0x00583ce0 _$E401
// FUNCTION: STBC 0x00583cf0 _$E406
// FUNCTION: STBC 0x00583d00 _$E405
// FUNCTION: STBC 0x00583d10 _$E410
// FUNCTION: STBC 0x00583d20 _$E409
// FUNCTION: STBC 0x00583d30 _$E414
// FUNCTION: STBC 0x00583d40 _$E413
// FUNCTION: STBC 0x00583d50 _$E418
// FUNCTION: STBC 0x00583d60 _$E417
// FUNCTION: STBC 0x00583d70 _$E206
// FUNCTION: STBC 0x00583d80 _$E205
// FUNCTION: STBC 0x00583d90 _$E422
// FUNCTION: STBC 0x00583da0 _$E421
// FUNCTION: STBC 0x00583db0 _$E211
// FUNCTION: STBC 0x00583dc0 _$E210
// FUNCTION: STBC 0x00583dd0 _$E216
// FUNCTION: STBC 0x00583de0 _$E215
// FUNCTION: STBC 0x00583df0 _$E426
// FUNCTION: STBC 0x00583e00 _$E425
// FUNCTION: STBC 0x00583e10 _$E431
// FUNCTION: STBC 0x00583e20 _$E430
// FUNCTION: STBC 0x00583e30 _$E435
// FUNCTION: STBC 0x00583e40 _$E434

/*
================
Weapon::Cast

The object as a Weapon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00583200
Weapon *Weapon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WEAPON_TYPE ) ) {
		return (Weapon *)object;
	}
	return NULL;
}

/*
================
Weapon::GetProperty
================
*/
// FUNCTION: STBC 0x00583230
WeaponProperty *Weapon::GetProperty( void ) {
	return (WeaponProperty *)property;
}

/*
================
Weapon::IsMemberOfGroup
================
*/
// FUNCTION: STBC 0x00583240
bool Weapon::IsMemberOfGroup( int group ) {
	return GetProperty()->IsMemberOfGroup( group );
}

/*
================
Weapon::GetDamageRadiusFactor
================
*/
// FUNCTION: STBC 0x00583260
float Weapon::GetDamageRadiusFactor( void ) {
	return GetProperty()->GetDamageRadiusFactor();
}

/*
================
Weapon::IsDumbFire
================
*/
// FUNCTION: STBC 0x00583270
bool Weapon::IsDumbFire( void ) {
	return GetProperty()->IsDumbfire();
}

/*
================
Weapon::Weapon

Not firing and without a target.
================
*/
// FUNCTION: STBC 0x00583280
Weapon::Weapon( long id ) : ShipSubsystem( id ) {
	firing = false;
	unknown9C = fmod( rand() * ( 1.0f / RAND_MAX ), FIRING_INTERVAL );
}

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005831b0 _$E441
// FUNCTION: STBC 0x005831c0 _$E437
// FUNCTION: STBC 0x005831d0 _$E440
// FUNCTION: STBC 0x005831e0 _$E438
// DATA: STBC 0x0098d1d0
TGEventHandlerTable Weapon::classHandlers;

// DATA: STBC 0x00893830
const float Weapon::FIRING_INTERVAL = 0.33f;

/*
================
Weapon::GetObjType
================
*/
// FUNCTION: STBC 0x005832f0
int Weapon::GetObjType( void ) {
	return WEAPON_TYPE;
}

/*
================
Weapon::GetClassName
================
*/
// FUNCTION: STBC 0x00583350
char *Weapon::GetClassName( void ) {
	return "Weapon";
}

/*
================
Weapon::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00583360
char *Weapon::GetSwigTypeName( void ) {
	return "_p_Weapon";
}

/*
================
Weapon::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00583370
char *Weapon::GetPythonClassName( void ) {
	return "WeaponPtr";
}

/*
================
Weapon::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00583380
TGEventHandlerTable *Weapon::GetClassHandlers( void ) {
	return &classHandlers;
}
/*
================
Weapon::CalculateWeaponAppeal

A plain weapon has no appeal.
================
*/
// FUNCTION: STBC 0x00583390
float Weapon::CalculateWeaponAppeal( void ) {
	return 0.0f;
}

/*
================
Weapon::~Weapon
================
*/
// FUNCTION: STBC 0x005833d0
// FUNCTION: STBC 0x005833a0 ??_GWeapon@@UAEPAXI@Z
Weapon::~Weapon( void ) {
}

/*
================
Weapon::Update
================
*/
// FUNCTION: STBC 0x005833e0
void Weapon::Update( float deltaTime ) {
	ShipSubsystem::Update( deltaTime );
}

/*
================
Weapon::StopFiring
================
*/
// FUNCTION: STBC 0x005833f0
void Weapon::StopFiring( void ) {
	target.targetID = 0;
}

/*
================
Weapon::WriteToStream
================
*/
// FUNCTION: STBC 0x00583400
bool Weapon::WriteToStream( TGStream *stream ) {
	if ( !ShipSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( unknown9C );
	target.WriteToStream( stream );
	return true;
}

/*
================
Weapon::ReadFromStream

A loaded weapon is not firing.
================
*/
// FUNCTION: STBC 0x00583440
bool Weapon::ReadFromStream( TGStream *stream ) {
	if ( !ShipSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	firing = false;
	unknown9C = stream->ReadFloat();
	target.ReadFromStream( stream );
	return true;
}
