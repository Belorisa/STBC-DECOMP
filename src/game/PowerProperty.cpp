/*
===============================================================================

	PowerProperty.cpp

===============================================================================
*/

#include <iostream>

#include "PowerProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0068a8c0 _$E32
// FUNCTION: STBC 0x0068a8d0 _$E29
// FUNCTION: STBC 0x0068a8e0 _$E31
// FUNCTION: STBC 0x0068a8f0 _$E30
// FUNCTION: STBC 0x0068a900 _$E38
// FUNCTION: STBC 0x0068a910 _$E35
// FUNCTION: STBC 0x0068a920 _$E37
// FUNCTION: STBC 0x0068a930 _$E36
// FUNCTION: STBC 0x0068ad00 _$E433
// FUNCTION: STBC 0x0068ad20 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0068a940 _$E65
// FUNCTION: STBC 0x0068a950 _$E64
// FUNCTION: STBC 0x0068a960 _$E69
// FUNCTION: STBC 0x0068a970 _$E68
// FUNCTION: STBC 0x0068a980 _$E80
// FUNCTION: STBC 0x0068a990 _$E79
// FUNCTION: STBC 0x0068a9a0 _$E84
// FUNCTION: STBC 0x0068a9b0 _$E83
// FUNCTION: STBC 0x0068a9c0 _$E88
// FUNCTION: STBC 0x0068a9d0 _$E87
// FUNCTION: STBC 0x0068a9e0 _$E92
// FUNCTION: STBC 0x0068a9f0 _$E91
// FUNCTION: STBC 0x0068aa00 _$E211
// FUNCTION: STBC 0x0068aa10 _$E210
// FUNCTION: STBC 0x0068aa20 _$E153
// FUNCTION: STBC 0x0068aa30 _$E152
// FUNCTION: STBC 0x0068aa40 _$E166
// FUNCTION: STBC 0x0068aa50 _$E165
// FUNCTION: STBC 0x0068aa60 _$E216
// FUNCTION: STBC 0x0068aa70 _$E215
// FUNCTION: STBC 0x0068aa80 _$E221
// FUNCTION: STBC 0x0068aa90 _$E220
// FUNCTION: STBC 0x0068aaa0 _$E225
// FUNCTION: STBC 0x0068aab0 _$E224
// FUNCTION: STBC 0x0068aac0 _$E229
// FUNCTION: STBC 0x0068aad0 _$E228
// FUNCTION: STBC 0x0068aae0 _$E236
// FUNCTION: STBC 0x0068aaf0 _$E235
// FUNCTION: STBC 0x0068ab00 _$E258
// FUNCTION: STBC 0x0068ab10 _$E257
// FUNCTION: STBC 0x0068ab20 _$E262
// FUNCTION: STBC 0x0068ab30 _$E261
// FUNCTION: STBC 0x0068ab40 _$E111
// FUNCTION: STBC 0x0068ab50 _$E110
// FUNCTION: STBC 0x0068ab60 _$E244
// FUNCTION: STBC 0x0068ab70 _$E243
// FUNCTION: STBC 0x0068ab80 _$E251
// FUNCTION: STBC 0x0068ab90 _$E250
// FUNCTION: STBC 0x0068aba0 _$E274
// FUNCTION: STBC 0x0068abb0 _$E273
// FUNCTION: STBC 0x0068abc0 _$E278
// FUNCTION: STBC 0x0068abd0 _$E277
// FUNCTION: STBC 0x0068abe0 _$E318
// FUNCTION: STBC 0x0068abf0 _$E317
// FUNCTION: STBC 0x0068ac00 _$E324
// FUNCTION: STBC 0x0068ac10 _$E323
// FUNCTION: STBC 0x0068ac20 _$E266
// FUNCTION: STBC 0x0068ac30 _$E265
// FUNCTION: STBC 0x0068ac40 _$E270
// FUNCTION: STBC 0x0068ac50 _$E269
// FUNCTION: STBC 0x0068ac60 _$E282
// FUNCTION: STBC 0x0068ac70 _$E281
// FUNCTION: STBC 0x0068ac80 _$E286
// FUNCTION: STBC 0x0068ac90 _$E285
// FUNCTION: STBC 0x0068aca0 _$E290
// FUNCTION: STBC 0x0068acb0 _$E289
// FUNCTION: STBC 0x0068acc0 _$E294
// FUNCTION: STBC 0x0068acd0 _$E293
// FUNCTION: STBC 0x0068ace0 _$E298
// FUNCTION: STBC 0x0068acf0 _$E297
// FUNCTION: STBC 0x0068ad30 _$E302
// FUNCTION: STBC 0x0068ad40 _$E301
// FUNCTION: STBC 0x0068ad50 _$E306
// FUNCTION: STBC 0x0068ad60 _$E305
// FUNCTION: STBC 0x0068ad70 _$E310
// FUNCTION: STBC 0x0068ad80 _$E309
// FUNCTION: STBC 0x0068ad90 _$E116
// FUNCTION: STBC 0x0068ada0 _$E115
// FUNCTION: STBC 0x0068adb0 _$E314
// FUNCTION: STBC 0x0068adc0 _$E313
// FUNCTION: STBC 0x0068add0 _$E121
// FUNCTION: STBC 0x0068ade0 _$E120
// FUNCTION: STBC 0x0068adf0 _$E328
// FUNCTION: STBC 0x0068ae00 _$E327
// FUNCTION: STBC 0x0068ae10 _$E332
// FUNCTION: STBC 0x0068ae20 _$E331
// FUNCTION: STBC 0x0068ae30 _$E336
// FUNCTION: STBC 0x0068ae40 _$E335
// FUNCTION: STBC 0x0068ae50 _$E340
// FUNCTION: STBC 0x0068ae60 _$E339
// FUNCTION: STBC 0x0068ae70 _$E130
// FUNCTION: STBC 0x0068ae80 _$E129
// FUNCTION: STBC 0x0068ae90 _$E344
// FUNCTION: STBC 0x0068aea0 _$E343
// FUNCTION: STBC 0x0068aeb0 _$E348
// FUNCTION: STBC 0x0068aec0 _$E347
// FUNCTION: STBC 0x0068aed0 _$E352
// FUNCTION: STBC 0x0068aee0 _$E351
// FUNCTION: STBC 0x0068aef0 _$E136
// FUNCTION: STBC 0x0068af00 _$E135
// FUNCTION: STBC 0x0068af10 _$E141
// FUNCTION: STBC 0x0068af20 _$E140
// FUNCTION: STBC 0x0068af30 _$E171
// FUNCTION: STBC 0x0068af40 _$E170
// FUNCTION: STBC 0x0068af50 _$E356
// FUNCTION: STBC 0x0068af60 _$E355
// FUNCTION: STBC 0x0068af70 _$E176
// FUNCTION: STBC 0x0068af80 _$E175
// FUNCTION: STBC 0x0068af90 _$E360
// FUNCTION: STBC 0x0068afa0 _$E359
// FUNCTION: STBC 0x0068afb0 _$E364
// FUNCTION: STBC 0x0068afc0 _$E363
// FUNCTION: STBC 0x0068afd0 _$E181
// FUNCTION: STBC 0x0068afe0 _$E180
// FUNCTION: STBC 0x0068aff0 _$E368
// FUNCTION: STBC 0x0068b000 _$E367
// FUNCTION: STBC 0x0068b010 _$E146
// FUNCTION: STBC 0x0068b020 _$E145
// FUNCTION: STBC 0x0068b030 _$E372
// FUNCTION: STBC 0x0068b040 _$E371
// FUNCTION: STBC 0x0068b050 _$E376
// FUNCTION: STBC 0x0068b060 _$E375
// FUNCTION: STBC 0x0068b070 _$E186
// FUNCTION: STBC 0x0068b080 _$E185
// FUNCTION: STBC 0x0068b090 _$E191
// FUNCTION: STBC 0x0068b0a0 _$E190
// FUNCTION: STBC 0x0068b0b0 _$E380
// FUNCTION: STBC 0x0068b0c0 _$E379
// FUNCTION: STBC 0x0068b0d0 _$E384
// FUNCTION: STBC 0x0068b0e0 _$E383
// FUNCTION: STBC 0x0068b0f0 _$E388
// FUNCTION: STBC 0x0068b100 _$E387
// FUNCTION: STBC 0x0068b110 _$E392
// FUNCTION: STBC 0x0068b120 _$E391
// FUNCTION: STBC 0x0068b130 _$E396
// FUNCTION: STBC 0x0068b140 _$E395
// FUNCTION: STBC 0x0068b150 _$E400
// FUNCTION: STBC 0x0068b160 _$E399
// FUNCTION: STBC 0x0068b170 _$E404
// FUNCTION: STBC 0x0068b180 _$E403
// FUNCTION: STBC 0x0068b190 _$E408
// FUNCTION: STBC 0x0068b1a0 _$E407
// FUNCTION: STBC 0x0068b1b0 _$E196
// FUNCTION: STBC 0x0068b1c0 _$E195
// FUNCTION: STBC 0x0068b1d0 _$E412
// FUNCTION: STBC 0x0068b1e0 _$E411
// FUNCTION: STBC 0x0068b1f0 _$E201
// FUNCTION: STBC 0x0068b200 _$E200
// FUNCTION: STBC 0x0068b210 _$E206
// FUNCTION: STBC 0x0068b220 _$E205
// FUNCTION: STBC 0x0068b230 _$E416
// FUNCTION: STBC 0x0068b240 _$E415
// FUNCTION: STBC 0x0068b250 _$E421
// FUNCTION: STBC 0x0068b260 _$E420
// FUNCTION: STBC 0x0068b270 _$E425
// FUNCTION: STBC 0x0068b280 _$E424

/*
================
PowerProperty::Cast

The property as a PowerProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0068b290 ?Cast@PowerProperty@@SAPAV1@PAVTGModelProperty@@@Z
PowerProperty *PowerProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( POWERPROPERTY_TYPE ) ) {
		return (PowerProperty *)property;
	}
	return NULL;
}

/*
================
PowerProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0068b2c0
PowerProperty *PowerProperty::Create( char *name ) {
	PowerProperty *property = new PowerProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
PowerProperty::PowerProperty
================
*/
// FUNCTION: STBC 0x0068b310
PowerProperty::PowerProperty( long id ) : SubsystemProperty( id ) {
	mainBatteryLimit = 70000.0f;
	backupBatteryLimit = 10000.0f;
	mainConduitCapacity = 400.0f;
	backupConduitCapacity = 200.0f;
	powerOutput = 100.0f;
}

/*
================
PowerProperty::WriteScript

Writes "<variable> = App.PowerProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0068b3d0
void PowerProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.PowerProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
PowerProperty::~PowerProperty
================
*/
// FUNCTION: STBC 0x0068b500
PowerProperty::~PowerProperty( void ) {
}

/*
================
PowerProperty::Copy
================
*/
// FUNCTION: STBC 0x0068b510
bool PowerProperty::Copy( TGModelProperty *property ) {
	PowerProperty *power = Cast( property );
	bool copied = SubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetMainBatteryLimit( power->GetMainBatteryLimit() );
		SetBackupBatteryLimit( power->GetBackupBatteryLimit() );
		SetMainConduitCapacity( power->GetMainConduitCapacity() );
		SetBackupConduitCapacity( power->GetBackupConduitCapacity() );
		SetPowerOutput( power->GetPowerOutput() );
	}
	return copied;
}

/*
================
PowerProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0068b560
void PowerProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	SubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
PowerProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0068b580
void PowerProperty::WriteInit( TGStream *stream, char *variableName ) {
	SubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetMainBatteryLimit(%f)\n", variableName, GetMainBatteryLimit() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetBackupBatteryLimit(%f)\n", variableName, GetBackupBatteryLimit() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMainConduitCapacity(%f)\n", variableName, GetMainConduitCapacity() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetBackupConduitCapacity(%f)\n", variableName, GetBackupConduitCapacity() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPowerOutput(%f)\n", variableName, GetPowerOutput() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
PowerProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0068b670
bool PowerProperty::WriteToStream( TGStream *stream ) {
	if ( !SubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( mainBatteryLimit );
	stream->WriteFloat( backupBatteryLimit );
	stream->WriteFloat( mainConduitCapacity );
	stream->WriteFloat( backupConduitCapacity );
	stream->WriteFloat( powerOutput );
	return true;
}

/*
================
PowerProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0068b6d0
bool PowerProperty::ReadFromStream( TGStream *stream ) {
	if ( !SubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	mainBatteryLimit = stream->ReadFloat();
	backupBatteryLimit = stream->ReadFloat();
	mainConduitCapacity = stream->ReadFloat();
	backupConduitCapacity = stream->ReadFloat();
	powerOutput = stream->ReadFloat();
	return true;
}
