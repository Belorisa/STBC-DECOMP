/*
===============================================================================

	EnergyWeapon.cpp

===============================================================================
*/

#include <iostream>
#include "EnergyWeapon.h"
#include "EnergyWeaponProperty.h"
#include "PoweredSubsystem.h"
#include "TGEventHandlerTable.h"
#include "TGSoundManager.h"
#include "TGStream.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005705e0 _$E452
// FUNCTION: STBC 0x00570600 _$E451

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0056ff80 _$E32
// FUNCTION: STBC 0x0056ff90 _$E29
// FUNCTION: STBC 0x0056ffa0 _$E31
// FUNCTION: STBC 0x0056ffb0 _$E30
// FUNCTION: STBC 0x0056ffc0 _$E38
// FUNCTION: STBC 0x0056ffd0 _$E35
// FUNCTION: STBC 0x0056ffe0 _$E37
// FUNCTION: STBC 0x0056fff0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00570000 _$E78
// FUNCTION: STBC 0x00570010 _$E77
// FUNCTION: STBC 0x00570020 _$E82
// FUNCTION: STBC 0x00570030 _$E81
// FUNCTION: STBC 0x00570040 _$E93
// FUNCTION: STBC 0x00570050 _$E92
// FUNCTION: STBC 0x00570060 _$E97
// FUNCTION: STBC 0x00570070 _$E96
// FUNCTION: STBC 0x00570080 _$E101
// FUNCTION: STBC 0x00570090 _$E100
// FUNCTION: STBC 0x005700a0 _$E105
// FUNCTION: STBC 0x005700b0 _$E104
// FUNCTION: STBC 0x005700c0 _$E224
// FUNCTION: STBC 0x005700d0 _$E223
// FUNCTION: STBC 0x005700e0 _$E166
// FUNCTION: STBC 0x005700f0 _$E165
// FUNCTION: STBC 0x00570100 _$E179
// FUNCTION: STBC 0x00570110 _$E178
// FUNCTION: STBC 0x00570120 _$E229
// FUNCTION: STBC 0x00570130 _$E228
// FUNCTION: STBC 0x00570140 _$E234
// FUNCTION: STBC 0x00570150 _$E233
// FUNCTION: STBC 0x00570160 _$E238
// FUNCTION: STBC 0x00570170 _$E237
// FUNCTION: STBC 0x00570180 _$E242
// FUNCTION: STBC 0x00570190 _$E241
// FUNCTION: STBC 0x005701a0 _$E249
// FUNCTION: STBC 0x005701b0 _$E248
// FUNCTION: STBC 0x005701c0 _$E271
// FUNCTION: STBC 0x005701d0 _$E270
// FUNCTION: STBC 0x005701e0 _$E275
// FUNCTION: STBC 0x005701f0 _$E274
// FUNCTION: STBC 0x00570200 _$E124
// FUNCTION: STBC 0x00570210 _$E123
// FUNCTION: STBC 0x00570220 _$E257
// FUNCTION: STBC 0x00570230 _$E256
// FUNCTION: STBC 0x00570240 _$E264
// FUNCTION: STBC 0x00570250 _$E263
// FUNCTION: STBC 0x00570260 _$E287
// FUNCTION: STBC 0x00570270 _$E286
// FUNCTION: STBC 0x00570280 _$E291
// FUNCTION: STBC 0x00570290 _$E290
// FUNCTION: STBC 0x005702a0 _$E331
// FUNCTION: STBC 0x005702b0 _$E330
// FUNCTION: STBC 0x005702c0 _$E337
// FUNCTION: STBC 0x005702d0 _$E336
// FUNCTION: STBC 0x005702e0 _$E279
// FUNCTION: STBC 0x005702f0 _$E278
// FUNCTION: STBC 0x00570300 _$E283
// FUNCTION: STBC 0x00570310 _$E282
// FUNCTION: STBC 0x00570320 _$E295
// FUNCTION: STBC 0x00570330 _$E294
// FUNCTION: STBC 0x00570340 _$E299
// FUNCTION: STBC 0x00570350 _$E298
// FUNCTION: STBC 0x00570360 _$E303
// FUNCTION: STBC 0x00570370 _$E302
// FUNCTION: STBC 0x00570380 _$E307
// FUNCTION: STBC 0x00570390 _$E306
// FUNCTION: STBC 0x005703a0 _$E311
// FUNCTION: STBC 0x005703b0 _$E310
// FUNCTION: STBC 0x005703c0 _$E315
// FUNCTION: STBC 0x005703d0 _$E314
// FUNCTION: STBC 0x005703e0 _$E319
// FUNCTION: STBC 0x005703f0 _$E318
// FUNCTION: STBC 0x00570400 _$E323
// FUNCTION: STBC 0x00570410 _$E322
// FUNCTION: STBC 0x00570420 _$E129
// FUNCTION: STBC 0x00570430 _$E128
// FUNCTION: STBC 0x00570440 _$E327
// FUNCTION: STBC 0x00570450 _$E326
// FUNCTION: STBC 0x00570460 _$E134
// FUNCTION: STBC 0x00570470 _$E133
// FUNCTION: STBC 0x00570480 _$E341
// FUNCTION: STBC 0x00570490 _$E340
// FUNCTION: STBC 0x005704a0 _$E345
// FUNCTION: STBC 0x005704b0 _$E344
// FUNCTION: STBC 0x005704c0 _$E349
// FUNCTION: STBC 0x005704d0 _$E348
// FUNCTION: STBC 0x005704e0 _$E353
// FUNCTION: STBC 0x005704f0 _$E352
// FUNCTION: STBC 0x00570500 _$E143
// FUNCTION: STBC 0x00570510 _$E142
// FUNCTION: STBC 0x00570520 _$E357
// FUNCTION: STBC 0x00570530 _$E356
// FUNCTION: STBC 0x00570540 _$E361
// FUNCTION: STBC 0x00570550 _$E360
// FUNCTION: STBC 0x00570560 _$E365
// FUNCTION: STBC 0x00570570 _$E364
// FUNCTION: STBC 0x00570580 _$E149
// FUNCTION: STBC 0x00570590 _$E148
// FUNCTION: STBC 0x005705a0 _$E154
// FUNCTION: STBC 0x005705b0 _$E153
// FUNCTION: STBC 0x005705c0 _$E184
// FUNCTION: STBC 0x005705d0 _$E183
// FUNCTION: STBC 0x00570610 _$E369
// FUNCTION: STBC 0x00570620 _$E368
// FUNCTION: STBC 0x00570630 _$E189
// FUNCTION: STBC 0x00570640 _$E188
// FUNCTION: STBC 0x00570650 _$E373
// FUNCTION: STBC 0x00570660 _$E372
// FUNCTION: STBC 0x00570670 _$E377
// FUNCTION: STBC 0x00570680 _$E376
// FUNCTION: STBC 0x00570690 _$E194
// FUNCTION: STBC 0x005706a0 _$E193
// FUNCTION: STBC 0x005706b0 _$E381
// FUNCTION: STBC 0x005706c0 _$E380
// FUNCTION: STBC 0x005706d0 _$E159
// FUNCTION: STBC 0x005706e0 _$E158
// FUNCTION: STBC 0x005706f0 _$E385
// FUNCTION: STBC 0x00570700 _$E384
// FUNCTION: STBC 0x00570710 _$E389
// FUNCTION: STBC 0x00570720 _$E388
// FUNCTION: STBC 0x00570730 _$E199
// FUNCTION: STBC 0x00570740 _$E198
// FUNCTION: STBC 0x00570750 _$E204
// FUNCTION: STBC 0x00570760 _$E203
// FUNCTION: STBC 0x00570770 _$E393
// FUNCTION: STBC 0x00570780 _$E392
// FUNCTION: STBC 0x00570790 _$E397
// FUNCTION: STBC 0x005707a0 _$E396
// FUNCTION: STBC 0x005707b0 _$E401
// FUNCTION: STBC 0x005707c0 _$E400
// FUNCTION: STBC 0x005707d0 _$E405
// FUNCTION: STBC 0x005707e0 _$E404
// FUNCTION: STBC 0x005707f0 _$E409
// FUNCTION: STBC 0x00570800 _$E408
// FUNCTION: STBC 0x00570810 _$E413
// FUNCTION: STBC 0x00570820 _$E412
// FUNCTION: STBC 0x00570830 _$E417
// FUNCTION: STBC 0x00570840 _$E416
// FUNCTION: STBC 0x00570850 _$E421
// FUNCTION: STBC 0x00570860 _$E420
// FUNCTION: STBC 0x00570870 _$E209
// FUNCTION: STBC 0x00570880 _$E208
// FUNCTION: STBC 0x00570890 _$E425
// FUNCTION: STBC 0x005708a0 _$E424
// FUNCTION: STBC 0x005708b0 _$E214
// FUNCTION: STBC 0x005708c0 _$E213
// FUNCTION: STBC 0x005708d0 _$E219
// FUNCTION: STBC 0x005708e0 _$E218
// FUNCTION: STBC 0x005708f0 _$E429
// FUNCTION: STBC 0x00570900 _$E428
// FUNCTION: STBC 0x00570910 _$E434
// FUNCTION: STBC 0x00570920 _$E433
// FUNCTION: STBC 0x00570930 _$E438
// FUNCTION: STBC 0x00570940 _$E437

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0056f850 _$E444
// FUNCTION: STBC 0x0056f860 _$E440
// FUNCTION: STBC 0x0056f870 _$E443
// FUNCTION: STBC 0x0056f880 _$E441

/*
================
EnergyWeapon::Cast

The object as an EnergyWeapon; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0056f8a0
EnergyWeapon *EnergyWeapon::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ENERGYWEAPON_TYPE ) ) {
		return (EnergyWeapon *)object;
	}
	return NULL;
}

/*
================
EnergyWeapon::GetProperty
================
*/
// FUNCTION: STBC 0x0056f8d0
EnergyWeaponProperty *EnergyWeapon::GetProperty( void ) {
	return (EnergyWeaponProperty *)property;
}

/*
================
EnergyWeapon::GetRechargeRate
================
*/
// FUNCTION: STBC 0x0056f8e0
float EnergyWeapon::GetRechargeRate( void ) {
	return GetProperty()->GetRechargeRate();
}

/*
================
EnergyWeapon::GetNormalDischargeRate
================
*/
// FUNCTION: STBC 0x0056f8f0
float EnergyWeapon::GetNormalDischargeRate( void ) {
	return GetProperty()->GetNormalDischargeRate();
}

/*
================
EnergyWeapon::GetMaxCharge
================
*/
// FUNCTION: STBC 0x0056f900
float EnergyWeapon::GetMaxCharge( void ) {
	return GetProperty()->GetMaxCharge();
}

/*
================
EnergyWeapon::GetMinFiringCharge
================
*/
// FUNCTION: STBC 0x0056f910
float EnergyWeapon::GetMinFiringCharge( void ) {
	return GetProperty()->GetMinFiringCharge();
}

/*
================
EnergyWeapon::GetFireSound
================
*/
// FUNCTION: STBC 0x0056f920
char *EnergyWeapon::GetFireSound( void ) {
	return GetProperty()->GetFireSound();
}

/*
================
EnergyWeapon::GetMaxDamage
================
*/
// FUNCTION: STBC 0x0056f930
float EnergyWeapon::GetMaxDamage( void ) {
	return GetProperty()->GetMaxDamage();
}

/*
================
EnergyWeapon::GetMaxDamageDistance
================
*/
// FUNCTION: STBC 0x0056f940
float EnergyWeapon::GetMaxDamageDistance( void ) {
	return GetProperty()->GetMaxDamageDistance();
}

/*
================
EnergyWeapon::EnergyWeapon

Uncharged, at power setting 1, with the charge watcher on the charge
percentage.
================
*/
// FUNCTION: STBC 0x0056f950
EnergyWeapon::EnergyWeapon( long id ) : Weapon( id ) {
	unknownA4 = unknown896A9C;
	unknownA8 = unknown896A9C;
	chargePercentage = 0.0f;
	chargeWatcher.SetVariable( &chargePercentage );
	powerSetting = 1;
	chargeLevel = 0.0f;
	fireStartSound = NULL;
	fireLoopSound = NULL;
}

/*
================
EnergyWeapon::GetObjType
================
*/
// FUNCTION: STBC 0x0056fa50
int EnergyWeapon::GetObjType( void ) {
	return ENERGYWEAPON_TYPE;
}

// DATA: STBC 0x0098c5f0
TGEventHandlerTable EnergyWeapon::classHandlers;

/*
================
EnergyWeapon::GetClassName
================
*/
// FUNCTION: STBC 0x0056fac0
char *EnergyWeapon::GetClassName( void ) {
	return "EnergyWeapon";
}

/*
================
EnergyWeapon::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0056fad0
char *EnergyWeapon::GetSwigTypeName( void ) {
	return "_p_EnergyWeapon";
}

/*
================
EnergyWeapon::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0056fae0
char *EnergyWeapon::GetPythonClassName( void ) {
	return "EnergyWeaponPtr";
}

/*
================
EnergyWeapon::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0056faf0
TGEventHandlerTable *EnergyWeapon::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
EnergyWeapon::SetPowerSetting
================
*/
// FUNCTION: STBC 0x0056fb00
void EnergyWeapon::SetPowerSetting( int powerSetting_ ) {
	powerSetting = powerSetting_;
}

/*
================
EnergyWeapon::CalculateWeaponAppeal

The charge percentage, while the weapon can fire.
================
*/
// FUNCTION: STBC 0x0056fb10
float EnergyWeapon::CalculateWeaponAppeal( void ) {
	if ( CanFire() ) {
		return GetChargePercentage();
	}
	return 0.0f;
}

/*
================
EnergyWeapon::~EnergyWeapon
================
*/
// FUNCTION: STBC 0x0056fb60
// FUNCTION: STBC 0x0056fb30 ??_GEnergyWeapon@@UAEPAXI@Z
EnergyWeapon::~EnergyWeapon( void ) {
	delete fireStartSound;
	delete fireLoopSound;
}

/*
================
EnergyWeapon::SetProperty

A new energy weapon property charges the weapon fully.
================
*/
// FUNCTION: STBC 0x0056fbd0
void EnergyWeapon::SetProperty( SubsystemProperty *property_ ) {
	ShipSubsystem::SetProperty( property_ );

	EnergyWeaponProperty *energyProperty = EnergyWeaponProperty::Cast( property_ );
	if ( energyProperty != NULL ) {
		chargeLevel = energyProperty->GetMaxCharge();
		chargePercentage = 1.0f;
	}
}

/*
================
EnergyWeapon::GetFireStartSound

The fire sound's name followed by " Start", built on first use.
================
*/
// FUNCTION: STBC 0x0056fc10
char *EnergyWeapon::GetFireStartSound( void ) {
	if ( fireStartSound != NULL ) {
		return fireStartSound;
	}

	char *fireSound = GetFireSound();
	if ( fireSound == NULL ) {
		return NULL;
	}
	fireStartSound = new char[strlen( fireSound ) + strlen( " Start" ) + 1];
	strcpy( fireStartSound, fireSound );
	strcat( fireStartSound, " Start" );
	return fireStartSound;
}

/*
================
EnergyWeapon::GetFireLoopSound

The fire sound's name followed by " Loop", built on first use.
================
*/
// FUNCTION: STBC 0x0056fcc0
char *EnergyWeapon::GetFireLoopSound( void ) {
	if ( fireLoopSound != NULL ) {
		return fireLoopSound;
	}

	char *fireSound = GetFireSound();
	if ( fireSound == NULL ) {
		return NULL;
	}
	fireLoopSound = new char[strlen( fireSound ) + strlen( " Loop" ) + 1];
	strcpy( fireLoopSound, fireSound );
	strcat( fireLoopSound, " Loop" );
	return fireLoopSound;
}

/*
================
EnergyWeapon::UpdateCharge

Recharges the weapon over the time given, scaled by its condition and the
power factor given, up to the maximum charge.
================
*/
// FUNCTION: STBC 0x0056fd70
void EnergyWeapon::UpdateCharge( float deltaTime, float powerFactor ) {
	// Retail copies the condition through fld/fstp into its spill before the
	// GetRechargeRate call: the copy level of this named local.
	float condition = GetConditionPercentage();
	chargeLevel += condition * GetRechargeRate() * deltaTime * powerFactor;
	if ( chargeLevel > GetMaxCharge() ) {
		chargeLevel = GetMaxCharge();
	}
}

/*
================
EnergyWeapon::Update

The charge watcher sees the new charge percentage.
================
*/
// FUNCTION: STBC 0x0056fdc0
void EnergyWeapon::Update( float deltaTime ) {
	chargePercentage = chargeLevel / GetMaxCharge();
	chargeWatcher.CheckRanges();
	ShipSubsystem::Update( deltaTime );
}

/*
================
EnergyWeapon::GetChargePercentage

None while the weapon system is off or disabled.
================
*/
// FUNCTION: STBC 0x0056fdf0
float EnergyWeapon::GetChargePercentage( void ) {
	PoweredSubsystem *system = PoweredSubsystem::Cast( parentSubsystem );
	if ( system->IsOn() && system->IsDisabled() != true ) {
		return chargePercentage;
	}
	return 0.0f;
}

/*
================
EnergyWeapon::WriteToStream
================
*/
// FUNCTION: STBC 0x0056fe30
bool EnergyWeapon::WriteToStream( TGStream *stream ) {
	if ( !Weapon::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( chargeLevel );
	stream->WriteID( unknownA4 );
	stream->WriteID( unknownA8 );
	stream->WriteLong( powerSetting );
	if ( !chargeWatcher.WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( chargePercentage );
	return true;
}

/*
================
EnergyWeapon::ReadFromStream
================
*/
// FUNCTION: STBC 0x0056feb0
bool EnergyWeapon::ReadFromStream( TGStream *stream ) {
	if ( !Weapon::ReadFromStream( stream ) ) {
		return false;
	}

	chargeLevel = stream->ReadFloat();
	unknownA4 = stream->ReadID();
	unknownA8 = stream->ReadID();
	powerSetting = stream->ReadLong();
	if ( !chargeWatcher.ReadFromStream( stream ) ) {
		return false;
	}
	chargeWatcher.SetVariable( &chargePercentage );
	chargePercentage = stream->ReadFloat();
	return true;
}

/*
================
EnergyWeapon::FixUpPointers
================
*/
// FUNCTION: STBC 0x0056ff40
bool EnergyWeapon::FixUpPointers( void ) {
	if ( !ShipSubsystem::FixUpPointers() ) {
		return false;
	}
	if ( !chargeWatcher.FixUpPointers() ) {
		return false;
	}
	return true;
}

/*
================
EnergyWeapon::PostFixUp
================
*/
// FUNCTION: STBC 0x0056ff60
void EnergyWeapon::PostFixUp( void ) {
	ShipSubsystem::PostFixUp();
	chargeWatcher.PostFixUp();
}
