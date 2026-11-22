/*
===============================================================================

	WarpEngineProperty.cpp

===============================================================================
*/

#include <iostream>

#include "WarpEngineProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00698a10 _$E32
// FUNCTION: STBC 0x00698a20 _$E29
// FUNCTION: STBC 0x00698a30 _$E31
// FUNCTION: STBC 0x00698a40 _$E30
// FUNCTION: STBC 0x00698a50 _$E38
// FUNCTION: STBC 0x00698a60 _$E35
// FUNCTION: STBC 0x00698a70 _$E37
// FUNCTION: STBC 0x00698a80 _$E36
// FUNCTION: STBC 0x00699070 _$E433
// FUNCTION: STBC 0x00699090 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00698a90 _$E65
// FUNCTION: STBC 0x00698aa0 _$E64
// FUNCTION: STBC 0x00698ab0 _$E69
// FUNCTION: STBC 0x00698ac0 _$E68
// FUNCTION: STBC 0x00698ad0 _$E80
// FUNCTION: STBC 0x00698ae0 _$E79
// FUNCTION: STBC 0x00698af0 _$E84
// FUNCTION: STBC 0x00698b00 _$E83
// FUNCTION: STBC 0x00698b10 _$E88
// FUNCTION: STBC 0x00698b20 _$E87
// FUNCTION: STBC 0x00698b30 _$E92
// FUNCTION: STBC 0x00698b40 _$E91
// FUNCTION: STBC 0x00698b50 _$E211
// FUNCTION: STBC 0x00698b60 _$E210
// FUNCTION: STBC 0x00698b70 _$E153
// FUNCTION: STBC 0x00698b80 _$E152
// FUNCTION: STBC 0x00698b90 _$E166
// FUNCTION: STBC 0x00698ba0 _$E165
// FUNCTION: STBC 0x00698bb0 _$E216
// FUNCTION: STBC 0x00698bc0 _$E215
// FUNCTION: STBC 0x00698bd0 _$E221
// FUNCTION: STBC 0x00698be0 _$E220
// FUNCTION: STBC 0x00698bf0 _$E225
// FUNCTION: STBC 0x00698c00 _$E224
// FUNCTION: STBC 0x00698c10 _$E229
// FUNCTION: STBC 0x00698c20 _$E228
// FUNCTION: STBC 0x00698c30 _$E236
// FUNCTION: STBC 0x00698c40 _$E235
// FUNCTION: STBC 0x00698c50 _$E258
// FUNCTION: STBC 0x00698c60 _$E257
// FUNCTION: STBC 0x00698c70 _$E262
// FUNCTION: STBC 0x00698c80 _$E261
// FUNCTION: STBC 0x00698c90 _$E111
// FUNCTION: STBC 0x00698ca0 _$E110
// FUNCTION: STBC 0x00698cb0 _$E244
// FUNCTION: STBC 0x00698cc0 _$E243
// FUNCTION: STBC 0x00698cd0 _$E251
// FUNCTION: STBC 0x00698ce0 _$E250
// FUNCTION: STBC 0x00698cf0 _$E274
// FUNCTION: STBC 0x00698d00 _$E273
// FUNCTION: STBC 0x00698d10 _$E278
// FUNCTION: STBC 0x00698d20 _$E277
// FUNCTION: STBC 0x00698d30 _$E318
// FUNCTION: STBC 0x00698d40 _$E317
// FUNCTION: STBC 0x00698d50 _$E324
// FUNCTION: STBC 0x00698d60 _$E323
// FUNCTION: STBC 0x00698d70 _$E266
// FUNCTION: STBC 0x00698d80 _$E265
// FUNCTION: STBC 0x00698d90 _$E270
// FUNCTION: STBC 0x00698da0 _$E269
// FUNCTION: STBC 0x00698db0 _$E282
// FUNCTION: STBC 0x00698dc0 _$E281
// FUNCTION: STBC 0x00698dd0 _$E286
// FUNCTION: STBC 0x00698de0 _$E285
// FUNCTION: STBC 0x00698df0 _$E290
// FUNCTION: STBC 0x00698e00 _$E289
// FUNCTION: STBC 0x00698e10 _$E294
// FUNCTION: STBC 0x00698e20 _$E293
// FUNCTION: STBC 0x00698e30 _$E298
// FUNCTION: STBC 0x00698e40 _$E297
// FUNCTION: STBC 0x00698e50 _$E302
// FUNCTION: STBC 0x00698e60 _$E301
// FUNCTION: STBC 0x00698e70 _$E306
// FUNCTION: STBC 0x00698e80 _$E305
// FUNCTION: STBC 0x00698e90 _$E310
// FUNCTION: STBC 0x00698ea0 _$E309
// FUNCTION: STBC 0x00698eb0 _$E116
// FUNCTION: STBC 0x00698ec0 _$E115
// FUNCTION: STBC 0x00698ed0 _$E314
// FUNCTION: STBC 0x00698ee0 _$E313
// FUNCTION: STBC 0x00698ef0 _$E121
// FUNCTION: STBC 0x00698f00 _$E120
// FUNCTION: STBC 0x00698f10 _$E328
// FUNCTION: STBC 0x00698f20 _$E327
// FUNCTION: STBC 0x00698f30 _$E332
// FUNCTION: STBC 0x00698f40 _$E331
// FUNCTION: STBC 0x00698f50 _$E336
// FUNCTION: STBC 0x00698f60 _$E335
// FUNCTION: STBC 0x00698f70 _$E340
// FUNCTION: STBC 0x00698f80 _$E339
// FUNCTION: STBC 0x00698f90 _$E130
// FUNCTION: STBC 0x00698fa0 _$E129
// FUNCTION: STBC 0x00698fb0 _$E344
// FUNCTION: STBC 0x00698fc0 _$E343
// FUNCTION: STBC 0x00698fd0 _$E348
// FUNCTION: STBC 0x00698fe0 _$E347
// FUNCTION: STBC 0x00698ff0 _$E352
// FUNCTION: STBC 0x00699000 _$E351
// FUNCTION: STBC 0x00699010 _$E136
// FUNCTION: STBC 0x00699020 _$E135
// FUNCTION: STBC 0x00699030 _$E141
// FUNCTION: STBC 0x00699040 _$E140
// FUNCTION: STBC 0x00699050 _$E171
// FUNCTION: STBC 0x00699060 _$E170
// FUNCTION: STBC 0x006990a0 _$E356
// FUNCTION: STBC 0x006990b0 _$E355
// FUNCTION: STBC 0x006990c0 _$E176
// FUNCTION: STBC 0x006990d0 _$E175
// FUNCTION: STBC 0x006990e0 _$E360
// FUNCTION: STBC 0x006990f0 _$E359
// FUNCTION: STBC 0x00699100 _$E364
// FUNCTION: STBC 0x00699110 _$E363
// FUNCTION: STBC 0x00699120 _$E181
// FUNCTION: STBC 0x00699130 _$E180
// FUNCTION: STBC 0x00699140 _$E368
// FUNCTION: STBC 0x00699150 _$E367
// FUNCTION: STBC 0x00699160 _$E146
// FUNCTION: STBC 0x00699170 _$E145
// FUNCTION: STBC 0x00699180 _$E372
// FUNCTION: STBC 0x00699190 _$E371
// FUNCTION: STBC 0x006991a0 _$E376
// FUNCTION: STBC 0x006991b0 _$E375
// FUNCTION: STBC 0x006991c0 _$E186
// FUNCTION: STBC 0x006991d0 _$E185
// FUNCTION: STBC 0x006991e0 _$E191
// FUNCTION: STBC 0x006991f0 _$E190
// FUNCTION: STBC 0x00699200 _$E380
// FUNCTION: STBC 0x00699210 _$E379
// FUNCTION: STBC 0x00699220 _$E384
// FUNCTION: STBC 0x00699230 _$E383
// FUNCTION: STBC 0x00699240 _$E388
// FUNCTION: STBC 0x00699250 _$E387
// FUNCTION: STBC 0x00699260 _$E392
// FUNCTION: STBC 0x00699270 _$E391
// FUNCTION: STBC 0x00699280 _$E396
// FUNCTION: STBC 0x00699290 _$E395
// FUNCTION: STBC 0x006992a0 _$E400
// FUNCTION: STBC 0x006992b0 _$E399
// FUNCTION: STBC 0x006992c0 _$E404
// FUNCTION: STBC 0x006992d0 _$E403
// FUNCTION: STBC 0x006992e0 _$E408
// FUNCTION: STBC 0x006992f0 _$E407
// FUNCTION: STBC 0x00699300 _$E196
// FUNCTION: STBC 0x00699310 _$E195
// FUNCTION: STBC 0x00699320 _$E412
// FUNCTION: STBC 0x00699330 _$E411
// FUNCTION: STBC 0x00699340 _$E201
// FUNCTION: STBC 0x00699350 _$E200
// FUNCTION: STBC 0x00699360 _$E206
// FUNCTION: STBC 0x00699370 _$E205
// FUNCTION: STBC 0x00699380 _$E416
// FUNCTION: STBC 0x00699390 _$E415
// FUNCTION: STBC 0x006993a0 _$E421
// FUNCTION: STBC 0x006993b0 _$E420
// FUNCTION: STBC 0x006993c0 _$E425
// FUNCTION: STBC 0x006993d0 _$E424

/*
================
WarpEngineProperty::Cast

The property as a WarpEngineProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006993e0 ?Cast@WarpEngineProperty@@SAPAV1@PAVTGModelProperty@@@Z
WarpEngineProperty *WarpEngineProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( WARPENGINEPROPERTY_TYPE ) ) {
		return (WarpEngineProperty *)property;
	}
	return NULL;
}

/*
================
WarpEngineProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00699410
WarpEngineProperty *WarpEngineProperty::Create( char *name ) {
	WarpEngineProperty *property = new WarpEngineProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
WarpEngineProperty::WarpEngineProperty
================
*/
// FUNCTION: STBC 0x00699460
WarpEngineProperty::WarpEngineProperty( long id ) : PoweredSubsystemProperty( id ) {
}

/*
================
WarpEngineProperty::WriteScript

Writes "<variable> = App.WarpEngineProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00699500
void WarpEngineProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.WarpEngineProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
WarpEngineProperty::~WarpEngineProperty
================
*/
// FUNCTION: STBC 0x00699630
WarpEngineProperty::~WarpEngineProperty( void ) {
}

/*
================
WarpEngineProperty::Copy
================
*/
// FUNCTION: STBC 0x00699640
bool WarpEngineProperty::Copy( TGModelProperty *property ) {
	WarpEngineProperty *warpEngine = Cast( property );
	return PoweredSubsystemProperty::Copy( property );
}

/*
================
WarpEngineProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00699660
void WarpEngineProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	PoweredSubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
WarpEngineProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00699680
void WarpEngineProperty::WriteInit( TGStream *stream, char *variableName ) {
	PoweredSubsystemProperty::WriteInit( stream, variableName );
}
