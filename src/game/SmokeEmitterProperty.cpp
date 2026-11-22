/*
===============================================================================

	SmokeEmitterProperty.cpp

===============================================================================
*/

#include "SmokeEmitterProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067e480 _$E433
// FUNCTION: STBC 0x0067e4a0 _$E432

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067e040 _$E419
// FUNCTION: STBC 0x0067e050 _$E416
// FUNCTION: STBC 0x0067e060 _$E418
// FUNCTION: STBC 0x0067e070 _$E417
// FUNCTION: STBC 0x0067e080 _$E425
// FUNCTION: STBC 0x0067e090 _$E422
// FUNCTION: STBC 0x0067e0a0 _$E424
// FUNCTION: STBC 0x0067e0b0 _$E423

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067e0c0 _$E51
// FUNCTION: STBC 0x0067e0d0 _$E50
// FUNCTION: STBC 0x0067e0e0 _$E55
// FUNCTION: STBC 0x0067e0f0 _$E54
// FUNCTION: STBC 0x0067e100 _$E66
// FUNCTION: STBC 0x0067e110 _$E65
// FUNCTION: STBC 0x0067e120 _$E70
// FUNCTION: STBC 0x0067e130 _$E69
// FUNCTION: STBC 0x0067e140 _$E74
// FUNCTION: STBC 0x0067e150 _$E73
// FUNCTION: STBC 0x0067e160 _$E78
// FUNCTION: STBC 0x0067e170 _$E77
// FUNCTION: STBC 0x0067e180 _$E197
// FUNCTION: STBC 0x0067e190 _$E196
// FUNCTION: STBC 0x0067e1a0 _$E139
// FUNCTION: STBC 0x0067e1b0 _$E138
// FUNCTION: STBC 0x0067e1c0 _$E152
// FUNCTION: STBC 0x0067e1d0 _$E151
// FUNCTION: STBC 0x0067e1e0 _$E202
// FUNCTION: STBC 0x0067e1f0 _$E201
// FUNCTION: STBC 0x0067e200 _$E207
// FUNCTION: STBC 0x0067e210 _$E206
// FUNCTION: STBC 0x0067e220 _$E211
// FUNCTION: STBC 0x0067e230 _$E210
// FUNCTION: STBC 0x0067e240 _$E215
// FUNCTION: STBC 0x0067e250 _$E214
// FUNCTION: STBC 0x0067e260 _$E222
// FUNCTION: STBC 0x0067e270 _$E221
// FUNCTION: STBC 0x0067e280 _$E247
// FUNCTION: STBC 0x0067e290 _$E246
// FUNCTION: STBC 0x0067e2a0 _$E251
// FUNCTION: STBC 0x0067e2b0 _$E250
// FUNCTION: STBC 0x0067e2c0 _$E97
// FUNCTION: STBC 0x0067e2d0 _$E96
// FUNCTION: STBC 0x0067e2e0 _$E233
// FUNCTION: STBC 0x0067e2f0 _$E232
// FUNCTION: STBC 0x0067e300 _$E240
// FUNCTION: STBC 0x0067e310 _$E239
// FUNCTION: STBC 0x0067e320 _$E263
// FUNCTION: STBC 0x0067e330 _$E262
// FUNCTION: STBC 0x0067e340 _$E267
// FUNCTION: STBC 0x0067e350 _$E266
// FUNCTION: STBC 0x0067e360 _$E307
// FUNCTION: STBC 0x0067e370 _$E306
// FUNCTION: STBC 0x0067e380 _$E313
// FUNCTION: STBC 0x0067e390 _$E312
// FUNCTION: STBC 0x0067e3a0 _$E255
// FUNCTION: STBC 0x0067e3b0 _$E254
// FUNCTION: STBC 0x0067e3c0 _$E259
// FUNCTION: STBC 0x0067e3d0 _$E258
// FUNCTION: STBC 0x0067e3e0 _$E271
// FUNCTION: STBC 0x0067e3f0 _$E270
// FUNCTION: STBC 0x0067e400 _$E275
// FUNCTION: STBC 0x0067e410 _$E274
// FUNCTION: STBC 0x0067e420 _$E279
// FUNCTION: STBC 0x0067e430 _$E278
// FUNCTION: STBC 0x0067e440 _$E283
// FUNCTION: STBC 0x0067e450 _$E282
// FUNCTION: STBC 0x0067e460 _$E287
// FUNCTION: STBC 0x0067e470 _$E286
// FUNCTION: STBC 0x0067e4b0 _$E291
// FUNCTION: STBC 0x0067e4c0 _$E290
// FUNCTION: STBC 0x0067e4d0 _$E295
// FUNCTION: STBC 0x0067e4e0 _$E294
// FUNCTION: STBC 0x0067e4f0 _$E299
// FUNCTION: STBC 0x0067e500 _$E298
// FUNCTION: STBC 0x0067e510 _$E102
// FUNCTION: STBC 0x0067e520 _$E101
// FUNCTION: STBC 0x0067e530 _$E303
// FUNCTION: STBC 0x0067e540 _$E302
// FUNCTION: STBC 0x0067e550 _$E107
// FUNCTION: STBC 0x0067e560 _$E106
// FUNCTION: STBC 0x0067e570 _$E317
// FUNCTION: STBC 0x0067e580 _$E316
// FUNCTION: STBC 0x0067e590 _$E321
// FUNCTION: STBC 0x0067e5a0 _$E320
// FUNCTION: STBC 0x0067e5b0 _$E325
// FUNCTION: STBC 0x0067e5c0 _$E324
// FUNCTION: STBC 0x0067e5d0 _$E329
// FUNCTION: STBC 0x0067e5e0 _$E328
// FUNCTION: STBC 0x0067e5f0 _$E116
// FUNCTION: STBC 0x0067e600 _$E115
// FUNCTION: STBC 0x0067e610 _$E333
// FUNCTION: STBC 0x0067e620 _$E332
// FUNCTION: STBC 0x0067e630 _$E337
// FUNCTION: STBC 0x0067e640 _$E336
// FUNCTION: STBC 0x0067e650 _$E341
// FUNCTION: STBC 0x0067e660 _$E340
// FUNCTION: STBC 0x0067e670 _$E122
// FUNCTION: STBC 0x0067e680 _$E121
// FUNCTION: STBC 0x0067e690 _$E127
// FUNCTION: STBC 0x0067e6a0 _$E126
// FUNCTION: STBC 0x0067e6b0 _$E157
// FUNCTION: STBC 0x0067e6c0 _$E156
// FUNCTION: STBC 0x0067e6d0 _$E345
// FUNCTION: STBC 0x0067e6e0 _$E344
// FUNCTION: STBC 0x0067e6f0 _$E162
// FUNCTION: STBC 0x0067e700 _$E161
// FUNCTION: STBC 0x0067e710 _$E349
// FUNCTION: STBC 0x0067e720 _$E348
// FUNCTION: STBC 0x0067e730 _$E353
// FUNCTION: STBC 0x0067e740 _$E352
// FUNCTION: STBC 0x0067e750 _$E167
// FUNCTION: STBC 0x0067e760 _$E166
// FUNCTION: STBC 0x0067e770 _$E357
// FUNCTION: STBC 0x0067e780 _$E356
// FUNCTION: STBC 0x0067e790 _$E132
// FUNCTION: STBC 0x0067e7a0 _$E131
// FUNCTION: STBC 0x0067e7b0 _$E361
// FUNCTION: STBC 0x0067e7c0 _$E360
// FUNCTION: STBC 0x0067e7d0 _$E365
// FUNCTION: STBC 0x0067e7e0 _$E364
// FUNCTION: STBC 0x0067e7f0 _$E172
// FUNCTION: STBC 0x0067e800 _$E171
// FUNCTION: STBC 0x0067e810 _$E177
// FUNCTION: STBC 0x0067e820 _$E176
// FUNCTION: STBC 0x0067e830 _$E369
// FUNCTION: STBC 0x0067e840 _$E368
// FUNCTION: STBC 0x0067e850 _$E373
// FUNCTION: STBC 0x0067e860 _$E372
// FUNCTION: STBC 0x0067e870 _$E377
// FUNCTION: STBC 0x0067e880 _$E376
// FUNCTION: STBC 0x0067e890 _$E381
// FUNCTION: STBC 0x0067e8a0 _$E380
// FUNCTION: STBC 0x0067e8b0 _$E385
// FUNCTION: STBC 0x0067e8c0 _$E384
// FUNCTION: STBC 0x0067e8d0 _$E389
// FUNCTION: STBC 0x0067e8e0 _$E388
// FUNCTION: STBC 0x0067e8f0 _$E393
// FUNCTION: STBC 0x0067e900 _$E392
// FUNCTION: STBC 0x0067e910 _$E397
// FUNCTION: STBC 0x0067e920 _$E396
// FUNCTION: STBC 0x0067e930 _$E182
// FUNCTION: STBC 0x0067e940 _$E181
// FUNCTION: STBC 0x0067e950 _$E401
// FUNCTION: STBC 0x0067e960 _$E400
// FUNCTION: STBC 0x0067e970 _$E187
// FUNCTION: STBC 0x0067e980 _$E186
// FUNCTION: STBC 0x0067e990 _$E192
// FUNCTION: STBC 0x0067e9a0 _$E191
// FUNCTION: STBC 0x0067e9b0 _$E405
// FUNCTION: STBC 0x0067e9c0 _$E404
// FUNCTION: STBC 0x0067e9d0 _$E410
// FUNCTION: STBC 0x0067e9e0 _$E409
// FUNCTION: STBC 0x0067e9f0 _$E414
// FUNCTION: STBC 0x0067ea00 _$E413

/*
================
SmokeEmitterProperty::Cast

The property as a SmokeEmitterProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067ea10 ?Cast@SmokeEmitterProperty@@SAPAV1@PAVTGModelProperty@@@Z
SmokeEmitterProperty *SmokeEmitterProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SMOKEEMITTERPROPERTY_TYPE ) ) {
		return (SmokeEmitterProperty *)property;
	}
	return NULL;
}

/*
================
SmokeEmitterProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067ea40
SmokeEmitterProperty *SmokeEmitterProperty::Create( char *name ) {
	SmokeEmitterProperty *property = new SmokeEmitterProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
SmokeEmitterProperty::SmokeEmitterProperty
================
*/
// FUNCTION: STBC 0x0067ea90
SmokeEmitterProperty::SmokeEmitterProperty( long id ) : EffectEmitterProperty( id ) {
}

/*
================
SmokeEmitterProperty::WriteScript

Writes "<variable> = App.SmokeEmitterProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067eb30
void SmokeEmitterProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.SmokeEmitterProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
