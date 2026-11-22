/*
===============================================================================

	SensorProperty.cpp

===============================================================================
*/

#include <iostream>

#include "SensorProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0068e990 _$E32
// FUNCTION: STBC 0x0068e9a0 _$E29
// FUNCTION: STBC 0x0068e9b0 _$E31
// FUNCTION: STBC 0x0068e9c0 _$E30
// FUNCTION: STBC 0x0068e9d0 _$E38
// FUNCTION: STBC 0x0068e9e0 _$E35
// FUNCTION: STBC 0x0068e9f0 _$E37
// FUNCTION: STBC 0x0068ea00 _$E36
// FUNCTION: STBC 0x0068ee10 _$E433
// FUNCTION: STBC 0x0068ee30 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0068ea10 _$E65
// FUNCTION: STBC 0x0068ea20 _$E64
// FUNCTION: STBC 0x0068ea30 _$E69
// FUNCTION: STBC 0x0068ea40 _$E68
// FUNCTION: STBC 0x0068ea50 _$E80
// FUNCTION: STBC 0x0068ea60 _$E79
// FUNCTION: STBC 0x0068ea70 _$E84
// FUNCTION: STBC 0x0068ea80 _$E83
// FUNCTION: STBC 0x0068ea90 _$E88
// FUNCTION: STBC 0x0068eaa0 _$E87
// FUNCTION: STBC 0x0068eab0 _$E92
// FUNCTION: STBC 0x0068eac0 _$E91
// FUNCTION: STBC 0x0068ead0 _$E211
// FUNCTION: STBC 0x0068eae0 _$E210
// FUNCTION: STBC 0x0068eaf0 _$E153
// FUNCTION: STBC 0x0068eb00 _$E152
// FUNCTION: STBC 0x0068eb10 _$E166
// FUNCTION: STBC 0x0068eb20 _$E165
// FUNCTION: STBC 0x0068eb30 _$E216
// FUNCTION: STBC 0x0068eb40 _$E215
// FUNCTION: STBC 0x0068eb50 _$E221
// FUNCTION: STBC 0x0068eb60 _$E220
// FUNCTION: STBC 0x0068eb70 _$E225
// FUNCTION: STBC 0x0068eb80 _$E224
// FUNCTION: STBC 0x0068eb90 _$E229
// FUNCTION: STBC 0x0068eba0 _$E228
// FUNCTION: STBC 0x0068ebb0 _$E236
// FUNCTION: STBC 0x0068ebc0 _$E235
// FUNCTION: STBC 0x0068ebd0 _$E258
// FUNCTION: STBC 0x0068ebe0 _$E257
// FUNCTION: STBC 0x0068ebf0 _$E262
// FUNCTION: STBC 0x0068ec00 _$E261
// FUNCTION: STBC 0x0068ec10 _$E111
// FUNCTION: STBC 0x0068ec20 _$E110
// FUNCTION: STBC 0x0068ec30 _$E244
// FUNCTION: STBC 0x0068ec40 _$E243
// FUNCTION: STBC 0x0068ec50 _$E251
// FUNCTION: STBC 0x0068ec60 _$E250
// FUNCTION: STBC 0x0068ec70 _$E274
// FUNCTION: STBC 0x0068ec80 _$E273
// FUNCTION: STBC 0x0068ec90 _$E278
// FUNCTION: STBC 0x0068eca0 _$E277
// FUNCTION: STBC 0x0068ecb0 _$E318
// FUNCTION: STBC 0x0068ecc0 _$E317
// FUNCTION: STBC 0x0068ecd0 _$E324
// FUNCTION: STBC 0x0068ece0 _$E323
// FUNCTION: STBC 0x0068ecf0 _$E266
// FUNCTION: STBC 0x0068ed00 _$E265
// FUNCTION: STBC 0x0068ed10 _$E270
// FUNCTION: STBC 0x0068ed20 _$E269
// FUNCTION: STBC 0x0068ed30 _$E282
// FUNCTION: STBC 0x0068ed40 _$E281
// FUNCTION: STBC 0x0068ed50 _$E286
// FUNCTION: STBC 0x0068ed60 _$E285
// FUNCTION: STBC 0x0068ed70 _$E290
// FUNCTION: STBC 0x0068ed80 _$E289
// FUNCTION: STBC 0x0068ed90 _$E294
// FUNCTION: STBC 0x0068eda0 _$E293
// FUNCTION: STBC 0x0068edb0 _$E298
// FUNCTION: STBC 0x0068edc0 _$E297
// FUNCTION: STBC 0x0068edd0 _$E302
// FUNCTION: STBC 0x0068ede0 _$E301
// FUNCTION: STBC 0x0068edf0 _$E306
// FUNCTION: STBC 0x0068ee00 _$E305
// FUNCTION: STBC 0x0068ee40 _$E310
// FUNCTION: STBC 0x0068ee50 _$E309
// FUNCTION: STBC 0x0068ee60 _$E116
// FUNCTION: STBC 0x0068ee70 _$E115
// FUNCTION: STBC 0x0068ee80 _$E314
// FUNCTION: STBC 0x0068ee90 _$E313
// FUNCTION: STBC 0x0068eea0 _$E121
// FUNCTION: STBC 0x0068eeb0 _$E120
// FUNCTION: STBC 0x0068eec0 _$E328
// FUNCTION: STBC 0x0068eed0 _$E327
// FUNCTION: STBC 0x0068eee0 _$E332
// FUNCTION: STBC 0x0068eef0 _$E331
// FUNCTION: STBC 0x0068ef00 _$E336
// FUNCTION: STBC 0x0068ef10 _$E335
// FUNCTION: STBC 0x0068ef20 _$E340
// FUNCTION: STBC 0x0068ef30 _$E339
// FUNCTION: STBC 0x0068ef40 _$E130
// FUNCTION: STBC 0x0068ef50 _$E129
// FUNCTION: STBC 0x0068ef60 _$E344
// FUNCTION: STBC 0x0068ef70 _$E343
// FUNCTION: STBC 0x0068ef80 _$E348
// FUNCTION: STBC 0x0068ef90 _$E347
// FUNCTION: STBC 0x0068efa0 _$E352
// FUNCTION: STBC 0x0068efb0 _$E351
// FUNCTION: STBC 0x0068efc0 _$E136
// FUNCTION: STBC 0x0068efd0 _$E135
// FUNCTION: STBC 0x0068efe0 _$E141
// FUNCTION: STBC 0x0068eff0 _$E140
// FUNCTION: STBC 0x0068f000 _$E171
// FUNCTION: STBC 0x0068f010 _$E170
// FUNCTION: STBC 0x0068f020 _$E356
// FUNCTION: STBC 0x0068f030 _$E355
// FUNCTION: STBC 0x0068f040 _$E176
// FUNCTION: STBC 0x0068f050 _$E175
// FUNCTION: STBC 0x0068f060 _$E360
// FUNCTION: STBC 0x0068f070 _$E359
// FUNCTION: STBC 0x0068f080 _$E364
// FUNCTION: STBC 0x0068f090 _$E363
// FUNCTION: STBC 0x0068f0a0 _$E181
// FUNCTION: STBC 0x0068f0b0 _$E180
// FUNCTION: STBC 0x0068f0c0 _$E368
// FUNCTION: STBC 0x0068f0d0 _$E367
// FUNCTION: STBC 0x0068f0e0 _$E146
// FUNCTION: STBC 0x0068f0f0 _$E145
// FUNCTION: STBC 0x0068f100 _$E372
// FUNCTION: STBC 0x0068f110 _$E371
// FUNCTION: STBC 0x0068f120 _$E376
// FUNCTION: STBC 0x0068f130 _$E375
// FUNCTION: STBC 0x0068f140 _$E186
// FUNCTION: STBC 0x0068f150 _$E185
// FUNCTION: STBC 0x0068f160 _$E191
// FUNCTION: STBC 0x0068f170 _$E190
// FUNCTION: STBC 0x0068f180 _$E380
// FUNCTION: STBC 0x0068f190 _$E379
// FUNCTION: STBC 0x0068f1a0 _$E384
// FUNCTION: STBC 0x0068f1b0 _$E383
// FUNCTION: STBC 0x0068f1c0 _$E388
// FUNCTION: STBC 0x0068f1d0 _$E387
// FUNCTION: STBC 0x0068f1e0 _$E392
// FUNCTION: STBC 0x0068f1f0 _$E391
// FUNCTION: STBC 0x0068f200 _$E396
// FUNCTION: STBC 0x0068f210 _$E395
// FUNCTION: STBC 0x0068f220 _$E400
// FUNCTION: STBC 0x0068f230 _$E399
// FUNCTION: STBC 0x0068f240 _$E404
// FUNCTION: STBC 0x0068f250 _$E403
// FUNCTION: STBC 0x0068f260 _$E408
// FUNCTION: STBC 0x0068f270 _$E407
// FUNCTION: STBC 0x0068f280 _$E196
// FUNCTION: STBC 0x0068f290 _$E195
// FUNCTION: STBC 0x0068f2a0 _$E412
// FUNCTION: STBC 0x0068f2b0 _$E411
// FUNCTION: STBC 0x0068f2c0 _$E201
// FUNCTION: STBC 0x0068f2d0 _$E200
// FUNCTION: STBC 0x0068f2e0 _$E206
// FUNCTION: STBC 0x0068f2f0 _$E205
// FUNCTION: STBC 0x0068f300 _$E416
// FUNCTION: STBC 0x0068f310 _$E415
// FUNCTION: STBC 0x0068f320 _$E421
// FUNCTION: STBC 0x0068f330 _$E420
// FUNCTION: STBC 0x0068f340 _$E425
// FUNCTION: STBC 0x0068f350 _$E424

/*
================
SensorProperty::Cast

The property as a SensorProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0068f360 ?Cast@SensorProperty@@SAPAV1@PAVTGModelProperty@@@Z
SensorProperty *SensorProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SENSORPROPERTY_TYPE ) ) {
		return (SensorProperty *)property;
	}
	return NULL;
}

/*
================
SensorProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0068f390
SensorProperty *SensorProperty::Create( char *name ) {
	SensorProperty *property = new SensorProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
SensorProperty::SensorProperty
================
*/
// FUNCTION: STBC 0x0068f3e0
SensorProperty::SensorProperty( long id ) : PoweredSubsystemProperty( id ) {
	baseSensorRange = 1000.0f;
	maxProbes = 10;
}

/*
================
SensorProperty::WriteScript

Writes "<variable> = App.SensorProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0068f490
void SensorProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.SensorProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
SensorProperty::~SensorProperty
================
*/
// FUNCTION: STBC 0x0068f5c0
SensorProperty::~SensorProperty( void ) {
}

/*
================
SensorProperty::Copy
================
*/
// FUNCTION: STBC 0x0068f5d0
bool SensorProperty::Copy( TGModelProperty *property ) {
	SensorProperty *sensor = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetBaseSensorRange( sensor->GetBaseSensorRange() );
		SetMaxProbes( sensor->GetMaxProbes() );
	}
	return copied;
}

/*
================
SensorProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0068f610
void SensorProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	PoweredSubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
SensorProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0068f630
void SensorProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetBaseSensorRange(%f)\n", variableName, GetBaseSensorRange() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxProbes(%d)\n", variableName, GetMaxProbes() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
SensorProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0068f6a0
bool SensorProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( baseSensorRange );
	stream->WriteInt( maxProbes );
	return true;
}

/*
================
SensorProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0068f6e0
bool SensorProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	baseSensorRange = stream->ReadFloat();
	maxProbes = stream->ReadInt();
	return true;
}
