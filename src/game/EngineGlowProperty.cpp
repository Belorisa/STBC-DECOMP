/*
===============================================================================

	EngineGlowProperty.cpp

===============================================================================
*/

#include "EngineGlowProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067cbd0 _$E429
// FUNCTION: STBC 0x0067cbf0 _$E428

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067c7d0 _$E415
// FUNCTION: STBC 0x0067c7e0 _$E412
// FUNCTION: STBC 0x0067c7f0 _$E414
// FUNCTION: STBC 0x0067c800 _$E413
// FUNCTION: STBC 0x0067c810 _$E421
// FUNCTION: STBC 0x0067c820 _$E418
// FUNCTION: STBC 0x0067c830 _$E420
// FUNCTION: STBC 0x0067c840 _$E419

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067c850 _$E42
// FUNCTION: STBC 0x0067c860 _$E41
// FUNCTION: STBC 0x0067c870 _$E46
// FUNCTION: STBC 0x0067c880 _$E45
// FUNCTION: STBC 0x0067c890 _$E57
// FUNCTION: STBC 0x0067c8a0 _$E56
// FUNCTION: STBC 0x0067c8b0 _$E61
// FUNCTION: STBC 0x0067c8c0 _$E60
// FUNCTION: STBC 0x0067c8d0 _$E65
// FUNCTION: STBC 0x0067c8e0 _$E64
// FUNCTION: STBC 0x0067c8f0 _$E69
// FUNCTION: STBC 0x0067c900 _$E68
// FUNCTION: STBC 0x0067c910 _$E193
// FUNCTION: STBC 0x0067c920 _$E192
// FUNCTION: STBC 0x0067c930 _$E135
// FUNCTION: STBC 0x0067c940 _$E134
// FUNCTION: STBC 0x0067c950 _$E148
// FUNCTION: STBC 0x0067c960 _$E147
// FUNCTION: STBC 0x0067c970 _$E198
// FUNCTION: STBC 0x0067c980 _$E197
// FUNCTION: STBC 0x0067c990 _$E203
// FUNCTION: STBC 0x0067c9a0 _$E202
// FUNCTION: STBC 0x0067c9b0 _$E207
// FUNCTION: STBC 0x0067c9c0 _$E206
// FUNCTION: STBC 0x0067c9d0 _$E211
// FUNCTION: STBC 0x0067c9e0 _$E210
// FUNCTION: STBC 0x0067c9f0 _$E218
// FUNCTION: STBC 0x0067ca00 _$E217
// FUNCTION: STBC 0x0067ca10 _$E243
// FUNCTION: STBC 0x0067ca20 _$E242
// FUNCTION: STBC 0x0067ca30 _$E247
// FUNCTION: STBC 0x0067ca40 _$E246
// FUNCTION: STBC 0x0067ca50 _$E93
// FUNCTION: STBC 0x0067ca60 _$E92
// FUNCTION: STBC 0x0067ca70 _$E229
// FUNCTION: STBC 0x0067ca80 _$E228
// FUNCTION: STBC 0x0067ca90 _$E236
// FUNCTION: STBC 0x0067caa0 _$E235
// FUNCTION: STBC 0x0067cab0 _$E259
// FUNCTION: STBC 0x0067cac0 _$E258
// FUNCTION: STBC 0x0067cad0 _$E263
// FUNCTION: STBC 0x0067cae0 _$E262
// FUNCTION: STBC 0x0067caf0 _$E303
// FUNCTION: STBC 0x0067cb00 _$E302
// FUNCTION: STBC 0x0067cb10 _$E309
// FUNCTION: STBC 0x0067cb20 _$E308
// FUNCTION: STBC 0x0067cb30 _$E251
// FUNCTION: STBC 0x0067cb40 _$E250
// FUNCTION: STBC 0x0067cb50 _$E255
// FUNCTION: STBC 0x0067cb60 _$E254
// FUNCTION: STBC 0x0067cb70 _$E267
// FUNCTION: STBC 0x0067cb80 _$E266
// FUNCTION: STBC 0x0067cb90 _$E271
// FUNCTION: STBC 0x0067cba0 _$E270
// FUNCTION: STBC 0x0067cbb0 _$E275
// FUNCTION: STBC 0x0067cbc0 _$E274
// FUNCTION: STBC 0x0067cc00 _$E279
// FUNCTION: STBC 0x0067cc10 _$E278
// FUNCTION: STBC 0x0067cc20 _$E283
// FUNCTION: STBC 0x0067cc30 _$E282
// FUNCTION: STBC 0x0067cc40 _$E287
// FUNCTION: STBC 0x0067cc50 _$E286
// FUNCTION: STBC 0x0067cc60 _$E291
// FUNCTION: STBC 0x0067cc70 _$E290
// FUNCTION: STBC 0x0067cc80 _$E295
// FUNCTION: STBC 0x0067cc90 _$E294
// FUNCTION: STBC 0x0067cca0 _$E98
// FUNCTION: STBC 0x0067ccb0 _$E97
// FUNCTION: STBC 0x0067ccc0 _$E299
// FUNCTION: STBC 0x0067ccd0 _$E298
// FUNCTION: STBC 0x0067cce0 _$E103
// FUNCTION: STBC 0x0067ccf0 _$E102
// FUNCTION: STBC 0x0067cd00 _$E313
// FUNCTION: STBC 0x0067cd10 _$E312
// FUNCTION: STBC 0x0067cd20 _$E317
// FUNCTION: STBC 0x0067cd30 _$E316
// FUNCTION: STBC 0x0067cd40 _$E321
// FUNCTION: STBC 0x0067cd50 _$E320
// FUNCTION: STBC 0x0067cd60 _$E325
// FUNCTION: STBC 0x0067cd70 _$E324
// FUNCTION: STBC 0x0067cd80 _$E112
// FUNCTION: STBC 0x0067cd90 _$E111
// FUNCTION: STBC 0x0067cda0 _$E329
// FUNCTION: STBC 0x0067cdb0 _$E328
// FUNCTION: STBC 0x0067cdc0 _$E333
// FUNCTION: STBC 0x0067cdd0 _$E332
// FUNCTION: STBC 0x0067cde0 _$E337
// FUNCTION: STBC 0x0067cdf0 _$E336
// FUNCTION: STBC 0x0067ce00 _$E118
// FUNCTION: STBC 0x0067ce10 _$E117
// FUNCTION: STBC 0x0067ce20 _$E123
// FUNCTION: STBC 0x0067ce30 _$E122
// FUNCTION: STBC 0x0067ce40 _$E153
// FUNCTION: STBC 0x0067ce50 _$E152
// FUNCTION: STBC 0x0067ce60 _$E341
// FUNCTION: STBC 0x0067ce70 _$E340
// FUNCTION: STBC 0x0067ce80 _$E158
// FUNCTION: STBC 0x0067ce90 _$E157
// FUNCTION: STBC 0x0067cea0 _$E345
// FUNCTION: STBC 0x0067ceb0 _$E344
// FUNCTION: STBC 0x0067cec0 _$E349
// FUNCTION: STBC 0x0067ced0 _$E348
// FUNCTION: STBC 0x0067cee0 _$E163
// FUNCTION: STBC 0x0067cef0 _$E162
// FUNCTION: STBC 0x0067cf00 _$E353
// FUNCTION: STBC 0x0067cf10 _$E352
// FUNCTION: STBC 0x0067cf20 _$E128
// FUNCTION: STBC 0x0067cf30 _$E127
// FUNCTION: STBC 0x0067cf40 _$E357
// FUNCTION: STBC 0x0067cf50 _$E356
// FUNCTION: STBC 0x0067cf60 _$E361
// FUNCTION: STBC 0x0067cf70 _$E360
// FUNCTION: STBC 0x0067cf80 _$E168
// FUNCTION: STBC 0x0067cf90 _$E167
// FUNCTION: STBC 0x0067cfa0 _$E173
// FUNCTION: STBC 0x0067cfb0 _$E172
// FUNCTION: STBC 0x0067cfc0 _$E365
// FUNCTION: STBC 0x0067cfd0 _$E364
// FUNCTION: STBC 0x0067cfe0 _$E369
// FUNCTION: STBC 0x0067cff0 _$E368
// FUNCTION: STBC 0x0067d000 _$E373
// FUNCTION: STBC 0x0067d010 _$E372
// FUNCTION: STBC 0x0067d020 _$E377
// FUNCTION: STBC 0x0067d030 _$E376
// FUNCTION: STBC 0x0067d040 _$E381
// FUNCTION: STBC 0x0067d050 _$E380
// FUNCTION: STBC 0x0067d060 _$E385
// FUNCTION: STBC 0x0067d070 _$E384
// FUNCTION: STBC 0x0067d080 _$E389
// FUNCTION: STBC 0x0067d090 _$E388
// FUNCTION: STBC 0x0067d0a0 _$E393
// FUNCTION: STBC 0x0067d0b0 _$E392
// FUNCTION: STBC 0x0067d0c0 _$E178
// FUNCTION: STBC 0x0067d0d0 _$E177
// FUNCTION: STBC 0x0067d0e0 _$E397
// FUNCTION: STBC 0x0067d0f0 _$E396
// FUNCTION: STBC 0x0067d100 _$E183
// FUNCTION: STBC 0x0067d110 _$E182
// FUNCTION: STBC 0x0067d120 _$E188
// FUNCTION: STBC 0x0067d130 _$E187
// FUNCTION: STBC 0x0067d140 _$E401
// FUNCTION: STBC 0x0067d150 _$E400
// FUNCTION: STBC 0x0067d160 _$E406
// FUNCTION: STBC 0x0067d170 _$E405
// FUNCTION: STBC 0x0067d180 _$E410
// FUNCTION: STBC 0x0067d190 _$E409

/*
================
EngineGlowProperty::Cast

The property as a EngineGlowProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067d1a0 ?Cast@EngineGlowProperty@@SAPAV1@PAVTGModelProperty@@@Z
EngineGlowProperty *EngineGlowProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( ENGINEGLOWPROPERTY_TYPE ) ) {
		return (EngineGlowProperty *)property;
	}
	return NULL;
}

/*
================
EngineGlowProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067d1d0
EngineGlowProperty *EngineGlowProperty::Create( char *name ) {
	EngineGlowProperty *property = new EngineGlowProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
EngineGlowProperty::EngineGlowProperty
================
*/
// FUNCTION: STBC 0x0067d220
EngineGlowProperty::EngineGlowProperty( long id ) : TGModelProperty( id ) {
}

/*
================
EngineGlowProperty::WriteScript

Writes "<variable> = App.EngineGlowProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067d2b0
void EngineGlowProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.EngineGlowProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
