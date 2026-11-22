/*
===============================================================================

	TGAckMessage.cpp

===============================================================================
*/

#include <iostream>
#include "TGAckMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bd430 _$E422
// FUNCTION: STBC 0x006bd450 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006bd2b0 _$E32
// FUNCTION: STBC 0x006bd2c0 _$E29
// FUNCTION: STBC 0x006bd2d0 _$E31
// FUNCTION: STBC 0x006bd2e0 _$E30
// FUNCTION: STBC 0x006bd2f0 _$E38
// FUNCTION: STBC 0x006bd300 _$E35
// FUNCTION: STBC 0x006bd310 _$E37
// FUNCTION: STBC 0x006bd320 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006bd330 _$E59
// FUNCTION: STBC 0x006bd340 _$E58
// FUNCTION: STBC 0x006bd350 _$E63
// FUNCTION: STBC 0x006bd360 _$E62
// FUNCTION: STBC 0x006bd370 _$E74
// FUNCTION: STBC 0x006bd380 _$E73
// FUNCTION: STBC 0x006bd390 _$E78
// FUNCTION: STBC 0x006bd3a0 _$E77
// FUNCTION: STBC 0x006bd3b0 _$E82
// FUNCTION: STBC 0x006bd3c0 _$E81
// FUNCTION: STBC 0x006bd3d0 _$E86
// FUNCTION: STBC 0x006bd3e0 _$E85
// FUNCTION: STBC 0x006bd3f0 _$E210
// FUNCTION: STBC 0x006bd400 _$E209
// FUNCTION: STBC 0x006bd410 _$E152
// FUNCTION: STBC 0x006bd420 _$E151
// FUNCTION: STBC 0x006bd460 _$E165
// FUNCTION: STBC 0x006bd470 _$E164
// FUNCTION: STBC 0x006bd480 _$E215
// FUNCTION: STBC 0x006bd490 _$E214
// FUNCTION: STBC 0x006bd4a0 _$E220
// FUNCTION: STBC 0x006bd4b0 _$E219
// FUNCTION: STBC 0x006bd4c0 _$E224
// FUNCTION: STBC 0x006bd4d0 _$E223
// FUNCTION: STBC 0x006bd4e0 _$E228
// FUNCTION: STBC 0x006bd4f0 _$E227
// FUNCTION: STBC 0x006bd500 _$E235
// FUNCTION: STBC 0x006bd510 _$E234
// FUNCTION: STBC 0x006bd520 _$E260
// FUNCTION: STBC 0x006bd530 _$E259
// FUNCTION: STBC 0x006bd540 _$E264
// FUNCTION: STBC 0x006bd550 _$E263
// FUNCTION: STBC 0x006bd560 _$E110
// FUNCTION: STBC 0x006bd570 _$E109
// FUNCTION: STBC 0x006bd580 _$E246
// FUNCTION: STBC 0x006bd590 _$E245
// FUNCTION: STBC 0x006bd5a0 _$E253
// FUNCTION: STBC 0x006bd5b0 _$E252
// FUNCTION: STBC 0x006bd5c0 _$E276
// FUNCTION: STBC 0x006bd5d0 _$E275
// FUNCTION: STBC 0x006bd5e0 _$E280
// FUNCTION: STBC 0x006bd5f0 _$E279
// FUNCTION: STBC 0x006bd600 _$E320
// FUNCTION: STBC 0x006bd610 _$E319
// FUNCTION: STBC 0x006bd620 _$E326
// FUNCTION: STBC 0x006bd630 _$E325
// FUNCTION: STBC 0x006bd640 _$E268
// FUNCTION: STBC 0x006bd650 _$E267
// FUNCTION: STBC 0x006bd660 _$E272
// FUNCTION: STBC 0x006bd670 _$E271
// FUNCTION: STBC 0x006bd680 _$E284
// FUNCTION: STBC 0x006bd690 _$E283
// FUNCTION: STBC 0x006bd6a0 _$E288
// FUNCTION: STBC 0x006bd6b0 _$E287
// FUNCTION: STBC 0x006bd6c0 _$E292
// FUNCTION: STBC 0x006bd6d0 _$E291
// FUNCTION: STBC 0x006bd6e0 _$E296
// FUNCTION: STBC 0x006bd6f0 _$E295
// FUNCTION: STBC 0x006bd700 _$E300
// FUNCTION: STBC 0x006bd710 _$E299
// FUNCTION: STBC 0x006bd720 _$E304
// FUNCTION: STBC 0x006bd730 _$E303
// FUNCTION: STBC 0x006bd740 _$E308
// FUNCTION: STBC 0x006bd750 _$E307
// FUNCTION: STBC 0x006bd760 _$E312
// FUNCTION: STBC 0x006bd770 _$E311
// FUNCTION: STBC 0x006bd780 _$E115
// FUNCTION: STBC 0x006bd790 _$E114
// FUNCTION: STBC 0x006bd7a0 _$E316
// FUNCTION: STBC 0x006bd7b0 _$E315
// FUNCTION: STBC 0x006bd7c0 _$E120
// FUNCTION: STBC 0x006bd7d0 _$E119
// FUNCTION: STBC 0x006bd7e0 _$E330
// FUNCTION: STBC 0x006bd7f0 _$E329
// FUNCTION: STBC 0x006bd800 _$E334
// FUNCTION: STBC 0x006bd810 _$E333
// FUNCTION: STBC 0x006bd820 _$E338
// FUNCTION: STBC 0x006bd830 _$E337
// FUNCTION: STBC 0x006bd840 _$E342
// FUNCTION: STBC 0x006bd850 _$E341
// FUNCTION: STBC 0x006bd860 _$E129
// FUNCTION: STBC 0x006bd870 _$E128
// FUNCTION: STBC 0x006bd880 _$E346
// FUNCTION: STBC 0x006bd890 _$E345
// FUNCTION: STBC 0x006bd8a0 _$E350
// FUNCTION: STBC 0x006bd8b0 _$E349
// FUNCTION: STBC 0x006bd8c0 _$E354
// FUNCTION: STBC 0x006bd8d0 _$E353
// FUNCTION: STBC 0x006bd8e0 _$E135
// FUNCTION: STBC 0x006bd8f0 _$E134
// FUNCTION: STBC 0x006bd900 _$E140
// FUNCTION: STBC 0x006bd910 _$E139
// FUNCTION: STBC 0x006bd920 _$E170
// FUNCTION: STBC 0x006bd930 _$E169
// FUNCTION: STBC 0x006bd940 _$E358
// FUNCTION: STBC 0x006bd950 _$E357
// FUNCTION: STBC 0x006bd960 _$E175
// FUNCTION: STBC 0x006bd970 _$E174
// FUNCTION: STBC 0x006bd980 _$E362
// FUNCTION: STBC 0x006bd990 _$E361
// FUNCTION: STBC 0x006bd9a0 _$E366
// FUNCTION: STBC 0x006bd9b0 _$E365
// FUNCTION: STBC 0x006bd9c0 _$E180
// FUNCTION: STBC 0x006bd9d0 _$E179
// FUNCTION: STBC 0x006bd9e0 _$E370
// FUNCTION: STBC 0x006bd9f0 _$E369
// FUNCTION: STBC 0x006bda00 _$E145
// FUNCTION: STBC 0x006bda10 _$E144
// FUNCTION: STBC 0x006bda20 _$E374
// FUNCTION: STBC 0x006bda30 _$E373
// FUNCTION: STBC 0x006bda40 _$E378
// FUNCTION: STBC 0x006bda50 _$E377
// FUNCTION: STBC 0x006bda60 _$E185
// FUNCTION: STBC 0x006bda70 _$E184
// FUNCTION: STBC 0x006bda80 _$E190
// FUNCTION: STBC 0x006bda90 _$E189
// FUNCTION: STBC 0x006bdaa0 _$E382
// FUNCTION: STBC 0x006bdab0 _$E381
// FUNCTION: STBC 0x006bdac0 _$E386
// FUNCTION: STBC 0x006bdad0 _$E385
// FUNCTION: STBC 0x006bdae0 _$E390
// FUNCTION: STBC 0x006bdaf0 _$E389
// FUNCTION: STBC 0x006bdb00 _$E394
// FUNCTION: STBC 0x006bdb10 _$E393
// FUNCTION: STBC 0x006bdb20 _$E398
// FUNCTION: STBC 0x006bdb30 _$E397
// FUNCTION: STBC 0x006bdb40 _$E402
// FUNCTION: STBC 0x006bdb50 _$E401
// FUNCTION: STBC 0x006bdb60 _$E406
// FUNCTION: STBC 0x006bdb70 _$E405
// FUNCTION: STBC 0x006bdb80 _$E410
// FUNCTION: STBC 0x006bdb90 _$E409
// FUNCTION: STBC 0x006bdba0 _$E195
// FUNCTION: STBC 0x006bdbb0 _$E194
// FUNCTION: STBC 0x006bdbc0 _$E414
// FUNCTION: STBC 0x006bdbd0 _$E413
// FUNCTION: STBC 0x006bdbe0 _$E200
// FUNCTION: STBC 0x006bdbf0 _$E199
// FUNCTION: STBC 0x006bdc00 _$E205
// FUNCTION: STBC 0x006bdc10 _$E204

// An acknowledgement's size: the type, the sequence number and the flag byte;
// a part of a multi-part message adds its position byte.
#define ACK_SIZE	4

/*
================
TGAckMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006bd110
void TGAckMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGACKMESSAGE_TYPE, UnSerialize );
}

/*
================
TGAckMessage::TGAckMessage
================
*/
// FUNCTION: STBC 0x006bd120 ??0TGAckMessage@@QAE@XZ
TGAckMessage::TGAckMessage( void ) {
	firstResendTime = 0.0f;
	backoffType = BACKOFF_CONSTANT;
	backoffFactor = 0.6666f;
	systemMessage = true;
}

/*
================
TGAckMessage::~TGAckMessage
================
*/
// FUNCTION: STBC 0x006bd180
TGAckMessage::~TGAckMessage( void ) {
}

/*
================
TGAckMessage::Serialize

Writes the acknowledgement into buffer: the type, the acknowledged sequence
number, a flag byte (multi-part, system message) and, for a part of a
multi-part message, its position; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006bd190
int TGAckMessage::Serialize( char *buffer, int bufferSize ) {
	if ( bufferSize < GetBufferSpaceRequired() ) {
		return 0;
	}

	*buffer++ = VirtualSlot00();
	*(unsigned short *)buffer = sequenceNumber;
	buffer += sizeof( unsigned short );
	unsigned char flags = 0;
	if ( multiPart ) {
		flags = 1;
	}
	if ( systemMessage ) {
		flags |= 2;
	}
	*buffer = flags;

	int size = ACK_SIZE;
	if ( multiPart ) {
		buffer[1] = multiPartSequenceNumber;
		size = ACK_SIZE + 1;
	}
	return size;
}

/*
================
TGAckMessage::UnSerialize

Reads an acknowledgement: the acknowledged sequence number, then a flag byte
(multi-part, system message) and the multi-part position.
================
*/
// FUNCTION: STBC 0x006bd1f0
TGMessage *TGAckMessage::UnSerialize( char *buffer ) {
	TGAckMessage *message = new TGAckMessage;

	buffer++;
	message->sequenceNumber = *(unsigned short *)buffer;
	buffer += sizeof( unsigned short );
	unsigned char flags = *buffer;
	message->multiPart = flags & 1;
	message->systemMessage = ( flags >> 1 ) & 1;
	if ( message->multiPart ) {
		message->multiPartSequenceNumber = buffer[1];
	}
	return message;
}

/*
================
TGAckMessage::TGAckMessage
================
*/
// FUNCTION: STBC 0x006bd250 ??0TGAckMessage@@QAE@ABV0@@Z
// BUG: systemMessage isn't copied, so a copy always reads as a user message's acknowledgement
TGAckMessage::TGAckMessage( const TGAckMessage &message ) : TGMessage( message ) {
}

/*
================
TGAckMessage::Copy
================
*/
// FUNCTION: STBC 0x006bd270
TGMessage *TGAckMessage::Copy( void ) {
	return new TGAckMessage( *this );
}

/*
================
TGAckMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006bd2a0
int TGAckMessage::GetBufferSpaceRequired( void ) {
	return multiPart ? ACK_SIZE + 1 : ACK_SIZE;
}
