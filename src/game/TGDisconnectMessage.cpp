/*
===============================================================================

	TGDisconnectMessage.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGDisconnectMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bf680 _$E422
// FUNCTION: STBC 0x006bf6a0 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006bf500 _$E32
// FUNCTION: STBC 0x006bf510 _$E29
// FUNCTION: STBC 0x006bf520 _$E31
// FUNCTION: STBC 0x006bf530 _$E30
// FUNCTION: STBC 0x006bf540 _$E38
// FUNCTION: STBC 0x006bf550 _$E35
// FUNCTION: STBC 0x006bf560 _$E37
// FUNCTION: STBC 0x006bf570 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006bf580 _$E59
// FUNCTION: STBC 0x006bf590 _$E58
// FUNCTION: STBC 0x006bf5a0 _$E63
// FUNCTION: STBC 0x006bf5b0 _$E62
// FUNCTION: STBC 0x006bf5c0 _$E74
// FUNCTION: STBC 0x006bf5d0 _$E73
// FUNCTION: STBC 0x006bf5e0 _$E78
// FUNCTION: STBC 0x006bf5f0 _$E77
// FUNCTION: STBC 0x006bf600 _$E82
// FUNCTION: STBC 0x006bf610 _$E81
// FUNCTION: STBC 0x006bf620 _$E86
// FUNCTION: STBC 0x006bf630 _$E85
// FUNCTION: STBC 0x006bf640 _$E210
// FUNCTION: STBC 0x006bf650 _$E209
// FUNCTION: STBC 0x006bf660 _$E152
// FUNCTION: STBC 0x006bf670 _$E151
// FUNCTION: STBC 0x006bf6b0 _$E165
// FUNCTION: STBC 0x006bf6c0 _$E164
// FUNCTION: STBC 0x006bf6d0 _$E215
// FUNCTION: STBC 0x006bf6e0 _$E214
// FUNCTION: STBC 0x006bf6f0 _$E220
// FUNCTION: STBC 0x006bf700 _$E219
// FUNCTION: STBC 0x006bf710 _$E224
// FUNCTION: STBC 0x006bf720 _$E223
// FUNCTION: STBC 0x006bf730 _$E228
// FUNCTION: STBC 0x006bf740 _$E227
// FUNCTION: STBC 0x006bf750 _$E235
// FUNCTION: STBC 0x006bf760 _$E234
// FUNCTION: STBC 0x006bf770 _$E260
// FUNCTION: STBC 0x006bf780 _$E259
// FUNCTION: STBC 0x006bf790 _$E264
// FUNCTION: STBC 0x006bf7a0 _$E263
// FUNCTION: STBC 0x006bf7b0 _$E110
// FUNCTION: STBC 0x006bf7c0 _$E109
// FUNCTION: STBC 0x006bf7d0 _$E246
// FUNCTION: STBC 0x006bf7e0 _$E245
// FUNCTION: STBC 0x006bf7f0 _$E253
// FUNCTION: STBC 0x006bf800 _$E252
// FUNCTION: STBC 0x006bf810 _$E276
// FUNCTION: STBC 0x006bf820 _$E275
// FUNCTION: STBC 0x006bf830 _$E280
// FUNCTION: STBC 0x006bf840 _$E279
// FUNCTION: STBC 0x006bf850 _$E320
// FUNCTION: STBC 0x006bf860 _$E319
// FUNCTION: STBC 0x006bf870 _$E326
// FUNCTION: STBC 0x006bf880 _$E325
// FUNCTION: STBC 0x006bf890 _$E268
// FUNCTION: STBC 0x006bf8a0 _$E267
// FUNCTION: STBC 0x006bf8b0 _$E272
// FUNCTION: STBC 0x006bf8c0 _$E271
// FUNCTION: STBC 0x006bf8d0 _$E284
// FUNCTION: STBC 0x006bf8e0 _$E283
// FUNCTION: STBC 0x006bf8f0 _$E288
// FUNCTION: STBC 0x006bf900 _$E287
// FUNCTION: STBC 0x006bf910 _$E292
// FUNCTION: STBC 0x006bf920 _$E291
// FUNCTION: STBC 0x006bf930 _$E296
// FUNCTION: STBC 0x006bf940 _$E295
// FUNCTION: STBC 0x006bf950 _$E300
// FUNCTION: STBC 0x006bf960 _$E299
// FUNCTION: STBC 0x006bf970 _$E304
// FUNCTION: STBC 0x006bf980 _$E303
// FUNCTION: STBC 0x006bf990 _$E308
// FUNCTION: STBC 0x006bf9a0 _$E307
// FUNCTION: STBC 0x006bf9b0 _$E312
// FUNCTION: STBC 0x006bf9c0 _$E311
// FUNCTION: STBC 0x006bf9d0 _$E115
// FUNCTION: STBC 0x006bf9e0 _$E114
// FUNCTION: STBC 0x006bf9f0 _$E316
// FUNCTION: STBC 0x006bfa00 _$E315
// FUNCTION: STBC 0x006bfa10 _$E120
// FUNCTION: STBC 0x006bfa20 _$E119
// FUNCTION: STBC 0x006bfa30 _$E330
// FUNCTION: STBC 0x006bfa40 _$E329
// FUNCTION: STBC 0x006bfa50 _$E334
// FUNCTION: STBC 0x006bfa60 _$E333
// FUNCTION: STBC 0x006bfa70 _$E338
// FUNCTION: STBC 0x006bfa80 _$E337
// FUNCTION: STBC 0x006bfa90 _$E342
// FUNCTION: STBC 0x006bfaa0 _$E341
// FUNCTION: STBC 0x006bfab0 _$E129
// FUNCTION: STBC 0x006bfac0 _$E128
// FUNCTION: STBC 0x006bfad0 _$E346
// FUNCTION: STBC 0x006bfae0 _$E345
// FUNCTION: STBC 0x006bfaf0 _$E350
// FUNCTION: STBC 0x006bfb00 _$E349
// FUNCTION: STBC 0x006bfb10 _$E354
// FUNCTION: STBC 0x006bfb20 _$E353
// FUNCTION: STBC 0x006bfb30 _$E135
// FUNCTION: STBC 0x006bfb40 _$E134
// FUNCTION: STBC 0x006bfb50 _$E140
// FUNCTION: STBC 0x006bfb60 _$E139
// FUNCTION: STBC 0x006bfb70 _$E170
// FUNCTION: STBC 0x006bfb80 _$E169
// FUNCTION: STBC 0x006bfb90 _$E358
// FUNCTION: STBC 0x006bfba0 _$E357
// FUNCTION: STBC 0x006bfbb0 _$E175
// FUNCTION: STBC 0x006bfbc0 _$E174
// FUNCTION: STBC 0x006bfbd0 _$E362
// FUNCTION: STBC 0x006bfbe0 _$E361
// FUNCTION: STBC 0x006bfbf0 _$E366
// FUNCTION: STBC 0x006bfc00 _$E365
// FUNCTION: STBC 0x006bfc10 _$E180
// FUNCTION: STBC 0x006bfc20 _$E179
// FUNCTION: STBC 0x006bfc30 _$E370
// FUNCTION: STBC 0x006bfc40 _$E369
// FUNCTION: STBC 0x006bfc50 _$E145
// FUNCTION: STBC 0x006bfc60 _$E144
// FUNCTION: STBC 0x006bfc70 _$E374
// FUNCTION: STBC 0x006bfc80 _$E373
// FUNCTION: STBC 0x006bfc90 _$E378
// FUNCTION: STBC 0x006bfca0 _$E377
// FUNCTION: STBC 0x006bfcb0 _$E185
// FUNCTION: STBC 0x006bfcc0 _$E184
// FUNCTION: STBC 0x006bfcd0 _$E190
// FUNCTION: STBC 0x006bfce0 _$E189
// FUNCTION: STBC 0x006bfcf0 _$E382
// FUNCTION: STBC 0x006bfd00 _$E381
// FUNCTION: STBC 0x006bfd10 _$E386
// FUNCTION: STBC 0x006bfd20 _$E385
// FUNCTION: STBC 0x006bfd30 _$E390
// FUNCTION: STBC 0x006bfd40 _$E389
// FUNCTION: STBC 0x006bfd50 _$E394
// FUNCTION: STBC 0x006bfd60 _$E393
// FUNCTION: STBC 0x006bfd70 _$E398
// FUNCTION: STBC 0x006bfd80 _$E397
// FUNCTION: STBC 0x006bfd90 _$E402
// FUNCTION: STBC 0x006bfda0 _$E401
// FUNCTION: STBC 0x006bfdb0 _$E406
// FUNCTION: STBC 0x006bfdc0 _$E405
// FUNCTION: STBC 0x006bfdd0 _$E410
// FUNCTION: STBC 0x006bfde0 _$E409
// FUNCTION: STBC 0x006bfdf0 _$E195
// FUNCTION: STBC 0x006bfe00 _$E194
// FUNCTION: STBC 0x006bfe10 _$E414
// FUNCTION: STBC 0x006bfe20 _$E413
// FUNCTION: STBC 0x006bfe30 _$E200
// FUNCTION: STBC 0x006bfe40 _$E199
// FUNCTION: STBC 0x006bfe50 _$E205
// FUNCTION: STBC 0x006bfe60 _$E204

/*
================
TGDisconnectMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006bf2d0
void TGDisconnectMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGDISCONNECTMESSAGE_TYPE, UnSerialize );
}

/*
================
TGDisconnectMessage::TGDisconnectMessage
================
*/
// FUNCTION: STBC 0x006bf2e0 ??0TGDisconnectMessage@@QAE@XZ
TGDisconnectMessage::TGDisconnectMessage( void ) {
	// BUG: redundant: builds and destroys a second TGMessage that is never used (the base constructor already ran)
	TGMessage();
	guaranteed = true;
	highPriority = true;
}

/*
================
TGDisconnectMessage::~TGDisconnectMessage
================
*/
// FUNCTION: STBC 0x006bf370
TGDisconnectMessage::~TGDisconnectMessage( void ) {
}

/*
================
TGDisconnectMessage::Serialize

Writes the message into buffer; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006bf380
int TGDisconnectMessage::Serialize( char *buffer, int bufferSize ) {
	if ( bufferSize < GetBufferSpaceRequired() ) {
		return 0;
	}

	*buffer++ = VirtualSlot00();
	unsigned short *header = (unsigned short *)buffer;
	buffer += sizeof( unsigned short );
	int size = MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		*(unsigned short *)buffer = sequenceNumber;
		buffer += sizeof( unsigned short );
		size = MESSAGE_HEADER_SIZE + sizeof( unsigned short );
	}
	memcpy( buffer, data, dataLength );
	size += dataLength;

	unsigned short flags = size & 0x3FFF;
	if ( guaranteed ) {
		flags |= MESSAGE_GUARANTEED;
	}
	if ( highPriority ) {
		flags |= MESSAGE_HIGH_PRIORITY;
	}
	*header = flags;
	return size;
}

/*
================
TGDisconnectMessage::UnSerialize

Reads a message Serialize wrote.
================
*/
// FUNCTION: STBC 0x006bf410
TGMessage *TGDisconnectMessage::UnSerialize( char *buffer ) {
	TGDisconnectMessage *message = new TGDisconnectMessage;
	unsigned short sequenceNumber = 0;

	buffer++;
	unsigned short flags = *(unsigned short *)buffer;
	buffer += sizeof( unsigned short );
	bool guaranteed = ( flags >> 15 ) & 1;
	bool highPriority = ( flags >> 14 ) & 1;
	int length = ( flags & 0x3FFF ) - MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		sequenceNumber = *(unsigned short *)buffer;
		buffer += sizeof( unsigned short );
		length -= sizeof( unsigned short );
	}

	message->guaranteed = guaranteed;
	message->highPriority = highPriority;
	message->sequenceNumber = sequenceNumber;
	message->SetData( buffer, length );
	return message;
}

/*
================
TGDisconnectMessage::TGDisconnectMessage
================
*/
// FUNCTION: STBC 0x006bf490 ??0TGDisconnectMessage@@QAE@ABV0@@Z
TGDisconnectMessage::TGDisconnectMessage( const TGDisconnectMessage &message ) : TGMessage( message ) {
}

/*
================
TGDisconnectMessage::Copy
================
*/
// FUNCTION: STBC 0x006bf4b0
TGMessage *TGDisconnectMessage::Copy( void ) {
	return new TGDisconnectMessage( *this );
}

/*
================
TGDisconnectMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006bf4e0
int TGDisconnectMessage::GetBufferSpaceRequired( void ) {
	int size = dataLength + MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		size += sizeof( unsigned short );
	}
	return size;
}
