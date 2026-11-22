/*
===============================================================================

	BlinkingLightProperty.cpp

===============================================================================
*/

#include <stdio.h>
#include <string.h>

#include "BlinkingLightProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).

/*
================
BlinkingLightProperty::Cast

The property as a BlinkingLightProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067b510 ?Cast@BlinkingLightProperty@@SAPAV1@PAVTGModelProperty@@@Z
BlinkingLightProperty *BlinkingLightProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( BLINKINGLIGHTPROPERTY_TYPE ) ) {
		return (BlinkingLightProperty *)property;
	}
	return NULL;
}

/*
================
BlinkingLightProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067b540
BlinkingLightProperty *BlinkingLightProperty::Create( char *name ) {
	BlinkingLightProperty *property = new BlinkingLightProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
BlinkingLightProperty::BlinkingLightProperty
================
*/
// FUNCTION: STBC 0x0067b590
BlinkingLightProperty::BlinkingLightProperty( long id ) : EffectEmitterProperty( id ) {
	color = NiColorA::WHITE;
	textureName = NULL;
	radius = 0.1f;
	period = 3.0f;
	duration = 1.0f;
}

/*
================
BlinkingLightProperty::WriteScript

Writes "<variable> = App.BlinkingLightProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067b6e0
void BlinkingLightProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.BlinkingLightProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
BlinkingLightProperty::~BlinkingLightProperty
================
*/
// FUNCTION: STBC 0x0067b820
BlinkingLightProperty::~BlinkingLightProperty( void ) {
	delete textureName;
}

/*
================
BlinkingLightProperty::Copy
================
*/
// FUNCTION: STBC 0x0067b840
bool BlinkingLightProperty::Copy( TGModelProperty *property ) {
	BlinkingLightProperty *blinkingLight = Cast( property );
	bool copied = EffectEmitterProperty::Copy( property );
	if ( copied == true ) {
		SetColor( blinkingLight->GetColor() );
		SetRadius( blinkingLight->GetRadius() );
		SetPeriod( blinkingLight->GetPeriod() );
		SetDuration( blinkingLight->GetDuration() );
		SetTextureName( blinkingLight->GetTextureName() );
	}
	return copied;
}

/*
================
BlinkingLightProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0067b8a0
void BlinkingLightProperty::WriteInit( TGStream *stream, char *variableName ) {
	char line[2048];

	EffectEmitterProperty::WriteInit( stream, variableName );

	sprintf( line, "%sLightColor = App.TGColorA()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sLightColor.SetRGBA(%f, %f, %f, %f)\n", variableName, color.r, color.g, color.b, color.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetColor(%sLightColor)\n", variableName, variableName );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRadius(%f)\n", variableName, GetRadius() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPeriod(%f)\n", variableName, GetPeriod() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDuration(%f)\n", variableName, GetDuration() );
	stream->WriteCLine( line );
	if ( textureName != NULL ) {
		sprintf( line, "%s.SetTextureName(\"%s\")\n", variableName, textureName );
		stream->WriteCLine( line );
	}
}

/*
================
BlinkingLightProperty::SetTextureName

Keeps a copy of the name of the light's texture.
================
*/
// FUNCTION: STBC 0x0067b9f0 ?SetTextureName@BlinkingLightProperty@@QAEXPAD@Z
void BlinkingLightProperty::SetTextureName( char *name ) {
	if ( textureName != NULL ) {
		delete [] textureName;
		textureName = NULL;
	}
	if ( name != NULL ) {
		textureName = new char[strlen( name ) + 1];
		strcpy( textureName, name );
	}
}

/*
================
BlinkingLightProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0067ba60
bool BlinkingLightProperty::WriteToStream( TGStream *stream ) {
	if ( !EffectEmitterProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( color.GetR() );
	stream->WriteFloat( color.GetG() );
	stream->WriteFloat( color.GetB() );
	stream->WriteFloat( color.GetA() );
	stream->WriteFloat( radius );
	stream->WriteFloat( period );
	stream->WriteFloat( duration );
	stream->VirtualSlot11( textureName );
	return true;
}

/*
================
BlinkingLightProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0067baf0
bool BlinkingLightProperty::ReadFromStream( TGStream *stream ) {
	if ( !EffectEmitterProperty::ReadFromStream( stream ) ) {
		return false;
	}
	color.SetRGBA( stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat() );
	radius = stream->ReadFloat();
	period = stream->ReadFloat();
	duration = stream->ReadFloat();
	textureName = stream->VirtualSlot10( -1 );
	return true;
}
