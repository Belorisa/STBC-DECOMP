/*
===============================================================================

	TGNameChangeMessage.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGNameChangeMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bc910 _$E422
// FUNCTION: STBC 0x006bc930 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006bc790 _$E32
// FUNCTION: STBC 0x006bc7a0 _$E29
// FUNCTION: STBC 0x006bc7b0 _$E31
// FUNCTION: STBC 0x006bc7c0 _$E30
// FUNCTION: STBC 0x006bc7d0 _$E38
// FUNCTION: STBC 0x006bc7e0 _$E35
// FUNCTION: STBC 0x006bc7f0 _$E37
// FUNCTION: STBC 0x006bc800 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006bc810 _$E59
// FUNCTION: STBC 0x006bc820 _$E58
// FUNCTION: STBC 0x006bc830 _$E63
// FUNCTION: STBC 0x006bc840 _$E62
// FUNCTION: STBC 0x006bc850 _$E74
// FUNCTION: STBC 0x006bc860 _$E73
// FUNCTION: STBC 0x006bc870 _$E78
// FUNCTION: STBC 0x006bc880 _$E77
// FUNCTION: STBC 0x006bc890 _$E82
// FUNCTION: STBC 0x006bc8a0 _$E81
// FUNCTION: STBC 0x006bc8b0 _$E86
// FUNCTION: STBC 0x006bc8c0 _$E85
// FUNCTION: STBC 0x006bc8d0 _$E210
// FUNCTION: STBC 0x006bc8e0 _$E209
// FUNCTION: STBC 0x006bc8f0 _$E152
// FUNCTION: STBC 0x006bc900 _$E151
// FUNCTION: STBC 0x006bc940 _$E165
// FUNCTION: STBC 0x006bc950 _$E164
// FUNCTION: STBC 0x006bc960 _$E215
// FUNCTION: STBC 0x006bc970 _$E214
// FUNCTION: STBC 0x006bc980 _$E220
// FUNCTION: STBC 0x006bc990 _$E219
// FUNCTION: STBC 0x006bc9a0 _$E224
// FUNCTION: STBC 0x006bc9b0 _$E223
// FUNCTION: STBC 0x006bc9c0 _$E228
// FUNCTION: STBC 0x006bc9d0 _$E227
// FUNCTION: STBC 0x006bc9e0 _$E235
// FUNCTION: STBC 0x006bc9f0 _$E234
// FUNCTION: STBC 0x006bca00 _$E260
// FUNCTION: STBC 0x006bca10 _$E259
// FUNCTION: STBC 0x006bca20 _$E264
// FUNCTION: STBC 0x006bca30 _$E263
// FUNCTION: STBC 0x006bca40 _$E110
// FUNCTION: STBC 0x006bca50 _$E109
// FUNCTION: STBC 0x006bca60 _$E246
// FUNCTION: STBC 0x006bca70 _$E245
// FUNCTION: STBC 0x006bca80 _$E253
// FUNCTION: STBC 0x006bca90 _$E252
// FUNCTION: STBC 0x006bcaa0 _$E276
// FUNCTION: STBC 0x006bcab0 _$E275
// FUNCTION: STBC 0x006bcac0 _$E280
// FUNCTION: STBC 0x006bcad0 _$E279
// FUNCTION: STBC 0x006bcae0 _$E320
// FUNCTION: STBC 0x006bcaf0 _$E319
// FUNCTION: STBC 0x006bcb00 _$E326
// FUNCTION: STBC 0x006bcb10 _$E325
// FUNCTION: STBC 0x006bcb20 _$E268
// FUNCTION: STBC 0x006bcb30 _$E267
// FUNCTION: STBC 0x006bcb40 _$E272
// FUNCTION: STBC 0x006bcb50 _$E271
// FUNCTION: STBC 0x006bcb60 _$E284
// FUNCTION: STBC 0x006bcb70 _$E283
// FUNCTION: STBC 0x006bcb80 _$E288
// FUNCTION: STBC 0x006bcb90 _$E287
// FUNCTION: STBC 0x006bcba0 _$E292
// FUNCTION: STBC 0x006bcbb0 _$E291
// FUNCTION: STBC 0x006bcbc0 _$E296
// FUNCTION: STBC 0x006bcbd0 _$E295
// FUNCTION: STBC 0x006bcbe0 _$E300
// FUNCTION: STBC 0x006bcbf0 _$E299
// FUNCTION: STBC 0x006bcc00 _$E304
// FUNCTION: STBC 0x006bcc10 _$E303
// FUNCTION: STBC 0x006bcc20 _$E308
// FUNCTION: STBC 0x006bcc30 _$E307
// FUNCTION: STBC 0x006bcc40 _$E312
// FUNCTION: STBC 0x006bcc50 _$E311
// FUNCTION: STBC 0x006bcc60 _$E115
// FUNCTION: STBC 0x006bcc70 _$E114
// FUNCTION: STBC 0x006bcc80 _$E316
// FUNCTION: STBC 0x006bcc90 _$E315
// FUNCTION: STBC 0x006bcca0 _$E120
// FUNCTION: STBC 0x006bccb0 _$E119
// FUNCTION: STBC 0x006bccc0 _$E330
// FUNCTION: STBC 0x006bccd0 _$E329
// FUNCTION: STBC 0x006bcce0 _$E334
// FUNCTION: STBC 0x006bccf0 _$E333
// FUNCTION: STBC 0x006bcd00 _$E338
// FUNCTION: STBC 0x006bcd10 _$E337
// FUNCTION: STBC 0x006bcd20 _$E342
// FUNCTION: STBC 0x006bcd30 _$E341
// FUNCTION: STBC 0x006bcd40 _$E129
// FUNCTION: STBC 0x006bcd50 _$E128
// FUNCTION: STBC 0x006bcd60 _$E346
// FUNCTION: STBC 0x006bcd70 _$E345
// FUNCTION: STBC 0x006bcd80 _$E350
// FUNCTION: STBC 0x006bcd90 _$E349
// FUNCTION: STBC 0x006bcda0 _$E354
// FUNCTION: STBC 0x006bcdb0 _$E353
// FUNCTION: STBC 0x006bcdc0 _$E135
// FUNCTION: STBC 0x006bcdd0 _$E134
// FUNCTION: STBC 0x006bcde0 _$E140
// FUNCTION: STBC 0x006bcdf0 _$E139
// FUNCTION: STBC 0x006bce00 _$E170
// FUNCTION: STBC 0x006bce10 _$E169
// FUNCTION: STBC 0x006bce20 _$E358
// FUNCTION: STBC 0x006bce30 _$E357
// FUNCTION: STBC 0x006bce40 _$E175
// FUNCTION: STBC 0x006bce50 _$E174
// FUNCTION: STBC 0x006bce60 _$E362
// FUNCTION: STBC 0x006bce70 _$E361
// FUNCTION: STBC 0x006bce80 _$E366
// FUNCTION: STBC 0x006bce90 _$E365
// FUNCTION: STBC 0x006bcea0 _$E180
// FUNCTION: STBC 0x006bceb0 _$E179
// FUNCTION: STBC 0x006bcec0 _$E370
// FUNCTION: STBC 0x006bced0 _$E369
// FUNCTION: STBC 0x006bcee0 _$E145
// FUNCTION: STBC 0x006bcef0 _$E144
// FUNCTION: STBC 0x006bcf00 _$E374
// FUNCTION: STBC 0x006bcf10 _$E373
// FUNCTION: STBC 0x006bcf20 _$E378
// FUNCTION: STBC 0x006bcf30 _$E377
// FUNCTION: STBC 0x006bcf40 _$E185
// FUNCTION: STBC 0x006bcf50 _$E184
// FUNCTION: STBC 0x006bcf60 _$E190
// FUNCTION: STBC 0x006bcf70 _$E189
// FUNCTION: STBC 0x006bcf80 _$E382
// FUNCTION: STBC 0x006bcf90 _$E381
// FUNCTION: STBC 0x006bcfa0 _$E386
// FUNCTION: STBC 0x006bcfb0 _$E385
// FUNCTION: STBC 0x006bcfc0 _$E390
// FUNCTION: STBC 0x006bcfd0 _$E389
// FUNCTION: STBC 0x006bcfe0 _$E394
// FUNCTION: STBC 0x006bcff0 _$E393
// FUNCTION: STBC 0x006bd000 _$E398
// FUNCTION: STBC 0x006bd010 _$E397
// FUNCTION: STBC 0x006bd020 _$E402
// FUNCTION: STBC 0x006bd030 _$E401
// FUNCTION: STBC 0x006bd040 _$E406
// FUNCTION: STBC 0x006bd050 _$E405
// FUNCTION: STBC 0x006bd060 _$E410
// FUNCTION: STBC 0x006bd070 _$E409
// FUNCTION: STBC 0x006bd080 _$E195
// FUNCTION: STBC 0x006bd090 _$E194
// FUNCTION: STBC 0x006bd0a0 _$E414
// FUNCTION: STBC 0x006bd0b0 _$E413
// FUNCTION: STBC 0x006bd0c0 _$E200
// FUNCTION: STBC 0x006bd0d0 _$E199
// FUNCTION: STBC 0x006bd0e0 _$E205
// FUNCTION: STBC 0x006bd0f0 _$E204

// The size field of a message without the multi-part flag.
#define NAMECHANGE_SIZE_MASK	0x3FFF

/*
================
TGNameChangeMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006bc5a0
void TGNameChangeMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGNAMECHANGEMESSAGE_TYPE, UnSerialize );
}

/*
================
TGNameChangeMessage::TGNameChangeMessage
================
*/
// FUNCTION: STBC 0x006bc5b0 ??0TGNameChangeMessage@@QAE@XZ
TGNameChangeMessage::TGNameChangeMessage( void ) {
	guaranteed = true;
	highPriority = true;
}

/*
================
TGNameChangeMessage::~TGNameChangeMessage
================
*/
// FUNCTION: STBC 0x006bc600
// FUNCTION: STBC 0x006bc5d0 ??_GTGNameChangeMessage@@UAEPAXI@Z
TGNameChangeMessage::~TGNameChangeMessage( void ) {
}

/*
================
TGNameChangeMessage::Serialize

Writes the message into buffer; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006bc610
int TGNameChangeMessage::Serialize( char *buffer, int bufferSize ) {
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

	unsigned short flags = size & NAMECHANGE_SIZE_MASK;
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
TGNameChangeMessage::UnSerialize

Reads a message Serialize wrote.
================
*/
// FUNCTION: STBC 0x006bc6a0
TGMessage *TGNameChangeMessage::UnSerialize( char *buffer ) {
	TGNameChangeMessage *message = new TGNameChangeMessage;
	unsigned short sequenceNumber = 0;

	buffer++;
	unsigned short flags = *(unsigned short *)buffer;
	buffer += sizeof( unsigned short );
	bool guaranteed = ( flags >> 15 ) & 1;
	bool highPriority = ( flags >> 14 ) & 1;
	int length = ( flags & NAMECHANGE_SIZE_MASK ) - MESSAGE_HEADER_SIZE;
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
TGNameChangeMessage::TGNameChangeMessage
================
*/
// FUNCTION: STBC 0x006bc720 ??0TGNameChangeMessage@@QAE@ABV0@@Z
TGNameChangeMessage::TGNameChangeMessage( const TGNameChangeMessage &message ) : TGMessage( message ) {
}

/*
================
TGNameChangeMessage::Copy
================
*/
// FUNCTION: STBC 0x006bc740
TGMessage *TGNameChangeMessage::Copy( void ) {
	return new TGNameChangeMessage( *this );
}

/*
================
TGNameChangeMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006bc770
int TGNameChangeMessage::GetBufferSpaceRequired( void ) {
	int size = dataLength + MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		size += sizeof( unsigned short );
	}
	return size;
}
