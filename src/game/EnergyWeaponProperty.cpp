/*
===============================================================================

	EnergyWeaponProperty.cpp

===============================================================================
*/

#include <iostream>

#include <string.h>

#include "EnergyWeaponProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiExtraData.h"
#include "NiObject.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00681080 _$E32
// FUNCTION: STBC 0x00681090 _$E29
// FUNCTION: STBC 0x006810a0 _$E31
// FUNCTION: STBC 0x006810b0 _$E30
// FUNCTION: STBC 0x006810c0 _$E38
// FUNCTION: STBC 0x006810d0 _$E35
// FUNCTION: STBC 0x006810e0 _$E37
// FUNCTION: STBC 0x006810f0 _$E36

/*
================
EnergyWeaponProperty::Cast

The property as a EnergyWeaponProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00681150 ?Cast@EnergyWeaponProperty@@SAPAV1@PAVTGModelProperty@@@Z
EnergyWeaponProperty *EnergyWeaponProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( ENERGYWEAPONPROPERTY_TYPE ) ) {
		return (EnergyWeaponProperty *)property;
	}
	return NULL;
}

/*
================
EnergyWeaponProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00681180
EnergyWeaponProperty *EnergyWeaponProperty::Create( char *name ) {
	EnergyWeaponProperty *property = new EnergyWeaponProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
EnergyWeaponProperty::EnergyWeaponProperty
================
*/
// FUNCTION: STBC 0x006811e0
EnergyWeaponProperty::EnergyWeaponProperty( long id ) : WeaponProperty( id ) {
	maxCharge = 5.0f;
	rechargeRate = 0.3f;
	normalDischargeRate = 1.0f;
	minFiringCharge = 3.0f;
	maxDamage = 25.0f;
	maxDamageDistance = 100.0f;
	fireSound = NULL;
	indicatorIconNum = 0;
	indicatorIconPositionX = 0.0f;
	indicatorIconPositionY = 0.0f;
}

/*
================
EnergyWeaponProperty::WriteScript

Writes "<variable> = App.EnergyWeaponProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x006812c0
void EnergyWeaponProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.EnergyWeaponProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
EnergyWeaponProperty::~EnergyWeaponProperty
================
*/
// FUNCTION: STBC 0x006813f0
EnergyWeaponProperty::~EnergyWeaponProperty( void ) {
	if ( fireSound != NULL ) {
		delete fireSound;
		fireSound = NULL;
	}
}

/*
================
EnergyWeaponProperty::SetFireSound

Keeps a copy of the name of the firing sound; setting the name it already
holds does nothing.
================
*/
// FUNCTION: STBC 0x00681420 ?SetFireSound@EnergyWeaponProperty@@QAEXPAD@Z
void EnergyWeaponProperty::SetFireSound( char *sound ) {
	if ( sound != fireSound ) {
		if ( fireSound != NULL ) {
			delete [] fireSound;
			fireSound = NULL;
		}
		if ( sound != NULL ) {
			fireSound = new char[strlen( sound ) + 1];
			strcpy( fireSound, sound );
		}
	}
}

/*
================
EnergyWeaponProperty::Copy
================
*/
// FUNCTION: STBC 0x00681490
bool EnergyWeaponProperty::Copy( TGModelProperty *property ) {
	EnergyWeaponProperty *energyWeapon = Cast( property );
	bool copied = WeaponProperty::Copy( property );
	if ( copied == true ) {
		SetMaxDamage( energyWeapon->GetMaxDamage() );
		SetMaxDamageDistance( energyWeapon->GetMaxDamageDistance() );
		SetMinFiringCharge( energyWeapon->GetMinFiringCharge() );
		SetMaxCharge( energyWeapon->GetMaxCharge() );
		SetNormalDischargeRate( energyWeapon->GetNormalDischargeRate() );
		SetRechargeRate( energyWeapon->GetRechargeRate() );
		SetFireSound( energyWeapon->GetFireSound() );
		SetIndicatorIconNum( energyWeapon->GetIndicatorIconNum() );
		SetIndicatorIconPositionX( energyWeapon->GetIndicatorIconPositionX() );
		SetIndicatorIconPositionY( energyWeapon->GetIndicatorIconPositionY() );
	}
	return copied;
}

/*
================
EnergyWeaponProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00681520
void EnergyWeaponProperty::WriteInit( TGStream *stream, char *variableName ) {
	WeaponProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetFireSound(\"%s\")\n", variableName, GetFireSound() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxCharge(%f)\n", variableName, GetMaxCharge() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxDamage(%f)\n", variableName, GetMaxDamage() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxDamageDistance(%f)\n", variableName, GetMaxDamageDistance() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMinFiringCharge(%f)\n", variableName, GetMinFiringCharge() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetNormalDischargeRate(%f)\n", variableName, GetNormalDischargeRate() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRechargeRate(%f)\n", variableName, GetRechargeRate() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIndicatorIconNum(%d)\n", variableName, GetIndicatorIconNum() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIndicatorIconPositionX(%f)\n", variableName, GetIndicatorIconPositionX() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIndicatorIconPositionY(%f)\n", variableName, GetIndicatorIconPositionY() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
EnergyWeaponProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x006816c0
bool EnergyWeaponProperty::WriteToStream( TGStream *stream ) {
	if ( !WeaponProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( maxCharge );
	stream->WriteFloat( rechargeRate );
	stream->WriteFloat( normalDischargeRate );
	stream->WriteFloat( minFiringCharge );
	stream->WriteFloat( maxDamage );
	stream->WriteFloat( maxDamageDistance );
	stream->VirtualSlot11( fireSound );
	stream->WriteInt( indicatorIconNum );
	stream->WriteFloat( indicatorIconPositionX );
	stream->WriteFloat( indicatorIconPositionY );
	return true;
}

/*
================
EnergyWeaponProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00681760
bool EnergyWeaponProperty::ReadFromStream( TGStream *stream ) {
	if ( !WeaponProperty::ReadFromStream( stream ) ) {
		return false;
	}
	maxCharge = stream->ReadFloat();
	rechargeRate = stream->ReadFloat();
	normalDischargeRate = stream->ReadFloat();
	minFiringCharge = stream->ReadFloat();
	maxDamage = stream->ReadFloat();
	maxDamageDistance = stream->ReadFloat();
	fireSound = stream->VirtualSlot10( -1 );
	indicatorIconNum = stream->ReadInt();
	indicatorIconPositionX = stream->ReadFloat();
	indicatorIconPositionY = stream->ReadFloat();
	return true;
}
