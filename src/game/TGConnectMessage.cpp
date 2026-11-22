/*
===============================================================================

	TGConnectMessage.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGConnectMessage.h"
#include "TGNetwork.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006bead0 _$E422
// FUNCTION: STBC 0x006beaf0 _$E421

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006be950 _$E32
// FUNCTION: STBC 0x006be960 _$E29
// FUNCTION: STBC 0x006be970 _$E31
// FUNCTION: STBC 0x006be980 _$E30
// FUNCTION: STBC 0x006be990 _$E38
// FUNCTION: STBC 0x006be9a0 _$E35
// FUNCTION: STBC 0x006be9b0 _$E37
// FUNCTION: STBC 0x006be9c0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006be9d0 _$E59
// FUNCTION: STBC 0x006be9e0 _$E58
// FUNCTION: STBC 0x006be9f0 _$E63
// FUNCTION: STBC 0x006bea00 _$E62
// FUNCTION: STBC 0x006bea10 _$E74
// FUNCTION: STBC 0x006bea20 _$E73
// FUNCTION: STBC 0x006bea30 _$E78
// FUNCTION: STBC 0x006bea40 _$E77
// FUNCTION: STBC 0x006bea50 _$E82
// FUNCTION: STBC 0x006bea60 _$E81
// FUNCTION: STBC 0x006bea70 _$E86
// FUNCTION: STBC 0x006bea80 _$E85
// FUNCTION: STBC 0x006bea90 _$E210
// FUNCTION: STBC 0x006beaa0 _$E209
// FUNCTION: STBC 0x006beab0 _$E152
// FUNCTION: STBC 0x006beac0 _$E151
// FUNCTION: STBC 0x006beb00 _$E165
// FUNCTION: STBC 0x006beb10 _$E164
// FUNCTION: STBC 0x006beb20 _$E215
// FUNCTION: STBC 0x006beb30 _$E214
// FUNCTION: STBC 0x006beb40 _$E220
// FUNCTION: STBC 0x006beb50 _$E219
// FUNCTION: STBC 0x006beb60 _$E224
// FUNCTION: STBC 0x006beb70 _$E223
// FUNCTION: STBC 0x006beb80 _$E228
// FUNCTION: STBC 0x006beb90 _$E227
// FUNCTION: STBC 0x006beba0 _$E235
// FUNCTION: STBC 0x006bebb0 _$E234
// FUNCTION: STBC 0x006bebc0 _$E260
// FUNCTION: STBC 0x006bebd0 _$E259
// FUNCTION: STBC 0x006bebe0 _$E264
// FUNCTION: STBC 0x006bebf0 _$E263
// FUNCTION: STBC 0x006bec00 _$E110
// FUNCTION: STBC 0x006bec10 _$E109
// FUNCTION: STBC 0x006bec20 _$E246
// FUNCTION: STBC 0x006bec30 _$E245
// FUNCTION: STBC 0x006bec40 _$E253
// FUNCTION: STBC 0x006bec50 _$E252
// FUNCTION: STBC 0x006bec60 _$E276
// FUNCTION: STBC 0x006bec70 _$E275
// FUNCTION: STBC 0x006bec80 _$E280
// FUNCTION: STBC 0x006bec90 _$E279
// FUNCTION: STBC 0x006beca0 _$E320
// FUNCTION: STBC 0x006becb0 _$E319
// FUNCTION: STBC 0x006becc0 _$E326
// FUNCTION: STBC 0x006becd0 _$E325
// FUNCTION: STBC 0x006bece0 _$E268
// FUNCTION: STBC 0x006becf0 _$E267
// FUNCTION: STBC 0x006bed00 _$E272
// FUNCTION: STBC 0x006bed10 _$E271
// FUNCTION: STBC 0x006bed20 _$E284
// FUNCTION: STBC 0x006bed30 _$E283
// FUNCTION: STBC 0x006bed40 _$E288
// FUNCTION: STBC 0x006bed50 _$E287
// FUNCTION: STBC 0x006bed60 _$E292
// FUNCTION: STBC 0x006bed70 _$E291
// FUNCTION: STBC 0x006bed80 _$E296
// FUNCTION: STBC 0x006bed90 _$E295
// FUNCTION: STBC 0x006beda0 _$E300
// FUNCTION: STBC 0x006bedb0 _$E299
// FUNCTION: STBC 0x006bedc0 _$E304
// FUNCTION: STBC 0x006bedd0 _$E303
// FUNCTION: STBC 0x006bede0 _$E308
// FUNCTION: STBC 0x006bedf0 _$E307
// FUNCTION: STBC 0x006bee00 _$E312
// FUNCTION: STBC 0x006bee10 _$E311
// FUNCTION: STBC 0x006bee20 _$E115
// FUNCTION: STBC 0x006bee30 _$E114
// FUNCTION: STBC 0x006bee40 _$E316
// FUNCTION: STBC 0x006bee50 _$E315
// FUNCTION: STBC 0x006bee60 _$E120
// FUNCTION: STBC 0x006bee70 _$E119
// FUNCTION: STBC 0x006bee80 _$E330
// FUNCTION: STBC 0x006bee90 _$E329
// FUNCTION: STBC 0x006beea0 _$E334
// FUNCTION: STBC 0x006beeb0 _$E333
// FUNCTION: STBC 0x006beec0 _$E338
// FUNCTION: STBC 0x006beed0 _$E337
// FUNCTION: STBC 0x006beee0 _$E342
// FUNCTION: STBC 0x006beef0 _$E341
// FUNCTION: STBC 0x006bef00 _$E129
// FUNCTION: STBC 0x006bef10 _$E128
// FUNCTION: STBC 0x006bef20 _$E346
// FUNCTION: STBC 0x006bef30 _$E345
// FUNCTION: STBC 0x006bef40 _$E350
// FUNCTION: STBC 0x006bef50 _$E349
// FUNCTION: STBC 0x006bef60 _$E354
// FUNCTION: STBC 0x006bef70 _$E353
// FUNCTION: STBC 0x006bef80 _$E135
// FUNCTION: STBC 0x006bef90 _$E134
// FUNCTION: STBC 0x006befa0 _$E140
// FUNCTION: STBC 0x006befb0 _$E139
// FUNCTION: STBC 0x006befc0 _$E170
// FUNCTION: STBC 0x006befd0 _$E169
// FUNCTION: STBC 0x006befe0 _$E358
// FUNCTION: STBC 0x006beff0 _$E357
// FUNCTION: STBC 0x006bf000 _$E175
// FUNCTION: STBC 0x006bf010 _$E174
// FUNCTION: STBC 0x006bf020 _$E362
// FUNCTION: STBC 0x006bf030 _$E361
// FUNCTION: STBC 0x006bf040 _$E366
// FUNCTION: STBC 0x006bf050 _$E365
// FUNCTION: STBC 0x006bf060 _$E180
// FUNCTION: STBC 0x006bf070 _$E179
// FUNCTION: STBC 0x006bf080 _$E370
// FUNCTION: STBC 0x006bf090 _$E369
// FUNCTION: STBC 0x006bf0a0 _$E145
// FUNCTION: STBC 0x006bf0b0 _$E144
// FUNCTION: STBC 0x006bf0c0 _$E374
// FUNCTION: STBC 0x006bf0d0 _$E373
// FUNCTION: STBC 0x006bf0e0 _$E378
// FUNCTION: STBC 0x006bf0f0 _$E377
// FUNCTION: STBC 0x006bf100 _$E185
// FUNCTION: STBC 0x006bf110 _$E184
// FUNCTION: STBC 0x006bf120 _$E190
// FUNCTION: STBC 0x006bf130 _$E189
// FUNCTION: STBC 0x006bf140 _$E382
// FUNCTION: STBC 0x006bf150 _$E381
// FUNCTION: STBC 0x006bf160 _$E386
// FUNCTION: STBC 0x006bf170 _$E385
// FUNCTION: STBC 0x006bf180 _$E390
// FUNCTION: STBC 0x006bf190 _$E389
// FUNCTION: STBC 0x006bf1a0 _$E394
// FUNCTION: STBC 0x006bf1b0 _$E393
// FUNCTION: STBC 0x006bf1c0 _$E398
// FUNCTION: STBC 0x006bf1d0 _$E397
// FUNCTION: STBC 0x006bf1e0 _$E402
// FUNCTION: STBC 0x006bf1f0 _$E401
// FUNCTION: STBC 0x006bf200 _$E406
// FUNCTION: STBC 0x006bf210 _$E405
// FUNCTION: STBC 0x006bf220 _$E410
// FUNCTION: STBC 0x006bf230 _$E409
// FUNCTION: STBC 0x006bf240 _$E195
// FUNCTION: STBC 0x006bf250 _$E194
// FUNCTION: STBC 0x006bf260 _$E414
// FUNCTION: STBC 0x006bf270 _$E413
// FUNCTION: STBC 0x006bf280 _$E200
// FUNCTION: STBC 0x006bf290 _$E199
// FUNCTION: STBC 0x006bf2a0 _$E205
// FUNCTION: STBC 0x006bf2b0 _$E204

/*
================
TGConnectMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006be720
void TGConnectMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGCONNECTMESSAGE_TYPE, UnSerialize );
}

/*
================
TGConnectMessage::TGConnectMessage
================
*/
// FUNCTION: STBC 0x006be730 ??0TGConnectMessage@@QAE@XZ
TGConnectMessage::TGConnectMessage( void ) {
	// BUG: redundant: builds and destroys a second TGMessage that is never used (the base constructor already ran)
	TGMessage();
	guaranteed = true;
	highPriority = true;
}

/*
================
TGConnectMessage::~TGConnectMessage
================
*/
// FUNCTION: STBC 0x006be7c0
TGConnectMessage::~TGConnectMessage( void ) {
}

/*
================
TGConnectMessage::Serialize

Writes the message into buffer; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006be7d0
int TGConnectMessage::Serialize( char *buffer, int bufferSize ) {
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
TGConnectMessage::UnSerialize

Reads a message Serialize wrote.
================
*/
// FUNCTION: STBC 0x006be860
TGMessage *TGConnectMessage::UnSerialize( char *buffer ) {
	TGConnectMessage *message = new TGConnectMessage;
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
TGConnectMessage::TGConnectMessage
================
*/
// FUNCTION: STBC 0x006be8e0 ??0TGConnectMessage@@QAE@ABV0@@Z
TGConnectMessage::TGConnectMessage( const TGConnectMessage &message ) : TGMessage( message ) {
}

/*
================
TGConnectMessage::Copy
================
*/
// FUNCTION: STBC 0x006be900
TGMessage *TGConnectMessage::Copy( void ) {
	return new TGConnectMessage( *this );
}

/*
================
TGConnectMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006be930
int TGConnectMessage::GetBufferSpaceRequired( void ) {
	int size = dataLength + MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		size += sizeof( unsigned short );
	}
	return size;
}
