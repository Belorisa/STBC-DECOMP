/*
===============================================================================

	CloakingSubsystemProperty.cpp

===============================================================================
*/

#include "CloakingSubsystemProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067fce0 _$E433
// FUNCTION: STBC 0x0067fd00 _$E432

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067f8a0 _$E419
// FUNCTION: STBC 0x0067f8b0 _$E416
// FUNCTION: STBC 0x0067f8c0 _$E418
// FUNCTION: STBC 0x0067f8d0 _$E417
// FUNCTION: STBC 0x0067f8e0 _$E425
// FUNCTION: STBC 0x0067f8f0 _$E422
// FUNCTION: STBC 0x0067f900 _$E424
// FUNCTION: STBC 0x0067f910 _$E423

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067f920 _$E54
// FUNCTION: STBC 0x0067f930 _$E53
// FUNCTION: STBC 0x0067f940 _$E58
// FUNCTION: STBC 0x0067f950 _$E57
// FUNCTION: STBC 0x0067f960 _$E69
// FUNCTION: STBC 0x0067f970 _$E68
// FUNCTION: STBC 0x0067f980 _$E73
// FUNCTION: STBC 0x0067f990 _$E72
// FUNCTION: STBC 0x0067f9a0 _$E77
// FUNCTION: STBC 0x0067f9b0 _$E76
// FUNCTION: STBC 0x0067f9c0 _$E81
// FUNCTION: STBC 0x0067f9d0 _$E80
// FUNCTION: STBC 0x0067f9e0 _$E200
// FUNCTION: STBC 0x0067f9f0 _$E199
// FUNCTION: STBC 0x0067fa00 _$E142
// FUNCTION: STBC 0x0067fa10 _$E141
// FUNCTION: STBC 0x0067fa20 _$E155
// FUNCTION: STBC 0x0067fa30 _$E154
// FUNCTION: STBC 0x0067fa40 _$E205
// FUNCTION: STBC 0x0067fa50 _$E204
// FUNCTION: STBC 0x0067fa60 _$E210
// FUNCTION: STBC 0x0067fa70 _$E209
// FUNCTION: STBC 0x0067fa80 _$E214
// FUNCTION: STBC 0x0067fa90 _$E213
// FUNCTION: STBC 0x0067faa0 _$E218
// FUNCTION: STBC 0x0067fab0 _$E217
// FUNCTION: STBC 0x0067fac0 _$E225
// FUNCTION: STBC 0x0067fad0 _$E224
// FUNCTION: STBC 0x0067fae0 _$E247
// FUNCTION: STBC 0x0067faf0 _$E246
// FUNCTION: STBC 0x0067fb00 _$E251
// FUNCTION: STBC 0x0067fb10 _$E250
// FUNCTION: STBC 0x0067fb20 _$E100
// FUNCTION: STBC 0x0067fb30 _$E99
// FUNCTION: STBC 0x0067fb40 _$E233
// FUNCTION: STBC 0x0067fb50 _$E232
// FUNCTION: STBC 0x0067fb60 _$E240
// FUNCTION: STBC 0x0067fb70 _$E239
// FUNCTION: STBC 0x0067fb80 _$E263
// FUNCTION: STBC 0x0067fb90 _$E262
// FUNCTION: STBC 0x0067fba0 _$E267
// FUNCTION: STBC 0x0067fbb0 _$E266
// FUNCTION: STBC 0x0067fbc0 _$E307
// FUNCTION: STBC 0x0067fbd0 _$E306
// FUNCTION: STBC 0x0067fbe0 _$E313
// FUNCTION: STBC 0x0067fbf0 _$E312
// FUNCTION: STBC 0x0067fc00 _$E255
// FUNCTION: STBC 0x0067fc10 _$E254
// FUNCTION: STBC 0x0067fc20 _$E259
// FUNCTION: STBC 0x0067fc30 _$E258
// FUNCTION: STBC 0x0067fc40 _$E271
// FUNCTION: STBC 0x0067fc50 _$E270
// FUNCTION: STBC 0x0067fc60 _$E275
// FUNCTION: STBC 0x0067fc70 _$E274
// FUNCTION: STBC 0x0067fc80 _$E279
// FUNCTION: STBC 0x0067fc90 _$E278
// FUNCTION: STBC 0x0067fca0 _$E283
// FUNCTION: STBC 0x0067fcb0 _$E282
// FUNCTION: STBC 0x0067fcc0 _$E287
// FUNCTION: STBC 0x0067fcd0 _$E286
// FUNCTION: STBC 0x0067fd10 _$E291
// FUNCTION: STBC 0x0067fd20 _$E290
// FUNCTION: STBC 0x0067fd30 _$E295
// FUNCTION: STBC 0x0067fd40 _$E294
// FUNCTION: STBC 0x0067fd50 _$E299
// FUNCTION: STBC 0x0067fd60 _$E298
// FUNCTION: STBC 0x0067fd70 _$E105
// FUNCTION: STBC 0x0067fd80 _$E104
// FUNCTION: STBC 0x0067fd90 _$E303
// FUNCTION: STBC 0x0067fda0 _$E302
// FUNCTION: STBC 0x0067fdb0 _$E110
// FUNCTION: STBC 0x0067fdc0 _$E109
// FUNCTION: STBC 0x0067fdd0 _$E317
// FUNCTION: STBC 0x0067fde0 _$E316
// FUNCTION: STBC 0x0067fdf0 _$E321
// FUNCTION: STBC 0x0067fe00 _$E320
// FUNCTION: STBC 0x0067fe10 _$E325
// FUNCTION: STBC 0x0067fe20 _$E324
// FUNCTION: STBC 0x0067fe30 _$E329
// FUNCTION: STBC 0x0067fe40 _$E328
// FUNCTION: STBC 0x0067fe50 _$E119
// FUNCTION: STBC 0x0067fe60 _$E118
// FUNCTION: STBC 0x0067fe70 _$E333
// FUNCTION: STBC 0x0067fe80 _$E332
// FUNCTION: STBC 0x0067fe90 _$E337
// FUNCTION: STBC 0x0067fea0 _$E336
// FUNCTION: STBC 0x0067feb0 _$E341
// FUNCTION: STBC 0x0067fec0 _$E340
// FUNCTION: STBC 0x0067fed0 _$E125
// FUNCTION: STBC 0x0067fee0 _$E124
// FUNCTION: STBC 0x0067fef0 _$E130
// FUNCTION: STBC 0x0067ff00 _$E129
// FUNCTION: STBC 0x0067ff10 _$E160
// FUNCTION: STBC 0x0067ff20 _$E159
// FUNCTION: STBC 0x0067ff30 _$E345
// FUNCTION: STBC 0x0067ff40 _$E344
// FUNCTION: STBC 0x0067ff50 _$E165
// FUNCTION: STBC 0x0067ff60 _$E164
// FUNCTION: STBC 0x0067ff70 _$E349
// FUNCTION: STBC 0x0067ff80 _$E348
// FUNCTION: STBC 0x0067ff90 _$E353
// FUNCTION: STBC 0x0067ffa0 _$E352
// FUNCTION: STBC 0x0067ffb0 _$E170
// FUNCTION: STBC 0x0067ffc0 _$E169
// FUNCTION: STBC 0x0067ffd0 _$E357
// FUNCTION: STBC 0x0067ffe0 _$E356
// FUNCTION: STBC 0x0067fff0 _$E135
// FUNCTION: STBC 0x00680000 _$E134
// FUNCTION: STBC 0x00680010 _$E361
// FUNCTION: STBC 0x00680020 _$E360
// FUNCTION: STBC 0x00680030 _$E365
// FUNCTION: STBC 0x00680040 _$E364
// FUNCTION: STBC 0x00680050 _$E175
// FUNCTION: STBC 0x00680060 _$E174
// FUNCTION: STBC 0x00680070 _$E180
// FUNCTION: STBC 0x00680080 _$E179
// FUNCTION: STBC 0x00680090 _$E369
// FUNCTION: STBC 0x006800a0 _$E368
// FUNCTION: STBC 0x006800b0 _$E373
// FUNCTION: STBC 0x006800c0 _$E372
// FUNCTION: STBC 0x006800d0 _$E377
// FUNCTION: STBC 0x006800e0 _$E376
// FUNCTION: STBC 0x006800f0 _$E381
// FUNCTION: STBC 0x00680100 _$E380
// FUNCTION: STBC 0x00680110 _$E385
// FUNCTION: STBC 0x00680120 _$E384
// FUNCTION: STBC 0x00680130 _$E389
// FUNCTION: STBC 0x00680140 _$E388
// FUNCTION: STBC 0x00680150 _$E393
// FUNCTION: STBC 0x00680160 _$E392
// FUNCTION: STBC 0x00680170 _$E397
// FUNCTION: STBC 0x00680180 _$E396
// FUNCTION: STBC 0x00680190 _$E185
// FUNCTION: STBC 0x006801a0 _$E184
// FUNCTION: STBC 0x006801b0 _$E401
// FUNCTION: STBC 0x006801c0 _$E400
// FUNCTION: STBC 0x006801d0 _$E190
// FUNCTION: STBC 0x006801e0 _$E189
// FUNCTION: STBC 0x006801f0 _$E195
// FUNCTION: STBC 0x00680200 _$E194
// FUNCTION: STBC 0x00680210 _$E405
// FUNCTION: STBC 0x00680220 _$E404
// FUNCTION: STBC 0x00680230 _$E410
// FUNCTION: STBC 0x00680240 _$E409
// FUNCTION: STBC 0x00680250 _$E414
// FUNCTION: STBC 0x00680260 _$E413

/*
================
CloakingSubsystemProperty::Cast

The property as a CloakingSubsystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00680270 ?Cast@CloakingSubsystemProperty@@SAPAV1@PAVTGModelProperty@@@Z
CloakingSubsystemProperty *CloakingSubsystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( CLOAKINGSUBSYSTEMPROPERTY_TYPE ) ) {
		return (CloakingSubsystemProperty *)property;
	}
	return NULL;
}

/*
================
CloakingSubsystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x006802a0
CloakingSubsystemProperty *CloakingSubsystemProperty::Create( char *name ) {
	CloakingSubsystemProperty *property = new CloakingSubsystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
CloakingSubsystemProperty::CloakingSubsystemProperty
================
*/
// FUNCTION: STBC 0x006802f0
CloakingSubsystemProperty::CloakingSubsystemProperty( long id ) : PoweredSubsystemProperty( id ) {
	cloakStrength = 0.0f;
}

/*
================
CloakingSubsystemProperty::WriteScript

Writes "<variable> = App.CloakingSubsystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00680390
void CloakingSubsystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.CloakingSubsystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
CloakingSubsystemProperty::~CloakingSubsystemProperty
================
*/
// FUNCTION: STBC 0x006804c0
CloakingSubsystemProperty::~CloakingSubsystemProperty( void ) {
}

/*
================
CloakingSubsystemProperty::Copy
================
*/
// FUNCTION: STBC 0x006804d0
bool CloakingSubsystemProperty::Copy( TGModelProperty *property ) {
	CloakingSubsystemProperty *cloakingSubsystem = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetCloakStrength( cloakingSubsystem->GetCloakStrength() );
	}
	return copied;
}

/*
================
CloakingSubsystemProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00680500
void CloakingSubsystemProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	PoweredSubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
CloakingSubsystemProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00680520
void CloakingSubsystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetCloakStrength(%f)\n", variableName, GetCloakStrength() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
CloakingSubsystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00680570
bool CloakingSubsystemProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( cloakStrength );
	return true;
}

/*
================
CloakingSubsystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x006805a0
bool CloakingSubsystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	cloakStrength = stream->ReadFloat();
	return true;
}
