/*
===============================================================================

	TGMessage.h

	A network message: its data and the header fields the network layer
	sends with it. The member names are those of the SWIG accessors
	TGMessage_Get*, Set* and Is*, whose inline bodies read and write these
	offsets.

	On the wire a message is a type byte, a 16-bit header (the total size in
	the low 13 bits, then the multi-part, high-priority and guaranteed
	flags), the sequence number of a guaranteed message, the multi-part
	position (and, in the first part, the part count), then the data.

	0x40 bytes, allocated from its own pool (TGPOOL_ALLOCATION).

===============================================================================
*/

#ifndef __TGMESSAGE_H__
#define __TGMESSAGE_H__

#include "TGPoolManager.h"

class TGBufferStream;

// How the resend delay grows with each retry; the names are invented.
enum tgBackoffType_t {
	BACKOFF_CONSTANT	= 0,	// the factor, once the first resend has passed
	BACKOFF_LINEAR		= 1,	// grows by the factor, back to it past MAX_BACKOFF_TIME
	BACKOFF_EXPONENTIAL	= 2		// doubles, starting from the factor
};

// Where a linear backoff starts over.
#define MAX_BACKOFF_TIME		5.0f

// The header's size field and flags.
#define MESSAGE_SIZE_MASK		0x1FFF
#define MESSAGE_MULTI_PART		0x2000
#define MESSAGE_HIGH_PRIORITY	0x4000
#define MESSAGE_GUARANTEED		0x8000

// A plain message's type byte.
#define TGMESSAGE_TYPE			50

// The type byte and the 16-bit header.
#define MESSAGE_HEADER_SIZE		3

// FUNCTION: STBC 0x006b82f0 ??_GTGMessage@@UAEPAXI@Z @game/TGMessage
class TGMessage {
public:
	TGPOOL_ALLOCATION

					TGMessage( void );
					TGMessage( const TGMessage &message );

	virtual int		VirtualSlot00( void );
	virtual			~TGMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual bool	Merge( TGMessage *message );
	virtual bool	OverrideOldPackets( void );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );
	virtual TGMessage **	BreakUpMessage( int *count, int maxSize );

	static TGMessage *	Create( void ) { return new TGMessage; }
	static TGMessage *	UnSerialize( char *buffer );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void		RegisterMessageType( void );

	void			SetData( char *data, int length );
	char *			GetData( int *length );
	void			SetDataNoCopy( char *data, int length );
	void			SetDataFromStream( TGBufferStream *stream );
	void			SetNumRetries( int retries );
	bool			ReadyToResend( void );
	TGBufferStream *	GetBufferStream( void );

	int				GetDataLength( void ) { return dataLength; }
	void			SetGuaranteed( bool guaranteed ) { this->guaranteed = guaranteed; }
	bool			IsGuaranteed( void ) { return guaranteed; }
	void			SetHighPriority( bool highPriority ) { this->highPriority = highPriority; }
	bool			IsHighPriority( void ) { return highPriority; }
	int				GetFromID( void ) { return fromID; }
	void			SetFromID( int id ) { fromID = id; }
	int				GetFromAddress( void ) { return fromAddress; }
	void			SetFromAddress( int address ) { fromAddress = address; }
	int				GetToID( void ) { return toID; }
	void			SetToID( int id ) { toID = id; }
	unsigned short	GetSequenceNumber( void ) { return sequenceNumber; }
	void			SetSequenceNumber( unsigned short number ) { sequenceNumber = number; }
	int				GetNumRetries( void ) { return numRetries; }
	void			IncrementNumRetries( void ) { SetNumRetries( numRetries + 1 ); }
	float			GetBackoffTime( void ) { return backoffTime; }
	void			SetBackoffTime( float time ) { backoffTime = time; }
	float			GetTimeStamp( void ) { return timeStamp; }
	void			SetTimeStamp( float time ) { timeStamp = time; }
	float			GetFirstSendTime( void ) { return firstSendTime; }
	void			SetFirstSendTime( float time ) { firstSendTime = time; }
	int				GetBackoffType( void ) { return backoffType; }
	void			SetBackoffType( int type ) { backoffType = type; }
	float			GetFirstResendTime( void ) { return firstResendTime; }
	void			SetFirstResendTime( float time ) { firstResendTime = time; }
	float			GetBackoffFactor( void ) { return backoffFactor; }
	void			SetBackoffFactor( float factor ) { backoffFactor = factor; }
	unsigned char	GetMultiPartCount( void ) { return multiPartCount; }
	void			SetMultiPartCount( unsigned char count ) { multiPartCount = count; }
	unsigned char	GetMultiPartSequenceNumber( void ) { return multiPartSequenceNumber; }
	void			SetMultiPartSequenceNumber( unsigned char number ) { multiPartSequenceNumber = number; }
	bool			IsMultiPart( void ) { return multiPart; }
	void			SetMultiPart( bool multiPart ) { this->multiPart = multiPart; }
	bool			IsAggregate( void ) { return aggregate; }
	void			SetAggregate( bool aggregate ) { this->aggregate = aggregate; }

protected:
	char *			data;						// 0x04
	int				dataLength;					// 0x08
	int				fromID;						// 0x0C
	int				fromAddress;				// 0x10
	unsigned short	sequenceNumber;				// 0x14
	int				numRetries;					// 0x18
	float			backoffTime;				// 0x1C
	float			timeStamp;					// 0x20
	float			firstSendTime;				// 0x24
	int				toID;						// 0x28
	int				backoffType;				// 0x2C a tgBackoffType_t
	float			firstResendTime;			// 0x30
	float			backoffFactor;				// 0x34
	unsigned char	multiPartCount;				// 0x38
	unsigned char	multiPartSequenceNumber;	// 0x39 0 in a multi-part message's first part
	bool			guaranteed;					// 0x3A
	bool			highPriority;				// 0x3B
	bool			multiPart;					// 0x3C
	bool			aggregate;					// 0x3D

	// The stream GetBufferStream returns, over the asking message's data;
	// a class static: its exit destructor is guarded (invented name).
	static TGBufferStream	messageStream;
};

/*
================
TGMessage::VirtualSlot00

The message's type byte.
================
*/
// FUNCTION: STBC 0x006b9430
inline int TGMessage::VirtualSlot00( void ) {
	return TGMESSAGE_TYPE;
}

/*
================
TGMessage::Merge

A plain message merges with nothing.
================
*/
// FUNCTION: STBC 0x006b9440
inline bool TGMessage::Merge( TGMessage *message ) {
	return false;
}

/*
================
TGMessage::OverrideOldPackets
================
*/
// FUNCTION: STBC 0x006b9450
inline bool TGMessage::OverrideOldPackets( void ) {
	return false;
}

#endif /* !__TGMESSAGE_H__ */
