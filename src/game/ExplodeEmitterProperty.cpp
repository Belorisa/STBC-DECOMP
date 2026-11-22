/*
===============================================================================

	ExplodeEmitterProperty.cpp

===============================================================================
*/

#include "ExplodeEmitterProperty.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067d7c0 _$E433
// FUNCTION: STBC 0x0067d7e0 _$E432

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067d400 _$E419
// FUNCTION: STBC 0x0067d410 _$E416
// FUNCTION: STBC 0x0067d420 _$E418
// FUNCTION: STBC 0x0067d430 _$E417
// FUNCTION: STBC 0x0067d440 _$E425
// FUNCTION: STBC 0x0067d450 _$E422
// FUNCTION: STBC 0x0067d460 _$E424
// FUNCTION: STBC 0x0067d470 _$E423

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0067d480 _$E51
// FUNCTION: STBC 0x0067d490 _$E50
// FUNCTION: STBC 0x0067d4a0 _$E55
// FUNCTION: STBC 0x0067d4b0 _$E54
// FUNCTION: STBC 0x0067d4c0 _$E66
// FUNCTION: STBC 0x0067d4d0 _$E65
// FUNCTION: STBC 0x0067d4e0 _$E70
// FUNCTION: STBC 0x0067d4f0 _$E69
// FUNCTION: STBC 0x0067d500 _$E74
// FUNCTION: STBC 0x0067d510 _$E73
// FUNCTION: STBC 0x0067d520 _$E78
// FUNCTION: STBC 0x0067d530 _$E77
// FUNCTION: STBC 0x0067d540 _$E197
// FUNCTION: STBC 0x0067d550 _$E196
// FUNCTION: STBC 0x0067d560 _$E139
// FUNCTION: STBC 0x0067d570 _$E138
// FUNCTION: STBC 0x0067d580 _$E152
// FUNCTION: STBC 0x0067d590 _$E151
// FUNCTION: STBC 0x0067d5a0 _$E202
// FUNCTION: STBC 0x0067d5b0 _$E201
// FUNCTION: STBC 0x0067d5c0 _$E207
// FUNCTION: STBC 0x0067d5d0 _$E206
// FUNCTION: STBC 0x0067d5e0 _$E211
// FUNCTION: STBC 0x0067d5f0 _$E210
// FUNCTION: STBC 0x0067d600 _$E215
// FUNCTION: STBC 0x0067d610 _$E214
// FUNCTION: STBC 0x0067d620 _$E222
// FUNCTION: STBC 0x0067d630 _$E221
// FUNCTION: STBC 0x0067d640 _$E247
// FUNCTION: STBC 0x0067d650 _$E246
// FUNCTION: STBC 0x0067d660 _$E251
// FUNCTION: STBC 0x0067d670 _$E250
// FUNCTION: STBC 0x0067d680 _$E97
// FUNCTION: STBC 0x0067d690 _$E96
// FUNCTION: STBC 0x0067d6a0 _$E233
// FUNCTION: STBC 0x0067d6b0 _$E232
// FUNCTION: STBC 0x0067d6c0 _$E240
// FUNCTION: STBC 0x0067d6d0 _$E239
// FUNCTION: STBC 0x0067d6e0 _$E263
// FUNCTION: STBC 0x0067d6f0 _$E262
// FUNCTION: STBC 0x0067d700 _$E267
// FUNCTION: STBC 0x0067d710 _$E266
// FUNCTION: STBC 0x0067d720 _$E307
// FUNCTION: STBC 0x0067d730 _$E306
// FUNCTION: STBC 0x0067d740 _$E313
// FUNCTION: STBC 0x0067d750 _$E312
// FUNCTION: STBC 0x0067d760 _$E255
// FUNCTION: STBC 0x0067d770 _$E254
// FUNCTION: STBC 0x0067d780 _$E259
// FUNCTION: STBC 0x0067d790 _$E258
// FUNCTION: STBC 0x0067d7a0 _$E271
// FUNCTION: STBC 0x0067d7b0 _$E270
// FUNCTION: STBC 0x0067d7f0 _$E275
// FUNCTION: STBC 0x0067d800 _$E274
// FUNCTION: STBC 0x0067d810 _$E279
// FUNCTION: STBC 0x0067d820 _$E278
// FUNCTION: STBC 0x0067d830 _$E283
// FUNCTION: STBC 0x0067d840 _$E282
// FUNCTION: STBC 0x0067d850 _$E287
// FUNCTION: STBC 0x0067d860 _$E286
// FUNCTION: STBC 0x0067d870 _$E291
// FUNCTION: STBC 0x0067d880 _$E290
// FUNCTION: STBC 0x0067d890 _$E295
// FUNCTION: STBC 0x0067d8a0 _$E294
// FUNCTION: STBC 0x0067d8b0 _$E299
// FUNCTION: STBC 0x0067d8c0 _$E298
// FUNCTION: STBC 0x0067d8d0 _$E102
// FUNCTION: STBC 0x0067d8e0 _$E101
// FUNCTION: STBC 0x0067d8f0 _$E303
// FUNCTION: STBC 0x0067d900 _$E302
// FUNCTION: STBC 0x0067d910 _$E107
// FUNCTION: STBC 0x0067d920 _$E106
// FUNCTION: STBC 0x0067d930 _$E317
// FUNCTION: STBC 0x0067d940 _$E316
// FUNCTION: STBC 0x0067d950 _$E321
// FUNCTION: STBC 0x0067d960 _$E320
// FUNCTION: STBC 0x0067d970 _$E325
// FUNCTION: STBC 0x0067d980 _$E324
// FUNCTION: STBC 0x0067d990 _$E329
// FUNCTION: STBC 0x0067d9a0 _$E328
// FUNCTION: STBC 0x0067d9b0 _$E116
// FUNCTION: STBC 0x0067d9c0 _$E115
// FUNCTION: STBC 0x0067d9d0 _$E333
// FUNCTION: STBC 0x0067d9e0 _$E332
// FUNCTION: STBC 0x0067d9f0 _$E337
// FUNCTION: STBC 0x0067da00 _$E336
// FUNCTION: STBC 0x0067da10 _$E341
// FUNCTION: STBC 0x0067da20 _$E340
// FUNCTION: STBC 0x0067da30 _$E122
// FUNCTION: STBC 0x0067da40 _$E121
// FUNCTION: STBC 0x0067da50 _$E127
// FUNCTION: STBC 0x0067da60 _$E126
// FUNCTION: STBC 0x0067da70 _$E157
// FUNCTION: STBC 0x0067da80 _$E156
// FUNCTION: STBC 0x0067da90 _$E345
// FUNCTION: STBC 0x0067daa0 _$E344
// FUNCTION: STBC 0x0067dab0 _$E162
// FUNCTION: STBC 0x0067dac0 _$E161
// FUNCTION: STBC 0x0067dad0 _$E349
// FUNCTION: STBC 0x0067dae0 _$E348
// FUNCTION: STBC 0x0067daf0 _$E353
// FUNCTION: STBC 0x0067db00 _$E352
// FUNCTION: STBC 0x0067db10 _$E167
// FUNCTION: STBC 0x0067db20 _$E166
// FUNCTION: STBC 0x0067db30 _$E357
// FUNCTION: STBC 0x0067db40 _$E356
// FUNCTION: STBC 0x0067db50 _$E132
// FUNCTION: STBC 0x0067db60 _$E131
// FUNCTION: STBC 0x0067db70 _$E361
// FUNCTION: STBC 0x0067db80 _$E360
// FUNCTION: STBC 0x0067db90 _$E365
// FUNCTION: STBC 0x0067dba0 _$E364
// FUNCTION: STBC 0x0067dbb0 _$E172
// FUNCTION: STBC 0x0067dbc0 _$E171
// FUNCTION: STBC 0x0067dbd0 _$E177
// FUNCTION: STBC 0x0067dbe0 _$E176
// FUNCTION: STBC 0x0067dbf0 _$E369
// FUNCTION: STBC 0x0067dc00 _$E368
// FUNCTION: STBC 0x0067dc10 _$E373
// FUNCTION: STBC 0x0067dc20 _$E372
// FUNCTION: STBC 0x0067dc30 _$E377
// FUNCTION: STBC 0x0067dc40 _$E376
// FUNCTION: STBC 0x0067dc50 _$E381
// FUNCTION: STBC 0x0067dc60 _$E380
// FUNCTION: STBC 0x0067dc70 _$E385
// FUNCTION: STBC 0x0067dc80 _$E384
// FUNCTION: STBC 0x0067dc90 _$E389
// FUNCTION: STBC 0x0067dca0 _$E388
// FUNCTION: STBC 0x0067dcb0 _$E393
// FUNCTION: STBC 0x0067dcc0 _$E392
// FUNCTION: STBC 0x0067dcd0 _$E397
// FUNCTION: STBC 0x0067dce0 _$E396
// FUNCTION: STBC 0x0067dcf0 _$E182
// FUNCTION: STBC 0x0067dd00 _$E181
// FUNCTION: STBC 0x0067dd10 _$E401
// FUNCTION: STBC 0x0067dd20 _$E400
// FUNCTION: STBC 0x0067dd30 _$E187
// FUNCTION: STBC 0x0067dd40 _$E186
// FUNCTION: STBC 0x0067dd50 _$E192
// FUNCTION: STBC 0x0067dd60 _$E191
// FUNCTION: STBC 0x0067dd70 _$E405
// FUNCTION: STBC 0x0067dd80 _$E404
// FUNCTION: STBC 0x0067dd90 _$E410
// FUNCTION: STBC 0x0067dda0 _$E409
// FUNCTION: STBC 0x0067ddb0 _$E414
// FUNCTION: STBC 0x0067ddc0 _$E413

/*
================
ExplodeEmitterProperty::Cast

The property as a ExplodeEmitterProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0067dde0 ?Cast@ExplodeEmitterProperty@@SAPAV1@PAVTGModelProperty@@@Z
ExplodeEmitterProperty *ExplodeEmitterProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( EXPLODEEMITTERPROPERTY_TYPE ) ) {
		return (ExplodeEmitterProperty *)property;
	}
	return NULL;
}

/*
================
ExplodeEmitterProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0067de10
ExplodeEmitterProperty *ExplodeEmitterProperty::Create( char *name ) {
	ExplodeEmitterProperty *property = new ExplodeEmitterProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
ExplodeEmitterProperty::ExplodeEmitterProperty
================
*/
// FUNCTION: STBC 0x0067de60
ExplodeEmitterProperty::ExplodeEmitterProperty( long id ) : EffectEmitterProperty( id ) {
}

/*
================
ExplodeEmitterProperty::WriteScript

Writes "<variable> = App.ExplodeEmitterProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0067df00
void ExplodeEmitterProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.ExplodeEmitterProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
