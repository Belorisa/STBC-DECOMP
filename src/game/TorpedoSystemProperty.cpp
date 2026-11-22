/*
===============================================================================

	TorpedoSystemProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>

#include "TorpedoSystemProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00693500 _$E32
// FUNCTION: STBC 0x00693510 _$E29
// FUNCTION: STBC 0x00693520 _$E31
// FUNCTION: STBC 0x00693530 _$E30
// FUNCTION: STBC 0x00693540 _$E38
// FUNCTION: STBC 0x00693550 _$E35
// FUNCTION: STBC 0x00693560 _$E37
// FUNCTION: STBC 0x00693570 _$E36
// FUNCTION: STBC 0x006939c0 _$E433
// FUNCTION: STBC 0x006939e0 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00693580 _$E65
// FUNCTION: STBC 0x00693590 _$E64
// FUNCTION: STBC 0x006935a0 _$E69
// FUNCTION: STBC 0x006935b0 _$E68
// FUNCTION: STBC 0x006935c0 _$E80
// FUNCTION: STBC 0x006935d0 _$E79
// FUNCTION: STBC 0x006935e0 _$E84
// FUNCTION: STBC 0x006935f0 _$E83
// FUNCTION: STBC 0x00693600 _$E88
// FUNCTION: STBC 0x00693610 _$E87
// FUNCTION: STBC 0x00693620 _$E92
// FUNCTION: STBC 0x00693630 _$E91
// FUNCTION: STBC 0x00693640 _$E211
// FUNCTION: STBC 0x00693650 _$E210
// FUNCTION: STBC 0x00693660 _$E153
// FUNCTION: STBC 0x00693670 _$E152
// FUNCTION: STBC 0x00693680 _$E166
// FUNCTION: STBC 0x00693690 _$E165
// FUNCTION: STBC 0x006936a0 _$E216
// FUNCTION: STBC 0x006936b0 _$E215
// FUNCTION: STBC 0x006936c0 _$E221
// FUNCTION: STBC 0x006936d0 _$E220
// FUNCTION: STBC 0x006936e0 _$E225
// FUNCTION: STBC 0x006936f0 _$E224
// FUNCTION: STBC 0x00693700 _$E229
// FUNCTION: STBC 0x00693710 _$E228
// FUNCTION: STBC 0x00693720 _$E236
// FUNCTION: STBC 0x00693730 _$E235
// FUNCTION: STBC 0x00693740 _$E258
// FUNCTION: STBC 0x00693750 _$E257
// FUNCTION: STBC 0x00693760 _$E262
// FUNCTION: STBC 0x00693770 _$E261
// FUNCTION: STBC 0x00693780 _$E111
// FUNCTION: STBC 0x00693790 _$E110
// FUNCTION: STBC 0x006937a0 _$E244
// FUNCTION: STBC 0x006937b0 _$E243
// FUNCTION: STBC 0x006937c0 _$E251
// FUNCTION: STBC 0x006937d0 _$E250
// FUNCTION: STBC 0x006937e0 _$E274
// FUNCTION: STBC 0x006937f0 _$E273
// FUNCTION: STBC 0x00693800 _$E278
// FUNCTION: STBC 0x00693810 _$E277
// FUNCTION: STBC 0x00693820 _$E318
// FUNCTION: STBC 0x00693830 _$E317
// FUNCTION: STBC 0x00693840 _$E324
// FUNCTION: STBC 0x00693850 _$E323
// FUNCTION: STBC 0x00693860 _$E266
// FUNCTION: STBC 0x00693870 _$E265
// FUNCTION: STBC 0x00693880 _$E270
// FUNCTION: STBC 0x00693890 _$E269
// FUNCTION: STBC 0x006938a0 _$E282
// FUNCTION: STBC 0x006938b0 _$E281
// FUNCTION: STBC 0x006938c0 _$E286
// FUNCTION: STBC 0x006938d0 _$E285
// FUNCTION: STBC 0x006938e0 _$E290
// FUNCTION: STBC 0x006938f0 _$E289
// FUNCTION: STBC 0x00693900 _$E294
// FUNCTION: STBC 0x00693910 _$E293
// FUNCTION: STBC 0x00693920 _$E298
// FUNCTION: STBC 0x00693930 _$E297
// FUNCTION: STBC 0x00693940 _$E302
// FUNCTION: STBC 0x00693950 _$E301
// FUNCTION: STBC 0x00693960 _$E306
// FUNCTION: STBC 0x00693970 _$E305
// FUNCTION: STBC 0x00693980 _$E310
// FUNCTION: STBC 0x00693990 _$E309
// FUNCTION: STBC 0x006939a0 _$E116
// FUNCTION: STBC 0x006939b0 _$E115
// FUNCTION: STBC 0x006939f0 _$E314
// FUNCTION: STBC 0x00693a00 _$E313
// FUNCTION: STBC 0x00693a10 _$E121
// FUNCTION: STBC 0x00693a20 _$E120
// FUNCTION: STBC 0x00693a30 _$E328
// FUNCTION: STBC 0x00693a40 _$E327
// FUNCTION: STBC 0x00693a50 _$E332
// FUNCTION: STBC 0x00693a60 _$E331
// FUNCTION: STBC 0x00693a70 _$E336
// FUNCTION: STBC 0x00693a80 _$E335
// FUNCTION: STBC 0x00693a90 _$E340
// FUNCTION: STBC 0x00693aa0 _$E339
// FUNCTION: STBC 0x00693ab0 _$E130
// FUNCTION: STBC 0x00693ac0 _$E129
// FUNCTION: STBC 0x00693ad0 _$E344
// FUNCTION: STBC 0x00693ae0 _$E343
// FUNCTION: STBC 0x00693af0 _$E348
// FUNCTION: STBC 0x00693b00 _$E347
// FUNCTION: STBC 0x00693b10 _$E352
// FUNCTION: STBC 0x00693b20 _$E351
// FUNCTION: STBC 0x00693b30 _$E136
// FUNCTION: STBC 0x00693b40 _$E135
// FUNCTION: STBC 0x00693b50 _$E141
// FUNCTION: STBC 0x00693b60 _$E140
// FUNCTION: STBC 0x00693b70 _$E171
// FUNCTION: STBC 0x00693b80 _$E170
// FUNCTION: STBC 0x00693b90 _$E356
// FUNCTION: STBC 0x00693ba0 _$E355
// FUNCTION: STBC 0x00693bb0 _$E176
// FUNCTION: STBC 0x00693bc0 _$E175
// FUNCTION: STBC 0x00693bd0 _$E360
// FUNCTION: STBC 0x00693be0 _$E359
// FUNCTION: STBC 0x00693bf0 _$E364
// FUNCTION: STBC 0x00693c00 _$E363
// FUNCTION: STBC 0x00693c10 _$E181
// FUNCTION: STBC 0x00693c20 _$E180
// FUNCTION: STBC 0x00693c30 _$E368
// FUNCTION: STBC 0x00693c40 _$E367
// FUNCTION: STBC 0x00693c50 _$E146
// FUNCTION: STBC 0x00693c60 _$E145
// FUNCTION: STBC 0x00693c70 _$E372
// FUNCTION: STBC 0x00693c80 _$E371
// FUNCTION: STBC 0x00693c90 _$E376
// FUNCTION: STBC 0x00693ca0 _$E375
// FUNCTION: STBC 0x00693cb0 _$E186
// FUNCTION: STBC 0x00693cc0 _$E185
// FUNCTION: STBC 0x00693cd0 _$E191
// FUNCTION: STBC 0x00693ce0 _$E190
// FUNCTION: STBC 0x00693cf0 _$E380
// FUNCTION: STBC 0x00693d00 _$E379
// FUNCTION: STBC 0x00693d10 _$E384
// FUNCTION: STBC 0x00693d20 _$E383
// FUNCTION: STBC 0x00693d30 _$E388
// FUNCTION: STBC 0x00693d40 _$E387
// FUNCTION: STBC 0x00693d50 _$E392
// FUNCTION: STBC 0x00693d60 _$E391
// FUNCTION: STBC 0x00693d70 _$E396
// FUNCTION: STBC 0x00693d80 _$E395
// FUNCTION: STBC 0x00693d90 _$E400
// FUNCTION: STBC 0x00693da0 _$E399
// FUNCTION: STBC 0x00693db0 _$E404
// FUNCTION: STBC 0x00693dc0 _$E403
// FUNCTION: STBC 0x00693dd0 _$E408
// FUNCTION: STBC 0x00693de0 _$E407
// FUNCTION: STBC 0x00693df0 _$E196
// FUNCTION: STBC 0x00693e00 _$E195
// FUNCTION: STBC 0x00693e10 _$E412
// FUNCTION: STBC 0x00693e20 _$E411
// FUNCTION: STBC 0x00693e30 _$E201
// FUNCTION: STBC 0x00693e40 _$E200
// FUNCTION: STBC 0x00693e50 _$E206
// FUNCTION: STBC 0x00693e60 _$E205
// FUNCTION: STBC 0x00693e70 _$E416
// FUNCTION: STBC 0x00693e80 _$E415
// FUNCTION: STBC 0x00693e90 _$E421
// FUNCTION: STBC 0x00693ea0 _$E420
// FUNCTION: STBC 0x00693eb0 _$E425
// FUNCTION: STBC 0x00693ec0 _$E424

/*
================
TorpedoSystemProperty::Cast

The property as a TorpedoSystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00693ed0
TorpedoSystemProperty *TorpedoSystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( TORPEDOSYSTEMPROPERTY_TYPE ) ) {
		return (TorpedoSystemProperty *)property;
	}
	return NULL;
}

/*
================
TorpedoSystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00693f00
TorpedoSystemProperty *TorpedoSystemProperty::Create( char *name ) {
	TorpedoSystemProperty *property = new TorpedoSystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
TorpedoSystemProperty::TorpedoSystemProperty
================
*/
// FUNCTION: STBC 0x00693f60
TorpedoSystemProperty::TorpedoSystemProperty( long id ) : WeaponSystemProperty( id ) {
	weaponSystemType = WST_TORPEDO;
	numAmmoTypes = 1;
}

/*
================
TorpedoSystemProperty::WriteScript

Writes "<variable> = App.TorpedoSystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00694070
void TorpedoSystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.TorpedoSystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
TorpedoSystemProperty::~TorpedoSystemProperty
================
*/
// FUNCTION: STBC 0x006941a0
TorpedoSystemProperty::~TorpedoSystemProperty( void ) {
}

/*
================
TorpedoSystemProperty::Copy
================
*/
// FUNCTION: STBC 0x00694370
bool TorpedoSystemProperty::Copy( TGModelProperty *property ) {
	TorpedoSystemProperty *torpedoSystem = Cast( property );
	bool copied = WeaponSystemProperty::Copy( property );
	if ( copied == true ) {
		for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
			SetMaxTorpedoes( i, torpedoSystem->GetMaxTorpedoes( i ) );
			SetTorpedoScript( i, torpedoSystem->GetTorpedoScript( i ) );
		}
		SetNumAmmoTypes( torpedoSystem->GetNumAmmoTypes() );
	}
	return copied;
}

/*
================
TorpedoSystemProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x006943e0
void TorpedoSystemProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	WeaponSystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
TorpedoSystemProperty::WriteInit

Writes the count and script of each torpedo type in use, then the number of
types.
================
*/
// FUNCTION: STBC 0x00694400
void TorpedoSystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	char *line = new char[2048];

	WeaponSystemProperty::WriteInit( stream, variableName );

	for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
		if ( i < numAmmoTypes ) {
			sprintf( line, "%s.SetMaxTorpedoes(%d, %d)\n", variableName, i, GetMaxTorpedoes( i ) );
			stream->WriteCLine( line );
			sprintf( line, "%s.SetTorpedoScript(%d, \"%s\")\n", variableName, i, GetTorpedoScript( i ) );
			stream->WriteCLine( line );
		}
	}
	sprintf( line, "%s.SetNumAmmoTypes(%d)\n", variableName, numAmmoTypes );
	stream->WriteCLine( line );

	delete line;
}

/*
================
TorpedoSystemProperty::GetAmmoType

The torpedo type with the given number (invented name).
================
*/
// FUNCTION: STBC 0x006944c0
TorpedoAmmoType *TorpedoSystemProperty::GetAmmoType( int index ) {
	return &ammoTypes[index];
}

/*
================
TorpedoSystemProperty::GetMaxTorpedoes

How many torpedoes of the type the system carries.
================
*/
// FUNCTION: STBC 0x006944d0
int TorpedoSystemProperty::GetMaxTorpedoes( int index ) {
	return ammoTypes[index].GetMaxTorpedoes();
}

/*
================
TorpedoSystemProperty::GetTorpedoScript

The script that describes the torpedoes of the type.
================
*/
// FUNCTION: STBC 0x006944e0
char *TorpedoSystemProperty::GetTorpedoScript( int index ) {
	return ammoTypes[index].GetTorpedoScript();
}

/*
================
TorpedoSystemProperty::SetMaxTorpedoes
================
*/
// FUNCTION: STBC 0x006944f0
void TorpedoSystemProperty::SetMaxTorpedoes( int index, int maxTorpedoes ) {
	ammoTypes[index].SetMaxTorpedoes( maxTorpedoes );
}

/*
================
TorpedoSystemProperty::SetTorpedoScript

Replaces the script that describes the torpedoes of the type.
================
*/
// FUNCTION: STBC 0x00694510
void TorpedoSystemProperty::SetTorpedoScript( int index, char *script ) {
	ammoTypes[index].SetTorpedoScript( script );
}

/*
================
TorpedoSystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00694530
bool TorpedoSystemProperty::WriteToStream( TGStream *stream ) {
	if ( !WeaponSystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( numAmmoTypes );
	for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
		stream->WriteInt( ammoTypes[i].maxTorpedoes );
		stream->VirtualSlot11( ammoTypes[i].torpedoScript );
		stream->WriteFloat( ammoTypes[i].launchSpeed );
		stream->VirtualSlot11( ammoTypes[i].launchSound );
	}
	return true;
}

/*
================
TorpedoSystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x006945a0
bool TorpedoSystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !WeaponSystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	numAmmoTypes = stream->ReadLong();
	for ( int i = 0; i < MAX_AMMO_TYPES; i++ ) {
		ammoTypes[i].maxTorpedoes = stream->ReadInt();
		ammoTypes[i].torpedoScript = stream->VirtualSlot10( -1 );
		ammoTypes[i].launchSpeed = stream->ReadFloat();
		ammoTypes[i].launchSound = stream->VirtualSlot10( -1 );
	}
	return true;
}
