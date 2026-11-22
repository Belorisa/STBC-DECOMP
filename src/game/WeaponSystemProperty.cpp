/*
===============================================================================

	WeaponSystemProperty.cpp

===============================================================================
*/

#include <iostream>

#include "WeaponSystemProperty.h"
#include "TGStream.h"
#include <string.h>

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0069a590 _$E32
// FUNCTION: STBC 0x0069a5a0 _$E29
// FUNCTION: STBC 0x0069a5b0 _$E31
// FUNCTION: STBC 0x0069a5c0 _$E30
// FUNCTION: STBC 0x0069a5d0 _$E38
// FUNCTION: STBC 0x0069a5e0 _$E35
// FUNCTION: STBC 0x0069a5f0 _$E37
// FUNCTION: STBC 0x0069a600 _$E36
// FUNCTION: STBC 0x0069aa10 _$E433
// FUNCTION: STBC 0x0069aa30 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0069a610 _$E65
// FUNCTION: STBC 0x0069a620 _$E64
// FUNCTION: STBC 0x0069a630 _$E69
// FUNCTION: STBC 0x0069a640 _$E68
// FUNCTION: STBC 0x0069a650 _$E80
// FUNCTION: STBC 0x0069a660 _$E79
// FUNCTION: STBC 0x0069a670 _$E84
// FUNCTION: STBC 0x0069a680 _$E83
// FUNCTION: STBC 0x0069a690 _$E88
// FUNCTION: STBC 0x0069a6a0 _$E87
// FUNCTION: STBC 0x0069a6b0 _$E92
// FUNCTION: STBC 0x0069a6c0 _$E91
// FUNCTION: STBC 0x0069a6d0 _$E211
// FUNCTION: STBC 0x0069a6e0 _$E210
// FUNCTION: STBC 0x0069a6f0 _$E153
// FUNCTION: STBC 0x0069a700 _$E152
// FUNCTION: STBC 0x0069a710 _$E166
// FUNCTION: STBC 0x0069a720 _$E165
// FUNCTION: STBC 0x0069a730 _$E216
// FUNCTION: STBC 0x0069a740 _$E215
// FUNCTION: STBC 0x0069a750 _$E221
// FUNCTION: STBC 0x0069a760 _$E220
// FUNCTION: STBC 0x0069a770 _$E225
// FUNCTION: STBC 0x0069a780 _$E224
// FUNCTION: STBC 0x0069a790 _$E229
// FUNCTION: STBC 0x0069a7a0 _$E228
// FUNCTION: STBC 0x0069a7b0 _$E236
// FUNCTION: STBC 0x0069a7c0 _$E235
// FUNCTION: STBC 0x0069a7d0 _$E258
// FUNCTION: STBC 0x0069a7e0 _$E257
// FUNCTION: STBC 0x0069a7f0 _$E262
// FUNCTION: STBC 0x0069a800 _$E261
// FUNCTION: STBC 0x0069a810 _$E111
// FUNCTION: STBC 0x0069a820 _$E110
// FUNCTION: STBC 0x0069a830 _$E244
// FUNCTION: STBC 0x0069a840 _$E243
// FUNCTION: STBC 0x0069a850 _$E251
// FUNCTION: STBC 0x0069a860 _$E250
// FUNCTION: STBC 0x0069a870 _$E274
// FUNCTION: STBC 0x0069a880 _$E273
// FUNCTION: STBC 0x0069a890 _$E278
// FUNCTION: STBC 0x0069a8a0 _$E277
// FUNCTION: STBC 0x0069a8b0 _$E318
// FUNCTION: STBC 0x0069a8c0 _$E317
// FUNCTION: STBC 0x0069a8d0 _$E324
// FUNCTION: STBC 0x0069a8e0 _$E323
// FUNCTION: STBC 0x0069a8f0 _$E266
// FUNCTION: STBC 0x0069a900 _$E265
// FUNCTION: STBC 0x0069a910 _$E270
// FUNCTION: STBC 0x0069a920 _$E269
// FUNCTION: STBC 0x0069a930 _$E282
// FUNCTION: STBC 0x0069a940 _$E281
// FUNCTION: STBC 0x0069a950 _$E286
// FUNCTION: STBC 0x0069a960 _$E285
// FUNCTION: STBC 0x0069a970 _$E290
// FUNCTION: STBC 0x0069a980 _$E289
// FUNCTION: STBC 0x0069a990 _$E294
// FUNCTION: STBC 0x0069a9a0 _$E293
// FUNCTION: STBC 0x0069a9b0 _$E298
// FUNCTION: STBC 0x0069a9c0 _$E297
// FUNCTION: STBC 0x0069a9d0 _$E302
// FUNCTION: STBC 0x0069a9e0 _$E301
// FUNCTION: STBC 0x0069a9f0 _$E306
// FUNCTION: STBC 0x0069aa00 _$E305
// FUNCTION: STBC 0x0069aa40 _$E310
// FUNCTION: STBC 0x0069aa50 _$E309
// FUNCTION: STBC 0x0069aa60 _$E116
// FUNCTION: STBC 0x0069aa70 _$E115
// FUNCTION: STBC 0x0069aa80 _$E314
// FUNCTION: STBC 0x0069aa90 _$E313
// FUNCTION: STBC 0x0069aaa0 _$E121
// FUNCTION: STBC 0x0069aab0 _$E120
// FUNCTION: STBC 0x0069aac0 _$E328
// FUNCTION: STBC 0x0069aad0 _$E327
// FUNCTION: STBC 0x0069aae0 _$E332
// FUNCTION: STBC 0x0069aaf0 _$E331
// FUNCTION: STBC 0x0069ab00 _$E336
// FUNCTION: STBC 0x0069ab10 _$E335
// FUNCTION: STBC 0x0069ab20 _$E340
// FUNCTION: STBC 0x0069ab30 _$E339
// FUNCTION: STBC 0x0069ab40 _$E130
// FUNCTION: STBC 0x0069ab50 _$E129
// FUNCTION: STBC 0x0069ab60 _$E344
// FUNCTION: STBC 0x0069ab70 _$E343
// FUNCTION: STBC 0x0069ab80 _$E348
// FUNCTION: STBC 0x0069ab90 _$E347
// FUNCTION: STBC 0x0069aba0 _$E352
// FUNCTION: STBC 0x0069abb0 _$E351
// FUNCTION: STBC 0x0069abc0 _$E136
// FUNCTION: STBC 0x0069abd0 _$E135
// FUNCTION: STBC 0x0069abe0 _$E141
// FUNCTION: STBC 0x0069abf0 _$E140
// FUNCTION: STBC 0x0069ac00 _$E171
// FUNCTION: STBC 0x0069ac10 _$E170
// FUNCTION: STBC 0x0069ac20 _$E356
// FUNCTION: STBC 0x0069ac30 _$E355
// FUNCTION: STBC 0x0069ac40 _$E176
// FUNCTION: STBC 0x0069ac50 _$E175
// FUNCTION: STBC 0x0069ac60 _$E360
// FUNCTION: STBC 0x0069ac70 _$E359
// FUNCTION: STBC 0x0069ac80 _$E364
// FUNCTION: STBC 0x0069ac90 _$E363
// FUNCTION: STBC 0x0069aca0 _$E181
// FUNCTION: STBC 0x0069acb0 _$E180
// FUNCTION: STBC 0x0069acc0 _$E368
// FUNCTION: STBC 0x0069acd0 _$E367
// FUNCTION: STBC 0x0069ace0 _$E146
// FUNCTION: STBC 0x0069acf0 _$E145
// FUNCTION: STBC 0x0069ad00 _$E372
// FUNCTION: STBC 0x0069ad10 _$E371
// FUNCTION: STBC 0x0069ad20 _$E376
// FUNCTION: STBC 0x0069ad30 _$E375
// FUNCTION: STBC 0x0069ad40 _$E186
// FUNCTION: STBC 0x0069ad50 _$E185
// FUNCTION: STBC 0x0069ad60 _$E191
// FUNCTION: STBC 0x0069ad70 _$E190
// FUNCTION: STBC 0x0069ad80 _$E380
// FUNCTION: STBC 0x0069ad90 _$E379
// FUNCTION: STBC 0x0069ada0 _$E384
// FUNCTION: STBC 0x0069adb0 _$E383
// FUNCTION: STBC 0x0069adc0 _$E388
// FUNCTION: STBC 0x0069add0 _$E387
// FUNCTION: STBC 0x0069ade0 _$E392
// FUNCTION: STBC 0x0069adf0 _$E391
// FUNCTION: STBC 0x0069ae00 _$E396
// FUNCTION: STBC 0x0069ae10 _$E395
// FUNCTION: STBC 0x0069ae20 _$E400
// FUNCTION: STBC 0x0069ae30 _$E399
// FUNCTION: STBC 0x0069ae40 _$E404
// FUNCTION: STBC 0x0069ae50 _$E403
// FUNCTION: STBC 0x0069ae60 _$E408
// FUNCTION: STBC 0x0069ae70 _$E407
// FUNCTION: STBC 0x0069ae80 _$E196
// FUNCTION: STBC 0x0069ae90 _$E195
// FUNCTION: STBC 0x0069aea0 _$E412
// FUNCTION: STBC 0x0069aeb0 _$E411
// FUNCTION: STBC 0x0069aec0 _$E201
// FUNCTION: STBC 0x0069aed0 _$E200
// FUNCTION: STBC 0x0069aee0 _$E206
// FUNCTION: STBC 0x0069aef0 _$E205
// FUNCTION: STBC 0x0069af00 _$E416
// FUNCTION: STBC 0x0069af10 _$E415
// FUNCTION: STBC 0x0069af20 _$E421
// FUNCTION: STBC 0x0069af30 _$E420
// FUNCTION: STBC 0x0069af40 _$E425
// FUNCTION: STBC 0x0069af50 _$E424

/*
================
WeaponSystemProperty::Cast

The property as a WeaponSystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0069af60 ?Cast@WeaponSystemProperty@@SAPAV1@PAVTGModelProperty@@@Z
WeaponSystemProperty *WeaponSystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( WEAPONSYSTEMPROPERTY_TYPE ) ) {
		return (WeaponSystemProperty *)property;
	}
	return NULL;
}

/*
================
WeaponSystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0069af90
WeaponSystemProperty *WeaponSystemProperty::Create( char *name ) {
	WeaponSystemProperty *property = new WeaponSystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
WeaponSystemProperty::WeaponSystemProperty
================
*/
// FUNCTION: STBC 0x0069afe0
WeaponSystemProperty::WeaponSystemProperty( long id ) : PoweredSubsystemProperty( id ) {
	weaponSystemType = WST_UNKNOWN;
	singleFire = true;
	aimedWeapon = false;
	firingChainString.AssignC( "" );
}

/*
================
WeaponSystemProperty::WriteScript

Writes "<variable> = App.WeaponSystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0069b0e0
void WeaponSystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.WeaponSystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
WeaponSystemProperty::~WeaponSystemProperty
================
*/
// FUNCTION: STBC 0x0069b210
WeaponSystemProperty::~WeaponSystemProperty( void ) {
}

/*
================
WeaponSystemProperty::Copy
================
*/
// FUNCTION: STBC 0x0069b270
bool WeaponSystemProperty::Copy( TGModelProperty *property ) {
	WeaponSystemProperty *weaponSystem = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetWeaponSystemType( weaponSystem->GetWeaponSystemType() );
		SetSingleFire( weaponSystem->IsSingleFire() );
		SetAimedWeapon( weaponSystem->IsAimedWeapon() );
		SetFiringChainString( &weaponSystem->GetFiringChainString() );
	}
	return copied;
}

/*
================
WeaponSystemProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0069b310
void WeaponSystemProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	PoweredSubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
WeaponSystemProperty::WriteInit

The firing chain goes through a script TGString, kFiringChainString.
================
*/
// FUNCTION: STBC 0x0069b330
void WeaponSystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	char *line = new char[2048];
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	sprintf( line, "%s.SetWeaponSystemType(%s.", variableName, variableName );
	switch ( GetWeaponSystemType() ) {
		case WST_UNKNOWN:
			strcat( line, "WST_UNKNOWN)\n" );
			break;
		case WST_PHASER:
			strcat( line, "WST_PHASER)\n" );
			break;
		case WST_TORPEDO:
			strcat( line, "WST_TORPEDO)\n" );
			break;
		case WST_PULSE:
			strcat( line, "WST_PULSE)\n" );
			break;
		case WST_TRACTOR:
			strcat( line, "WST_TRACTOR)\n" );
			break;
	}
	stream->WriteCLine( line );
	sprintf( line, "%s.SetSingleFire(%d)\n", variableName, IsSingleFire() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetAimedWeapon(%d)\n", variableName, IsAimedWeapon() == true );
	stream->WriteCLine( line );

	char *chain = GetFiringChainString().Unknown6F56F0();
	sprintf( line, "kFiringChainString = App.TGString()\n" );
	stream->WriteCLine( line );
	sprintf( line, "kFiringChainString.SetString(\"%s\")\n", chain );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetFiringChainString(kFiringChainString)\n", variableName );
	stream->WriteCLine( line );

	delete[] chain;
	delete line;
}

/*
================
WeaponSystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0069b4e0
bool WeaponSystemProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( weaponSystemType );
	stream->WriteBool( singleFire );
	stream->WriteBool( aimedWeapon );
	firingChainString.WriteToStream( stream );
	return true;
}

/*
================
WeaponSystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0069b530
bool WeaponSystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	weaponSystemType = stream->ReadLong();
	singleFire = stream->ReadBool();
	aimedWeapon = stream->ReadBool();
	firingChainString.ReadFromStream( stream );
	return true;
}
