/*
===============================================================================

	Sun.cpp

===============================================================================
*/

#include <iostream>
#include "Sun.h"
#include "SunEffect.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x005a50d0 _$E447
// FUNCTION: STBC 0x005a50f0 _$E446

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005a3b00 _$E32
// FUNCTION: STBC 0x005a3b10 _$E29
// FUNCTION: STBC 0x005a3b20 _$E31
// FUNCTION: STBC 0x005a3b30 _$E30
// FUNCTION: STBC 0x005a3b40 _$E38
// FUNCTION: STBC 0x005a3b50 _$E35
// FUNCTION: STBC 0x005a3b60 _$E37
// FUNCTION: STBC 0x005a3b70 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005a4850 _$E77
// FUNCTION: STBC 0x005a4860 _$E76
// FUNCTION: STBC 0x005a4870 _$E81
// FUNCTION: STBC 0x005a4880 _$E80
// FUNCTION: STBC 0x005a4890 _$E85
// FUNCTION: STBC 0x005a48a0 _$E84
// FUNCTION: STBC 0x005a48b0 _$E89
// FUNCTION: STBC 0x005a48c0 _$E88
// FUNCTION: STBC 0x005a48d0 _$E206
// FUNCTION: STBC 0x005a48e0 _$E205
// FUNCTION: STBC 0x005a48f0 _$E148
// FUNCTION: STBC 0x005a4900 _$E147
// FUNCTION: STBC 0x005a4910 _$E161
// FUNCTION: STBC 0x005a4920 _$E160
// FUNCTION: STBC 0x005a4930 _$E211
// FUNCTION: STBC 0x005a4940 _$E210
// FUNCTION: STBC 0x005a4950 _$E230
// FUNCTION: STBC 0x005a4960 _$E229
// FUNCTION: STBC 0x005a4970 _$E215
// FUNCTION: STBC 0x005a4980 _$E214
// FUNCTION: STBC 0x005a4990 _$E234
// FUNCTION: STBC 0x005a49a0 _$E233
// FUNCTION: STBC 0x005a49b0 _$E241
// FUNCTION: STBC 0x005a49c0 _$E240
// FUNCTION: STBC 0x005a49d0 _$E266
// FUNCTION: STBC 0x005a49e0 _$E265
// FUNCTION: STBC 0x005a49f0 _$E270
// FUNCTION: STBC 0x005a4a00 _$E269
// FUNCTION: STBC 0x005a4a10 _$E106
// FUNCTION: STBC 0x005a4a20 _$E105
// FUNCTION: STBC 0x005a4a30 _$E252
// FUNCTION: STBC 0x005a4a40 _$E251
// FUNCTION: STBC 0x005a4a50 _$E259
// FUNCTION: STBC 0x005a4a60 _$E258
// FUNCTION: STBC 0x005a4a70 _$E282
// FUNCTION: STBC 0x005a4a80 _$E281
// FUNCTION: STBC 0x005a4a90 _$E286
// FUNCTION: STBC 0x005a4aa0 _$E285
// FUNCTION: STBC 0x005a4ab0 _$E326
// FUNCTION: STBC 0x005a4ac0 _$E325
// FUNCTION: STBC 0x005a4ad0 _$E332
// FUNCTION: STBC 0x005a4ae0 _$E331
// FUNCTION: STBC 0x005a4af0 _$E274
// FUNCTION: STBC 0x005a4b00 _$E273
// FUNCTION: STBC 0x005a4b10 _$E278
// FUNCTION: STBC 0x005a4b20 _$E277
// FUNCTION: STBC 0x005a4b30 _$E290
// FUNCTION: STBC 0x005a4b40 _$E289
// FUNCTION: STBC 0x005a4b50 _$E294
// FUNCTION: STBC 0x005a4b60 _$E293
// FUNCTION: STBC 0x005a4b70 _$E298
// FUNCTION: STBC 0x005a4b80 _$E297
// FUNCTION: STBC 0x005a4b90 _$E302
// FUNCTION: STBC 0x005a4ba0 _$E301
// FUNCTION: STBC 0x005a4bb0 _$E306
// FUNCTION: STBC 0x005a4bc0 _$E305
// FUNCTION: STBC 0x005a4bd0 _$E310
// FUNCTION: STBC 0x005a4be0 _$E309
// FUNCTION: STBC 0x005a4bf0 _$E314
// FUNCTION: STBC 0x005a4c00 _$E313
// FUNCTION: STBC 0x005a4c10 _$E318
// FUNCTION: STBC 0x005a4c20 _$E317
// FUNCTION: STBC 0x005a4c30 _$E111
// FUNCTION: STBC 0x005a4c40 _$E110
// FUNCTION: STBC 0x005a4c50 _$E322
// FUNCTION: STBC 0x005a4c60 _$E321
// FUNCTION: STBC 0x005a4c70 _$E116
// FUNCTION: STBC 0x005a4c80 _$E115
// FUNCTION: STBC 0x005a4c90 _$E336
// FUNCTION: STBC 0x005a4ca0 _$E335
// FUNCTION: STBC 0x005a4cb0 _$E340
// FUNCTION: STBC 0x005a4cc0 _$E339
// FUNCTION: STBC 0x005a4cd0 _$E344
// FUNCTION: STBC 0x005a4ce0 _$E343
// FUNCTION: STBC 0x005a4cf0 _$E348
// FUNCTION: STBC 0x005a4d00 _$E347
// FUNCTION: STBC 0x005a4d10 _$E125
// FUNCTION: STBC 0x005a4d20 _$E124
// FUNCTION: STBC 0x005a4d30 _$E352
// FUNCTION: STBC 0x005a4d40 _$E351
// FUNCTION: STBC 0x005a4d50 _$E356
// FUNCTION: STBC 0x005a4d60 _$E355
// FUNCTION: STBC 0x005a4d70 _$E360
// FUNCTION: STBC 0x005a4d80 _$E359
// FUNCTION: STBC 0x005a4d90 _$E131
// FUNCTION: STBC 0x005a4da0 _$E130
// FUNCTION: STBC 0x005a4db0 _$E136
// FUNCTION: STBC 0x005a4dc0 _$E135
// FUNCTION: STBC 0x005a4dd0 _$E166
// FUNCTION: STBC 0x005a4de0 _$E165
// FUNCTION: STBC 0x005a4df0 _$E364
// FUNCTION: STBC 0x005a4e00 _$E363
// FUNCTION: STBC 0x005a4e10 _$E171
// FUNCTION: STBC 0x005a4e20 _$E170
// FUNCTION: STBC 0x005a4e30 _$E368
// FUNCTION: STBC 0x005a4e40 _$E367
// FUNCTION: STBC 0x005a4e50 _$E372
// FUNCTION: STBC 0x005a4e60 _$E371
// FUNCTION: STBC 0x005a4e70 _$E176
// FUNCTION: STBC 0x005a4e80 _$E175
// FUNCTION: STBC 0x005a4e90 _$E376
// FUNCTION: STBC 0x005a4ea0 _$E375
// FUNCTION: STBC 0x005a4eb0 _$E141
// FUNCTION: STBC 0x005a4ec0 _$E140
// FUNCTION: STBC 0x005a4ed0 _$E380
// FUNCTION: STBC 0x005a4ee0 _$E379
// FUNCTION: STBC 0x005a4ef0 _$E384
// FUNCTION: STBC 0x005a4f00 _$E383
// FUNCTION: STBC 0x005a4f10 _$E181
// FUNCTION: STBC 0x005a4f20 _$E180
// FUNCTION: STBC 0x005a4f30 _$E186
// FUNCTION: STBC 0x005a4f40 _$E185
// FUNCTION: STBC 0x005a4f50 _$E388
// FUNCTION: STBC 0x005a4f60 _$E387
// FUNCTION: STBC 0x005a4f70 _$E392
// FUNCTION: STBC 0x005a4f80 _$E391
// FUNCTION: STBC 0x005a4f90 _$E396
// FUNCTION: STBC 0x005a4fa0 _$E395
// FUNCTION: STBC 0x005a4fb0 _$E400
// FUNCTION: STBC 0x005a4fc0 _$E399
// FUNCTION: STBC 0x005a4fd0 _$E404
// FUNCTION: STBC 0x005a4fe0 _$E403
// FUNCTION: STBC 0x005a4ff0 _$E408
// FUNCTION: STBC 0x005a5000 _$E407
// FUNCTION: STBC 0x005a5010 _$E412
// FUNCTION: STBC 0x005a5020 _$E411
// FUNCTION: STBC 0x005a5030 _$E416
// FUNCTION: STBC 0x005a5040 _$E415
// FUNCTION: STBC 0x005a5050 _$E191
// FUNCTION: STBC 0x005a5060 _$E190
// FUNCTION: STBC 0x005a5070 _$E420
// FUNCTION: STBC 0x005a5080 _$E419
// FUNCTION: STBC 0x005a5090 _$E196
// FUNCTION: STBC 0x005a50a0 _$E195
// FUNCTION: STBC 0x005a50b0 _$E201
// FUNCTION: STBC 0x005a50c0 _$E200
// FUNCTION: STBC 0x005a5100 _$E424
// FUNCTION: STBC 0x005a5110 _$E423
// FUNCTION: STBC 0x005a5120 _$E429
// FUNCTION: STBC 0x005a5130 _$E428
// FUNCTION: STBC 0x005a5140 _$E433
// FUNCTION: STBC 0x005a5150 _$E432

/*
================
Sun::Cast

The object as a Sun; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005a4650
Sun *Sun::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SUN_TYPE ) ) {
		return (Sun *)object;
	}
	return NULL;
}

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x005a4600 _$E439
// FUNCTION: STBC 0x005a4610 _$E435
// FUNCTION: STBC 0x005a4620 _$E438
// FUNCTION: STBC 0x005a4630 _$E436
// DATA: STBC 0x0098e3c0
TGEventHandlerTable Sun::classHandlers;

/*
================
Sun::Sun
================
*/
// FUNCTION: STBC 0x005a4680
Sun::Sun( float radius_, float atmosphereRadius_, float unknown_, char *unknown7C_, char *unknown80_ ) : Planet( radius_, atmosphereRadius_, NULL ), unknown7C( NULL ), unknown80( NULL ) {
	if ( unknown7C_ != NULL ) {
		unknown7C = new char[strlen( unknown7C_ ) + 1];
		strcpy( unknown7C, unknown7C_ );
		// the second name is kept only with the first
		if ( unknown80_ != NULL ) {
			unknown80 = new char[strlen( unknown80_ ) + 1];
			strcpy( unknown80, unknown80_ );
		}
	}
	Unknown5A4780();
}

/*
================
Sun::Unknown5A4780

Builds the sun's model from its two textures at its radius, the flares
reaching out to the atmosphere (125 beyond the radius when there is
none), and makes it the object's.
================
*/
// FUNCTION: STBC 0x005a4780
// DRAFT: x87 order: retail loads radius before the branch and adds 125 or the atmosphere in each arm; this build loads the chosen addend and adds radius after
void Sun::Unknown5A4780( void ) {
	SetNiObject( SunEffect::Create( unknown7C, unknown80, radius + ( atmosphereRadius == 0.0f ? 125.0f : atmosphereRadius ), radius ) );
}

/*
================
Sun::WriteToStream
================
*/
// FUNCTION: STBC 0x005a47d0
bool Sun::WriteToStream( TGStream *stream ) {
	if ( !Planet::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( unknown7C );
	stream->VirtualSlot11( unknown80 );
	return true;
}

/*
================
Sun::ReadFromStream
================
*/
// FUNCTION: STBC 0x005a4810
bool Sun::ReadFromStream( TGStream *stream ) {
	if ( !Planet::ReadFromStream( stream ) ) {
		return false;
	}
	unknown7C = stream->VirtualSlot10( -1 );
	unknown80 = stream->VirtualSlot10( -1 );
	Unknown5A4780();
	return true;
}
