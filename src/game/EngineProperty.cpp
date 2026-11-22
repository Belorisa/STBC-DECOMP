/*
===============================================================================

	EngineProperty.cpp

===============================================================================
*/

#include <iostream>

#include "EngineProperty.h"
#include "TGStream.h"
#include <string.h>

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006817f0 _$E32
// FUNCTION: STBC 0x00681800 _$E29
// FUNCTION: STBC 0x00681810 _$E31
// FUNCTION: STBC 0x00681820 _$E30
// FUNCTION: STBC 0x00681830 _$E38
// FUNCTION: STBC 0x00681840 _$E35
// FUNCTION: STBC 0x00681850 _$E37
// FUNCTION: STBC 0x00681860 _$E36
// FUNCTION: STBC 0x00681cd0 _$E433
// FUNCTION: STBC 0x00681cf0 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00681870 _$E65
// FUNCTION: STBC 0x00681880 _$E64
// FUNCTION: STBC 0x00681890 _$E69
// FUNCTION: STBC 0x006818a0 _$E68
// FUNCTION: STBC 0x006818b0 _$E80
// FUNCTION: STBC 0x006818c0 _$E79
// FUNCTION: STBC 0x006818d0 _$E84
// FUNCTION: STBC 0x006818e0 _$E83
// FUNCTION: STBC 0x006818f0 _$E88
// FUNCTION: STBC 0x00681900 _$E87
// FUNCTION: STBC 0x00681910 _$E92
// FUNCTION: STBC 0x00681920 _$E91
// FUNCTION: STBC 0x00681930 _$E211
// FUNCTION: STBC 0x00681940 _$E210
// FUNCTION: STBC 0x00681950 _$E153
// FUNCTION: STBC 0x00681960 _$E152
// FUNCTION: STBC 0x00681970 _$E166
// FUNCTION: STBC 0x00681980 _$E165
// FUNCTION: STBC 0x00681990 _$E216
// FUNCTION: STBC 0x006819a0 _$E215
// FUNCTION: STBC 0x006819b0 _$E221
// FUNCTION: STBC 0x006819c0 _$E220
// FUNCTION: STBC 0x006819d0 _$E225
// FUNCTION: STBC 0x006819e0 _$E224
// FUNCTION: STBC 0x006819f0 _$E229
// FUNCTION: STBC 0x00681a00 _$E228
// FUNCTION: STBC 0x00681a10 _$E236
// FUNCTION: STBC 0x00681a20 _$E235
// FUNCTION: STBC 0x00681a30 _$E258
// FUNCTION: STBC 0x00681a40 _$E257
// FUNCTION: STBC 0x00681a50 _$E262
// FUNCTION: STBC 0x00681a60 _$E261
// FUNCTION: STBC 0x00681a70 _$E111
// FUNCTION: STBC 0x00681a80 _$E110
// FUNCTION: STBC 0x00681a90 _$E244
// FUNCTION: STBC 0x00681aa0 _$E243
// FUNCTION: STBC 0x00681ab0 _$E251
// FUNCTION: STBC 0x00681ac0 _$E250
// FUNCTION: STBC 0x00681ad0 _$E274
// FUNCTION: STBC 0x00681ae0 _$E273
// FUNCTION: STBC 0x00681af0 _$E278
// FUNCTION: STBC 0x00681b00 _$E277
// FUNCTION: STBC 0x00681b10 _$E318
// FUNCTION: STBC 0x00681b20 _$E317
// FUNCTION: STBC 0x00681b30 _$E324
// FUNCTION: STBC 0x00681b40 _$E323
// FUNCTION: STBC 0x00681b50 _$E266
// FUNCTION: STBC 0x00681b60 _$E265
// FUNCTION: STBC 0x00681b70 _$E270
// FUNCTION: STBC 0x00681b80 _$E269
// FUNCTION: STBC 0x00681b90 _$E282
// FUNCTION: STBC 0x00681ba0 _$E281
// FUNCTION: STBC 0x00681bb0 _$E286
// FUNCTION: STBC 0x00681bc0 _$E285
// FUNCTION: STBC 0x00681bd0 _$E290
// FUNCTION: STBC 0x00681be0 _$E289
// FUNCTION: STBC 0x00681bf0 _$E294
// FUNCTION: STBC 0x00681c00 _$E293
// FUNCTION: STBC 0x00681c10 _$E298
// FUNCTION: STBC 0x00681c20 _$E297
// FUNCTION: STBC 0x00681c30 _$E302
// FUNCTION: STBC 0x00681c40 _$E301
// FUNCTION: STBC 0x00681c50 _$E306
// FUNCTION: STBC 0x00681c60 _$E305
// FUNCTION: STBC 0x00681c70 _$E310
// FUNCTION: STBC 0x00681c80 _$E309
// FUNCTION: STBC 0x00681c90 _$E116
// FUNCTION: STBC 0x00681ca0 _$E115
// FUNCTION: STBC 0x00681cb0 _$E314
// FUNCTION: STBC 0x00681cc0 _$E313
// FUNCTION: STBC 0x00681d00 _$E121
// FUNCTION: STBC 0x00681d10 _$E120
// FUNCTION: STBC 0x00681d20 _$E328
// FUNCTION: STBC 0x00681d30 _$E327
// FUNCTION: STBC 0x00681d40 _$E332
// FUNCTION: STBC 0x00681d50 _$E331
// FUNCTION: STBC 0x00681d60 _$E336
// FUNCTION: STBC 0x00681d70 _$E335
// FUNCTION: STBC 0x00681d80 _$E340
// FUNCTION: STBC 0x00681d90 _$E339
// FUNCTION: STBC 0x00681da0 _$E130
// FUNCTION: STBC 0x00681db0 _$E129
// FUNCTION: STBC 0x00681dc0 _$E344
// FUNCTION: STBC 0x00681dd0 _$E343
// FUNCTION: STBC 0x00681de0 _$E348
// FUNCTION: STBC 0x00681df0 _$E347
// FUNCTION: STBC 0x00681e00 _$E352
// FUNCTION: STBC 0x00681e10 _$E351
// FUNCTION: STBC 0x00681e20 _$E136
// FUNCTION: STBC 0x00681e30 _$E135
// FUNCTION: STBC 0x00681e40 _$E141
// FUNCTION: STBC 0x00681e50 _$E140
// FUNCTION: STBC 0x00681e60 _$E171
// FUNCTION: STBC 0x00681e70 _$E170
// FUNCTION: STBC 0x00681e80 _$E356
// FUNCTION: STBC 0x00681e90 _$E355
// FUNCTION: STBC 0x00681ea0 _$E176
// FUNCTION: STBC 0x00681eb0 _$E175
// FUNCTION: STBC 0x00681ec0 _$E360
// FUNCTION: STBC 0x00681ed0 _$E359
// FUNCTION: STBC 0x00681ee0 _$E364
// FUNCTION: STBC 0x00681ef0 _$E363
// FUNCTION: STBC 0x00681f00 _$E181
// FUNCTION: STBC 0x00681f10 _$E180
// FUNCTION: STBC 0x00681f20 _$E368
// FUNCTION: STBC 0x00681f30 _$E367
// FUNCTION: STBC 0x00681f40 _$E146
// FUNCTION: STBC 0x00681f50 _$E145
// FUNCTION: STBC 0x00681f60 _$E372
// FUNCTION: STBC 0x00681f70 _$E371
// FUNCTION: STBC 0x00681f80 _$E376
// FUNCTION: STBC 0x00681f90 _$E375
// FUNCTION: STBC 0x00681fa0 _$E186
// FUNCTION: STBC 0x00681fb0 _$E185
// FUNCTION: STBC 0x00681fc0 _$E191
// FUNCTION: STBC 0x00681fd0 _$E190
// FUNCTION: STBC 0x00681fe0 _$E380
// FUNCTION: STBC 0x00681ff0 _$E379
// FUNCTION: STBC 0x00682000 _$E384
// FUNCTION: STBC 0x00682010 _$E383
// FUNCTION: STBC 0x00682020 _$E388
// FUNCTION: STBC 0x00682030 _$E387
// FUNCTION: STBC 0x00682040 _$E392
// FUNCTION: STBC 0x00682050 _$E391
// FUNCTION: STBC 0x00682060 _$E396
// FUNCTION: STBC 0x00682070 _$E395
// FUNCTION: STBC 0x00682080 _$E400
// FUNCTION: STBC 0x00682090 _$E399
// FUNCTION: STBC 0x006820a0 _$E404
// FUNCTION: STBC 0x006820b0 _$E403
// FUNCTION: STBC 0x006820c0 _$E408
// FUNCTION: STBC 0x006820d0 _$E407
// FUNCTION: STBC 0x006820e0 _$E196
// FUNCTION: STBC 0x006820f0 _$E195
// FUNCTION: STBC 0x00682100 _$E412
// FUNCTION: STBC 0x00682110 _$E411
// FUNCTION: STBC 0x00682120 _$E201
// FUNCTION: STBC 0x00682130 _$E200
// FUNCTION: STBC 0x00682140 _$E206
// FUNCTION: STBC 0x00682150 _$E205
// FUNCTION: STBC 0x00682160 _$E416
// FUNCTION: STBC 0x00682170 _$E415
// FUNCTION: STBC 0x00682180 _$E421
// FUNCTION: STBC 0x00682190 _$E420
// FUNCTION: STBC 0x006821a0 _$E425
// FUNCTION: STBC 0x006821b0 _$E424

/*
================
EngineProperty::Cast

The property as a EngineProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006821c0 ?Cast@EngineProperty@@SAPAV1@PAVTGModelProperty@@@Z
EngineProperty *EngineProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( ENGINEPROPERTY_TYPE ) ) {
		return (EngineProperty *)property;
	}
	return NULL;
}

/*
================
EngineProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x006821f0
EngineProperty *EngineProperty::Create( char *name ) {
	EngineProperty *property = new EngineProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
EngineProperty::EngineProperty
================
*/
// FUNCTION: STBC 0x00682240
EngineProperty::EngineProperty( long id ) : SubsystemProperty( id ) {
	engineType = EP_IMPULSE;
}

/*
================
EngineProperty::WriteScript

Writes "<variable> = App.EngineProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x006822e0
void EngineProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.EngineProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
EngineProperty::~EngineProperty
================
*/
// FUNCTION: STBC 0x00682410
EngineProperty::~EngineProperty( void ) {
}

/*
================
EngineProperty::Copy
================
*/
// FUNCTION: STBC 0x00682420
bool EngineProperty::Copy( TGModelProperty *property ) {
	EngineProperty *engine = Cast( property );
	bool copied = SubsystemProperty::Copy( property );
	if ( copied == true ) {
		SetEngineType( engine->GetEngineType() );
	}
	return copied;
}

/*
================
EngineProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00682450
void EngineProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	SubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
EngineProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00682470
void EngineProperty::WriteInit( TGStream *stream, char *variableName ) {
	SubsystemProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%s.SetEngineType(%s.", variableName, variableName );
	strcat( line, GetEngineType() == EP_IMPULSE ? "EP_IMPULSE)\n" : "EP_WARP)\n" );
	stream->WriteCLine( line );

	delete line;
}

/*
================
EngineProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00682500
bool EngineProperty::WriteToStream( TGStream *stream ) {
	if ( !SubsystemProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( engineType );
	return true;
}

/*
================
EngineProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00682530
bool EngineProperty::ReadFromStream( TGStream *stream ) {
	if ( !SubsystemProperty::ReadFromStream( stream ) ) {
		return false;
	}
	engineType = stream->ReadLong();
	return true;
}
