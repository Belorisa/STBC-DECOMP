/*
===============================================================================

	ShieldProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>

#include "ShieldProperty.h"
#include "ShieldClass.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0068f720 _$E32
// FUNCTION: STBC 0x0068f730 _$E29
// FUNCTION: STBC 0x0068f740 _$E31
// FUNCTION: STBC 0x0068f750 _$E30
// FUNCTION: STBC 0x0068f760 _$E38
// FUNCTION: STBC 0x0068f770 _$E35
// FUNCTION: STBC 0x0068f780 _$E37
// FUNCTION: STBC 0x0068f790 _$E36
// FUNCTION: STBC 0x0068fba0 _$E443
// FUNCTION: STBC 0x0068fbc0 _$E442

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0068f7a0 _$E79
// FUNCTION: STBC 0x0068f7b0 _$E78
// FUNCTION: STBC 0x0068f7c0 _$E83
// FUNCTION: STBC 0x0068f7d0 _$E82
// FUNCTION: STBC 0x0068f7e0 _$E94
// FUNCTION: STBC 0x0068f7f0 _$E93
// FUNCTION: STBC 0x0068f800 _$E98
// FUNCTION: STBC 0x0068f810 _$E97
// FUNCTION: STBC 0x0068f820 _$E102
// FUNCTION: STBC 0x0068f830 _$E101
// FUNCTION: STBC 0x0068f840 _$E106
// FUNCTION: STBC 0x0068f850 _$E105
// FUNCTION: STBC 0x0068f860 _$E221
// FUNCTION: STBC 0x0068f870 _$E220
// FUNCTION: STBC 0x0068f880 _$E163
// FUNCTION: STBC 0x0068f890 _$E162
// FUNCTION: STBC 0x0068f8a0 _$E176
// FUNCTION: STBC 0x0068f8b0 _$E175
// FUNCTION: STBC 0x0068f8c0 _$E226
// FUNCTION: STBC 0x0068f8d0 _$E225
// FUNCTION: STBC 0x0068f8e0 _$E231
// FUNCTION: STBC 0x0068f8f0 _$E230
// FUNCTION: STBC 0x0068f900 _$E235
// FUNCTION: STBC 0x0068f910 _$E234
// FUNCTION: STBC 0x0068f920 _$E239
// FUNCTION: STBC 0x0068f930 _$E238
// FUNCTION: STBC 0x0068f940 _$E246
// FUNCTION: STBC 0x0068f950 _$E245
// FUNCTION: STBC 0x0068f960 _$E268
// FUNCTION: STBC 0x0068f970 _$E267
// FUNCTION: STBC 0x0068f980 _$E272
// FUNCTION: STBC 0x0068f990 _$E271
// FUNCTION: STBC 0x0068f9a0 _$E125
// FUNCTION: STBC 0x0068f9b0 _$E124
// FUNCTION: STBC 0x0068f9c0 _$E254
// FUNCTION: STBC 0x0068f9d0 _$E253
// FUNCTION: STBC 0x0068f9e0 _$E261
// FUNCTION: STBC 0x0068f9f0 _$E260
// FUNCTION: STBC 0x0068fa00 _$E284
// FUNCTION: STBC 0x0068fa10 _$E283
// FUNCTION: STBC 0x0068fa20 _$E288
// FUNCTION: STBC 0x0068fa30 _$E287
// FUNCTION: STBC 0x0068fa40 _$E328
// FUNCTION: STBC 0x0068fa50 _$E327
// FUNCTION: STBC 0x0068fa60 _$E334
// FUNCTION: STBC 0x0068fa70 _$E333
// FUNCTION: STBC 0x0068fa80 _$E276
// FUNCTION: STBC 0x0068fa90 _$E275
// FUNCTION: STBC 0x0068faa0 _$E280
// FUNCTION: STBC 0x0068fab0 _$E279
// FUNCTION: STBC 0x0068fac0 _$E292
// FUNCTION: STBC 0x0068fad0 _$E291
// FUNCTION: STBC 0x0068fae0 _$E296
// FUNCTION: STBC 0x0068faf0 _$E295
// FUNCTION: STBC 0x0068fb00 _$E300
// FUNCTION: STBC 0x0068fb10 _$E299
// FUNCTION: STBC 0x0068fb20 _$E304
// FUNCTION: STBC 0x0068fb30 _$E303
// FUNCTION: STBC 0x0068fb40 _$E308
// FUNCTION: STBC 0x0068fb50 _$E307
// FUNCTION: STBC 0x0068fb60 _$E312
// FUNCTION: STBC 0x0068fb70 _$E311
// FUNCTION: STBC 0x0068fb80 _$E316
// FUNCTION: STBC 0x0068fb90 _$E315
// FUNCTION: STBC 0x0068fbd0 _$E320
// FUNCTION: STBC 0x0068fbe0 _$E319
// FUNCTION: STBC 0x0068fbf0 _$E130
// FUNCTION: STBC 0x0068fc00 _$E129
// FUNCTION: STBC 0x0068fc10 _$E324
// FUNCTION: STBC 0x0068fc20 _$E323
// FUNCTION: STBC 0x0068fc30 _$E135
// FUNCTION: STBC 0x0068fc40 _$E134
// FUNCTION: STBC 0x0068fc50 _$E338
// FUNCTION: STBC 0x0068fc60 _$E337
// FUNCTION: STBC 0x0068fc70 _$E342
// FUNCTION: STBC 0x0068fc80 _$E341
// FUNCTION: STBC 0x0068fc90 _$E346
// FUNCTION: STBC 0x0068fca0 _$E345
// FUNCTION: STBC 0x0068fcb0 _$E350
// FUNCTION: STBC 0x0068fcc0 _$E349
// FUNCTION: STBC 0x0068fcd0 _$E140
// FUNCTION: STBC 0x0068fce0 _$E139
// FUNCTION: STBC 0x0068fcf0 _$E354
// FUNCTION: STBC 0x0068fd00 _$E353
// FUNCTION: STBC 0x0068fd10 _$E358
// FUNCTION: STBC 0x0068fd20 _$E357
// FUNCTION: STBC 0x0068fd30 _$E362
// FUNCTION: STBC 0x0068fd40 _$E361
// FUNCTION: STBC 0x0068fd50 _$E146
// FUNCTION: STBC 0x0068fd60 _$E145
// FUNCTION: STBC 0x0068fd70 _$E151
// FUNCTION: STBC 0x0068fd80 _$E150
// FUNCTION: STBC 0x0068fd90 _$E181
// FUNCTION: STBC 0x0068fda0 _$E180
// FUNCTION: STBC 0x0068fdb0 _$E366
// FUNCTION: STBC 0x0068fdc0 _$E365
// FUNCTION: STBC 0x0068fdd0 _$E186
// FUNCTION: STBC 0x0068fde0 _$E185
// FUNCTION: STBC 0x0068fdf0 _$E370
// FUNCTION: STBC 0x0068fe00 _$E369
// FUNCTION: STBC 0x0068fe10 _$E374
// FUNCTION: STBC 0x0068fe20 _$E373
// FUNCTION: STBC 0x0068fe30 _$E191
// FUNCTION: STBC 0x0068fe40 _$E190
// FUNCTION: STBC 0x0068fe50 _$E378
// FUNCTION: STBC 0x0068fe60 _$E377
// FUNCTION: STBC 0x0068fe70 _$E156
// FUNCTION: STBC 0x0068fe80 _$E155
// FUNCTION: STBC 0x0068fe90 _$E382
// FUNCTION: STBC 0x0068fea0 _$E381
// FUNCTION: STBC 0x0068feb0 _$E386
// FUNCTION: STBC 0x0068fec0 _$E385
// FUNCTION: STBC 0x0068fed0 _$E196
// FUNCTION: STBC 0x0068fee0 _$E195
// FUNCTION: STBC 0x0068fef0 _$E201
// FUNCTION: STBC 0x0068ff00 _$E200
// FUNCTION: STBC 0x0068ff10 _$E390
// FUNCTION: STBC 0x0068ff20 _$E389
// FUNCTION: STBC 0x0068ff30 _$E394
// FUNCTION: STBC 0x0068ff40 _$E393
// FUNCTION: STBC 0x0068ff50 _$E398
// FUNCTION: STBC 0x0068ff60 _$E397
// FUNCTION: STBC 0x0068ff70 _$E402
// FUNCTION: STBC 0x0068ff80 _$E401
// FUNCTION: STBC 0x0068ff90 _$E406
// FUNCTION: STBC 0x0068ffa0 _$E405
// FUNCTION: STBC 0x0068ffb0 _$E410
// FUNCTION: STBC 0x0068ffc0 _$E409
// FUNCTION: STBC 0x0068ffd0 _$E414
// FUNCTION: STBC 0x0068ffe0 _$E413
// FUNCTION: STBC 0x0068fff0 _$E418
// FUNCTION: STBC 0x00690000 _$E417
// FUNCTION: STBC 0x00690010 _$E206
// FUNCTION: STBC 0x00690020 _$E205
// FUNCTION: STBC 0x00690030 _$E422
// FUNCTION: STBC 0x00690040 _$E421
// FUNCTION: STBC 0x00690050 _$E211
// FUNCTION: STBC 0x00690060 _$E210
// FUNCTION: STBC 0x00690070 _$E216
// FUNCTION: STBC 0x00690080 _$E215
// FUNCTION: STBC 0x00690090 _$E426
// FUNCTION: STBC 0x006900a0 _$E425
// FUNCTION: STBC 0x006900b0 _$E431
// FUNCTION: STBC 0x006900c0 _$E430
// FUNCTION: STBC 0x006900d0 _$E435
// FUNCTION: STBC 0x006900e0 _$E434

/*
================
ShieldProperty::Cast

The property as a ShieldProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006900f0 ?Cast@ShieldProperty@@SAPAV1@PAVTGModelProperty@@@Z
ShieldProperty *ShieldProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SHIELDPROPERTY_TYPE ) ) {
		return (ShieldProperty *)property;
	}
	return NULL;
}

/*
================
ShieldProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00690120
ShieldProperty *ShieldProperty::Create( char *name ) {
	ShieldProperty *property = new ShieldProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
ShieldProperty::ShieldProperty
================
*/
// FUNCTION: STBC 0x00690180
ShieldProperty::ShieldProperty( long id ) : PoweredSubsystemProperty( id ) {
	shieldGlowDecay = 1.0f;
	shieldGlowColor.SetRGBA( 0.0f, 0.0f, 0.0f, 0.0f );
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
		maxShields[i] = 1000.0f;
		shieldChargePerSecond[i] = 1.0f;
	}
}

/*
================
ShieldProperty::WriteScript

Writes "<variable> = App.ShieldProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00690260
void ShieldProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.ShieldProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
ShieldProperty::~ShieldProperty
================
*/
// FUNCTION: STBC 0x00690390
ShieldProperty::~ShieldProperty( void ) {
}

/*
================
ShieldProperty::Copy
================
*/
// FUNCTION: STBC 0x006903a0
bool ShieldProperty::Copy( TGModelProperty *property ) {
	ShieldProperty *shield = Cast( property );
	bool copied = PoweredSubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetShieldGlowColor( shield->GetShieldGlowColor() );
		SetShieldGlowDecay( shield->GetShieldGlowDecay() );
		for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
			maxShields[i] = shield->maxShields[i];
			shieldChargePerSecond[i] = shield->shieldChargePerSecond[i];
		}
	}
	return copied;
}

/*
================
ShieldProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00690410
void ShieldProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%sShieldGlowColor = App.TGColorA()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sShieldGlowColor.SetRGBA(%f, %f, %f, %f)\n", variableName, shieldGlowColor.r, shieldGlowColor.g, shieldGlowColor.b, shieldGlowColor.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldGlowColor(%sShieldGlowColor)\n", variableName, variableName );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldGlowDecay(%f)\n", variableName, GetShieldGlowDecay() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.FRONT_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::FRONT_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.REAR_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::REAR_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.TOP_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::TOP_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.BOTTOM_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::BOTTOM_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.LEFT_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::LEFT_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxShields(%s.RIGHT_SHIELDS, %f)\n", variableName, variableName, GetMaxShields( ShieldClass::RIGHT_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.FRONT_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::FRONT_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.REAR_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::REAR_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.TOP_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::TOP_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.BOTTOM_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::BOTTOM_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.LEFT_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::LEFT_SHIELDS ) );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetShieldChargePerSecond(%s.RIGHT_SHIELDS, %f)\n", variableName, variableName, GetShieldChargePerSecond( ShieldClass::RIGHT_SHIELDS ) );
	stream->WriteCLine( line );

	delete line;
}

/*
================
ShieldProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x006906b0
bool ShieldProperty::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( shieldGlowColor.GetR() );
	stream->WriteFloat( shieldGlowColor.GetG() );
	stream->WriteFloat( shieldGlowColor.GetB() );
	stream->WriteFloat( shieldGlowColor.GetA() );
	stream->WriteFloat( shieldGlowDecay );
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
		stream->WriteFloat( maxShields[i] );
		stream->WriteFloat( shieldChargePerSecond[i] );
	}
	return true;
}

/*
================
ShieldProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00690740
bool ShieldProperty::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	shieldGlowColor.r = stream->ReadFloat();
	shieldGlowColor.g = stream->ReadFloat();
	shieldGlowColor.b = stream->ReadFloat();
	shieldGlowColor.a = stream->ReadFloat();
	shieldGlowDecay = stream->ReadFloat();
	for ( int i = 0; i < ShieldClass::NUM_SHIELDS; i++ ) {
		maxShields[i] = stream->ReadFloat();
		shieldChargePerSecond[i] = stream->ReadFloat();
	}
	return true;
}
