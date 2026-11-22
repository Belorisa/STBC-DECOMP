/*
===============================================================================

	PoweredSubsystemProperty.cpp

===============================================================================
*/

#include <iostream>

#include "PoweredSubsystemProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00689bb0 _$E32
// FUNCTION: STBC 0x00689bc0 _$E29
// FUNCTION: STBC 0x00689bd0 _$E31
// FUNCTION: STBC 0x00689be0 _$E30
// FUNCTION: STBC 0x00689bf0 _$E38
// FUNCTION: STBC 0x00689c00 _$E35
// FUNCTION: STBC 0x00689c10 _$E37
// FUNCTION: STBC 0x00689c20 _$E36
// FUNCTION: STBC 0x0068a050 _$E433
// FUNCTION: STBC 0x0068a070 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00689c30 _$E65
// FUNCTION: STBC 0x00689c40 _$E64
// FUNCTION: STBC 0x00689c50 _$E69
// FUNCTION: STBC 0x00689c60 _$E68
// FUNCTION: STBC 0x00689c70 _$E80
// FUNCTION: STBC 0x00689c80 _$E79
// FUNCTION: STBC 0x00689c90 _$E84
// FUNCTION: STBC 0x00689ca0 _$E83
// FUNCTION: STBC 0x00689cb0 _$E88
// FUNCTION: STBC 0x00689cc0 _$E87
// FUNCTION: STBC 0x00689cd0 _$E92
// FUNCTION: STBC 0x00689ce0 _$E91
// FUNCTION: STBC 0x00689cf0 _$E211
// FUNCTION: STBC 0x00689d00 _$E210
// FUNCTION: STBC 0x00689d10 _$E153
// FUNCTION: STBC 0x00689d20 _$E152
// FUNCTION: STBC 0x00689d30 _$E166
// FUNCTION: STBC 0x00689d40 _$E165
// FUNCTION: STBC 0x00689d50 _$E216
// FUNCTION: STBC 0x00689d60 _$E215
// FUNCTION: STBC 0x00689d70 _$E221
// FUNCTION: STBC 0x00689d80 _$E220
// FUNCTION: STBC 0x00689d90 _$E225
// FUNCTION: STBC 0x00689da0 _$E224
// FUNCTION: STBC 0x00689db0 _$E229
// FUNCTION: STBC 0x00689dc0 _$E228
// FUNCTION: STBC 0x00689dd0 _$E236
// FUNCTION: STBC 0x00689de0 _$E235
// FUNCTION: STBC 0x00689df0 _$E258
// FUNCTION: STBC 0x00689e00 _$E257
// FUNCTION: STBC 0x00689e10 _$E262
// FUNCTION: STBC 0x00689e20 _$E261
// FUNCTION: STBC 0x00689e30 _$E111
// FUNCTION: STBC 0x00689e40 _$E110
// FUNCTION: STBC 0x00689e50 _$E244
// FUNCTION: STBC 0x00689e60 _$E243
// FUNCTION: STBC 0x00689e70 _$E251
// FUNCTION: STBC 0x00689e80 _$E250
// FUNCTION: STBC 0x00689e90 _$E274
// FUNCTION: STBC 0x00689ea0 _$E273
// FUNCTION: STBC 0x00689eb0 _$E278
// FUNCTION: STBC 0x00689ec0 _$E277
// FUNCTION: STBC 0x00689ed0 _$E318
// FUNCTION: STBC 0x00689ee0 _$E317
// FUNCTION: STBC 0x00689ef0 _$E324
// FUNCTION: STBC 0x00689f00 _$E323
// FUNCTION: STBC 0x00689f10 _$E266
// FUNCTION: STBC 0x00689f20 _$E265
// FUNCTION: STBC 0x00689f30 _$E270
// FUNCTION: STBC 0x00689f40 _$E269
// FUNCTION: STBC 0x00689f50 _$E282
// FUNCTION: STBC 0x00689f60 _$E281
// FUNCTION: STBC 0x00689f70 _$E286
// FUNCTION: STBC 0x00689f80 _$E285
// FUNCTION: STBC 0x00689f90 _$E290
// FUNCTION: STBC 0x00689fa0 _$E289
// FUNCTION: STBC 0x00689fb0 _$E294
// FUNCTION: STBC 0x00689fc0 _$E293
// FUNCTION: STBC 0x00689fd0 _$E298
// FUNCTION: STBC 0x00689fe0 _$E297
// FUNCTION: STBC 0x00689ff0 _$E302
// FUNCTION: STBC 0x0068a000 _$E301
// FUNCTION: STBC 0x0068a010 _$E306
// FUNCTION: STBC 0x0068a020 _$E305
// FUNCTION: STBC 0x0068a030 _$E310
// FUNCTION: STBC 0x0068a040 _$E309
// FUNCTION: STBC 0x0068a080 _$E116
// FUNCTION: STBC 0x0068a090 _$E115
// FUNCTION: STBC 0x0068a0a0 _$E314
// FUNCTION: STBC 0x0068a0b0 _$E313
// FUNCTION: STBC 0x0068a0c0 _$E121
// FUNCTION: STBC 0x0068a0d0 _$E120
// FUNCTION: STBC 0x0068a0e0 _$E328
// FUNCTION: STBC 0x0068a0f0 _$E327
// FUNCTION: STBC 0x0068a100 _$E332
// FUNCTION: STBC 0x0068a110 _$E331
// FUNCTION: STBC 0x0068a120 _$E336
// FUNCTION: STBC 0x0068a130 _$E335
// FUNCTION: STBC 0x0068a140 _$E340
// FUNCTION: STBC 0x0068a150 _$E339
// FUNCTION: STBC 0x0068a160 _$E130
// FUNCTION: STBC 0x0068a170 _$E129
// FUNCTION: STBC 0x0068a180 _$E344
// FUNCTION: STBC 0x0068a190 _$E343
// FUNCTION: STBC 0x0068a1a0 _$E348
// FUNCTION: STBC 0x0068a1b0 _$E347
// FUNCTION: STBC 0x0068a1c0 _$E352
// FUNCTION: STBC 0x0068a1d0 _$E351
// FUNCTION: STBC 0x0068a1e0 _$E136
// FUNCTION: STBC 0x0068a1f0 _$E135
// FUNCTION: STBC 0x0068a200 _$E141
// FUNCTION: STBC 0x0068a210 _$E140
// FUNCTION: STBC 0x0068a220 _$E171
// FUNCTION: STBC 0x0068a230 _$E170
// FUNCTION: STBC 0x0068a240 _$E356
// FUNCTION: STBC 0x0068a250 _$E355
// FUNCTION: STBC 0x0068a260 _$E176
// FUNCTION: STBC 0x0068a270 _$E175
// FUNCTION: STBC 0x0068a280 _$E360
// FUNCTION: STBC 0x0068a290 _$E359
// FUNCTION: STBC 0x0068a2a0 _$E364
// FUNCTION: STBC 0x0068a2b0 _$E363
// FUNCTION: STBC 0x0068a2c0 _$E181
// FUNCTION: STBC 0x0068a2d0 _$E180
// FUNCTION: STBC 0x0068a2e0 _$E368
// FUNCTION: STBC 0x0068a2f0 _$E367
// FUNCTION: STBC 0x0068a300 _$E146
// FUNCTION: STBC 0x0068a310 _$E145
// FUNCTION: STBC 0x0068a320 _$E372
// FUNCTION: STBC 0x0068a330 _$E371
// FUNCTION: STBC 0x0068a340 _$E376
// FUNCTION: STBC 0x0068a350 _$E375
// FUNCTION: STBC 0x0068a360 _$E186
// FUNCTION: STBC 0x0068a370 _$E185
// FUNCTION: STBC 0x0068a380 _$E191
// FUNCTION: STBC 0x0068a390 _$E190
// FUNCTION: STBC 0x0068a3a0 _$E380
// FUNCTION: STBC 0x0068a3b0 _$E379
// FUNCTION: STBC 0x0068a3c0 _$E384
// FUNCTION: STBC 0x0068a3d0 _$E383
// FUNCTION: STBC 0x0068a3e0 _$E388
// FUNCTION: STBC 0x0068a3f0 _$E387
// FUNCTION: STBC 0x0068a400 _$E392
// FUNCTION: STBC 0x0068a410 _$E391
// FUNCTION: STBC 0x0068a420 _$E396
// FUNCTION: STBC 0x0068a430 _$E395
// FUNCTION: STBC 0x0068a440 _$E400
// FUNCTION: STBC 0x0068a450 _$E399
// FUNCTION: STBC 0x0068a460 _$E404
// FUNCTION: STBC 0x0068a470 _$E403
// FUNCTION: STBC 0x0068a480 _$E408
// FUNCTION: STBC 0x0068a490 _$E407
// FUNCTION: STBC 0x0068a4a0 _$E196
// FUNCTION: STBC 0x0068a4b0 _$E195
// FUNCTION: STBC 0x0068a4c0 _$E412
// FUNCTION: STBC 0x0068a4d0 _$E411
// FUNCTION: STBC 0x0068a4e0 _$E201
// FUNCTION: STBC 0x0068a4f0 _$E200
// FUNCTION: STBC 0x0068a500 _$E206
// FUNCTION: STBC 0x0068a510 _$E205
// FUNCTION: STBC 0x0068a520 _$E416
// FUNCTION: STBC 0x0068a530 _$E415
// FUNCTION: STBC 0x0068a540 _$E421
// FUNCTION: STBC 0x0068a550 _$E420
// FUNCTION: STBC 0x0068a560 _$E425
// FUNCTION: STBC 0x0068a570 _$E424

/*
================
PoweredSubsystemProperty::Cast

The property as a PoweredSubsystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0068a580 ?Cast@PoweredSubsystemProperty@@SAPAV1@PAVTGModelProperty@@@Z
PoweredSubsystemProperty *PoweredSubsystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( POWEREDSUBSYSTEMPROPERTY_TYPE ) ) {
		return (PoweredSubsystemProperty *)property;
	}
	return NULL;
}

/*
================
PoweredSubsystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0068a5b0
PoweredSubsystemProperty *PoweredSubsystemProperty::Create( char *name ) {
	PoweredSubsystemProperty *property = new PoweredSubsystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
PoweredSubsystemProperty::PoweredSubsystemProperty
================
*/
// FUNCTION: STBC 0x0068a600
PoweredSubsystemProperty::PoweredSubsystemProperty( long id ) : SubsystemProperty( id ) {
	normalPowerPerSecond = 1.0f;
}

/*
================
PoweredSubsystemProperty::WriteScript

Writes "<variable> = App.PoweredSubsystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0068a6a0
void PoweredSubsystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.PoweredSubsystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
PoweredSubsystemProperty::~PoweredSubsystemProperty
================
*/
// FUNCTION: STBC 0x0068a7d0
PoweredSubsystemProperty::~PoweredSubsystemProperty( void ) {
}

/*
================
PoweredSubsystemProperty::Copy
================
*/
// FUNCTION: STBC 0x0068a7e0
bool PoweredSubsystemProperty::Copy( TGModelProperty *property ) {
	PoweredSubsystemProperty *poweredSubsystem = Cast( property );
	bool copied = SubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetNormalPowerPerSecond( poweredSubsystem->GetNormalPowerPerSecond() );
	}
	return copied;
}

/*
================
PoweredSubsystemProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0068a810
void PoweredSubsystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	SubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetNormalPowerPerSecond(%f)\n", variableName, GetNormalPowerPerSecond() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
PoweredSubsystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0068a860
bool PoweredSubsystemProperty::WriteToStream( TGStream *stream ) {
	if ( !SubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( normalPowerPerSecond );
	return true;
}

/*
================
PoweredSubsystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0068a890
bool PoweredSubsystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !SubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	normalPowerPerSecond = stream->ReadFloat();
	return true;
}
