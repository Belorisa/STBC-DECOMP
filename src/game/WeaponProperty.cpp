/*
===============================================================================

	WeaponProperty.cpp

===============================================================================
*/

#include <iostream>

#include "WeaponProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006996a0 _$E32
// FUNCTION: STBC 0x006996b0 _$E29
// FUNCTION: STBC 0x006996c0 _$E31
// FUNCTION: STBC 0x006996d0 _$E30
// FUNCTION: STBC 0x006996e0 _$E38
// FUNCTION: STBC 0x006996f0 _$E35
// FUNCTION: STBC 0x00699700 _$E37
// FUNCTION: STBC 0x00699710 _$E36
// FUNCTION: STBC 0x00699b00 _$E433
// FUNCTION: STBC 0x00699b20 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00699720 _$E65
// FUNCTION: STBC 0x00699730 _$E64
// FUNCTION: STBC 0x00699740 _$E69
// FUNCTION: STBC 0x00699750 _$E68
// FUNCTION: STBC 0x00699760 _$E80
// FUNCTION: STBC 0x00699770 _$E79
// FUNCTION: STBC 0x00699780 _$E84
// FUNCTION: STBC 0x00699790 _$E83
// FUNCTION: STBC 0x006997a0 _$E88
// FUNCTION: STBC 0x006997b0 _$E87
// FUNCTION: STBC 0x006997c0 _$E92
// FUNCTION: STBC 0x006997d0 _$E91
// FUNCTION: STBC 0x006997e0 _$E211
// FUNCTION: STBC 0x006997f0 _$E210
// FUNCTION: STBC 0x00699800 _$E153
// FUNCTION: STBC 0x00699810 _$E152
// FUNCTION: STBC 0x00699820 _$E166
// FUNCTION: STBC 0x00699830 _$E165
// FUNCTION: STBC 0x00699840 _$E216
// FUNCTION: STBC 0x00699850 _$E215
// FUNCTION: STBC 0x00699860 _$E221
// FUNCTION: STBC 0x00699870 _$E220
// FUNCTION: STBC 0x00699880 _$E225
// FUNCTION: STBC 0x00699890 _$E224
// FUNCTION: STBC 0x006998a0 _$E229
// FUNCTION: STBC 0x006998b0 _$E228
// FUNCTION: STBC 0x006998c0 _$E236
// FUNCTION: STBC 0x006998d0 _$E235
// FUNCTION: STBC 0x006998e0 _$E258
// FUNCTION: STBC 0x006998f0 _$E257
// FUNCTION: STBC 0x00699900 _$E262
// FUNCTION: STBC 0x00699910 _$E261
// FUNCTION: STBC 0x00699920 _$E111
// FUNCTION: STBC 0x00699930 _$E110
// FUNCTION: STBC 0x00699940 _$E244
// FUNCTION: STBC 0x00699950 _$E243
// FUNCTION: STBC 0x00699960 _$E251
// FUNCTION: STBC 0x00699970 _$E250
// FUNCTION: STBC 0x00699980 _$E274
// FUNCTION: STBC 0x00699990 _$E273
// FUNCTION: STBC 0x006999a0 _$E278
// FUNCTION: STBC 0x006999b0 _$E277
// FUNCTION: STBC 0x006999c0 _$E318
// FUNCTION: STBC 0x006999d0 _$E317
// FUNCTION: STBC 0x006999e0 _$E324
// FUNCTION: STBC 0x006999f0 _$E323
// FUNCTION: STBC 0x00699a00 _$E266
// FUNCTION: STBC 0x00699a10 _$E265
// FUNCTION: STBC 0x00699a20 _$E270
// FUNCTION: STBC 0x00699a30 _$E269
// FUNCTION: STBC 0x00699a40 _$E282
// FUNCTION: STBC 0x00699a50 _$E281
// FUNCTION: STBC 0x00699a60 _$E286
// FUNCTION: STBC 0x00699a70 _$E285
// FUNCTION: STBC 0x00699a80 _$E290
// FUNCTION: STBC 0x00699a90 _$E289
// FUNCTION: STBC 0x00699aa0 _$E294
// FUNCTION: STBC 0x00699ab0 _$E293
// FUNCTION: STBC 0x00699ac0 _$E298
// FUNCTION: STBC 0x00699ad0 _$E297
// FUNCTION: STBC 0x00699ae0 _$E302
// FUNCTION: STBC 0x00699af0 _$E301
// FUNCTION: STBC 0x00699b30 _$E306
// FUNCTION: STBC 0x00699b40 _$E305
// FUNCTION: STBC 0x00699b50 _$E310
// FUNCTION: STBC 0x00699b60 _$E309
// FUNCTION: STBC 0x00699b70 _$E116
// FUNCTION: STBC 0x00699b80 _$E115
// FUNCTION: STBC 0x00699b90 _$E314
// FUNCTION: STBC 0x00699ba0 _$E313
// FUNCTION: STBC 0x00699bb0 _$E121
// FUNCTION: STBC 0x00699bc0 _$E120
// FUNCTION: STBC 0x00699bd0 _$E328
// FUNCTION: STBC 0x00699be0 _$E327
// FUNCTION: STBC 0x00699bf0 _$E332
// FUNCTION: STBC 0x00699c00 _$E331
// FUNCTION: STBC 0x00699c10 _$E336
// FUNCTION: STBC 0x00699c20 _$E335
// FUNCTION: STBC 0x00699c30 _$E340
// FUNCTION: STBC 0x00699c40 _$E339
// FUNCTION: STBC 0x00699c50 _$E130
// FUNCTION: STBC 0x00699c60 _$E129
// FUNCTION: STBC 0x00699c70 _$E344
// FUNCTION: STBC 0x00699c80 _$E343
// FUNCTION: STBC 0x00699c90 _$E348
// FUNCTION: STBC 0x00699ca0 _$E347
// FUNCTION: STBC 0x00699cb0 _$E352
// FUNCTION: STBC 0x00699cc0 _$E351
// FUNCTION: STBC 0x00699cd0 _$E136
// FUNCTION: STBC 0x00699ce0 _$E135
// FUNCTION: STBC 0x00699cf0 _$E141
// FUNCTION: STBC 0x00699d00 _$E140
// FUNCTION: STBC 0x00699d10 _$E171
// FUNCTION: STBC 0x00699d20 _$E170
// FUNCTION: STBC 0x00699d30 _$E356
// FUNCTION: STBC 0x00699d40 _$E355
// FUNCTION: STBC 0x00699d50 _$E176
// FUNCTION: STBC 0x00699d60 _$E175
// FUNCTION: STBC 0x00699d70 _$E360
// FUNCTION: STBC 0x00699d80 _$E359
// FUNCTION: STBC 0x00699d90 _$E364
// FUNCTION: STBC 0x00699da0 _$E363
// FUNCTION: STBC 0x00699db0 _$E181
// FUNCTION: STBC 0x00699dc0 _$E180
// FUNCTION: STBC 0x00699dd0 _$E368
// FUNCTION: STBC 0x00699de0 _$E367
// FUNCTION: STBC 0x00699df0 _$E146
// FUNCTION: STBC 0x00699e00 _$E145
// FUNCTION: STBC 0x00699e10 _$E372
// FUNCTION: STBC 0x00699e20 _$E371
// FUNCTION: STBC 0x00699e30 _$E376
// FUNCTION: STBC 0x00699e40 _$E375
// FUNCTION: STBC 0x00699e50 _$E186
// FUNCTION: STBC 0x00699e60 _$E185
// FUNCTION: STBC 0x00699e70 _$E191
// FUNCTION: STBC 0x00699e80 _$E190
// FUNCTION: STBC 0x00699e90 _$E380
// FUNCTION: STBC 0x00699ea0 _$E379
// FUNCTION: STBC 0x00699eb0 _$E384
// FUNCTION: STBC 0x00699ec0 _$E383
// FUNCTION: STBC 0x00699ed0 _$E388
// FUNCTION: STBC 0x00699ee0 _$E387
// FUNCTION: STBC 0x00699ef0 _$E392
// FUNCTION: STBC 0x00699f00 _$E391
// FUNCTION: STBC 0x00699f10 _$E396
// FUNCTION: STBC 0x00699f20 _$E395
// FUNCTION: STBC 0x00699f30 _$E400
// FUNCTION: STBC 0x00699f40 _$E399
// FUNCTION: STBC 0x00699f50 _$E404
// FUNCTION: STBC 0x00699f60 _$E403
// FUNCTION: STBC 0x00699f70 _$E408
// FUNCTION: STBC 0x00699f80 _$E407
// FUNCTION: STBC 0x00699f90 _$E196
// FUNCTION: STBC 0x00699fa0 _$E195
// FUNCTION: STBC 0x00699fb0 _$E412
// FUNCTION: STBC 0x00699fc0 _$E411
// FUNCTION: STBC 0x00699fd0 _$E201
// FUNCTION: STBC 0x00699fe0 _$E200
// FUNCTION: STBC 0x00699ff0 _$E206
// FUNCTION: STBC 0x0069a000 _$E205
// FUNCTION: STBC 0x0069a010 _$E416
// FUNCTION: STBC 0x0069a020 _$E415
// FUNCTION: STBC 0x0069a030 _$E421
// FUNCTION: STBC 0x0069a040 _$E420
// FUNCTION: STBC 0x0069a050 _$E425
// FUNCTION: STBC 0x0069a060 _$E424

/*
================
WeaponProperty::Cast

The property as a WeaponProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0069a070 ?Cast@WeaponProperty@@SAPAV1@PAVTGModelProperty@@@Z
WeaponProperty *WeaponProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( WEAPONPROPERTY_TYPE ) ) {
		return (WeaponProperty *)property;
	}
	return NULL;
}

/*
================
WeaponProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0069a0a0
WeaponProperty *WeaponProperty::Create( char *name ) {
	WeaponProperty *property = new WeaponProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
WeaponProperty::WeaponProperty
================
*/
// FUNCTION: STBC 0x0069a0f0
WeaponProperty::WeaponProperty( long id ) : SubsystemProperty( id ) {
	dumbfire = false;
	weaponID = 0;
	groups = 0;
	damageRadiusFactor = 0.3f;
	iconNum = 0;
	iconPositionX = 0.0f;
	iconPositionY = 0.0f;
	iconAboveShip = true;
}

/*
================
WeaponProperty::WriteScript

Writes "<variable> = App.WeaponProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0069a1b0
void WeaponProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.WeaponProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
WeaponProperty::~WeaponProperty
================
*/
// FUNCTION: STBC 0x0069a2e0
WeaponProperty::~WeaponProperty( void ) {
}

/*
================
WeaponProperty::Copy
================
*/
// FUNCTION: STBC 0x0069a2f0
bool WeaponProperty::Copy( TGModelProperty *property ) {
	WeaponProperty *weapon = Cast( property );
	bool copied = SubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetDumbfire( weapon->IsDumbfire() );
		SetWeaponID( weapon->GetWeaponID() );
		SetGroups( weapon->GetGroups() );
		SetDamageRadiusFactor( weapon->GetDamageRadiusFactor() );
		SetIconNum( weapon->GetIconNum() );
		SetIconPositionX( weapon->GetIconPositionX() );
		SetIconPositionY( weapon->GetIconPositionY() );
		SetIconAboveShip( weapon->IsIconAboveShip() );
	}
	return copied;
}

/*
================
WeaponProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0069a350
void WeaponProperty::WriteInit( TGStream *stream, char *variableName ) {
	SubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetDumbfire(%d)\n", variableName, IsDumbfire() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetWeaponID(%d)\n", variableName, GetWeaponID() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetGroups(%d)\n", variableName, GetGroups() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDamageRadiusFactor(%f)\n", variableName, GetDamageRadiusFactor() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIconNum(%d)\n", variableName, GetIconNum() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIconPositionX(%f)\n", variableName, GetIconPositionX() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIconPositionY(%f)\n", variableName, GetIconPositionY() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetIconAboveShip(%d)\n", variableName, IsIconAboveShip() == true );
	stream->WriteCLine( line );

	delete line;
}

/*
================
WeaponProperty::IsMemberOfGroup

Whether the weapon fires with group number group (1-based bits of groups);
every weapon is a member of group 0.
================
*/
// FUNCTION: STBC 0x0069a480 ?IsMemberOfGroup@WeaponProperty@@QAE_NH@Z
bool WeaponProperty::IsMemberOfGroup( int group ) {
	if ( group == 0 ) {
		return true;
	}
	return ( groups & ( 1 << ( group - 1 ) ) ) ? true : false;
}

/*
================
WeaponProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0069a4a0
bool WeaponProperty::WriteToStream( TGStream *stream ) {
	if ( !SubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( dumbfire );
	stream->WriteLong( weaponID );
	stream->WriteLong( groups );
	stream->WriteFloat( damageRadiusFactor );
	stream->WriteInt( iconNum );
	stream->WriteFloat( iconPositionX );
	stream->WriteFloat( iconPositionY );
	stream->WriteBool( iconAboveShip );
	return true;
}

/*
================
WeaponProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0069a520
bool WeaponProperty::ReadFromStream( TGStream *stream ) {
	if ( !SubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	dumbfire = stream->ReadBool();
	weaponID = stream->ReadLong();
	groups = stream->ReadLong();
	damageRadiusFactor = stream->ReadFloat();
	iconNum = stream->ReadInt();
	iconPositionX = stream->ReadFloat();
	iconPositionY = stream->ReadFloat();
	iconAboveShip = stream->ReadBool();
	return true;
}
