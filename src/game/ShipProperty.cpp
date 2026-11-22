/*
===============================================================================

	ShipProperty.cpp

===============================================================================
*/

#include <iostream>

#include "ShipProperty.h"
#include "TGStream.h"

#include <stdio.h>

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006907c0 _$E32
// FUNCTION: STBC 0x006907d0 _$E29
// FUNCTION: STBC 0x006907e0 _$E31
// FUNCTION: STBC 0x006907f0 _$E30
// FUNCTION: STBC 0x00690800 _$E38
// FUNCTION: STBC 0x00690810 _$E35
// FUNCTION: STBC 0x00690820 _$E37
// FUNCTION: STBC 0x00690830 _$E36
// FUNCTION: STBC 0x00690ca0 _$E429
// FUNCTION: STBC 0x00690cc0 _$E428

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00690840 _$E53
// FUNCTION: STBC 0x00690850 _$E52
// FUNCTION: STBC 0x00690860 _$E57
// FUNCTION: STBC 0x00690870 _$E56
// FUNCTION: STBC 0x00690880 _$E68
// FUNCTION: STBC 0x00690890 _$E67
// FUNCTION: STBC 0x006908a0 _$E72
// FUNCTION: STBC 0x006908b0 _$E71
// FUNCTION: STBC 0x006908c0 _$E76
// FUNCTION: STBC 0x006908d0 _$E75
// FUNCTION: STBC 0x006908e0 _$E80
// FUNCTION: STBC 0x006908f0 _$E79
// FUNCTION: STBC 0x00690900 _$E204
// FUNCTION: STBC 0x00690910 _$E203
// FUNCTION: STBC 0x00690920 _$E146
// FUNCTION: STBC 0x00690930 _$E145
// FUNCTION: STBC 0x00690940 _$E159
// FUNCTION: STBC 0x00690950 _$E158
// FUNCTION: STBC 0x00690960 _$E209
// FUNCTION: STBC 0x00690970 _$E208
// FUNCTION: STBC 0x00690980 _$E214
// FUNCTION: STBC 0x00690990 _$E213
// FUNCTION: STBC 0x006909a0 _$E218
// FUNCTION: STBC 0x006909b0 _$E217
// FUNCTION: STBC 0x006909c0 _$E222
// FUNCTION: STBC 0x006909d0 _$E221
// FUNCTION: STBC 0x006909e0 _$E229
// FUNCTION: STBC 0x006909f0 _$E228
// FUNCTION: STBC 0x00690a00 _$E254
// FUNCTION: STBC 0x00690a10 _$E253
// FUNCTION: STBC 0x00690a20 _$E258
// FUNCTION: STBC 0x00690a30 _$E257
// FUNCTION: STBC 0x00690a40 _$E104
// FUNCTION: STBC 0x00690a50 _$E103
// FUNCTION: STBC 0x00690a60 _$E240
// FUNCTION: STBC 0x00690a70 _$E239
// FUNCTION: STBC 0x00690a80 _$E247
// FUNCTION: STBC 0x00690a90 _$E246
// FUNCTION: STBC 0x00690aa0 _$E270
// FUNCTION: STBC 0x00690ab0 _$E269
// FUNCTION: STBC 0x00690ac0 _$E274
// FUNCTION: STBC 0x00690ad0 _$E273
// FUNCTION: STBC 0x00690ae0 _$E314
// FUNCTION: STBC 0x00690af0 _$E313
// FUNCTION: STBC 0x00690b00 _$E320
// FUNCTION: STBC 0x00690b10 _$E319
// FUNCTION: STBC 0x00690b20 _$E262
// FUNCTION: STBC 0x00690b30 _$E261
// FUNCTION: STBC 0x00690b40 _$E266
// FUNCTION: STBC 0x00690b50 _$E265
// FUNCTION: STBC 0x00690b60 _$E278
// FUNCTION: STBC 0x00690b70 _$E277
// FUNCTION: STBC 0x00690b80 _$E282
// FUNCTION: STBC 0x00690b90 _$E281
// FUNCTION: STBC 0x00690ba0 _$E286
// FUNCTION: STBC 0x00690bb0 _$E285
// FUNCTION: STBC 0x00690bc0 _$E290
// FUNCTION: STBC 0x00690bd0 _$E289
// FUNCTION: STBC 0x00690be0 _$E294
// FUNCTION: STBC 0x00690bf0 _$E293
// FUNCTION: STBC 0x00690c00 _$E298
// FUNCTION: STBC 0x00690c10 _$E297
// FUNCTION: STBC 0x00690c20 _$E302
// FUNCTION: STBC 0x00690c30 _$E301
// FUNCTION: STBC 0x00690c40 _$E306
// FUNCTION: STBC 0x00690c50 _$E305
// FUNCTION: STBC 0x00690c60 _$E109
// FUNCTION: STBC 0x00690c70 _$E108
// FUNCTION: STBC 0x00690c80 _$E310
// FUNCTION: STBC 0x00690c90 _$E309
// FUNCTION: STBC 0x00690cd0 _$E114
// FUNCTION: STBC 0x00690ce0 _$E113
// FUNCTION: STBC 0x00690cf0 _$E324
// FUNCTION: STBC 0x00690d00 _$E323
// FUNCTION: STBC 0x00690d10 _$E328
// FUNCTION: STBC 0x00690d20 _$E327
// FUNCTION: STBC 0x00690d30 _$E332
// FUNCTION: STBC 0x00690d40 _$E331
// FUNCTION: STBC 0x00690d50 _$E336
// FUNCTION: STBC 0x00690d60 _$E335
// FUNCTION: STBC 0x00690d70 _$E123
// FUNCTION: STBC 0x00690d80 _$E122
// FUNCTION: STBC 0x00690d90 _$E340
// FUNCTION: STBC 0x00690da0 _$E339
// FUNCTION: STBC 0x00690db0 _$E344
// FUNCTION: STBC 0x00690dc0 _$E343
// FUNCTION: STBC 0x00690dd0 _$E348
// FUNCTION: STBC 0x00690de0 _$E347
// FUNCTION: STBC 0x00690df0 _$E129
// FUNCTION: STBC 0x00690e00 _$E128
// FUNCTION: STBC 0x00690e10 _$E134
// FUNCTION: STBC 0x00690e20 _$E133
// FUNCTION: STBC 0x00690e30 _$E164
// FUNCTION: STBC 0x00690e40 _$E163
// FUNCTION: STBC 0x00690e50 _$E352
// FUNCTION: STBC 0x00690e60 _$E351
// FUNCTION: STBC 0x00690e70 _$E169
// FUNCTION: STBC 0x00690e80 _$E168
// FUNCTION: STBC 0x00690e90 _$E356
// FUNCTION: STBC 0x00690ea0 _$E355
// FUNCTION: STBC 0x00690eb0 _$E360
// FUNCTION: STBC 0x00690ec0 _$E359
// FUNCTION: STBC 0x00690ed0 _$E174
// FUNCTION: STBC 0x00690ee0 _$E173
// FUNCTION: STBC 0x00690ef0 _$E364
// FUNCTION: STBC 0x00690f00 _$E363
// FUNCTION: STBC 0x00690f10 _$E139
// FUNCTION: STBC 0x00690f20 _$E138
// FUNCTION: STBC 0x00690f30 _$E368
// FUNCTION: STBC 0x00690f40 _$E367
// FUNCTION: STBC 0x00690f50 _$E372
// FUNCTION: STBC 0x00690f60 _$E371
// FUNCTION: STBC 0x00690f70 _$E179
// FUNCTION: STBC 0x00690f80 _$E178
// FUNCTION: STBC 0x00690f90 _$E184
// FUNCTION: STBC 0x00690fa0 _$E183
// FUNCTION: STBC 0x00690fb0 _$E376
// FUNCTION: STBC 0x00690fc0 _$E375
// FUNCTION: STBC 0x00690fd0 _$E380
// FUNCTION: STBC 0x00690fe0 _$E379
// FUNCTION: STBC 0x00690ff0 _$E384
// FUNCTION: STBC 0x00691000 _$E383
// FUNCTION: STBC 0x00691010 _$E388
// FUNCTION: STBC 0x00691020 _$E387
// FUNCTION: STBC 0x00691030 _$E392
// FUNCTION: STBC 0x00691040 _$E391
// FUNCTION: STBC 0x00691050 _$E396
// FUNCTION: STBC 0x00691060 _$E395
// FUNCTION: STBC 0x00691070 _$E400
// FUNCTION: STBC 0x00691080 _$E399
// FUNCTION: STBC 0x00691090 _$E404
// FUNCTION: STBC 0x006910a0 _$E403
// FUNCTION: STBC 0x006910b0 _$E189
// FUNCTION: STBC 0x006910c0 _$E188
// FUNCTION: STBC 0x006910d0 _$E408
// FUNCTION: STBC 0x006910e0 _$E407
// FUNCTION: STBC 0x006910f0 _$E194
// FUNCTION: STBC 0x00691100 _$E193
// FUNCTION: STBC 0x00691110 _$E199
// FUNCTION: STBC 0x00691120 _$E198
// FUNCTION: STBC 0x00691130 _$E412
// FUNCTION: STBC 0x00691140 _$E411
// FUNCTION: STBC 0x00691150 _$E417
// FUNCTION: STBC 0x00691160 _$E416
// FUNCTION: STBC 0x00691170 _$E421
// FUNCTION: STBC 0x00691180 _$E420

/*
================
ShipProperty::Cast

The property as a ShipProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00691190
ShipProperty *ShipProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SHIPPROPERTY_TYPE ) ) {
		return (ShipProperty *)property;
	}
	return NULL;
}

/*
================
ShipProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x006911c0
ShipProperty *ShipProperty::Create( char *name ) {
	ShipProperty *property = new ShipProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
ShipProperty::ShipProperty
================
*/
// FUNCTION: STBC 0x00691210
ShipProperty::ShipProperty( long id ) : TGModelProperty( id ) {
	genus = GENUS_UNKNOWN;
	species = 0;
	mass = 1.0f;
	rotationalInertia = 1.0f;
	shipName = NULL;
	modelFilename = NULL;
	damageResolution = 15.0f;
	affiliation = 0;
	stationary = false;
	aiString = new char[strlen( "" ) + 1];
	strcpy( aiString, "" );
	deathExplosionSound = new char[strlen( "" ) + 1];
	strcpy( deathExplosionSound, "" );
}

/*
================
ShipProperty::WriteScript

Writes "<variable> = App.ShipProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00691370
void ShipProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.ShipProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
ShipProperty::~ShipProperty
================
*/
// FUNCTION: STBC 0x006914a0
ShipProperty::~ShipProperty( void ) {
	if ( shipName != NULL ) {
		delete shipName;
		shipName = NULL;
	}
	if ( modelFilename != NULL ) {
		delete modelFilename;
		modelFilename = NULL;
	}
	if ( aiString != NULL ) {
		delete aiString;
		aiString = NULL;
	}
	if ( deathExplosionSound != NULL ) {
		delete deathExplosionSound;
		deathExplosionSound = NULL;
	}
}

/*
================
ShipProperty::Copy
================
*/
// FUNCTION: STBC 0x00691510
bool ShipProperty::Copy( TGModelProperty *property ) {
	ShipProperty *ship = Cast( property );
	bool copied = TGModelProperty::Copy( property );
	if ( copied == true ) {
		SetGenus( ship->GetGenus() );
		SetSpecies( ship->GetSpecies() );
		SetMass( ship->GetMass() );
		SetRotationalInertia( ship->GetRotationalInertia() );
		SetShipName( ship->GetShipName() );
		SetModelFilename( ship->GetModelFilename() );
		SetDamageResolution( ship->GetDamageResolution() );
		SetAffiliation( ship->GetAffiliation() );
		SetStationary( ship->IsStationary() );
		SetAIString( ship->GetAIString() );
		SetDeathExplosionSound( ship->GetDeathExplosionSound() );
	}
	return copied;
}

/*
================
ShipProperty::VirtualSlot15

A ship property has nothing to show on the model.
================
*/
// FUNCTION: STBC 0x006915a0
void ShipProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
}

/*
================
ShipProperty::SetShipName

Keeps a copy of the name.
================
*/
// FUNCTION: STBC 0x006915b0
void ShipProperty::SetShipName( char *name ) {
	if ( shipName != NULL ) {
		delete shipName;
		shipName = NULL;
	}
	if ( name != NULL ) {
		shipName = new char[strlen( name ) + 1];
		strcpy( shipName, name );
	}
}

/*
================
ShipProperty::SetModelFilename

Keeps a copy of the file name.
================
*/
// FUNCTION: STBC 0x00691620
void ShipProperty::SetModelFilename( char *filename ) {
	if ( modelFilename != NULL ) {
		delete modelFilename;
		modelFilename = NULL;
	}
	if ( filename != NULL ) {
		modelFilename = new char[strlen( filename ) + 1];
		strcpy( modelFilename, filename );
	}
}

/*
================
ShipProperty::SetAIString

Keeps a copy of the string.
================
*/
// FUNCTION: STBC 0x00691690
void ShipProperty::SetAIString( char *string ) {
	if ( aiString != NULL ) {
		delete aiString;
		aiString = NULL;
	}
	if ( string != NULL ) {
		aiString = new char[strlen( string ) + 1];
		strcpy( aiString, string );
	}
}

/*
================
ShipProperty::SetDeathExplosionSound

Keeps a copy of the sound's name.
================
*/
// FUNCTION: STBC 0x00691700
void ShipProperty::SetDeathExplosionSound( char *sound ) {
	if ( deathExplosionSound != NULL ) {
		delete deathExplosionSound;
		deathExplosionSound = NULL;
	}
	if ( sound != NULL ) {
		deathExplosionSound = new char[strlen( sound ) + 1];
		strcpy( deathExplosionSound, sound );
	}
}

/*
================
ShipProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00691770
void ShipProperty::WriteInit( TGStream *stream, char *variableName ) {
	TGModelProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetGenus(%d)\n", variableName, GetGenus() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetSpecies(%d)\n", variableName, GetSpecies() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMass(%f)\n", variableName, GetMass() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRotationalInertia(%f)\n", variableName, GetRotationalInertia() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShipName(\"%s\")\n", variableName, GetShipName() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetModelFilename(\"%s\")\n", variableName, GetModelFilename() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDamageResolution(%f)\n", variableName, GetDamageResolution() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetAffiliation(%d)\n", variableName, GetAffiliation() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetStationary(%d)\n", variableName, IsStationary() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetAIString(\"%s\")\n", variableName, GetAIString() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDeathExplosionSound(\"%s\")\n", variableName, GetDeathExplosionSound() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
ShipProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x006918e0
bool ShipProperty::WriteToStream( TGStream *stream ) {
	if ( !TGModelProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( genus );
	stream->WriteLong( species );
	stream->WriteFloat( mass );
	stream->WriteFloat( rotationalInertia );
	stream->VirtualSlot11( shipName );
	stream->VirtualSlot11( modelFilename );
	stream->WriteFloat( damageResolution );
	stream->WriteLong( affiliation );
	stream->WriteBool( stationary );
	stream->VirtualSlot11( aiString );
	stream->VirtualSlot11( deathExplosionSound );
	return true;
}

/*
================
ShipProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00691980
bool ShipProperty::ReadFromStream( TGStream *stream ) {
	if ( !TGModelProperty::ReadFromStream( stream ) ) {
		return false;
	}
	genus = (genus_t)stream->ReadLong();
	species = stream->ReadLong();
	mass = stream->ReadFloat();
	rotationalInertia = stream->ReadFloat();
	shipName = stream->VirtualSlot10( -1 );
	modelFilename = stream->VirtualSlot10( -1 );
	damageResolution = stream->ReadFloat();
	affiliation = stream->ReadLong();
	stationary = stream->ReadBool();
	aiString = stream->VirtualSlot10( -1 );
	deathExplosionSound = stream->VirtualSlot10( -1 );
	return true;
}
