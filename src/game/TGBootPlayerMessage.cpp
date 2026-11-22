/*
===============================================================================

	TGBootPlayerMessage.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGBootPlayerMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bb040 _$E422
// FUNCTION: STBC 0x006bb060 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006baec0 _$E32
// FUNCTION: STBC 0x006baed0 _$E29
// FUNCTION: STBC 0x006baee0 _$E31
// FUNCTION: STBC 0x006baef0 _$E30
// FUNCTION: STBC 0x006baf00 _$E38
// FUNCTION: STBC 0x006baf10 _$E35
// FUNCTION: STBC 0x006baf20 _$E37
// FUNCTION: STBC 0x006baf30 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006baf40 _$E59
// FUNCTION: STBC 0x006baf50 _$E58
// FUNCTION: STBC 0x006baf60 _$E63
// FUNCTION: STBC 0x006baf70 _$E62
// FUNCTION: STBC 0x006baf80 _$E74
// FUNCTION: STBC 0x006baf90 _$E73
// FUNCTION: STBC 0x006bafa0 _$E78
// FUNCTION: STBC 0x006bafb0 _$E77
// FUNCTION: STBC 0x006bafc0 _$E82
// FUNCTION: STBC 0x006bafd0 _$E81
// FUNCTION: STBC 0x006bafe0 _$E86
// FUNCTION: STBC 0x006baff0 _$E85
// FUNCTION: STBC 0x006bb000 _$E210
// FUNCTION: STBC 0x006bb010 _$E209
// FUNCTION: STBC 0x006bb020 _$E152
// FUNCTION: STBC 0x006bb030 _$E151
// FUNCTION: STBC 0x006bb070 _$E165
// FUNCTION: STBC 0x006bb080 _$E164
// FUNCTION: STBC 0x006bb090 _$E215
// FUNCTION: STBC 0x006bb0a0 _$E214
// FUNCTION: STBC 0x006bb0b0 _$E220
// FUNCTION: STBC 0x006bb0c0 _$E219
// FUNCTION: STBC 0x006bb0d0 _$E224
// FUNCTION: STBC 0x006bb0e0 _$E223
// FUNCTION: STBC 0x006bb0f0 _$E228
// FUNCTION: STBC 0x006bb100 _$E227
// FUNCTION: STBC 0x006bb110 _$E235
// FUNCTION: STBC 0x006bb120 _$E234
// FUNCTION: STBC 0x006bb130 _$E260
// FUNCTION: STBC 0x006bb140 _$E259
// FUNCTION: STBC 0x006bb150 _$E264
// FUNCTION: STBC 0x006bb160 _$E263
// FUNCTION: STBC 0x006bb170 _$E110
// FUNCTION: STBC 0x006bb180 _$E109
// FUNCTION: STBC 0x006bb190 _$E246
// FUNCTION: STBC 0x006bb1a0 _$E245
// FUNCTION: STBC 0x006bb1b0 _$E253
// FUNCTION: STBC 0x006bb1c0 _$E252
// FUNCTION: STBC 0x006bb1d0 _$E276
// FUNCTION: STBC 0x006bb1e0 _$E275
// FUNCTION: STBC 0x006bb1f0 _$E280
// FUNCTION: STBC 0x006bb200 _$E279
// FUNCTION: STBC 0x006bb210 _$E320
// FUNCTION: STBC 0x006bb220 _$E319
// FUNCTION: STBC 0x006bb230 _$E326
// FUNCTION: STBC 0x006bb240 _$E325
// FUNCTION: STBC 0x006bb250 _$E268
// FUNCTION: STBC 0x006bb260 _$E267
// FUNCTION: STBC 0x006bb270 _$E272
// FUNCTION: STBC 0x006bb280 _$E271
// FUNCTION: STBC 0x006bb290 _$E284
// FUNCTION: STBC 0x006bb2a0 _$E283
// FUNCTION: STBC 0x006bb2b0 _$E288
// FUNCTION: STBC 0x006bb2c0 _$E287
// FUNCTION: STBC 0x006bb2d0 _$E292
// FUNCTION: STBC 0x006bb2e0 _$E291
// FUNCTION: STBC 0x006bb2f0 _$E296
// FUNCTION: STBC 0x006bb300 _$E295
// FUNCTION: STBC 0x006bb310 _$E300
// FUNCTION: STBC 0x006bb320 _$E299
// FUNCTION: STBC 0x006bb330 _$E304
// FUNCTION: STBC 0x006bb340 _$E303
// FUNCTION: STBC 0x006bb350 _$E308
// FUNCTION: STBC 0x006bb360 _$E307
// FUNCTION: STBC 0x006bb370 _$E312
// FUNCTION: STBC 0x006bb380 _$E311
// FUNCTION: STBC 0x006bb390 _$E115
// FUNCTION: STBC 0x006bb3a0 _$E114
// FUNCTION: STBC 0x006bb3b0 _$E316
// FUNCTION: STBC 0x006bb3c0 _$E315
// FUNCTION: STBC 0x006bb3d0 _$E120
// FUNCTION: STBC 0x006bb3e0 _$E119
// FUNCTION: STBC 0x006bb3f0 _$E330
// FUNCTION: STBC 0x006bb400 _$E329
// FUNCTION: STBC 0x006bb410 _$E334
// FUNCTION: STBC 0x006bb420 _$E333
// FUNCTION: STBC 0x006bb430 _$E338
// FUNCTION: STBC 0x006bb440 _$E337
// FUNCTION: STBC 0x006bb450 _$E342
// FUNCTION: STBC 0x006bb460 _$E341
// FUNCTION: STBC 0x006bb470 _$E129
// FUNCTION: STBC 0x006bb480 _$E128
// FUNCTION: STBC 0x006bb490 _$E346
// FUNCTION: STBC 0x006bb4a0 _$E345
// FUNCTION: STBC 0x006bb4b0 _$E350
// FUNCTION: STBC 0x006bb4c0 _$E349
// FUNCTION: STBC 0x006bb4d0 _$E354
// FUNCTION: STBC 0x006bb4e0 _$E353
// FUNCTION: STBC 0x006bb4f0 _$E135
// FUNCTION: STBC 0x006bb500 _$E134
// FUNCTION: STBC 0x006bb510 _$E140
// FUNCTION: STBC 0x006bb520 _$E139
// FUNCTION: STBC 0x006bb530 _$E170
// FUNCTION: STBC 0x006bb540 _$E169
// FUNCTION: STBC 0x006bb550 _$E358
// FUNCTION: STBC 0x006bb560 _$E357
// FUNCTION: STBC 0x006bb570 _$E175
// FUNCTION: STBC 0x006bb580 _$E174
// FUNCTION: STBC 0x006bb590 _$E362
// FUNCTION: STBC 0x006bb5a0 _$E361
// FUNCTION: STBC 0x006bb5b0 _$E366
// FUNCTION: STBC 0x006bb5c0 _$E365
// FUNCTION: STBC 0x006bb5d0 _$E180
// FUNCTION: STBC 0x006bb5e0 _$E179
// FUNCTION: STBC 0x006bb5f0 _$E370
// FUNCTION: STBC 0x006bb600 _$E369
// FUNCTION: STBC 0x006bb610 _$E145
// FUNCTION: STBC 0x006bb620 _$E144
// FUNCTION: STBC 0x006bb630 _$E374
// FUNCTION: STBC 0x006bb640 _$E373
// FUNCTION: STBC 0x006bb650 _$E378
// FUNCTION: STBC 0x006bb660 _$E377
// FUNCTION: STBC 0x006bb670 _$E185
// FUNCTION: STBC 0x006bb680 _$E184
// FUNCTION: STBC 0x006bb690 _$E190
// FUNCTION: STBC 0x006bb6a0 _$E189
// FUNCTION: STBC 0x006bb6b0 _$E382
// FUNCTION: STBC 0x006bb6c0 _$E381
// FUNCTION: STBC 0x006bb6d0 _$E386
// FUNCTION: STBC 0x006bb6e0 _$E385
// FUNCTION: STBC 0x006bb6f0 _$E390
// FUNCTION: STBC 0x006bb700 _$E389
// FUNCTION: STBC 0x006bb710 _$E394
// FUNCTION: STBC 0x006bb720 _$E393
// FUNCTION: STBC 0x006bb730 _$E398
// FUNCTION: STBC 0x006bb740 _$E397
// FUNCTION: STBC 0x006bb750 _$E402
// FUNCTION: STBC 0x006bb760 _$E401
// FUNCTION: STBC 0x006bb770 _$E406
// FUNCTION: STBC 0x006bb780 _$E405
// FUNCTION: STBC 0x006bb790 _$E410
// FUNCTION: STBC 0x006bb7a0 _$E409
// FUNCTION: STBC 0x006bb7b0 _$E195
// FUNCTION: STBC 0x006bb7c0 _$E194
// FUNCTION: STBC 0x006bb7d0 _$E414
// FUNCTION: STBC 0x006bb7e0 _$E413
// FUNCTION: STBC 0x006bb7f0 _$E200
// FUNCTION: STBC 0x006bb800 _$E199
// FUNCTION: STBC 0x006bb810 _$E205
// FUNCTION: STBC 0x006bb820 _$E204

/*
================
TGBootPlayerMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006bac60
void TGBootPlayerMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGBOOTPLAYERMESSAGE_TYPE, UnSerialize );
}

/*
================
TGBootPlayerMessage::TGBootPlayerMessage
================
*/
// FUNCTION: STBC 0x006bac70 ??0TGBootPlayerMessage@@QAE@XZ
TGBootPlayerMessage::TGBootPlayerMessage( void ) {
	// BUG: redundant: builds and destroys a second TGMessage that is never used (the base constructor already ran)
	TGMessage();
	guaranteed = true;
	highPriority = true;
	bootReason = 0;
}

/*
================
TGBootPlayerMessage::~TGBootPlayerMessage
================
*/
// FUNCTION: STBC 0x006bad10
TGBootPlayerMessage::~TGBootPlayerMessage( void ) {
}

/*
================
TGBootPlayerMessage::Serialize

Writes the message into buffer, the boot reason before the data; 0 when it
doesn't fit.
================
*/
// FUNCTION: STBC 0x006bad20
int TGBootPlayerMessage::Serialize( char *buffer, int bufferSize ) {
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
	*buffer++ = bootReason;
	memcpy( buffer, data, dataLength );
	size += dataLength + 1;

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
TGBootPlayerMessage::UnSerialize

Reads a message Serialize wrote: the boot reason is the first data byte.
================
*/
// FUNCTION: STBC 0x006badb0
// PARKED: message and buffer pointers get swapped registers (esi/edi); rest identical
TGMessage *TGBootPlayerMessage::UnSerialize( char *buffer ) {
	TGBootPlayerMessage *message = new TGBootPlayerMessage;
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

	int reason = *buffer++;
	length--;
	message->guaranteed = guaranteed;
	message->bootReason = reason;
	message->highPriority = highPriority;
	message->sequenceNumber = sequenceNumber;
	message->SetData( buffer, length );
	return message;
}

/*
================
TGBootPlayerMessage::TGBootPlayerMessage
================
*/
// FUNCTION: STBC 0x006bae40 ??0TGBootPlayerMessage@@QAE@ABV0@@Z
TGBootPlayerMessage::TGBootPlayerMessage( const TGBootPlayerMessage &message ) : TGMessage( message ) {
}

/*
================
TGBootPlayerMessage::Copy
================
*/
// FUNCTION: STBC 0x006bae60
TGMessage *TGBootPlayerMessage::Copy( void ) {
	TGBootPlayerMessage *message = new TGBootPlayerMessage( *this );
	message->bootReason = bootReason;
	return message;
}

/*
================
TGBootPlayerMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006baea0
int TGBootPlayerMessage::GetBufferSpaceRequired( void ) {
	int size = dataLength + MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		size += sizeof( unsigned short );
	}
	return size + 1;
}
