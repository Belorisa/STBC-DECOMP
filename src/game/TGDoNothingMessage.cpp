/*
===============================================================================

	TGDoNothingMessage.cpp

===============================================================================
*/

#include <iostream>
#include "TGDoNothingMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bdf20 _$E422
// FUNCTION: STBC 0x006bdf40 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006bdda0 _$E32
// FUNCTION: STBC 0x006bddb0 _$E29
// FUNCTION: STBC 0x006bddc0 _$E31
// FUNCTION: STBC 0x006bddd0 _$E30
// FUNCTION: STBC 0x006bdde0 _$E38
// FUNCTION: STBC 0x006bddf0 _$E35
// FUNCTION: STBC 0x006bde00 _$E37
// FUNCTION: STBC 0x006bde10 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006bde20 _$E59
// FUNCTION: STBC 0x006bde30 _$E58
// FUNCTION: STBC 0x006bde40 _$E63
// FUNCTION: STBC 0x006bde50 _$E62
// FUNCTION: STBC 0x006bde60 _$E74
// FUNCTION: STBC 0x006bde70 _$E73
// FUNCTION: STBC 0x006bde80 _$E78
// FUNCTION: STBC 0x006bde90 _$E77
// FUNCTION: STBC 0x006bdea0 _$E82
// FUNCTION: STBC 0x006bdeb0 _$E81
// FUNCTION: STBC 0x006bdec0 _$E86
// FUNCTION: STBC 0x006bded0 _$E85
// FUNCTION: STBC 0x006bdee0 _$E210
// FUNCTION: STBC 0x006bdef0 _$E209
// FUNCTION: STBC 0x006bdf00 _$E152
// FUNCTION: STBC 0x006bdf10 _$E151
// FUNCTION: STBC 0x006bdf50 _$E165
// FUNCTION: STBC 0x006bdf60 _$E164
// FUNCTION: STBC 0x006bdf70 _$E215
// FUNCTION: STBC 0x006bdf80 _$E214
// FUNCTION: STBC 0x006bdf90 _$E220
// FUNCTION: STBC 0x006bdfa0 _$E219
// FUNCTION: STBC 0x006bdfb0 _$E224
// FUNCTION: STBC 0x006bdfc0 _$E223
// FUNCTION: STBC 0x006bdfd0 _$E228
// FUNCTION: STBC 0x006bdfe0 _$E227
// FUNCTION: STBC 0x006bdff0 _$E235
// FUNCTION: STBC 0x006be000 _$E234
// FUNCTION: STBC 0x006be010 _$E260
// FUNCTION: STBC 0x006be020 _$E259
// FUNCTION: STBC 0x006be030 _$E264
// FUNCTION: STBC 0x006be040 _$E263
// FUNCTION: STBC 0x006be050 _$E110
// FUNCTION: STBC 0x006be060 _$E109
// FUNCTION: STBC 0x006be070 _$E246
// FUNCTION: STBC 0x006be080 _$E245
// FUNCTION: STBC 0x006be090 _$E253
// FUNCTION: STBC 0x006be0a0 _$E252
// FUNCTION: STBC 0x006be0b0 _$E276
// FUNCTION: STBC 0x006be0c0 _$E275
// FUNCTION: STBC 0x006be0d0 _$E280
// FUNCTION: STBC 0x006be0e0 _$E279
// FUNCTION: STBC 0x006be0f0 _$E320
// FUNCTION: STBC 0x006be100 _$E319
// FUNCTION: STBC 0x006be110 _$E326
// FUNCTION: STBC 0x006be120 _$E325
// FUNCTION: STBC 0x006be130 _$E268
// FUNCTION: STBC 0x006be140 _$E267
// FUNCTION: STBC 0x006be150 _$E272
// FUNCTION: STBC 0x006be160 _$E271
// FUNCTION: STBC 0x006be170 _$E284
// FUNCTION: STBC 0x006be180 _$E283
// FUNCTION: STBC 0x006be190 _$E288
// FUNCTION: STBC 0x006be1a0 _$E287
// FUNCTION: STBC 0x006be1b0 _$E292
// FUNCTION: STBC 0x006be1c0 _$E291
// FUNCTION: STBC 0x006be1d0 _$E296
// FUNCTION: STBC 0x006be1e0 _$E295
// FUNCTION: STBC 0x006be1f0 _$E300
// FUNCTION: STBC 0x006be200 _$E299
// FUNCTION: STBC 0x006be210 _$E304
// FUNCTION: STBC 0x006be220 _$E303
// FUNCTION: STBC 0x006be230 _$E308
// FUNCTION: STBC 0x006be240 _$E307
// FUNCTION: STBC 0x006be250 _$E312
// FUNCTION: STBC 0x006be260 _$E311
// FUNCTION: STBC 0x006be270 _$E115
// FUNCTION: STBC 0x006be280 _$E114
// FUNCTION: STBC 0x006be290 _$E316
// FUNCTION: STBC 0x006be2a0 _$E315
// FUNCTION: STBC 0x006be2b0 _$E120
// FUNCTION: STBC 0x006be2c0 _$E119
// FUNCTION: STBC 0x006be2d0 _$E330
// FUNCTION: STBC 0x006be2e0 _$E329
// FUNCTION: STBC 0x006be2f0 _$E334
// FUNCTION: STBC 0x006be300 _$E333
// FUNCTION: STBC 0x006be310 _$E338
// FUNCTION: STBC 0x006be320 _$E337
// FUNCTION: STBC 0x006be330 _$E342
// FUNCTION: STBC 0x006be340 _$E341
// FUNCTION: STBC 0x006be350 _$E129
// FUNCTION: STBC 0x006be360 _$E128
// FUNCTION: STBC 0x006be370 _$E346
// FUNCTION: STBC 0x006be380 _$E345
// FUNCTION: STBC 0x006be390 _$E350
// FUNCTION: STBC 0x006be3a0 _$E349
// FUNCTION: STBC 0x006be3b0 _$E354
// FUNCTION: STBC 0x006be3c0 _$E353
// FUNCTION: STBC 0x006be3d0 _$E135
// FUNCTION: STBC 0x006be3e0 _$E134
// FUNCTION: STBC 0x006be3f0 _$E140
// FUNCTION: STBC 0x006be400 _$E139
// FUNCTION: STBC 0x006be410 _$E170
// FUNCTION: STBC 0x006be420 _$E169
// FUNCTION: STBC 0x006be430 _$E358
// FUNCTION: STBC 0x006be440 _$E357
// FUNCTION: STBC 0x006be450 _$E175
// FUNCTION: STBC 0x006be460 _$E174
// FUNCTION: STBC 0x006be470 _$E362
// FUNCTION: STBC 0x006be480 _$E361
// FUNCTION: STBC 0x006be490 _$E366
// FUNCTION: STBC 0x006be4a0 _$E365
// FUNCTION: STBC 0x006be4b0 _$E180
// FUNCTION: STBC 0x006be4c0 _$E179
// FUNCTION: STBC 0x006be4d0 _$E370
// FUNCTION: STBC 0x006be4e0 _$E369
// FUNCTION: STBC 0x006be4f0 _$E145
// FUNCTION: STBC 0x006be500 _$E144
// FUNCTION: STBC 0x006be510 _$E374
// FUNCTION: STBC 0x006be520 _$E373
// FUNCTION: STBC 0x006be530 _$E378
// FUNCTION: STBC 0x006be540 _$E377
// FUNCTION: STBC 0x006be550 _$E185
// FUNCTION: STBC 0x006be560 _$E184
// FUNCTION: STBC 0x006be570 _$E190
// FUNCTION: STBC 0x006be580 _$E189
// FUNCTION: STBC 0x006be590 _$E382
// FUNCTION: STBC 0x006be5a0 _$E381
// FUNCTION: STBC 0x006be5b0 _$E386
// FUNCTION: STBC 0x006be5c0 _$E385
// FUNCTION: STBC 0x006be5d0 _$E390
// FUNCTION: STBC 0x006be5e0 _$E389
// FUNCTION: STBC 0x006be5f0 _$E394
// FUNCTION: STBC 0x006be600 _$E393
// FUNCTION: STBC 0x006be610 _$E398
// FUNCTION: STBC 0x006be620 _$E397
// FUNCTION: STBC 0x006be630 _$E402
// FUNCTION: STBC 0x006be640 _$E401
// FUNCTION: STBC 0x006be650 _$E406
// FUNCTION: STBC 0x006be660 _$E405
// FUNCTION: STBC 0x006be670 _$E410
// FUNCTION: STBC 0x006be680 _$E409
// FUNCTION: STBC 0x006be690 _$E195
// FUNCTION: STBC 0x006be6a0 _$E194
// FUNCTION: STBC 0x006be6b0 _$E414
// FUNCTION: STBC 0x006be6c0 _$E413
// FUNCTION: STBC 0x006be6d0 _$E200
// FUNCTION: STBC 0x006be6e0 _$E199
// FUNCTION: STBC 0x006be6f0 _$E205
// FUNCTION: STBC 0x006be700 _$E204

/*
================
TGDoNothingMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006bdc30
void TGDoNothingMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGDONOTHINGMESSAGE_TYPE, UnSerialize );
}

/*
================
TGDoNothingMessage::TGDoNothingMessage
================
*/
// FUNCTION: STBC 0x006bdc40 ??0TGDoNothingMessage@@QAE@XZ
TGDoNothingMessage::TGDoNothingMessage( void ) {
	// BUG: redundant: builds and destroys a second TGMessage that is never used (the base constructor already ran)
	TGMessage message;
}

/*
================
TGDoNothingMessage::~TGDoNothingMessage
================
*/
// FUNCTION: STBC 0x006bdcd0
TGDoNothingMessage::~TGDoNothingMessage( void ) {
}

/*
================
TGDoNothingMessage::Serialize

Writes the type byte, all the message holds; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006bdce0
int TGDoNothingMessage::Serialize( char *buffer, int bufferSize ) {
	if ( bufferSize < GetBufferSpaceRequired() ) {
		return 0;
	}

	*buffer = VirtualSlot00();
	return sizeof( char );
}

/*
================
TGDoNothingMessage::UnSerialize

The message carries nothing, so there is nothing to read.
================
*/
// FUNCTION: STBC 0x006bdd10
TGMessage *TGDoNothingMessage::UnSerialize( char *buffer ) {
	return new TGDoNothingMessage;
}

/*
================
TGDoNothingMessage::TGDoNothingMessage
================
*/
// FUNCTION: STBC 0x006bdd40 ??0TGDoNothingMessage@@QAE@ABV0@@Z
TGDoNothingMessage::TGDoNothingMessage( const TGDoNothingMessage &message ) : TGMessage( message ) {
}

/*
================
TGDoNothingMessage::Copy
================
*/
// FUNCTION: STBC 0x006bdd60
TGMessage *TGDoNothingMessage::Copy( void ) {
	return new TGDoNothingMessage( *this );
}

/*
================
TGDoNothingMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006bdd90
int TGDoNothingMessage::GetBufferSpaceRequired( void ) {
	return sizeof( char );
}
