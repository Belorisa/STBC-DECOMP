/*
===============================================================================

	TGMessage.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGMessage.h"
#include "TGBufferStream.h"
#include "TGNetwork.h"
#include "TGSystemWrapperClass.h"
#include "NiTList.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b8c30 _$E428
// FUNCTION: STBC 0x006b8c50 _$E427

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006b8a30 _$E32
// FUNCTION: STBC 0x006b8a40 _$E29
// FUNCTION: STBC 0x006b8a50 _$E31
// FUNCTION: STBC 0x006b8a60 _$E30
// FUNCTION: STBC 0x006b8a70 _$E38
// FUNCTION: STBC 0x006b8a80 _$E35
// FUNCTION: STBC 0x006b8a90 _$E37
// FUNCTION: STBC 0x006b8aa0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006b8ab0 _$E59
// FUNCTION: STBC 0x006b8ac0 _$E58
// FUNCTION: STBC 0x006b8ad0 _$E63
// FUNCTION: STBC 0x006b8ae0 _$E62
// FUNCTION: STBC 0x006b8af0 _$E74
// FUNCTION: STBC 0x006b8b00 _$E73
// FUNCTION: STBC 0x006b8b10 _$E78
// FUNCTION: STBC 0x006b8b20 _$E77
// FUNCTION: STBC 0x006b8b30 _$E82
// FUNCTION: STBC 0x006b8b40 _$E81
// FUNCTION: STBC 0x006b8b50 _$E86
// FUNCTION: STBC 0x006b8b60 _$E85
// FUNCTION: STBC 0x006b8b70 _$E210
// FUNCTION: STBC 0x006b8b80 _$E209
// FUNCTION: STBC 0x006b8bd0 _$E152
// FUNCTION: STBC 0x006b8be0 _$E151
// FUNCTION: STBC 0x006b8bf0 _$E165
// FUNCTION: STBC 0x006b8c00 _$E164
// FUNCTION: STBC 0x006b8c10 _$E215
// FUNCTION: STBC 0x006b8c20 _$E214
// FUNCTION: STBC 0x006b8cb0 _$E220
// FUNCTION: STBC 0x006b8cc0 _$E219
// FUNCTION: STBC 0x006b8cd0 _$E224
// FUNCTION: STBC 0x006b8ce0 _$E223
// FUNCTION: STBC 0x006b8cf0 _$E228
// FUNCTION: STBC 0x006b8d00 _$E227
// FUNCTION: STBC 0x006b8d10 _$E235
// FUNCTION: STBC 0x006b8d20 _$E234
// FUNCTION: STBC 0x006b8d30 _$E260
// FUNCTION: STBC 0x006b8d40 _$E259
// FUNCTION: STBC 0x006b8d50 _$E264
// FUNCTION: STBC 0x006b8d60 _$E263
// FUNCTION: STBC 0x006b8d70 _$E110
// FUNCTION: STBC 0x006b8d80 _$E109
// FUNCTION: STBC 0x006b8d90 _$E246
// FUNCTION: STBC 0x006b8da0 _$E245
// FUNCTION: STBC 0x006b8db0 _$E253
// FUNCTION: STBC 0x006b8dc0 _$E252
// FUNCTION: STBC 0x006b8dd0 _$E276
// FUNCTION: STBC 0x006b8de0 _$E275
// FUNCTION: STBC 0x006b8df0 _$E280
// FUNCTION: STBC 0x006b8e00 _$E279
// FUNCTION: STBC 0x006b8e10 _$E320
// FUNCTION: STBC 0x006b8e20 _$E319
// FUNCTION: STBC 0x006b8e30 _$E326
// FUNCTION: STBC 0x006b8e40 _$E325
// FUNCTION: STBC 0x006b8e50 _$E268
// FUNCTION: STBC 0x006b8e60 _$E267
// FUNCTION: STBC 0x006b8e70 _$E272
// FUNCTION: STBC 0x006b8e80 _$E271
// FUNCTION: STBC 0x006b8e90 _$E284
// FUNCTION: STBC 0x006b8ea0 _$E283
// FUNCTION: STBC 0x006b8eb0 _$E288
// FUNCTION: STBC 0x006b8ec0 _$E287
// FUNCTION: STBC 0x006b8ed0 _$E292
// FUNCTION: STBC 0x006b8ee0 _$E291
// FUNCTION: STBC 0x006b8ef0 _$E296
// FUNCTION: STBC 0x006b8f00 _$E295
// FUNCTION: STBC 0x006b8f10 _$E300
// FUNCTION: STBC 0x006b8f20 _$E299
// FUNCTION: STBC 0x006b8f30 _$E304
// FUNCTION: STBC 0x006b8f40 _$E303
// FUNCTION: STBC 0x006b8f50 _$E308
// FUNCTION: STBC 0x006b8f60 _$E307
// FUNCTION: STBC 0x006b8f70 _$E312
// FUNCTION: STBC 0x006b8f80 _$E311
// FUNCTION: STBC 0x006b8f90 _$E115
// FUNCTION: STBC 0x006b8fa0 _$E114
// FUNCTION: STBC 0x006b8fb0 _$E316
// FUNCTION: STBC 0x006b8fc0 _$E315
// FUNCTION: STBC 0x006b8fd0 _$E120
// FUNCTION: STBC 0x006b8fe0 _$E119
// FUNCTION: STBC 0x006b8ff0 _$E330
// FUNCTION: STBC 0x006b9000 _$E329
// FUNCTION: STBC 0x006b9010 _$E334
// FUNCTION: STBC 0x006b9020 _$E333
// FUNCTION: STBC 0x006b9030 _$E338
// FUNCTION: STBC 0x006b9040 _$E337
// FUNCTION: STBC 0x006b9050 _$E342
// FUNCTION: STBC 0x006b9060 _$E341
// FUNCTION: STBC 0x006b9070 _$E129
// FUNCTION: STBC 0x006b9080 _$E128
// FUNCTION: STBC 0x006b9090 _$E346
// FUNCTION: STBC 0x006b90a0 _$E345
// FUNCTION: STBC 0x006b90b0 _$E350
// FUNCTION: STBC 0x006b90c0 _$E349
// FUNCTION: STBC 0x006b90d0 _$E354
// FUNCTION: STBC 0x006b90e0 _$E353
// FUNCTION: STBC 0x006b90f0 _$E135
// FUNCTION: STBC 0x006b9100 _$E134
// FUNCTION: STBC 0x006b9110 _$E140
// FUNCTION: STBC 0x006b9120 _$E139
// FUNCTION: STBC 0x006b9130 _$E170
// FUNCTION: STBC 0x006b9140 _$E169
// FUNCTION: STBC 0x006b9150 _$E358
// FUNCTION: STBC 0x006b9160 _$E357
// FUNCTION: STBC 0x006b9170 _$E175
// FUNCTION: STBC 0x006b9180 _$E174
// FUNCTION: STBC 0x006b9190 _$E362
// FUNCTION: STBC 0x006b91a0 _$E361
// FUNCTION: STBC 0x006b91b0 _$E366
// FUNCTION: STBC 0x006b91c0 _$E365
// FUNCTION: STBC 0x006b91d0 _$E180
// FUNCTION: STBC 0x006b91e0 _$E179
// FUNCTION: STBC 0x006b91f0 _$E370
// FUNCTION: STBC 0x006b9200 _$E369
// FUNCTION: STBC 0x006b9210 _$E145
// FUNCTION: STBC 0x006b9220 _$E144
// FUNCTION: STBC 0x006b9230 _$E374
// FUNCTION: STBC 0x006b9240 _$E373
// FUNCTION: STBC 0x006b9250 _$E378
// FUNCTION: STBC 0x006b9260 _$E377
// FUNCTION: STBC 0x006b9270 _$E185
// FUNCTION: STBC 0x006b9280 _$E184
// FUNCTION: STBC 0x006b9290 _$E190
// FUNCTION: STBC 0x006b92a0 _$E189
// FUNCTION: STBC 0x006b92b0 _$E382
// FUNCTION: STBC 0x006b92c0 _$E381
// FUNCTION: STBC 0x006b92d0 _$E386
// FUNCTION: STBC 0x006b92e0 _$E385
// FUNCTION: STBC 0x006b92f0 _$E390
// FUNCTION: STBC 0x006b9300 _$E389
// FUNCTION: STBC 0x006b9310 _$E394
// FUNCTION: STBC 0x006b9320 _$E393
// FUNCTION: STBC 0x006b9330 _$E398
// FUNCTION: STBC 0x006b9340 _$E397
// FUNCTION: STBC 0x006b9350 _$E402
// FUNCTION: STBC 0x006b9360 _$E401
// FUNCTION: STBC 0x006b9370 _$E406
// FUNCTION: STBC 0x006b9380 _$E405
// FUNCTION: STBC 0x006b9390 _$E410
// FUNCTION: STBC 0x006b93a0 _$E409
// FUNCTION: STBC 0x006b93b0 _$E195
// FUNCTION: STBC 0x006b93c0 _$E194
// FUNCTION: STBC 0x006b93d0 _$E414
// FUNCTION: STBC 0x006b93e0 _$E413
// FUNCTION: STBC 0x006b93f0 _$E200
// FUNCTION: STBC 0x006b9400 _$E199
// FUNCTION: STBC 0x006b9410 _$E205
// FUNCTION: STBC 0x006b9420 _$E204

// Items per block of BreakUpMessage's list of parts.
#define MESSAGE_PARTS_BLOCK		128

// The messageStream quad (hi, constructor, atexit thunk, guarded exit).
// FUNCTION: STBC 0x006b8240 _$E420
// FUNCTION: STBC 0x006b8250 _$E416
// FUNCTION: STBC 0x006b8260 _$E419
// FUNCTION: STBC 0x006b8270 _$E417

// DATA: STBC 0x00996810 ?messageStream@TGMessage@@1VTGBufferStream@@A
TGBufferStream TGMessage::messageStream;

/*
================
TGMessage::RegisterMessageType
================
*/
// FUNCTION: STBC 0x006b8290
void TGMessage::RegisterMessageType( void ) {
	TGNetwork::RegisterMessageType( TGMESSAGE_TYPE, UnSerialize );
}

/*
================
TGMessage::TGMessage
================
*/
// FUNCTION: STBC 0x006b82a0 ??0TGMessage@@QAE@XZ
TGMessage::TGMessage( void ) {
	aggregate = true;
	data = NULL;
	dataLength = 0;
	guaranteed = false;
	highPriority = false;
	sequenceNumber = 0;
	numRetries = 0;
	backoffTime = 0.0f;
	timeStamp = 0.0f;
	firstSendTime = 0.0f;
	fromID = 0;
	fromAddress = 0;
	multiPart = false;
	multiPartCount = 0;
	multiPartSequenceNumber = 0;
	backoffType = BACKOFF_LINEAR;
	backoffFactor = 1.0f;
	firstResendTime = 1.0f;
}

/*
================
TGMessage::~TGMessage
================
*/
// FUNCTION: STBC 0x006b8320
TGMessage::~TGMessage( void ) {
	if ( data != NULL ) {
		delete data;
	}
}

/*
================
TGMessage::Serialize

Writes the message into buffer; 0 when it doesn't fit.
================
*/
// FUNCTION: STBC 0x006b8340
int TGMessage::Serialize( char *buffer, int bufferSize ) {
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
	if ( multiPart ) {
		*buffer++ = multiPartSequenceNumber;
		size++;
		if ( multiPartSequenceNumber == 0 ) {
			*buffer++ = multiPartCount;
			size++;
		}
	}
	memcpy( buffer, data, dataLength );
	size += dataLength;

	unsigned short flags = size & MESSAGE_SIZE_MASK;
	if ( guaranteed ) {
		flags |= MESSAGE_GUARANTEED;
	}
	if ( highPriority ) {
		flags |= MESSAGE_HIGH_PRIORITY;
	}
	if ( multiPart ) {
		flags |= MESSAGE_MULTI_PART;
	}
	*header = flags;
	return size;
}

/*
================
TGMessage::UnSerialize

Reads a message Serialize wrote.
================
*/
// FUNCTION: STBC 0x006b83f0
// PARKED: body and control flow match; VC6 allocates the parsed fields to other registers and spills, no source form found yet
TGMessage *TGMessage::UnSerialize( char *buffer ) {
	TGMessage *message = new TGMessage;
	unsigned short sequenceNumber = 0;

	buffer++;
	unsigned short flags = *(unsigned short *)buffer;
	buffer += sizeof( unsigned short );
	bool guaranteed = ( flags >> 15 ) & 1;
	bool highPriority = ( flags >> 14 ) & 1;
	bool multiPart = ( flags >> 13 ) & 1;
	int length = ( flags & MESSAGE_SIZE_MASK ) - MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		sequenceNumber = *(unsigned short *)buffer;
		buffer += sizeof( unsigned short );
		length -= sizeof( unsigned short );
	}

	unsigned char multiPartSequenceNumber = 0;
	unsigned char multiPartCount = 0;
	if ( multiPart ) {
		multiPartSequenceNumber = *buffer++;
		length--;
		if ( multiPartSequenceNumber == 0 ) {
			multiPartCount = *buffer++;
			length--;
		}
	}

	message->guaranteed = guaranteed;
	message->sequenceNumber = sequenceNumber;
	message->highPriority = highPriority;
	message->SetData( buffer, length );
	message->multiPartCount = multiPartCount;
	message->multiPart = multiPart;
	message->multiPartSequenceNumber = multiPartSequenceNumber;
	return message;
}

/*
================
TGMessage::SetData

Takes a copy of length bytes of data.
================
*/
// FUNCTION: STBC 0x006b84d0
void TGMessage::SetData( char *data, int length ) {
	if ( this->data != NULL ) {
		delete this->data;
		this->data = NULL;
	}
	if ( length != 0 ) {
		this->data = new char[length];
		memcpy( this->data, data, length );
	}
	dataLength = length;
}

/*
================
TGMessage::GetData
================
*/
// FUNCTION: STBC 0x006b8530
char *TGMessage::GetData( int *length ) {
	if ( length != NULL ) {
		*length = dataLength;
	}
	return data;
}

/*
================
TGMessage::TGMessage

Copies everything but fromAddress.
================
*/
// FUNCTION: STBC 0x006b8550 ??0TGMessage@@QAE@ABV0@@Z
TGMessage::TGMessage( const TGMessage &message ) {
	if ( message.data != NULL ) {
		dataLength = message.dataLength;
		data = new char[dataLength];
		memcpy( data, message.data, dataLength );
	} else {
		data = NULL;
		dataLength = message.dataLength;
	}
	guaranteed = message.guaranteed;
	highPriority = message.highPriority;
	fromID = message.fromID;
	aggregate = message.aggregate;
	sequenceNumber = message.sequenceNumber;
	numRetries = message.numRetries;
	backoffTime = message.backoffTime;
	timeStamp = message.timeStamp;
	firstSendTime = message.firstSendTime;
	toID = message.toID;
	multiPart = message.multiPart;
	multiPartCount = message.multiPartCount;
	multiPartSequenceNumber = message.multiPartSequenceNumber;
	backoffType = message.backoffType;
	backoffFactor = message.backoffFactor;
	firstResendTime = message.firstResendTime;
}

/*
================
TGMessage::Copy
================
*/
// FUNCTION: STBC 0x006b8610
TGMessage *TGMessage::Copy( void ) {
	return new TGMessage( *this );
}

/*
================
TGMessage::GetBufferSpaceRequired

The size Serialize writes.
================
*/
// FUNCTION: STBC 0x006b8640
int TGMessage::GetBufferSpaceRequired( void ) {
	int size = dataLength + MESSAGE_HEADER_SIZE;
	if ( guaranteed ) {
		size += sizeof( unsigned short );
	}
	if ( multiPart ) {
		size++;
		if ( multiPartSequenceNumber == 0 ) {
			size++;
		}
	}
	return size;
}

/*
================
TGMessage::SetNumRetries

Sets the retry count and the delay before the next resend.
================
*/
// FUNCTION: STBC 0x006b8670
void TGMessage::SetNumRetries( int retries ) {
	numRetries = retries;
	if ( retries == 0 ) {
		backoffTime = firstResendTime;
		return;
	}
	switch ( backoffType ) {
	case BACKOFF_CONSTANT:
		if ( backoffTime == 0.0f ) {
			backoffTime = backoffFactor;
		}
		break;
	case BACKOFF_LINEAR:
		backoffTime += backoffFactor;
		if ( backoffTime > MAX_BACKOFF_TIME ) {
			backoffTime = backoffFactor;
		}
		break;
	case BACKOFF_EXPONENTIAL: {
		float delay = backoffTime - firstResendTime;
		if ( delay == 0.0f ) {
			backoffTime = backoffFactor + firstResendTime;
		} else {
			backoffTime = delay + delay + firstResendTime;
		}
		break;
	}
	}
}

/*
================
TGMessage::ReadyToResend

True once the backoff delay has passed since the last send.
================
*/
// FUNCTION: STBC 0x006b8700
bool TGMessage::ReadyToResend( void ) {
	if ( g_kSystemWrapper.GetTimeInSeconds() > timeStamp + backoffTime ) {
		return true;
	}
	return false;
}

/*
================
TGMessage::BreakUpMessage

Splits a message too big for maxSize into guaranteed multi-part
messages: this message carries the first part, copies of it the others.
Returns an array of the parts (new[]) and their number in count; a
message that fits is returned alone.
================
*/
// FUNCTION: STBC 0x006b8720
// PARKED: same calls and control flow; retail swaps esi/edi for count and maxSize and copies the part count through al before storing it (register residue)
TGMessage **TGMessage::BreakUpMessage( int *count, int maxSize ) {
	NiTList<TGMessage *> parts( MESSAGE_PARTS_BLOCK );
	TGMessage **messages;

	if ( GetBufferSpaceRequired() > maxSize ) {
		*count = 0;
		if ( !IsGuaranteed() ) {
			SetGuaranteed( true );
		}

		int remaining = dataLength;
		char *wholeData = data;
		char *partData = data;
		data = NULL;
		dataLength = 0;

		int part = 0;
		while ( remaining > 0 ) {
			TGMessage *message;
			if ( part > 0 ) {
				message = Copy();
			} else {
				message = this;
			}
			message->SetMultiPart( true );
			message->SetMultiPartSequenceNumber( part );

			int partSize = maxSize - message->GetBufferSpaceRequired();
			if ( remaining > partSize ) {
				// the first part's data is set below, once the copies are made
				if ( part > 0 ) {
					message->SetData( partData, partSize );
				}
				remaining -= partSize;
				partData += partSize;
			} else {
				message->SetData( partData, remaining );
				remaining = 0;
			}

			parts.AddTail( message );
			part++;
		}

		SetData( wholeData, maxSize - GetBufferSpaceRequired() );
		*count = part;
		parts.GetHead()->SetMultiPartCount( part );
		delete wholeData;

		messages = new TGMessage *[*count];
		for ( int i = 0; i < *count; i++ ) {
			messages[i] = parts.RemoveHead();
		}
	} else {
		*count = 1;
		messages = new TGMessage *[1];
		messages[0] = this;
	}
	return messages;
}

/*
================
TGMessage::SetDataNoCopy

Takes data itself.
================
*/
// FUNCTION: STBC 0x006b89a0
void TGMessage::SetDataNoCopy( char *data, int length ) {
	if ( this->data != NULL ) {
		delete this->data;
		this->data = NULL;
	}
	this->data = data;
	dataLength = length;
}

/*
================
TGMessage::GetBufferStream

A stream over the message's data, shared by all messages.
================
*/
// FUNCTION: STBC 0x006b89e0
TGBufferStream *TGMessage::GetBufferStream( void ) {
	messageStream.VirtualSlot60( data, dataLength );
	return &messageStream;
}

/*
================
TGMessage::SetDataFromStream

Takes a copy of what was written to stream.
================
*/
// FUNCTION: STBC 0x006b8a00
void TGMessage::SetDataFromStream( TGBufferStream *stream ) {
	char *buffer = stream->GetBuffer();
	SetData( buffer, stream->GetPos() );
}
