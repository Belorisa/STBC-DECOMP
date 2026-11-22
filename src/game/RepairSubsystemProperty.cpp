/*
===============================================================================

	RepairSubsystemProperty.cpp

===============================================================================
*/

#include <iostream>

#include "RepairSubsystemProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0068dc00 _$E32
// FUNCTION: STBC 0x0068dc10 _$E29
// FUNCTION: STBC 0x0068dc20 _$E31
// FUNCTION: STBC 0x0068dc30 _$E30
// FUNCTION: STBC 0x0068dc40 _$E38
// FUNCTION: STBC 0x0068dc50 _$E35
// FUNCTION: STBC 0x0068dc60 _$E37
// FUNCTION: STBC 0x0068dc70 _$E36
// FUNCTION: STBC 0x0068e220 _$E433
// FUNCTION: STBC 0x0068e240 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0068dc80 _$E65
// FUNCTION: STBC 0x0068dc90 _$E64
// FUNCTION: STBC 0x0068dca0 _$E69
// FUNCTION: STBC 0x0068dcb0 _$E68
// FUNCTION: STBC 0x0068dcc0 _$E80
// FUNCTION: STBC 0x0068dcd0 _$E79
// FUNCTION: STBC 0x0068dce0 _$E84
// FUNCTION: STBC 0x0068dcf0 _$E83
// FUNCTION: STBC 0x0068dd00 _$E88
// FUNCTION: STBC 0x0068dd10 _$E87
// FUNCTION: STBC 0x0068dd20 _$E92
// FUNCTION: STBC 0x0068dd30 _$E91
// FUNCTION: STBC 0x0068dd40 _$E211
// FUNCTION: STBC 0x0068dd50 _$E210
// FUNCTION: STBC 0x0068dd60 _$E153
// FUNCTION: STBC 0x0068dd70 _$E152
// FUNCTION: STBC 0x0068dd80 _$E166
// FUNCTION: STBC 0x0068dd90 _$E165
// FUNCTION: STBC 0x0068dda0 _$E216
// FUNCTION: STBC 0x0068ddb0 _$E215
// FUNCTION: STBC 0x0068ddc0 _$E221
// FUNCTION: STBC 0x0068ddd0 _$E220
// FUNCTION: STBC 0x0068dde0 _$E225
// FUNCTION: STBC 0x0068ddf0 _$E224
// FUNCTION: STBC 0x0068de00 _$E229
// FUNCTION: STBC 0x0068de10 _$E228
// FUNCTION: STBC 0x0068de20 _$E236
// FUNCTION: STBC 0x0068de30 _$E235
// FUNCTION: STBC 0x0068de40 _$E258
// FUNCTION: STBC 0x0068de50 _$E257
// FUNCTION: STBC 0x0068de60 _$E262
// FUNCTION: STBC 0x0068de70 _$E261
// FUNCTION: STBC 0x0068de80 _$E111
// FUNCTION: STBC 0x0068de90 _$E110
// FUNCTION: STBC 0x0068dea0 _$E244
// FUNCTION: STBC 0x0068deb0 _$E243
// FUNCTION: STBC 0x0068dec0 _$E251
// FUNCTION: STBC 0x0068ded0 _$E250
// FUNCTION: STBC 0x0068dee0 _$E274
// FUNCTION: STBC 0x0068def0 _$E273
// FUNCTION: STBC 0x0068df00 _$E278
// FUNCTION: STBC 0x0068df10 _$E277
// FUNCTION: STBC 0x0068df20 _$E318
// FUNCTION: STBC 0x0068df30 _$E317
// FUNCTION: STBC 0x0068df40 _$E324
// FUNCTION: STBC 0x0068df50 _$E323
// FUNCTION: STBC 0x0068df60 _$E266
// FUNCTION: STBC 0x0068df70 _$E265
// FUNCTION: STBC 0x0068df80 _$E270
// FUNCTION: STBC 0x0068df90 _$E269
// FUNCTION: STBC 0x0068dfa0 _$E282
// FUNCTION: STBC 0x0068dfb0 _$E281
// FUNCTION: STBC 0x0068dfc0 _$E286
// FUNCTION: STBC 0x0068dfd0 _$E285
// FUNCTION: STBC 0x0068dfe0 _$E290
// FUNCTION: STBC 0x0068dff0 _$E289
// FUNCTION: STBC 0x0068e000 _$E294
// FUNCTION: STBC 0x0068e010 _$E293
// FUNCTION: STBC 0x0068e020 _$E298
// FUNCTION: STBC 0x0068e030 _$E297
// FUNCTION: STBC 0x0068e040 _$E302
// FUNCTION: STBC 0x0068e050 _$E301
// FUNCTION: STBC 0x0068e060 _$E306
// FUNCTION: STBC 0x0068e070 _$E305
// FUNCTION: STBC 0x0068e080 _$E310
// FUNCTION: STBC 0x0068e090 _$E309
// FUNCTION: STBC 0x0068e0a0 _$E116
// FUNCTION: STBC 0x0068e0b0 _$E115
// FUNCTION: STBC 0x0068e0c0 _$E314
// FUNCTION: STBC 0x0068e0d0 _$E313
// FUNCTION: STBC 0x0068e0e0 _$E121
// FUNCTION: STBC 0x0068e0f0 _$E120
// FUNCTION: STBC 0x0068e100 _$E328
// FUNCTION: STBC 0x0068e110 _$E327
// FUNCTION: STBC 0x0068e120 _$E332
// FUNCTION: STBC 0x0068e130 _$E331
// FUNCTION: STBC 0x0068e140 _$E336
// FUNCTION: STBC 0x0068e150 _$E335
// FUNCTION: STBC 0x0068e160 _$E340
// FUNCTION: STBC 0x0068e170 _$E339
// FUNCTION: STBC 0x0068e180 _$E130
// FUNCTION: STBC 0x0068e190 _$E129
// FUNCTION: STBC 0x0068e1a0 _$E344
// FUNCTION: STBC 0x0068e1b0 _$E343
// FUNCTION: STBC 0x0068e1c0 _$E348
// FUNCTION: STBC 0x0068e1d0 _$E347
// FUNCTION: STBC 0x0068e1e0 _$E352
// FUNCTION: STBC 0x0068e1f0 _$E351
// FUNCTION: STBC 0x0068e200 _$E136
// FUNCTION: STBC 0x0068e210 _$E135
// FUNCTION: STBC 0x0068e250 _$E141
// FUNCTION: STBC 0x0068e260 _$E140
// FUNCTION: STBC 0x0068e270 _$E171
// FUNCTION: STBC 0x0068e280 _$E170
// FUNCTION: STBC 0x0068e290 _$E356
// FUNCTION: STBC 0x0068e2a0 _$E355
// FUNCTION: STBC 0x0068e2b0 _$E176
// FUNCTION: STBC 0x0068e2c0 _$E175
// FUNCTION: STBC 0x0068e2d0 _$E360
// FUNCTION: STBC 0x0068e2e0 _$E359
// FUNCTION: STBC 0x0068e2f0 _$E364
// FUNCTION: STBC 0x0068e300 _$E363
// FUNCTION: STBC 0x0068e310 _$E181
// FUNCTION: STBC 0x0068e320 _$E180
// FUNCTION: STBC 0x0068e330 _$E368
// FUNCTION: STBC 0x0068e340 _$E367
// FUNCTION: STBC 0x0068e350 _$E146
// FUNCTION: STBC 0x0068e360 _$E145
// FUNCTION: STBC 0x0068e370 _$E372
// FUNCTION: STBC 0x0068e380 _$E371
// FUNCTION: STBC 0x0068e390 _$E376
// FUNCTION: STBC 0x0068e3a0 _$E375
// FUNCTION: STBC 0x0068e3b0 _$E186
// FUNCTION: STBC 0x0068e3c0 _$E185
// FUNCTION: STBC 0x0068e3d0 _$E191
// FUNCTION: STBC 0x0068e3e0 _$E190
// FUNCTION: STBC 0x0068e3f0 _$E380
// FUNCTION: STBC 0x0068e400 _$E379
// FUNCTION: STBC 0x0068e410 _$E384
// FUNCTION: STBC 0x0068e420 _$E383
// FUNCTION: STBC 0x0068e430 _$E388
// FUNCTION: STBC 0x0068e440 _$E387
// FUNCTION: STBC 0x0068e450 _$E392
// FUNCTION: STBC 0x0068e460 _$E391
// FUNCTION: STBC 0x0068e470 _$E396
// FUNCTION: STBC 0x0068e480 _$E395
// FUNCTION: STBC 0x0068e490 _$E400
// FUNCTION: STBC 0x0068e4a0 _$E399
// FUNCTION: STBC 0x0068e4b0 _$E404
// FUNCTION: STBC 0x0068e4c0 _$E403
// FUNCTION: STBC 0x0068e4d0 _$E408
// FUNCTION: STBC 0x0068e4e0 _$E407
// FUNCTION: STBC 0x0068e4f0 _$E196
// FUNCTION: STBC 0x0068e500 _$E195
// FUNCTION: STBC 0x0068e510 _$E412
// FUNCTION: STBC 0x0068e520 _$E411
// FUNCTION: STBC 0x0068e530 _$E201
// FUNCTION: STBC 0x0068e540 _$E200
// FUNCTION: STBC 0x0068e550 _$E206
// FUNCTION: STBC 0x0068e560 _$E205
// FUNCTION: STBC 0x0068e570 _$E416
// FUNCTION: STBC 0x0068e580 _$E415
// FUNCTION: STBC 0x0068e590 _$E421
// FUNCTION: STBC 0x0068e5a0 _$E420
// FUNCTION: STBC 0x0068e5b0 _$E425
// FUNCTION: STBC 0x0068e5c0 _$E424

/*
================
RepairSubsystemProperty::Cast

The property as a RepairSubsystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0068e5d0 ?Cast@RepairSubsystemProperty@@SAPAV1@PAVTGModelProperty@@@Z
RepairSubsystemProperty *RepairSubsystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( REPAIRSUBSYSTEMPROPERTY_TYPE ) ) {
		return (RepairSubsystemProperty *)property;
	}
	return NULL;
}

/*
================
RepairSubsystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0068e600
RepairSubsystemProperty *RepairSubsystemProperty::Create( char *name ) {
	RepairSubsystemProperty *property = new RepairSubsystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
RepairSubsystemProperty::RepairSubsystemProperty
================
*/
// FUNCTION: STBC 0x0068e650
RepairSubsystemProperty::RepairSubsystemProperty( long id ) : PoweredSubsystemProperty( id ) {
	maxRepairPoints = 0.0f;
	numRepairTeams = 1;
}

/*
================
RepairSubsystemProperty::WriteScript

Writes "<variable> = App.RepairSubsystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0068e700
void RepairSubsystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.RepairSubsystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
RepairSubsystemProperty::~RepairSubsystemProperty
================
*/
// FUNCTION: STBC 0x0068e830
RepairSubsystemProperty::~RepairSubsystemProperty( void ) {
}

/*
================
RepairSubsystemProperty::Copy
================
*/
// FUNCTION: STBC 0x0068e840
bool RepairSubsystemProperty::Copy( TGModelProperty *property ) {
	RepairSubsystemProperty *repairSubsystem = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetMaxRepairPoints( repairSubsystem->GetMaxRepairPoints() );
		SetNumRepairTeams( repairSubsystem->GetNumRepairTeams() );
	}
	return copied;
}

/*
================
RepairSubsystemProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x0068e880
void RepairSubsystemProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	PoweredSubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
RepairSubsystemProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0068e8a0
void RepairSubsystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetMaxRepairPoints(%f)\n", variableName, GetMaxRepairPoints() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetNumRepairTeams(%d)\n", variableName, GetNumRepairTeams() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
RepairSubsystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x0068e910
bool RepairSubsystemProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( maxRepairPoints );
	stream->WriteLong( numRepairTeams );
	return true;
}

/*
================
RepairSubsystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0068e950
bool RepairSubsystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	maxRepairPoints = stream->ReadFloat();
	numRepairTeams = stream->ReadLong();
	return true;
}
