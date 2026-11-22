/*
===============================================================================

	HullProperty.cpp

===============================================================================
*/

#include <iostream>

#include "HullProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00682560 _$E32
// FUNCTION: STBC 0x00682570 _$E29
// FUNCTION: STBC 0x00682580 _$E31
// FUNCTION: STBC 0x00682590 _$E30
// FUNCTION: STBC 0x006825a0 _$E38
// FUNCTION: STBC 0x006825b0 _$E35
// FUNCTION: STBC 0x006825c0 _$E37
// FUNCTION: STBC 0x006825d0 _$E36
// FUNCTION: STBC 0x006829c0 _$E433
// FUNCTION: STBC 0x006829e0 _$E432

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006825e0 _$E65
// FUNCTION: STBC 0x006825f0 _$E64
// FUNCTION: STBC 0x00682600 _$E69
// FUNCTION: STBC 0x00682610 _$E68
// FUNCTION: STBC 0x00682620 _$E80
// FUNCTION: STBC 0x00682630 _$E79
// FUNCTION: STBC 0x00682640 _$E84
// FUNCTION: STBC 0x00682650 _$E83
// FUNCTION: STBC 0x00682660 _$E88
// FUNCTION: STBC 0x00682670 _$E87
// FUNCTION: STBC 0x00682680 _$E92
// FUNCTION: STBC 0x00682690 _$E91
// FUNCTION: STBC 0x006826a0 _$E211
// FUNCTION: STBC 0x006826b0 _$E210
// FUNCTION: STBC 0x006826c0 _$E153
// FUNCTION: STBC 0x006826d0 _$E152
// FUNCTION: STBC 0x006826e0 _$E166
// FUNCTION: STBC 0x006826f0 _$E165
// FUNCTION: STBC 0x00682700 _$E216
// FUNCTION: STBC 0x00682710 _$E215
// FUNCTION: STBC 0x00682720 _$E221
// FUNCTION: STBC 0x00682730 _$E220
// FUNCTION: STBC 0x00682740 _$E225
// FUNCTION: STBC 0x00682750 _$E224
// FUNCTION: STBC 0x00682760 _$E229
// FUNCTION: STBC 0x00682770 _$E228
// FUNCTION: STBC 0x00682780 _$E236
// FUNCTION: STBC 0x00682790 _$E235
// FUNCTION: STBC 0x006827a0 _$E258
// FUNCTION: STBC 0x006827b0 _$E257
// FUNCTION: STBC 0x006827c0 _$E262
// FUNCTION: STBC 0x006827d0 _$E261
// FUNCTION: STBC 0x006827e0 _$E111
// FUNCTION: STBC 0x006827f0 _$E110
// FUNCTION: STBC 0x00682800 _$E244
// FUNCTION: STBC 0x00682810 _$E243
// FUNCTION: STBC 0x00682820 _$E251
// FUNCTION: STBC 0x00682830 _$E250
// FUNCTION: STBC 0x00682840 _$E274
// FUNCTION: STBC 0x00682850 _$E273
// FUNCTION: STBC 0x00682860 _$E278
// FUNCTION: STBC 0x00682870 _$E277
// FUNCTION: STBC 0x00682880 _$E318
// FUNCTION: STBC 0x00682890 _$E317
// FUNCTION: STBC 0x006828a0 _$E324
// FUNCTION: STBC 0x006828b0 _$E323
// FUNCTION: STBC 0x006828c0 _$E266
// FUNCTION: STBC 0x006828d0 _$E265
// FUNCTION: STBC 0x006828e0 _$E270
// FUNCTION: STBC 0x006828f0 _$E269
// FUNCTION: STBC 0x00682900 _$E282
// FUNCTION: STBC 0x00682910 _$E281
// FUNCTION: STBC 0x00682920 _$E286
// FUNCTION: STBC 0x00682930 _$E285
// FUNCTION: STBC 0x00682940 _$E290
// FUNCTION: STBC 0x00682950 _$E289
// FUNCTION: STBC 0x00682960 _$E294
// FUNCTION: STBC 0x00682970 _$E293
// FUNCTION: STBC 0x00682980 _$E298
// FUNCTION: STBC 0x00682990 _$E297
// FUNCTION: STBC 0x006829a0 _$E302
// FUNCTION: STBC 0x006829b0 _$E301
// FUNCTION: STBC 0x006829f0 _$E306
// FUNCTION: STBC 0x00682a00 _$E305
// FUNCTION: STBC 0x00682a10 _$E310
// FUNCTION: STBC 0x00682a20 _$E309
// FUNCTION: STBC 0x00682a30 _$E116
// FUNCTION: STBC 0x00682a40 _$E115
// FUNCTION: STBC 0x00682a50 _$E314
// FUNCTION: STBC 0x00682a60 _$E313
// FUNCTION: STBC 0x00682a70 _$E121
// FUNCTION: STBC 0x00682a80 _$E120
// FUNCTION: STBC 0x00682a90 _$E328
// FUNCTION: STBC 0x00682aa0 _$E327
// FUNCTION: STBC 0x00682ab0 _$E332
// FUNCTION: STBC 0x00682ac0 _$E331
// FUNCTION: STBC 0x00682ad0 _$E336
// FUNCTION: STBC 0x00682ae0 _$E335
// FUNCTION: STBC 0x00682af0 _$E340
// FUNCTION: STBC 0x00682b00 _$E339
// FUNCTION: STBC 0x00682b10 _$E130
// FUNCTION: STBC 0x00682b20 _$E129
// FUNCTION: STBC 0x00682b30 _$E344
// FUNCTION: STBC 0x00682b40 _$E343
// FUNCTION: STBC 0x00682b50 _$E348
// FUNCTION: STBC 0x00682b60 _$E347
// FUNCTION: STBC 0x00682b70 _$E352
// FUNCTION: STBC 0x00682b80 _$E351
// FUNCTION: STBC 0x00682b90 _$E136
// FUNCTION: STBC 0x00682ba0 _$E135
// FUNCTION: STBC 0x00682bb0 _$E141
// FUNCTION: STBC 0x00682bc0 _$E140
// FUNCTION: STBC 0x00682bd0 _$E171
// FUNCTION: STBC 0x00682be0 _$E170
// FUNCTION: STBC 0x00682bf0 _$E356
// FUNCTION: STBC 0x00682c00 _$E355
// FUNCTION: STBC 0x00682c10 _$E176
// FUNCTION: STBC 0x00682c20 _$E175
// FUNCTION: STBC 0x00682c30 _$E360
// FUNCTION: STBC 0x00682c40 _$E359
// FUNCTION: STBC 0x00682c50 _$E364
// FUNCTION: STBC 0x00682c60 _$E363
// FUNCTION: STBC 0x00682c70 _$E181
// FUNCTION: STBC 0x00682c80 _$E180
// FUNCTION: STBC 0x00682c90 _$E368
// FUNCTION: STBC 0x00682ca0 _$E367
// FUNCTION: STBC 0x00682cb0 _$E146
// FUNCTION: STBC 0x00682cc0 _$E145
// FUNCTION: STBC 0x00682cd0 _$E372
// FUNCTION: STBC 0x00682ce0 _$E371
// FUNCTION: STBC 0x00682cf0 _$E376
// FUNCTION: STBC 0x00682d00 _$E375
// FUNCTION: STBC 0x00682d10 _$E186
// FUNCTION: STBC 0x00682d20 _$E185
// FUNCTION: STBC 0x00682d30 _$E191
// FUNCTION: STBC 0x00682d40 _$E190
// FUNCTION: STBC 0x00682d50 _$E380
// FUNCTION: STBC 0x00682d60 _$E379
// FUNCTION: STBC 0x00682d70 _$E384
// FUNCTION: STBC 0x00682d80 _$E383
// FUNCTION: STBC 0x00682d90 _$E388
// FUNCTION: STBC 0x00682da0 _$E387
// FUNCTION: STBC 0x00682db0 _$E392
// FUNCTION: STBC 0x00682dc0 _$E391
// FUNCTION: STBC 0x00682dd0 _$E396
// FUNCTION: STBC 0x00682de0 _$E395
// FUNCTION: STBC 0x00682df0 _$E400
// FUNCTION: STBC 0x00682e00 _$E399
// FUNCTION: STBC 0x00682e10 _$E404
// FUNCTION: STBC 0x00682e20 _$E403
// FUNCTION: STBC 0x00682e30 _$E408
// FUNCTION: STBC 0x00682e40 _$E407
// FUNCTION: STBC 0x00682e50 _$E196
// FUNCTION: STBC 0x00682e60 _$E195
// FUNCTION: STBC 0x00682e70 _$E412
// FUNCTION: STBC 0x00682e80 _$E411
// FUNCTION: STBC 0x00682e90 _$E201
// FUNCTION: STBC 0x00682ea0 _$E200
// FUNCTION: STBC 0x00682eb0 _$E206
// FUNCTION: STBC 0x00682ec0 _$E205
// FUNCTION: STBC 0x00682ed0 _$E416
// FUNCTION: STBC 0x00682ee0 _$E415
// FUNCTION: STBC 0x00682ef0 _$E421
// FUNCTION: STBC 0x00682f00 _$E420
// FUNCTION: STBC 0x00682f10 _$E425
// FUNCTION: STBC 0x00682f20 _$E424

/*
================
HullProperty::Cast

The property as a HullProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00682f30 ?Cast@HullProperty@@SAPAV1@PAVTGModelProperty@@@Z
HullProperty *HullProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( HULLPROPERTY_TYPE ) ) {
		return (HullProperty *)property;
	}
	return NULL;
}

/*
================
HullProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00682f60
HullProperty *HullProperty::Create( char *name ) {
	HullProperty *property = new HullProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
HullProperty::HullProperty
================
*/
// FUNCTION: STBC 0x00682fb0
HullProperty::HullProperty( long id ) : SubsystemProperty( id ) {
}

/*
================
HullProperty::WriteScript

Writes "<variable> = App.HullProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00683050
void HullProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.HullProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
HullProperty::~HullProperty
================
*/
// FUNCTION: STBC 0x00683180
HullProperty::~HullProperty( void ) {
}

/*
================
HullProperty::Copy
================
*/
// FUNCTION: STBC 0x00683190
bool HullProperty::Copy( TGModelProperty *property ) {
	HullProperty *hull = Cast( property );
	return SubsystemProperty::Copy( property );
}

/*
================
HullProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x006831b0
void HullProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	SubsystemProperty::VirtualSlot15( show, viewer, parent );
}

/*
================
HullProperty::WriteInit
================
*/
// FUNCTION: STBC 0x006831d0
void HullProperty::WriteInit( TGStream *stream, char *variableName ) {
	SubsystemProperty::WriteInit( stream, variableName );
}
