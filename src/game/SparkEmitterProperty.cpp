/*
===============================================================================

	SparkEmitterProperty.cpp

===============================================================================
*/

#include "SparkEmitterProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067f0b0 _$E433
// FUNCTION: STBC 0x0067f0d0 _$E432

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067ec70 _$E419
// FUNCTION: STBC 0x0067ec80 _$E416
// FUNCTION: STBC 0x0067ec90 _$E418
// FUNCTION: STBC 0x0067eca0 _$E417
// FUNCTION: STBC 0x0067ecb0 _$E425
// FUNCTION: STBC 0x0067ecc0 _$E422
// FUNCTION: STBC 0x0067ecd0 _$E424
// FUNCTION: STBC 0x0067ece0 _$E423

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067ecf0 _$E51
// FUNCTION: STBC 0x0067ed00 _$E50
// FUNCTION: STBC 0x0067ed10 _$E55
// FUNCTION: STBC 0x0067ed20 _$E54
// FUNCTION: STBC 0x0067ed30 _$E66
// FUNCTION: STBC 0x0067ed40 _$E65
// FUNCTION: STBC 0x0067ed50 _$E70
// FUNCTION: STBC 0x0067ed60 _$E69
// FUNCTION: STBC 0x0067ed70 _$E74
// FUNCTION: STBC 0x0067ed80 _$E73
// FUNCTION: STBC 0x0067ed90 _$E78
// FUNCTION: STBC 0x0067eda0 _$E77
// FUNCTION: STBC 0x0067edb0 _$E197
// FUNCTION: STBC 0x0067edc0 _$E196
// FUNCTION: STBC 0x0067edd0 _$E139
// FUNCTION: STBC 0x0067ede0 _$E138
// FUNCTION: STBC 0x0067edf0 _$E152
// FUNCTION: STBC 0x0067ee00 _$E151
// FUNCTION: STBC 0x0067ee10 _$E202
// FUNCTION: STBC 0x0067ee20 _$E201
// FUNCTION: STBC 0x0067ee30 _$E207
// FUNCTION: STBC 0x0067ee40 _$E206
// FUNCTION: STBC 0x0067ee50 _$E211
// FUNCTION: STBC 0x0067ee60 _$E210
// FUNCTION: STBC 0x0067ee70 _$E215
// FUNCTION: STBC 0x0067ee80 _$E214
// FUNCTION: STBC 0x0067ee90 _$E222
// FUNCTION: STBC 0x0067eea0 _$E221
// FUNCTION: STBC 0x0067eeb0 _$E247
// FUNCTION: STBC 0x0067eec0 _$E246
// FUNCTION: STBC 0x0067eed0 _$E251
// FUNCTION: STBC 0x0067eee0 _$E250
// FUNCTION: STBC 0x0067eef0 _$E97
// FUNCTION: STBC 0x0067ef00 _$E96
// FUNCTION: STBC 0x0067ef10 _$E233
// FUNCTION: STBC 0x0067ef20 _$E232
// FUNCTION: STBC 0x0067ef30 _$E240
// FUNCTION: STBC 0x0067ef40 _$E239
// FUNCTION: STBC 0x0067ef50 _$E263
// FUNCTION: STBC 0x0067ef60 _$E262
// FUNCTION: STBC 0x0067ef70 _$E267
// FUNCTION: STBC 0x0067ef80 _$E266
// FUNCTION: STBC 0x0067ef90 _$E307
// FUNCTION: STBC 0x0067efa0 _$E306
// FUNCTION: STBC 0x0067efb0 _$E313
// FUNCTION: STBC 0x0067efc0 _$E312
// FUNCTION: STBC 0x0067efd0 _$E255
// FUNCTION: STBC 0x0067efe0 _$E254
// FUNCTION: STBC 0x0067eff0 _$E259
// FUNCTION: STBC 0x0067f000 _$E258
// FUNCTION: STBC 0x0067f010 _$E271
// FUNCTION: STBC 0x0067f020 _$E270
// FUNCTION: STBC 0x0067f030 _$E275
// FUNCTION: STBC 0x0067f040 _$E274
// FUNCTION: STBC 0x0067f050 _$E279
// FUNCTION: STBC 0x0067f060 _$E278
// FUNCTION: STBC 0x0067f070 _$E283
// FUNCTION: STBC 0x0067f080 _$E282
// FUNCTION: STBC 0x0067f090 _$E287
// FUNCTION: STBC 0x0067f0a0 _$E286
// FUNCTION: STBC 0x0067f0e0 _$E291
// FUNCTION: STBC 0x0067f0f0 _$E290
// FUNCTION: STBC 0x0067f100 _$E295
// FUNCTION: STBC 0x0067f110 _$E294
// FUNCTION: STBC 0x0067f120 _$E299
// FUNCTION: STBC 0x0067f130 _$E298
// FUNCTION: STBC 0x0067f140 _$E102
// FUNCTION: STBC 0x0067f150 _$E101
// FUNCTION: STBC 0x0067f160 _$E303
// FUNCTION: STBC 0x0067f170 _$E302
// FUNCTION: STBC 0x0067f180 _$E107
// FUNCTION: STBC 0x0067f190 _$E106
// FUNCTION: STBC 0x0067f1a0 _$E317
// FUNCTION: STBC 0x0067f1b0 _$E316
// FUNCTION: STBC 0x0067f1c0 _$E321
// FUNCTION: STBC 0x0067f1d0 _$E320
// FUNCTION: STBC 0x0067f1e0 _$E325
// FUNCTION: STBC 0x0067f1f0 _$E324
// FUNCTION: STBC 0x0067f200 _$E329
// FUNCTION: STBC 0x0067f210 _$E328
// FUNCTION: STBC 0x0067f220 _$E116
// FUNCTION: STBC 0x0067f230 _$E115
// FUNCTION: STBC 0x0067f240 _$E333
// FUNCTION: STBC 0x0067f250 _$E332
// FUNCTION: STBC 0x0067f260 _$E337
// FUNCTION: STBC 0x0067f270 _$E336
// FUNCTION: STBC 0x0067f280 _$E341
// FUNCTION: STBC 0x0067f290 _$E340
// FUNCTION: STBC 0x0067f2a0 _$E122
// FUNCTION: STBC 0x0067f2b0 _$E121
// FUNCTION: STBC 0x0067f2c0 _$E127
// FUNCTION: STBC 0x0067f2d0 _$E126
// FUNCTION: STBC 0x0067f2e0 _$E157
// FUNCTION: STBC 0x0067f2f0 _$E156
// FUNCTION: STBC 0x0067f300 _$E345
// FUNCTION: STBC 0x0067f310 _$E344
// FUNCTION: STBC 0x0067f320 _$E162
// FUNCTION: STBC 0x0067f330 _$E161
// FUNCTION: STBC 0x0067f340 _$E349
// FUNCTION: STBC 0x0067f350 _$E348
// FUNCTION: STBC 0x0067f360 _$E353
// FUNCTION: STBC 0x0067f370 _$E352
// FUNCTION: STBC 0x0067f380 _$E167
// FUNCTION: STBC 0x0067f390 _$E166
// FUNCTION: STBC 0x0067f3a0 _$E357
// FUNCTION: STBC 0x0067f3b0 _$E356
// FUNCTION: STBC 0x0067f3c0 _$E132
// FUNCTION: STBC 0x0067f3d0 _$E131
// FUNCTION: STBC 0x0067f3e0 _$E361
// FUNCTION: STBC 0x0067f3f0 _$E360
// FUNCTION: STBC 0x0067f400 _$E365
// FUNCTION: STBC 0x0067f410 _$E364
// FUNCTION: STBC 0x0067f420 _$E172
// FUNCTION: STBC 0x0067f430 _$E171
// FUNCTION: STBC 0x0067f440 _$E177
// FUNCTION: STBC 0x0067f450 _$E176
// FUNCTION: STBC 0x0067f460 _$E369
// FUNCTION: STBC 0x0067f470 _$E368
// FUNCTION: STBC 0x0067f480 _$E373
// FUNCTION: STBC 0x0067f490 _$E372
// FUNCTION: STBC 0x0067f4a0 _$E377
// FUNCTION: STBC 0x0067f4b0 _$E376
// FUNCTION: STBC 0x0067f4c0 _$E381
// FUNCTION: STBC 0x0067f4d0 _$E380
// FUNCTION: STBC 0x0067f4e0 _$E385
// FUNCTION: STBC 0x0067f4f0 _$E384
// FUNCTION: STBC 0x0067f500 _$E389
// FUNCTION: STBC 0x0067f510 _$E388
// FUNCTION: STBC 0x0067f520 _$E393
// FUNCTION: STBC 0x0067f530 _$E392
// FUNCTION: STBC 0x0067f540 _$E397
// FUNCTION: STBC 0x0067f550 _$E396
// FUNCTION: STBC 0x0067f560 _$E182
// FUNCTION: STBC 0x0067f570 _$E181
// FUNCTION: STBC 0x0067f580 _$E401
// FUNCTION: STBC 0x0067f590 _$E400
// FUNCTION: STBC 0x0067f5a0 _$E187
// FUNCTION: STBC 0x0067f5b0 _$E186
// FUNCTION: STBC 0x0067f5c0 _$E192
// FUNCTION: STBC 0x0067f5d0 _$E191
// FUNCTION: STBC 0x0067f5e0 _$E405
// FUNCTION: STBC 0x0067f5f0 _$E404
// FUNCTION: STBC 0x0067f600 _$E410
// FUNCTION: STBC 0x0067f610 _$E409
// FUNCTION: STBC 0x0067f620 _$E414
// FUNCTION: STBC 0x0067f630 _$E413

/*
================
SparkEmitterProperty::Cast

The property as a SparkEmitterProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067f640 ?Cast@SparkEmitterProperty@@SAPAV1@PAVTGModelProperty@@@Z
SparkEmitterProperty *SparkEmitterProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SPARKEMITTERPROPERTY_TYPE ) ) {
		return (SparkEmitterProperty *)property;
	}
	return NULL;
}

/*
================
SparkEmitterProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067f670
SparkEmitterProperty *SparkEmitterProperty::Create( char *name ) {
	SparkEmitterProperty *property = new SparkEmitterProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
SparkEmitterProperty::SparkEmitterProperty
================
*/
// FUNCTION: STBC 0x0067f6c0
SparkEmitterProperty::SparkEmitterProperty( long id ) : EffectEmitterProperty( id ) {
}

/*
================
SparkEmitterProperty::WriteScript

Writes "<variable> = App.SparkEmitterProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067f760
void SparkEmitterProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.SparkEmitterProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
