/*
===============================================================================

	ImpulseEngineProperty.cpp

===============================================================================
*/

#include <iostream>

#include <string.h>

#include "ImpulseEngineProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006831f0 _$E32
// FUNCTION: STBC 0x00683200 _$E29
// FUNCTION: STBC 0x00683210 _$E31
// FUNCTION: STBC 0x00683220 _$E30
// FUNCTION: STBC 0x00683230 _$E38
// FUNCTION: STBC 0x00683240 _$E35
// FUNCTION: STBC 0x00683250 _$E37
// FUNCTION: STBC 0x00683260 _$E36
// FUNCTION: STBC 0x00683630 _$E433
// FUNCTION: STBC 0x00683650 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00683270 _$E65
// FUNCTION: STBC 0x00683280 _$E64
// FUNCTION: STBC 0x00683290 _$E69
// FUNCTION: STBC 0x006832a0 _$E68
// FUNCTION: STBC 0x006832b0 _$E80
// FUNCTION: STBC 0x006832c0 _$E79
// FUNCTION: STBC 0x006832d0 _$E84
// FUNCTION: STBC 0x006832e0 _$E83
// FUNCTION: STBC 0x006832f0 _$E88
// FUNCTION: STBC 0x00683300 _$E87
// FUNCTION: STBC 0x00683310 _$E92
// FUNCTION: STBC 0x00683320 _$E91
// FUNCTION: STBC 0x00683330 _$E211
// FUNCTION: STBC 0x00683340 _$E210
// FUNCTION: STBC 0x00683350 _$E153
// FUNCTION: STBC 0x00683360 _$E152
// FUNCTION: STBC 0x00683370 _$E166
// FUNCTION: STBC 0x00683380 _$E165
// FUNCTION: STBC 0x00683390 _$E216
// FUNCTION: STBC 0x006833a0 _$E215
// FUNCTION: STBC 0x006833b0 _$E221
// FUNCTION: STBC 0x006833c0 _$E220
// FUNCTION: STBC 0x006833d0 _$E225
// FUNCTION: STBC 0x006833e0 _$E224
// FUNCTION: STBC 0x006833f0 _$E229
// FUNCTION: STBC 0x00683400 _$E228
// FUNCTION: STBC 0x00683410 _$E236
// FUNCTION: STBC 0x00683420 _$E235
// FUNCTION: STBC 0x00683430 _$E258
// FUNCTION: STBC 0x00683440 _$E257
// FUNCTION: STBC 0x00683450 _$E262
// FUNCTION: STBC 0x00683460 _$E261
// FUNCTION: STBC 0x00683470 _$E111
// FUNCTION: STBC 0x00683480 _$E110
// FUNCTION: STBC 0x00683490 _$E244
// FUNCTION: STBC 0x006834a0 _$E243
// FUNCTION: STBC 0x006834b0 _$E251
// FUNCTION: STBC 0x006834c0 _$E250
// FUNCTION: STBC 0x006834d0 _$E274
// FUNCTION: STBC 0x006834e0 _$E273
// FUNCTION: STBC 0x006834f0 _$E278
// FUNCTION: STBC 0x00683500 _$E277
// FUNCTION: STBC 0x00683510 _$E318
// FUNCTION: STBC 0x00683520 _$E317
// FUNCTION: STBC 0x00683530 _$E324
// FUNCTION: STBC 0x00683540 _$E323
// FUNCTION: STBC 0x00683550 _$E266
// FUNCTION: STBC 0x00683560 _$E265
// FUNCTION: STBC 0x00683570 _$E270
// FUNCTION: STBC 0x00683580 _$E269
// FUNCTION: STBC 0x00683590 _$E282
// FUNCTION: STBC 0x006835a0 _$E281
// FUNCTION: STBC 0x006835b0 _$E286
// FUNCTION: STBC 0x006835c0 _$E285
// FUNCTION: STBC 0x006835d0 _$E290
// FUNCTION: STBC 0x006835e0 _$E289
// FUNCTION: STBC 0x006835f0 _$E294
// FUNCTION: STBC 0x00683600 _$E293
// FUNCTION: STBC 0x00683610 _$E298
// FUNCTION: STBC 0x00683620 _$E297
// FUNCTION: STBC 0x00683660 _$E302
// FUNCTION: STBC 0x00683670 _$E301
// FUNCTION: STBC 0x00683680 _$E306
// FUNCTION: STBC 0x00683690 _$E305
// FUNCTION: STBC 0x006836a0 _$E310
// FUNCTION: STBC 0x006836b0 _$E309
// FUNCTION: STBC 0x006836c0 _$E116
// FUNCTION: STBC 0x006836d0 _$E115
// FUNCTION: STBC 0x006836e0 _$E314
// FUNCTION: STBC 0x006836f0 _$E313
// FUNCTION: STBC 0x00683700 _$E121
// FUNCTION: STBC 0x00683710 _$E120
// FUNCTION: STBC 0x00683720 _$E328
// FUNCTION: STBC 0x00683730 _$E327
// FUNCTION: STBC 0x00683740 _$E332
// FUNCTION: STBC 0x00683750 _$E331
// FUNCTION: STBC 0x00683760 _$E336
// FUNCTION: STBC 0x00683770 _$E335
// FUNCTION: STBC 0x00683780 _$E340
// FUNCTION: STBC 0x00683790 _$E339
// FUNCTION: STBC 0x006837a0 _$E130
// FUNCTION: STBC 0x006837b0 _$E129
// FUNCTION: STBC 0x006837c0 _$E344
// FUNCTION: STBC 0x006837d0 _$E343
// FUNCTION: STBC 0x006837e0 _$E348
// FUNCTION: STBC 0x006837f0 _$E347
// FUNCTION: STBC 0x00683800 _$E352
// FUNCTION: STBC 0x00683810 _$E351
// FUNCTION: STBC 0x00683820 _$E136
// FUNCTION: STBC 0x00683830 _$E135
// FUNCTION: STBC 0x00683840 _$E141
// FUNCTION: STBC 0x00683850 _$E140
// FUNCTION: STBC 0x00683860 _$E171
// FUNCTION: STBC 0x00683870 _$E170
// FUNCTION: STBC 0x00683880 _$E356
// FUNCTION: STBC 0x00683890 _$E355
// FUNCTION: STBC 0x006838a0 _$E176
// FUNCTION: STBC 0x006838b0 _$E175
// FUNCTION: STBC 0x006838c0 _$E360
// FUNCTION: STBC 0x006838d0 _$E359
// FUNCTION: STBC 0x006838e0 _$E364
// FUNCTION: STBC 0x006838f0 _$E363
// FUNCTION: STBC 0x00683900 _$E181
// FUNCTION: STBC 0x00683910 _$E180
// FUNCTION: STBC 0x00683920 _$E368
// FUNCTION: STBC 0x00683930 _$E367
// FUNCTION: STBC 0x00683940 _$E146
// FUNCTION: STBC 0x00683950 _$E145
// FUNCTION: STBC 0x00683960 _$E372
// FUNCTION: STBC 0x00683970 _$E371
// FUNCTION: STBC 0x00683980 _$E376
// FUNCTION: STBC 0x00683990 _$E375
// FUNCTION: STBC 0x006839a0 _$E186
// FUNCTION: STBC 0x006839b0 _$E185
// FUNCTION: STBC 0x006839c0 _$E191
// FUNCTION: STBC 0x006839d0 _$E190
// FUNCTION: STBC 0x006839e0 _$E380
// FUNCTION: STBC 0x006839f0 _$E379
// FUNCTION: STBC 0x00683a00 _$E384
// FUNCTION: STBC 0x00683a10 _$E383
// FUNCTION: STBC 0x00683a20 _$E388
// FUNCTION: STBC 0x00683a30 _$E387
// FUNCTION: STBC 0x00683a40 _$E392
// FUNCTION: STBC 0x00683a50 _$E391
// FUNCTION: STBC 0x00683a60 _$E396
// FUNCTION: STBC 0x00683a70 _$E395
// FUNCTION: STBC 0x00683a80 _$E400
// FUNCTION: STBC 0x00683a90 _$E399
// FUNCTION: STBC 0x00683aa0 _$E404
// FUNCTION: STBC 0x00683ab0 _$E403
// FUNCTION: STBC 0x00683ac0 _$E408
// FUNCTION: STBC 0x00683ad0 _$E407
// FUNCTION: STBC 0x00683ae0 _$E196
// FUNCTION: STBC 0x00683af0 _$E195
// FUNCTION: STBC 0x00683b00 _$E412
// FUNCTION: STBC 0x00683b10 _$E411
// FUNCTION: STBC 0x00683b20 _$E201
// FUNCTION: STBC 0x00683b30 _$E200
// FUNCTION: STBC 0x00683b40 _$E206
// FUNCTION: STBC 0x00683b50 _$E205
// FUNCTION: STBC 0x00683b60 _$E416
// FUNCTION: STBC 0x00683b70 _$E415
// FUNCTION: STBC 0x00683b80 _$E421
// FUNCTION: STBC 0x00683b90 _$E420
// FUNCTION: STBC 0x00683ba0 _$E425
// FUNCTION: STBC 0x00683bb0 _$E424

/*
================
ImpulseEngineProperty::Cast

The property as a ImpulseEngineProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00683bc0 ?Cast@ImpulseEngineProperty@@SAPAV1@PAVTGModelProperty@@@Z
ImpulseEngineProperty *ImpulseEngineProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( IMPULSEENGINEPROPERTY_TYPE ) ) {
		return (ImpulseEngineProperty *)property;
	}
	return NULL;
}

/*
================
ImpulseEngineProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00683bf0
ImpulseEngineProperty *ImpulseEngineProperty::Create( char *name ) {
	ImpulseEngineProperty *property = new ImpulseEngineProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
ImpulseEngineProperty::ImpulseEngineProperty
================
*/
// FUNCTION: STBC 0x00683c40
ImpulseEngineProperty::ImpulseEngineProperty( long id ) : PoweredSubsystemProperty( id ) {
	maxAccel = 10.0f;
	maxAngularAccel = 0.1f;
	maxAngularVelocity = 0.25f;
	maxSpeed = 25.0f;
	engineSound = NULL;
}

/*
================
ImpulseEngineProperty::WriteScript

Writes "<variable> = App.ImpulseEngineProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00683d00
void ImpulseEngineProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.ImpulseEngineProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
ImpulseEngineProperty::~ImpulseEngineProperty
================
*/
// FUNCTION: STBC 0x00683e30
ImpulseEngineProperty::~ImpulseEngineProperty( void ) {
	if ( engineSound != NULL ) {
		delete engineSound;
		engineSound = NULL;
	}
}

/*
================
ImpulseEngineProperty::SetEngineSound

Keeps a copy of the name of the engine sound.
================
*/
// FUNCTION: STBC 0x00683e60 ?SetEngineSound@ImpulseEngineProperty@@QAEXPAD@Z
void ImpulseEngineProperty::SetEngineSound( char *sound ) {
	if ( engineSound != NULL ) {
		delete [] engineSound;
		engineSound = NULL;
	}
	if ( sound != NULL ) {
		engineSound = new char[strlen( sound ) + 1];
		strcpy( engineSound, sound );
	}
}

/*
================
ImpulseEngineProperty::Copy
================
*/
// FUNCTION: STBC 0x00683ed0
bool ImpulseEngineProperty::Copy( TGModelProperty *property ) {
	ImpulseEngineProperty *impulseEngine = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetMaxAccel( impulseEngine->GetMaxAccel() );
		SetMaxAngularAccel( impulseEngine->GetMaxAngularAccel() );
		SetMaxAngularVelocity( impulseEngine->GetMaxAngularVelocity() );
		SetMaxSpeed( impulseEngine->GetMaxSpeed() );
		SetEngineSound( impulseEngine->GetEngineSound() );
	}
	return copied;
}

/*
================
ImpulseEngineProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00683f20
void ImpulseEngineProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetMaxAccel(%f)\n", variableName, GetMaxAccel() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxAngularAccel(%f)\n", variableName, GetMaxAngularAccel() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxAngularVelocity(%f)\n", variableName, GetMaxAngularVelocity() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxSpeed(%f)\n", variableName, GetMaxSpeed() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetEngineSound(\"%s\")\n", variableName, GetEngineSound() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
ImpulseEngineProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00683ff0
bool ImpulseEngineProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( maxSpeed );
	stream->WriteFloat( maxAccel );
	stream->WriteFloat( maxAngularVelocity );
	stream->WriteFloat( maxAngularAccel );
	stream->VirtualSlot11( engineSound );
	return true;
}

/*
================
ImpulseEngineProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00684050
bool ImpulseEngineProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	maxSpeed = stream->ReadFloat();
	maxAccel = stream->ReadFloat();
	maxAngularVelocity = stream->ReadFloat();
	maxAngularAccel = stream->ReadFloat();
	engineSound = stream->VirtualSlot10( -1 );
	return true;
}
