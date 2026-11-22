/*
===============================================================================

	EffectEmitterProperty.cpp

===============================================================================
*/

#include "EffectEmitterProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067c030 _$E433
// FUNCTION: STBC 0x0067c050 _$E432

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067bb90 _$E419
// FUNCTION: STBC 0x0067bba0 _$E416
// FUNCTION: STBC 0x0067bbb0 _$E418
// FUNCTION: STBC 0x0067bbc0 _$E417
// FUNCTION: STBC 0x0067bbd0 _$E425
// FUNCTION: STBC 0x0067bbe0 _$E422
// FUNCTION: STBC 0x0067bbf0 _$E424
// FUNCTION: STBC 0x0067bc00 _$E423

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067bc10 _$E51
// FUNCTION: STBC 0x0067bc20 _$E50
// FUNCTION: STBC 0x0067bc30 _$E55
// FUNCTION: STBC 0x0067bc40 _$E54
// FUNCTION: STBC 0x0067bc50 _$E66
// FUNCTION: STBC 0x0067bc60 _$E65
// FUNCTION: STBC 0x0067bc70 _$E70
// FUNCTION: STBC 0x0067bc80 _$E69
// FUNCTION: STBC 0x0067bc90 _$E74
// FUNCTION: STBC 0x0067bca0 _$E73
// FUNCTION: STBC 0x0067bcb0 _$E78
// FUNCTION: STBC 0x0067bcc0 _$E77
// FUNCTION: STBC 0x0067bcd0 _$E197
// FUNCTION: STBC 0x0067bce0 _$E196
// FUNCTION: STBC 0x0067bcf0 _$E139
// FUNCTION: STBC 0x0067bd00 _$E138
// FUNCTION: STBC 0x0067bd10 _$E152
// FUNCTION: STBC 0x0067bd20 _$E151
// FUNCTION: STBC 0x0067bd30 _$E202
// FUNCTION: STBC 0x0067bd40 _$E201
// FUNCTION: STBC 0x0067bd50 _$E207
// FUNCTION: STBC 0x0067bd60 _$E206
// FUNCTION: STBC 0x0067bd70 _$E211
// FUNCTION: STBC 0x0067bd80 _$E210
// FUNCTION: STBC 0x0067bd90 _$E215
// FUNCTION: STBC 0x0067bda0 _$E214
// FUNCTION: STBC 0x0067bdb0 _$E222
// FUNCTION: STBC 0x0067bdc0 _$E221
// FUNCTION: STBC 0x0067bdd0 _$E247
// FUNCTION: STBC 0x0067bde0 _$E246
// FUNCTION: STBC 0x0067bdf0 _$E251
// FUNCTION: STBC 0x0067be00 _$E250
// FUNCTION: STBC 0x0067be10 _$E97
// FUNCTION: STBC 0x0067be20 _$E96
// FUNCTION: STBC 0x0067be30 _$E233
// FUNCTION: STBC 0x0067be40 _$E232
// FUNCTION: STBC 0x0067be50 _$E240
// FUNCTION: STBC 0x0067be60 _$E239
// FUNCTION: STBC 0x0067be70 _$E263
// FUNCTION: STBC 0x0067be80 _$E262
// FUNCTION: STBC 0x0067be90 _$E267
// FUNCTION: STBC 0x0067bea0 _$E266
// FUNCTION: STBC 0x0067beb0 _$E307
// FUNCTION: STBC 0x0067bec0 _$E306
// FUNCTION: STBC 0x0067bed0 _$E313
// FUNCTION: STBC 0x0067bee0 _$E312
// FUNCTION: STBC 0x0067bef0 _$E255
// FUNCTION: STBC 0x0067bf00 _$E254
// FUNCTION: STBC 0x0067bf10 _$E259
// FUNCTION: STBC 0x0067bf20 _$E258
// FUNCTION: STBC 0x0067bf30 _$E271
// FUNCTION: STBC 0x0067bf40 _$E270
// FUNCTION: STBC 0x0067bf50 _$E275
// FUNCTION: STBC 0x0067bf60 _$E274
// FUNCTION: STBC 0x0067bf70 _$E279
// FUNCTION: STBC 0x0067bf80 _$E278
// FUNCTION: STBC 0x0067bf90 _$E283
// FUNCTION: STBC 0x0067bfa0 _$E282
// FUNCTION: STBC 0x0067bfb0 _$E287
// FUNCTION: STBC 0x0067bfc0 _$E286
// FUNCTION: STBC 0x0067bfd0 _$E291
// FUNCTION: STBC 0x0067bfe0 _$E290
// FUNCTION: STBC 0x0067bff0 _$E295
// FUNCTION: STBC 0x0067c000 _$E294
// FUNCTION: STBC 0x0067c010 _$E299
// FUNCTION: STBC 0x0067c020 _$E298
// FUNCTION: STBC 0x0067c060 _$E102
// FUNCTION: STBC 0x0067c070 _$E101
// FUNCTION: STBC 0x0067c080 _$E303
// FUNCTION: STBC 0x0067c090 _$E302
// FUNCTION: STBC 0x0067c0a0 _$E107
// FUNCTION: STBC 0x0067c0b0 _$E106
// FUNCTION: STBC 0x0067c0c0 _$E317
// FUNCTION: STBC 0x0067c0d0 _$E316
// FUNCTION: STBC 0x0067c0e0 _$E321
// FUNCTION: STBC 0x0067c0f0 _$E320
// FUNCTION: STBC 0x0067c100 _$E325
// FUNCTION: STBC 0x0067c110 _$E324
// FUNCTION: STBC 0x0067c120 _$E329
// FUNCTION: STBC 0x0067c130 _$E328
// FUNCTION: STBC 0x0067c140 _$E116
// FUNCTION: STBC 0x0067c150 _$E115
// FUNCTION: STBC 0x0067c160 _$E333
// FUNCTION: STBC 0x0067c170 _$E332
// FUNCTION: STBC 0x0067c180 _$E337
// FUNCTION: STBC 0x0067c190 _$E336
// FUNCTION: STBC 0x0067c1a0 _$E341
// FUNCTION: STBC 0x0067c1b0 _$E340
// FUNCTION: STBC 0x0067c1c0 _$E122
// FUNCTION: STBC 0x0067c1d0 _$E121
// FUNCTION: STBC 0x0067c1e0 _$E127
// FUNCTION: STBC 0x0067c1f0 _$E126
// FUNCTION: STBC 0x0067c200 _$E157
// FUNCTION: STBC 0x0067c210 _$E156
// FUNCTION: STBC 0x0067c220 _$E345
// FUNCTION: STBC 0x0067c230 _$E344
// FUNCTION: STBC 0x0067c240 _$E162
// FUNCTION: STBC 0x0067c250 _$E161
// FUNCTION: STBC 0x0067c260 _$E349
// FUNCTION: STBC 0x0067c270 _$E348
// FUNCTION: STBC 0x0067c280 _$E353
// FUNCTION: STBC 0x0067c290 _$E352
// FUNCTION: STBC 0x0067c2a0 _$E167
// FUNCTION: STBC 0x0067c2b0 _$E166
// FUNCTION: STBC 0x0067c2c0 _$E357
// FUNCTION: STBC 0x0067c2d0 _$E356
// FUNCTION: STBC 0x0067c2e0 _$E132
// FUNCTION: STBC 0x0067c2f0 _$E131
// FUNCTION: STBC 0x0067c300 _$E361
// FUNCTION: STBC 0x0067c310 _$E360
// FUNCTION: STBC 0x0067c320 _$E365
// FUNCTION: STBC 0x0067c330 _$E364
// FUNCTION: STBC 0x0067c340 _$E172
// FUNCTION: STBC 0x0067c350 _$E171
// FUNCTION: STBC 0x0067c360 _$E177
// FUNCTION: STBC 0x0067c370 _$E176
// FUNCTION: STBC 0x0067c380 _$E369
// FUNCTION: STBC 0x0067c390 _$E368
// FUNCTION: STBC 0x0067c3a0 _$E373
// FUNCTION: STBC 0x0067c3b0 _$E372
// FUNCTION: STBC 0x0067c3c0 _$E377
// FUNCTION: STBC 0x0067c3d0 _$E376
// FUNCTION: STBC 0x0067c3e0 _$E381
// FUNCTION: STBC 0x0067c3f0 _$E380
// FUNCTION: STBC 0x0067c400 _$E385
// FUNCTION: STBC 0x0067c410 _$E384
// FUNCTION: STBC 0x0067c420 _$E389
// FUNCTION: STBC 0x0067c430 _$E388
// FUNCTION: STBC 0x0067c440 _$E393
// FUNCTION: STBC 0x0067c450 _$E392
// FUNCTION: STBC 0x0067c460 _$E397
// FUNCTION: STBC 0x0067c470 _$E396
// FUNCTION: STBC 0x0067c480 _$E182
// FUNCTION: STBC 0x0067c490 _$E181
// FUNCTION: STBC 0x0067c4a0 _$E401
// FUNCTION: STBC 0x0067c4b0 _$E400
// FUNCTION: STBC 0x0067c4c0 _$E187
// FUNCTION: STBC 0x0067c4d0 _$E186
// FUNCTION: STBC 0x0067c4e0 _$E192
// FUNCTION: STBC 0x0067c4f0 _$E191
// FUNCTION: STBC 0x0067c500 _$E405
// FUNCTION: STBC 0x0067c510 _$E404
// FUNCTION: STBC 0x0067c520 _$E410
// FUNCTION: STBC 0x0067c530 _$E409
// FUNCTION: STBC 0x0067c540 _$E414
// FUNCTION: STBC 0x0067c550 _$E413

/*
================
EffectEmitterProperty::Cast

The property as a EffectEmitterProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067c580 ?Cast@EffectEmitterProperty@@SAPAV1@PAVTGModelProperty@@@Z
EffectEmitterProperty *EffectEmitterProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( EFFECTEMITTERPROPERTY_TYPE ) ) {
		return (EffectEmitterProperty *)property;
	}
	return NULL;
}

/*
================
EffectEmitterProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067c5b0
EffectEmitterProperty *EffectEmitterProperty::Create( char *name ) {
	EffectEmitterProperty *property = new EffectEmitterProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
EffectEmitterProperty::EffectEmitterProperty
================
*/
// FUNCTION: STBC 0x0067c600
EffectEmitterProperty::EffectEmitterProperty( long id ) : PositionOrientationProperty( id ) {
}

/*
================
EffectEmitterProperty::WriteScript

Writes "<variable> = App.EffectEmitterProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067c6a0
void EffectEmitterProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.EffectEmitterProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
