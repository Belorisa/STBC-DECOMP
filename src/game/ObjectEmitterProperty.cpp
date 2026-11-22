/*
===============================================================================

	ObjectEmitterProperty.cpp

===============================================================================
*/

#include <iostream>

#include "ObjectEmitterProperty.h"
#include "TGStream.h"
#include <string.h>

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006840b0 _$E32
// FUNCTION: STBC 0x006840c0 _$E29
// FUNCTION: STBC 0x006840d0 _$E31
// FUNCTION: STBC 0x006840e0 _$E30
// FUNCTION: STBC 0x006840f0 _$E38
// FUNCTION: STBC 0x00684100 _$E35
// FUNCTION: STBC 0x00684110 _$E37
// FUNCTION: STBC 0x00684120 _$E36
// FUNCTION: STBC 0x00684530 _$E433
// FUNCTION: STBC 0x00684550 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00684130 _$E62
// FUNCTION: STBC 0x00684140 _$E61
// FUNCTION: STBC 0x00684150 _$E66
// FUNCTION: STBC 0x00684160 _$E65
// FUNCTION: STBC 0x00684170 _$E77
// FUNCTION: STBC 0x00684180 _$E76
// FUNCTION: STBC 0x00684190 _$E81
// FUNCTION: STBC 0x006841a0 _$E80
// FUNCTION: STBC 0x006841b0 _$E85
// FUNCTION: STBC 0x006841c0 _$E84
// FUNCTION: STBC 0x006841d0 _$E89
// FUNCTION: STBC 0x006841e0 _$E88
// FUNCTION: STBC 0x006841f0 _$E208
// FUNCTION: STBC 0x00684200 _$E207
// FUNCTION: STBC 0x00684210 _$E150
// FUNCTION: STBC 0x00684220 _$E149
// FUNCTION: STBC 0x00684230 _$E163
// FUNCTION: STBC 0x00684240 _$E162
// FUNCTION: STBC 0x00684250 _$E213
// FUNCTION: STBC 0x00684260 _$E212
// FUNCTION: STBC 0x00684270 _$E218
// FUNCTION: STBC 0x00684280 _$E217
// FUNCTION: STBC 0x00684290 _$E222
// FUNCTION: STBC 0x006842a0 _$E221
// FUNCTION: STBC 0x006842b0 _$E226
// FUNCTION: STBC 0x006842c0 _$E225
// FUNCTION: STBC 0x006842d0 _$E233
// FUNCTION: STBC 0x006842e0 _$E232
// FUNCTION: STBC 0x006842f0 _$E258
// FUNCTION: STBC 0x00684300 _$E257
// FUNCTION: STBC 0x00684310 _$E262
// FUNCTION: STBC 0x00684320 _$E261
// FUNCTION: STBC 0x00684330 _$E108
// FUNCTION: STBC 0x00684340 _$E107
// FUNCTION: STBC 0x00684350 _$E244
// FUNCTION: STBC 0x00684360 _$E243
// FUNCTION: STBC 0x00684370 _$E251
// FUNCTION: STBC 0x00684380 _$E250
// FUNCTION: STBC 0x00684390 _$E274
// FUNCTION: STBC 0x006843a0 _$E273
// FUNCTION: STBC 0x006843b0 _$E278
// FUNCTION: STBC 0x006843c0 _$E277
// FUNCTION: STBC 0x006843d0 _$E318
// FUNCTION: STBC 0x006843e0 _$E317
// FUNCTION: STBC 0x006843f0 _$E324
// FUNCTION: STBC 0x00684400 _$E323
// FUNCTION: STBC 0x00684410 _$E266
// FUNCTION: STBC 0x00684420 _$E265
// FUNCTION: STBC 0x00684430 _$E270
// FUNCTION: STBC 0x00684440 _$E269
// FUNCTION: STBC 0x00684450 _$E282
// FUNCTION: STBC 0x00684460 _$E281
// FUNCTION: STBC 0x00684470 _$E286
// FUNCTION: STBC 0x00684480 _$E285
// FUNCTION: STBC 0x00684490 _$E290
// FUNCTION: STBC 0x006844a0 _$E289
// FUNCTION: STBC 0x006844b0 _$E294
// FUNCTION: STBC 0x006844c0 _$E293
// FUNCTION: STBC 0x006844d0 _$E298
// FUNCTION: STBC 0x006844e0 _$E297
// FUNCTION: STBC 0x006844f0 _$E302
// FUNCTION: STBC 0x00684500 _$E301
// FUNCTION: STBC 0x00684510 _$E306
// FUNCTION: STBC 0x00684520 _$E305
// FUNCTION: STBC 0x00684560 _$E310
// FUNCTION: STBC 0x00684570 _$E309
// FUNCTION: STBC 0x00684580 _$E113
// FUNCTION: STBC 0x00684590 _$E112
// FUNCTION: STBC 0x006845a0 _$E314
// FUNCTION: STBC 0x006845b0 _$E313
// FUNCTION: STBC 0x006845c0 _$E118
// FUNCTION: STBC 0x006845d0 _$E117
// FUNCTION: STBC 0x006845e0 _$E328
// FUNCTION: STBC 0x006845f0 _$E327
// FUNCTION: STBC 0x00684600 _$E332
// FUNCTION: STBC 0x00684610 _$E331
// FUNCTION: STBC 0x00684620 _$E336
// FUNCTION: STBC 0x00684630 _$E335
// FUNCTION: STBC 0x00684640 _$E340
// FUNCTION: STBC 0x00684650 _$E339
// FUNCTION: STBC 0x00684660 _$E127
// FUNCTION: STBC 0x00684670 _$E126
// FUNCTION: STBC 0x00684680 _$E344
// FUNCTION: STBC 0x00684690 _$E343
// FUNCTION: STBC 0x006846a0 _$E348
// FUNCTION: STBC 0x006846b0 _$E347
// FUNCTION: STBC 0x006846c0 _$E352
// FUNCTION: STBC 0x006846d0 _$E351
// FUNCTION: STBC 0x006846e0 _$E133
// FUNCTION: STBC 0x006846f0 _$E132
// FUNCTION: STBC 0x00684700 _$E138
// FUNCTION: STBC 0x00684710 _$E137
// FUNCTION: STBC 0x00684720 _$E168
// FUNCTION: STBC 0x00684730 _$E167
// FUNCTION: STBC 0x00684740 _$E356
// FUNCTION: STBC 0x00684750 _$E355
// FUNCTION: STBC 0x00684760 _$E173
// FUNCTION: STBC 0x00684770 _$E172
// FUNCTION: STBC 0x00684780 _$E360
// FUNCTION: STBC 0x00684790 _$E359
// FUNCTION: STBC 0x006847a0 _$E364
// FUNCTION: STBC 0x006847b0 _$E363
// FUNCTION: STBC 0x006847c0 _$E178
// FUNCTION: STBC 0x006847d0 _$E177
// FUNCTION: STBC 0x006847e0 _$E368
// FUNCTION: STBC 0x006847f0 _$E367
// FUNCTION: STBC 0x00684800 _$E143
// FUNCTION: STBC 0x00684810 _$E142
// FUNCTION: STBC 0x00684820 _$E372
// FUNCTION: STBC 0x00684830 _$E371
// FUNCTION: STBC 0x00684840 _$E376
// FUNCTION: STBC 0x00684850 _$E375
// FUNCTION: STBC 0x00684860 _$E183
// FUNCTION: STBC 0x00684870 _$E182
// FUNCTION: STBC 0x00684880 _$E188
// FUNCTION: STBC 0x00684890 _$E187
// FUNCTION: STBC 0x006848a0 _$E380
// FUNCTION: STBC 0x006848b0 _$E379
// FUNCTION: STBC 0x006848c0 _$E384
// FUNCTION: STBC 0x006848d0 _$E383
// FUNCTION: STBC 0x006848e0 _$E388
// FUNCTION: STBC 0x006848f0 _$E387
// FUNCTION: STBC 0x00684900 _$E392
// FUNCTION: STBC 0x00684910 _$E391
// FUNCTION: STBC 0x00684920 _$E396
// FUNCTION: STBC 0x00684930 _$E395
// FUNCTION: STBC 0x00684940 _$E400
// FUNCTION: STBC 0x00684950 _$E399
// FUNCTION: STBC 0x00684960 _$E404
// FUNCTION: STBC 0x00684970 _$E403
// FUNCTION: STBC 0x00684980 _$E408
// FUNCTION: STBC 0x00684990 _$E407
// FUNCTION: STBC 0x006849a0 _$E193
// FUNCTION: STBC 0x006849b0 _$E192
// FUNCTION: STBC 0x006849c0 _$E412
// FUNCTION: STBC 0x006849d0 _$E411
// FUNCTION: STBC 0x006849e0 _$E198
// FUNCTION: STBC 0x006849f0 _$E197
// FUNCTION: STBC 0x00684a00 _$E203
// FUNCTION: STBC 0x00684a10 _$E202
// FUNCTION: STBC 0x00684a20 _$E416
// FUNCTION: STBC 0x00684a30 _$E415
// FUNCTION: STBC 0x00684a40 _$E421
// FUNCTION: STBC 0x00684a50 _$E420
// FUNCTION: STBC 0x00684a60 _$E425
// FUNCTION: STBC 0x00684a70 _$E424

/*
================
ObjectEmitterProperty::Cast

The property as a ObjectEmitterProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00684a80 ?Cast@ObjectEmitterProperty@@SAPAV1@PAVTGModelProperty@@@Z
ObjectEmitterProperty *ObjectEmitterProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( OBJECTEMITTERPROPERTY_TYPE ) ) {
		return (ObjectEmitterProperty *)property;
	}
	return NULL;
}

/*
================
ObjectEmitterProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00684ab0
ObjectEmitterProperty *ObjectEmitterProperty::Create( char *name ) {
	ObjectEmitterProperty *property = new ObjectEmitterProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
ObjectEmitterProperty::ObjectEmitterProperty
================
*/
// FUNCTION: STBC 0x00684b00
ObjectEmitterProperty::ObjectEmitterProperty( long id ) : PositionOrientationProperty( id ) {
	emittedObjectType = OEP_UNKNOWN;
}

/*
================
ObjectEmitterProperty::WriteScript

Writes "<variable> = App.ObjectEmitterProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00684ba0
void ObjectEmitterProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.ObjectEmitterProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
ObjectEmitterProperty::~ObjectEmitterProperty
================
*/
// FUNCTION: STBC 0x00684cd0
ObjectEmitterProperty::~ObjectEmitterProperty( void ) {
}

/*
================
ObjectEmitterProperty::Copy
================
*/
// FUNCTION: STBC 0x00684ce0
bool ObjectEmitterProperty::Copy( TGModelProperty *property ) {
	ObjectEmitterProperty *objectEmitter = Cast( property );
	bool copied = PositionOrientationProperty::Copy( property );
	if ( copied == true ) {
		SetEmittedObjectType( objectEmitter->GetEmittedObjectType() );
	}
	return copied;
}

/*
================
ObjectEmitterProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00684d10
void ObjectEmitterProperty::WriteInit( TGStream *stream, char *variableName ) {
	char line[512];

	PositionOrientationProperty::WriteInit( stream, variableName );

	sprintf( line, "%s.SetEmittedObjectType(%s.", variableName, variableName );
	switch ( GetEmittedObjectType() ) {
		case OEP_SHUTTLE:
			strcat( line, "OEP_SHUTTLE)\n" );
			break;
		case OEP_PROBE:
			strcat( line, "OEP_PROBE)\n" );
			break;
		case OEP_DECOY:
			strcat( line, "OEP_DECOY)\n" );
			break;
		case OEP_UNKNOWN:
			strcat( line, "OEP_UNKNOWN)\n" );
			break;
		default:
			strcat( line, "OEP_UNKNOWN)\n" );
			break;
	}
	stream->WriteCLine( line );
}

/*
================
ObjectEmitterProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00684dd0
bool ObjectEmitterProperty::WriteToStream( TGStream *stream ) {
	if ( !PositionOrientationProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( emittedObjectType );
	return true;
}

/*
================
ObjectEmitterProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00684e00
bool ObjectEmitterProperty::ReadFromStream( TGStream *stream ) {
	if ( !PositionOrientationProperty::ReadFromStream( stream ) ) {
		return false;
	}
	emittedObjectType = stream->ReadLong();
	return true;
}
